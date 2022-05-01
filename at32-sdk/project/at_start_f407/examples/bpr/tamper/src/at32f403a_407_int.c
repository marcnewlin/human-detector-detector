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

extern uint32_t bpr_reg_judge(void);

/** @addtogroup AT32F407_periph_examples
  * @{
  */

/** @addtogroup 407_BPR_tamper
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
  * @brief  this function handles tamper interrupt request.
  * @param  none
  * @retval none
  */
void TAMPER_IRQHandler(void)
{
  if(bpr_flag_get(BPR_TAMPER_INTERRUPT_FLAG) != RESET)
  {
    /* check if bpr registers are cleared */
    if(bpr_reg_judge() == 0)
    {
      /* ok, bpr registers are reset as expected */
      at32_led_on(LED3);
    }
    else
    {
      /* bpr registers are not reset */
      at32_led_off(LED3);
    }

    /* clear tamper pin interrupt pending bit */
    bpr_flag_clear(BPR_TAMPER_INTERRUPT_FLAG);

    /* clear tamper pin event(te) pending flag */
    bpr_flag_clear(BPR_TAMPER_EVENT_FLAG);

    /* disable tamper pin */
    bpr_tamper_pin_enable(FALSE);

    /* enable tamper pin */
    bpr_tamper_pin_enable(TRUE);
  }
}

/**
  * @}
  */

/**
  * @}
  */
