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
#include "rtc.h"

/** @addtogroup AT32F403A_periph_examples
  * @{
  */

/** @addtogroup 403A_RTC_calendar RTC_calendar
  * @{
  */

char const weekday_table[7][10] = { "Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};

/**
  * @brief  init alarm.
  * @param  none
  * @retval none
  */
void alarm_init(void)
{
  calendar_type alarm_struct;

  /* clear alarm flag */
  rtc_flag_clear(RTC_TA_FLAG);

  /* wait for the register write to complete */
  rtc_wait_config_finish();

  /* configure and enable rtc interrupt */
  nvic_irq_enable(RTC_IRQn, 0, 0);

  /* enable alarm interrupt */
  rtc_interrupt_enable(RTC_TA_INT, TRUE);

  /* wait for the register write to complete */
  rtc_wait_config_finish();

  /* config alarm */

  alarm_struct.year  = 2021;
  alarm_struct.month = 5;
  alarm_struct.date  = 1;
  alarm_struct.hour  = 12;
  alarm_struct.min   = 0;
  alarm_struct.sec   = 5;

  rtc_alarm_clock_set(&alarm_struct);
}

/**
  * @brief  main function.
  * @param  none
  * @retval none
  */
int main(void)
{
  calendar_type time_struct;

  /* config nvic priority group */
  nvic_priority_group_config(NVIC_PRIORITY_GROUP_4);

  system_clock_config();

  at32_board_init();

  uart_print_init(115200);

  /* config calendar */
  time_struct.year  = 2021;
  time_struct.month = 5;
  time_struct.date  = 1;
  time_struct.hour  = 12;
  time_struct.min   = 0;
  time_struct.sec   = 0;
  rtc_init(&time_struct);

  /* config alarm */
  alarm_init();

  printf("initial ok\r\n");

  while(1)
  {
    if(rtc_flag_get(RTC_TS_FLAG) != RESET)
    {
      at32_led_toggle(LED3);

      /* get time */
      rtc_time_get();

      /* print time */
      printf("%d/%d/%d ", calendar.year, calendar.month, calendar.date);
      printf("%02d:%02d:%02d %s\r\n", calendar.hour, calendar.min, calendar.sec, weekday_table[calendar.week]);

      /* wait for the register write to complete */
      rtc_wait_config_finish();

      /* clear the rtc second flag */
      rtc_flag_clear(RTC_TS_FLAG);

      /* wait for the register write to complete */
      rtc_wait_config_finish();
    }
  }
}

/**
  * @}
  */

/**
  * @}
  */
