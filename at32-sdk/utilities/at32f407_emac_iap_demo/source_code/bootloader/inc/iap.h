/**
  **************************************************************************
  * @file     iap.h
  * @version  v2.0.8
  * @date     2022-04-02
  * @brief    iap header file
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

#ifndef __IAP_H__
#define __IAP_H__

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

#define USE_IAP_TFTP   /*!< enable iap using tftp */
#define USE_IAP_HTTP   /*!< enable iap using http */

/* app starting address */
#define APP_START_SECTOR_ADDR            0x08010000
#define APP_LAST_SECTOR_ADDR             0x080FF800
#define APP_END_ADDR                     0x080FFFFF

/* the previous sector of app starting address is iap upgrade flag */
#define IAP_UPGRADE_FLAG_ADDR            (APP_START_SECTOR_ADDR - 0x800)

/* when app need upgrade, will set up the flag,
indicates that an app upgrade will follow, see iap application note for more details */
#define IAP_UPGRADE_FLAG         0x41544B38

/**
  * @}
  */

/** @defgroup bootloader_exported_functions
  * @{
  */

typedef void (*iapfun)(void);

void app_load(uint32_t appxaddr);
void iap_command_handle(void);
void iap_init(void);

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
