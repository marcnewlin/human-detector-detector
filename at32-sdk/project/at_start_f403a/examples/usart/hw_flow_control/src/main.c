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

/** @addtogroup 403A_USART_hw_flow_control USART_hw_flow_control
  * @{
  */

#define BUFFER_SIZE                      16

uint8_t rx_buffer[BUFFER_SIZE];
uint8_t data_count;

/**
  * @brief  config usart
  * @param  none
  * @retval none
  */
void usart_configuration(void)
{
  gpio_init_type gpio_init_struct;

  /* enable the usart2 and gpio clock */
  crm_periph_clock_enable(CRM_USART2_PERIPH_CLOCK, TRUE);
  crm_periph_clock_enable(CRM_GPIOD_PERIPH_CLOCK, TRUE);
  crm_periph_clock_enable(CRM_IOMUX_PERIPH_CLOCK, TRUE);

  gpio_default_para_init(&gpio_init_struct);

  /* configure the usart2 tx and rts pin */
  gpio_init_struct.gpio_drive_strength = GPIO_DRIVE_STRENGTH_STRONGER;
  gpio_init_struct.gpio_out_type = GPIO_OUTPUT_PUSH_PULL;
  gpio_init_struct.gpio_mode = GPIO_MODE_MUX;
  gpio_init_struct.gpio_pins = GPIO_PINS_4 | GPIO_PINS_5;
  gpio_init_struct.gpio_pull = GPIO_PULL_NONE;
  gpio_init(GPIOD, &gpio_init_struct);

  /* configure the usart2 rx and cts pin */
  gpio_init_struct.gpio_drive_strength = GPIO_DRIVE_STRENGTH_STRONGER;
  gpio_init_struct.gpio_out_type = GPIO_OUTPUT_PUSH_PULL;
  gpio_init_struct.gpio_mode = GPIO_MODE_INPUT;
  gpio_init_struct.gpio_pins = GPIO_PINS_3 | GPIO_PINS_6;
  gpio_init_struct.gpio_pull = GPIO_PULL_UP;
  gpio_init(GPIOD, &gpio_init_struct);

  /* config usart2 iomux */
  gpio_pin_remap_config(USART2_GMUX_0001, TRUE);

  /* configure usart2 param */
  usart_init(USART2, 115200, USART_DATA_8BITS, USART_STOP_1_BIT);
  usart_hardware_flow_control_set(USART2, USART_HARDWARE_FLOW_RTS_CTS);
  usart_transmitter_enable(USART2, TRUE);
  usart_receiver_enable(USART2, TRUE);
  usart_enable(USART2, TRUE);
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
  usart_configuration();

  /* receive a string (BUFFER_SIZE bytes) from the hyperterminal */
  data_count = BUFFER_SIZE;
  while(data_count)
  {
    while(usart_flag_get(USART2, USART_RDBF_FLAG) == RESET);
    rx_buffer[BUFFER_SIZE-data_count] = usart_data_receive(USART2);
    data_count--;
  }

  /* send a buffer from usart to hyperterminal */
  data_count = BUFFER_SIZE;
  while(data_count)
  {
    while(usart_flag_get(USART2, USART_TDBE_FLAG) == RESET);
    usart_data_transmit(USART2, rx_buffer[BUFFER_SIZE-data_count]);
    data_count--;
  }

  while(1)
  {
    at32_led_toggle(LED2);
    at32_led_toggle(LED3);
    at32_led_toggle(LED4);
    delay_sec(1);
  }
}

/**
  * @}
  */

/**
  * @}
  */
