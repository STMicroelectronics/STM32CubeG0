/**
  @page PWR_EnterStopMode PWR standby example
  
  @verbatim
  ******************************************************************************
  * @file    Examples_LL/PWR/PWR_EnterStopMode/readme.txt 
  * @author  MCD Application Team
  * @brief   Description of the PWR STOP 0 mode example.
  ******************************************************************************
  *
  * Copyright (c) 2019 STMicroelectronics. All rights reserved.
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                       opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  @endverbatim

@par Example Description

How to enter the STOP 0 mode.

After start-up LED3 is toggling during 5 seconds, then the system automatically 
enter in STOP 0 mode (Final state).

LED3 is used to monitor the system state as follows:
 - LED3 toggling : system in RUN mode
 - LED3 off : system in STOP 0 mode

@note To measure the current consumption in STOP0 mode, remove JP1 jumper 
      and connect an ampere meter to JP1 to measure IDD current.

@note This example can not be used in DEBUG mode due to the fact 
      that the Cortex-M0+ core is no longer clocked during low power mode 
      so debugging features are disabled.

@par Keywords

Power, PWR, stop mode, Interrupt, Low Power

@par Directory contents 

  - PWR/PWR_EnterStopMode/Inc/stm32g0xx_it.h          Interrupt handlers header file
  - PWR/PWR_EnterStopMode/Inc/main.h                  Header for main.c module
  - PWR/PWR_EnterStopMode/Inc/stm32_assert.h          Template file to include assert_failed function
  - PWR/PWR_EnterStopMode/Src/stm32g0xx_it.c          Interrupt handlers
  - PWR/PWR_EnterStopMode/Src/main.c                  Main program
  - PWR/PWR_EnterStopMode/Src/system_stm32g0xx.c      STM32G0xx system source file

@par Hardware and Software environment

  - This example runs on STM32G031K8Tx devices.
    
  - This example has been tested with STMicroelectronics NUCLEO-G031K8
    board and can be easily tailored to any other supported device
    and development board.

  - NUCLEO-G031K8 Set-up
    - LED3 connected to PC.06 pin

@par How to use it ? 

In order to make the program work, you must do the following :
 - Open your preferred toolchain
 - Rebuild all files and load your image into target memory
 - Run the example

 * <h3><center>&copy; COPYRIGHT STMicroelectronics</center></h3>
 */
