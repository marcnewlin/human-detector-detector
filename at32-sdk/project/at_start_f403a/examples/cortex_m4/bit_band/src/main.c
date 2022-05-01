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

/** @addtogroup 403A_CORTEX_m4_bit_band CORTEX_m4_bit_band
  * @{
  */


/* bit band for sram  */
#define RAM_BASE                         0x20000000
#define RAM_BITBAND_BASE                 0x22000000

#define VARIABLES_RESET_BIT(variables_addr, bit_number)    \
        (*(uint32_t *)(RAM_BITBAND_BASE + ((variables_addr - RAM_BASE) * 32) + ((bit_number) * 4)) = 0)

#define VARIABLES_SET_BIT(variables_addr, bit_number)       \
        (*(uint32_t *)(RAM_BITBAND_BASE + ((variables_addr - RAM_BASE) * 32) + ((bit_number) * 4)) = 1)

#define VARIABLES_GET_BIT(variables_addr, bit_number)       \
        (*(uint32_t *)(RAM_BITBAND_BASE + ((variables_addr - RAM_BASE) * 32) + ((bit_number) * 4)))

/* bit band for peripheral  */
#define PERIPHERAL_BASE                  0x40000000
#define PERIPHERAL_BITBAND_BASE          0x42000000

#define PERIPHERAL_RESET_BIT(peripheral_addr, bit_number)    \
        (*(uint32_t *)(PERIPHERAL_BITBAND_BASE + ((peripheral_addr - PERIPHERAL_BASE) * 32) + ((bit_number) * 4)) = 0)

#define PERIPHERAL_SET_BIT(peripheral_addr, bit_number)       \
        (*(uint32_t *)(PERIPHERAL_BITBAND_BASE + ((peripheral_addr - PERIPHERAL_BASE) * 32) + ((bit_number) * 4)) = 1)

__IO uint32_t variables, variables_addr = 0, variables_bit_val = 0;

/**
  * @brief  led4 flick for error.
  * @param  none
  * @retval none
  */
void result_error(void)
{
  while(1)
  {
    at32_led_toggle(LED4);
    delay_sec(1);
  }
}

/**
  * @brief  main function.
  * @param  none
  * @retval none
  */
int main(void)
{
  system_clock_config();
  at32_board_init();
  variables = 0xA5A5A5A5;

  /* get the variables address */
  variables_addr = (uint32_t)&variables;

  /* modify variables bit0 and check result */
  VARIABLES_RESET_BIT(variables_addr, 0);
  if((variables != 0xA5A5A5A4) || (VARIABLES_GET_BIT(variables_addr, 0) != 0))
  {
    result_error();
  }

  /* modify variables bit0 and check result */
  VARIABLES_SET_BIT(variables_addr, 0);
  if((variables != 0xA5A5A5A5) || (VARIABLES_GET_BIT(variables_addr, 0) != 1))
  {
    result_error();
  }

  /* modify variables bit16 and check result */
  VARIABLES_RESET_BIT(variables_addr, 16);
  if((variables != 0xA5A4A5A5) || (VARIABLES_GET_BIT(variables_addr, 16) != 0))
  {
    result_error();
  }

  /* modify variables bit16 and check result */
  VARIABLES_SET_BIT(variables_addr, 16);
  if((variables != 0xA5A5A5A5) || (VARIABLES_GET_BIT(variables_addr, 16) != 1))
  {
    result_error();
  }

  /* modify variables bit31 and check result */
  VARIABLES_RESET_BIT(variables_addr, 31);
  if((variables != 0x25A5A5A5) || (VARIABLES_GET_BIT(variables_addr, 31) != 0))
  {
    result_error();
  }

  /* modify variables bit31 and check result */
  VARIABLES_SET_BIT(variables_addr, 31);
  if((variables != 0xA5A5A5A5) || (VARIABLES_GET_BIT(variables_addr, 31) != 1))
  {
    result_error();
  }

  while(1)
  {
    /* led2 toggle */
    PERIPHERAL_RESET_BIT((uint32_t)&LED2_GPIO->odt, 13);
    delay_ms(500);
    PERIPHERAL_SET_BIT((uint32_t)&LED2_GPIO->odt, 13);
    delay_ms(500);
  }
}

/**
  * @}
  */

/**
  * @}
  */
