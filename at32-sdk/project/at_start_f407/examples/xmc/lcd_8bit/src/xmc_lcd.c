/**
  **************************************************************************
  * @file     xmc_lcd.c
  * @version  v2.0.8
  * @date     2022-04-02
  * @brief    xmc_lcd config program
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
#include "xmc_lcd.h"
#include "picture.h"

/** @addtogroup AT32F407_periph_examples
  * @{
  */

/** @addtogroup 407_XMC_lcd_8bit
  * @{
  */


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

  gpio_init_struct.gpio_pins = GPIO_PINS_14;
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

  /* lcd reset lines configuration */
  gpio_init_struct.gpio_pins = GPIO_PINS_9;
  gpio_init_struct.gpio_mode = GPIO_MODE_OUTPUT;
  gpio_init_struct.gpio_out_type = GPIO_OUTPUT_PUSH_PULL;
  gpio_init_struct.gpio_pull = GPIO_PULL_NONE;
  gpio_init_struct.gpio_drive_strength = GPIO_DRIVE_STRENGTH_STRONGER;
  gpio_init(GPIOB, &gpio_init_struct);

  /* lcd rs/cs/wr/rd lines configuration */
  gpio_init_struct.gpio_pins = GPIO_PINS_3 | GPIO_PINS_4 | GPIO_PINS_5;
  gpio_init_struct.gpio_mode = GPIO_MODE_MUX;
  gpio_init_struct.gpio_out_type = GPIO_OUTPUT_PUSH_PULL;
  gpio_init_struct.gpio_pull = GPIO_PULL_NONE;
  gpio_init_struct.gpio_drive_strength = GPIO_DRIVE_STRENGTH_STRONGER;
  gpio_init(GPIOC, &gpio_init_struct);

  gpio_init_struct.gpio_pins = GPIO_PINS_2;
  gpio_init_struct.gpio_mode = GPIO_MODE_MUX;
  gpio_init_struct.gpio_out_type = GPIO_OUTPUT_PUSH_PULL;
  gpio_init_struct.gpio_pull = GPIO_PULL_NONE;
  gpio_init_struct.gpio_drive_strength = GPIO_DRIVE_STRENGTH_STRONGER;
  gpio_init(GPIOD, &gpio_init_struct);

  /*-- xmc configuration ------------------------------------------------------*/
  xmc_norsram_default_para_init(&xmc_norsram_init_struct);
  xmc_norsram_init_struct.subbank = XMC_BANK1_NOR_SRAM4;
  xmc_norsram_init_struct.data_addr_multiplex = XMC_DATA_ADDR_MUX_DISABLE;
  xmc_norsram_init_struct.device = XMC_DEVICE_SRAM;
  xmc_norsram_init_struct.bus_type = XMC_BUSTYPE_8_BITS;
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
  rw_timing_struct.subbank = XMC_BANK1_NOR_SRAM4;
  rw_timing_struct.write_timing_enable = XMC_WRITE_TIMING_ENABLE;
  rw_timing_struct.addr_setup_time = 0xf;
  rw_timing_struct.addr_hold_time = 0x0;
  rw_timing_struct.data_setup_time = 0xf;
  rw_timing_struct.bus_latency_time = 0x0;
  rw_timing_struct.clk_psc = 0x0;
  rw_timing_struct.data_latency_time = 0x0;
  rw_timing_struct.mode = XMC_ACCESS_MODE_A;
  w_timing_struct.subbank = XMC_BANK1_NOR_SRAM4;
  w_timing_struct.write_timing_enable = XMC_WRITE_TIMING_ENABLE;
  w_timing_struct.addr_setup_time = 0xf;
  w_timing_struct.addr_hold_time = 0x0;
  w_timing_struct.data_setup_time = 0xf;
  w_timing_struct.bus_latency_time = 0x0;
  w_timing_struct.clk_psc = 0x0;
  w_timing_struct.data_latency_time = 0x0;
  w_timing_struct.mode = XMC_ACCESS_MODE_A;
  xmc_nor_sram_timing_config(&rw_timing_struct, &w_timing_struct);

  /* bus turnaround phase for consecutive read duration and consecutive write duration */
  xmc_ext_timing_config(XMC_BANK1_NOR_SRAM4, 0x08, 0x08);

  /* enable xmc_bank1_nor_sram4 */
  xmc_nor_sram_enable(XMC_BANK1_NOR_SRAM4, TRUE);
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
  xmc_init();
  delay_ms(50);

  /* reset lcd */
  LCD_RESET_HIGH;
  delay_ms(1);
  LCD_RESET_LOW;
  delay_ms(10);
  LCD_RESET_HIGH;
  delay_ms(120);
  lcd_wr_command(0x36);
  lcd_wr_data(0x00);
  lcd_wr_command(0x3a);
  lcd_wr_data(0x55);

  /* 320*240 */
  lcd_wr_command(0x2a);
  lcd_wr_data(0x00);
  lcd_wr_data(0x00);
  lcd_wr_data(0x01);
  lcd_wr_data(0x3f);
  lcd_wr_command(0x2b);
  lcd_wr_data(0x00);
  lcd_wr_data(0x00);
  lcd_wr_data(0x00);
  lcd_wr_data(0xef);
  lcd_wr_command(0xb2);
  lcd_wr_data(0x0c);
  lcd_wr_data(0x0c);
  lcd_wr_data(0x00);
  lcd_wr_data(0x33);
  lcd_wr_data(0x33);
  lcd_wr_command(0xb7);
  lcd_wr_data(0x35);
  lcd_wr_command(0xbb);
  lcd_wr_data(0x30);
  lcd_wr_command(0xc3);
  lcd_wr_data(0x1c);
  lcd_wr_command(0xc4);
  lcd_wr_data(0x18);
  lcd_wr_command(0xc6);
  lcd_wr_data(0x0f);
  lcd_wr_command(0xd0);
  lcd_wr_data(0xa4);
  lcd_wr_data(0xa2);
  lcd_wr_command(0xe0);
  lcd_wr_data(0xf0);
  lcd_wr_data(0x00);
  lcd_wr_data(0x0a);
  lcd_wr_data(0x10);
  lcd_wr_data(0x12);
  lcd_wr_data(0x1b);
  lcd_wr_data(0x39);
  lcd_wr_data(0x44);
  lcd_wr_data(0x47);
  lcd_wr_data(0x28);
  lcd_wr_data(0x12);
  lcd_wr_data(0x10);
  lcd_wr_data(0x16);
  lcd_wr_data(0x1b);
  lcd_wr_command(0xe1);
  lcd_wr_data(0xf0);
  lcd_wr_data(0x00);
  lcd_wr_data(0x0a);
  lcd_wr_data(0x10);
  lcd_wr_data(0x11);
  lcd_wr_data(0x1a);
  lcd_wr_data(0x3b);
  lcd_wr_data(0x34);
  lcd_wr_data(0x4e);
  lcd_wr_data(0x3a);
  lcd_wr_data(0x17);
  lcd_wr_data(0x16);
  lcd_wr_data(0x21);
  lcd_wr_data(0x22);
  lcd_wr_command(0x3a);
  lcd_wr_data(0x55);
  lcd_wr_command(0x11);
  delay_ms(120);

  /* display on */
  lcd_wr_command(0x29);
}

/**
  * @brief  this function is write command to lcd.
  * @param  command : the command to write.
  * @retval none
  */
void lcd_wr_command(uint8_t command)
{
  *(__IO uint8_t *) XMC_LCD_COMMAND = command;
}

/**
  * @brief  this function is write data to lcd.
  * @param  data : the data to write.
  * @retval none
  */
void lcd_wr_data(uint8_t data)
{
  *(__IO uint8_t *) XMC_LCD_DATA = data;
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
  lcd_wr_data(color >> 8);
  lcd_wr_data(color);
}

/**
  * @brief  this function is draw point to lcd.
  * @param  data : the data to write.
  * @retval none
  */
void lcd_draw_point(uint16_t x, uint16_t y, uint16_t color)
{
  lcd_set_block(x, y, x, y);

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

  lcd_set_block(0, 0, 240, 320);

  for(i = 0; i < 76800; i++)
  {
    lcd_write_one_point(color);
  }
}

/**
  * @brief  this function is display number on the lcd.
  * @param  none
  * @retval none
  */
void lcd_display_number(void)
{
  int i;

  lcd_set_block(50, 50, 199, 219);

  for(i=0; i<sizeof(gimage_8); i++)
  {
    lcd_wr_data(gimage_8[i]);
  }

  delay_ms(1000);
  lcd_set_block(50, 50, 199, 219);

  for(i=0; i<sizeof(gimage_3); i++)
  {
    lcd_wr_data(gimage_3[i]);
  }

  delay_ms(1000);
  lcd_set_block(50, 50, 199, 219);

  for(i=0; i<sizeof(gimage_5); i++)
  {
    lcd_wr_data(gimage_5[i]);
  }

  delay_ms(1000);
  lcd_set_block(50, 50, 199, 219);

  for(i=0; i<sizeof(gimage_9); i++)
  {
    lcd_wr_data(gimage_9[i]);
  }

  delay_ms(1000);
  lcd_set_block(50, 50, 199, 219);

  for(i=0; i<sizeof(gimage_0); i++)
  {
    lcd_wr_data(gimage_0[i]);
  }

  delay_ms(1000);
}


/**
  * @}
  */

/**
  * @}
  */

