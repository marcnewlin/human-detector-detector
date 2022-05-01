/**
  **************************************************************************
  * @file     tmr.h
  * @version  v2.0.8
  * @date     2022-04-02
  * @brief    tmr header file
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

#ifndef __TMR_H
#define __TMR_H

#ifdef __cplusplus
extern "C" {
#endif

#include "at32f403a_407_board.h"

extern uint8_t time_ira_cnt;
extern uint8_t get_data_from_usart_flag;

/** @addtogroup UTILITIES_examples
  * @{
  */

/** @addtogroup USART_IAP_bootloader
  * @{
  */

/** @defgroup bootloader_exported_functions
  * @{
  */

void tmr_init(void);

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
