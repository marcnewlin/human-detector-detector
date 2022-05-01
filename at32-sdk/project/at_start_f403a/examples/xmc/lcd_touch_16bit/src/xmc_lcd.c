/**
  **************************************************************************
  * @file     xmc_lcd.c
  * @version  v2.0.8
  * @date     2022-04-02
  * @brief    xmc_lcd program file
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

#include "xmc_lcd.h"

/** @addtogroup AT32F403A_periph_examples
  * @{
  */

/** @addtogroup 403A_XMC_lcd_touch_16bit
  * @{
  */

lcd_dev_type lcd_dev_struct =
{
  0,
  xmc_init,       /*!< function for xmc and gpios init */
  lcd_init,       /*!< function for configures the lcd */
  lcd_set_block,   /*!< lcd function to set block or set window */
  lcd_draw_point,  /*!< lcd function to drawpoint */
  lcd_clear,      /*!< lcd function to clear */
};

lcd_dev_type *lcd_struct;

/**
  * @brief  configures the xmc and gpios to interface with the lcd.
  *         this function must be called before any write/read operation
  *         on the lcd.
  * @param  none
  * @retval none
  */
void xmc_init(void)
{
  gpio_init_type  gpio_init_struct = {0};
  xmc_norsram_init_type  xmc_norsram_init_struct;
  xmc_norsram_timing_init_type rw_timing_struct, w_timing_struct;

  /* enable the gpio clock */
  crm_periph_clock_enable(CRM_GPIOA_PERIPH_CLOCK, TRUE);
  crm_periph_clock_enable(CRM_GPIOB_PERIPH_CLOCK, TRUE);
  crm_periph_clock_enable(CRM_GPIOC_PERIPH_CLOCK, TRUE);
  crm_periph_clock_enable(CRM_GPIOD_PERIPH_CLOCK, TRUE);
  crm_periph_clock_enable(CRM_GPIOE_PERIPH_CLOCK, TRUE);
  crm_periph_clock_enable(CRM_IOMUX_PERIPH_CLOCK, TRUE);

  /* enable the xmc clock */
  crm_periph_clock_enable(CRM_XMC_PERIPH_CLOCK, TRUE);

  /* remap */
  gpio_pin_remap_config(XMC_GMUX_001, TRUE);

  /*-- gpio configuration ------------------------------------------------------*/
  /* lcd data lines configuration */
  gpio_init_struct.gpio_pins = GPIO_PINS_2 | GPIO_PINS_3 | GPIO_PINS_4 | GPIO_PINS_5;
  gpio_init_struct.gpio_mode = GPIO_MODE_MUX;
  gpio_init_struct.gpio_out_type = GPIO_OUTPUT_PUSH_PULL;
  gpio_init_struct.gpio_pull = GPIO_PULL_NONE;
  gpio_init_struct.gpio_drive_strength = GPIO_DRIVE_STRENGTH_STRONGER;
  gpio_init(GPIOA, &gpio_init_struct);

  gpio_init_struct.gpio_pins = GPIO_PINS_12 | GPIO_PINS_14;
  gpio_init_struct.gpio_mode = GPIO_MODE_MUX;
  gpio_init_struct.gpio_out_type = GPIO_OUTPUT_PUSH_PULL;
  gpio_init_struct.gpio_pull = GPIO_PULL_NONE;
  gpio_init_struct.gpio_drive_strength = GPIO_DRIVE_STRENGTH_STRONGER;
  gpio_init(GPIOB, &gpio_init_struct);

  gpio_init_struct.gpio_pins = GPIO_PINS_6 | GPIO_PINS_11 | GPIO_PINS_12;
  gpio_init_struct.gpio_mode = GPIO_MODE_MUX;
  gpio_init_struct.gpio_out_type = GPIO_OUTPUT_PUSH_PULL;
  gpio_init_struct.gpio_pull = GPIO_PULL_NONE;
  gpio_init_struct.gpio_drive_strength = GPIO_DRIVE_STRENGTH_STRONGER;
  gpio_init(GPIOC, &gpio_init_struct);

  gpio_init_struct.gpio_pins = GPIO_PINS_11 | GPIO_PINS_12 | GPIO_PINS_13 | GPIO_PINS_14 | GPIO_PINS_15;
  gpio_init_struct.gpio_mode = GPIO_MODE_MUX;
  gpio_init_struct.gpio_out_type = GPIO_OUTPUT_PUSH_PULL;
  gpio_init_struct.gpio_pull = GPIO_PULL_NONE;
  gpio_init_struct.gpio_drive_strength = GPIO_DRIVE_STRENGTH_STRONGER;
  gpio_init(GPIOE, &gpio_init_struct);

  gpio_init_struct.gpio_pins = GPIO_PINS_9 | GPIO_PINS_10;
  gpio_init_struct.gpio_mode = GPIO_MODE_MUX;
  gpio_init_struct.gpio_out_type = GPIO_OUTPUT_PUSH_PULL;
  gpio_init_struct.gpio_pull = GPIO_PULL_NONE;
  gpio_init_struct.gpio_drive_strength = GPIO_DRIVE_STRENGTH_STRONGER;
  gpio_init(GPIOD, &gpio_init_struct);

  /* lcd reset lines configuration */
  gpio_init_struct.gpio_pins = GPIO_PINS_0;
  gpio_init_struct.gpio_mode = GPIO_MODE_OUTPUT;
  gpio_init_struct.gpio_out_type = GPIO_OUTPUT_PUSH_PULL;
  gpio_init_struct.gpio_pull = GPIO_PULL_NONE;
  gpio_init_struct.gpio_drive_strength = GPIO_DRIVE_STRENGTH_STRONGER;
  gpio_init(GPIOB, &gpio_init_struct);

  gpio_init_struct.gpio_pins = GPIO_PINS_3;
  gpio_init_struct.gpio_mode = GPIO_MODE_OUTPUT;
  gpio_init_struct.gpio_out_type = GPIO_OUTPUT_PUSH_PULL;
  gpio_init_struct.gpio_pull = GPIO_PULL_NONE;
  gpio_init_struct.gpio_drive_strength = GPIO_DRIVE_STRENGTH_STRONGER;
  gpio_init(GPIOD, &gpio_init_struct);

  /* lcd rs/cs/wr/rd lines configuration */
  gpio_init_struct.gpio_pins = GPIO_PINS_5;
  gpio_init_struct.gpio_mode = GPIO_MODE_MUX;
  gpio_init_struct.gpio_out_type = GPIO_OUTPUT_PUSH_PULL;
  gpio_init_struct.gpio_pull = GPIO_PULL_NONE;
  gpio_init_struct.gpio_drive_strength = GPIO_DRIVE_STRENGTH_STRONGER;
  gpio_init(GPIOC, &gpio_init_struct);

  gpio_init_struct.gpio_pins = GPIO_PINS_2 | GPIO_PINS_7 | GPIO_PINS_11;
  gpio_init_struct.gpio_mode = GPIO_MODE_MUX;
  gpio_init_struct.gpio_out_type = GPIO_OUTPUT_PUSH_PULL;
  gpio_init_struct.gpio_pull = GPIO_PULL_NONE;
  gpio_init_struct.gpio_drive_strength = GPIO_DRIVE_STRENGTH_STRONGER;
  gpio_init(GPIOD, &gpio_init_struct);

  /*-- xmc configuration ------------------------------------------------------*/
  xmc_norsram_default_para_init(&xmc_norsram_init_struct);
  xmc_norsram_init_struct.subbank = XMC_BANK1_NOR_SRAM1;
  xmc_norsram_init_struct.data_addr_multiplex = XMC_DATA_ADDR_MUX_DISABLE;
  xmc_norsram_init_struct.device = XMC_DEVICE_SRAM;
  xmc_norsram_init_struct.bus_type = XMC_BUSTYPE_16_BITS;
  xmc_norsram_init_struct.burst_mode_enable = XMC_BURST_MODE_DISABLE;
  xmc_norsram_init_struct.asynwait_enable = XMC_ASYN_WAIT_DISABLE;
  xmc_norsram_init_struct.wait_signal_lv = XMC_WAIT_SIGNAL_LEVEL_LOW;
  xmc_norsram_init_struct.wrapped_mode_enable = XMC_WRAPPED_MODE_DISABLE;
  xmc_norsram_init_struct.wait_signal_config = XMC_WAIT_SIGNAL_SYN_BEFORE;
  xmc_norsram_init_struct.write_enable = XMC_WRITE_OPERATION_ENABLE;
  xmc_norsram_init_struct.wait_signal_enable = XMC_WAIT_SIGNAL_DISABLE;
  xmc_norsram_init_struct.write_timing_enable = XMC_WRITE_TIMING_ENABLE;
  xmc_norsram_init_struct.write_burst_syn = XMC_WRITE_BURST_SYN_DISABLE;
  xmc_nor_sram_init(&xmc_norsram_init_struct);

  /* timing configuration */
  xmc_norsram_timing_default_para_init(&rw_timing_struct, &w_timing_struct);
  rw_timing_struct.subbank = XMC_BANK1_NOR_SRAM1;
  rw_timing_struct.write_timing_enable = XMC_WRITE_TIMING_ENABLE;
  rw_timing_struct.addr_setup_time = 0x2;
  rw_timing_struct.addr_hold_time = 0x0;
  rw_timing_struct.data_setup_time = 0x2;
  rw_timing_struct.bus_latency_time = 0x0;
  rw_timing_struct.clk_psc = 0x0;
  rw_timing_struct.data_latency_time = 0x0;
  rw_timing_struct.mode = XMC_ACCESS_MODE_A;
  w_timing_struct.subbank = XMC_BANK1_NOR_SRAM1;
  w_timing_struct.write_timing_enable = XMC_WRITE_TIMING_ENABLE;
  w_timing_struct.addr_setup_time = 0x2;
  w_timing_struct.addr_hold_time = 0x0;
  w_timing_struct.data_setup_time = 0x2;
  w_timing_struct.bus_latency_time = 0x0;
  w_timing_struct.clk_psc = 0x0;
  w_timing_struct.data_latency_time = 0x0;
  w_timing_struct.mode = XMC_ACCESS_MODE_A;
  xmc_nor_sram_timing_config(&rw_timing_struct, &w_timing_struct);

  /* bus turnaround phase for consecutive read duration and consecutive write duration */
  xmc_ext_timing_config(XMC_BANK1_NOR_SRAM1, 0x08, 0x08);

  /* enable xmc_bank1_nor_sram4 */
  xmc_nor_sram_enable(XMC_BANK1_NOR_SRAM1, TRUE);
}

/**
  * @brief  configures the lcd.
  *         this function must be called before any write/read operation
  *         on the lcd.
  * @param  none
  * @retval none
  */
void lcd_init(void)
{
  /* init xmc */
  lcd_struct->xmc_init();

  LCD_RESET_HIGH;

  delay_ms(50);

  /* read id */
  lcd_wr_command(0x0000);
  delay_ms(5);

  lcd_struct->lcd_id = lcd_rd_data();
  lcd_wr_command(0xD3);

  lcd_struct->lcd_id = lcd_rd_data();
  lcd_struct->lcd_id = lcd_rd_data();
  lcd_struct->lcd_id = lcd_rd_data();
  lcd_struct->lcd_id = lcd_rd_data();
  lcd_struct->lcd_id = lcd_struct->lcd_id << 8;
  lcd_struct->lcd_id |= lcd_rd_data();

  lcd_wr_command(0xcf);
  lcd_wr_data(0x00);
  lcd_wr_data(0xc1);
  lcd_wr_data(0x30);
  lcd_wr_command(0xed);
  lcd_wr_data(0x64);
  lcd_wr_data(0x03);
  lcd_wr_data(0x12);
  lcd_wr_data(0x81);
  lcd_wr_command(0xe8);
  lcd_wr_data(0x85);
  lcd_wr_data(0x10);
  lcd_wr_data(0x7a);
  lcd_wr_command(0xcb);
  lcd_wr_data(0x39);
  lcd_wr_data(0x2c);
  lcd_wr_data(0x00);
  lcd_wr_data(0x34);
  lcd_wr_data(0x02);
  lcd_wr_command(0xf7);
  lcd_wr_data(0x20);
  lcd_wr_command(0xea);
  lcd_wr_data(0x00);
  lcd_wr_data(0x00);
  lcd_wr_command(0xc0);
  lcd_wr_data(0x1b);
  lcd_wr_command(0xc1);
  lcd_wr_data(0x01);
  lcd_wr_command(0xc5);
  lcd_wr_data(0x30);
  lcd_wr_data(0x30);
  lcd_wr_command(0xc7);
  lcd_wr_data(0xb7);
  lcd_wr_command(0x36);
  lcd_wr_data(0x48);
  lcd_wr_command(0x3a);
  lcd_wr_data(0x55);
  lcd_wr_command(0xb1);
  lcd_wr_data(0x00);
  lcd_wr_data(0x1a);
  lcd_wr_command(0xb6);
  lcd_wr_data(0x0a);
  lcd_wr_data(0xa2);
  lcd_wr_command(0xf2);
  lcd_wr_data(0x00);
  lcd_wr_command(0x26);
  lcd_wr_data(0x01);
  lcd_wr_command(0xe0);
  lcd_wr_data(0x0f);
  lcd_wr_data(0x2a);
  lcd_wr_data(0x28);
  lcd_wr_data(0x08);
  lcd_wr_data(0x0e);
  lcd_wr_data(0x08);
  lcd_wr_data(0x54);
  lcd_wr_data(0xa9);
  lcd_wr_data(0x43);
  lcd_wr_data(0x0a);
  lcd_wr_data(0x0f);
  lcd_wr_data(0x00);
  lcd_wr_data(0x00);
  lcd_wr_data(0x00);
  lcd_wr_data(0x00);
  lcd_wr_command(0xe1);
  lcd_wr_data(0x00);
  lcd_wr_data(0x15);
  lcd_wr_data(0x17);
  lcd_wr_data(0x07);
  lcd_wr_data(0x11);
  lcd_wr_data(0x06);
  lcd_wr_data(0x2b);
  lcd_wr_data(0x56);
  lcd_wr_data(0x3c);
  lcd_wr_data(0x05);
  lcd_wr_data(0x10);
  lcd_wr_data(0x0f);
  lcd_wr_data(0x3f);
  lcd_wr_data(0x3f);
  lcd_wr_data(0x0f);
  lcd_wr_command(0x2b);
  lcd_wr_data(0x00);
  lcd_wr_data(0x00);
  lcd_wr_data(0x01);
  lcd_wr_data(0x3f);
  lcd_wr_command(0x2a);
  lcd_wr_data(0x00);
  lcd_wr_data(0x00);
  lcd_wr_data(0x00);
  lcd_wr_data(0xef);
  lcd_wr_command(0x11);
  delay_ms(120);
  lcd_wr_command(0x29);
  lcd_wr_command(0x36);
  lcd_wr_data(0x08);
  lcd_wr_command(0x2a);
  lcd_wr_data(0x00);
  lcd_wr_data(0x00);
  lcd_wr_data(0xef >> 8);
  lcd_wr_data(0xef & 0xff);
  lcd_wr_command(0x2b);
  lcd_wr_data(0x00);
  lcd_wr_data(0x00);
  lcd_wr_data(0x13f >> 8);
  lcd_wr_data(0x13f & 0xff);
  LCD_BL_HIGH;
}

/**
  * @brief  this function is write command to lcd.
  * @param  command : the command to write.
  * @retval none
  */
void lcd_wr_command(uint16_t command)
{
  *(__IO uint16_t*) XMC_LCD_COMMAND = command;
}

/**
  * @brief  this function is write data to lcd.
  * @param  data : the data to write.
  * @retval none
  */
void lcd_wr_data(uint16_t data)
{
  *(__IO uint16_t*) XMC_LCD_DATA = data;
}

uint16_t lcd_rd_data(void)
{
  uint16_t data;

  data = *(uint16_t*)XMC_LCD_DATA;

  return data;
}

/**
  * @brief  this function is set row&column coordinates for lcd.
  * @param  xstart : row coordinates starting vaule.
  * @param  ystart : column coordinates starting vaule.
  * @param  xend : row coordinates ending vaule.
  * @param  yend : column coordinates ending vaule.
  * @retval none
  */
void lcd_set_block(uint16_t xstart, uint16_t ystart, uint16_t xend, uint16_t yend)
{
  /* set row coordinates */
  lcd_wr_command(0x2a);
  lcd_wr_data(xstart >> 8);
  lcd_wr_data(xstart);
  lcd_wr_data(xend >> 8);
  lcd_wr_data(xend);

  /* set column coordinates */
  lcd_wr_command(0x2b);
  lcd_wr_data(ystart >> 8);
  lcd_wr_data(ystart);
  lcd_wr_data(yend >> 8);
  lcd_wr_data(yend);

  /* enable write menory */
  lcd_wr_command(0x2c);
}

/**
  * @brief  this function is write one point to lcd.
  * @param  data : the data to write.
  * @retval none
  */
void lcd_write_one_point(uint16_t color)
{
  lcd_wr_data(color);
}

/**
  * @brief  this function is draw point to lcd.
  * @param  data : the data to write.
  * @retval none
  */
void lcd_draw_point(uint16_t x, uint16_t y, uint16_t color)
{
  lcd_struct->lcd_set_block(x, y ,x ,y);

  lcd_write_one_point(color);
}

/**
  * @brief  this function is clear the lcd.
  * @param  data : the data to write.
  * @retval none
  */
void lcd_clear(uint16_t color)
{
  uint32_t i;

  lcd_struct->lcd_set_block(0, 0, 240, 320);
  for(i = 0; i < 76800; i++)
  {
    lcd_write_one_point(color);
  }
}

/**
  * @}
  */

/**
  * @}
  */
