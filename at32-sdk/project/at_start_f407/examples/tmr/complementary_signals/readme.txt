/**
  **************************************************************************
  * @file     readme.txt
  * @version  v2.0.8
  * @date     2022-04-02
  * @brief    readme
  **************************************************************************
  */

  this demo is based on the at-start board, in this demo, shows how to configure
  the tmr1 peripheral to generate three complementary tmr1 signals, to insert
  a defined dead time value, to use the break feature and to lock the desired
  parameters.

  tmr1 configuration to:

  step1: generate 3 complementary pwm signals with 3 different duty cycles:
         tim1clk is fixed to system_core_clock, the tmr1 prescaler is equal to
         0 so the tmr1 counter clock used is system_core_clock.
         system_core_clock is set to 240 mhz .

  the objective is to generate pwm signal at 17.57 khz:
  - tim1_period = (system_core_clock / 17570) - 1

  the three duty cycles are computed as the following description:

  the channel 1 duty cycle is set to 50% so channel 1n is set to 50%.
  the channel 2 duty cycle is set to 25% so channel 2n is set to 75%.
  the channel 3 duty cycle is set to 12.5% so channel 3n is set to 87.5%.
  the timer pulse is calculated as follows:
  - channelxpulse = duty_cycle * (tim1_period - 1) / 100

  step2: insert a dead time equal to 11/system_core_clock ns
  step3: configure the break feature, active at high level, and using the automatic
         output enable feature
  step4:  use the locking parameters level1.

  the tmr1 waveform can be displayed using an oscilloscope.

  set-up
  connect the tmr1 pins to an oscilloscope to monitor the different waveforms:
  - tmr1_ch1  pin (pa.08)
  - tmr1_ch1c pin (pb.13)
  - tmr1_ch2  pin (pa.09)
  - tmr1_ch2c pin (pb.14)
  - tmr1_ch3  pin (pa.10)
  - tmr1_ch3c pin (pb.15)

- connect the tmr1 break pin tmr1_brkin pin (pb.12) to the gnd. to generate a
  break event, switch this pin level from 0v to 3.3v.