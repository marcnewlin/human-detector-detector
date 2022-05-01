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
#include "run_in_spim.h"


/** @addtogroup AT32F407_periph_examples
  * @{
  */

/** @addtogroup 407_FLASH_run_in_spim FLASH_run_in_spim
  * @{
  */

/**
  * @brief  init the spim flash
  * @param  none
  * @retval none
  */
void spim_init(void)
{
  gpio_init_type gpio_init_struct;

  /* enable the clock */
  crm_periph_clock_enable(CRM_IOMUX_PERIPH_CLOCK, TRUE);
  crm_periph_clock_enable(CRM_GPIOA_PERIPH_CLOCK, TRUE);
  crm_periph_clock_enable(CRM_GPIOB_PERIPH_CLOCK, TRUE);

  /* init spim io */
  gpio_default_para_init(&gpio_init_struct);
  gpio_init_struct.gpio_drive_strength = GPIO_DRIVE_STRENGTH_STRONGER;
  gpio_init_struct.gpio_out_type = GPIO_OUTPUT_PUSH_PULL;
  gpio_init_struct.gpio_mode = GPIO_MODE_MUX;
  gpio_init_struct.gpio_pull = GPIO_PULL_NONE;
  gpio_init_struct.gpio_pins = GPIO_PINS_8;
  gpio_init(GPIOA, &gpio_init_struct);
  gpio_init_struct.gpio_pins = GPIO_PINS_1 | GPIO_PINS_6 | GPIO_PINS_7 | GPIO_PINS_10 | GPIO_PINS_11;
  gpio_init(GPIOB, &gpio_init_struct);

  /* enable spim, and select pb10, pb11 as spim io */
  gpio_pin_remap_config(EXT_SPIM_GMUX_1001, TRUE);

  /* in this example, use on-board en25qh128a as spim flash */
  flash_spim_model_select(FLASH_SPIM_MODEL2);

  /* unlock the spim flash program erase controller */
  while(flash_flag_get(FLASH_SPIM_OBF_FLAG));
  flash_spim_unlock();
  while(FLASH->ctrl3_bit.oplk);

  /* if the data written to spim flash need to be scrambled, please specify the scrambled range */
  flash_spim_encryption_range_set(0);

  return;
}

/**
  * @brief  main function.
  * @param  none
  * @retval none
  */
int main(void)
{
  system_clock_config();
  at32_board_init();

  /* configures the spim flash */
  spim_init();

  /* check the led toggle in spim */
  spim_run();

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
