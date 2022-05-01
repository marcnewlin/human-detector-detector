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
#include <stdio.h>
#include "math_helper.h"
#include "fir_filter.h"

extern float32_t testInput_f32_1kHz_15kHz[];
extern float32_t refOutput[];

/** @addtogroup UTILITIES_examples
  * @{
  */

/** @addtogroup SLIB_project_l1
  * @{
  */

#define SNR_THRESHOLD_F32                140.0f
#define TEST_LENGTH_SAMPLES              320

static float32_t testOutput[TEST_LENGTH_SAMPLES];

void config_flash_interrupt(void);

/**
  * @brief  config flash error interrupt.
  * @param  none
  * @retval flash status
  */
void config_flash_interrupt(void)
{
  flash_unlock();
  flash_interrupt_enable(FLASH_BANK1_ERR_INT, TRUE);
  flash_lock();
  /* config nvic priority group */
  nvic_priority_group_config(NVIC_PRIORITY_GROUP_4);

  nvic_irq_enable(FLASH_IRQn, 0, 0);
}

/**
  * @brief  main function.
  * @param  none
  * @retval none
  */
int main(void)
{
  arm_status status;
  float32_t  *inputf32, *outputf32, snr;

  system_clock_config();
  at32_board_init();

  /* initialize input and output buffer pointers */
  inputf32 = &testInput_f32_1kHz_15kHz[0];
  outputf32 = &testOutput[0];

 /* configure flash to generate an interrupt when a write protect error occur */
  config_flash_interrupt();

  /* wait for key button to be pushed, then turn off led3 */
  while(at32_button_press() == NO_BUTTON)
  {
    at32_led_toggle(LED3);
    delay_ms(100);
  }
  at32_led_off(LED3);

  /* call the fir low pass filter function */
  FIR_lowpass_filter(inputf32, outputf32, TEST_LENGTH_SAMPLES);

  /* compare the generated output against the reference output computed in matlab. */
  snr = arm_snr_f32(&refOutput[0], &testOutput[0], TEST_LENGTH_SAMPLES);
  if(snr < SNR_THRESHOLD_F32)
  {
    status = ARM_MATH_TEST_FAILURE;
  }
  else
  {
    status = ARM_MATH_SUCCESS;
  }

  /* infinite loop */
  while(1)
  {
    if(status == ARM_MATH_TEST_FAILURE)
    {
      /* toggle red led2 if fir function failed */
      at32_led_toggle(LED2);
      delay_ms(500);
    }
    else
    {
      /* toggle green led4 if fir function success */
      at32_led_toggle(LED4);
      delay_ms(500);
    }
  }
}

/**
  * @}
  */

/**
  * @}
  */



