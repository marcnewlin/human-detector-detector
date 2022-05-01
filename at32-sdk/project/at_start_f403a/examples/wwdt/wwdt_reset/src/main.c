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

/** @addtogroup 403A_WWDT_reset WWDT_reset
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

  if(crm_flag_get(CRM_WWDT_RESET_FLAG) != RESET)
  {
    /* reset from wwdt */
    crm_flag_clear(CRM_WWDT_RESET_FLAG);

    at32_led_on(LED4);
  }
  else
  {
    /* reset from other mode */
    at32_led_off(LED4);
  }

  /* enable the wwdt clock */
  crm_periph_clock_enable(CRM_WWDT_PERIPH_CLOCK, TRUE);

  /* set the wwdt divider value */
  wwdt_divider_set(WWDT_PCLK1_DIV_32768);

  /* set the wwdt window counter value */
  wwdt_window_counter_set(0x6F);

  /* enable wwdt

   window_value:   (0x7F - 0x6F)
   timeout_value:  (0x7F - 0x40) + 1

   timeout = timeout_value * (divider / pclk1_freq )    (s)
   window  = window_value  * (divider / pclk1_freq )    (s)

   pclk1_freq   = 120 MHz
   divider      = 32768
   reload_value = 0x40 = 64
   window_value = 0x10 = 16

   timeout = 64 * (32768 / 120000000 ) = 0.0174s = 17.4ms
   window  = 16 * (32768 / 120000000 ) = 0.0044s = 4.4ms

  */
  wwdt_enable(0x7F);

  while(1)
  {
    at32_led_toggle(LED3);

    /* the reload time must within the window (6ms > 4.4ms) */
    delay_ms(6);

    /* update the wwdt window counter */
    wwdt_counter_set(0x7F);

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
