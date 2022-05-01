/**
  **************************************************************************
  * @file     usart.c
  * @version  v2.0.8
  * @date     2022-04-02
  * @brief    usart program
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

#include "usart.h"
#include "tmr.h"

/** @addtogroup UTILITIES_examples
  * @{
  */

/** @addtogroup USART_IAP_app_led3_toggle
  * @{
  */

uint8_t iap_flag = IAP_REV_FLAG_NO;

/**
  * @brief  init usart.
  * @param  none
  * @retval none
  */
void uart_init(uint32_t baudrate)
{
  gpio_init_type gpio_init_struct;
  /* enable the usart and it's io clock */
  crm_periph_clock_enable(CRM_USART1_PERIPH_CLOCK, TRUE);
  crm_periph_clock_enable(CRM_GPIOA_PERIPH_CLOCK, TRUE);

  /* set default parameter */
  gpio_default_para_init(&gpio_init_struct);

  /* configure the usart1_tx  pa9 */
  gpio_init_struct.gpio_drive_strength = GPIO_DRIVE_STRENGTH_STRONGER;
  gpio_init_struct.gpio_out_type  = GPIO_OUTPUT_PUSH_PULL;
  gpio_init_struct.gpio_mode = GPIO_MODE_MUX;
  gpio_init_struct.gpio_pins = GPIO_PINS_9;
  gpio_init_struct.gpio_pull = GPIO_PULL_NONE;
  gpio_init(GPIOA, &gpio_init_struct);

  /* configure the usart1_rx  pa10 */
  gpio_init_struct.gpio_drive_strength = GPIO_DRIVE_STRENGTH_STRONGER;
  gpio_init_struct.gpio_out_type  = GPIO_OUTPUT_PUSH_PULL;
  gpio_init_struct.gpio_mode = GPIO_MODE_INPUT;
  gpio_init_struct.gpio_pins = GPIO_PINS_10;
  gpio_init_struct.gpio_pull = GPIO_PULL_UP;
  gpio_init(GPIOA, &gpio_init_struct);

  /*configure usart nvic interrupt */
  nvic_irq_enable(USART1_IRQn, 0, 0);

  /*configure usart param*/
  usart_init(USART1, baudrate, USART_DATA_8BITS, USART_STOP_1_BIT);
  usart_transmitter_enable(USART1, TRUE);
  usart_receiver_enable(USART1, TRUE);
  usart_interrupt_enable(USART1, USART_RDBF_INT, TRUE);
  usart_enable(USART1, TRUE);
}

/**
  * @brief  usart1 interrupt handler.
  * @param  none
  * @retval none
  */
void USART1_IRQHandler(void)
{
  uint16_t reval;
  if(usart_flag_get(USART1, USART_RDBF_FLAG) != RESET)
  {
    reval = usart_data_receive(USART1);
    if((reval == 0x5A) && (iap_flag == IAP_REV_FLAG_NO))
      iap_flag = IAP_REV_FLAG_5A;
    else if((reval == 0xA5) && (iap_flag == IAP_REV_FLAG_5A))
      iap_flag = IAP_REV_FLAG_DONE;
    else
      iap_flag = IAP_REV_FLAG_NO;
  }
}

/**
  * @brief  command response ok.
  * @param  none
  * @retval none
  */
void back_ok(void)
{
  usart_data_transmit(USART1, 0xCC);
  while(usart_flag_get(USART1, USART_TDC_FLAG) == RESET);
  usart_data_transmit(USART1, 0xDD);
  while(usart_flag_get(USART1, USART_TDC_FLAG) == RESET);
}

/**
  * @brief  command response error.
  * @param  none
  * @retval none
  */
void back_err(void)
{
  usart_data_transmit(USART1, 0xEE);
  while(usart_flag_get(USART1, USART_TDC_FLAG) == RESET);
  usart_data_transmit(USART1, 0xFF);
  while(usart_flag_get(USART1, USART_TDC_FLAG) == RESET);
}

/**
  * @}
  */

/**
  * @}
  */
