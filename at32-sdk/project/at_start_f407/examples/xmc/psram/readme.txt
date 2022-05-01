/**
  ******************************************************************************
  * @file     readme.txt
  * @version  v2.0.8
  * @date     2022-04-02
  * @brief    readme
  ******************************************************************************
  */

  this demo is based on the artery ic, in this demo, provides a basic example of how
  to use the xmc  firmware library and  an associate  driver to perform  read/write
  operations on the nand flash memory w957d6hb(winbond).
  usart1 using pa9 to printf the result.

    xmc                         at32f403a           psram
  - xmc_d0                      pd14         --->   dq[0]
  - xmc_d1                      pd15         --->   dq[1]
  - xmc_d2                      pd0          --->   dq[2]
  - xmc_d3                      pd1          --->   dq[3]
  - xmc_d4                      pe7          --->   dq[4]
  - xmc_d5                      pe8          --->   dq[5]
  - xmc_d6                      pe9          --->   dq[6]
  - xmc_d7                      pe10         --->   dq[7]
  - xmc_d8                      pe11         --->   dq[8]
  - xmc_d9                      pe12         --->   dq[9]
  - xmc_d10                     pe13         --->   dq[10]
  - xmc_d11                     pe14         --->   dq[11]
  - xmc_d12                     pe15         --->   dq[12]
  - xmc_d13                     pd8          --->   dq[13]
  - xmc_d14                     pd9          --->   dq[14]
  - xmc_d15                     pd10         --->   dq[15]
  - xmc_a16                     pd11         --->   a[16]
  - xmc_a17                     pd12         --->   a[17]
  - xmc_a18                     pd13         --->   a[18]
  - xmc_a19                     pe3          --->   a[19]
  - xmc_a20                     pe4          --->   a[20]
  - xmc_a21                     pe5          --->   a[21]
  - xmc_a22                     pe6          --->   a[22]
  - xmc_nce2 / xmc_ne1          pd7          --->   ce#
  - xmc_nadv                    pb7          --->   adv#
  - xmc_noe                     pd4          --->   oe#  
  - xmc_nwe                     pd5          --->   we#
  - xmc_lb                      pe0          --->   lb#
  - xmc_ub                      pe1          --->   ub#
