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

/** @addtogroup 407_PWC_sleep_tmr2 PWC_sleep_tmr2
  * @{
  */


/**
  * @brief  tmr2 configuration.
  * @param  none
  * @retval none
  */
void tmr2_config(void)
{
  crm_clocks_freq_type crm_clocks_freq_struct = {0};

  /* enable tmr 2 clock */
  crm_periph_clock_enable(CRM_TMR2_PERIPH_CLOCK, TRUE);

  /* get system clock */
  crm_clocks_freq_get(&crm_clocks_freq_struct);

  /* (systemclock/(systemclock/10000))/10000 = 1Hz(1s) */
  tmr_base_init(TMR2, 9999, (crm_clocks_freq_struct.sclk_freq/10000 - 1));

  /* config the counting direction */
  tmr_cnt_dir_set(TMR2, TMR_COUNT_UP);

  /* config the clock divider value */
  tmr_clock_source_div_set(TMR2, TMR_CLOCK_DIV1);

  /* enable tmr 2 interrupt */
  tmr_interrupt_enable(TMR2, TMR_OVF_INT, TRUE);

  /* config tmr 2 nvic */
  nvic_irq_enable(TMR2_GLOBAL_IRQn, 0, 0);

  /* enable tmr 2 */
  tmr_counter_enable(TMR2, TRUE);
}

/**
  * @brief  main function.
  * @param  none
  * @retval none
  */
int main(void)
{
  __IO uint32_t index = 0;
  __IO uint32_t systick_index = 0;

  /* congfig the system clock */
  system_clock_config();

  /* init at start board */
  at32_board_init();

  /* config priority group */
  nvic_priority_group_config(NVIC_PRIORITY_GROUP_4);

  /* turn on the led light */
  at32_led_on(LED2);
  at32_led_on(LED3);
  at32_led_on(LED4);

  /* enable pwc clock */
  crm_periph_clock_enable(CRM_PWC_PERIPH_CLOCK, TRUE);

  /* config tmr 2 */
  tmr2_config();

  while(1)
  {
    at32_led_off(LED2);

    /* save systick register configuration */
    systick_index = SysTick->CTRL;
    systick_index &= ~((uint32_t)0xFFFFFFFE);

    /* disable systick */
    SysTick->CTRL &= (uint32_t)0xFFFFFFFE;

    /* enter sleep mode */
    pwc_sleep_mode_enter(PWC_SLEEP_ENTER_WFI);

    /* restore systick register configuration */
    SysTick->CTRL |= systick_index;

    /* wake up from sleep mode */
    at32_led_on(LED2);
    for(index = 0; index < 500000; index++);
  }
}

/**
  * @}
  */

/**
  * @}
  */
