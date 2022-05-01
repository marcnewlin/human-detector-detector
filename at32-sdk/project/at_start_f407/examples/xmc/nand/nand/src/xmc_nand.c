/**
  **************************************************************************
  * @file     xmc_nand.c
  * @version  v2.0.8
  * @date     2022-04-02
  * @brief    nand configuration
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
#include "xmc_nand.h"

/** @addtogroup AT32F407_periph_examples
  * @{
  */

/** @addtogroup 407_XMC_nand
  * @{
  */

/**
  * @brief  configures the xmc and gpios to interface with the nand memory.
  *         this function must be called before any write/read operation on the
  *         nand.
  * @param  none
  * @retval none
  */
void nand_init(void)
{
  gpio_init_type gpio_init_struct;
  xmc_nand_init_type nand_init_struct;
  xmc_nand_timinginit_type  regular_spacetiming_struct, special_spacetiming_struct;

  /* enable the xmc clock */
  crm_periph_clock_enable(CRM_XMC_PERIPH_CLOCK, TRUE);

  /* enable gpiod/gpiob/gpioe clock */
  crm_periph_clock_enable(CRM_GPIOD_PERIPH_CLOCK, TRUE);
  crm_periph_clock_enable(CRM_GPIOE_PERIPH_CLOCK, TRUE);

  /*-- gpio configuration ------------------------------------------------------*/
  /* cle, ale, d0->d3, noe, nwe and nce2  nand pin configuration  */
  gpio_default_para_init(&gpio_init_struct);
  gpio_init_struct.gpio_pins =  GPIO_PINS_11 | GPIO_PINS_12 | GPIO_PINS_14 | GPIO_PINS_15 |
                                 GPIO_PINS_0 | GPIO_PINS_1  | GPIO_PINS_4  | GPIO_PINS_5  |
                                 GPIO_PINS_7;
  gpio_init_struct.gpio_drive_strength = GPIO_DRIVE_STRENGTH_MODERATE;
  gpio_init_struct.gpio_mode = GPIO_MODE_MUX;
  gpio_init_struct.gpio_out_type = GPIO_OUTPUT_PUSH_PULL;
  gpio_init_struct.gpio_pull = GPIO_PULL_NONE;
  gpio_init(GPIOD, &gpio_init_struct);

  /* d4->d7 nand pin configuration  */
  gpio_init_struct.gpio_pins = GPIO_PINS_7 | GPIO_PINS_8 | GPIO_PINS_9 | GPIO_PINS_10;
  gpio_init(GPIOE, &gpio_init_struct);


  /* nwait nand pin configuration */
  gpio_init_struct.gpio_pins = GPIO_PINS_6;
  gpio_init_struct.gpio_drive_strength = GPIO_DRIVE_STRENGTH_MODERATE;
  gpio_init_struct.gpio_mode = GPIO_MODE_INPUT;

  gpio_init(GPIOD, &gpio_init_struct);

  /*-- xmc configuration ------------------------------------------------------*/
  xmc_nand_default_para_init(&nand_init_struct);
  nand_init_struct.nand_bank = XMC_BANK2_NAND;
  nand_init_struct.wait_enable = XMC_WAIT_OPERATION_DISABLE;
  nand_init_struct.bus_type = XMC_BUSTYPE_8_BITS;
  nand_init_struct.ecc_enable = XMC_ECC_OPERATION_DISABLE;
#ifdef H27U1G8F2CTR
  nand_init_struct.ecc_pagesize = XMC_ECC_PAGESIZE_2048_BYTES;
#elif defined K9GAG08U0E
  nand_init_struct.ecc_pagesize = XMC_ECC_PAGESIZE_8192_BYTES;
#endif
  nand_init_struct.delay_time_cycle = 0x00;
  nand_init_struct.delay_time_ar = 0x00;
  xmc_nand_init(&nand_init_struct);

  xmc_nand_timing_default_para_init(&regular_spacetiming_struct, &special_spacetiming_struct);
  regular_spacetiming_struct.class_bank = XMC_BANK2_NAND;
  regular_spacetiming_struct.mem_setup_time = 254;
  regular_spacetiming_struct.mem_hiz_time = 254;
  regular_spacetiming_struct.mem_hold_time = 254;
  regular_spacetiming_struct.mem_waite_time = 254;
  special_spacetiming_struct.class_bank = XMC_BANK2_NAND;
  special_spacetiming_struct.mem_setup_time = 254;
  special_spacetiming_struct.mem_hiz_time = 254;
  special_spacetiming_struct.mem_hold_time = 254;
  special_spacetiming_struct.mem_waite_time = 254;
  xmc_nand_timing_config(&regular_spacetiming_struct, &special_spacetiming_struct);

  /* xmc nand bank cmd test */
  xmc_nand_enable(XMC_BANK2_NAND, TRUE);
}

/**
  * @brief  reads nand memory's id.
  * @param  nand_id: pointer to a nand_id_type structure which will hold
  *         the manufacturer and device id.
  * @retval none
  */
void nand_read_id(nand_id_type* nand_id_struct)
{
  uint32_t data = 0;

  /* send command to the command area */
  *(__IO uint8_t *)(Bank_NAND_ADDR | CMD_AREA) = 0x90;
  *(__IO uint8_t *)(Bank_NAND_ADDR | ADDR_AREA) = 0x00;

#ifdef H27U1G8F2CTR
  /* sequence to read id from nand flash */
   data = *(__IO uint32_t *)(Bank_NAND_ADDR | DATA_AREA);
   nand_id_struct->maker_id   = addr_1st_cycle (data);
   nand_id_struct->device_id  = addr_2nd_cycle (data);
   nand_id_struct->third_id   = addr_3rd_cycle (data);
   nand_id_struct->fourth_id  = addr_4th_cycle (data);
#elif defined K9GAG08U0E
  /* sequence to read id from nand flash */
   data = *(__IO uint32_t *)(Bank_NAND_ADDR | DATA_AREA);
   nand_id_struct->maker_id   = addr_1st_cycle (data);
   nand_id_struct->device_id  = addr_2nd_cycle (data);
   nand_id_struct->third_id   = addr_3rd_cycle (data);
   nand_id_struct->fourth_id  = addr_4th_cycle (data);
   data = *(__IO uint16_t *)(Bank_NAND_ADDR | DATA_AREA);
   nand_id_struct->fifth_id   = addr_1st_cycle (data);
   nand_id_struct->sixth_id   = addr_2nd_cycle (data);
#endif
}

/**
  * @brief  this routine is for writing one or several 512 bytes page size.
  * @param  pbuffer: pointer on the buffer containing data to be written
  * @param  address: first page address
  * @param  num_page_to_write: number of page to write
  * @retval new status of the nand operation. this parameter can be:
  *         - NAND_TIMEOUT_ERROR: when the previous operation generate
  *           a timeout error
  *         - NAND_READY: when memory is ready for the next operation
  *           and the new status of the increment address operation. it can be:
  *         - NAND_VALID_ADDRESS: when the new address is valid address
  *         - NAND_INVALID_ADDRESS: when the new address is invalid address
  */
uint32_t nand_write_small_page(uint8_t *pbuffer, nand_address_type address_struct, uint32_t num_page_to_write)
{
  uint32_t index = 0x00, num_page_written = 0x00, address_status = NAND_VALID_ADDRESS;
  uint32_t status = NAND_READY, size = 0x00;

  while((num_page_to_write != 0x00) && (address_status == NAND_VALID_ADDRESS) && (status == NAND_READY))
  {
    /* page write command and address */
    *(__IO uint8_t *)(Bank_NAND_ADDR | CMD_AREA) = NAND_CMD_AREA_A;
    *(__IO uint8_t *)(Bank_NAND_ADDR | CMD_AREA) = NAND_CMD_WRITE0;
#ifdef H27U1G8F2CTR
    *(__IO uint8_t *)(Bank_NAND_ADDR | ADDR_AREA) = addr_1st_cycle(COL_ADDRESS);
    *(__IO uint8_t *)(Bank_NAND_ADDR | ADDR_AREA) = addr_2nd_cycle(COL_ADDRESS);
    *(__IO uint8_t *)(Bank_NAND_ADDR | ADDR_AREA) = addr_1st_cycle(ROW_ADDRESS);
    *(__IO uint8_t *)(Bank_NAND_ADDR | ADDR_AREA) = addr_2nd_cycle(ROW_ADDRESS);
#elif defined K9GAG08U0E
    *(__IO uint8_t *)(Bank_NAND_ADDR | ADDR_AREA) = addr_1st_cycle(COL_ADDRESS);
    *(__IO uint8_t *)(Bank_NAND_ADDR | ADDR_AREA) = addr_2nd_cycle(COL_ADDRESS);
    *(__IO uint8_t *)(Bank_NAND_ADDR | ADDR_AREA) = addr_1st_cycle(ROW_ADDRESS);
    *(__IO uint8_t *)(Bank_NAND_ADDR | ADDR_AREA) = addr_2nd_cycle(ROW_ADDRESS);
    *(__IO uint8_t *)(Bank_NAND_ADDR | ADDR_AREA) = addr_3rd_cycle(ROW_ADDRESS);
#endif

    /* calculate the size */
    size = NAND_PAGE_SIZE + (NAND_PAGE_SIZE * num_page_written);

    /* write data */
    for(index = 0; index < size; index++)
    {
      *(__IO uint8_t *)(Bank_NAND_ADDR | DATA_AREA) = pbuffer[index];
    }

    *(__IO uint8_t *)(Bank_NAND_ADDR | CMD_AREA) = NAND_CMD_WRITE_TRUE1;

    /* check status for successful operation */
    status = nand_get_status();

    if(status == NAND_READY)
    {
      num_page_written++;

      num_page_to_write--;

      /* calculate next small page address */
      address_status = nand_address_increment(&address_struct);
    }
  }

  return (status | address_status);
}

/**
  * @brief  this routine is for sequential read from one or several 512 bytes page size.
  * @param  pbuffer: pointer on the buffer to fill
  * @param  address: first page address
  * @param  num_page_to_read: number of page to read
  * @retval new status of the nand operation. this parameter can be:
  *         - NAND_TIMEOUT_ERROR: when the previous operation generate
  *           a timeout error
  *         - NAND_READY: when memory is ready for the next operation
  *           and the new status of the increment address operation. it can be:
  *         - NAND_VALID_ADDRESS: when the new address is valid address
  *         - NAND_INVALID_ADDRESS: when the new address is invalid address
  */
uint32_t nand_read_small_page(uint8_t *pbuffer, nand_address_type address_struct, uint32_t num_page_to_read)
{
  uint32_t index = 0x00, num_page_read = 0x00, address_status = NAND_VALID_ADDRESS;
  uint32_t status = NAND_READY, size = 0x00;

  while((num_page_to_read != 0x0) && (address_status == NAND_VALID_ADDRESS))
  {
    /* page read command and page address */
    *(__IO uint8_t *)(Bank_NAND_ADDR | CMD_AREA) = NAND_CMD_AREA_A;

#ifdef H27U1G8F2CTR
    *(__IO uint8_t *)(Bank_NAND_ADDR | ADDR_AREA) = addr_1st_cycle(COL_ADDRESS);
    *(__IO uint8_t *)(Bank_NAND_ADDR | ADDR_AREA) = addr_2nd_cycle(COL_ADDRESS);
    *(__IO uint8_t *)(Bank_NAND_ADDR | ADDR_AREA) = addr_1st_cycle(ROW_ADDRESS);
    *(__IO uint8_t *)(Bank_NAND_ADDR | ADDR_AREA) = addr_2nd_cycle(ROW_ADDRESS);
#elif defined K9GAG08U0E
    *(__IO uint8_t *)(Bank_NAND_ADDR | ADDR_AREA) = addr_1st_cycle(COL_ADDRESS);
    *(__IO uint8_t *)(Bank_NAND_ADDR | ADDR_AREA) = addr_2nd_cycle(COL_ADDRESS);
    *(__IO uint8_t *)(Bank_NAND_ADDR | ADDR_AREA) = addr_1st_cycle(ROW_ADDRESS);
    *(__IO uint8_t *)(Bank_NAND_ADDR | ADDR_AREA) = addr_2nd_cycle(ROW_ADDRESS);
    *(__IO uint8_t *)(Bank_NAND_ADDR | ADDR_AREA) = addr_3rd_cycle(ROW_ADDRESS);
#endif
    *(__IO uint8_t *)(Bank_NAND_ADDR | CMD_AREA) = NAND_CMD_AREA_TRUE1;

    delay_ms(1);

    /* calculate the size */
    size = (NAND_PAGE_SIZE + NAND_SPARE_AREA_SIZE) + ((NAND_PAGE_SIZE + NAND_SPARE_AREA_SIZE)  * num_page_read);

    /* get data into buffer */
    for(index = 0x00; index < size; index++)
    {
      pbuffer[index]= *(__IO uint8_t *)(Bank_NAND_ADDR | DATA_AREA);
    }

    num_page_read++;

    num_page_to_read--;

    /* calculate page address */
    address_status = nand_address_increment(&address_struct);
  }

  status = nand_get_status();

  return (status | address_status);
}

/**
  * @brief  this routine write the spare area information for the specified
  *         pages addresses.
  * @param  pbuffer: pointer on the buffer containing data to be written
  * @param  address: first page address
  * @param  num_spare_area_to_write: number of spare area to write
  * @retval new status of the nand operation. this parameter can be:
  *         - NAND_TIMEOUT_ERROR: when the previous operation generate
  *           a timeout error
  *         - NAND_READY: when memory is ready for the next operation
  *           and the new status of the increment address operation. it can be:
  *         - NAND_VALID_ADDRESS: when the new address is valid address
  *         - NAND_INVALID_ADDRESS: when the new address is invalid address
  */
uint32_t nand_write_spare_area(uint8_t *pbuffer, nand_address_type address_struct, uint32_t num_spare_area_to_write)
{
  uint32_t index = 0x00, num_spare_area_written = 0x00, address_status = NAND_VALID_ADDRESS;
  uint32_t status = NAND_READY, size = 0x00;

  while((num_spare_area_to_write != 0x00) && (address_status == NAND_VALID_ADDRESS) && (status == NAND_READY))
  {
    /* page write spare area command and address */
    *(__IO uint8_t *)(Bank_NAND_ADDR | CMD_AREA) = NAND_CMD_AREA_C;
    *(__IO uint8_t *)(Bank_NAND_ADDR | CMD_AREA) = NAND_CMD_WRITE0;

    *(__IO uint8_t *)(Bank_NAND_ADDR | ADDR_AREA) = addr_1st_cycle(COL_ADDRESS);
    *(__IO uint8_t *)(Bank_NAND_ADDR | ADDR_AREA) = addr_2nd_cycle(COL_ADDRESS);
    *(__IO uint8_t *)(Bank_NAND_ADDR | ADDR_AREA) = addr_1st_cycle(ROW_ADDRESS);
    *(__IO uint8_t *)(Bank_NAND_ADDR | ADDR_AREA) = addr_2nd_cycle(ROW_ADDRESS);

    /* calculate the size */
    size = NAND_SPARE_AREA_SIZE + (NAND_SPARE_AREA_SIZE * num_spare_area_written);

    /* write the data */
    for(index = 0x00; index < size; index++)
    {
      *(__IO uint8_t *)(Bank_NAND_ADDR | DATA_AREA) = pbuffer[index];
    }

    *(__IO uint8_t *)(Bank_NAND_ADDR | CMD_AREA) = NAND_CMD_WRITE_TRUE1;

    /* check status for successful operation */
    status = nand_get_status();

    if(status == NAND_READY)
    {
      num_spare_area_written++;

      num_spare_area_to_write--;

      /* calculate next page address */
      address_status = nand_address_increment(&address_struct);
    }
  }

  return (status | address_status);
}

/**
  * @brief  this routine read the spare area information from the specified
  *         pages addresses.
  * @param  pbuffer: pointer on the buffer to fill
  * @param  address: first page address
  * @param  num_spare_area_to_read: number of spare area to read
  * @retval new status of the nand operation. this parameter can be:
  *         - NAND_TIMEOUT_ERROR: when the previous operation generate
  *           a timeout error
  *         - NAND_READY: when memory is ready for the next operation
  *           and the new status of the increment address operation. it can be:
  *         - NAND_VALID_ADDRESS: when the new address is valid address
  *         - NAND_INVALID_ADDRESS: when the new address is invalid address
  */
uint32_t nand_read_spare_area(uint8_t *pbuffer, nand_address_type address_struct, uint32_t num_spare_area_to_read)
{
  uint32_t num_spare_area_read = 0x00, index = 0x00, address_status = NAND_VALID_ADDRESS;
  uint32_t status = NAND_READY, size = 0x00;

  while((num_spare_area_to_read != 0x0) && (address_status == NAND_VALID_ADDRESS))
  {
    /* page read command and page address */
    *(__IO uint8_t *)(Bank_NAND_ADDR | CMD_AREA) = NAND_CMD_AREA_C;

    *(__IO uint8_t *)(Bank_NAND_ADDR | ADDR_AREA) = 0x00;
    *(__IO uint8_t *)(Bank_NAND_ADDR | ADDR_AREA) = addr_1st_cycle(ROW_ADDRESS);
    *(__IO uint8_t *)(Bank_NAND_ADDR | ADDR_AREA) = addr_2nd_cycle(ROW_ADDRESS);
    *(__IO uint8_t *)(Bank_NAND_ADDR | ADDR_AREA) = addr_3rd_cycle(ROW_ADDRESS);
    *(__IO uint8_t *)(Bank_NAND_ADDR | ADDR_AREA) = addr_4th_cycle(ROW_ADDRESS);

    *(__IO uint8_t *)(Bank_NAND_ADDR | CMD_AREA) = NAND_CMD_AREA_TRUE1;

    delay_ms(1);

    /* data read */
    size = NAND_SPARE_AREA_SIZE +  (NAND_SPARE_AREA_SIZE * num_spare_area_read);

    /* get data into buffer */
    for(index = 0x00 ;index < size; index++)
    {
      pbuffer[index] = *(__IO uint8_t *)(Bank_NAND_ADDR | DATA_AREA);
    }

    num_spare_area_read++;

    num_spare_area_to_read--;

    /* calculate page address */
    address_status = nand_address_increment(&address_struct);
  }

  status = nand_get_status();

  return (status | address_status);
}

/**
  * @brief  this routine erase complete block from nand flash
  * @param  address: any address into block to be erased
  * @retval new status of the nand operation. this parameter can be:
  *         - NAND_TIMEOUT_ERROR: when the previous operation generate
  *           a timeout error
  *         - NAND_READY: when memory is ready for the next operation
  */
uint32_t nand_erase_block(nand_address_type address_struct)
{
  *(__IO uint8_t *)(Bank_NAND_ADDR | CMD_AREA) = NAND_CMD_ERASE0;

#ifdef H27U1G8F2CTR
  *(__IO uint8_t *)(Bank_NAND_ADDR | ADDR_AREA) = addr_1st_cycle(ROW_ADDRESS);
  *(__IO uint8_t *)(Bank_NAND_ADDR | ADDR_AREA) = addr_2nd_cycle(ROW_ADDRESS);
#elif defined K9GAG08U0E
  *(__IO uint8_t *)(Bank_NAND_ADDR | ADDR_AREA) = addr_1st_cycle(ROW_ADDRESS);
  *(__IO uint8_t *)(Bank_NAND_ADDR | ADDR_AREA) = addr_2nd_cycle(ROW_ADDRESS);
  *(__IO uint8_t *)(Bank_NAND_ADDR | ADDR_AREA) = addr_3rd_cycle(ROW_ADDRESS);
#endif
  *(__IO uint8_t *)(Bank_NAND_ADDR | CMD_AREA) = NAND_CMD_ERASE1;

  return (nand_get_status());
}

/**
  * @brief  this routine reset the nand flash.
  * @param  none
  * @retval NAND_READY
  */
uint32_t nand_reset(void)
{
  *(__IO uint8_t *)(Bank_NAND_ADDR | CMD_AREA) = NAND_CMD_RESET;

  return (NAND_READY);
}

/**
  * @brief  get the nand operation status.
  * @param  none
  * @retval new status of the nand operation. this parameter can be:
  *         - NAND_TIMEOUT_ERROR: when the previous operation generate
  *           a timeout error
  *         - NAND_READY: when memory is ready for the next operation
  */
uint32_t nand_get_status(void)
{
  uint32_t timeout = 0x1000000, status = NAND_READY;

  status = nand_read_status();

  /* wait for a nand operation to complete or a timeout to occur */
  while((status != NAND_READY) && ( timeout != 0x00))
  {
     status = nand_read_status();
     timeout --;
  }

  if(timeout == 0x00)
  {
    status =  NAND_TIMEOUT_ERROR;
  }

  /* return the operation status */
  return (status);
}

/**
  * @brief  reads the nand memory status using the read status command.
  * @param  none
  * @retval the status of the nand memory. this parameter can be:
  *         - NAND_BUSY: when memory is busy
  *         - NAND_READY: when memory is ready for the next operation
  *         - NAND_ERROR: when the previous operation gererates error
  */
uint32_t nand_read_status(void)
{
  uint32_t data = 0x00, status = NAND_BUSY;

  /* read status operation ------------------------------------*/
  *(__IO uint8_t *)(Bank_NAND_ADDR | CMD_AREA) = NAND_CMD_STATUS;
  data = *(__IO uint8_t *)(Bank_NAND_ADDR);

  if((data & NAND_ERROR) == NAND_ERROR)
  {
    status = NAND_ERROR;
  }
  else if((data & NAND_READY) == NAND_READY)
  {
    status = NAND_READY;
  }
  else
  {
    status = NAND_BUSY;
  }

  return (status);
}

/**
  * @brief  increment the nand memory address.
  * @param  address: address to be incremented.
  * @retval the new status of the increment address operation. it can be:
  *         - NAND_VALID_ADDRESS: when the new address is valid address
  *         - NAND_INVALID_ADDRESS: when the new address is invalid address
  */
uint32_t nand_address_increment(nand_address_type* address_struct)
{
  uint32_t status = NAND_VALID_ADDRESS;

  address_struct->page++;

  if(address_struct->page == NAND_BLOCK_SIZE)
  {
    address_struct->page = 0;
    address_struct->block++;

    if(address_struct->block == NAND_ZONE_SIZE)
    {
      address_struct->block = 0;
      address_struct->zone++;

      if(address_struct->zone == NAND_MAX_ZONE)
      {
        status = NAND_INVALID_ADDRESS;
      }
    }
  }

  return (status);
}

/**
  * @}
  */

/**
  * @}
  */
