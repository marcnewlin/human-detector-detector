/**
  **************************************************************************
  * @file     readme.txt
  * @version  v2.0.8
  * @date     2022-04-02
  * @brief    readme
  **************************************************************************
  */

  this demo is based on the at-start board, in this demo, shows in fullduplex
  mode how to use dma transfer data.
  the pins connection as follow:
  i2s2 master                    i2s3 slaver
  - pb12(ws)           <--->     pa4 (ws)
  - pb13(sck)          <--->     pc10(sck)
  - pb14(miso_ext) rx  <--->     pc11(miso_ext) tx
  - pb15(mosi) tx      <--->     pc12(mosi) rx
