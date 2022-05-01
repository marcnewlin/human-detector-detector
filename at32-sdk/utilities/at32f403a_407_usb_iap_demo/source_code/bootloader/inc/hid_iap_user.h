/**
  **************************************************************************
  * @file     hid_iap_user.h
  * @version  v2.0.8
  * @date     2022-04-02
  * @brief    usb config header file
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

/* define to prevent recursive inclusion -------------------------------------*/
#ifndef __HID_IAP_USER_H
#define __HID_IAP_USER_H

#ifdef __cplusplus
extern "C" {
#endif

#include "hid_iap_class.h"

/** @addtogroup UTILITIES_examples
  * @{
  */

/** @addtogroup USB_IAP_bootloader
  * @{
  */

/** @defgroup bootloader_definition
  * @{
  */

#define FLASH_APP_ADDRESS                0x08004000

/**
  * @}
  */

/** @defgroup bootloader_exported_functions
  * @{
  */

void iap_init(void);
iap_result_type iap_get_upgrade_flag(void);
void iap_loop(void);
void jump_to_app(uint32_t address);

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


