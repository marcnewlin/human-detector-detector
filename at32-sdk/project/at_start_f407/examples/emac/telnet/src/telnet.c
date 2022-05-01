/**
  **************************************************************************
  * @file     telnet.c
  * @version  v2.0.8
  * @date     2022-04-02
  * @brief    telnet implement
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

/* Includes ------------------------------------------------------------------*/
#include "telnet.h"
#include "lwip/tcp.h"
#include <string.h>
#include <stdio.h>

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define GREETING                         "Hello. What is your name?\r\n"
#define HELLO                            "Nice to meet you, "
#define MAX_NAME_SIZE                    (32)

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
struct name
{
  int length;
  char bytes[MAX_NAME_SIZE];
};

/* Private function prototypes -----------------------------------------------*/
static err_t helloworld_recv(void *arg, struct tcp_pcb *pcb, struct pbuf *p, err_t err);
static err_t helloworld_accept(void *arg, struct tcp_pcb *pcb, err_t err);
static void helloworld_conn_err(void *arg, err_t err);

/* Private functions ---------------------------------------------------------*/

/**
  * @brief  called when a data is received on the telnet connection
  * @param  arg: the user argument
  * @param  pcb: the tcp_pcb that has received the data
  * @param  p: the packet buffer
  * @param  err: the error value linked with the received data
  * @retval error value
  */
static err_t helloworld_recv(void *arg, struct tcp_pcb *pcb, struct pbuf *p, err_t err)
{
  struct pbuf *q;
  struct name *name = (struct name *)arg;
  int done;
  char *c;
  int i;


  /* We perform here any necessary processing on the pbuf */
  if (p != NULL)
  {
	/* We call this function to tell the LwIp that we have processed the data */
	/* This lets the stack advertise a larger window, so more data can be received*/
	tcp_recved(pcb, p->tot_len);

    /* Check the name if NULL, no data passed, return withh illegal argument error */
	if(!name)
    {
      pbuf_free(p);
      return ERR_ARG;
    }

    done = 0;
    for(q=p; q != NULL; q = q->next)
    {
      c = q->payload;
      for(i=0; i<q->len && !done; i++)
	  {
        done = ((c[i] == '\r') || (c[i] == '\n'));
        if(name->length < MAX_NAME_SIZE)
	    {
          name->bytes[name->length++] = c[i];
        }
      }
    }
    if(done)
    {
      if(name->bytes[name->length-2] != '\r' || name->bytes[name->length-1] != '\n')
	  {
        if((name->bytes[name->length-1] == '\r' || name->bytes[name->length-1] == '\n') && (name->length+1 <= MAX_NAME_SIZE))
	    {
	      name->length += 1;
        }
	    else if(name->length+2 <= MAX_NAME_SIZE)
	    {
          name->length += 2;
        }
	    else
	    {
          name->length = MAX_NAME_SIZE;
        }

        name->bytes[name->length-2] = '\r';
        name->bytes[name->length-1] = '\n';
      }
      tcp_write(pcb, HELLO, strlen(HELLO), 1);

      tcp_write(pcb, name->bytes, name->length, TCP_WRITE_FLAG_COPY);
      name->length = 0;

      /* for next prompt */
      tcp_write(pcb, GREETING, strlen(GREETING), 1);
    }

	/* End of processing, we free the pbuf */
    pbuf_free(p);
  }
  else if (err == ERR_OK)
  {
    /* When the pbuf is NULL and the err is ERR_OK, the remote end is closing the connection. */
    /* We free the allocated memory and we close the connection */
    mem_free(name);
    return tcp_close(pcb);
  }
  return ERR_OK;


}

/**
  * @brief  this function when the telnet connection is established
  * @param  arg: user supplied argument
  * @param  pcb: the tcp_pcb which accepted the connection
  * @param  err: error value
  * @retval error value
  */
static err_t helloworld_accept(void *arg, struct tcp_pcb *pcb, err_t err)
{

  /* Tell LwIP to associate this structure with this connection. */
  tcp_arg(pcb, mem_calloc(sizeof(struct name), 1));

  /* Configure LwIP to use our call back functions. */
  tcp_err(pcb, helloworld_conn_err);
  tcp_recv(pcb, helloworld_recv);

  /* Send out the first message */
  tcp_write(pcb, GREETING, strlen(GREETING), 1);
  return ERR_OK;
}

/**
  * @brief  initialize the hello application
  * @param  none
  * @retval none
  */
void helloworld_init(void)
{
  struct tcp_pcb *pcb;
  ip_addr_t src;

  IP4_ADDR(&src,TELNET_IP_ADDR_0,TELNET_IP_ADDR_1,TELNET_IP_ADDR_2,TELNET_IP_ADDR_3);   //assign a specific IP

  /* Create a new TCP control block  */
  pcb = tcp_new();

  /* Assign to the new pcb a local IP address and a port number */
  /* Using IP_ADDR_ANY allow the pcb to be used by any local interface */
  tcp_bind(pcb, &src, 23);

  /* Set the connection to the LISTEN state */
  pcb = tcp_listen(pcb);

  /* Specify the function to be called when a connection is established */
  tcp_accept(pcb, helloworld_accept);
}

/**
  * @brief  this function is called when an error occurs on the connection
  * @param  arg: user supplied argument
  * @parm   err: error value
  * @retval none
  */
static void helloworld_conn_err(void *arg, err_t err)
{
  struct name *name;
  name = (struct name *)arg;

  mem_free(name);
}
