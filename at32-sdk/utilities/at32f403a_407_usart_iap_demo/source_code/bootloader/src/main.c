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
#include "tmr.h"
#include "usart.h"
#include "flash.h"
#include "iap.h"

/** @addtogroup UTILITIES_examples
  * @{
  */

/** @addtogroup USART_IAP_bootloader
  * @{
  */

/**
  * @brief  main function.
  * @param  none
  * @retval none
  */
int main(void)
{
  system_clock_config();
  at32_board_init();

  /* config nvic priority group */
  nvic_priority_group_config(NVIC_PRIORITY_GROUP_4);

  /* check iap_upgrade_flag flag */
  if(flash_upgrade_flag_read() == RESET)
  {
    /* check app starting address whether 0x08xxxxxx */
    if(((*(uint32_t*)(APP_START_ADDR + 4)) & 0xFF000000) == 0x08000000)
      app_load(APP_START_ADDR);
  }

  /* init usart used for app update */
  uart_init(115200);

  /* check whether need to upgrade, if yes, response ok to pc-tool */
  if(flash_upgrade_flag_read() != RESET)
    back_ok();

  /* init tmr used for show code running state(led cycle toggle) */
  tmr_init();

  while(1)
  {
    /* upgrade handle */
    iap_upgrade_app_handle();
  }
}

/**
  * @}
  */

/**
  * @}
  */
