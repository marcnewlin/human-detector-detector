/**
  **************************************************************************
  * @file     httpserver.h
  * @version  v2.0.8
  * @date     2022-04-02
  * @brief    httpserver header file
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

#ifndef __HTTPSERVER_H__
#define __HTTPSERVER_H__

#ifdef __cplusplus
 extern "C" {
#endif

#include "lwip/def.h"
#include "fsdata.h"

#define USERID                           "user"
#define PASSWORD                         "at32"
#define LOGIN_SIZE                       (15+ sizeof(USERID) + sizeof(PASSWORD))

struct fs_file {
  char *data;
  int len;
};

void iap_httpd_init(void);

#ifdef __cplusplus
}
#endif

#endif
