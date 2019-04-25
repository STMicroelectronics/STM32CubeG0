/**
  @page PWR_STANDBY PWR STANDBY example
  
  @verbatim
  ******************************************************************************
  * @file    PWR/PWR_STANDBY/readme.txt 
  * @author  MCD Application Team
  * @brief   Description of the PWR Standby example.
  ******************************************************************************
  *
  * Copyright (c) 2018 STMicroelectronics. All rights reserved.
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
reset or the WKUP pin.

In the associated software, the system clock is set to 56 MHz, an EXTI line
is connected to the User push-button thru PC.13 and configured to generate an 
interrupt on falling edge.
The SysTick is programmed to generate an interrupt each 1 ms and in the SysTick 
interrupt handler, LED4 is toggled in order to indicate whether the MCU is in Standby or Run mode.

When a rising edge is detected on the EXTI line, an interrupt is generated. In the 
EXTI handler routine, the wake-up pin PWR_WAKEUP_PIN2 is enabled and the corresponding
wake-up flag cleared. Then, the system enters Standby mode causing LED4 to stop toggling. 

Next, the user can wake-up the system in pressing the User push-button which is connected
to the wake-up pin PWR_WAKEUP_PIN2.
A rising edge on WKUP pin will wake-up the system from Standby.
Alternatively, an external RESET of the board will lead to a system wake-up as well.

After wake-up from Standby mode, program execution restarts in the same way as after
a RESET and LED4 restarts toggling.

LED4 is used to monitor the system state as follows:
 - LED4 toggling: system in Run mode
 - LED4 off : system in Standby mode

These steps are repeated in an infinite loop.

@note To measure the current consumption in Standby mode, remove JP3 jumper 
      and connect an amperemeter to JP3 to measure IDD current.     


@note This example can not be used in DEBUG mode due to the fact 
      that the Cortex-M0+ core is no longer clocked during low power mode 
      so debugging features are disabled.

@note Care must be taken when using HAL_Delay(), this function provides accurate delay (in milliseconds)
      based on variable incremented in SysTick ISR. This implies that if HAL_Delay() is called from
      a peripheral ISR process, then the SysTick interrupt must have higher priority (numerically lower)
      than the peripheral interrupt. Otherwise the caller ISR process will be blocked.
      To change the SysTick interrupt priority you have to use HAL_NVIC_SetPriority() function.
      
@note The application needs to ensure that the SysTick time base is always set to 1 millisecond
      to have correct HAL operation.

@par Keywords

Power, PWR, Standby mode, Interrupt, EXTI, Wakeup, Low Power, External reset

@par Directory contents 

  - PWR/PWR_STANDBY/Inc/stm32g0xx_conf.h         HAL Configuration file
  - PWR/PWR_STANDBY/Inc/stm32g0xx_it.h           Header for stm32g0xx_it.c
  - PWR/PWR_STANDBY/Inc/main.h                         Header file for main.c
  - PWR/PWR_STANDBY/Src/system_stm32g0xx.c       STM32G0xx system clock configuration file
  - PWR/PWR_STANDBY/Src/stm32g0xx_it.c           Interrupt handlers
  - PWR/PWR_STANDBY/Src/main.c                         Main program
  - PWR/PWR_STANDBY/Src/stm32g0xx_hal_msp.c      HAL MSP module

@par Hardware and Software environment

  - This example runs on STM32G070RBTx devices
    
      
  - This example has been tested with STMicroelectronics NUCLEO-G070RB
    board and can be easily tailored to any other supported device 
    and development board.

  - NUCLEO-G070RB Set-up
    - LED4 connected to PA.05 pin
  
    - User push-button connected to pin PC.13 (EXTI_Line4_15)
    - WakeUp Pin PWR_WAKEUP_PIN2 connected to PC.13

@par How to use it ? 

In order to make the program work, you must do the following :
 - Open your preferred toolchain 
 - Rebuild all files and load your image into target memory
 - Run the example


 * <h3><center>&copy; COPYRIGHT STMicroelectronics</center></h3>
 */
