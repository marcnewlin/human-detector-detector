/**
  **************************************************************************
  * @file     readme.txt
  * @version  v2.0.8
  * @date     2022-04-02
  * @brief    readme
  **************************************************************************
  */

  this demo is based on the at-start board, in this demo, provides a basic
  communication between usart2 (synchronous mode) and spi2. the usart2 simulate
  spi master, the spi2 is spi slave. if both usart2 and spi2 received data right,
  the three leds will turn on.

  usart2 configured as follow:
  - 115200 baudrate
  - 8 bits data
  - one stop bit
  - no parity
  - receive and transmit enabled
  - usart clock enabled
  - usart clkpol: clock is active high
  - usart clkpha: data is captured on the second edge
  - usart lbcp: last bit clock pulse is output to the sclk pin

  spi2 configured as follow:
  - fullduplex
  - slave mode
  - 8 bits datasize
  - clkpol: clock is active high
  - clkpha: data is captured on the second edge
  - cs: cs software
  - first bit: first bit is the lsb

  set-up
  - connect usart2_tx(pa2) to spi2_mosi(pb15)
  - connect usart2_rx(pa3) to spi2_miso(pb14)
  - connect usart2_ck(pa4) to spi2_sck(pb13)
