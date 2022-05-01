/**
  **************************************************************************
  * @file     random.c
  * @version  v2.0.8
  * @date     2022-04-02
  * @brief    set of firmware functions to random function
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
#include "random.h"
#include "stdio.h"
#include <stdlib.h>

/** @addtogroup UTILITIES_examples
  * @{
  */

/** @addtogroup GEN_random_number_demo
  * @{
  */

/* define at32 mcu uid address */
#define DEVICE_ID_ADDR1 0x1FFFF7E8

/**
  * @brief  get uid value as seed
  * @param  none
  * @retval uid_one_word
  */
int get_uid_for_seed (void)
{
  uint32_t   id[3] = {0};
  uint32_t   uid_one_word;

  /* get uid */
  id[0] = *(int*)DEVICE_ID_ADDR1;
  id[2] = *(int*)(DEVICE_ID_ADDR1+8);
  uid_one_word=id[0]&0x7fffffff+((id[2]&(~0x7fffffff))<<16);
  return uid_one_word;
}

/**
  * @brief  enable rtc as seed
  * @param  none
  * @retval none
  */
#if ENABLE_RTC_ASSEED
void rtc_init_for_seed (void)
{
  /* enable pwc and bpr clocks */
  crm_periph_clock_enable(CRM_PWC_PERIPH_CLOCK, TRUE);
  crm_periph_clock_enable(CRM_BPR_PERIPH_CLOCK, TRUE);

  /* allow access to bpr domain */
  pwc_battery_powered_domain_access(TRUE);

  if (bpr_data_read(BPR_DATA1) != 0x5051)
  {
    /* reset bpr domain */
    bpr_reset();

    /* enable the lext osc */
    crm_clock_source_enable(CRM_CLOCK_SOURCE_LEXT, TRUE);
    /* wait lext is ready */
    while(crm_flag_get(CRM_LEXT_STABLE_FLAG) == RESET);
    /* select the rtc clock source */
    crm_rtc_clock_select(CRM_RTC_CLOCK_LEXT);

    /* enable rtc clock */
    crm_rtc_clock_enable(TRUE);

    /* wait for rtc registers update */
    rtc_wait_update_finish();

    /* wait for the register write to complete */
    rtc_wait_config_finish();

    /* set rtc divider: set rtc period to 1sec */
    rtc_divider_set(32767);

    /* wait for the register write to complete */
    rtc_wait_config_finish();

    /* writes data to bpr register */
    bpr_data_write(BPR_DATA1, 0x5051);
  }
  else
  {
    /* wait for rtc registers update */
    rtc_wait_update_finish();

    /* wait for the register write to complete */
    rtc_wait_config_finish();
  }
}

#endif

/**
  * @brief  random number test
  * @param  none
  * @retval none
  */
void randnum_test( void)
{
#if ENABLE_RTC_ASSEED
  rtc_init_for_seed();

  /* set uid and rtc as seed for random */
  srand(rtc_counter_get()+get_uid_for_seed());
#else
  /* set only  uid as seed for random */
  srand(get_uid_for_seed());
#endif

  while(1)
  {
    delay_ms(500);
    printf("%d\r\n",rand());
  }
}

/**
  * @}
  */

/**
  * @}
  */
