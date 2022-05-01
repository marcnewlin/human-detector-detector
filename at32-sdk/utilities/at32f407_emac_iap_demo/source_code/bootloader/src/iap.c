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

#include "stdlib.h"
#include "string.h"
#include "iap.h"
#include "flash.h"
#include "tmr.h"

/** @addtogroup UTILITIES_examples
  * @{
  */

/** @addtogroup EMAC_IAP_bootloader
  * @{
  */

iapfun jump_to_app;

/* app_load don't optimize */
#if defined (__CC_ARM)
  #pragma O0
#elif defined (__ICCARM__)
  #pragma optimize=s none
#endif
/**
  * @brief  app load.
  * @param  app_addr
  *         app code starting address
  * @retval none
  */
void app_load(uint32_t app_addr)
{
  /* check the address of stack */
  if(((*(uint32_t*)app_addr) - 0x20000000) < (SRAM_SIZE * 1024))
  {
    /* disable periph clock */
    crm_periph_clock_enable(CRM_EMAC_PERIPH_CLOCK, FALSE);
    crm_periph_clock_enable(CRM_EMACTX_PERIPH_CLOCK, FALSE);
    crm_periph_clock_enable(CRM_EMACRX_PERIPH_CLOCK, FALSE);
    crm_periph_clock_enable(CRM_TMR3_PERIPH_CLOCK, FALSE);

    /* disable nvic irq and clear pending */
    nvic_irq_disable(EMAC_IRQn);
    nvic_irq_disable(TMR3_GLOBAL_IRQn);
    __NVIC_ClearPendingIRQ(EMAC_IRQn);
    __NVIC_ClearPendingIRQ(TMR3_GLOBAL_IRQn);

    jump_to_app = (iapfun)*(uint32_t*)(app_addr + 4);        /* code second word is reset address */
    __set_MSP(*(uint32_t*)app_addr);                        /* init app stack pointer(code first word is stack address) */
    jump_to_app();                                          /* jump to user app */
  }
}

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
