/**
  **************************************************************************
  * @file     readme.txt
  * @version  v2.0.8
  * @date     2022-04-02
  * @brief    readme
  **************************************************************************
  */

  this demo is based on the at-start board, in this demo, shows how to use
  the same trigger source trigger 3 adcs.
  the convert data as follow:
  - adc1_ordinary_valuetab[n][0] ---> (adc2_channel_7<<16) | adc1_channel_4
  - adc1_ordinary_valuetab[n][1] ---> (adc2_channel_8<<16) | adc1_channel_5
  - adc1_ordinary_valuetab[n][2] ---> (adc2_channel_9<<16) | adc1_channel_6
  - adc3_ordinary_valuetab[n][0] --->  adc3_channel_10
  - adc3_ordinary_valuetab[n][1] --->  adc3_channel_11
  - adc3_ordinary_valuetab[n][2] --->  adc3_channel_12
