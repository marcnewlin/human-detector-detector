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
#include "at32f403a_407_board.h"

extern uint8_t spi1_tx_buffer[];
extern uint8_t spi2_tx_buffer[];
extern uint8_t spi1_rx_buffer[];
extern uint8_t spi2_rx_buffer[];
extern uint32_t tx_index;
extern uint32_t rx_index;

/** @addtogroup AT32F407_periph_examples
  * @{
  */

/** @addtogroup 407_SPI_halfduplex_transceiver_switch
  * @{
  */

#define BUFFERSIZE 32

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
  * @brief  This function handles the spi1 interrupt request.
  * @param  None
  * @retval None
  */
 void SPI1_IRQHandler(void)
{
  if(SPI1->ctrl2_bit.tdbeie != RESET)
  {
    if(spi_i2s_flag_get(SPI1, SPI_I2S_TDBE_FLAG) != RESET)
    {
      spi_i2s_data_transmit(SPI1, spi1_tx_buffer[tx_index++]);
      if(tx_index == BUFFERSIZE)
      {
        spi_i2s_interrupt_enable(SPI1, SPI_I2S_TDBE_INT, FALSE);
      }
    }
  }
  if(SPI1->ctrl2_bit.rdbfie != RESET)
  {
    if(spi_i2s_flag_get(SPI1, SPI_I2S_RDBF_FLAG) != RESET)
    {
      spi_enable(SPI1, FALSE);
      spi1_rx_buffer[rx_index++] = spi_i2s_data_receive(SPI1);
      spi_enable(SPI1, TRUE);
      if(rx_index == BUFFERSIZE)
      {
        spi_i2s_interrupt_enable(SPI1, SPI_I2S_RDBF_FLAG, FALSE);
      }
    }
  }
}

/**
  * @brief  This function handles the spi2 interrupt request.
  * @param  None
  * @retval None
  */
 void SPI2_I2S2EXT_IRQHandler(void)
{
  if(SPI2->ctrl2_bit.tdbeie != RESET)
  {
    if(spi_i2s_flag_get(SPI2, SPI_I2S_TDBE_FLAG) != RESET)
    {
      spi_i2s_data_transmit(SPI2, spi2_tx_buffer[tx_index++]);
      if(tx_index == BUFFERSIZE)
      {
        spi_i2s_interrupt_enable(SPI2, SPI_I2S_TDBE_INT, FALSE);
      }
    }
  }
  if(SPI2->ctrl2_bit.rdbfie != RESET)
  {
    if(spi_i2s_flag_get(SPI2, SPI_I2S_RDBF_FLAG) != RESET)
    {
      spi2_rx_buffer[rx_index++] = spi_i2s_data_receive(SPI2);
    }
  }
}

/**
  * @}
  */

/**
  * @}
  */
