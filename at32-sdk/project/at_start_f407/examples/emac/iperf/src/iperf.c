/**
  **************************************************************************
  * @file     iperf.c
  * @version  v2.0.8
  * @date     2022-04-02
  * @brief    iperf tool
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

#include "iperf.h"

#include "lwip/opt.h"
#include "lwip/tcp.h"

/**
  * @brief  iperf receive callback
  * @param  arg: the user argument
  * @param  pcb: the tcp_pcb that has received the data
  * @param  p: the packet buffer
  * @param  err: the error value linked with the received data
  * @retval error value
  */
static err_t iperf_recv(void *arg, struct tcp_pcb *pcb, struct pbuf *p, err_t err)
{
  LWIP_UNUSED_ARG(arg);

  if (err == ERR_OK && p != NULL) {
    tcp_recved(pcb, p->tot_len);
    pbuf_free(p);
  } else
  if (err != ERR_OK && p != NULL) {
    pbuf_free(p);
  }

  if (err == ERR_OK && p == NULL) {
    tcp_arg(pcb, NULL);
    tcp_sent(pcb, NULL);
    tcp_recv(pcb, NULL);
    tcp_close(pcb);
  }
  return ERR_OK;
}

/**
  * @brief  accept iperf connection
  * @param  arg: user supplied argument
  * @param  pcb: the tcp_pcb which accepted the connection
  * @param  err: error value
  * @retval error value
  */
static err_t iperf_accept(void *arg, struct tcp_pcb *pcb, err_t err)
{
  LWIP_UNUSED_ARG(arg);
  LWIP_UNUSED_ARG(err);

  tcp_arg(pcb, NULL);
  tcp_sent(pcb, NULL);
  tcp_recv(pcb, iperf_recv);
  return ERR_OK;
}

/**
  * @brief  initialize iperf server
  * @param  none
  * @retval none
  */
void iperf_init(void)
{
  struct tcp_pcb *pcb;
  pcb = tcp_new();
  tcp_bind(pcb, IP_ADDR_ANY, 5001); // bind to iperf port
  pcb = tcp_listen(pcb);
  tcp_accept(pcb, iperf_accept);
}
