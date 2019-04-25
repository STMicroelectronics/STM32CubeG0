/**
  @page RCC_LSIConfig RCC Clock Config example
  
  @verbatim
  ******************************************************************************
  * @file    RCC/RCC_LSIConfig/readme.txt 
  * @author  MCD Application Team
  * @brief   Description of the RCC Clock Config example.
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

Enabling/disabling of the low-speed internal (LSI) RC oscillator (about 32 KHz) at run time, using the RCC HAL API.


Board: NUCLEO-G031K8 (embeds a STM32G031K8 device)
Microcontroller Clock Output MCO1 Pin: PA.08

   _________________________                   _______________________
  |     _________CN3_______|                 |  _______________      |
  |    |   RCC              |                 | |               | O   |
  |    |                    |                 | |   _   _   _   | O   |
  |    |(pin 12) MCO1(PA.08)|_________________| | _| |_| |_| |_ | --  |
  |    |                    |                 | |_______________| --  |
  |    |                    |                 |                       |
  |    |                    |                 |_oscilloscope__________|
  |    |                    |
  |    |                    |
  |    |____________________|
  |                         |
  |                         |
  |_STM32_Board ____________|

In this example, after startup SYSCLK is configured to the max frequency using the PLL with
HSI as clock source, the PA.15 (Arduino D2) (connected to External line 15)
is used to enable/disable LSI:
each time the PA.15 (Arduino D2) is connected/disconnected to GND, External line 15 interrupt is generated and the ISR
enables the LSI when it is off, disables it when it is on.

When only one LED is available on the Board
LED3 is toggling as follows according to LSI activation:
- when LSI is on, LED3 is emitting a double flash every half-second
- when LSI is off, LED3 is fast-toggling (every 100 ms)


LSI clock waveform is available on the MCO1 pin PA.08(D9 or connector CN3 pin12)   and can be captured
on an oscilloscope.

In case of configuration error, LED3 is slowly toggling (1 sec. period).

@note Care must be taken when using HAL_Delay(), this function provides accurate delay (in milliseconds)
      based on variable incremented in SysTick ISR. This implies that if HAL_Delay() is called from
      a peripheral ISR process, then the SysTick interrupt must have higher priority (numerically lower)
      than the peripheral interrupt. Otherwise the caller ISR process will be blocked.
      To change the SysTick interrupt priority you have to use HAL_NVIC_SetPriority() function.

@note The application need to ensure that the SysTick time base is always set to 1 millisecond
      to have correct HAL operation.

@par Keywords

System, RCC, PLL, PLLCLK, SYSCLK, LSI, Clock, Oscillator,

@par Directory contents

  - RCC/RCC_LSIConfig/Inc/stm32g0xx_hal_conf.h    HAL configuration file
  - RCC/RCC_LSIConfig/Inc/stm32g0xx_it.h          Interrupt handlers header file
  - RCC/RCC_LSIConfig/Inc/main.h                        Header for main.c module
  - RCC/RCC_LSIConfig/Src/stm32g0xx_it.c          Interrupt handlers
  - RCC/RCC_LSIConfig/Src/main.c                        Main program
  - RCC/RCC_LSIConfig/Src/system_stm32g0xx.c      STM32G0xx system source file
  - RCC/RCC_LSIConfig/Src/stm32g0xx_hal_msp.c     HAL MSP module

@par Hardware and Software environment

  - This example runs on STM32G031K8Tx devices.

  - This example has been tested with NUCLEO-G031K8
    board and can be easily tailored to any other supported device
    and development board.

@par How to use it ?

In order to make the program work, you must do the following :
 - Open your preferred toolchain
 - Rebuild all files and load your image into target memory
 - Run the example

 * <h3><center>&copy; COPYRIGHT STMicroelectronics</center></h3>
 */
