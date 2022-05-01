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

/** @addtogroup 407_DAC_double_mode_dma_sinewave DAC_double_mode_dma_sinewave
  * @{
  */

gpio_init_type  gpio_init_struct = {0};
dma_init_type dma_init_struct = {0};
crm_clocks_freq_type crm_clocks_freq_struct = {0};
const uint16_t sine12bit[32] = {2047, 2447, 2831, 3185, 3498, 3750, 3939, 4056,
                                4095, 4056, 3939, 3750, 3495, 3185, 2831, 2447,
                                2047, 1647, 1263, 909,  599,  344,  155,  38,
                                0,    38,   155,  344,  599,  909,  1263, 1647};
uint32_t dualsine12bit[32];
uint32_t idx = 0;

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
  crm_periph_clock_enable(CRM_DMA1_PERIPH_CLOCK, TRUE);
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

  dac_wave_generate(DAC1_SELECT, DAC_WAVE_GENERATE_NONE);
  dac_wave_generate(DAC2_SELECT, DAC_WAVE_GENERATE_NONE);

  dac_output_buffer_enable(DAC1_SELECT, FALSE);
  dac_output_buffer_enable(DAC2_SELECT, FALSE);

  dac_dma_enable(DAC1_SELECT, TRUE);
  dac_dma_enable(DAC2_SELECT, TRUE);

  /* fill sine32bit table */
  for(idx = 0; idx < 32; idx++)
  {
    dualsine12bit[idx] = (sine12bit[idx] << 16) + (sine12bit[idx]);
  }

  /* dma1 channel1 configuration */
  dma_reset(DMA1_CHANNEL1);
  dma_init_struct.buffer_size = 32;
  dma_init_struct.direction = DMA_DIR_MEMORY_TO_PERIPHERAL;
  dma_init_struct.memory_base_addr = (uint32_t)dualsine12bit;
  dma_init_struct.memory_data_width = DMA_MEMORY_DATA_WIDTH_WORD;
  dma_init_struct.memory_inc_enable = TRUE;
  dma_init_struct.peripheral_base_addr = (uint32_t)0x40007420;
  dma_init_struct.peripheral_data_width = DMA_PERIPHERAL_DATA_WIDTH_WORD;
  dma_init_struct.peripheral_inc_enable = FALSE;
  dma_init_struct.priority = DMA_PRIORITY_MEDIUM;
  dma_init_struct.loop_mode_enable = TRUE;
  dma_init(DMA1_CHANNEL1, &dma_init_struct);

  /* enable dma flexible function */
  dma_flexible_config(DMA1, FLEX_CHANNEL1, DMA_FLEXIBLE_DAC2);

  dma_channel_enable(DMA1_CHANNEL1, TRUE);

  /* enable dac1: once the dac1 is enabled, pa.04 is
     automatically connected to the dac converter. */
  dac_enable(DAC1_SELECT, TRUE);

  /* enable dac2: once the dac2 is enabled, pa.05 is
     automatically connected to the dac converter. */
  dac_enable(DAC2_SELECT, TRUE);

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

