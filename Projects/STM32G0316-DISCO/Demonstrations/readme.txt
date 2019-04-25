/**
  @page Demo   Demo STM32G0316-DISCO

  @verbatim
  ******************************************************************************
  * @file    readme.txt 
  * @author  MCD Application Team
  * @brief   Description of STM32G0316-DISCO Demo
  ******************************************************************************
  *
  * Copyright (c) 2019 STMicroelectronics. All rights reserved.
  *
  * This software component is licensed by ST under Ultimate Liberty license
  * SLA0044, the "License"; You may not use this file except in compliance with
  * the License. You may obtain a copy of the License at:
  *                               www.st.com/SLA0044
  *
  ******************************************************************************
  @endverbatim

@par Demo Description

Demonstration of firmware based on STM32Cube. This demonstration provides firmware
to help you to discover STM32 Cortex-M devices that are plugged onto an your
STM32G0316-DISCO board.

This demonstration aims at highlighting the possibility to use PF2 pin for different 
purposes: Reset or GPIO. 

When the discovery board is powered-on, the demonstration firmware checks the PF2 
configuration by reading the option bytes. 

The demonstration firmware has 2 functioning modes which depends on the PF2 pin configuration:
- When PF2 pin is configured in RESET Input/Output mode the demonstration firmware enters
  the PF2-NRST mode. In this mode, reset can be forced by grounding the PF2 pin and
  internal resets are propagated to the PF2 pin (20µs pulse generation).

- When PF2 is configured in GPIO mode, the demonstration firmware enters the PF2-GPIO mode. 
  In this mode PF2 can be used as a standard GPIO, with all functionalities 
  (Input/Output/Alternate Functions/Analog) available.
 
PF2-NRST mode
When the demonstration firmware operates in PF2-NRST mode, the independent watchdog is configured 
in order to regularly reset the system. 
User can then check that a pulse is generated on the PF2 pin. 
If the user presses the RST/USER button, option bytes are updated in order to configure PF2 in GPIO mode. 
When option bytes loading operation completes, system is reset and the demonstration firmware switches to 
the PF2-GPIO mode.

PF2-GPIO mode
When the demonstration firmware operates in PF2-GPIO, RST/USER button is used to select amongst the following sub-modes:
- Short press on the RST/USER button: the system is put in standby mode.  
  User can then measure the power consumption using the JP1 pins. 
  Another press on the RST/USER button wakes up the system from standby mode.
- Double press on the RST/USER button: independent watchdog is configured to reset the system after a short delay.
- Long press on the RST/USER button: option bytes are updated in order to configure PF2 in GPIO mode. 
  When option bytes loading operation completes, system is reset and the demonstration firmware switches 
  back to the PF2-GPIO mode

Whatever the operating mode is (PF2-NRSTor PF2-GPIO), the demonstration firmware starts by identifying the origin of the reset.

The reset origin is reported (during 5s) to the user thanks to LED2 (one toggling scheme per reset origin).
  - Exit from Standby mode reset: 4 fast successive blink of LED2 in 1 second
  - Independent watchdog reset: 3 fast successive blink of LED2 in 1 second
  - Option byte loader reset: 2 fast successive blink of LED2 in 1 second
  
After 5 seconds, LED blinking scheme indicates to the user in which mode the PF2 pin is configured:
  - PF2-GPIO: slow (2s) blinking
  - PF2-NRST: fast (1s) blinking

@note Care must be taken when using HAL_Delay(), this function provides accurate
      delay (in milliseconds) based on variable incremented in SysTick ISR. 
      This implies that if HAL_Delay() is called from a peripheral ISR process, 
      then the SysTick interrupt must have higher priority (numerically lower)
      than the peripheral interrupt. Otherwise the caller ISR process will be blocked.
      To change the SysTick interrupt priority you have to use HAL_NVIC_SetPriority() function.
      
@note The application need to ensure that the SysTick time base is always set to
      1 millisecond to have correct HAL operation.

@par Directory contents 

  - Inc/main.h                  Header for main.c module  
  - Inc/stm32g0xx_conf.h        HAL configuration file
  - Inc/stm32g0xx_it.h          Interrupt handlers header file
  - Src/main.c                  Main program
  - Src/stm32g0xx_hal_msp.c     HAL MSP configuration file
  - Src/stm32g0xx_it.c          Interrupt handlers
  - Src/system_stm32g0xx.c      STM32G0xx system source file


@par Hardware and Software environment

  - This example runs on STM32G031xx devices.
  - This demo has been tested with STM32G0316-DISCO board and can be
    easily tailored to any other supported device and development board.

@par How to use it ? 

In order to make the program work, you must do the following :
 - Open your preferred toolchain
 - Rebuild all files and load your image into target memory
 - Run the demo

 * <h3><center>&copy; COPYRIGHT STMicroelectronics</center></h3>
 */
