/**
  @page RCC_LSEConfig RCC Clock Config example

  @verbatim
  ******************************************************************************
  * @file    RCC/RCC_LSEConfig/readme.txt
  * @author  MCD Application Team
  * @brief   Description of the RCC Clock Config example.
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

Enabling/disabling of the low-speed external(LSE) RC oscillator (about 32 KHz) at run time, using the RCC HAL API.


Board: NUCLEO-G071RB (embeds a STM32G071RB device)
Microcontroller Clock Output MCO1 Pin: PA.08

   _________________________                   _______________________
  |     _________CN10_______|                 |  _______________      |
  |    |   RCC              |                 | |               | O   |
  |    |                    |                 | |   _   _   _   | O   |
  |    |(pin 23) MCO1(PA.08)|_________________| | _| |_| |_| |_ | --  |
  |    |                    |                 | |_______________| --  |
  |    |                    |                 |                       |
  |    |                    |                 |_Oscilloscope__________|
  |    |                    |
  |    |                    |
  |    |____________________|
  |                         |
  |                         |
  |_STM32_Board ____________|

In this example, after startup SYSCLK is configured to the max frequency using the PLL with
HSI as clock source, the User push-button (connected to EXTI_Line4_15)
is used to enable/disable LSE:
each time the User push-button is pressed, EXTI_Line4_15 interrupt is generated and the ISR
enables the LSE when it is off, disables it when it is on.

LED4 is toggling as follows according to LSE activation:
- when LSE is on, LED4 is emitting a double flash every half-second
- when LSE is off, LED4 is fast-toggling (every 100 ms)


LSE clock waveform is available on the MCO1 pin PA.08 and can be captured
on an oscilloscope.

In case of configuration error, LED4 is slowly blinking (1 sec. Period).


@note Care must be taken when using HAL_Delay(), this function provides accurate delay (in milliseconds)
      based on variable incremented in SysTick ISR. This implies that if HAL_Delay() is called from
      a peripheral ISR process, then the SysTick interrupt must have higher priority (numerically lower)
      than the peripheral interrupt. Otherwise the caller ISR process will be blocked.
      To change the SysTick interrupt priority you have to use HAL_NVIC_SetPriority() function.

@note The application need to ensure that the SysTick time base is always set to 1 millisecond
      to have correct HAL operation.

@par Keywords

System, RCC, PLL, PLLCLK, SYSCLK, LSE, Clock, Oscillator,

@par Directory contents

  - RCC/RCC_LSEConfig/Inc/stm32g0xx_hal_conf.h    HAL configuration file
  - RCC/RCC_LSEConfig/Inc/stm32g0xx_it.h          Interrupt handlers header file
  - RCC/RCC_LSEConfig/Inc/main.h                        Header for main.c module
  - RCC/RCC_LSEConfig/Src/stm32g0xx_it.c          Interrupt handlers
  - RCC/RCC_LSEConfig/Src/main.c                        Main program
  - RCC/RCC_LSEConfig/Src/system_stm32g0xx.c      STM32G0xx system source file
  - RCC/RCC_LSEConfig/Src/stm32g0xx_hal_msp.c     HAL MSP module

@par Hardware and Software environment

  - This example runs on  STM32G071xx devices.

  - This example has been tested with NUCLEO-G071RB
    board and can be easily tailored to any other supported device
    and development board.

@par How to use it ?

In order to make the program work, you must do the following :
 - Open your preferred toolchain
 - Rebuild all files and load your image into target memory
 - Run the example

 * <h3><center>&copy; COPYRIGHT STMicroelectronics</center></h3>
 */
