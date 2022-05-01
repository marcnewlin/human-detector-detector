/**
  **************************************************************************
  * @file     telnet.c
  * @version  v2.0.8
  * @date     2022-04-02
  * @brief    telnet implement header file
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

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __HELLOWERLOD_H
#define __HELLOWERLOD_H

#ifdef __cplusplus
 extern "C" {
#endif

#define TELNET_IP_ADDR_0                 (192)
#define TELNET_IP_ADDR_1                 (168)
#define TELNET_IP_ADDR_2                 (81)
#define TELNET_IP_ADDR_3                 (37)

void helloworld_init(void);
void httpd_init(void);

#ifdef __cplusplus
}
#endif

#endif /* __HELLOWERLOD_H */
