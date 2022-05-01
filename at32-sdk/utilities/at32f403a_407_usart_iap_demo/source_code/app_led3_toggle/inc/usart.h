/**
  **************************************************************************
  * @file     usart.h
  * @version  v2.0.8
  * @date     2022-04-02
  * @brief    usart header file
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

#ifndef __USART_H
#define __USART_H

#ifdef __cplusplus
extern "C" {
#endif

#include "at32f403a_407_board.h"

extern uint8_t iap_flag;

/** @addtogroup UTILITIES_examples
  * @{
  */

/** @addtogroup USART_IAP_app_led3_toggle
  * @{
  */

/** @defgroup app_led3_definition
  * @{
  */

#define IAP_REV_FLAG_NO     0
#define IAP_REV_FLAG_5A     1
#define IAP_REV_FLAG_DONE   2
#define IAP_REV_FLAG_ERROR  3

/**
  * @}
  */

/** @defgroup app_led3_exported_functions
  * @{
  */

void uart_init(uint32_t baudrate);
void back_ok(void);
void back_err(void);

/**
  * @}
  */

/**
  * @}
  */

/**
  * @}
  */

#ifdef __cplusplus
}
#endif

#endif


