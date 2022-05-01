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

extern uint8_t usart2_tx_dma_status;
extern uint8_t usart2_rx_dma_status;
extern uint8_t usart3_tx_dma_status;
extern uint8_t usart3_rx_dma_status;

/** @addtogroup AT32F403A_periph_examples
  * @{
  */

/** @addtogroup 403A_USART_transfer_by_dma_interrupt
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
  * @brief  this function handles dma1 channel1 handler.
  * @param  none
  * @retval none
  */
void DMA1_Channel1_IRQHandler(void)
{
  if(dma_flag_get(DMA1_FDT1_FLAG))
  {
    usart2_tx_dma_status = 1;
    dma_flag_clear(DMA1_FDT1_FLAG);
    dma_channel_enable(DMA1_CHANNEL1, FALSE);
  }
}

/**
  * @brief  this function handles dma1 channel2 handler.
  * @param  none
  * @retval none
  */
void DMA1_Channel2_IRQHandler(void)
{
  if(dma_flag_get(DMA1_FDT2_FLAG))
  {
    usart2_rx_dma_status = 1;
    dma_flag_clear(DMA1_FDT2_FLAG);
    dma_channel_enable(DMA1_CHANNEL2, FALSE);
  }
}

/**
  * @brief  this function handles dma1 channel3 handler.
  * @param  none
  * @retval none
  */
void DMA1_Channel3_IRQHandler(void)
{
  if(dma_flag_get(DMA1_FDT3_FLAG))
  {
    usart3_tx_dma_status = 1;
    dma_flag_clear(DMA1_FDT3_FLAG);
    dma_channel_enable(DMA1_CHANNEL3, FALSE);
  }
}

/**
  * @brief  this function handles dma1 channel4 handler.
  * @param  none
  * @retval none
  */
void DMA1_Channel4_IRQHandler(void)
{
  if(dma_flag_get(DMA1_FDT4_FLAG))
  {
    usart3_rx_dma_status = 1;
    dma_flag_clear(DMA1_FDT4_FLAG);
    dma_channel_enable(DMA1_CHANNEL4, FALSE);
  }
}

/**
  * @}
  */

/**
  * @}
  */
