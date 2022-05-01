/**
  **************************************************************************
  * @file     readme.txt
  * @version  v2.0.8
  * @date     2022-04-02
  * @brief    readme
  **************************************************************************
  */

  this demo is based on the at-start board, in this demo, shows how to update
  the tmr1 channel1 period and the duty cycle using the tmr1 dma burst feature.

  every update dma request, the dma will do 3 transfers of half words into timer
  registers beginning from arr register.
  on the dma update request, 0x0fff will be transferred into arr, 0x0000
  will be transferred into rcr, 0x0555 will be transferred into ccr1.

  tmr1 configuration: generate 1 pwm signal using the dma burst mode:
  the tmr1clk frequency is set to system_core_clock (hz), to get tmr1 counter
  clock at 24 mhz the prescaler is computed as following:
  - prescaler = (tim1clk / tmr1 counter clock) - 1
  system_core_clock is set to 240 mhz .
  the tmr1 period is 5.86 khz: tmr1 frequency = tmr1 counter clock/(arr + 1)
                                               = 24 mhz / 4096 = 5.86khz khz
  tmr1 channel1 duty cycle = (tim1_ccr1/ tmr1_arr)* 100 = 33.33%

  set-up
  connect the following pins to an oscilloscope to monitor the different
  waveforms:
  - tim1 ch1 ---> pa8