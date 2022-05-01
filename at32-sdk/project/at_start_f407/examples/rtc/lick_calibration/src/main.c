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

/** @addtogroup 407_RTC_lick_calibration RTC_lick_calibration
  * @{
  */

crm_clocks_freq_type crm_clocks;
__IO uint32_t periodvalue = 0;
__IO uint32_t lickfreq = 0;
__IO uint32_t operationcomplete = 0;

/**
  * @brief  configures the nested vectored interrupt controller.
  * @param  none
  * @retval none
  */
void nvic_configuration(void)
{
  /* configure one bit for preemption priority */
  nvic_priority_group_config(NVIC_PRIORITY_GROUP_4);

  /* enable the rtc interrupt */
  nvic_irq_enable(RTC_IRQn, 0, 0);

  /* enable the tmr5 interrupt */
  nvic_irq_enable(TMR5_GLOBAL_IRQn, 0, 0);
}

/**
  * @brief  configures the rtc.
  * @param  none
  * @retval none
  */
void rtc_configuration(void)
{
  /* enable pwc and bpr clocks */
  crm_periph_clock_enable(CRM_PWC_PERIPH_CLOCK, TRUE);
  crm_periph_clock_enable(CRM_BPR_PERIPH_CLOCK, TRUE);

  /* allow access to bpr domain */
  pwc_battery_powered_domain_access(TRUE);

  /* reset backup domain */
  bpr_reset();

  /* enable the lick osc */
  crm_clock_source_enable(CRM_CLOCK_SOURCE_LICK, TRUE);
  /* wait till lick is ready */
  while(crm_flag_get(CRM_LICK_STABLE_FLAG) == RESET);
  /* select the rtc clock source */
  crm_rtc_clock_select(CRM_RTC_CLOCK_LICK);

  /* enable rtc clock */
  crm_rtc_clock_enable(TRUE);

  /* wait for rtc registers update */
  rtc_wait_update_finish();

  /* wait for the register write to complete */
  rtc_wait_config_finish();

  /* enable the rtc second */
  rtc_interrupt_enable(RTC_TS_INT, TRUE);

  /* wait for the register write to complete */
  rtc_wait_config_finish();

  /* set rtc divider: set rtc period to 1sec */
  rtc_divider_set(40000);

  /* wait for the register write to complete */
  rtc_wait_config_finish();

  /* tamper pin disabled */
  bpr_tamper_pin_enable(FALSE);

  /* enable the rtc second output on tamper pin */
  bpr_rtc_output_select(BPR_RTC_OUTPUT_SECOND);
}

/**
  * @brief  increments operationcomplete variable and return its value
  *         before increment operation.
  * @param  none
  * @retval operationcomplete value before increment
  */
uint32_t incrementvar_operationcomplete(void)
{
  operationcomplete++;

  return (uint32_t)(operationcomplete - 1);
}

/**
  * @brief  returns operationcomplete value.
  * @param  none
  * @retval operationcomplete value
  */
uint32_t getvar_operationcomplete(void)
{
  return (uint32_t)operationcomplete;
}

/**
  * @brief  sets the periodvalue variable with input parameter.
  * @param  value: value of periodvalue to be set.
  * @retval none
  */
void setvar_periodvalue(uint32_t value)
{
  periodvalue = (uint32_t)(value);
}

/**
  * @brief  main function.
  * @param  none
  * @retval none
  */
int main(void)
{
  tmr_input_config_type tmr_ic_init_structure;

  system_clock_config();

  at32_board_init();

  uart_print_init(115200);

  /* rtc configuration */
  rtc_configuration();

  printf("\r\n\nstart calib\r\n\r\n");

  /* get the frequency value */
  crm_clocks_freq_get(&crm_clocks);

  /* enable tmr5 apb1 clocks */
  crm_periph_clock_enable(CRM_TMR5_PERIPH_CLOCK, TRUE);
  crm_periph_clock_enable(CRM_IOMUX_PERIPH_CLOCK, TRUE);

  /* connect internally the tm5_ch4 input capture to the lick clock output */
  gpio_pin_remap_config(TMR5CH4_MUX, TRUE);

  /* tmr5 time base configuration */
  tmr_base_init(TMR5, 0xFFFF, 0);
  tmr_cnt_dir_set(TMR5, TMR_COUNT_UP);
  tmr_clock_source_div_set(TMR5, TMR_CLOCK_DIV1);

  /* tmr5 channel4 input capture mode configuration */
  tmr_input_default_para_init(&tmr_ic_init_structure);
  tmr_ic_init_structure.input_channel_select = TMR_SELECT_CHANNEL_4;
  tmr_ic_init_structure.input_polarity_select = TMR_INPUT_RISING_EDGE;
  tmr_ic_init_structure.input_mapped_select = TMR_CC_CHANNEL_MAPPED_DIRECT;
  tmr_ic_init_structure.input_filter_value = 0;
  tmr_input_channel_init(TMR5, &tmr_ic_init_structure, TMR_CHANNEL_INPUT_DIV_1);

  /* reinitialize the index for the interrupt */
  operationcomplete = 0;

  /* enable the tmr5 input capture counter */
  tmr_counter_enable(TMR5, TRUE);

  /* Reset all tmr5 flags */
  tmr_flag_get(TMR5, TMR_C4_FLAG);

  /* enable the tmr5 channel 4 */
  tmr_interrupt_enable(TMR5, TMR_C4_INT, TRUE);

  /* nvic configuration */
  nvic_configuration();

  /* wait the tmr5 measuring operation to be completed */
  while(operationcomplete != 2);

  /* compute the actual frequency of the lick. (tim5_clk = 1 * pclk1)  */
  if(periodvalue != 0)
  {
    lickfreq = (uint32_t)((uint32_t)(crm_clocks.apb1_freq * 2) / (uint32_t)periodvalue);
  }

  printf("apb1_freq    = %d\r\n", crm_clocks.apb1_freq);
  printf("period_value = %d\r\n", periodvalue);
  printf("lick_freq    = %d\r\n", lickfreq);

  /* adjust the rtc prescaler value */
  rtc_divider_set((lickfreq - 1));

  /* wait for the register write to complete */
  rtc_wait_config_finish();

  /* turn on led2 */
  at32_led_on(LED2);

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
