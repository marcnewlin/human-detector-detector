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

/** @addtogroup 403A_DAC_two_dac_trianglewave DAC_two_dac_trianglewave
  * @{
  */

gpio_init_type gpio_init_struct = {0};
crm_clocks_freq_type crm_clocks_freq_struct = {0};
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

  /* enable dac/tmr2/gpioa clock */
  crm_periph_clock_enable(CRM_DAC_PERIPH_CLOCK, TRUE);
  crm_periph_clock_enable(CRM_TMR2_PERIPH_CLOCK, TRUE);
  crm_periph_clock_enable(CRM_GPIOA_PERIPH_CLOCK, TRUE);

  /* once the dac is enabled, the corresponding gpio pin is automatically
     connected to the dac converter. in order to avoid parasitic consumption,
     the gpio pin should be configured in analog */
  gpio_init_struct.gpio_pins = GPIO_PINS_4 | GPIO_PINS_5;
  gpio_init_struct.gpio_mode = GPIO_MODE_ANALOG;
  gpio_init_struct.gpio_out_type = GPIO_OUTPUT_PUSH_PULL;
  gpio_init_struct.gpio_pull = GPIO_PULL_NONE;
  gpio_init_struct.gpio_drive_strength = GPIO_DRIVE_STRENGTH_STRONGER;
  gpio_init(GPIOA, &gpio_init_struct);

  /* get system clock */
  crm_clocks_freq_get(&crm_clocks_freq_struct);

  /* (systemclock/(systemclock/1000000))/100 = 10KHz */
  tmr_base_init(TMR2, 99, (crm_clocks_freq_struct.sclk_freq/1000000 - 1));
  tmr_cnt_dir_set(TMR2, TMR_COUNT_UP);

  /* primary tmr2 output selection */
  tmr_primary_mode_select(TMR2, TMR_PRIMARY_SEL_OVERFLOW);

  /* dac1 and dac2 configuration */
  dac_trigger_select(DAC1_SELECT, DAC_TMR2_TRGOUT_EVENT);
  dac_trigger_select(DAC2_SELECT, DAC_TMR2_TRGOUT_EVENT);

  dac_trigger_enable(DAC1_SELECT, TRUE);
  dac_trigger_enable(DAC2_SELECT, TRUE);

  dac_wave_generate(DAC1_SELECT, DAC_WAVE_GENERATE_TRIANGLE);
  dac_wave_generate(DAC2_SELECT, DAC_WAVE_GENERATE_TRIANGLE);

  dac_mask_amplitude_select(DAC1_SELECT, DAC_LSFR_BIT90_AMPLITUDE_1023);
  dac_mask_amplitude_select(DAC2_SELECT, DAC_LSFR_BITA0_AMPLITUDE_2047);

  dac_output_buffer_enable(DAC1_SELECT, FALSE);
  dac_output_buffer_enable(DAC2_SELECT, FALSE);

  dac_dma_enable(DAC1_SELECT, TRUE);
  dac_dma_enable(DAC2_SELECT, TRUE);

  /* enable dac1: once the dac1 is enabled, pa.04 is
     automatically connected to the dac converter. */
  dac_enable(DAC1_SELECT, TRUE);

  /* enable dac2: once the dac2 is enabled, pa.05 is
     automatically connected to the dac converter. */
  dac_enable(DAC2_SELECT, TRUE);

  dac_dual_data_set(DAC_DUAL_12BIT_RIGHT, 0x100, 0x100);

  /* enable tmr2 */
  tmr_counter_enable(TMR2, TRUE);

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
