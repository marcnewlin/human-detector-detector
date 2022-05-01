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
#include "flash.h"

/** @addtogroup AT32F407_periph_examples
  * @{
  */

/** @addtogroup 407_FLASH_write_read FLASH_write_read
  * @{
  */

#define TEST_BUFEER_SIZE                 3000
#define TEST_FLASH_ADDRESS_START         (0x08000000 + 1024 * 512)

uint16_t buffer_write[TEST_BUFEER_SIZE];
uint16_t buffer_read[TEST_BUFEER_SIZE];

error_status buffer_compare(uint16_t* p_buffer1, uint16_t* p_buffer2, uint16_t buffer_length);

/**
  * @brief  compares two buffers.
  * @param  p_buffer1, p_buffer2: buffers to be compared.
  * @param  buffer_length: buffer's length
  * @retval SUCCESS: p_buffer1 identical to p_buffer2
  *         failed: p_buffer1 differs from p_buffer2
  */
error_status buffer_compare(uint16_t* p_buffer1, uint16_t* p_buffer2, uint16_t buffer_length)
{
  while(buffer_length--)
  {
    if(*p_buffer1 != *p_buffer2)
    {
      return ERROR;
    }
    p_buffer1++;
    p_buffer2++;
  }
  return SUCCESS;
}

/**
  * @brief  main function.
  * @param  none
  * @retval none
  */
int main(void)
{
  uint32_t index=0;
  system_clock_config();
  at32_board_init();
  /* fill buffer_write data to test */
  for(index = 0; index < TEST_BUFEER_SIZE; index++)
  {
    buffer_write[index] = index;
  }

  /* write data to flash */
  flash_write(TEST_FLASH_ADDRESS_START, buffer_write, TEST_BUFEER_SIZE);

  /* read data from flash */
  flash_read(TEST_FLASH_ADDRESS_START, buffer_read, TEST_BUFEER_SIZE);

  /* compare the buffer */
  if(buffer_compare(buffer_write, buffer_read, TEST_BUFEER_SIZE) == SUCCESS)
  {
    at32_led_on(LED2);
    at32_led_on(LED3);
    at32_led_on(LED4);
  }

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
