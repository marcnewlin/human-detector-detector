/**
  **************************************************************************
  * @file     readme.txt
  * @version  v2.0.8
  * @date     2022-04-02
  * @brief    readme
  **************************************************************************
  */

  this demo is based on the at-start board, in this demo, shows how to configure
  the tmr peripheral in output compare inactive mode with the corresponding
  interrupt requests for each channel.

  tmr2 configuration:
  tmr2clk = system_core_clock / 4,
  the objective is to get tmr2 counter clock at 1 khz:
  - prescaler = (tim2clk / tmr2 counter clock) - 1
  and generate 4 signals with 4 different delays:
  tmr2_ch1 delay = c1dt_val/tmr2 counter clock = 1000 ms
  tmr2_ch2 delay = c2dt_val/tmr2 counter clock = 500 ms
  tmr2_ch3 delay = c3dt_val/tmr2 counter clock = 250 ms
  tmr2_ch4 delay = c4dt_val/tmr2 counter clock = 125 ms

  connect the following pins to an oscilloscope to monitor the different waveforms:
  - pc6
  - pc7
  - pc8
  - pc9
