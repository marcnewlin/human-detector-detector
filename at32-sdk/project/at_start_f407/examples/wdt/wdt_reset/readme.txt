/**
  **************************************************************************
  * @file     wdt_reset/readme.txt 
  * @version  v2.0.8
  * @date     2022-04-02
  * @brief    readme
  **************************************************************************
  */

  this demo is based on the at-start board, in this demo, during normal 
  operation, the wdt count value is continuously reloaded in the main function, 
  and wdt reset will not occur at this time. when the wakeup button is pressed, 
  the function stops reloading the wdt count value, resulting in wdt reset.
