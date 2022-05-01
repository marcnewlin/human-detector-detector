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
#include <stdio.h>

/** @addtogroup AT32F403A_periph_examples
  * @{
  */

/** @addtogroup 403A_SPI_fullduplex_polling SPI_fullduplex_polling
  * @{
  */

#define BUFFER_SIZE                      32

spi_init_type spi_init_struct;
uint8_t spi1_tx_buffer[BUFFER_SIZE] = {0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08,
                                       0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F, 0x10,
                                       0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 0x18,
                                       0x19, 0x1A, 0x1B, 0x1C, 0x1D, 0x1E, 0x1F, 0x20};
uint8_t spi2_tx_buffer[BUFFER_SIZE] = {0x51, 0x52, 0x53, 0x54, 0x55, 0x56, 0x57, 0x58,
                                       0x59, 0x5A, 0x5B, 0x5C, 0x5D, 0x5E, 0x5F, 0x60,
                                       0x61, 0x62, 0x63, 0x64, 0x65, 0x66, 0x67, 0x68,
                                       0x69, 0x6A, 0x6B, 0x6C, 0x6D, 0x6E, 0x6F, 0x70};
uint8_t spi1_rx_buffer[BUFFER_SIZE], spi2_rx_buffer[BUFFER_SIZE];
uint32_t tx_index = 0, rx_index = 0;
volatile error_status transfer_status1 = ERROR, transfer_status2 = ERROR;
volatile error_status transfer_status3 = ERROR, transfer_status4 = ERROR;

static void gpio_config(uint16_t spi1_mode, uint16_t spi2_mode);
static void spi_config(void);
error_status buffer_compare(uint8_t* pbuffer1, uint8_t* pbuffer2, uint16_t bufferlength);

/**
  * @brief  buffer compare function.
  * @param  pbuffer1, pbuffer2: buffers to be compared.
  * @param  buffer_length: buffer's length
  * @retval the result of compare
  */
error_status buffer_compare(uint8_t* pbuffer1, uint8_t* pbuffer2, uint16_t bufferlength)
{
  while(bufferlength--)
  {
    if(*pbuffer1 != *pbuffer2)
    {
      return ERROR;
    }

    pbuffer1++;
    pbuffer2++;
  }
  return SUCCESS;
}

/**
  * @brief  spi configuration.
  * @param  none
  * @retval none
  */
static void spi_config(void)
{
  crm_periph_clock_enable(CRM_SPI1_PERIPH_CLOCK, TRUE);
  crm_periph_clock_enable(CRM_SPI2_PERIPH_CLOCK, TRUE);
  spi_default_para_init(&spi_init_struct);
  spi_init_struct.transmission_mode = SPI_TRANSMIT_FULL_DUPLEX;
  spi_init_struct.master_slave_mode = SPI_MODE_MASTER;
  spi_init_struct.mclk_freq_division = SPI_MCLK_DIV_8;
  spi_init_struct.first_bit_transmission = SPI_FIRST_BIT_LSB;
  spi_init_struct.frame_bit_num = SPI_FRAME_8BIT;
  spi_init_struct.clock_polarity = SPI_CLOCK_POLARITY_LOW;
  spi_init_struct.clock_phase = SPI_CLOCK_PHASE_2EDGE;
  spi_init_struct.cs_mode_selection = SPI_CS_SOFTWARE_MODE;
  spi_init(SPI1, &spi_init_struct);

  spi_init_struct.master_slave_mode = SPI_MODE_SLAVE;
  spi_init(SPI2, &spi_init_struct);

  spi_enable(SPI1, TRUE);
  spi_enable(SPI2, TRUE);
}

/**
  * @brief  gpio configuration.
  * @param  none
  * @retval none
  */
static void gpio_config(uint16_t spi1_mode, uint16_t spi2_mode)
{
  gpio_init_type gpio_initstructure;
  crm_periph_clock_enable(CRM_GPIOA_PERIPH_CLOCK, TRUE);
  crm_periph_clock_enable(CRM_GPIOB_PERIPH_CLOCK, TRUE);
  gpio_default_para_init(&gpio_initstructure);
  /* spi1 sck pin */
  gpio_initstructure.gpio_out_type       = GPIO_OUTPUT_PUSH_PULL;
  gpio_initstructure.gpio_drive_strength = GPIO_DRIVE_STRENGTH_STRONGER;
  gpio_initstructure.gpio_pull           = GPIO_PULL_DOWN;
  if(spi1_mode == SPI_MODE_MASTER)
  {
    gpio_initstructure.gpio_mode         = GPIO_MODE_MUX;
  }
  else
  {
    gpio_initstructure.gpio_mode         = GPIO_MODE_INPUT;
  }
  gpio_initstructure.gpio_pins = GPIO_PINS_5;
  gpio_init(GPIOA, &gpio_initstructure);

  /* spi1 miso pin */
  gpio_initstructure.gpio_pull           = GPIO_PULL_UP;
  if(spi1_mode == SPI_MODE_MASTER)
  {
    gpio_initstructure.gpio_mode         = GPIO_MODE_INPUT;
  }
  else
  {
    gpio_initstructure.gpio_mode         = GPIO_MODE_MUX;
  }
  gpio_initstructure.gpio_pins = GPIO_PINS_6;
  gpio_init(GPIOA, &gpio_initstructure);

  /* spi1 mosi pin */
  gpio_initstructure.gpio_pull           = GPIO_PULL_UP;
  if(spi1_mode == SPI_MODE_MASTER)
  {
    gpio_initstructure.gpio_mode         = GPIO_MODE_MUX;
  }
  else
  {
    gpio_initstructure.gpio_mode         = GPIO_MODE_INPUT;
  }
  gpio_initstructure.gpio_pins = GPIO_PINS_7;
  gpio_init(GPIOA, &gpio_initstructure);

  /* spi2 sck pin */
  gpio_initstructure.gpio_pull           = GPIO_PULL_DOWN;
  if(spi2_mode == SPI_MODE_SLAVE)
  {
    gpio_initstructure.gpio_mode         = GPIO_MODE_INPUT;
  }
  else
  {
    gpio_initstructure.gpio_mode         = GPIO_MODE_MUX;
  }
  gpio_initstructure.gpio_pins = GPIO_PINS_13;
  gpio_init(GPIOB, &gpio_initstructure);

  /* spi2 miso pin */
  gpio_initstructure.gpio_pull           = GPIO_PULL_UP;
  if(spi2_mode == SPI_MODE_SLAVE)
  {
    gpio_initstructure.gpio_mode         = GPIO_MODE_MUX;
  }
  else
  {
    gpio_initstructure.gpio_mode         = GPIO_MODE_INPUT;
  }
  gpio_initstructure.gpio_pins = GPIO_PINS_14;
  gpio_init(GPIOB, &gpio_initstructure);

  /* spi2 mosi pin */
  gpio_initstructure.gpio_pull           = GPIO_PULL_UP;
  if(spi2_mode == SPI_MODE_SLAVE)
  {
    gpio_initstructure.gpio_mode         = GPIO_MODE_INPUT;
  }
  else
  {
    gpio_initstructure.gpio_mode         = GPIO_MODE_MUX;
  }
  gpio_initstructure.gpio_pins = GPIO_PINS_15;
  gpio_init(GPIOB, &gpio_initstructure);
}

/**
  * @brief  main function.
  * @param  none
  * @retval none
  */
int main(void)
{
  __IO uint32_t index = 0;
  system_clock_config();
  at32_board_init();
  gpio_config(SPI_MODE_MASTER, SPI_MODE_SLAVE);
  spi_config();

  /* transfer procedure:the "BUFFER_SIZE" data transfer */
  while(tx_index < BUFFER_SIZE)
  {
    while(spi_i2s_flag_get(SPI2, SPI_I2S_TDBE_FLAG) == RESET);
    spi_i2s_data_transmit(SPI2, spi2_tx_buffer[tx_index]);
    while(spi_i2s_flag_get(SPI1, SPI_I2S_TDBE_FLAG) == RESET);
    spi_i2s_data_transmit(SPI1, spi1_tx_buffer[tx_index++]);
    while(spi_i2s_flag_get(SPI2, SPI_I2S_RDBF_FLAG) == RESET);
    spi2_rx_buffer[rx_index] = spi_i2s_data_receive(SPI2);
    while(spi_i2s_flag_get(SPI1, SPI_I2S_RDBF_FLAG) == RESET);
    spi1_rx_buffer[rx_index++] = spi_i2s_data_receive(SPI1);
  }

  /* test result:the data check */
  transfer_status1 = buffer_compare(spi2_rx_buffer, spi1_tx_buffer, BUFFER_SIZE);
  transfer_status2 = buffer_compare(spi1_rx_buffer, spi2_tx_buffer, BUFFER_SIZE);

  /* master &slave mode switch */
  spi_enable(SPI1, FALSE);
  spi_enable(SPI2, FALSE);
  gpio_config(SPI_MODE_SLAVE, SPI_MODE_MASTER);
  spi_init_struct.master_slave_mode =SPI_MODE_SLAVE;
  spi_init(SPI1, &spi_init_struct);

  spi_init_struct.master_slave_mode =SPI_MODE_MASTER;
  spi_init(SPI2, &spi_init_struct);

  tx_index = 0;
  rx_index = 0;
  for(index = 0; index < BUFFER_SIZE; index++)
    spi1_rx_buffer[index] = 0;
  for(index = 0; index < BUFFER_SIZE; index++)
    spi2_rx_buffer[index] = 0;

  spi_enable(SPI2, TRUE);
  spi_enable(SPI1, TRUE);

  /* transfer procedure:the "BUFFER_SIZE" data transfer */
  while(tx_index < BUFFER_SIZE)
  {
    while(spi_i2s_flag_get(SPI1, SPI_I2S_TDBE_FLAG) == RESET);
    spi_i2s_data_transmit(SPI1, spi1_tx_buffer[tx_index]);
    while(spi_i2s_flag_get(SPI2, SPI_I2S_TDBE_FLAG) == RESET);
    spi_i2s_data_transmit(SPI2, spi2_tx_buffer[tx_index++]);
    while(spi_i2s_flag_get(SPI1, SPI_I2S_RDBF_FLAG) == RESET);
    spi1_rx_buffer[rx_index] = spi_i2s_data_receive(SPI1);
    while(spi_i2s_flag_get(SPI2, SPI_I2S_RDBF_FLAG) == RESET);
    spi2_rx_buffer[rx_index++] = spi_i2s_data_receive(SPI2);
  }

  /* test result:the data check */
  transfer_status3 = buffer_compare(spi2_rx_buffer, spi1_tx_buffer, BUFFER_SIZE);
  transfer_status4 = buffer_compare(spi1_rx_buffer, spi2_tx_buffer, BUFFER_SIZE);

  /* test result indicate:if SUCCESS ,led2 lights */
  if((transfer_status1 == SUCCESS) && (transfer_status2 == SUCCESS) && \
     (transfer_status3 == SUCCESS) && (transfer_status4 == SUCCESS))
  {
    at32_led_on(LED2);
  }
  else
  {
    at32_led_off(LED2);
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
