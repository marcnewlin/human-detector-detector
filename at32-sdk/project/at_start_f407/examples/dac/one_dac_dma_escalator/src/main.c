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

/** @addtogroup 407_DAC_one_dac_dma_escalator DAC_one_dac_dma_escalator
  * @{
  */

gpio_init_type  gpio_init_struct = {0};
dma_init_type dma_init_struct = {0};
crm_clocks_freq_type crm_clocks_freq_struct = {0};
const uint8_t escalator8bit[6] = {0x0, 0x33, 0x66, 0x99, 0xCC, 0xFF};

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

  /* enable dac/dma1/gpioa/tmr2 clock */
  crm_periph_clock_enable(CRM_TMR2_PERIPH_CLOCK, TRUE);
  crm_periph_clock_enable(CRM_DMA1_PERIPH_CLOCK, TRUE);
  crm_periph_clock_enable(CRM_DAC_PERIPH_CLOCK, TRUE);
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

  /* (systemclock/(systemclock/1000000))/1000 = 1KHz */
  tmr_base_init(TMR2, 999, (crm_clocks_freq_struct.sclk_freq/1000000 - 1));
  tmr_cnt_dir_set(TMR2, TMR_COUNT_UP);

  /* primary tmr2 output selection */
  tmr_primary_mode_select(TMR2, TMR_PRIMARY_SEL_OVERFLOW);

  /* dac1 configuration */
  dac_trigger_select(DAC1_SELECT, DAC_TMR2_TRGOUT_EVENT);

  dac_trigger_enable(DAC1_SELECT, TRUE);

  dac_wave_generate(DAC1_SELECT, DAC_WAVE_GENERATE_NONE);

  dac_output_buffer_enable(DAC1_SELECT, FALSE);

  dac_dma_enable(DAC1_SELECT, TRUE);

  /* dma1 channel2 configuration */
  dma_reset(DMA1_CHANNEL2);
  dma_init_struct.buffer_size = 6;
  dma_init_struct.direction = DMA_DIR_MEMORY_TO_PERIPHERAL;
  dma_init_struct.memory_base_addr = (uint32_t)escalator8bit;
  dma_init_struct.memory_data_width = DMA_MEMORY_DATA_WIDTH_BYTE;
  dma_init_struct.memory_inc_enable = TRUE;
  dma_init_struct.peripheral_base_addr = (uint32_t)0x40007410;
  dma_init_struct.peripheral_data_width = DMA_PERIPHERAL_DATA_WIDTH_BYTE;
  dma_init_struct.peripheral_inc_enable = FALSE;
  dma_init_struct.priority = DMA_PRIORITY_MEDIUM;
  dma_init_struct.loop_mode_enable = TRUE;
  dma_init(DMA1_CHANNEL2, &dma_init_struct);

  /* enable dma flexible function */
  dma_flexible_config(DMA1, FLEX_CHANNEL2, DMA_FLEXIBLE_DAC1);

  dma_channel_enable(DMA1_CHANNEL2, TRUE);

  /* enable dac1: once the dac1 is enabled, pa.04 is
     automatically connected to the dac converter. */
  dac_enable(DAC1_SELECT, TRUE);

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

