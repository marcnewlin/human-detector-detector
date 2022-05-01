/**
  **************************************************************************
  * @file     tcp_server.h
  * @version  v2.0.8
  * @date     2022-04-02
  * @brief    tcp server header
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

#ifndef _TCP_SERVER_H_
#define _TCP_SERVER_H_

#ifdef __cplusplus
 extern "C" {
#endif

/*TCP server port*/
#define TCP_LOCAL_PORT                   (1030)
#define QUIZ_MESSAGE                     "\r\nHello. 8+9 = ?\r\n"

void tcp_server_init(void);

#ifdef __cplusplus
}
#endif

#endif
