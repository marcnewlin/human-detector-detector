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
#include "xmc_psram.h"

/** @addtogroup AT32F403A_periph_examples
  * @{
  */

/** @addtogroup 403A_XMC_psram XMC_psram
  * @{
  */

#define BUFFER_SIZE        0x400
#define WRITE_READ_ADDR    0x8000

uint16_t txbuffer[BUFFER_SIZE];
uint16_t rxbuffer[BUFFER_SIZE];
uint32_t writereadstatus = 0;
uint16_t index_cnt = 0;

void fill_buffer(uint16_t *pBuffer, uint16_t BufferLenght, uint32_t Offset);

/**
  * @brief  main function.
  * @param  none
  * @retval none
  */
int main(void)
{
  /* configure the system clock */
  system_clock_config();

  /* initialize leds */
  at32_board_init();

  /* initialize usart */
  uart_print_init(115200);

  /* configure xmc bank1 nor/sram1 */
  psram_init();

  /* fill the buffer to send */
  fill_buffer(txbuffer, BUFFER_SIZE, 0x3212);

  /* write data to xmc sram memory */
  psram_writebuffer(txbuffer, WRITE_READ_ADDR, BUFFER_SIZE);

  /* read data from xmc sram memory */
  psram_readbuffer(rxbuffer, WRITE_READ_ADDR, BUFFER_SIZE);

  /* read back sram memory and check content correctness */
  for(index_cnt = 0x00; (index_cnt < BUFFER_SIZE) && (writereadstatus == 0); index_cnt++)
  {
    if(rxbuffer[index_cnt] != txbuffer[index_cnt])
    {
      writereadstatus = index_cnt + 1;
    }
  }

  if(writereadstatus == 0)
  {
    /* pass */
    printf("data is right\r\n");
  }
  else
  {
    /* fail */
    printf("data is error\r\n");
  }

  while(1)
  {
  }
}

/**
  * @brief  fill the global buffer
  * @param  pbuffer: pointer on the buffer to fill
  * @param  buffersize: size of the buffer to fill
  * @param  offset: first value to fill on the buffer
  */
void fill_buffer(uint16_t *pbuffer, uint16_t bufferlenght, uint32_t offset)
{
  uint16_t indextmp = 0;

  /* put in global buffer same values */
  for(indextmp = 0; indextmp < bufferlenght; indextmp++ )
  {
    pbuffer[indextmp] = indextmp + offset;
  }
}

/**
  * @}
  */

/**
  * @}
  */

