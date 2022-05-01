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
#include "stdio.h"

extern uint32_t incrementvar_operationcomplete(void);
extern uint32_t getvar_operationcomplete(void);
extern void setvar_periodvalue(uint32_t Value);

extern __IO uint32_t periodvalue;
extern __IO uint32_t operationcomplete;

/** @addtogroup AT32F403A_periph_examples
  * @{
  */

/** @addtogroup 403A_RTC_lick_calibration
  * @{
  */

uint16_t tmpCC4[2] = {0, 0};

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
  * @brief  this function handles rtc global interrupt request.
  * @param  none
  * @retval none
  */
void RTC_IRQHandler(void)
{
  if(rtc_flag_get(RTC_TS_FLAG) != RESET)
  {
    /* toggle led3 */
    at32_led_toggle(LED3);

    /* clear interrupt pending bit */
    rtc_flag_clear(RTC_TS_FLAG);

    printf("%d\r\n", rtc_counter_get());
  }
}

/**
  * @brief  this function handles tmr5 global interrupt request.
  * @param  none
  * @retval none
  */
void TMR5_GLOBAL_IRQHandler(void)
{
  uint32_t tmp = 0;

  if(tmr_flag_get(TMR5, TMR_C4_FLAG) == SET)
  {
    tmpCC4[incrementvar_operationcomplete()] = (uint16_t)(TMR5->c4dt);

    tmr_flag_get(TMR5, TMR_C4_FLAG);

    if(getvar_operationcomplete() >= 2)
    {
      /* compute the period length */
      tmp = (uint16_t)(tmpCC4[1] - tmpCC4[0] + 1);
      setvar_periodvalue(tmp);

      /* disable the interrupt */
      tmr_interrupt_enable(TMR5, TMR_C4_INT, FALSE);
      tmr_counter_enable(TMR5, FALSE);
    }
  }
}

/**
  * @}
  */

/**
  * @}
  */
