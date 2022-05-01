/**
  **************************************************************************
  * @file     usb_conf.h
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
#ifndef __USB_CONF_H
#define __USB_CONF_H

#ifdef __cplusplus
extern "C" {
#endif

#include "at32f403a_407.h"
#include "at32f403a_407_board.h"
#include "stdio.h"

/** @addtogroup AT32F403A_periph_examples
  * @{
  */

/** @addtogroup 403A_ACC_calibration
  * @{
  */

/**
  * @brief usb endpoint max num define
  */
#ifndef USB_EPT_MAX_NUM
#define USB_EPT_MAX_NUM                   8
#endif

/**
  * @brief usb buffer extend to 768-1280 bytes
  */
//#define USB_BUFFER_SIZE_EX


/**
  * @brief auto malloc usb endpoint buffer
  */
//#define USB_EPT_AUTO_MALLOC_BUFFER


#ifndef USB_EPT_AUTO_MALLOC_BUFFER
/**
  * @brief user custom endpoint buffer
  *        EPTn_TX_ADDR, EPTn_RX_ADDR must less than usb buffer size
  */

/* ept0 tx start address 0x40, size 0x40,
   so rx start address is 0x40 + 0x40 = 0x80 */
#define EPT0_TX_ADDR                     0x40
#define EPT0_RX_ADDR                     0x80

#define EPT1_TX_ADDR                     0xC0
#define EPT1_RX_ADDR                     0x100

#define EPT2_TX_ADDR                     0x140
#define EPT2_RX_ADDR                     0x180

#define EPT3_TX_ADDR                     0x00
#define EPT3_RX_ADDR                     0x00

#define EPT4_TX_ADDR                     0x00
#define EPT4_RX_ADDR                     0x00

#define EPT5_TX_ADDR                     0x00
#define EPT5_RX_ADDR                     0x00

#define EPT6_TX_ADDR                     0x00
#define EPT6_RX_ADDR                     0x00

#define EPT7_TX_ADDR                     0x00
#define EPT7_RX_ADDR                     0x00

#endif
void usb_delay_ms(uint32_t ms);
void usb_delay_us(uint32_t us);

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
