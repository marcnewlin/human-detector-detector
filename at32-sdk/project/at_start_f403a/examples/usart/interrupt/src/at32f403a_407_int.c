/**
  **************************************************************************
  * @file     at32f403a_407_int.c
  * @version  v2.0.8
  * @date     2022-04-02
  * @brief    main interrupt service routines.
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

/* includes ------------------------------------------------------------------*/
#include "at32f403a_407_int.h"

extern uint8_t usart2_tx_buffer[];
extern uint8_t usart3_tx_buffer[];
extern uint8_t usart2_rx_buffer[];
extern uint8_t usart3_rx_buffer[];
extern uint8_t usart2_tx_counter;
extern uint8_t usart3_tx_counter;
extern uint8_t usart2_rx_counter;
extern uint8_t usart3_rx_counter;
extern uint8_t usart2_tx_buffer_size;
extern uint8_t usart3_tx_buffer_size;

/** @addtogroup AT32F403A_periph_examples
  * @{
  */

/** @addtogroup 403A_USART_interrupt
  * @{
  */

/**
  * @brief  this function handles nmi exception.
  * @param  none
  * @retval none
  */
void NMI_Handler(void)
{
}

/**
  * @brief  this function handles hard fault exception.
  * @param  none
  * @retval none
  */
void HardFault_Handler(void)
{
  /* go to infinite loop when hard fault exception occurs */
  while(1)
  {
  }
}

/**
  * @brief  this function handles memory manage exception.
  * @param  none
  * @retval none
  */
void MemManage_Handler(void)
{
  /* go to infinite loop when memory manage exception occurs */
  while(1)
  {
  }
}

/**
  * @brief  this function handles bus fault exception.
  * @param  none
  * @retval none
  */
void BusFault_Handler(void)
{
  /* go to infinite loop when bus fault exception occurs */
  while(1)
  {
  }
}

/**
  * @brief  this function handles usage fault exception.
  * @param  none
  * @retval none
  */
void UsageFault_Handler(void)
{
  /* go to infinite loop when usage fault exception occurs */
  while(1)
  {
  }
}

/**
  * @brief  this function handles svcall exception.
  * @param  none
  * @retval none
  */
void SVC_Handler(void)
{
}

/**
  * @brief  this function handles debug monitor exception.
  * @param  none
  * @retval none
  */
void DebugMon_Handler(void)
{
}

/**
  * @brief  this function handles pendsv_handler exception.
  * @param  none
  * @retval none
  */
void PendSV_Handler(void)
{
}

/**
  * @brief  this function handles systick handler.
  * @param  none
  * @retval none
  */
void SysTick_Handler(void)
{
}

/**
  * @brief  this function handles usart2 handler.
  * @param  none
  * @retval none
  */
void USART2_IRQHandler(void)
{
  if(usart_flag_get(USART2, USART_RDBF_FLAG) != RESET)
  {
    /* read one byte from the receive data register */
    usart2_rx_buffer[usart2_rx_counter++] = usart_data_receive(USART2);

    if(usart2_rx_counter == usart3_tx_buffer_size)
    {
      /* disable the usart2 receive interrupt */
      usart_interrupt_enable(USART2, USART_RDBF_INT, FALSE);
    }
  }

  if(usart_flag_get(USART2, USART_TDBE_FLAG) != RESET)
  {
    /* write one byte to the transmit data register */
    usart_data_transmit(USART2, usart2_tx_buffer[usart2_tx_counter++]);

    if(usart2_tx_counter == usart2_tx_buffer_size)
    {
      /* disable the usart2 transmit interrupt */
      usart_interrupt_enable(USART2, USART_TDBE_INT, FALSE);
    }
  }
}

/**
  * @brief  this function handles usart3 handler.
  * @param  none
  * @retval none
  */
void USART3_IRQHandler(void)
{
  if(usart_flag_get(USART3, USART_RDBF_FLAG) != RESET)
  {
    /* read one byte from the receive data register */
    usart3_rx_buffer[usart3_rx_counter++] = usart_data_receive(USART3);

    if(usart3_rx_counter == usart2_tx_buffer_size)
    {
      /* disable the usart3 receive interrupt */
      usart_interrupt_enable(USART3, USART_RDBF_INT, FALSE);
    }
  }

  if(usart_flag_get(USART3, USART_TDBE_FLAG) != RESET)
  {
    /* write one byte to the transmit data register */
    usart_data_transmit(USART3, usart3_tx_buffer[usart3_tx_counter++]);

    if(usart3_tx_counter == usart3_tx_buffer_size)
    {
      /* disable the usart3 transmit interrupt */
      usart_interrupt_enable(USART3, USART_TDBE_INT, FALSE);
    }
  }
}

/**
  * @}
  */

/**
  * @}
  */
