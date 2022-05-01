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

/** @addtogroup 407_ADC_triple_adc_synchro_trigger ADC_triple_adc_synchro_trigger
  * @{
  */

__IO uint32_t adc1_ordinary_valuetab[5][3];
__IO uint16_t adc3_ordinary_valuetab[5][3];
__IO uint32_t dma1_trans_complete_flag = 0;
__IO uint32_t dma2_trans_complete_flag = 0;

static void gpio_config(void);
static void dma_config(void);
static void tmr1_config(void);
static void adc_config(void);

/**
  * @brief  gpio configuration.
  * @param  none
  * @retval none
  */
static void gpio_config(void)
{
  gpio_init_type gpio_initstructure;
  crm_periph_clock_enable(CRM_GPIOA_PERIPH_CLOCK, TRUE);
  crm_periph_clock_enable(CRM_GPIOB_PERIPH_CLOCK, TRUE);
  crm_periph_clock_enable(CRM_GPIOC_PERIPH_CLOCK, TRUE);

  gpio_default_para_init(&gpio_initstructure);
  gpio_initstructure.gpio_mode = GPIO_MODE_ANALOG;
  gpio_initstructure.gpio_pins = GPIO_PINS_4 | GPIO_PINS_5 | GPIO_PINS_6 | GPIO_PINS_7;
  gpio_init(GPIOA, &gpio_initstructure);

  gpio_initstructure.gpio_mode = GPIO_MODE_ANALOG;
  gpio_initstructure.gpio_pins = GPIO_PINS_0 | GPIO_PINS_1;
  gpio_init(GPIOB, &gpio_initstructure);

  gpio_initstructure.gpio_mode = GPIO_MODE_ANALOG;
  gpio_initstructure.gpio_pins = GPIO_PINS_0 | GPIO_PINS_1 | GPIO_PINS_2;
  gpio_init(GPIOC, &gpio_initstructure);
}

/**
  * @brief  dma configuration.
  * @param  none
  * @retval none
  */
static void dma_config(void)
{
  dma_init_type dma_init_struct;
  crm_periph_clock_enable(CRM_DMA1_PERIPH_CLOCK, TRUE);
  crm_periph_clock_enable(CRM_DMA2_PERIPH_CLOCK, TRUE);
  nvic_irq_enable(DMA1_Channel1_IRQn, 0, 0);
  nvic_irq_enable(DMA2_Channel4_5_IRQn, 0, 0);
  dma_reset(DMA1_CHANNEL1);
  dma_reset(DMA2_CHANNEL5);
  dma_default_para_init(&dma_init_struct);
  dma_init_struct.buffer_size = 15;
  dma_init_struct.direction = DMA_DIR_PERIPHERAL_TO_MEMORY;
  dma_init_struct.memory_base_addr = (uint32_t)adc1_ordinary_valuetab;
  dma_init_struct.memory_data_width = DMA_MEMORY_DATA_WIDTH_WORD;
  dma_init_struct.memory_inc_enable = TRUE;
  dma_init_struct.peripheral_base_addr = (uint32_t)&(ADC1->odt);
  dma_init_struct.peripheral_data_width = DMA_PERIPHERAL_DATA_WIDTH_WORD;
  dma_init_struct.peripheral_inc_enable = FALSE;
  dma_init_struct.priority = DMA_PRIORITY_HIGH;
  dma_init_struct.loop_mode_enable = FALSE;
  dma_init(DMA1_CHANNEL1, &dma_init_struct);

  dma_init_struct.memory_base_addr = (uint32_t)adc3_ordinary_valuetab;
  dma_init_struct.memory_data_width = DMA_MEMORY_DATA_WIDTH_HALFWORD;
  dma_init_struct.peripheral_base_addr = (uint32_t)&(ADC3->odt);
  dma_init_struct.peripheral_data_width = DMA_PERIPHERAL_DATA_WIDTH_HALFWORD;
  dma_init(DMA2_CHANNEL5, &dma_init_struct);

  dma_interrupt_enable(DMA1_CHANNEL1, DMA_FDT_INT, TRUE);
  dma_interrupt_enable(DMA2_CHANNEL5, DMA_FDT_INT, TRUE);
  dma_channel_enable(DMA1_CHANNEL1, TRUE);
  dma_channel_enable(DMA2_CHANNEL5, TRUE);
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

  /* (systemclock/(systemclock/10000))/1000 = 10Hz(100ms) */
  tmr_base_init(TMR1, 999, (crm_clocks_freq_struct.sclk_freq/10000 - 1));
  tmr_cnt_dir_set(TMR1, TMR_COUNT_UP);
  tmr_clock_source_div_set(TMR1, TMR_CLOCK_DIV1);
  tmr_primary_mode_select(TMR1, TMR_PRIMARY_SEL_OVERFLOW);
}

/**
  * @brief  adc configuration.
  * @param  none
  * @retval none
  */
static void adc_config(void)
{
  adc_base_config_type adc_base_struct;
  crm_periph_clock_enable(CRM_ADC1_PERIPH_CLOCK, TRUE);
  crm_periph_clock_enable(CRM_ADC2_PERIPH_CLOCK, TRUE);
  crm_periph_clock_enable(CRM_ADC3_PERIPH_CLOCK, TRUE);
  crm_adc_clock_div_set(CRM_ADC_DIV_6);

  /* select combine mode */
  adc_combine_mode_select(ADC_ORDINARY_SMLT_ONLY_MODE);
  adc_base_default_para_init(&adc_base_struct);
  adc_base_struct.sequence_mode = TRUE;
  adc_base_struct.repeat_mode = FALSE;
  adc_base_struct.data_align = ADC_RIGHT_ALIGNMENT;
  adc_base_struct.ordinary_channel_length = 3;
  adc_base_config(ADC1, &adc_base_struct);
  adc_ordinary_channel_set(ADC1, ADC_CHANNEL_4, 1, ADC_SAMPLETIME_239_5);
  adc_ordinary_channel_set(ADC1, ADC_CHANNEL_5, 2, ADC_SAMPLETIME_239_5);
  adc_ordinary_channel_set(ADC1, ADC_CHANNEL_6, 3, ADC_SAMPLETIME_239_5);
  adc_ordinary_conversion_trigger_set(ADC1, ADC12_ORDINARY_TRIG_TMR1TRGOUT, TRUE);
  adc_dma_mode_enable(ADC1, TRUE);

  adc_base_config(ADC2, &adc_base_struct);
  adc_ordinary_channel_set(ADC2, ADC_CHANNEL_7, 1, ADC_SAMPLETIME_239_5);
  adc_ordinary_channel_set(ADC2, ADC_CHANNEL_8, 2, ADC_SAMPLETIME_239_5);
  adc_ordinary_channel_set(ADC2, ADC_CHANNEL_9, 3, ADC_SAMPLETIME_239_5);
  adc_ordinary_conversion_trigger_set(ADC2, ADC12_ORDINARY_TRIG_SOFTWARE, TRUE);

  adc_base_config(ADC3, &adc_base_struct);
  adc_ordinary_channel_set(ADC3, ADC_CHANNEL_10, 1, ADC_SAMPLETIME_239_5);
  adc_ordinary_channel_set(ADC3, ADC_CHANNEL_11, 2, ADC_SAMPLETIME_239_5);
  adc_ordinary_channel_set(ADC3, ADC_CHANNEL_12, 3, ADC_SAMPLETIME_239_5);
  adc_ordinary_conversion_trigger_set(ADC3, ADC3_ORDINARY_TRIG_TMR1TRGOUT, TRUE);
  adc_dma_mode_enable(ADC3, TRUE);

  adc_enable(ADC1, TRUE);
  adc_enable(ADC2, TRUE);
  adc_calibration_init(ADC1);
  while(adc_calibration_init_status_get(ADC1));
  adc_calibration_start(ADC1);
  while(adc_calibration_status_get(ADC1));
  adc_calibration_init(ADC2);
  while(adc_calibration_init_status_get(ADC2));
  adc_calibration_start(ADC2);
  while(adc_calibration_status_get(ADC2));

  adc_enable(ADC3, TRUE);
  adc_calibration_init(ADC3);
  while(adc_calibration_init_status_get(ADC3));
  adc_calibration_start(ADC3);
  while(adc_calibration_status_get(ADC3));
}

/**
  * @brief  main function.
  * @param  none
  * @retval none
  */
int main(void)
{
  __IO uint32_t index = 0;
  nvic_priority_group_config(NVIC_PRIORITY_GROUP_4);
  system_clock_config();
  at32_board_init();
  at32_led_off(LED2);
  at32_led_off(LED3);
  at32_led_off(LED4);
  uart_print_init(115200);
  gpio_config();
  tmr1_config();
  dma_config();
  adc_config();

  printf("triple_adc_synchro_trigger \r\n");

  tmr_counter_enable(TMR1, TRUE);
  while(dma1_trans_complete_flag == 0);
  while(dma2_trans_complete_flag == 0);
  tmr_counter_enable(TMR1, FALSE);

  for(index = 0; index < 5; index++)
  {
    printf("adc1_ordinary_valuetab[%d][0] = 0x%x\r\n", index, adc1_ordinary_valuetab[index][0]);
    printf("adc1_ordinary_valuetab[%d][1] = 0x%x\r\n", index, adc1_ordinary_valuetab[index][1]);
    printf("adc1_ordinary_valuetab[%d][2] = 0x%x\r\n", index, adc1_ordinary_valuetab[index][2]);
    printf("adc3_ordinary_valuetab[%d][0] = 0x%x\r\n", index, adc3_ordinary_valuetab[index][0]);
    printf("adc3_ordinary_valuetab[%d][1] = 0x%x\r\n", index, adc3_ordinary_valuetab[index][1]);
    printf("adc3_ordinary_valuetab[%d][2] = 0x%x\r\n", index, adc3_ordinary_valuetab[index][2]);
    printf("\r\n");
  }

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
