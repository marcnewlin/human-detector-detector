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
#include "keyboard_class.h"
#include "keyboard_desc.h"
#include "usbd_int.h"

/** @addtogroup AT32F407_periph_examples
  * @{
  */

/** @addtogroup 407_USB_device_keyboard USB_device_keyboard
  * @{
  */

usbd_core_type usb_core_dev;
extern __IO uint8_t hid_suspend_flag;
void keyboard_send_string(void *udev, uint8_t *string, uint8_t len);
void system_clock_recover(void);
void button_exint_init(void);
void usb_low_power_wakeup_config(void);

/**
  * @brief  keyboard send string
  * @param  string: send string
  * @param  len: send string length
  * @retval none
  */
void keyboard_send_string(void *udev, uint8_t *string, uint8_t len)
{
  uint8_t index = 0;
  usbd_core_type *pudev = (usbd_core_type *)udev;
  keyboard_type *pkeyboard = (keyboard_type *)pudev->class_handler->pdata;
  for(index = 0; index < len; index ++)
  {
    while(1)
    {
      if(pkeyboard->g_u8tx_completed == 1)
      {
        pkeyboard->g_u8tx_completed = 0;
        usb_hid_keyboard_send_char(udev, string[index]);
        break;
      }
    }
    /* send 0x00 */
    while(1)
    {
      if(pkeyboard->g_u8tx_completed == 1)
      {
        pkeyboard->g_u8tx_completed = 0;
        usb_hid_keyboard_send_char(udev, 0x00);
        break;
      }
    }
  }
}

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

    /* enable the acc calibration ready interrupt */
    crm_periph_clock_enable(CRM_ACC_PERIPH_CLOCK, TRUE);

    /* update the c1\c2\c3 value */
    acc_write_c1(7980);
    acc_write_c2(8000);
    acc_write_c3(8020);

    /* open acc calibration */
    acc_calibration_mode_enable(ACC_CAL_HICKTRIM, TRUE);
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
        break;

    }
  }
}

/**
  * @brief  main function.
  * @param  none
  * @retval none
  */
int main(void)
{
  /* config nvic priority group */
  nvic_priority_group_config(NVIC_PRIORITY_GROUP_4);

  system_clock_config();

  at32_board_init();

  /* select usb 48m clcok source */
  usb_clock48m_select(USB_CLK_HEXT);

#ifdef USB_LOW_POWER_WAKUP
  /* enable pwc and bpr clock */
  crm_periph_clock_enable(CRM_PWC_PERIPH_CLOCK, TRUE);
  button_exint_init();
  usb_low_power_wakeup_config();
#endif

  /* enable usb clock */
  crm_periph_clock_enable(CRM_USB_PERIPH_CLOCK, TRUE);

  /* enable usb interrupt */
  nvic_irq_enable(USBFS_L_CAN1_RX0_IRQn, 0, 0);

  /* usb core init */
  usbd_core_init(&usb_core_dev, USB, &keyboard_class_handler, &keyboard_desc_handler, 0);

  /* enable usb pull-up */
  usbd_connect(&usb_core_dev);

  while(1)
  {
    if(at32_button_press() == USER_BUTTON)
    {
      if(usbd_connect_state_get(&usb_core_dev) == USB_CONN_STATE_CONFIGURED)
      {
        keyboard_send_string(&usb_core_dev, (uint8_t *)" Keyboard Demo\r\n", 16);
      }
      /* remote wakeup */
      if(usbd_connect_state_get(&usb_core_dev) == USB_CONN_STATE_SUSPENDED
        &&(usb_core_dev.remote_wakup == 1))
      {
        usbd_remote_wakeup(&usb_core_dev);
      }
    }
#ifdef USB_LOW_POWER_WAKUP
     /* enter deep sleep */
    if(((keyboard_type *)(usb_core_dev.class_handler->pdata))->hid_suspend_flag == 1)
    {
      at32_led_off(LED2);
      at32_led_off(LED3);
      at32_led_off(LED4);
      /* congfig the voltage regulator mode */
      pwc_voltage_regulate_set(PWC_REGULATOR_LOW_POWER);

      /* enter deep sleep mode */
      pwc_deep_sleep_mode_enter(PWC_DEEP_SLEEP_ENTER_WFI);
      /* wait clock stable */

      system_clock_recover();
      ((keyboard_type *)(usb_core_dev.class_handler->pdata))->hid_suspend_flag = 0;
      at32_led_on(LED2);
      at32_led_on(LED3);
      at32_led_on(LED4);
    }
#endif
  }
}

#ifdef USB_LOW_POWER_WAKUP
/**
  * @brief  configure button exint
  * @param  none
  * @retval none
  */
void button_exint_init(void)
{
  exint_init_type exint_init_struct;

  crm_periph_clock_enable(CRM_IOMUX_PERIPH_CLOCK, TRUE);
  gpio_exint_line_config(GPIO_PORT_SOURCE_GPIOA, GPIO_PINS_SOURCE0);

  exint_default_para_init(&exint_init_struct);
  exint_init_struct.line_enable = TRUE;
  exint_init_struct.line_mode = EXINT_LINE_INTERRUPUT;
  exint_init_struct.line_select = EXINT_LINE_0;
  exint_init_struct.line_polarity = EXINT_TRIGGER_RISING_EDGE;
  exint_init(&exint_init_struct);

  nvic_priority_group_config(NVIC_PRIORITY_GROUP_4);
  nvic_irq_enable(EXINT0_IRQn, 0, 0);
}

/**
  * @brief  exint0 interrupt handler
  * @param  none
  * @retval none
  */
void EXINT0_IRQHandler(void)
{
  /* clear interrupt pending bit */
  exint_flag_clear(EXINT_LINE_0);
}

/**
  * @brief  usb low power wakeup interrupt config
  * @param  none
  * @retval none
  */
void usb_low_power_wakeup_config(void)
{
  exint_init_type exint_init_struct;

  exint_default_para_init(&exint_init_struct);

  exint_init_struct.line_enable = TRUE;
  exint_init_struct.line_mode = EXINT_LINE_INTERRUPUT;
  exint_init_struct.line_select = EXINT_LINE_18;
  exint_init_struct.line_polarity = EXINT_TRIGGER_RISING_EDGE;
  exint_init(&exint_init_struct);

  nvic_irq_enable(USBFSWakeUp_IRQn, 0, 0);
}

/**
  * @brief  this function handles usbfs wakup interrupt.
  * @param  none
  * @retval none
  */
void USBFSWakeUp_IRQHandler(void)
{
  exint_flag_clear(EXINT_LINE_18);
}

/**
  * @brief  systemclock recover.
  * @param  none
  * @retval none
  */
void system_clock_recover(void)
{
  /* enable external high-speed crystal oscillator - hext */
  crm_clock_source_enable(CRM_CLOCK_SOURCE_HEXT, TRUE);

  /* wait till hext is ready */
  while(crm_hext_stable_wait() == ERROR);

  /* enable pll */
  crm_clock_source_enable(CRM_CLOCK_SOURCE_PLL, TRUE);

  /* wait till pll is ready */
  while(crm_flag_get(CRM_PLL_STABLE_FLAG) == RESET);

  /* enable auto step mode */
  crm_auto_step_mode_enable(TRUE);

  /* select pll as system clock source */
  crm_sysclk_switch(CRM_SCLK_PLL);

  /* wait till pll is used as system clock source */
  while(crm_sysclk_switch_status_get() != CRM_SCLK_PLL);
}

#endif
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
