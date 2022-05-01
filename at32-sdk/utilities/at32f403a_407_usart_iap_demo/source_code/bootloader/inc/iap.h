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

/** @addtogroup USART_IAP_bootloader
  * @{
  */

/** @defgroup bootloader_definition
  * @{
  */

/* app starting address */
#define APP_START_ADDR          0x08004000

/* the previous sector of app starting address is iap upgrade flag */
#define IAP_UPGRADE_FLAG_ADDR    (APP_START_ADDR - 0x800)

/* when app received cmd 0x5aa5 from pc-tool, will set up the flag,
indicates that an app upgrade will follow, see iap application note for more details */
#define IAP_UPGRADE_FLAG         0x41544B38

/**
  * @}
  */

/** @defgroup bootloader_exported_types
  * @{
  */

/**
  * @brief  cmd data group type
  */
typedef struct
{
  uint8_t cmd_head;
  uint8_t cmd_addr[4];
  uint8_t cmd_buf[0x800];
  uint8_t cmd_check;
} cmd_data_group_type;

/**
  * @brief  cmd data step type
  */
typedef enum
{
  CMD_DATA_IDLE,
  CMD_DATA_ADDR,
  CMD_DATA_BUF,
  CMD_DATA_CHACK,
  CMD_DATA_DONE,
  CMD_DATA_ERR,
} cmd_data_step_type;

/**
  * @brief  cmd ctr step type
  */
typedef enum
{
  CMD_CTR_IDLE,
  CMD_CTR_INIT,
  CMD_CTR_DONE,
  CMD_CTR_ERR,
  CMD_CTR_APP,
} cmd_ctr_step_type;

/**
  * @brief  update status type
  */
typedef enum
{
  UPDATE_PRE,
  UPDATE_CLEAR_FLAG,
  UPDATE_ING,
  UPDATE_DONE,
} update_status_type;

typedef void (*iapfun)(void);

/**
  * @}
  */

extern cmd_data_group_type cmd_data_group_type_struct;
extern cmd_data_step_type cmd_data_step;
extern cmd_ctr_step_type cmd_ctr_step;
extern update_status_type update_status;

/** @defgroup bootloader_exported_functions
  * @{
  */

void command_handle(void);
void back_err(void);
void back_ok(void);
void iap_upgrade_app_handle(void);
void app_load(uint32_t appxaddr);

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
