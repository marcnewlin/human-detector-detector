/**
  **************************************************************************
  * @file     smartcard_config.h
  * @version  v2.0.8
  * @date     2022-04-02
  * @brief    header file of clock program
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

/* define to prevent recursive inclusion -------------------------------------*/
#ifndef __SMARTCARD_CONFIG_H
#define __SMARTCARD_CONFIG_H

#ifdef __cplusplus
extern "C" {
#endif

/* smartcard inteface usart pins */
#define SC_USART                 USART2
#define SC_USART_CLK             CRM_USART2_PERIPH_CLOCK
#define SC_USART_GPIO_CLK        CRM_GPIOA_PERIPH_CLOCK
#define SC_USART_TX_PIN          GPIO_PINS_2
#define SC_USART_TX_PORT         GPIOA
#define SC_USART_CLK_PIN         GPIO_PINS_4
#define SC_USART_CLK_PORT        GPIOA
#define SC_USART_IRQn            USART2_IRQn
#define SC_USART_IRQHandler      USART2_IRQHandler

/* smartcard inteface gpio pins */
#define SC_3_5V_PIN              GPIO_PINS_5
#define SC_RESET_PIN             GPIO_PINS_6
#define SC_CMDVCC_PIN            GPIO_PINS_7
#define SC_OFF_PIN               GPIO_PINS_8
#define SC_3_5V_PORT             GPIOA
#define SC_RESET_PORT            GPIOA
#define SC_CMDVCC_PORT           GPIOA
#define SC_OFF_PORT              GPIOA
#define SC_3_5V_CLK              CRM_GPIOA_PERIPH_CLOCK
#define SC_RESET_CLK             CRM_GPIOA_PERIPH_CLOCK
#define SC_CMDVCC_CLK            CRM_GPIOA_PERIPH_CLOCK
#define SC_OFF_CLK               CRM_GPIOA_PERIPH_CLOCK
#define SC_OFF_EXINT             EXINT_LINE_8
#define SC_OFF_PORTSOURCE        GPIO_PORT_SOURCE_GPIOA
#define SC_OFF_PINSOURCE         GPIO_PINS_SOURCE8
#define SC_OFF_EXINT_IRQ         EXINT9_5_IRQn

#ifdef __cplusplus
}
#endif

#endif /* __SMARTCARD_CONFIG_H */

