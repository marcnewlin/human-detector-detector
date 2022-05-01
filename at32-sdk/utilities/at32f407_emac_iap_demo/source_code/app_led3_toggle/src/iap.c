/**
  **************************************************************************
  * @file     iap.c
  * @version  v2.0.8
  * @date     2022-04-02
  * @brief    iap program
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

#include "iap.h"
#include "tmr.h"

/** @addtogroup UTILITIES_examples
  * @{
  */

/** @addtogroup EMAC_IAP_app_led3_toggle
  * @{
  */

/**
  * @brief  iap command handle handle.
  * @param  none
  * @retval none
  */
void iap_command_handle(void)
{
  flash_unlock();
  flash_sector_erase(IAP_UPGRADE_FLAG_ADDR);
  flash_word_program(IAP_UPGRADE_FLAG_ADDR, IAP_UPGRADE_FLAG);
  flash_lock();
  nvic_system_reset();
}

/**
  * @brief  iap init.
  * @param  none
  * @retval none
  */
void iap_init(void)
{
  if((*(uint32_t*)IAP_UPGRADE_FLAG_ADDR) == IAP_UPGRADE_FLAG)
  {
    flash_unlock();
    flash_sector_erase(IAP_UPGRADE_FLAG_ADDR);
    flash_lock();
  }
}

/**
  * @}
  */

/**
  * @}
  */
