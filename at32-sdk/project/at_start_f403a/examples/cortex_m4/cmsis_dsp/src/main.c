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
#include "arm_math.h"

/** @addtogroup AT32F403A_periph_examples
  * @{
  */

/** @addtogroup 403A_CORTEX_m4_cmsis_dsp CORTEX_m4_cmsis_dsp
  * @{
  */

#define USE_STATIC_INIT

 /* Global defines */

#define TEST_LENGTH_SAMPLES   (20*4)

/* List of Marks scored by 20 students for 4 subjects */
const float32_t testMarks_f32[TEST_LENGTH_SAMPLES] =
{
  42.000000,  37.000000,  81.000000,  28.000000,
  83.000000,  72.000000,  36.000000,  38.000000,
  32.000000,  51.000000,  63.000000,  64.000000,
  97.000000,  82.000000,  95.000000,  90.000000,
  66.000000,  51.000000,  54.000000,  42.000000,
  67.000000,  56.000000,  45.000000,  57.000000,
  67.000000,  69.000000,  35.000000,  52.000000,
  29.000000,  81.000000,  58.000000,  47.000000,
  38.000000,  76.000000, 100.000000,  29.000000,
  33.000000,  47.000000,  29.000000,  50.000000,
  34.000000,  41.000000,  61.000000,  46.000000,
  52.000000,  50.000000,  48.000000,  36.000000,
  47.000000,  55.000000,  44.000000,  40.000000,
 100.000000,  94.000000,  84.000000,  37.000000,
  32.000000,  71.000000,  47.000000,  77.000000,
  31.000000,  50.000000,  49.000000,  35.000000,
  63.000000,  67.000000,  40.000000,  31.000000,
  29.000000,  68.000000,  61.000000,  38.000000,
  31.000000,  28.000000,  28.000000,  76.000000,
  55.000000,  33.000000,  29.000000,  39.000000
};


/* Number of subjects X 1 */
const float32_t testUnity_f32[4] =
{
  1.000,  1.000,   1.000,  1.000
};


/* f32 Output buffer */
static float32_t testOutput[TEST_LENGTH_SAMPLES];


/* Global defines */
#define   NUMSTUDENTS  20
#define   NUMSUBJECTS  4

/* Global variables */

uint32_t    numStudents = 20;
uint32_t    numSubjects = 4;
float32_t   max_marks, min_marks, mean, std, var;
uint32_t    student_num;

/**
  * @brief  main function.
  * @param  none
  * @retval none
  */
int32_t main()
{
  system_clock_config();

  uart_print_init(115200);

#ifndef  USE_STATIC_INIT

  arm_matrix_instance_f32 srcA;
  arm_matrix_instance_f32 srcB;
  arm_matrix_instance_f32 dstC;

  /* Input and output matrices initializations */
  arm_mat_init_f32(&srcA, numStudents, numSubjects, (float32_t *)testMarks_f32);
  arm_mat_init_f32(&srcB, numSubjects, 1, (float32_t *)testUnity_f32);
  arm_mat_init_f32(&dstC, numStudents, 1, testOutput);

#else

  /* Static Initializations of Input and output matrix sizes and array */
  arm_matrix_instance_f32 srcA = {NUMSTUDENTS, NUMSUBJECTS, (float32_t *)testMarks_f32};
  arm_matrix_instance_f32 srcB = {NUMSUBJECTS, 1, (float32_t *)testUnity_f32};
  arm_matrix_instance_f32 dstC = {NUMSTUDENTS, 1, testOutput};

#endif

  /* Call the Matrix multiplication process function */
  arm_mat_mult_f32(&srcA, &srcB, &dstC);

  /* Call the Max function to calculate max marks among numStudents */
  arm_max_f32(testOutput, numStudents, &max_marks, &student_num);

  /* Call the Min function to calculate min marks among numStudents */
  arm_min_f32(testOutput, numStudents, &min_marks, &student_num);

  /* Call the Mean function to calculate mean */
  arm_mean_f32(testOutput, numStudents, &mean);

  /* Call the std function to calculate standard deviation */
  arm_std_f32(testOutput, numStudents, &std);

  /* Call the var function to calculate variance */
  arm_var_f32(testOutput, numStudents, &var);

  printf("max_marks = %f, min_marks = %f, mean = %f, std = %f, var = %f\n", max_marks, min_marks, mean, std, var);

  while (1);
}

/**
  * @}
  */

/**
  * @}
  */
