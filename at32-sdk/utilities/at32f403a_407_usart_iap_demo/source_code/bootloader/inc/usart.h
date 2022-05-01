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

/** @addtogroup UTILITIES_examples
  * @{
  */

/** @addtogroup USART_IAP_bootloader
  * @{
  */

/** @defgroup bootloader_definition
  * @{
  */

#define USART_REC_LEN      1024

/**
  * @}
  */

/** @defgroup bootloader_exported_types
  * @{
  */

/**
  * @brief  usart group type
  */
typedef struct
{
  uint16_t buf[USART_REC_LEN];
  uint16_t head;
  uint16_t tail;
  uint16_t count;
} usart_group_type;

/**
  * @}
  */

extern usart_group_type usart_group_struct;

/** @defgroup bootloader_exported_functions
  * @{
  */

void uart_init(uint32_t baudrate);

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
