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

/** @addtogroup 407_TMR_cascade_synchro TMR_cascade_synchro
  * @{
  */

gpio_init_type gpio_init_struct = {0};
tmr_output_config_type tmr_output_struct;

/**
  * @brief  main function.
  * @param  none
  * @retval none
  */
int main(void)
{
  system_clock_config();

  at32_board_init();

  /* turn led2/led3/led4 on */
  at32_led_on(LED2);
  at32_led_on(LED3);
  at32_led_on(LED4);

  /* enable tmr2/tmr3/tmr4/gpioa/gpiob clock */
  crm_periph_clock_enable(CRM_TMR2_PERIPH_CLOCK, TRUE);
  crm_periph_clock_enable(CRM_TMR3_PERIPH_CLOCK, TRUE);
  crm_periph_clock_enable(CRM_TMR4_PERIPH_CLOCK, TRUE);
  crm_periph_clock_enable(CRM_GPIOA_PERIPH_CLOCK, TRUE);
  crm_periph_clock_enable(CRM_GPIOB_PERIPH_CLOCK, TRUE);

  /* timer1 output pin Configuration */
  gpio_init_struct.gpio_pins = GPIO_PINS_6 | GPIO_PINS_0;
  gpio_init_struct.gpio_mode = GPIO_MODE_MUX;
  gpio_init_struct.gpio_out_type = GPIO_OUTPUT_PUSH_PULL;
  gpio_init_struct.gpio_pull = GPIO_PULL_NONE;
  gpio_init_struct.gpio_drive_strength = GPIO_DRIVE_STRENGTH_STRONGER;
  gpio_init(GPIOA, &gpio_init_struct);

  gpio_init_struct.gpio_pins = GPIO_PINS_6;
  gpio_init_struct.gpio_mode = GPIO_MODE_MUX;
  gpio_init_struct.gpio_out_type = GPIO_OUTPUT_PUSH_PULL;
  gpio_init_struct.gpio_pull = GPIO_PULL_NONE;
  gpio_init_struct.gpio_drive_strength = GPIO_DRIVE_STRENGTH_STRONGER;
  gpio_init(GPIOB, &gpio_init_struct);


  /* timers synchronisation in cascade mode
   1/tmr2 is configured as primary timer:
   - pwm mode is used
   - the tmr2 update event is used as trigger output

   2/tmr3 is sub for tmr2 and primary for tmr4,
   - pwm mode is used
   - the itr1(tmr2) is used as input trigger
   - gated mode is used, so start and stop of sub counter
    are controlled by the master trigger output signal(tmr2 update event).
    - the tmr3 update event is used as trigger output.

    3/tmr4 is slave for tmr3,
   - pwm mode is used
   - the itr2(tmr3) is used as input trigger
   - gated mode is used, so start and stop of sub counter
    are controlled by the master trigger output signal(tmr3 overflow event).

   *  the timxclk is fixed to 240 mhz, the tmr2 counter clock is 240 mhz.

    the primary timer tmr2 is running at tmr2 frequency :
    tim2 frequency = (tmr2 counter clock)/ (tmr2 period + 1) = 1 mhz
    and the duty cycle = tmr2_ccr1/(tmr2_arr + 1) = 25%.

    the tmr3 is running at:
    (tmr2 frequency)/ (tmr3 period + 1) = 100 khz and a duty cycle equal
    to tmr3_ccr1/(tmr3_arr + 1) = 25%

    the tmr4 is running at:
    (tmr3 frequency)/ (tmr4 period + 1) = 100 khz and a duty cycle equal
    to tmr4_ccr1/(tmr4_arr + 1) = 25%

   * the timxclk is fixed to 240 mhz, the tmr2 counter clock is 240 mhz.
    so tmr2 frequency = 1mhz,
    tmr3 is running at 41.67 khz,
    and tmr4 is running at 1.74 khz */

  tmr_base_init(TMR2, 239, 0);
  tmr_cnt_dir_set(TMR2, TMR_COUNT_UP);

  tmr_base_init(TMR3, 23, 0);
  tmr_cnt_dir_set(TMR3, TMR_COUNT_UP);

  tmr_base_init(TMR4, 23, 0);
  tmr_cnt_dir_set(TMR4, TMR_COUNT_UP);

  /* channelx Configuration in output mode */
  tmr_output_default_para_init(&tmr_output_struct);
  tmr_output_struct.oc_mode = TMR_OUTPUT_CONTROL_PWM_MODE_A;
  tmr_output_struct.oc_output_state = TRUE;
  tmr_output_struct.oc_polarity = TMR_OUTPUT_ACTIVE_LOW;
  tmr_output_struct.oc_idle_state = TRUE;
  tmr_output_struct.occ_output_state = FALSE;
  tmr_output_struct.occ_polarity = TMR_OUTPUT_ACTIVE_HIGH;
  tmr_output_struct.occ_idle_state = FALSE;

  /* timer2 channel 1 */
  tmr_output_channel_config(TMR2, TMR_SELECT_CHANNEL_1, &tmr_output_struct);
  tmr_channel_value_set(TMR2, TMR_SELECT_CHANNEL_1, 59);

  /* timer2 select enable signal to sub timer */
  tmr_primary_mode_select(TMR2, TMR_PRIMARY_SEL_OVERFLOW);
  tmr_sub_sync_mode_set(TMR2, TRUE);

  /* timer3 channel 1 */
  tmr_output_channel_config(TMR3, TMR_SELECT_CHANNEL_1, &tmr_output_struct);
  tmr_channel_value_set(TMR3, TMR_SELECT_CHANNEL_1, 5);

  /* sub mode selection: tmr3  */
  tmr_sub_mode_select(TMR3, TMR_SUB_HANG_MODE);
  tmr_trigger_input_select(TMR3, TMR_SUB_INPUT_SEL_IS1);

  /* timer3 select enable signal to sub timer */
  tmr_primary_mode_select(TMR3, TMR_PRIMARY_SEL_OVERFLOW);
  tmr_sub_sync_mode_set(TMR3, TRUE);

  /* timer4 channel 1 */
  tmr_output_channel_config(TMR4, TMR_SELECT_CHANNEL_1, &tmr_output_struct);
  tmr_channel_value_set(TMR4, TMR_SELECT_CHANNEL_1, 5);

  /* sub mode selection: tmr4  */
  tmr_sub_mode_select(TMR4, TMR_SUB_HANG_MODE);
  tmr_trigger_input_select(TMR4, TMR_SUB_INPUT_SEL_IS2);

  /* enable tmr2/tmr3/tmr4 */
  tmr_counter_enable(TMR2, TRUE);
  tmr_counter_enable(TMR3, TRUE);
  tmr_counter_enable(TMR4, TRUE);

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
