/**
  **************************************************************************
  * @file     main.c
  * @version  v2.0.8
  * @date     2022-04-02
  * @brief    main program
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
#include "at32f403a_407_clock.h"


/** @addtogroup AT32F407_periph_examples
  * @{
  */

/** @addtogroup 407_DEBUG_tmr DEBUG_tmr
  * @{
  */

crm_clocks_freq_type crm_clocks_freq_struct = {0};
uint16_t counter = 0;

/**
  * @brief  this function handles tmr1 overflow handler.
  * @param  none
  * @retval none
  */
void TMR1_OVF_TMR10_IRQHandler(void)
{
  if(tmr_flag_get(TMR1, TMR_OVF_FLAG) != RESET)
  {
    /* add user code... */
    at32_led_toggle(LED3);
    tmr_flag_clear(TMR1, TMR_OVF_FLAG);
  }
}

/**
  * @brief  main function.
  * @param  none
  * @retval none
  */
int main(void)
{
  system_clock_config();

  at32_board_init();

  /* get system clock */
  crm_clocks_freq_get(&crm_clocks_freq_struct);

  /* turn led2/led3/led4 on */
  at32_led_on(LED2);
  at32_led_on(LED3);
  at32_led_on(LED4);

  /* enable tmr1 clock */
  crm_periph_clock_enable(CRM_TMR1_PERIPH_CLOCK, TRUE);

  /* enable tmr1 debug mode  */
  debug_periph_mode_set(DEBUG_TMR1_PAUSE, TRUE);

  /* tmr1 configuration */
  tmr_base_init(TMR1, 10000, 0);
  tmr_cnt_dir_set(TMR1, TMR_COUNT_UP);

  /* overflow interrupt enable */
  tmr_interrupt_enable(TMR1, TMR_OVF_INT, TRUE);

  /* tmr1 interrupt nvic init */
  nvic_priority_group_config(NVIC_PRIORITY_GROUP_4);
  nvic_irq_enable(TMR1_OVF_TMR10_IRQn, 1, 0);

  /* enable tmr1 */
  tmr_counter_enable(TMR1, TRUE);

  while(1)
  {
    /* in debug mode,tmr1 counter will pause */
    counter = tmr_counter_value_get(TMR1);
    counter = tmr_counter_value_get(TMR1);
    counter = tmr_counter_value_get(TMR1);
    counter = tmr_counter_value_get(TMR1);
    counter = tmr_counter_value_get(TMR1);
  }
}

/**
  * @}
  */

/**
  * @}
  */
