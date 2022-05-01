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
#include "msc_class.h"
#include "msc_desc.h"
#include "usbd_int.h"
#include "flash_fat16.h"

/** @addtogroup AT32F403A_periph_examples
  * @{
  */

/** @addtogroup 403A_USB_device_virtual_msc_iap USB_device_virtual_msc_iap
  * @{
  */

usbd_core_type usb_core_dev;

void (*pftarget)(void);

void jump_to_app(uint32_t address);


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

  flash_fat16_init();

  at32_board_init();

  if(flash_fat16_get_upgrade_flag() == IAP_SUCCESS &&
  	(at32_button_press() == NO_BUTTON))
  {
    jump_to_app(flash_iap.flash_app_addr);
  }

  /* select usb 48m clcok source */
  usb_clock48m_select(USB_CLK_HEXT);

  /* enable usb clock */
  crm_periph_clock_enable(CRM_USB_PERIPH_CLOCK, TRUE);

  /* enable usb interrupt */
  nvic_irq_enable(USBFS_L_CAN1_RX0_IRQn, 0, 0);

  /* usb core init */
  usbd_core_init(&usb_core_dev, USB, &msc_class_handler, &msc_desc_handler, 0);

  /* enable usb pull-up */
  usbd_connect(&usb_core_dev);

  while(1)
  {
    flash_fat16_loop_status();
  }
}

/**
  * @brief  usb re-connect
  * @param  none
  * @retval none
  */
void usb_reset()
{
  delay_ms(1000);
  usbd_disconnect(&usb_core_dev);
  delay_ms(1000);
  usbd_connect(&usb_core_dev);
}

/**
  * @brief  loop status
  * @param  none
  * @retval none
  */
void flash_fat16_loop_status(void)
{
  static uint8_t status = UPGRADE_READY;
  if(status == flash_iap.msc_up_status)
  {
    return;
  }
  else
  {
    status = flash_iap.msc_up_status;
  }

  switch(flash_iap.msc_up_status)
  {
    case UPGRADE_READY:
      flash_fat16_set_name((const uint8_t *)"Ready", 5);
      break;
    case UPGRAGE_ONGOING:
      break;
    case UPGRADE_SUCCESS:
      flash_fat16_set_name((const uint8_t *)"Success", 7);
      flash_iap.msc_up_status = UPGRADE_DONE;
      usb_reset();
      break;
    case UPGRADE_FAILED:
      flash_fat16_set_name((const uint8_t *)"Failed", 6);
      flash_iap.msc_up_status = UPGRADE_DONE;
      usb_reset();
      break;
    case UPGRADE_LARGE:
      flash_fat16_set_name((const uint8_t *)"Large", 5);
      flash_iap.msc_up_status = UPGRADE_DONE;
      usb_reset();
      break;
    case UPGRADE_UNKNOWN:
      flash_fat16_set_name((const uint8_t *)"Unkown", 6);
      break;
    case UPGRADE_DONE:
      break;
    case UPGRADE_JUMP:
      flash_fat16_set_name((const uint8_t *)"Jump", 6);
      delay_ms(1000);
      jump_to_app(flash_iap.write_addr);
      break;
    default:
      break;
  }

}

/**
  * @brief  jump to app
  * @param  none
  * @retval none
  */
void jump_to_app(uint32_t address)
{
  uint32_t stkptr, jumpaddr;
  stkptr = *(uint32_t *)address;
  jumpaddr = *(uint32_t *)(address + sizeof(uint32_t));

  /* clear usb crm */
  crm_periph_clock_enable(CRM_USB_PERIPH_CLOCK, FALSE);
  crm_periph_clock_enable(CRM_USB_PERIPH_CLOCK, FALSE);

  crm_periph_reset(CRM_USB_PERIPH_RESET, TRUE);
  crm_periph_reset(CRM_USB_PERIPH_RESET, FALSE);

  __set_MSP(stkptr);
  pftarget = (void (*) (void))jumpaddr;
  pftarget();
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
