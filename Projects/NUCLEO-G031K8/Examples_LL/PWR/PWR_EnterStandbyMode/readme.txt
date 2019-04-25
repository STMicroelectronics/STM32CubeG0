/**
  @page PWR_EnterStandbyMode PWR standby example
  
  @verbatim
  ******************************************************************************
  * @file    Examples_LL/PWR/PWR_EnterStandbyMode/readme.txt 
  * @author  MCD Application Team
  * @brief   Description of the PWR STANDBY mode example.
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

How to enter the Standby mode and wake up from this mode by using an external 
reset or a wakeup interrupt.

In the associated software, the system clock is set to 16 MHz.
LED3 toggles in order to indicate that MCU is in RUN mode:
- quickly (each 200ms) if program starts from reset
- slowly (each 500ms) if program wake-up from Standby mode

LED3 is toggling during 5 seconds, then the system automatically 
enters in Standby mode.

Next, a falling edge on PA.00 (connected to LL_PWR_WAKEUP_PIN1) will 
wake-up the system from Standby.
Alternatively, an external RESET of the board will lead to a system wake-up as well.

After wake-up from Standby mode, program execution restarts in the same way as 
after a RESET and LED3 restarts toggling.

LED3 is used to monitor the system state as follows:
 - LED3 fast toggling: system in RUN mode
 - LED3 slow toggling: system in RUN mode after exiting from Standby mode
 - LED3 off : system in Standby mode

These steps are repeated in an infinite loop.

@note To measure the current consumption in Standby mode, remove JP1 jumper 
      and connect an ampere meter to JP1 to measure IDD current.

@note This example can not be used in DEBUG mode due to the fact 
      that the Cortex-M0+ core is no longer clocked during low power mode 
      so debugging features are disabled.

@par Keywords

Power, PWR, Standby mode, Interrupt, EXTI, Wakeup, Low Power, External reset,

@par Directory contents 

  - PWR/PWR_EnterStandbyMode/Inc/stm32g0xx_it.h          Interrupt handlers header file
  - PWR/PWR_EnterStandbyMode/Inc/main.h                  Header for main.c module
  - PWR/PWR_EnterStandbyMode/Inc/stm32_assert.h          Template file to include assert_failed function
  - PWR/PWR_EnterStandbyMode/Src/stm32g0xx_it.c          Interrupt handlers
  - PWR/PWR_EnterStandbyMode/Src/main.c                  Main program
  - PWR/PWR_EnterStandbyMode/Src/system_stm32g0xx.c      STM32G0xx system source file


@par Hardware and Software environment

  - This example runs on STM32G031K8Tx devices.
    
  - This example has been tested with STMicroelectronics NUCLEO-G031K8
    board and can be easily tailored to any other supported device
    and development board.

  - NUCLEO-G031K8 Set-up
    - LED3 connected to PC.06 pin
    - Put a wire between PA.00 (connected to pin 12 on CN4 connector) and 
      GND pin (pin 2 on CN4) to generate a falling edge on LL_PWR_WAKEUP_PIN1.
    - WakeUp Pin LL_PWR_WAKEUP_PIN1 connected to PA.00

@par How to use it ? 

In order to make the program work, you must do the following :
 - Open your preferred toolchain
 - Rebuild all files and load your image into target memory
 - Run the example

 * <h3><center>&copy; COPYRIGHT STMicroelectronics</center></h3>
 */
