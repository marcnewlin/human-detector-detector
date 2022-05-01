/**
  **************************************************************************
  * @file     tcp_client.c
  * @version  v2.0.8
  * @date     2022-04-02
  * @brief    implement tcp client
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

#include "tcp_client.h"
#include "lwip/tcp.h"

extern struct tcp_pcb *tcp_active_pcbs;

/* Gloable functions ---------------------------------------------------------*/
unsigned char connect_flag = 0;

/**
  * @brief  generate delay
  * @param  ulval: loop times
  * @retval none
  */
void Delay_s(unsigned long ulVal)
{
	while ( --ulVal != 0 );
}

/**
  * @brief  tcp client send data
  * @param  cpcb: the tcp_pcb that has sent the data
  * @param  buff: the packet buffer
  * @param  length: data size
  * @retval error value
  */
err_t tcp_client_send_data(struct tcp_pcb *cpcb,unsigned char *buff,unsigned int length)
{
	err_t err;

	err = tcp_write(cpcb,buff,length,TCP_WRITE_FLAG_COPY);	//send data
	tcp_output(cpcb);
	return err;
}

/**
  * @brief  check connection
  * @param  none
  * @retval the tcp_pcb which accepted the connection
  */
struct tcp_pcb *check_tcp_connect(void)
{
	struct tcp_pcb *cpcb = 0;
	connect_flag = 0;
	for(cpcb = tcp_active_pcbs;cpcb != NULL; cpcb = cpcb->next)
	{
		if(cpcb -> state == ESTABLISHED)
		{
			connect_flag = 1;
			break;
		}
	}

	if(connect_flag == 0)
	{
		tcp_client_init(TCP_LOCAL_PORT,TCP_SERVER_PORT,TCP_SERVER_IP);
		cpcb = 0;
	}
	return cpcb;
}

/**
  * @brief  report connection is establish
  * @param  arg: user supplied argument
  * @param  pcb: the tcp_pcb which accepted the connection
  * @param  err: error value
  * @retval err_t: it will response error code
  */
err_t tcp_connected(void *arg,struct tcp_pcb *pcb,err_t err)
{
	//tcp_client_pcb = pcb;
	return ERR_OK;
}

/**
  * @brief  receiving callback funtion for TCP client
  * @param  arg: the user argument
  * @param  pcb: the tcp_pcb that has received the data
  * @param  p: the packet buffer
  * @param  err: the error value linked with the received data
  * @retval error value
  */
err_t tcp_client_recv(void *arg, struct tcp_pcb *pcb,struct pbuf *p,err_t err)
{

	if(p != NULL)
	{
		tcp_recved(pcb, p->tot_len);
		tcp_write(pcb,p->payload,p->tot_len,TCP_WRITE_FLAG_COPY);
		tcp_output(pcb);
	}
	else
	{
		tcp_close(pcb);
	}
	pbuf_free(p);
	err = ERR_OK;
	return err;
}

/**
  * @brief  initialize tcp client
  * @param  local_port: local port
  * @param  remote_port: remote port
  * @param  a: ip byte 0
  * @param  b: ip byte 1
  * @param  c: ip byte 2
  * @param  d: ip byte 3
  * @retval none
  */
void tcp_client_init(uint16_t local_port,uint16_t remote_port,unsigned char a,unsigned char b,unsigned char c,unsigned char d)
{

	ip_addr_t ipaddr;
	struct tcp_pcb *tcp_client_pcb;
	err_t err;
	IP4_ADDR(&ipaddr,a,b,c,d);
	tcp_client_pcb = tcp_new();
	if (!tcp_client_pcb)
	{
		return ;
	}
	err = tcp_bind(tcp_client_pcb,IP_ADDR_ANY,local_port);
    if(err != ERR_OK)
	{
		return ;
	}
	tcp_connect(tcp_client_pcb, &ipaddr, remote_port, tcp_connected);
	tcp_recv(tcp_client_pcb, tcp_client_recv);
}
