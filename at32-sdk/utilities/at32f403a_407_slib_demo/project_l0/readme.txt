/**
  **************************************************************************
  * @file     readme.txt
  * @version  v2.0.8
  * @date     2022-04-02
  * @brief    readme
  **************************************************************************
  */

  this demo is based on the at-start board, in this demo(project_l0, step1 of
  the slib example) describes how to generate an execute-only ip-code (using
  compiler options), place and protect it (protection using slib) in some preferred
  sectors in internal flash memory. the fir filter algorithm from cmsis library
  is chosen as an example of ip-code to be protected. enabling slib is managed
  by software in slib_enable() function.
  
  the ip-code function fir_lowpass_filter() is called in main.c file. note that
  symbol definition file have to be generated in project configuration described
  below, it has to be used in customer project project_l1.
  
  with the project configurations, the compiler is configured to generate an 
  execute-only ip-code avoiding any data read from it (avoiding literal pools
  and branch tables).
  
  @note the ip-code and ip-data are placed in sector 2 to sector 5 which will
        be protected.
  
        activating the slib on these sectors is completed by slib_enable() function
        descripes in main file.

  @note the application needs to ensure that the systick time base is always
        set to 1 millisecond to have correct operation.
 
  @par Directory contents 
  extra used files:
  - project_l0/src/math_helper.c               calculation of snr 
  - project_l0/src/arm_fir_data.c              contains fir_filter's input signal and reference output signal computed with matlab
  - project_l0/inc/fir_filter.h                fir-filter slib protected ip-code header file

  fir filter files representing the ip-code to be protected:
  - project_l0/src/fir_filter.c                file containing global fir low pass filter function   
  - project_l0/src/arm_fir_f32.c               file containing fir filter basic function from cmsis lib
  - project_l0/src/arm_fir_init_f32.c          file containing fir filter initialization function from cmsis lib
  - project_l0/src/fir_coefficient.c           file containing fir coefficients generated using fir1() matlab function.
     
  @par hardware and software environment  
  - this example runs on at-start board.
  - this example has been tested with the following toolchain:
    keil mdk-arm uvision v5.18.0.0

  @par generating symbol definition file 
	generating symbol definition file is mandatory for project_l1 and the provided
  file must be replaced by the newly generated.	to generate symbol definition
  file required for project_l1, click on rebuild all button then symbol definition
  file is created. 
  - for mdk-arm toolchain: fir_filter_symbol.txt symbol definition file is created
    in project_l0\mdk_v5\Objects. it must be modified by deleting all lines 
    and keeping only fir-filter ip-code functions (please follow the provided 
    fir_filter_symbol.txt file in project_l0\mdk_v5). once modified, copy it
    to project_l1\mdk_v5 and replace the existing file.

  @par how to use it ?   
  before running this project the following conditions must be met:
  - the same toolchain and compiler version must be used for both projects in
    project_l0 and project_l1: for instance if mdk-arm v5.18 is used in project_l0
    then it must be used for project_l1 too.
  - check if any flash access protection(FAP), erase/program protection(EPP) 
    or slib protection is enabled, if yes disable these protection first. 
  
  in order to make the program work, you must do the following :
  
  configurate the compiler to generate an execute only ip-code avoiding any 
  data read from it (literal pools and branch tables) . 
  1. open project located in project_l0 directory and choose your preferred toolchain 
  2. select the project configuration
  3. rebuild all files. 
  4. run the example following the sequence below:
     (a) power on the board then load the code, led3 should toggle fast if not
         press reset button.
     (b) press the user button key to execute the ip-code called in main.c file.
     (c) if macro "use_slib_function" is defined and slib is never enabled, go
         to step(e). 
     (d) if the ip-code is successfully executed, the green led4 should toggle 
         continuously. else the red led2 toggles continuously.  
     (e) the slib_enable() function will set slib configuration once the ip-code
         is correctly executed. the green led4 will turn on some time and then
         a system rest is executed to activate slib. the code will run to step(b).

  for details, please refer to the application note document AN0040.
