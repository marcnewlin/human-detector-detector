/**
  **************************************************************************
  * @file     eeprom/readme.txt 
  * @version  v2.0.8
  * @date     2022-04-02
  * @brief    readme
  **************************************************************************
  */

  this demo is based on the at-start board, in this demo, use hardware i2c1 
  write or read data based on the memory device. if the communication is
  successful, led3 will turn on, if the communication fails, led2 will keep flashing.
  
  attention:
    1. i2c bus must pull-up
    2. press the slave button first, then press the master button to start communication.

  pin used:
    1. scl --- pb6
    2. sda --- pb7


