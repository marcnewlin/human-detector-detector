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

/** @addtogroup 403A_WDT_reset WDT_reset
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

  if(crm_flag_get(CRM_WDT_RESET_FLAG) != RESET)
  {
    /* reset from wdt */
    crm_flag_clear(CRM_WDT_RESET_FLAG);

    at32_led_on(LED4);
  }
  else
  {
    /* reset from other mode */
    at32_led_off(LED4);
  }

  /* disable register write protection */
  wdt_register_write_enable(TRUE);

  /* set the wdt divider value */
  wdt_divider_set(WDT_CLK_DIV_4);

  /* set reload value

   timeout = reload_value * (divider / lick_freq )    (s)

   lick_freq    = 40000 Hz
   divider      = 4
   reload_value = 3000

   timeout = 3000 * (4 / 40000 ) = 0.3s = 300ms
  */
  wdt_reload_value_set(3000 - 1);

  /* enable wdt */
  wdt_enable();

  while(1)
  {
    /* reload wdt counter */
    wdt_counter_reload();

    at32_led_toggle(LED3);

    delay_ms(200);

    if(at32_button_press() == USER_BUTTON)
    {
      while(1);
    }
  }
}

/**
  * @}
  */

/**
  * @}
  */
