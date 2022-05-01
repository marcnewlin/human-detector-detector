/**
  **************************************************************************
  * @file     xmc_lcd.h
  * @version  v2.0.8
  * @date     2022-04-02
  * @brief    xmc_lcd program header
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
#ifndef __XMC_LCD_H
#define __XMC_LCD_H

#ifdef __cplusplus
extern "C" {
#endif

#include "at32f403a_407_board.h"

/** @addtogroup AT32F403A_periph_examples
  * @{
  */

/** @addtogroup 403A_XMC_lcd_touch_16bit
  * @{
  */

/** @defgroup XMC_lcd_touch_16bit_device_type
  * @{
  */

typedef struct
{
  uint16_t lcd_id;                                                                        /*!< lcd device id define */
  void (*xmc_init)(void);                                                                 /*!< function for xmc and gpios init */
  void (*lcd_init)(void);                                                                 /*!< function for configures the lcd */
  void (*lcd_set_block)(uint16_t xstart, uint16_t ystart, uint16_t xend, uint16_t yend);   /*!< lcd function to set block or set window */
  void (*draw_point)(uint16_t x, uint16_t y, uint16_t color);                             /*!< lcd function to drawpoint */
  void (*lcd_clear)(uint16_t color);                                                      /*!< lcd function to clear */
} lcd_dev_type;

/**
  * @}
  */

/** @defgroup XMC_lcd_touch_16bit_address_of_data_and_command
  * @{
  */

/* the address of write data & command (xmc_a0) */
#define  XMC_LCD_COMMAND                 0x6001FFFE
#define  XMC_LCD_DATA                    0x60020000

/**
  * @}
  */

/** @defgroup XMC_lcd_touch_16bit_reset_line
  * @{
  */

/* the lcd reset line */
#define LCD_RESET_HIGH                   gpio_bits_set(GPIOD,GPIO_PINS_3)
#define LCD_RESET_LOW                    gpio_bits_reset(GPIOD,GPIO_PINS_3)

/**
  * @}
  */

#define LCD_BL_HIGH                      gpio_bits_set(GPIOB,GPIO_PINS_0)
#define LCD_BL_LOW                       gpio_bits_reset(GPIOB,GPIO_PINS_0)
static uint16_t LCD_ID;
#define WHITE                            0xFFFF
#define BLACK                            0x0000
#define BLUE                             0x001F
#define BRED                             0xF81F
#define GRED                             0xFFE0
#define GBLUE                            0x07FF
#define RED                              0xF800
#define MAGENTA                          0xF81F
#define GREEN                            0x07E0
#define CYAN                             0x7FFF
#define YELLOW                           0xFFE0
#define BROWN                            0xBC40
#define BRRED                            0xFC07
#define GRAY                             0x8430

extern lcd_dev_type lcd_dev_struct;
extern lcd_dev_type *lcd_struct;

/** @defgroup XMC_lcd_touch_16bit_exported_functions
  * @{
  */

void xmc_init(void);
void lcd_init(void);
void lcd_wr_command(uint16_t command);
void lcd_wr_data(uint16_t data);
uint16_t lcd_rd_data(void);
void lcd_set_block(uint16_t xstart, uint16_t ystart, uint16_t xend, uint16_t yend);
void lcd_write_one_point(uint16_t color);
void lcd_draw_point(uint16_t x, uint16_t y, uint16_t color);
void lcd_clear(uint16_t color);

/**
  * @}
  */

/**
  * @}
  */

/**
  * @}
  */

#ifdef __cplusplus
}
#endif

#endif
