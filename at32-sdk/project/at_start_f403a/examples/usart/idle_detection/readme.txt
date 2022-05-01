/**
  **************************************************************************
  * @file     readme.txt
  * @version  v2.0.8
  * @date     2022-04-02
  * @brief    readme
  **************************************************************************
  */

  this demo is based on the at-start board, in this demo, show the usart idle
  detection with interrupt. after received data, if the rx bus free time greater
  than idle time size, the usart idle flag will setup and the three led will turn
  on.

  set-up
  - connect usart2 tx pin(pa2) to rx pin(pa3)
