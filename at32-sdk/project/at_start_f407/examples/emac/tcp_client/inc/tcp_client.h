/**
  **************************************************************************
  * @file     tcp_client.h
  * @version  v2.0.8
  * @date     2022-04-02
  * @brief    tcp client header
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

#include "stdint.h"
#include "lwip/err.h"

#ifndef  _TCP_CLIENT_H_
#define  _TCP_CLIENT_H_

#ifdef __cplusplus
 extern "C" {
#endif

extern struct tcp_pcb *tcp_client_pcb;

/* TCP server and client configuration*/
#define TCP_LOCAL_PORT     		           (1030)
#define TCP_SERVER_PORT    		           (1031)
#define TCP_SERVER_IP                    192,168,81,19

void tcp_client_init(uint16_t local_port,uint16_t remote_port,unsigned char a,unsigned char b,unsigned char c,unsigned char d);
err_t tcp_client_send_data(struct tcp_pcb *cpcb,unsigned char *buff,unsigned int length);
struct tcp_pcb *check_tcp_connect(void);

#ifdef __cplusplus
}
#endif

#endif

