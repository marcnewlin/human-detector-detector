/**
  ******************************************************************************
  * @file    tftpserver.c
  * @brief   basic tftp server implementation for IAP (only Write Req supported)
  ******************************************************************************
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
#include "tftpserver.h"
#include "flash.h"
#include "string.h"
#include "stdio.h"
#include "iap.h"

#ifdef USE_IAP_TFTP

/* Private variables ---------------------------------------------------------*/
static uint32_t flash_write_address;
static struct udp_pcb *udppcb;
static __IO uint32_t total_count=0;


/* Private function prototypes -----------------------------------------------*/

static void iap_wrq_recv_callback(void *_args, struct udp_pcb *upcb, struct pbuf *pkt_buf,
                        ip_addr_t *addr, u16_t port);

static int iap_tftp_process_write(struct udp_pcb *upcb, ip_addr_t *to, int to_port);

static void iap_tftp_recv_callback(void *arg, struct udp_pcb *Upcb, struct pbuf *pkt_buf,
                        ip_addr_t *addr, u16_t port);

static void iap_tftp_cleanup_wr(struct udp_pcb *upcb, tftp_connection_args *args);
static tftp_opcode iap_tftp_decode_op(char *buf);
static u16_t iap_tftp_extract_block(char *buf);
static void iap_tftp_set_opcode(char *buffer, tftp_opcode opcode);
static void iap_tftp_set_block(char* packet, u16_t block);
static err_t iap_tftp_send_ack_packet(struct udp_pcb *upcb, ip_addr_t *to, int to_port, int block);

/* Private functions ---------------------------------------------------------*/


/**
  * @brief  returns the tftp opcode
  * @param  buf: pointer on the tftp packet
  * @retval none
  */
static tftp_opcode iap_tftp_decode_op(char *buf)
{
  return (tftp_opcode)(buf[1]);
}

/**
  * @brief  extracts the block number
  * @param  buf: pointer on the tftp packet
  * @retval block number
  */
static u16_t iap_tftp_extract_block(char *buf)
{
  u16_t *b = (u16_t*)buf;
  return ntohs(b[1]);
}

/**
  * @brief  sets the tftp opcode
  * @param  buffer: pointer on the tftp packet
  * @param  opcode: tftp opcode
  * @retval none
  */
static void iap_tftp_set_opcode(char *buffer, tftp_opcode opcode)
{
  buffer[0] = 0;
  buffer[1] = (u8_t)opcode;
}

/**
  * @brief  sets the tftp block number
  * @param  packet: pointer on the tftp packet
  * @param  block: block number
  * @retval none
  */
static void iap_tftp_set_block(char* packet, u16_t block)
{
  u16_t *p = (u16_t *)packet;
  p[1] = htons(block);
}

/**
  * @brief  sends tftp ack packet
  * @param  upcb: pointer on udp_pcb structure
  * @param  to: pointer on the receive ip address structure
  * @param  to_port: receive port number
  * @param  block: block number
  * @retval err_t: error code
  */
static err_t iap_tftp_send_ack_packet(struct udp_pcb *upcb, ip_addr_t *to, int to_port, int block)
{
  err_t err;
  struct pbuf *pkt_buf; /* chain of pbuf's to be sent */

  /* create the maximum possible size packet that a tftp ack packet can be */
  char packet[TFTP_ACK_PKT_LEN];

  /* define the first two bytes of the packet */
  iap_tftp_set_opcode(packet, TFTP_ACK);

  /* specify the block number being ack'd.
   * if we are ack'ing a data pkt then the block number echoes that of the data pkt being ack'd (duh)
   * if we are ack'ing a wrq pkt then the block number is always 0
   * rrq packets are never sent ack pkts by the server, instead the server sends data pkts to the
   * host which are, obviously, used as the "acknowledgement".  this saves from having to sendtransferboth
   * an ack packet and a data packet for rrqs - see rfc1350 for more info.  */
  iap_tftp_set_block(packet, block);

  /* pbuf_transport - specifies the transport layer */
  pkt_buf = pbuf_alloc(PBUF_TRANSPORT, TFTP_ACK_PKT_LEN, PBUF_POOL);

  if (!pkt_buf)      /*if the packet pbuf == null exit and endtransfertransmission */
  {
    return ERR_MEM;
  }

  /* copy the original data buffer over to the packet buffer's payload */
  memcpy(pkt_buf->payload, packet, TFTP_ACK_PKT_LEN);

  /* sending packet by udp protocol */
  err = udp_sendto(upcb, pkt_buf, to, to_port);

  /* free the buffer pbuf */
  pbuf_free(pkt_buf);

  return err;
}

/**
  * @brief  processes data transfers after a tftp write request
  * @param  _args: used as pointer on tftp connection args
  * @param  upcb: pointer on udp_pcb structure
  * @param  pkt_buf: pointer on a pbuf stucture
  * @param  ip_addr: pointer on the receive ip_address structure
  * @param  port: receive port address
  * @retval none
  */
static void iap_wrq_recv_callback(void *_args, struct udp_pcb *upcb, struct pbuf *pkt_buf, ip_addr_t *addr, u16_t port)
{
  tftp_connection_args *args = (tftp_connection_args *)_args;
  uint32_t data_buffer[128];
  u16 count=0;

  if (pkt_buf->len != pkt_buf->tot_len)
  {
    return;
  }

  /* does this packet have any valid data to write? */
  if ((pkt_buf->len > TFTP_DATA_PKT_HDR_LEN) &&
      (iap_tftp_extract_block(pkt_buf->payload) == (args->block + 1)))
  {
    /* copy packet payload to data_buffer */
    pbuf_copy_partial(pkt_buf, data_buffer, pkt_buf->len - TFTP_DATA_PKT_HDR_LEN,
                      TFTP_DATA_PKT_HDR_LEN);

    total_count += pkt_buf->len - TFTP_DATA_PKT_HDR_LEN;

    count = (pkt_buf->len - TFTP_DATA_PKT_HDR_LEN)/4;
    if (((pkt_buf->len - TFTP_DATA_PKT_HDR_LEN)%4)!=0)
    count++;

    /* write received data in flash */
    flash_if_write(&flash_write_address, data_buffer ,count);

    /* update our block number to match the block number just received */
    args->block++;
    /* update total bytes  */
    (args->tot_bytes) += (pkt_buf->len - TFTP_DATA_PKT_HDR_LEN);

    /* this is a valid pkt but it has no data.  this would occur if the file being
       written is an exact multiple of 512 bytes.  in this case, the args->block
       value must still be updated, but we can skip everything else.    */
  }
  else if (iap_tftp_extract_block(pkt_buf->payload) == (args->block + 1))
  {
    /* update our block number to match the block number just received  */
    args->block++;
  }

  /* send the appropriate ack pkt*/
  iap_tftp_send_ack_packet(upcb, addr, port, args->block);

  /* if the last write returned less than the maximum tftp data pkt length,
   * then we've received the whole file and so we can quit (this is how tftp
   * signals the endtransferof a transfer!)
   */
  if (pkt_buf->len < TFTP_DATA_PKT_LEN_MAX)
  {
    iap_init();
    iap_tftp_cleanup_wr(upcb, args);
    pbuf_free(pkt_buf);
  }
  else
  {
    pbuf_free(pkt_buf);
    return;
  }
}


/**
  * @brief  processes tftp write request
  * @param  to: pointer on the receive ip address
  * @param  to_port: receive port number
  * @retval none
  */
static int iap_tftp_process_write(struct udp_pcb *upcb, ip_addr_t *to, int to_port)
{
  tftp_connection_args *args = NULL;
  /* this function is called from a callback,
  * therefore interrupts are disabled,
  * therefore we can use regular malloc   */
  args = mem_malloc(sizeof *args);
  if (!args)
  {
    iap_tftp_cleanup_wr(upcb, args);
    return 0;
  }

  args->op = TFTP_WRQ;
  args->to_ip.addr = to->addr;
  args->to_port = to_port;
  /* the block # used as a positive response to a wrq is _always_ 0!!! (see rfc1350)  */
  args->block = 0;
  args->tot_bytes = 0;

  /* set callback for receives on this udp pcb (protocol control block) */
  udp_recv(upcb, (udp_recv_fn)iap_wrq_recv_callback, args);

  total_count =0;

  /* init flash */
  flash_unlock();

  flash_write_address = APP_START_SECTOR_ADDR;
  /* initiate the write transaction by sending the first ack */
  iap_tftp_send_ack_packet(upcb, to, to_port, args->block);

  return 0;
}


/**
  * @brief  processes traffic received on udp port 69
  * @param  args: pointer on tftp_connection arguments
  * @param  upcb: pointer on udp_pcb structure
  * @param  pbuf: pointer on packet buffer
  * @param  addr: pointer on the receive ip address
  * @param  port: receive port number
  * @retval none
  */
static void iap_tftp_recv_callback(void *arg, struct udp_pcb *upcb, struct pbuf *pkt_buf,
                        ip_addr_t *addr, u16_t port)
{
  tftp_opcode op;
  struct udp_pcb *upcb_tftp_data;
  err_t err;

  /* create new udp pcb structure */
  upcb_tftp_data = udp_new();
  if (!upcb_tftp_data)
  {
    /* error creating pcb. out of memory  */
    return;
  }

  /* bind to port 0 to receive next available free port */
  /* note:  this is how tftp works.  there is a udp pcb for the standard port
  * 69 which al transactions begin communication on, however, _all_ subsequent
  * transactions for a given "stream" occur on another port  */
  err = udp_bind(upcb_tftp_data, IP_ADDR_ANY, 0);
  if (err != ERR_OK)
  {
    /* unable to bind to port */
    return;
  }

  op = iap_tftp_decode_op(pkt_buf->payload);
  if (op != TFTP_WRQ)
  {
    /* remove pcb */
    udp_remove(upcb_tftp_data);
  }
  else
  {
    /* start the tftp write mode*/
    iap_tftp_process_write(upcb_tftp_data, addr, port);
  }
  pbuf_free(pkt_buf);
}


/**
  * @brief  disconnect and close the connection
  * @param  upcb: pointer on udp_pcb structure
  * @param  args: pointer on tftp_connection arguments
  * @retval none
  */
static void iap_tftp_cleanup_wr(struct udp_pcb *upcb, tftp_connection_args *args)
{
  /* Free the tftp_connection_args structure */
  mem_free(args);

  /* Disconnect the udp_pcb */
  udp_disconnect(upcb);

  /* close the connection */
  udp_remove(upcb);

  /* reset the callback function */
  udp_recv(udppcb, (udp_recv_fn)iap_tftp_recv_callback, NULL);

}

/* Global functions ---------------------------------------------------------*/

/**
  * @brief  creates and initializes a udp pcb for tftp receive operation
  * @param  none
  * @retval none
  */
void iap_tftpd_init(void)
{
  err_t err;
  unsigned port = 69; /* 69 is the port used for tftp protocol initial transaction */

  /* create a new UDP PCB structure  */
  udppcb = udp_new();
  if (!udppcb)
  {
    /* error creating pcb. out of memory */
    return ;
  }

  /* bind this pcb to port 69 */
  err = udp_bind(udppcb, IP_ADDR_ANY, port);
  if (err == ERR_OK)
  {
    /* initialize receive callback function */
    udp_recv(udppcb, (udp_recv_fn)iap_tftp_recv_callback, NULL);
  }
}

#endif /* USE_IAP_TFTP */
