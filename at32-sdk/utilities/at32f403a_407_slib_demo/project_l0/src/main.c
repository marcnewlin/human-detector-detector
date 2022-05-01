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

/** @addtogroup SLIB_project_l0
  * @{
  */

#define USE_SLIB_FUNCTION       /* define it when user want to activate slib in this project */

#if defined (USE_SLIB_FUNCTION)
#define FLASH_START_ADDR                 0x08000000
#define TEST_SLIB_PSW                    0x55665566
#define SLIB_START_ADDR                  0x08001000
#define SLIB_DATA_ADDR                   0x08002000
#define SLIB_END_ADDR                    0x08002800
#define FLASH_SECTOR_SIZE                0x800
#define SECTOR_NUM(dwAddr)               ((dwAddr & (FLASH_START_ADDR - 1)) / FLASH_SECTOR_SIZE)
#define SECTOR_WORD_CNT                  (FLASH_SECTOR_SIZE >> 2)    /* words per sector */
#endif
#define SNR_THRESHOLD_F32                140.0f
#define TEST_LENGTH_SAMPLES              320

#if defined (USE_SLIB_FUNCTION)
uint32_t dw_start_sector, dw_data_start_sector, dw_end_sector;
uint32_t temp_buf[512];       /* 2kb buffer */
#endif

static float32_t testOutput[TEST_LENGTH_SAMPLES];

#if defined (USE_SLIB_FUNCTION)
flash_status_type slib_enable(void);
#endif
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

#if defined (USE_SLIB_FUNCTION)
/**
  * @brief  config slib enable and program slib area.
  * @param  none
  * @retval flash status
  */
flash_status_type slib_enable(void)
{
  uint32_t *ptr;
  uint32_t i, j;
  flash_status_type status = FLASH_OPERATE_DONE;
  ptr = (uint32_t *)SLIB_START_ADDR;
  dw_start_sector = SECTOR_NUM(SLIB_START_ADDR);            /* slib start sector */
  dw_data_start_sector = SECTOR_NUM(SLIB_DATA_ADDR);         /* slib data sector */
  dw_end_sector = SECTOR_NUM(SLIB_END_ADDR);                /* slib end sector */
  flash_unlock();
  status = flash_slib_enable(TEST_SLIB_PSW, dw_start_sector, dw_data_start_sector, dw_end_sector);
  if(status != FLASH_OPERATE_DONE)
    return status;
  for(i = 0; i <= (dw_end_sector - dw_start_sector); i++)
  {
    memcpy(temp_buf, ptr, FLASH_SECTOR_SIZE);
    status = flash_sector_erase(SLIB_START_ADDR + i * FLASH_SECTOR_SIZE);
    if(status != FLASH_OPERATE_DONE)
      return status;
    for(j = 0; j < (FLASH_SECTOR_SIZE >> 2); j++)
    {
      status = flash_word_program(SLIB_START_ADDR + i * FLASH_SECTOR_SIZE + (j << 2), temp_buf[j]);
      if(status != FLASH_OPERATE_DONE)
        return status;
    }
    ptr += SECTOR_WORD_CNT;
  }
  return status;
}
#endif

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
#if defined (USE_SLIB_FUNCTION)
    /* enable slib protection from sector 2 to sector 5 when fir filter ip-code is tested ok */
    if(flash_slib_state_get() == RESET)
    {
      /* if slib has not been enabled, set slib configuration */
      if(slib_enable() == FLASH_OPERATE_DONE)
      {
        /* if slib configuration is correctly set, tuen on green led4 */
        at32_led_on(LED4);
        delay_sec(3);

        /* execute system reset to activate slib protection */
        nvic_system_reset();
      }
      else
      {
        /* turn on red led2 if slib activation failed */
        at32_led_on(LED2);
        while(1);
      }
    }
#endif
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



