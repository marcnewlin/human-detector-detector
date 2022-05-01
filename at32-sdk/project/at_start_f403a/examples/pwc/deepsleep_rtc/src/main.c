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

/** @addtogroup 403A_PWC_deepsleep_rtc PWC_deepsleep_rtc
  * @{
  */

/**
  * @brief  rtc configuration.
  * @param  none
  * @retval none
  */
void rtc_config(void)
{
  exint_init_type exint_init_struct;

  /* config the exint line of the rtc alarm */
  exint_init_struct.line_select   = EXINT_LINE_17;
  exint_init_struct.line_enable   = TRUE;
  exint_init_struct.line_mode     = EXINT_LINE_INTERRUPUT;
  exint_init_struct.line_polarity = EXINT_TRIGGER_BOTH_EDGE;
  exint_init(&exint_init_struct);

  /* enable the battery-powered domain write operations */
  pwc_battery_powered_domain_access(TRUE);

  /* reset battery-powered domain register */
  bpr_reset();

  /* enable the lick */
  crm_clock_source_enable(CRM_CLOCK_SOURCE_LICK, TRUE);

  /* wait lick is ready */
  while(crm_flag_get(CRM_LICK_STABLE_FLAG) == RESET);

  /* select the rtc clock source */
  crm_rtc_clock_select(CRM_RTC_CLOCK_LICK);

  /* enable rtc clock */
  crm_rtc_clock_enable(TRUE);

  /* wait for rtc registers update */
  rtc_wait_update_finish();

  /* set rtc divider: set rtc period to 1sec */
  rtc_divider_set(32767);

  /* wait for the register write to complete */
  rtc_wait_config_finish();

  /* enable alarm interrupt */
  rtc_interrupt_enable(RTC_TA_INT, TRUE);

  /* wait for the register write to complete */
  rtc_wait_config_finish();

  /* configure and enable rtc alarm interrupt */
  nvic_irq_enable(RTCAlarm_IRQn, 0, 0);
}

/**
  * @brief  systemclock recover.
  * @param  none
  * @retval none
  */
void system_clock_recover(void)
{
  /* enable external high-speed crystal oscillator - hext */
  crm_clock_source_enable(CRM_CLOCK_SOURCE_HEXT, TRUE);

  /* wait till hext is ready */
  while(crm_hext_stable_wait() == ERROR);

  /* enable pll */
  crm_clock_source_enable(CRM_CLOCK_SOURCE_PLL, TRUE);

  /* wait till pll is ready */
  while(crm_flag_get(CRM_PLL_STABLE_FLAG) == RESET);

  /* enable auto step mode */
  crm_auto_step_mode_enable(TRUE);

  /* select pll as system clock source */
  crm_sysclk_switch(CRM_SCLK_PLL);

  /* wait till pll is used as system clock source */
  while(crm_sysclk_switch_status_get() != CRM_SCLK_PLL);
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

  /* enable pwc and bpr clock */
  crm_periph_clock_enable(CRM_PWC_PERIPH_CLOCK, TRUE);
  crm_periph_clock_enable(CRM_BPR_PERIPH_CLOCK, TRUE);

  /* config rtc */
  rtc_config();

  while(1)
  {
    /* clear second flag */
    rtc_flag_clear(RTC_TS_FLAG);

    /* wait for the second flag to be set */
    while(rtc_flag_get(RTC_TS_FLAG) == RESET);

    /* set the wakeup time to 3 seconds */
    rtc_alarm_set(rtc_counter_get() + 3);

    /* wait for the register write to complete */
    rtc_wait_config_finish();

    at32_led_off(LED2);

    /* save systick register configuration */
    systick_index = SysTick->CTRL;
    systick_index &= ~((uint32_t)0xFFFFFFFE);

    /* disable systick */
    SysTick->CTRL &= (uint32_t)0xFFFFFFFE;

    /* congfig the voltage regulator mode */
    pwc_voltage_regulate_set(PWC_REGULATOR_LOW_POWER);

    /* enter deep sleep mode */
    pwc_deep_sleep_mode_enter(PWC_DEEP_SLEEP_ENTER_WFI);

    /* restore systick register configuration */
    SysTick->CTRL |= systick_index;

    /* wake up from deep sleep mode, congfig the system clock */
    system_clock_recover();

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
