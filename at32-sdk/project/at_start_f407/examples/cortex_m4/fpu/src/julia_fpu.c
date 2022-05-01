/**
  **************************************************************************
  * @file     julia_fpu.c
  * @version  v2.0.8
  * @date     2022-04-02
  * @brief    julia_fpu source file
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
#include "julia_fpu.h"

/** @addtogroup AT32F407_periph_examples
  * @{
  */

/** @addtogroup 407_CORTEX_m4_fpu
  * @{
  */

/**
  * @brief  julia calculation test with fpu option enable or disable
  * @param  size_x: width in pixel
  * @param  size_y: height in pixel
  * @param  offset_x: x offset for starting point
  * @param  offset_y: y offset for starting point
  * @param  zoom: iteration zoom ratio
  * @param  buffer: pointer to output buffer where values are stored
  * @retval none
  */
void generate_julia_fpu(uint16_t size_x, uint16_t size_y, uint16_t offset_x, uint16_t offset_y, uint16_t zoom, uint8_t * buffer)
{
  float tmp1, tmp2;
  float num_real, num_img;
  float rayon;
  uint16_t i;
  uint16_t x, y;
  for (y = 0; y < size_y; y++)
  {
    for (x = 0; x < size_x; x++)
    {
      num_real = y - offset_y;
      num_real = num_real / zoom;
      num_img = x - offset_x;
      num_img = num_img / zoom;
      i = 0;
      rayon = 0;
      while((i < ITERATION - 1) && (rayon < 4))
      {
        tmp1 = num_real * num_real;
        tmp2 = num_img * num_img;
        num_img = 2 * num_real * num_img + IMG_CONSTANT;
        num_real = tmp1 - tmp2 + REAL_CONSTANT;
        rayon = tmp1 + tmp2;
        i++;
      }
      /* store the value in the buffer */
      buffer[x + y * size_x] = i;
    }
  }
}

/**
  * @}
  */

/**
  * @}
  */
