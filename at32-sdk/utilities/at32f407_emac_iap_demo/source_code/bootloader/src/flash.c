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
#include "iap.h"

/** @addtogroup UTILITIES_examples
  * @{
  */

/** @addtogroup EMAC_IAP_bootloader
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
  * @brief  this function writes a data buffer in flash (data are 32-bit aligned).
  * @note   after writing data buffer, the flash content is checked.
  * @param  flash_address: start address for writing data buffer
  * @param  data: pointer on data buffer
  * @param  data_length: length of data buffer (unit is 32-bit word)
  * @retval 0: data successfully written to flash memory
  *         1: error occurred while writing data in flash memory
  *         2: written data in flash memory is different from expected one
  */
uint32_t flash_if_write(__IO uint32_t* flash_address, uint32_t* data ,uint16_t data_length)
{
  uint32_t i = 0;

  for (i = 0; (i < data_length) && (*flash_address <= (APP_END_ADDR - 4)); i++)
  {
    /* device voltage range supposed to be [2.7v to 3.6v], the operation will
       be done by word */
    if (flash_word_program(*flash_address, *(uint32_t*)(data + i)) == FLASH_OPERATE_DONE)
    {
     /* check the written value */
      if (*(uint32_t*)*flash_address != *(uint32_t*)(data + i))
      {
        /* flash content doesn't match sram content */
        return(2);
      }
      /* increment flash destination address */
      *flash_address += 4;
    }
    else
    {
      /* error occurred while writing data in flash memory */
      return (1);
    }
  }

  return (0);
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
