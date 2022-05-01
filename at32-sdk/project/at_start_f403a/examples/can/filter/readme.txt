/**
  **************************************************************************
  * @file     readme.txt 
  * @version  v2.0.8
  * @date     2022-04-02
  * @brief    readme
  **************************************************************************
  */

  this demo is based on the at-start board, in this demo, shows how to use
  the can filter funciton. the can tool transmit 6 specified messages in total
  (3 extended-id messages and 3 standard-id messages), when mcu receive one
  expect id message, test_result will add one, if test success, only 4 filter
  messages will be received, the three leds will toggle.
  set-up
  - can tx      --->   pb9
  - can rx      --->   pb8
