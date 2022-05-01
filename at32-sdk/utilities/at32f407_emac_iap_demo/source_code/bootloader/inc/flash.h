/**
  **************************************************************************
  * @file     flash.h
  * @version  v2.0.8
  * @date     2022-04-02
  * @brief    flash header file
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

#ifndef __FLASH_H__
#define __FLASH_H__

#ifdef __cplusplus
 extern "C" {
#endif

#include "at32f403a_407_board.h"

/** @addtogroup UTILITIES_examples
  * @{
  */

/** @addtogroup EMAC_IAP_bootloader
  * @{
  */

/** @defgroup bootloader_definition
  * @{
  */

#define FLASH_SIZE                       (*((uint32_t*)0x1FFFF7E0))  /* read from at32 flash capacity register(unit:kbyte) */
#define SRAM_SIZE                        224                         /* sram size, unit:kbyte */

/**
  * @}
  */

/** @defgroup bootloader_exported_functions
  * @{
  */

void flash_2kb_write(uint32_t write_addr, uint8_t *pbuffer);
uint32_t flash_if_write(__IO uint32_t* flash_address, uint32_t* data ,uint16_t data_length);
flag_status flash_upgrade_flag_read(void);

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
