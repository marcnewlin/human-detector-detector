/**
  **************************************************************************
  * @file     readme.txt 
  * @version  v2.0.8
  * @date     2022-04-02
  * @brief    readme
  **************************************************************************
  */

  this demo is based on the at-start board, in this demo, provides a basic 
  example of how to use the xmc firmware library and an associate driver to 
  perform read/write operations on the lcd. the lcd drive ic is st7789v and
  will display some number, it's actually showing pictures.this demo support
  100pin and 64pin pakeage.
  
  the pin to pin with lcd and xmc:
  - gpio      pb9     --->   lcd_reset
  - xmc_a0    pc3     --->   lcd_rs  
  - xmc_ne4   pc4     --->   lcd_cs
  - xmc_nwe   pd2     --->   lcd_wr 
  - xmc_noe   pc5     --->   lcd_rd 
  - xmc_d0    pb14    --->   data[0]
  - xmc_d1    pc6     --->   data[1]
  - xmc_d2    pc11    --->   data[2]
  - xmc_d3    pc12    --->   data[3]
  - xmc_d4    pa2     --->   data[4]
  - xmc_d5    pa3     --->   data[5]
  - xmc_d6    pa4     --->   data[6]
  - xmc_d7    pa5     --->   data[7]


