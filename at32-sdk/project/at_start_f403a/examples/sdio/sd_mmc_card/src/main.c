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

#include <stdbool.h>
#include <string.h>
#include "at32_sdio.h"
#include "at32f403a_407_board.h"
#include "at32f403a_407_clock.h"

/** @addtogroup AT32F403A_periph_examples
  * @{
  */

/** @addtogroup 403A_SDIO_sd_mmc_card SDIO_sd_mmc_card
  * @{
  */

#define BLOCK_SIZE                       (512)
#define BLOCKS_NUMBER                    (64)
#define MULTI_BUFFER_SIZE                (BLOCK_SIZE * BLOCKS_NUMBER)
#define STREAM_BUFFER_SIZE               (4096)

static uint8_t sblock_tbuffer[BLOCK_SIZE], sblock_rbuffer[BLOCK_SIZE];
static uint8_t mblock_tbuffer[MULTI_BUFFER_SIZE], mblock_rbuffer[MULTI_BUFFER_SIZE];
static uint8_t stream_tbuffer[STREAM_BUFFER_SIZE], stream_rbuffer[STREAM_BUFFER_SIZE];

static error_status sd_single_block_test(void);
static error_status sd_multiple_blocks_test(void);
uint8_t buffer_compare(uint8_t* pbuffer1, uint8_t* pbuffer2, uint16_t buffer_length);
static void sd_test_error(void);
static void nvic_configuration(void);

/**
  * @brief  sd card single block read/write test.
  * @param  none
  * @retval ERROR: fail.
  *         SUCCESS: success.
  */
static error_status sd_single_block_test(void)
{
  sd_error_status_type status = SD_OK;
  uint8_t bus_width;

  /* test 1-bit and 4-bit width */
  for(bus_width = 0; bus_width < 2; bus_width++)
  {
    memset(sblock_tbuffer, bus_width + 0xAB, BLOCK_SIZE);
    memset(sblock_rbuffer, 0, BLOCK_SIZE);

    /* set data width */
    status = sd_wide_bus_operation_config((sdio_bus_width_type)bus_width);
    if(status != SD_OK)
    {
      return ERROR;
    }

    /* write block of 512 bytes on address 0 */
    status = sd_block_write(sblock_tbuffer, 0x00, BLOCK_SIZE);
    if(status != SD_OK)
    {
      return ERROR;
    }

    /* read block of 512 bytes from address 0 */
    status = sd_block_read(sblock_rbuffer, 0x00, BLOCK_SIZE);
    if(status != SD_OK)
    {
      return ERROR;
    }

    if(!buffer_compare(sblock_tbuffer, sblock_rbuffer, BLOCK_SIZE))
    {
      return ERROR;
    }
  }

  return SUCCESS;
}

/**
  * @brief  sd card multiple blocks read/write test.
  * @param  none
  * @retval ERROR: fail.
  *         SUCCESS: success.
  */
static error_status sd_multiple_blocks_test(void)
{
  sd_error_status_type status = SD_OK;
  uint8_t bus_width;

  /* test 1-bit and 4-bit width */
  for(bus_width = 0; bus_width < 2; bus_width++)
  {
    memset(mblock_tbuffer, bus_width + 0x3C, MULTI_BUFFER_SIZE);
    memset(mblock_rbuffer, 0, MULTI_BUFFER_SIZE);

    /* set data width */
    status = sd_wide_bus_operation_config((sdio_bus_width_type)bus_width);
    if(status != SD_OK)
    {
      return ERROR;
    }

    /* write multiple block of many bytes on address 0 */
    status = sd_mult_blocks_write(mblock_tbuffer, 0x00, BLOCK_SIZE, BLOCKS_NUMBER);
    if(status != SD_OK)
    {
      return ERROR;
    }

    /* read block of many bytes from address 0 */
    status = sd_mult_blocks_read(mblock_rbuffer, 0x00, BLOCK_SIZE, BLOCKS_NUMBER);
    if(status != SD_OK)
    {
      return ERROR;
    }

    /* check the correctness of written data */
    if(!buffer_compare(mblock_tbuffer, mblock_rbuffer, MULTI_BUFFER_SIZE))
    {
      return ERROR;
    }
  }

  return SUCCESS;
}

/**
  * @brief  mmc card data stream read/write test.
  * @param  none
  * @retval ERROR:    fail.
  *         SUCCESS: success.
  */
static error_status mmc_stream_test(void)
{
  sd_error_status_type status = SD_OK;
  uint8_t bus_width;

  /* only test 1-bit width */
  for(bus_width = 0; bus_width < 1; bus_width++)
  {
    memset(stream_tbuffer, bus_width+0x5A, STREAM_BUFFER_SIZE);
    memset(stream_rbuffer, 0,              STREAM_BUFFER_SIZE);

    /* set data width */
    status = sd_wide_bus_operation_config((sdio_bus_width_type)bus_width);
    if(status != SD_OK)
    {
      return ERROR;
    }

    /* write stream of many bytes on address 0 */
    status = mmc_stream_write(stream_tbuffer, 0x00, STREAM_BUFFER_SIZE);
    if (status != SD_OK)
    {
      return ERROR;
    }

    /* read stream of many bytes from address 0 */
    status = mmc_stream_read(stream_rbuffer, 0x00, STREAM_BUFFER_SIZE);
    if (status != SD_OK)
    {
      return ERROR;
    }

    /* check the correctness of written data */
    if(!buffer_compare(stream_tbuffer, stream_rbuffer, STREAM_BUFFER_SIZE))
    {
      return ERROR;
    }
  }

  return SUCCESS;
}

/**
  * @brief  compares two buffers.
  * @param  pbuffer1, pbuffer2: buffers to be compared.
  * @param  buffer_length: buffer's length
  * @retval 1: pbuffer1 identical to pbuffer2
  *         0: pbuffer1 differs from pbuffer2
  */
uint8_t buffer_compare(uint8_t* pbuffer1, uint8_t* pbuffer2, uint16_t buffer_length)
{
  while(buffer_length--)
  {
    if(*pbuffer1 != *pbuffer2)
    {
      return 0;
    }
    pbuffer1++;
    pbuffer2++;
  }
  return 1;
}

/**
  * @brief  LED2 on off every 300ms for sd test failed.
  * @param  none
  * @retval none
  */
static void sd_test_error(void)
{
  at32_led_on(LED2);
  delay_ms(300);
  at32_led_off(LED2);
  delay_ms(300);
}

/**
  * @brief  configures sdio1 irq channel.
  * @param  none
  * @retval none
  */
static void nvic_configuration(void)
{
  nvic_priority_group_config(NVIC_PRIORITY_GROUP_4);
  nvic_irq_enable(SDIO1_IRQn, 0, 0);
}

/**
  * @brief  show sd/mmc card info.
  * @param  none
  * @retval none
  */
void show_card_info(void)
{
  printf("---------------------\r\n");
  switch(sd_card_info.card_type)
  {
  case SDIO_STD_CAPACITY_SD_CARD_V1_1: printf("card type: SDSC V1.1\r\n");break;
  case SDIO_STD_CAPACITY_SD_CARD_V2_0: printf("card type: SDSC V2.0\r\n");break;
  case SDIO_HIGH_CAPACITY_SD_CARD:     printf("card type: SDHC V2.0\r\n");break;
  case SDIO_MULTIMEDIA_CARD:           printf("card type: MMC Card\r\n");break;
  }
  printf("card manufacturer_id: %d\r\n", sd_card_info.sd_cid_reg.manufacturer_id);
  printf("card rca: 0x%X\r\n", sd_card_info.rca);
  printf("card c_size = %u\r\n", sd_card_info.sd_csd_reg.device_size);
  printf("card capacity: %uMB %uKB\r\n", (uint32_t)(sd_card_info.card_capacity>>20), (uint32_t)(sd_card_info.card_capacity>>10));
  printf("card block_size: %d\r\n", sd_card_info.card_blk_size);
  printf("---------------------\r\n");
}

/* gloable functions ---------------------------------------------------------*/
/**
  * @brief  main function.
  * @param  none
  * @retval none
  */
int main(void)
{
  system_clock_config();

  at32_board_init();

  nvic_configuration();

  uart_print_init(115200);
  printf("start test..\r\n");

  /* sdio init */
  if(SD_OK != sd_init())
  {
    printf("sdio init fail\r\n");
    while(1)
    {
      sd_test_error();
    }
  }
  printf("sdio init ok\r\n");
  show_card_info();

  /* sd card single block test */
  if(SUCCESS != sd_single_block_test())
  {
    printf("sd card single block test fail\r\n");
    /* single block test fail, led2 fresh */
    while(1)
    {
      sd_test_error();
    }
  }
  printf("sd card single block test ok\r\n");

  /* sd card multiple blocks test */
  if(SUCCESS != sd_multiple_blocks_test())
  {
    printf("sd card multiple blocks test fail\r\n");
    /* multiple blocks test fail, led2 fresh */
    while(1)
    {
      sd_test_error();
    }
  }
  printf("sd card multiple blocks test ok\r\n");

  /* mmc card stream data transfer test */
  if(sd_card_info.card_type == SDIO_MULTIMEDIA_CARD)
  {
    if(SUCCESS != mmc_stream_test())
    {
      printf("mmc card stream data test fail\r\n");
      /* mmc card stream data test fail, led2 fresh */
      while(1)
      {
        sd_test_error();
      }
    }
    printf("mmc card stream data test ok\r\n");
  }

  /* all tests pass, led3 and led4 fresh */
  while(1)
  {
    at32_led_toggle(LED3);
    at32_led_toggle(LED4);
    delay_ms(300);
  }
}

/**
  * @}
  */

/**
  * @}
  */
