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

extern __IO uint16_t ccr1_val;
extern __IO uint16_t ccr2_val;
extern __IO uint16_t ccr3_val;
extern __IO uint16_t ccr4_val;

/** @addtogroup AT32F403A_periph_examples
  * @{
  */

/** @addtogroup 403A_TMR_pwm_output_simulate
  * @{
  */

uint16_t capture = 0;

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

void TMR2_GLOBAL_IRQHandler(void)
{
  if(tmr_flag_get(TMR2, TMR_C1_FLAG) != RESET)
  {
    tmr_flag_clear(TMR2, TMR_C1_FLAG );

    /* Pin PC.06 toggling with frequency = 73.24 Hz */
    gpio_bits_write(GPIOC, GPIO_PINS_6, (confirm_state)(1 - gpio_output_data_bit_read(GPIOC, GPIO_PINS_6)));
    capture = tmr_channel_value_get(TMR2, TMR_SELECT_CHANNEL_1);
    tmr_channel_value_set(TMR2, TMR_SELECT_CHANNEL_1, capture + ccr1_val);
  }
  else if(tmr_flag_get(TMR2, TMR_C2_FLAG) != RESET)
  {
    tmr_flag_clear(TMR2, TMR_C2_FLAG);

    /* Pin PC.06 toggling with frequency = 73.24 Hz */
    gpio_bits_write(GPIOC, GPIO_PINS_7, (confirm_state)(1 - gpio_output_data_bit_read(GPIOC, GPIO_PINS_7)));
    capture = tmr_channel_value_get(TMR2, TMR_SELECT_CHANNEL_2);
    tmr_channel_value_set(TMR2, TMR_SELECT_CHANNEL_2, capture + ccr2_val);
  }
  else if(tmr_flag_get(TMR2, TMR_C3_FLAG) != RESET)
  {
    tmr_flag_clear(TMR2, TMR_C3_FLAG);

    /* Pin PC.06 toggling with frequency = 73.24 Hz */
    gpio_bits_write(GPIOC, GPIO_PINS_8, (confirm_state)(1 - gpio_output_data_bit_read(GPIOC, GPIO_PINS_8)));
    capture = tmr_channel_value_get(TMR2, TMR_SELECT_CHANNEL_3);
    tmr_channel_value_set(TMR2, TMR_SELECT_CHANNEL_3, capture + ccr3_val);
  }
  else
  {
    tmr_flag_clear(TMR2, TMR_C4_FLAG);

    /* Pin PC.06 toggling with frequency = 73.24 Hz */
    gpio_bits_write(GPIOC, GPIO_PINS_9, (confirm_state)(1 - gpio_output_data_bit_read(GPIOC, GPIO_PINS_9)));
    capture = tmr_channel_value_get(TMR2, TMR_SELECT_CHANNEL_4);
    tmr_channel_value_set(TMR2, TMR_SELECT_CHANNEL_4, capture + ccr4_val);
  }
}

/**
  * @}
  */

/**
  * @}
  */
