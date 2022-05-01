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

/** @addtogroup USB_IAP_bootloader
  * @{
  */

void (*pftarget)(void);
void iap_clear_upgrade_flag(void);
void iap_set_upgrade_flag(void);
uint32_t crc_cal(uint32_t addr, uint16_t nk);

void iap_idle(void);
void iap_start(void);
iap_result_type iap_address(uint8_t *pdata, uint32_t len);
void iap_finish(void);
iap_result_type iap_data_write(uint8_t *pdata, uint32_t len);
void iap_jump(void);
void iap_respond(uint8_t *res_buf, uint16_t iap_cmd, uint16_t result);

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

  /* disable nvic irq and periph clock, clear pending */
  nvic_irq_disable(USBFS_L_CAN1_RX0_IRQn);
  __NVIC_ClearPendingIRQ(USBFS_L_CAN1_RX0_IRQn);
  crm_periph_clock_enable(CRM_GPIOA_PERIPH_CLOCK, FALSE);
  crm_periph_clock_enable(CRM_USB_PERIPH_CLOCK, FALSE);

  crm_periph_reset(CRM_USB_PERIPH_RESET, TRUE);
  crm_periph_reset(CRM_USB_PERIPH_RESET, FALSE);

  __set_MSP(stkptr);
  pftarget = (void (*) (void))jumpaddr;
  pftarget();
}

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
  * @brief  set iap upgrade complete flag
  * @param  none
  * @retval none
  */
void iap_erase_sector(uint32_t address)
{
  flash_unlock();
  if(iap_info.sector_size == SECTOR_SIZE_1K)
  {
    flash_sector_erase(address);
  }
  else if(iap_info.sector_size == SECTOR_SIZE_2K)
  {
    /* check the address is sector align */
    if((address & (SECTOR_SIZE_2K-1)) == 0)
    {
      flash_sector_erase(address);
    }
  }
  else if(iap_info.sector_size == SECTOR_SIZE_4K)
  {
    if((address & (SECTOR_SIZE_4K-1)) == 0)
    {
      flash_sector_erase(address);
    }
  }
  flash_lock();
}

/**
  * @brief  crc cal
  * @param  addr: start address
  * @param  nk: nk byte
  * @retval crc value
  */
uint32_t crc_cal(uint32_t addr, uint16_t nk)
{
  uint32_t *paddr = (uint32_t *)addr;
  uint32_t wlen = nk * 1024 / sizeof(uint32_t);
  uint32_t value, i_index = 0;
  crm_periph_clock_enable(CRM_CRC_PERIPH_CLOCK, TRUE);
  crc_data_reset();

  for(i_index = 0; i_index < wlen; i_index ++)
  {
    value = *paddr;
    crc_one_word_calculate(CONVERT_ENDIAN(value));
    paddr ++;
  }
  return crc_data_get();
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
  iap_info.state = IAP_STS_START;
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

/**
  * @brief  iap parse address
  * @param  pdata: data buffer pointer.
  * @param  len: data length
  * @retval the result of the address parse
  */
iap_result_type iap_address(uint8_t *pdata, uint32_t len)
{
  iap_result_type status = IAP_SUCCESS;
  uint16_t result = IAP_ACK;
  uint8_t *paddr = pdata + 2;   /* skip iap cmd */
  uint32_t address;

  address = (paddr[0] << 24) |
            (paddr[1] << 16) |
            (paddr[2] << 8) |
             paddr[3];

  /* check the address is valid */
  if(address < iap_info.app_address || address > iap_info.flash_end_address)
  {
    status = IAP_FAILED;
    result = IAP_NACK;
  }
  else
  {
    iap_info.iap_address = address;

    if(iap_info.state == IAP_STS_START)
    {
      iap_clear_upgrade_flag();
    }
    iap_erase_sector(iap_info.iap_address);
  }

  iap_info.state = IAP_STS_ADDR;

  iap_respond(iap_info.iap_tx, IAP_CMD_ADDR, result);

  return status;
}
/*
  * @brief  iap data write
  * @param  pdata: data buffer pointer.
  * @param  len: buffer length
  * @retval the result of the address parse
  */
iap_result_type iap_data_write(uint8_t *pdata, uint32_t len)
{
  uint32_t data_len = (pdata[2] << 8 | pdata[3]);
  uint8_t *valid_data = pdata + 4;
  uint32_t *pbuf;
  uint32_t i_index = 0;

  if(iap_info.state == IAP_STS_ADDR)
  {
    if(data_len + iap_info.fifo_length <= HID_IAP_BUFFER_LEN)
    {
      for(i_index = 0; i_index < data_len; i_index ++)
      {
        iap_info.iap_fifo[iap_info.fifo_length ++] = valid_data[i_index];
      }
    }
    /* buffer full */
    if(iap_info.fifo_length == HID_IAP_BUFFER_LEN)
    {
      flash_unlock();
      pbuf = (uint32_t *)iap_info.iap_fifo;
      for(i_index = 0; i_index < iap_info.fifo_length / sizeof(uint32_t); i_index ++)
      {
        flash_word_program(iap_info.iap_address, *pbuf ++);
        iap_info.iap_address += 4;
      }
      flash_lock();

      iap_info.fifo_length = 0;
      iap_info.iap_address = 0;
      iap_respond(iap_info.iap_tx, IAP_CMD_DATA, IAP_ACK);
    }
  }
  else
  {
    iap_respond(iap_info.iap_tx, IAP_CMD_ADDR, IAP_NACK);
  }
  return IAP_SUCCESS;
}

/*
  * @brief  iap finish
  * @param  none
  * @retval none
  */
void iap_finish()
{
  iap_info.state = IAP_STS_FINISH;
  iap_set_upgrade_flag();
  iap_respond(iap_info.iap_tx, IAP_CMD_FINISH, IAP_ACK);
}

/*
  * @brief  iap crc
  * @param  pdata: data buffer pointer.
  * @param  len: buffer length
  * @retval the result of the address parse
  */
void iap_crc(uint8_t *pdata, uint32_t len)
{
  uint8_t *paddr = pdata + 2;   /* skip iap cmd */
  uint16_t crc_nk;
  uint32_t crc_value;
  uint32_t address = (paddr[0] << 24) |
                     (paddr[1] << 16) |
                     (paddr[2] << 8) |
                      paddr[3];
  paddr = pdata + 6;

  crc_nk = (paddr[0] << 16) | paddr[1];

  crc_value = crc_cal(address, crc_nk);

  iap_respond(iap_info.iap_tx, IAP_CMD_CRC, IAP_ACK);
  iap_info.iap_tx[4] = (uint8_t)((crc_value >> 24) & 0xFF);
  iap_info.iap_tx[5] = (uint8_t)((crc_value >> 16) & 0xFF);
  iap_info.iap_tx[6] = (uint8_t)((crc_value >> 8) & 0xFF);
  iap_info.iap_tx[7] = (uint8_t)((crc_value) & 0xFF);

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
    case IAP_CMD_ADDR:
      iap_address(pdata, len);
      break;
    case IAP_CMD_DATA:
      iap_data_write(pdata, len);
      break;
    case IAP_CMD_FINISH:
      iap_finish();
      break;
    case IAP_CMD_CRC:
      iap_crc(pdata, len);
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
void iap_loop(void)
{
  if(iap_info.state == IAP_STS_JMP)
  {
    jump_to_app(iap_info.app_address);
  }
}

/**
  * @}
  */

/**
  * @}
  */
