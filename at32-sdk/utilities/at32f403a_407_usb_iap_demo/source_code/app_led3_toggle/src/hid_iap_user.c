/**
  **************************************************************************
  * @file     hid_iap_user.c
  * @version  v2.0.8
  * @date     2022-04-02
  * @brief    usb hid iap user file
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

#include "hid_iap_user.h"
#include "hid_iap_class.h"
#include "string.h"

/** @addtogroup UTILITIES_examples
  * @{
  */

/** @addtogroup USB_IAP_app_led3_toggle
  * @{
  */

void (*pftarget)(void);
void iap_clear_upgrade_flag(void);
void iap_set_upgrade_flag(void);
uint32_t crc_cal(uint32_t addr, uint16_t nk);

void iap_idle(void);
void iap_start(void);
void iap_finish(void);
void iap_jump(void);
void iap_respond(uint8_t *res_buf, uint16_t iap_cmd, uint16_t result);

/**
  * @brief  clear iap upgrade flag
  * @param  none
  * @retval none
  */
void iap_clear_upgrade_flag(void)
{
  flash_unlock();
  flash_sector_erase(iap_info.flag_address);
  flash_lock();
}

/**
  * @brief  set iap upgrade complete flag
  * @param  none
  * @retval none
  */
void iap_set_upgrade_flag(void)
{
  flash_unlock();
  flash_word_program(iap_info.flag_address, IAP_UPGRADE_COMPLETE_FLAG);
  flash_lock();
}

/**
  * @brief  get iap upgrade complete flag
  * @param  none
  * @retval the status of the iap flag
  */
iap_result_type iap_get_upgrade_flag(void)
{
  uint32_t flag_address = iap_info.flag_address;
  if(*((uint32_t *)flag_address) == IAP_UPGRADE_COMPLETE_FLAG)
  {
    return IAP_SUCCESS;
  }
  else
  {
    return IAP_FAILED;
  }
}

/**
  * @brief  iap init
  * @param  none
  * @retval none
  */
void iap_init(void)
{
  iap_info.flash_size = KB_TO_B(FLASH_SIZE_REG());
  if(iap_info.flash_size  < KB_TO_B(256))
  {
    iap_info.sector_size = SECTOR_SIZE_1K;
  }
  else
  {
    iap_info.sector_size = SECTOR_SIZE_2K;
  }

  iap_info.flash_start_address = FLASH_BASE;
  iap_info.flash_end_address = iap_info.flash_start_address + iap_info.flash_size;

  iap_info.app_address = FLASH_APP_ADDRESS;
  iap_info.flag_address = iap_info.app_address -  iap_info.sector_size;

  iap_info.fifo_length = 0;
  iap_info.iap_address = 0;
}

/**
  * @brief  iap idle function
  * @param  none
  * @retval none
  */
void iap_idle(void)
{
  iap_info.state = IAP_STS_JMP_WAIT;
  iap_init();
  iap_respond(iap_info.iap_tx, IAP_CMD_IDLE, IAP_ACK);
}

/**
  * @brief  iap start function
  * @param  none
  * @retval none
  */
void iap_start(void)
{
  iap_info.state = IAP_STS_START;
  iap_init();
  iap_respond(iap_info.iap_tx, IAP_CMD_START, IAP_ACK);
}

/*
  * @brief  iap jump
  * @param  none
  * @retval none
  */
void iap_jump()
{
  iap_info.state = IAP_STS_JMP_WAIT;
  iap_respond(iap_info.iap_tx, IAP_CMD_JMP, IAP_ACK);
}

/*
  * @brief  iap get
  * @param  none
  * @retval none
  */
void iap_get()
{
  iap_respond(iap_info.iap_tx, IAP_CMD_GET, IAP_ACK);
  iap_info.iap_tx[4] = (uint8_t)((iap_info.app_address >> 24) & 0xFF);
  iap_info.iap_tx[5] = (uint8_t)((iap_info.app_address >> 16) & 0xFF);
  iap_info.iap_tx[6] = (uint8_t)((iap_info.app_address >> 8) & 0xFF);
  iap_info.iap_tx[7] = (uint8_t)((iap_info.app_address) & 0xFF);
}



/*
  * @brief  iap respond
  * @param  res_buf: data buffer pointer.
  * @param  iap_cmd: iap command
  * @param  result: iap result
  * @retval the result of the address parse
  */
void iap_respond(uint8_t *res_buf, uint16_t iap_cmd, uint16_t result)
{
  res_buf[0] = (uint8_t)((iap_cmd >> 8) & 0xFF);
  res_buf[1] = (uint8_t)((iap_cmd) & 0xFF);
  res_buf[2] = (uint8_t)((result >> 8) & 0xFF);
  res_buf[3] = (uint8_t)((result) & 0xFF);

  iap_info.respond_flag = 1;
}

/**
  * @brief  usb device iap function
  * @param  udev: to the structure of usbd_core_type
  * @param  pdata: data buffer point
  * @param  len: data length
  * @retval iap_result_type
  */
iap_result_type usbd_hid_iap_process(void *udev, uint8_t *pdata, uint16_t len)
{
  iap_result_type status = IAP_SUCCESS;
  uint16_t iap_cmd;

  if(len < 2)
  {
    return IAP_FAILED;
  }

  iap_info.respond_flag = 0;

  iap_cmd = (pdata[0] << 8) | pdata[1];

  switch(iap_cmd)
  {
    case IAP_CMD_IDLE:
      iap_idle();
      break;
    case IAP_CMD_START:
      iap_start();
      break;
    case IAP_CMD_JMP:
      iap_jump();
      break;
    case IAP_CMD_GET:
      iap_get();
      break;
    default:
      status = IAP_FAILED;
      break;
  }

  if(iap_info.respond_flag)
  {
    usb_iap_class_send_report(udev, iap_info.iap_tx, 64);
  }

  return status;
}

/**
  * @brief  usb device in transfer complete function
  * @param  udev: to the structure of usbd_core_type
  * @retval none
  */
void usbd_hid_iap_in_complete(void *udev)
{
  if(iap_info.state == IAP_STS_JMP_WAIT)
  {
    iap_info.state = IAP_STS_JMP;
  }
}

/**
  * @brief  iap loop
  * @param  none
  * @retval none
  */
void app_loop(void)
{
  if(iap_info.state == IAP_STS_JMP)
  {
    iap_clear_upgrade_flag();
    nvic_system_reset();
  }
}

/**
  * @}
  */

/**
  * @}
  */
