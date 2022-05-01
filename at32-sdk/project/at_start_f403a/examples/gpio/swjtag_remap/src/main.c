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


/** @addtogroup AT32F403A_periph_examples
  * @{
  */

/** @addtogroup 403A_GPIO_swjtag_remap GPIO_swjtag_remap
  * @{
  */

void swj_dp_config(void);
void gpio_pins_toggle(gpio_type* gpio_x, uint16_t gpio_pin);

/**
  * @brief  configure the gpio of swj-dp.
  * @param  none
  * @retval none
  */
void swj_dp_config(void)
{
  gpio_init_type gpio_init_struct;

  /* configure pa13 (jtms/swdat), pa14 (jtck/swclk) and pa15 (jtdi) as output push-pull */
  gpio_init_struct.gpio_pins = GPIO_PINS_13 | GPIO_PINS_14 | GPIO_PINS_15;
  gpio_init_struct.gpio_mode = GPIO_MODE_OUTPUT;
  gpio_init_struct.gpio_pull = GPIO_PULL_NONE;
  gpio_init_struct.gpio_out_type = GPIO_OUTPUT_PUSH_PULL;
  gpio_init_struct.gpio_drive_strength = GPIO_DRIVE_STRENGTH_STRONGER;
  gpio_init(GPIOA, &gpio_init_struct);

  /* configure pb3 (jtdo) and pb4 (jtrst) as output push-pull */
  gpio_init_struct.gpio_pins = GPIO_PINS_3 | GPIO_PINS_4;
  gpio_init(GPIOB, &gpio_init_struct);
}

/**
  * @brief  toggles the specified gpio pin
  * @param  gpiox: where x can be (a..g depending on device used) to select the gpio peripheral
  * @param  gpio_pin: specifies the pins to be toggled.
  * @retval none
  */
void gpio_pins_toggle(gpio_type* gpio_x, uint16_t gpio_pin)
{
  gpio_x->odt ^= gpio_pin;
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

  crm_periph_clock_enable(CRM_IOMUX_PERIPH_CLOCK, TRUE);
  crm_periph_clock_enable(CRM_GPIOA_PERIPH_CLOCK, TRUE);
  crm_periph_clock_enable(CRM_GPIOB_PERIPH_CLOCK, TRUE);

  swj_dp_config();

  while(1)
  {
    if(USER_BUTTON == at32_button_press())
    {
      /* disable the serial wire jtag debug port swj-dp */
      gpio_pin_remap_config(SWJTAG_MUX_100, TRUE);

      /* turn on led3 */
      at32_led_on(LED3);
    }

    /* toggle jtms/swdat pin */
    gpio_pins_toggle(GPIOA, GPIO_PINS_13);
    delay_us(200);

    /* toggle jtck/swclk pin */
    gpio_pins_toggle(GPIOA, GPIO_PINS_14);
    delay_us(200);

    /* toggle jtdi pin */
    gpio_pins_toggle(GPIOA, GPIO_PINS_15);
    delay_us(200);

    /* toggle jtdo pin */
    gpio_pins_toggle(GPIOB, GPIO_PINS_3);
    delay_us(200);

    /* toggle jtrst pin */
    gpio_pins_toggle(GPIOB, GPIO_PINS_4);
    delay_us(200);
  }
}

/**
  * @}
  */

/**
  * @}
  */
