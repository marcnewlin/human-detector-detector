/**
  **************************************************************************
  * @file     httpserver.c
  * @version  v2.0.8
  * @date     2022-04-02
  * @brief    httpserver program
  **************************************************************************
  *                       Copyright notice & Disclaimer
  *
  * The software Board Support Package (BSP) that is made available to
  * download from Artery official website is the copyrighted work of Artery.
  * Artery authorizes customers to use, copy, and distribute the BSP
  * software and its related documentation for the purpose of design and
  * development in conjunction with Artery microcontrollers. Use of the
  * software is governed by this copyright notice and the following disclaimer.
  *
  * THIS SOFTWARE IS PROVIDED ON "AS IS" BASIS WITHOUT WARRANTIES,
  * GUARANTEES OR REPRESENTATIONS OF ANY KIND. ARTERY EXPRESSLY DISCLAIMS,
  * TO THE FULLEST EXTENT PERMITTED BY LAW, ALL EXPRESS, IMPLIED OR
  * STATUTORY OR OTHER WARRANTIES, GUARANTEES OR REPRESENTATIONS,
  * INCLUDING BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY,
  * FITNESS FOR A PARTICULAR PURPOSE, OR NON-INFRINGEMENT.
  *
  **************************************************************************
  */

#include "httpserver.h"
#include "lwip/tcp.h"
#include "fsdata.h"
#include "flash.h"
#include "iap.h"
#include <string.h>
#include <stdio.h>

#ifdef USE_IAP_HTTP

static vu32 dataflag = 0;
static vu32 size = 0;
static __IO uint32_t flashwriteaddress;
static u32 totalreceived = 0;
static char leftbytestab[4];
static u8 leftbytes = 0;
static __IO u8 resetpage = 0;
static uint32_t contentlengthoffset = 0, browserflag = 0;
static __IO uint32_t totaldata = 0, checklogin = 0;

struct http_state
{
  char *file;
  u32_t left;
};

typedef enum
{
  LOGINPAGE = 0,
  FILEUPLOADPAGE,
  UPLOADDONEPAGE,
  RESETDONEPAGE
}htmlpageState;

htmlpageState htmlpage;

static const char http_crnl_2[4] =
/* "\r\n--" */
{0xd, 0xa,0x2d,0x2d};
static const char octet_stream[14] =
/* "octet-stream" */
{0x6f, 0x63, 0x74, 0x65, 0x74, 0x2d, 0x73, 0x74, 0x72, 0x65, 0x61, 0x6d,0x0d, };
static const char content_length[17] =
/* Content Length */
{0x43, 0x6f, 0x6e, 0x74, 0x65, 0x6e, 0x74, 0x2d, 0x4c, 0x65, 0x6e, 0x67,0x74, 0x68, 0x3a, 0x20, };
/* Upgrade-Insecure-Requests: 1/r/n */
static const char upgrade_insecure_request[30] =
/* Content Length */
{0x55, 0x70, 0x67, 0x72, 0x61, 0x64, 0x65, 0x2d, 0x49, 0x6e, 0x73, 0x65, 0x63, 0x75, 0x72, 0x65, 0x2d, 0x52, 0x65, 0x71, 0x75, 0x65, 0x73, 0x74, 0x73, 0x3a, 0x20, 0x31, 0x0d, 0x0a,};


static uint32_t parse_content_length(char *data, uint32_t len);
static void iap_http_writedata(char* data, uint32_t len);

/* file must be allocated by caller and will be filled in
   by the function. */
static int fs_open(char *name, struct fs_file *file);

/**
  * @brief  callback function for handling connection errors
  * @param  arg: pointer to an argument to be passed to callback function
  * @param  err: lwip error code
  * @retval none
  */
static void conn_err(void *arg, err_t err)
{
  struct http_state *hs;

  hs = arg;
  mem_free(hs);
}

/**
  * @brief  closes tcp connection
  * @param  pcb: pointer to a tcp_pcb struct
  * @param  hs: pointer to a http_state struct
  * @retval
  */
static void close_conn(struct tcp_pcb *pcb, struct http_state *hs)
{
  tcp_arg(pcb, NULL);
  tcp_sent(pcb, NULL);
  tcp_recv(pcb, NULL);
  mem_free(hs);
  tcp_close(pcb);
}

/**
  * @brief  sends data found in  member "file" of a http_state struct
  * @param  pcb: pointer to a tcp_pcb struct
  * @param  hs: pointer to a http_state struct
  * @retval none
  */
static void send_data(struct tcp_pcb *pcb, struct http_state *hs)
{
  err_t err;
  u16_t len;

  /* we cannot send more data than space available in the send buffer */
  if (tcp_sndbuf(pcb) < hs->left)
  {
    len = tcp_sndbuf(pcb);
  }
  else
  {
    len = hs->left;
  }
  err = tcp_write(pcb, hs->file, len, 0);
  if (err == ERR_OK)
  {
    hs->file += len;
    hs->left -= len;
  }
}

/**
  * @brief tcp poll callback function
  * @param arg: pointer to an argument to be passed to callback function
  * @param pcb: pointer on tcp_pcb structure
  * @retval err_t
  */
static err_t http_poll(void *arg, struct tcp_pcb *pcb)
{
  if (arg == NULL)
  {
    tcp_close(pcb);
  }
  else
  {
    send_data(pcb, (struct http_state *)arg);
  }
  return ERR_OK;
}

/**
  * @brief  callback function called after a successfull tcp data packet transmission
  * @param  arg: pointer to an argument to be passed to callback function
  * @param  pcb: pointer on tcp_pcb structure
  * @param  len
  * @retval err : lwip error code
  */
static err_t http_sent(void *arg, struct tcp_pcb *pcb, u16_t len)
{
  struct http_state *hs;

  hs = arg;

  if (hs->left > 0)
  {
    send_data(pcb, hs);
  }
  else
  {
    close_conn(pcb, hs);
    if(resetpage ==1)
    {
      iap_init();
      /* Generate a software reset */
      NVIC_SystemReset();
      while(1);
    }

  }
  return ERR_OK;
}

/**
  * @brief  callback function for handling tcp http traffic
  * @param  arg: pointer to an argument structure to be passed to callback function
  * @param  pcb: pointer to a tcp_pcb structure
  * @param  p: pointer to a packet buffer
  * @param  err: lwip error code
  * @retval err
  */
static err_t http_recv(void *arg, struct tcp_pcb *pcb,  struct pbuf *p, err_t err)
{
  int32_t i, len=0;
  uint32_t dataoffset, filenameoffset, contentoffsetfirefox;
  char *data, *ptr, login[LOGIN_SIZE];
  volatile char filename[13];
  struct fs_file file = {0, 0};
  struct http_state *hs;

  hs = arg;

  if (err == ERR_OK && p != NULL)
  {
    /* inform tcp that we have taken the data */
    tcp_recved(pcb, p->tot_len);

    if (hs->file == NULL)
    {
      data = p->payload;
      len = p->tot_len;

      /* process http get requests */
      if (strncmp(data, "GET /", 5) == 0)
      {
        if ((strncmp(data, "GET /resetmcu.cgi", 17) ==0)&&(htmlpage == UPLOADDONEPAGE))
        {
          htmlpage = RESETDONEPAGE;
          fs_open("/reset.html", &file);
          hs->file = file.data;
          hs->left = file.len;
          pbuf_free(p);

          /* send reset.html page */
          send_data(pcb, hs);
          resetpage = 1;

          /* tell tcp that we wish be to informed of data that has been
          successfully sent by a call to the http_sent() function. */
          tcp_sent(pcb, http_sent);
        }
        else if ((strncmp(data, "GET /favicon.ico", 16) ==0))
        {
          /*send the login page (which is the index page) */
          //htmlpage = LOGINPAGE;
          fs_open("/index.html", &file);
          hs->file = file.data;
          hs->left = file.len;
          pbuf_free(p);

          /* send index.html page */
          send_data(pcb, hs);

          /* tell tcp that we wish be to informed of data that has been
          successfully sent by a call to the http_sent() function. */
          tcp_sent(pcb, http_sent);
        }
        else
        {
          /*send the login page (which is the index page) */
          htmlpage = LOGINPAGE;
          fs_open("/index.html", &file);
          hs->file = file.data;
          hs->left = file.len;
          pbuf_free(p);

          /* send index.html page */
          send_data(pcb, hs);

          /* tell tcp that we wish be to informed of data that has been
          successfully sent by a call to the http_sent() function. */
          tcp_sent(pcb, http_sent);
        }
      }

      /* process post request for checking login */
      else if ((strncmp(data, "POST /checklogin.cgi",20)==0)&&(htmlpage== LOGINPAGE))
      {
          /* parse packet for the username & password */
          for (i=0;i<len;i++)
          {
            if (strncmp ((char*)(data+i), "username=", 9)==0)
            {
              sprintf(login,"username=%s&password=%s",USERID,PASSWORD);
              if (strncmp((char*)(data+i), login ,LOGIN_SIZE)==0)
              {
                htmlpage = FILEUPLOADPAGE;
                fs_open("/upload.html", &file);

              }
              else
              {
                htmlpage = LOGINPAGE;
                /* reload index.html */
                fs_open("/index.html", &file);
              }
              hs->file = file.data;
              hs->left = file.len;

              pbuf_free(p);

              /* send index.html page */
              send_data(pcb, hs);

              /* tell tcp that we wish be to informed of data that has been
                 successfully sent by a call to the http_sent() function. */
              tcp_sent(pcb, http_sent);
              break;
            }
          }
      }

      /* process post request for file upload and incoming data packets after post request*/
      else if (((strncmp(data, "POST /upload.cgi",16)==0)||(dataflag >=1))&&(htmlpage == FILEUPLOADPAGE))
      {
        dataoffset =0;
        contentoffsetfirefox = 0;

        /* post packet received */
        if (dataflag ==0)
        {
          browserflag=0;
          totalreceived =0;

          /* parse packet for Content-length field */
          size = parse_content_length(data, p->tot_len);

          /* parse packet for the octet-stream field */
          for (i=0;i<len;i++)
          {
            if (strncmp ((char*)(data+i), octet_stream, 13)==0)
            {
              dataoffset = i+16;
              break;
            }
          }
          /* case of msie8 : we do not receive data in the post packet*/
          if (dataoffset==0)
          {
            dataflag++;
            browserflag = 1;
            pbuf_free(p);
            return ERR_OK;
          }
          /* case of mozilla firefox v79.0 : we receive data in the post packet*/
          else
          {
            /* parse packet for the upgrade-insecure-request field */
            for(i = 0; i < len; i++)
            {
              if (strncmp ((char*)(data+i), upgrade_insecure_request, 30)==0)
              {
                contentoffsetfirefox = i + 32;
                break;
              }
            }
            totalreceived = len - contentoffsetfirefox;
          }
        }

        if (((dataflag ==1)&&(browserflag==1)) || ((dataflag ==0)&&(browserflag==0)))
        {
          if ((dataflag ==0)&&(browserflag==0))
          {
            dataflag++;
          }
          else if ((dataflag ==1)&&(browserflag==1))
          {
             /* parse packet for the octet-stream field */
            for (i=0;i<len;i++)
            {
              if (strncmp ((char*)(data+i), octet_stream, 13)==0)
              {
                dataoffset = i+16;
                break;
              }
            }
            totalreceived+=len;
            dataflag++;
          }

          /* parse packet for the filename field */
          filenameoffset = 0;
          for (i=0;i<len;i++)
          {
            if (strncmp ((char*)(data+i), "filename=", 9)==0)
            {
              filenameoffset = i+10;
              break;
            }
          }
          i =0;
          if (filenameoffset)
          {
            while((*(data+filenameoffset + i) != 0x22 )&&(i<13))
            {
              filename[i] = *(data+filenameoffset + i);
              i++;
            }
            filename[i] = 0x0;
          }

          if (i==0)
          {
            htmlpage = FILEUPLOADPAGE;
            /* no filename, in this case reload upload page */
            fs_open("/upload.html", &file);
            hs->file = file.data;
            hs->left = file.len;
            pbuf_free(p);

            /* send index.html page */
            send_data(pcb, hs);

            /* tell tcp that we wish be to informed of data that has been
               successfully sent by a call to the http_sent() function. */
            tcp_sent(pcb, http_sent);
            dataflag=0;
            return ERR_OK;

          }
          totaldata =0 ;

          flashwriteaddress = APP_START_SECTOR_ADDR;
        }
         /* dataflag >1 => the packet is data only  */
        else
        {
          totalreceived +=len;
        }

        ptr = (char*)(data + dataoffset);
        len-= dataoffset;

        /* update total data received counter */
        totaldata +=len;
        /* check if last data packet */
        if (totalreceived == size)
        {
          /* if last packet need to remove the http boundary tag */
          /* parse packet for "\r\n--" starting from end of data */
          i = 4;
          while (strncmp ((char*)(data+ p->tot_len -i),http_crnl_2 , 4))
          {
            i++;
          }
          len -= i;
          totaldata -= i;

          /* write data in flash */
          if (len)
            iap_http_writedata(ptr,len);

          dataflag=0;
          htmlpage = UPLOADDONEPAGE;
          /* send uploaddone.html page */
          fs_open("/uploaddone.html", &file);
          hs->file = file.data;
          hs->left = file.len;
          send_data(pcb, hs);

          /* tell tcp that we wish be to informed of data that has been
             successfully sent by a call to the http_sent() function. */
          tcp_sent(pcb, http_sent);
        }
        /* not last data packet */
        else
        {
          /* write data in flash */
          if(len)
            iap_http_writedata(ptr,len);
        }
        pbuf_free(p);
      }
      else
      {
        /* bad http requests */
        pbuf_free(p);
        close_conn(pcb, hs);
      }
    }
    else
    {
      pbuf_free(p);
      close_conn(pcb,hs);
    }
  }
  if (err == ERR_OK && p == NULL)
  {
    /* received empty frame */
    close_conn(pcb, hs);
  }
  return ERR_OK;
}

/**
  * @brief  callback function on tcp connection setup ( on port 80)
  * @param  arg: pointer to an argument structure to be passed to callback function
  * @param  pcb: pointer to a tcp_pcb structure
  * &param  err: lwip stack error code
  * @retval err
  */
static err_t http_accept(void *arg, struct tcp_pcb *pcb, err_t err)
{
  struct http_state *hs;

  /* allocate memory for the structure that holds the state of the connection */
  hs = mem_malloc(sizeof(struct http_state));

  if (hs == NULL)
  {
    return ERR_MEM;
  }

  /* initialize the structure. */
  hs->file = NULL;
  hs->left = 0;

  /* tell tcp that this is the structure we wish to be passed for our
     callbacks. */
  tcp_arg(pcb, hs);

  /* tell tcp that we wish to be informed of incoming data by a call
     to the http_recv() function. */
  tcp_recv(pcb, http_recv);

  tcp_err(pcb, conn_err);

  tcp_poll(pcb, http_poll, 10);
  return ERR_OK;
}

/**
  * @brief  intialize http webserver
  * @param  none
  * @retval none
  */
void iap_httpd_init(void)
{
  struct tcp_pcb *pcb;
  /*create new pcb*/
  pcb = tcp_new();
  /* bind http traffic to pcb */
  tcp_bind(pcb, IP_ADDR_ANY, 80);
  /* start listening on port 80 */
  pcb = tcp_listen(pcb);
  /* define callback function for tcp connection setup */
  tcp_accept(pcb, http_accept);
}

/**
  * @brief  opens a file defined in fsdata.c rom filesystem
  * @param  name : pointer to a file name
  * @param  file : pointer to a fs_file structure
  * @retval  1 if success, 0 if fail
  */
static int fs_open(char *name, struct fs_file *file)
{
  struct fsdata_file_noconst *f;

  for (f = (struct fsdata_file_noconst *)FS_ROOT; f != NULL; f = (struct fsdata_file_noconst *)f->next)
  {
    if (!strcmp(name, f->name))
    {
      file->data = f->data;
      file->len = f->len;
      return 1;
    }
  }
  return 0;
}

/**
  * @brief  extract the content_length data from html data
  * @param  data : pointer on receive packet buffer
  * @param  len  : buffer length
  * @retval size : content_length in numeric format
  */
static uint32_t parse_content_length(char *data, uint32_t len)
{
  uint32_t i=0,size=0, S=1;
  int32_t j=0;
  char sizestring[6], *ptr;

  contentlengthoffset =0;

  /* find content-length data in packet buffer */
  for (i = 0; i < len; i++)
  {
    if (strncmp ((char*)(data+i), content_length, 16)==0)
    {
      contentlengthoffset = i+16;
      break;
    }
  }
  /* read content-length value */
  if (contentlengthoffset)
  {
    i = 0;
    ptr = (char*)(data + contentlengthoffset);
    while(*(ptr+i)!=0x0d)
    {
      sizestring[i] = *(ptr+i);
      i++;
      contentlengthoffset++;
    }
    if (i>0)
    {
      /* transform string data into numeric format */
      for(j=i-1;j>=0;j--)
      {
        size += (sizestring[j]-0x30)*S;
        S=S*10;
      }
    }
  }
  return size;
}

/**
  * @brief  writes received data in flash
  * @param  ptr: data pointer
  * @param  len: data length
  * @retval none
  */
static void iap_http_writedata(char * ptr, uint32_t len)
{
  uint32_t count, i = 0, j = 0;

  /* check if any left bytes from previous packet transfer*/
  /* if it is the case do a concat with new data to create a 32-bit word */
  if (leftbytes)
  {
    while(leftbytes <= 3)
    {
      if(len > (j + 1))
      {
        leftbytestab[leftbytes++] = *(ptr+j);
      }
      else
      {
        leftbytestab[leftbytes++] = 0xFF;
      }
      j++;
    }
    flash_if_write(&flashwriteaddress, (u32*)(leftbytestab), 1);
    leftbytes = 0;

    /* update data pointer */
    ptr = (char*)(ptr + j);
    len = len - j;
  }

  /* write received bytes into flash */
  count = len / 4;

  /* check if remaining bytes < 4 */
  i= len % 4;
  if (i > 0)
  {
    if (totalreceived != size)
    {
      /* store bytes in leftbytestab */
      leftbytes = 0;
      for(; i > 0; i--)
      leftbytestab[leftbytes++] = *(char*)(ptr+ len-i);
    }
    else count++;
  }
  flash_if_write(&flashwriteaddress, (u32*)ptr ,count);
}
#endif
