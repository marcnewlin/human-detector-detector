/**
  ******************************************************************************
  * @file     readme.txt 
  * @version  v2.0.8
  * @date     2022-04-02
  * @brief    readme
  ******************************************************************************
  */

  this demo is based on the at-start board, in this demo, provides a basic example
  of how to use the xmc firmware library and an associate driver to perform read
  /write/erase operations on the nand flash£º
  h27u1g8f2ctr(hynix) or k9gag08u0e(samsung).
  usart1 using pa9 to printf the result.
          
    xmc                     at32f403a/407       nand
  - xmc_d0                  pd14         --->   io[0]  
  - xmc_d1                  pd15         --->   io[1]
  - xmc_d2                  pd0          --->   io[2]
  - xmc_d3                  pd1          --->   io[3]
  - xmc_d4                  pe7          --->   io[4]
  - xmc_d5                  pe8          --->   io[5]
  - xmc_d6                  pe9          --->   io[6]
  - xmc_d7                  pe10         --->   io[7]
  - xmc_a16                 pd11         --->   cle
  - xmc_a17                 pd12         --->   ale
  - xmc_nce2 / xmc_ne1      pd7          --->   ce#
  - xmc_noe                 pd4          --->   re#
  - xmc_nwe                 pd5          --->   we#
  - xmc_wait                pd6          --->   r/b#
