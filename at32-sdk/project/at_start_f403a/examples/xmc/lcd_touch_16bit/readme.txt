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
  perform read/write operations on the lcd. and use spi3 drive touch screen,
  user can drow picture on the lcd.when used must be use the tft_lcd_demo board.
  attention: tft_lcd_demo board lcd id is 0x9341 and the touchscreen is resistive
  touchscreen.

  the pin to pin with lcd and xmc:
  - spi3_sck  pb3     --->   spi_sck
  - spi3_miso pb4     --->   spi_miso
  - spi3_mosi pb5     --->   spi_mosi
  - gpio      pb8     --->   lcd_cs
  - gpio      pd3     --->   lcd_reset
  - gpio      pd5     --->   lcd_pen
  - gpio      pb0     --->   lcd_bl
  - xmc_a16   pd11    --->   lcd_rs
  - xmc_ne1   pd7     --->   lcd_cs
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
  - xmc_d8    pe11    --->   data[8]
  - xmc_d9    pe12    --->   data[9]
  - xmc_d10   pe13    --->   data[10]
  - xmc_d11   pe14    --->   data[11]
  - xmc_d12   pe15    --->   data[12]
  - xmc_d13   pb12    --->   data[13]
  - xmc_d14   pd9     --->   data[14]
  - xmc_d15   pd10    --->   data[15]



