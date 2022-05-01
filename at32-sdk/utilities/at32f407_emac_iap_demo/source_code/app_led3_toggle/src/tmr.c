/**
  **************************************************************************
  * @file     tmr.c
  * @version  v2.0.8
  * @date     2022-04-02
  * @brief    tmr program
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

#include "at32f403a_407_board.h"
#include "tmr.h"
#include "iap.h"

/** @addtogroup UTILITIES_examples
  * @{
  */

/** @addtogroup EMAC_IAP_app_led3_toggle
  * @{
  */

/**
  * @brief  init tmr.
  * @param  none
  * @retval none
  */
void tmr_init(void)
{
  crm_clocks_freq_type crm_clocks_freq_struct = {0};

  crm_periph_clock_enable(CRM_TMR3_PERIPH_CLOCK, TRUE);

  /* enable the tmr3 global interrupt */
  nvic_irq_enable(TMR3_GLOBAL_IRQn, 0, 0);

  /* get system clock */
  crm_clocks_freq_get(&crm_clocks_freq_struct);

  /* time base configuration */
  tmr_base_init(TMR3, 10000, crm_clocks_freq_struct.ahb_freq / 10000);
  tmr_cnt_dir_set(TMR3, TMR_COUNT_UP);

  /* overflow interrupt enable */
  tmr_interrupt_enable(TMR3, TMR_OVF_INT, TRUE);

  /* enable tmr */
  tmr_counter_enable(TMR3, TRUE);

}

/**
  * @brief  tmr3 global interrupt handler.
  * @param  none
  * @retval none
  */
void TMR3_GLOBAL_IRQHandler(void)
{
  if(tmr_flag_get(TMR3, TMR_OVF_FLAG) == SET)
  {
    tmr_flag_clear(TMR3, TMR_OVF_FLAG);
    at32_led_toggle(LED3);
  }
}

/**
  * @}
  */

/**
  * @}
  */
