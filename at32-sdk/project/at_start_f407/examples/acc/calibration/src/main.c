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
#include "usbd_core.h"
#include "cdc_class.h"
#include "cdc_desc.h"
#include "usbd_int.h"



/** @addtogroup AT32F407_periph_examples
  * @{
  */

/** @addtogroup 407_ACC_calibration ACC_calibration
  * @{
  */

usbd_core_type usb_core_dev;
uint8_t usb_buffer[256];

/* select system clock source from hext or hick */
//#define   SCLK_FROM_HEXT
#define   SCLK_FROM_HICK

/* select acc calibration mode */
//#define   ACC_CAL
#define     ACC_TRIM

void system_clock_config_for_acc(void);
void clkout_config(void);

uint32_t acc_c2_value = 0;


/**
  * @brief  usb 48M clock select
  * @param  clk_s:USB_CLK_HICK, USB_CLK_HEXT
  * @retval none
  */
void usb_clock48m_select(usb_clk48_s clk_s)
{
  if(clk_s == USB_CLK_HICK)
  {
    crm_usb_clock_source_select(CRM_USB_CLOCK_SOURCE_HICK);
  }
  else
  {
    switch(system_core_clock)
    {
      /* 48MHz */
      case 48000000:
        crm_usb_clock_div_set(CRM_USB_DIV_1);
        break;

      /* 72MHz */
      case 72000000:
        crm_usb_clock_div_set(CRM_USB_DIV_1_5);
        break;

      /* 96MHz */
      case 96000000:
        crm_usb_clock_div_set(CRM_USB_DIV_2);
        break;

      /* 120MHz */
      case 120000000:
        crm_usb_clock_div_set(CRM_USB_DIV_2_5);
        break;

      /* 144MHz */
      case 144000000:
        crm_usb_clock_div_set(CRM_USB_DIV_3);
        break;

      /* 168MHz */
      case 168000000:
        crm_usb_clock_div_set(CRM_USB_DIV_3_5);
        break;

      /* 192MHz */
      case 192000000:
        crm_usb_clock_div_set(CRM_USB_DIV_4);
        break;

      default:
        crm_usb_clock_source_select(CRM_USB_CLOCK_SOURCE_HICK);
        break;
    }
  }
}

/**
  * @brief  usbfs_l_can1_rx0 interrupt handler
  * @param  none
  * @retval none
  */
void USBFS_L_CAN1_RX0_IRQHandler(void)
{
  usbd_irq_handler(&usb_core_dev);
}

/**
  * @brief  acc interrupt handler
  * @param  none
  * @retval none
  */
void ACC_IRQHandler(void)
{
  if(acc_flag_get(ACC_CALRDY_FLAG) != RESET)
  {
    at32_led_toggle(LED2);

    /* clear acc calibration ready flag */
    acc_flag_clear(ACC_CALRDY_FLAG);
  }
  if(acc_flag_get(ACC_RSLOST_FLAG) != RESET)
  {
    at32_led_toggle(LED3);

    /* clear acc reference signal lost flag */
    acc_flag_clear(ACC_RSLOST_FLAG);
  }
}

/**
  * @brief  system clock config for acc
  * @param  none
  * @retval none
  */
void system_clock_config_for_acc(void)
{
  /* reset crm */
  crm_reset();

  #ifdef SCLK_FROM_HICK
  crm_clock_source_enable(CRM_CLOCK_SOURCE_HICK, TRUE);
  #elif defined SCLK_FROM_HEXT
  crm_clock_source_enable(CRM_CLOCK_SOURCE_HEXT, TRUE);
  #endif

  /* wait till hick is ready */
  while(SET != crm_flag_get(CRM_HICK_STABLE_FLAG))
  {
  }

  /* config pll clock resource */
  crm_pll_config(CRM_PLL_SOURCE_HICK, CRM_PLL_MULT_60, CRM_PLL_OUTPUT_RANGE_GT72MHZ);

  /* enable pll */
  crm_clock_source_enable(CRM_CLOCK_SOURCE_PLL, TRUE);

  /* wait till pll is ready */
  while(SET != crm_flag_get(CRM_PLL_STABLE_FLAG))
  {
  }

  /* config ahbclk */
  crm_ahb_div_set(CRM_AHB_DIV_1);

  /* config apb2clk */
  crm_apb2_div_set(CRM_APB2_DIV_2);

  /* config apb1clk */
  crm_apb1_div_set(CRM_APB1_DIV_2);

  /* enable auto step mode */
  crm_auto_step_mode_enable(TRUE);

  /* select pll as system clock source */
  crm_sysclk_switch(CRM_SCLK_PLL);

  /* wait till pll is used as system clock source */
  while(CRM_SCLK_PLL != crm_sysclk_switch_status_get())
  {
  }

  /* disable auto step mode */
  crm_auto_step_mode_enable(FALSE);

  /* update system_core_clock global variable */
  system_core_clock_update();

  /* pa8 output usb clk */
  clkout_config();
}

/**
  * @brief  clkout configuration
  * @param  none
  * @retval none
  */
void clkout_config(void)
{
  gpio_init_type gpio_init_struct;

  /* enable gpio port clock */
  crm_periph_clock_enable(CRM_GPIOA_PERIPH_CLOCK, TRUE);

  /* set default parameter */
  gpio_default_para_init(&gpio_init_struct);

  /* clkout gpio init */
  gpio_init_struct.gpio_drive_strength = GPIO_DRIVE_STRENGTH_STRONGER;
  gpio_init_struct.gpio_mode = GPIO_MODE_MUX;
  gpio_init_struct.gpio_pins = GPIO_PINS_8;
  gpio_init_struct.gpio_pull = GPIO_PULL_NONE;
  gpio_init(GPIOA, &gpio_init_struct);

  /* config clkout division */
  crm_clkout_div_set(CRM_CLKOUT_DIV_1);

  /* config clkout clock */
  crm_clock_out_set(CRM_CLKOUT_USB);
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
  * @brief  main function.
  * @param  none
  * @retval none
  */
int main(void)
{
  uint16_t data_len;

  uint32_t timeout;

  uint8_t send_zero_packet = 0;

  /* config nvic priority group */
  nvic_priority_group_config(NVIC_PRIORITY_GROUP_4);

  /* sclk = 240mhz from hick */
  system_clock_config_for_acc();

  at32_board_init();

  usb_clock48m_select(USB_CLK_HICK);

  crm_periph_clock_enable(CRM_USB_PERIPH_CLOCK, TRUE);

  nvic_irq_enable(USBFS_L_CAN1_RX0_IRQn, 1, 0);

  usbd_core_init(&usb_core_dev, USB, &cdc_class_handler, &cdc_desc_handler, 0);

  usbd_connect(&usb_core_dev);

  /* acc config */
  crm_periph_clock_enable(CRM_ACC_PERIPH_CLOCK, TRUE);

  /* enable the acc calibration ready interrupt */
  acc_interrupt_enable(ACC_CALRDYIEN_INT, TRUE);

  /* enable the acc reference signal lost interrupt */
  acc_interrupt_enable(ACC_EIEN_INT, TRUE);

  /* config nvic for acc int  */
  nvic_irq_enable(ACC_IRQn, 2, 0);

  /* update the c1\c2\c3 value */
  acc_c2_value = 8000;
  #ifdef ACC_CAL
  acc_write_c1(acc_c2_value - 20);
  acc_write_c2(acc_c2_value);
  acc_write_c3(acc_c2_value + 20);
  #else
  acc_write_c1(acc_c2_value - 10);
  acc_write_c2(acc_c2_value);
  acc_write_c3(acc_c2_value + 10);
  #endif

  /* open acc calibration */
  #if defined(ACC_CAL)
    acc_calibration_mode_enable(ACC_CAL_HICKCAL, TRUE);
  #elif defined(ACC_TRIM)
    acc_calibration_mode_enable(ACC_CAL_HICKTRIM, TRUE);
  #endif

  while(1)
  {
    /* get usb vcp receive data */
    data_len = usb_vcp_get_rxdata(&usb_core_dev, usb_buffer);

    if(data_len > 0 || send_zero_packet == 1)
    {

      /* bulk transfer is complete when the endpoint does one of the following
         1 has transferred exactly the amount of data expected
         2 transfers a packet with a payload size less than wMaxPacketSize or transfers a zero-length packet
      */
      if(data_len > 0)
        send_zero_packet = 1;

      if(data_len == 0)
        send_zero_packet = 0;

      timeout = 5000000;
      do
      {
        /* send data to host */
        if(usb_vcp_send_data(&usb_core_dev, usb_buffer, data_len) == SUCCESS)
        {
          break;
        }
      }while(timeout --);
    }
  }
}

/**
  * @}
  */

/**
  * @}
  */
