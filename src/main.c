/**
  **************************************************************************
  * @file     main.c
  * @version  v2.0.6
  * @date     2021-12-31
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
#include "usbd_core.h"
#include "cdc_class.h"
#include "cdc_desc.h"
#include "usbd_int.h"

// __IO uint32_t dma_trans_complete_flag;

/** @addtogroup AT32F403A_periph_examples
  * @{
  */

/** @addtogroup 403A_USB_device_vcp_loopback USB_device_vcp_loopback
  * @{
  */

// usbd_class_handler class_handler;
// usbd_desc_handler desc_handler;
usbd_core_type usb_core_dev;
uint8_t usb_buffer[4096];

/**
  * @brief  usb 48M clock select
  * @param  clk_s:USB_CLK_HICK, USB_CLK_HEXT
  * @retval none
  */
void usb_clock48m_select(usb_clk48_s clk_s)
{
  crm_usb_clock_source_select(CRM_USB_CLOCK_SOURCE_HICK);

  /* enable the acc calibration ready interrupt */
  crm_periph_clock_enable(CRM_ACC_PERIPH_CLOCK, TRUE);

  /* update the c1\c2\c3 value */
  acc_write_c1(7980);
  acc_write_c2(8000);
  acc_write_c3(8020);

  /* open acc calibration */
  acc_calibration_mode_enable(ACC_CAL_HICKTRIM, TRUE);
}

__IO uint16_t adc1_ordinary_valuetab[1024][2] = {0};
// __IO uint16_t adc1_preempt_valuetab[512][2] = {0};
__IO uint16_t dma_trans_complete_flag = 0;
// __IO uint16_t preempt_conversion_count = 0;

static void gpio_config(void);
static void dma_config(void);
static void adc_config(void);

/**
  * @brief  dma configuration.
  * @param  none
  * @retval none
  */
static void dma_config(void)
{
  dma_init_type dma_init_struct;
  crm_periph_clock_enable(CRM_DMA1_PERIPH_CLOCK, TRUE);
  nvic_irq_enable(DMA1_Channel1_IRQn, 0, 0);
  dma_reset(DMA1_CHANNEL1);
  dma_default_para_init(&dma_init_struct);
  dma_init_struct.buffer_size = 2048;
  dma_init_struct.direction = DMA_DIR_PERIPHERAL_TO_MEMORY;
  dma_init_struct.memory_base_addr = (uint16_t *)adc1_ordinary_valuetab;
  dma_init_struct.memory_data_width = DMA_MEMORY_DATA_WIDTH_HALFWORD;
  dma_init_struct.memory_inc_enable = TRUE;
  dma_init_struct.peripheral_base_addr = (uint32_t)&(ADC1->odt);
  dma_init_struct.peripheral_data_width = DMA_MEMORY_DATA_WIDTH_HALFWORD;
  dma_init_struct.peripheral_inc_enable = FALSE;
  dma_init_struct.priority = DMA_PRIORITY_HIGH;
  dma_init_struct.loop_mode_enable = TRUE;
  dma_init(DMA1_CHANNEL1, &dma_init_struct);

  dma_interrupt_enable(DMA1_CHANNEL1, DMA_FDT_INT, TRUE);
  dma_channel_enable(DMA1_CHANNEL1, TRUE);
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
  crm_adc_clock_div_set(CRM_ADC_DIV_2);

  adc_combine_mode_select(ADC_INDEPENDENT_MODE);
  adc_base_default_para_init(&adc_base_struct);
  adc_base_struct.sequence_mode = TRUE;
  adc_base_struct.repeat_mode = TRUE;
  adc_base_struct.data_align = ADC_RIGHT_ALIGNMENT;
  adc_base_struct.ordinary_channel_length = 2;

  adc_base_config(ADC1, &adc_base_struct);


  // opamp output stage 1?
  adc_ordinary_channel_set(ADC1, ADC_CHANNEL_6, 1, ADC_SAMPLETIME_71_5);
  adc_ordinary_channel_set(ADC1, ADC_CHANNEL_7, 2, ADC_SAMPLETIME_71_5);

  // opamp output stage 2?
  // adc_ordinary_channel_set(ADC1, ADC_CHANNEL_8, 1, ADC_SAMPLETIME_71_5);
  // adc_ordinary_channel_set(ADC1, ADC_CHANNEL_9, 2, ADC_SAMPLETIME_71_5);

  adc_ordinary_conversion_trigger_set(ADC1, ADC12_ORDINARY_TRIG_SOFTWARE, TRUE);
  adc_dma_mode_enable(ADC1, TRUE);

  adc_enable(ADC1, TRUE);
  adc_calibration_init(ADC1);
  while(adc_calibration_init_status_get(ADC1));
  adc_calibration_start(ADC1);
  while(adc_calibration_status_get(ADC1));
}


void init_gpio() {

  crm_periph_clock_enable(CRM_GPIOA_PERIPH_CLOCK, TRUE);
  crm_periph_clock_enable(CRM_GPIOB_PERIPH_CLOCK, TRUE);

  gpio_init_type gpio_initstructure;

  gpio_default_para_init(&gpio_initstructure);
  gpio_init(GPIOA, &gpio_initstructure);

  gpio_default_para_init(&gpio_initstructure);
  gpio_init(GPIOB, &gpio_initstructure);
}

int configure_gpio(uint32_t group, uint32_t pin, uint32_t mode) {
  gpio_init_type gpio_initstructure;
  gpio_default_para_init(&gpio_initstructure);

  gpio_initstructure.gpio_mode = mode;
  gpio_initstructure.gpio_pins = pin;

  switch(group) {
    case 0:
      gpio_init(GPIOA, &gpio_initstructure);
      break;
    case 1:
      gpio_init(GPIOB, &gpio_initstructure);
      break;
    default:
      return 1;
  }

  return 0;
}

#define CFG_GPIO_PIN 0x1000
#define CFG_DMA 0x1001
#define CFG_ADC 0x1002
#define TRIGGER_ADC 0x1003
#define READ_ADC 0x1004

struct usb_cmd_t {
  uint32_t cmd_code;
  uint32_t args[];
};


/**
  * @brief  main function.
  * @param  none
  * @retval none
  */
int main(void)
{
  system_clock_config();
  init_gpio();

  nvic_priority_group_config(NVIC_PRIORITY_GROUP_4);
  at32_board_init();
  usb_clock48m_select(USB_CLK_HICK);
  crm_periph_clock_enable(CRM_USB_PERIPH_CLOCK, TRUE);
  nvic_irq_enable(USBFS_L_CAN1_RX0_IRQn, 0, 0);
  usbd_core_init(&usb_core_dev, USB, &cdc_class_handler, &cdc_desc_handler, 0);
  usbd_connect(&usb_core_dev);

  int x;
  uint16_t data_len;
  uint32_t timeout;
  struct usb_cmd_t * cmd = (struct usb_cmd_t *)usb_buffer;

  while(1)
  {
    /* get usb vcp receive data */
    data_len = usb_vcp_get_rxdata(&usb_core_dev, usb_buffer);
    // data_len = 60;
    if(data_len >= 4) {
      switch(cmd->cmd_code) {
        case CFG_GPIO_PIN:
          x = configure_gpio(cmd->args[0], cmd->args[1], cmd->args[2]);
          if(data_len >= 20) {
            if(cmd->args[3] == 0) {
              gpio_bits_reset(cmd->args[0], cmd->args[1]);
            } else {
              gpio_bits_set(cmd->args[0], cmd->args[1]);
            }
          }
          cmd->args[0] = x;
          timeout = 50000;
          data_len = 8;
          do { timeout--; }
          while(usb_vcp_send_data(&usb_core_dev, usb_buffer, data_len) != SUCCESS);
          break;

        case CFG_DMA:
          data_len = 8;
          dma_config();
          cmd->args[0] = 0;
          timeout = 50000;
          do { timeout--; }
          while(usb_vcp_send_data(&usb_core_dev, usb_buffer, data_len) != SUCCESS);
          break;

        case CFG_ADC:
          data_len = 8;
          adc_config();
          cmd->args[0] = 0;
          timeout = 50000;
          do { timeout--; }
          while(usb_vcp_send_data(&usb_core_dev, usb_buffer, data_len) != SUCCESS);
          break;

        case TRIGGER_ADC:
          data_len = 8;
          adc_ordinary_software_trigger_enable(ADC1, TRUE);
          // adc_preempt_software_trigger_enable(ADC2, TRUE);
          cmd->args[0] = 0;
          timeout = 50000;
          do { timeout--; }
          while(usb_vcp_send_data(&usb_core_dev, usb_buffer, 8) != SUCCESS);
          break;

        case READ_ADC:

          while(1) {
            while(dma_trans_complete_flag == 0);
            // while(preempt_conversion_count < 2);

            // pack 12-bit I+Q
            for(x = 0; x < 1024; x++) {
              usb_buffer[x*3+0] = (adc1_ordinary_valuetab[x][0]>>4)&0xff;
              usb_buffer[x*3+1] = ((adc1_ordinary_valuetab[x][0]&0xf)<<4) | ((adc1_ordinary_valuetab[x][1]>>8)&0xf);
              usb_buffer[x*3+2] = (adc1_ordinary_valuetab[x][1]&0xff);
            }

            // memcpy(usb_buffer, adc1_ordinary_valuetab, 4096);
            // memcpy(usb_buffer, adc1_ordinary_valuetab, 3072);
            // memcpy(&usb_buffer[0], adc1_preempt_valuetab, 2048);
            // memcpy(&usb_buffer[2048], adc1_ordinary_valuetab, 2048);
            // memcpy(&usb_buffer[2048], adc1_preempt_valuetab, 2048);
            dma_trans_complete_flag = 0;
            timeout = 50000;
            do { timeout--; }
            while(usb_vcp_send_data(&usb_core_dev, usb_buffer, 3072) != SUCCESS);
          }


          break;



        default:
          // unhandled
          continue;
      }
    }
  }
}


/**
  * @brief  this function handles usb interrupt.
  * @param  none
  * @retval none
  */
void USBFS_L_CAN1_RX0_IRQHandler(void)
{
  usbd_irq_handler(&usb_core_dev);
}

/**
  * @brief  usb delay millisecond function.
  * @param  ms: number of millisecond delay
  * @retval none
  */
void usb_delay_ms(uint32_t ms)
{
  /* user can define self delay function */
  delay_ms(ms);
}

/**
  * @brief  usb delay microsecond function.
  * @param  us: number of microsecond delay
  * @retval none
  */
void usb_delay_us(uint32_t us)
{
  delay_us(us);
}

/**
  * @}
  */

/**
  * @}
  */
