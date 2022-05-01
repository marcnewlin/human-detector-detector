/**
  **************************************************************************
  * @file     random.h
  * @version  v2.0.8
  * @date     2022-04-02
  * @brief    at32f403a_407 random header file
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

#ifndef __RANDOM_H
#define __RANDOM_H

#ifdef __cplusplus
extern "C" {
#endif

#include "at32f403a_407.h"

/** @addtogroup UTILITIES_examples
  * @{
  */

/** @addtogroup GEN_random_number_demo
  * @{
  */

/** @defgroup RTC_asseed_definition
  * @{
  */

/* this define rtc seed for random number gen,*/
#define ENABLE_RTC_ASSEED                (1)

/**
  * @}
  */

/** @defgroup GEN_random_number_demo_exported_functions
  * @{
  */

void randnum_test( void);

/**
  * @}
  */

/**
  * @}
  */

/**
  * @}
  */

#ifdef __cplusplus
}
#endif

#endif
