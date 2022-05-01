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


/** @addtogroup AT32F403A_periph_examples
  * @{
  */

/** @addtogroup 403A_EXINT_software_trigger EXINT_software_trigger
  * @{
  */


void exint_line4_config(void);
static void tmr1_config(void);

/**
  * @brief  exint line4 config. configure pa0 in interrupt mode
  * @param  None
  * @retval None
  */
void exint_line4_config(void)
{
  exint_init_type exint_init_struct;

  crm_periph_clock_enable(CRM_IOMUX_PERIPH_CLOCK, TRUE);

  exint_default_para_init(&exint_init_struct);
  exint_init_struct.line_enable = TRUE;
  exint_init_struct.line_mode = EXINT_LINE_INTERRUPUT;
  exint_init_struct.line_select = EXINT_LINE_4;
  exint_init_struct.line_polarity = EXINT_TRIGGER_RISING_EDGE;
  exint_init(&exint_init_struct);

  nvic_irq_enable(EXINT4_IRQn, 1, 0);
}

/**
  * @brief  tmr1 configuration.
  * @param  none
  * @retval none
  */
static void tmr1_config(void)
{
  crm_clocks_freq_type crm_clocks_freq_struct = {0};

  /* get system clock */
  crm_clocks_freq_get(&crm_clocks_freq_struct);

  crm_periph_clock_enable(CRM_TMR1_PERIPH_CLOCK, TRUE);

  /* (systemclock / (system_core_clock/10000)) / 10000 = 1Hz(1s) */
  tmr_base_init(TMR1, 10000-1, system_core_clock/10000-1);
  tmr_cnt_dir_set(TMR1, TMR_COUNT_UP);
  tmr_clock_source_div_set(TMR1, TMR_CLOCK_DIV1);
  tmr_interrupt_enable(TMR1, TMR_OVF_INT, TRUE);
  nvic_irq_enable(TMR1_OVF_TMR10_IRQn, 0, 0);
}

/**
  * @brief  tmr1 interrupt handler
  * @param  none
  * @retval none
  */
void TMR1_OVF_TMR10_IRQHandler(void)
{
  if(tmr_flag_get(TMR1,TMR_OVF_FLAG) != RESET)
  {
    at32_led_toggle(LED2);
    exint_software_interrupt_event_generate(EXINT_LINE_4);
    tmr_flag_clear(TMR1,TMR_OVF_FLAG);
  }
}

/**
  * @brief  exint4 interrupt handler
  * @param  none
  * @retval none
  */
void EXINT4_IRQHandler(void)
{
  if(exint_flag_get(EXINT_LINE_4) != RESET)
  {
    at32_led_toggle(LED3);
    at32_led_toggle(LED4);
    exint_flag_clear(EXINT_LINE_4);
  }
}

/**
  * @brief  main function.
  * @param  none
  * @retval none
  */
int main(void)
{
  nvic_priority_group_config(NVIC_PRIORITY_GROUP_4);
  system_clock_config();

  at32_board_init();

  /* turn led2/led3/led4 on */
  at32_led_on(LED2);
  at32_led_on(LED3);
  at32_led_on(LED4);
  tmr1_config();
  exint_line4_config();
  tmr_counter_enable(TMR1, TRUE);
  while(1)
  {
  }
}

/**
  * @}
  */

/**
  * @}
  */
