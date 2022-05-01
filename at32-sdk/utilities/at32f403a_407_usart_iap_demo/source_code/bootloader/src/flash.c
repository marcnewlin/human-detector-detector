/**
  **************************************************************************
  * @file     flash.c
  * @version  v2.0.8
  * @date     2022-04-02
  * @brief    flash program
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

#include "flash.h"
#include "usart.h"
#include "iap.h"

/** @addtogroup UTILITIES_examples
  * @{
  */

/** @addtogroup USART_IAP_bootloader
  * @{
  */

/**
  * @brief  flash erase/program operation.
  * @note   follow 2kb operation of ont time
  * @param  none
  * @retval none
  */
void flash_2kb_write(uint32_t write_addr, uint8_t *pbuffer)
{
  uint16_t index, write_data;
  flash_unlock();
  flash_sector_erase(write_addr);
  if(FLASH_SIZE < 0x100)  /* less than 256kb, 1kb/sector */
    flash_sector_erase(write_addr + 0x400);
  for(index = 0; index < 2048; index += 2)
  {
    write_data = (pbuffer[index+1] << 8) + pbuffer[index];
    flash_halfword_program(write_addr, write_data);
    write_addr += sizeof(uint16_t);
  }
  flash_lock();
}

/**
  * @brief  check flash upgrade flag.
  * @param  none
  * @retval none
  */
flag_status flash_upgrade_flag_read(void)
{
  if((*(uint32_t*)IAP_UPGRADE_FLAG_ADDR) == IAP_UPGRADE_FLAG)
    return SET;
  else
    return RESET;
}

/**
  * @}
  */

/**
  * @}
  */
