/**
  **************************************************************************
  * @file     readme.txt
  * @version  v2.0.8
  * @date     2022-04-02
  * @brief    readme
  **************************************************************************
  */

  this demo is based on the at-start board, in this demo(project_l1, step2 of
  the slib example), the mcu is already preloaded with fir-filter slib protected
  ip-code (done in project_l0 project).
  to call the ip-code fir_lowpass_filter() function:
  - the fir_filter.h header file is included in main.c file
  - symbol definition file (fir_filter_symbol.txt for mdk-arm) containing the
    protected function symbols and addresses is included in this project. noting
    that symbol definition file have to be generated in project_l0 in order to 
    replace the existing one in this project.
  
  @note the application needs to ensure that the systick time base is always 
  set to 1 millisecond to have correct operation.
 
  @par directory contents 
  extra used files:
  - project_l1/src/math_helper.c               calculation of snr 
  - project_l1/src/arm_fir_data.c              contains fir_filter's input signal and reference output signal computed with matlab

  related ip-code files provided from project_l0:
  - project_l1/mdk_v5/fir_filter_symbol.txt    symbol definition file for mdk-arm project containing slib protected code symbols and addresses
  - project_l1/inc/fir_filter.h                fir-filter slib protected ip-code header file

  @par hardware and software environment  
  - this example runs on at-start board.
  - this example has been tested with the following toolchain:
    keil mdk-arm uvision v5.18.0.0

  @par how to use it ? 
  before running this project the following conditions must be met:
  - the same toolchain and compiler version must be used for both projects in
    project_l0 and project_l1: for instance if mdk-arm v5.18 is used in project_l0
    then it must be used for project_l1 too.
  - check if any flash access protection(FAP), erase/program protection(EPP) 
    or slib protection is enabled, if yes disable these protection first. 

  in order to make the program work, you must do the following :
  
  1. open project located in project_l1 directory and choose your preferred toolchain 
  2. select the project configuration
  3. rebuild all files. 
  4. run the example following the sequence below:
     (a) power on the board then load the code, led3 should toggle fast if not
         press reset button.
     (b) press the user button key to execute the ip-code called in main.c file.
     (c) if the ip-code is successfully executed, the green led4 should toggle
         continuously. else the red led2 toggles continuously.
    
  for details, please refer to the application note document AN0040.