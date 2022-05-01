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

/** @addtogroup 403A_PWC_standby_rtc PWC_standby_rtc
  * @{
  */


/**
  * @brief  rtc configuration.
  * @param  none
  * @retval none
  */
void rtc_config(void)
{
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
}

/**
  * @brief  rtc alarm set.
  * @param  none
  * @retval none
  */
void rtc_alarm_config(uint8_t alarm_time)
{
  /* clear second flag */
  rtc_flag_clear(RTC_TS_FLAG);

  /* wait for the second flag to be set */
  while(rtc_flag_get(RTC_TS_FLAG) == RESET);

  /* config the wakeup time */
  rtc_alarm_set(rtc_counter_get() + alarm_time);

  /* wait for the register write to complete */
  rtc_wait_config_finish();
}

/**
  * @brief  main function.
  * @param  none
  * @retval none
  */
int main(void)
{
  __IO uint32_t index = 0;

  /* congfig the system clock */
  system_clock_config();

  /* init at start board */
  at32_board_init();

  /* config priority group */
  nvic_priority_group_config(NVIC_PRIORITY_GROUP_4);

  /* turn on the led light */
  at32_led_off(LED2);
  at32_led_off(LED3);
  at32_led_off(LED4);

  /* enable pwc and bpr clock */
  crm_periph_clock_enable(CRM_PWC_PERIPH_CLOCK, TRUE);
  crm_periph_clock_enable(CRM_BPR_PERIPH_CLOCK, TRUE);

  if(pwc_flag_get(PWC_STANDBY_FLAG) != RESET)
  {
    /* wakeup from standby */
    pwc_flag_clear(PWC_STANDBY_FLAG);
    at32_led_on(LED2);
  }

  if(pwc_flag_get(PWC_WAKEUP_FLAG) != RESET)
  {
    /* wakeup event occurs */
    pwc_flag_clear(PWC_WAKEUP_FLAG);
    at32_led_on(LED3);
  }

  /* config rtc */
  rtc_config();

  at32_led_on(LED4);

  for(index = 0; index < 500000; index++);

  /* set the wakeup time to 10 seconds */
  rtc_alarm_config(10);

  /* enter standby mode */
  pwc_standby_mode_enter();

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
