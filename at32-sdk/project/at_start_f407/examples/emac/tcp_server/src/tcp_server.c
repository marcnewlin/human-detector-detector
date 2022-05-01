/**
  **************************************************************************
  * @file     tcp_server.c
  * @version  v2.0.8
  * @date     2022-04-02
  * @brief    implement tcp server
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

#include "tcp_server.h"
#include "lwip/err.h"
#include "lwip/tcp.h"
#include "string.h"

/**
  * @brief  receive and transmit tcp data
  * @param  arg: the user argument
  * @param  pcb: the tcp_pcb that has received the data
  * @param  p: the packet buffer
  * @param  err: the error value linked with the received data
  * @retval error value
  */
static err_t tcp_server_recv(void *arg, struct tcp_pcb *pcb,struct pbuf *p,err_t err)
{
  char* pPrintBuf;
	if(p != NULL)
	{
    pPrintBuf = (char*)malloc(p->tot_len * sizeof(char));
    memset(pPrintBuf, 0, p->tot_len * sizeof(char));
    strncpy(pPrintBuf, p->payload, p->tot_len);
		tcp_recved(pcb, p->tot_len);				                      /* Get data length; tot_len: length of tcp data block */
		tcp_write(pcb,p->payload,p->tot_len,TCP_WRITE_FLAG_COPY); /* payload is starting position of TCP data block */
		tcp_output(pcb);
    tcp_write(pcb, QUIZ_MESSAGE, strlen(QUIZ_MESSAGE), 1);
	}
	else
	{
		tcp_close(pcb); 											                    /* TCP server shouldn't close this session actively */
	}
	pbuf_free(p); 											                        /* free the TCP segment */
  free(pPrintBuf);
	err = ERR_OK;
	return err;
}

/**
  * @brief  callback function for receiving data
  * @param  arg: user supplied argument
  * @param  pcb: the tcp_pcb which accepted the connection
  * @param  err: error value
  * @retval error value
  */
static err_t tcp_server_accept(void *arg,struct tcp_pcb *pcb,err_t err)
{
	tcp_setprio(pcb, TCP_PRIO_MIN); 		                        /* set the priority of callback function, if there are multiple session exist, this function must be called */
	tcp_recv(pcb,tcp_server_recv); 			                        /* set callbacl function for TCP segments come in */

  tcp_write(pcb, QUIZ_MESSAGE, strlen(QUIZ_MESSAGE), 1);
	err = ERR_OK;
	return err;
}

/**
  * @brief  initialize tcp server
  * @param  none
  * @retval none
  */
void tcp_server_init(void)
{
	struct tcp_pcb *pcb;
	err_t err;
	/*****************************************************/
	pcb = tcp_new(); 								                            /* create TCP protocol control block for communication */
	if (!pcb)
	{
		return ;
	}
	err = tcp_bind(pcb,IP_ADDR_ANY,TCP_LOCAL_PORT); 	          /* bind local IP and port for TCP server */
	if(err != ERR_OK)
	{
		return ;
	}
	pcb = tcp_listen(pcb); 							                        /* entering listening status */
	tcp_accept(pcb,tcp_server_accept); 			                    /* set callback function for connection requestion */

}
