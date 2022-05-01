/**
  **************************************************************************
  * @file     psram.c
  * @version  v2.0.8
  * @date     2022-04-02
  * @brief    psram configuration
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
#include "xmc_psram.h"

/** @addtogroup AT32F403A_periph_examples
  * @{
  */

/** @addtogroup 403A_XMC_psram
  * @{
  */

#define Bank1_SRAM1_ADDR                 ((uint32_t)0x60000000)


/**
  * @brief  configures the xmc and gpios to interface with the sram memory.
  *         this function must be called before any write/read operation
  *         on the psram.
  * @param  none
  * @retval none
  */
void psram_init(void)
{
  xmc_norsram_init_type xmc_norsram_init_struct;
  xmc_norsram_timing_init_type  rw_timing_struct, w_timing_struct;
  gpio_init_type gpio_init_struct;

  /* enable the xmc clock */
  /* enable gpiod/gpiob/gpioe clock */
  crm_periph_clock_enable(CRM_XMC_PERIPH_CLOCK, TRUE);
  crm_periph_clock_enable(CRM_GPIOD_PERIPH_CLOCK, TRUE);
  crm_periph_clock_enable(CRM_GPIOB_PERIPH_CLOCK, TRUE);
  crm_periph_clock_enable(CRM_GPIOE_PERIPH_CLOCK, TRUE);

  /*-- gpio configuration ------------------------------------------------------*/
  /* sram data lines configuration */
  gpio_default_para_init(&gpio_init_struct);
  gpio_init_struct.gpio_pins = GPIO_PINS_0  | GPIO_PINS_1  | GPIO_PINS_7  | GPIO_PINS_8  | \
                               GPIO_PINS_9  | GPIO_PINS_10 | GPIO_PINS_14 | GPIO_PINS_15;
  gpio_init_struct.gpio_mode = GPIO_MODE_MUX;
  gpio_init_struct.gpio_drive_strength = GPIO_DRIVE_STRENGTH_MODERATE;
  gpio_init(GPIOD, &gpio_init_struct);

  gpio_init_struct.gpio_pins = GPIO_PINS_3  | GPIO_PINS_7  | GPIO_PINS_8  | GPIO_PINS_9  | \
                               GPIO_PINS_10 | GPIO_PINS_11 | GPIO_PINS_12 | GPIO_PINS_13 | \
                               GPIO_PINS_14 | GPIO_PINS_15;
  gpio_init(GPIOE, &gpio_init_struct);

  /* sram address lines configuration */
  gpio_init_struct.gpio_pins = GPIO_PINS_7;
  gpio_init(GPIOB, &gpio_init_struct);

  gpio_init_struct.gpio_pins = GPIO_PINS_11 | GPIO_PINS_12 | GPIO_PINS_13;
  gpio_init(GPIOD, &gpio_init_struct);

  /* noe and nwe configuration */
  gpio_init_struct.gpio_pins = GPIO_PINS_4 | GPIO_PINS_5;
  gpio_init(GPIOD, &gpio_init_struct);

  /* nbl0, nbl1 configuration */
  gpio_init_struct.gpio_pins = GPIO_PINS_0 | GPIO_PINS_1;
  gpio_init(GPIOE, &gpio_init_struct);

  /*-- xmc configuration ------------------------------------------------------*/
  xmc_norsram_init_struct.subbank                    = XMC_BANK1_NOR_SRAM1;
  xmc_norsram_init_struct.data_addr_multiplex        = XMC_DATA_ADDR_MUX_ENABLE;
  xmc_norsram_init_struct.device                     = XMC_DEVICE_PSRAM;
  xmc_norsram_init_struct.bus_type                   = XMC_BUSTYPE_16_BITS;
  xmc_norsram_init_struct.burst_mode_enable          = XMC_BURST_MODE_DISABLE;
  xmc_norsram_init_struct.asynwait_enable            = XMC_ASYN_WAIT_DISABLE;
  xmc_norsram_init_struct.wait_signal_lv             = XMC_WAIT_SIGNAL_LEVEL_LOW;
  xmc_norsram_init_struct.wrapped_mode_enable        = XMC_WRAPPED_MODE_DISABLE;
  xmc_norsram_init_struct.wait_signal_config         = XMC_WAIT_SIGNAL_SYN_BEFORE;
  xmc_norsram_init_struct.write_enable               = XMC_WRITE_OPERATION_ENABLE;
  xmc_norsram_init_struct.wait_signal_enable         = XMC_WAIT_SIGNAL_DISABLE;
  xmc_norsram_init_struct.write_burst_syn            = XMC_WRITE_BURST_SYN_DISABLE;
  xmc_norsram_init_struct.write_timing_enable        = XMC_WRITE_TIMING_DISABLE;
  xmc_nor_sram_init(&xmc_norsram_init_struct);

  rw_timing_struct.subbank               = XMC_BANK1_NOR_SRAM1;
  rw_timing_struct.mode                  = XMC_ACCESS_MODE_A;
  rw_timing_struct.write_timing_enable   = XMC_WRITE_TIMING_DISABLE;
  rw_timing_struct.addr_hold_time        = 0x08;
  rw_timing_struct.addr_setup_time       = 0x09;
  rw_timing_struct.data_setup_time       = 0x0F;
  rw_timing_struct.data_latency_time     = 0x0;
  rw_timing_struct.bus_latency_time      = 0x0;
  rw_timing_struct.clk_psc               = 0x0;

  w_timing_struct.subbank                = XMC_BANK1_NOR_SRAM1;
  w_timing_struct.mode                   = XMC_ACCESS_MODE_A;
  w_timing_struct.write_timing_enable    = XMC_WRITE_TIMING_DISABLE;
  w_timing_struct.addr_hold_time         = 0x08;
  w_timing_struct.addr_setup_time        = 0x09;
  w_timing_struct.data_setup_time        = 0x0F;
  w_timing_struct.data_latency_time      = 0x0;
  w_timing_struct.bus_latency_time       = 0x0;
  w_timing_struct.clk_psc                = 0x0;

  xmc_nor_sram_timing_config(&rw_timing_struct, &w_timing_struct);

  /* bus turnaround phase for consecutive read duration and consecutive write duration */
  xmc_ext_timing_config(XMC_BANK1_NOR_SRAM1, 0x08, 0x08);

  /* enable xmc bank1_sram bank */
  xmc_nor_sram_enable(XMC_BANK1_NOR_SRAM1, TRUE);
}

/**
  * @brief  writes a half-word buffer to the xmc psram memory.
  * @param  pbuffer : pointer to buffer.
  * @param  write_addr : sram memory internal address from which the data will be
  *         written.
  * @param  num_halfword_to_write : number of half-words to write.
  * @retval none
  */
void psram_writebuffer(uint16_t* pbuffer, uint32_t write_addr, uint32_t num_halfword_to_write)
{
  /* while there is data to write */
  for(; num_halfword_to_write != 0; num_halfword_to_write--)
  {
    /* transfer data to the memory */
    *(__IO uint16_t *) (Bank1_SRAM1_ADDR + write_addr) = *pbuffer++;

    /* increment the address */
    write_addr += 2;
  }
}

/**
  * @brief  reads a block of data from the xmc psram memory.
  * @param  pbuffer : pointer to the buffer that receives the data read from the
  *         sram memory.
  * @param  read_addr : sram memory internal address to read from.
  * @param  num_halfword_to_read : number of half-words to read.
  * @retval none
  */
void psram_readbuffer(uint16_t* pbuffer, uint32_t read_addr, uint32_t num_halfword_to_read)
{
  /* while there is data to read */
  for(; num_halfword_to_read != 0; num_halfword_to_read--)
  {
    /* read a half-word from the memory */
    *pbuffer++ = *(__IO uint16_t*) (Bank1_SRAM1_ADDR + read_addr);

    /* increment the address */
    read_addr += 2;
  }
}

/**
  * @}
  */

/**
  * @}
  */
