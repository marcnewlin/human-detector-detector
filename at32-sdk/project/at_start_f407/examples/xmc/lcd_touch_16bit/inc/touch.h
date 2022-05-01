/**
  **************************************************************************
  * @file     touch.h
  * @version  v2.0.8
  * @date     2022-04-02
  * @brief    this file contains all the functions prototypes for the
  *           touch firmware driver.
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

#ifndef __TOUCH_H
#define __TOUCH_H

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

/** @defgroup TOUCH_device_type
  * @{
  */

typedef struct
{
  void (*init)(void);                                   /*!< pin init */
  void (*touch_read_xy)(uint16_t *x, uint16_t *y);      /*!<  read x/y coordinate */
  void (*touch_scan)(void);                             /*!<  touch scan func */
  error_status (*touch_adjust)(void);                   /*!<  touch adjust func */
  uint16_t x_p[6];                                      /*!<  save x coordinate */
  uint16_t y_p[6];                                      /*!<  save y coordinate */
}touch_dev_type;

/**
  * @}
  */

#define TCLK_HIGH                        gpio_bits_set(GPIOB,GPIO_PINS_3)
#define TCLK_LOW                         gpio_bits_reset(GPIOB,GPIO_PINS_3)

#define TDOUT                            gpio_input_data_bit_read(GPIOB,GPIO_PINS_4)

#define TDIN_HIGH                        gpio_bits_set(GPIOB,GPIO_PINS_5)
#define TDIN_LOW                         gpio_bits_reset(GPIOB,GPIO_PINS_5)

#define PEN_HIGH                         gpio_input_data_bit_read(GPIOD,GPIO_PINS_5)

#define TCS_HIGH                         gpio_bits_set(GPIOB,GPIO_PINS_8)
#define TCS_LOW                          gpio_bits_reset(GPIOB,GPIO_PINS_8)

extern touch_dev_type touch_dev_struct;
extern touch_dev_type *touch_struct;

/** @defgroup TOUCH_exported_functions
  * @{
  */

void touch_pin_init(void);
void touch_read_xy(uint16_t *x, uint16_t *y);
void touch_scan(void);
uint16_t touch_read_x_or_y(uint8_t xy);
uint16_t touch_read_ad(uint8_t xy);
void touch_write_1byte(uint8_t cmd);
uint8_t touch_read_1byte(void);
error_status touch_adjust(void);

/**
  * @}
  */

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
