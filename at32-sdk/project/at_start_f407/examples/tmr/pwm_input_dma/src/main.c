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

/** @addtogroup 407_TMR_pwm_input_dma TMR_pwm_input_dma
  * @{
  */

#define TMR1_C1DT_ADDRESS                0x40012C34
#define TMR1_C2DT_ADDRESS                0x40012C38

#define SAMPLING_NUM                     1000

tmr_input_config_type tmr_ic_init_structure;
dma_init_type dma_init_structure;

__IO uint16_t buffer_cc1[1000];
__IO uint16_t buffer_cc2[1000];

__IO uint32_t frequency = 0;
__IO float duty_cycle = 0;

uint32_t freq_result;
float duty_result;

void crm_configuration(void);
void gpio_configuration(void);

void DMA1_Channel2_IRQHandler(void)
{
  uint16_t i;

  dma_flag_clear(DMA1_FDT2_FLAG);

  duty_result = 0;
  freq_result = 0.0;

  for(i = 0; i < SAMPLING_NUM; i++)
  {
    duty_result += ((buffer_cc2[i] * 100.0) / buffer_cc1[i]);
    freq_result += (system_core_clock / buffer_cc1[i]);
  }

  duty_cycle = duty_result / SAMPLING_NUM;
  frequency = freq_result / SAMPLING_NUM;
  printf("\r\nFreq=%dHZ, Duty=%.2f%%\r\n", frequency, duty_cycle);
}

/**
  * @brief  configure the tmr1 pins.
  * @param  none
  * @retval none
  */
void gpio_configuration(void)
{
  gpio_init_type gpio_init_struct;

  gpio_default_para_init(&gpio_init_struct);

  gpio_init_struct.gpio_pins = GPIO_PINS_8;
  gpio_init_struct.gpio_pull = GPIO_PULL_NONE;
  gpio_init_struct.gpio_mode = GPIO_MODE_INPUT;
  gpio_init_struct.gpio_drive_strength = GPIO_DRIVE_STRENGTH_STRONGER;
  gpio_init(GPIOA, &gpio_init_struct);
}

/**
  * @brief  configures the different peripheral clocks.
  * @param  none
  * @retval none
  */
void crm_configuration(void)
{
  /* tmr1 clock enable */
  crm_periph_clock_enable(CRM_TMR1_PERIPH_CLOCK, TRUE);

  /* gpioa clock enable */
  crm_periph_clock_enable(CRM_GPIOA_PERIPH_CLOCK, TRUE);

  /* dma clock enable */
  crm_periph_clock_enable(CRM_DMA1_PERIPH_CLOCK, TRUE);
}

/**
  * @brief  main function.
  * @param  none
  * @retval none
  */
int main(void)
{
  system_clock_config();

  /* peripheral clocks configuration */
  crm_configuration();

  /* nvic configuration */
  nvic_priority_group_config(NVIC_PRIORITY_GROUP_4);
  nvic_irq_enable(DMA1_Channel2_IRQn, 0, 0);

  /* gpio configuration */
  gpio_configuration();

  uart_print_init(115200);

  dma_reset(DMA1_CHANNEL2);
  dma_default_para_init(&dma_init_structure);
  dma_init_structure.peripheral_base_addr    = (uint32_t)TMR1_C1DT_ADDRESS;
  dma_init_structure.memory_base_addr        = (uint32_t)buffer_cc1;
  dma_init_structure.direction               = DMA_DIR_PERIPHERAL_TO_MEMORY;
  dma_init_structure.buffer_size             = SAMPLING_NUM;
  dma_init_structure.peripheral_inc_enable   = FALSE;
  dma_init_structure.memory_inc_enable       = TRUE;
  dma_init_structure.peripheral_data_width   = DMA_PERIPHERAL_DATA_WIDTH_HALFWORD;
  dma_init_structure.memory_data_width       = DMA_MEMORY_DATA_WIDTH_HALFWORD;
  dma_init_structure.loop_mode_enable    = TRUE;
  dma_init_structure.priority                = DMA_PRIORITY_HIGH;
  dma_init(DMA1_CHANNEL2, &dma_init_structure);

  dma_reset(DMA1_CHANNEL3);
  dma_init_structure.peripheral_base_addr    = (uint32_t)TMR1_C2DT_ADDRESS;
  dma_init_structure.memory_base_addr        = (uint32_t)buffer_cc2;
  dma_init(DMA1_CHANNEL3, &dma_init_structure);

  dma_interrupt_enable(DMA1_CHANNEL2, DMA_FDT_INT, TRUE);

  dma_channel_enable(DMA1_CHANNEL2, TRUE);
  dma_channel_enable(DMA1_CHANNEL3, TRUE);

  tmr_input_default_para_init(&tmr_ic_init_structure);
  tmr_ic_init_structure.input_filter_value = 0;
  tmr_ic_init_structure.input_channel_select = TMR_SELECT_CHANNEL_1;
  tmr_ic_init_structure.input_mapped_select = TMR_CC_CHANNEL_MAPPED_DIRECT;
  tmr_ic_init_structure.input_polarity_select = TMR_INPUT_RISING_EDGE;

  tmr_pwm_input_config(TMR1, &tmr_ic_init_structure, TMR_CHANNEL_INPUT_DIV_1);

  /* select the TMR1 input trigger: C2IF2 */
  tmr_trigger_input_select(TMR1, TMR_SUB_INPUT_SEL_C1DF1);

  /* select the sub mode: reset mode */
  tmr_sub_mode_select(TMR1, TMR_SUB_RESET_MODE);

  /* enable the sub sync mode */
  tmr_sub_sync_mode_set(TMR1, TRUE);

  /* enable the c1/c2 dma */
  tmr_dma_request_enable(TMR1, TMR_C1_DMA_REQUEST, TRUE);
  tmr_dma_request_enable(TMR1, TMR_C2_DMA_REQUEST, TRUE);

  /* tmr enable counter */
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
