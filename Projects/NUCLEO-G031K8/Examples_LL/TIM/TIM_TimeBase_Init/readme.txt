/**
  @page TIM_TimeBase_Init TIM example
  
  @verbatim
  ******************************************************************************
  * @file    Examples_LL/TIM/TIM_TimeBase_Init/readme.txt 
  * @author  MCD Application Team
  * @brief   Description of the TIM_TimeBase_Init example.
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

Configuration of the TIM peripheral to generate a timebase. This 
example is based on the STM32G0xx TIM LL API. The peripheral initialization 
uses LL unitary service functions for optimization purposes (performance and size). 

   In this example TIM1 input clock TIM1CLK is set to APB2 clock (PCLK2),   
   since APB2 pre-scaler is equal to 1.                                     
      TIM1CLK = PCLK2                                                       
      PCLK1 = HCLK                                                          
      => TIM1CLK = SystemCoreClock (56 MHz)                                 

To set the TIM1 counter clock frequency to 10 KHz, the pre-scaler (PSC) is calculated as follows:
PSC = (TIM1CLK / TIM1 counter clock) - 1
PSC = (SystemCoreClock /10 KHz) - 1

SystemCoreClock is set to 64 MHz for STM32G0xx Devices.

The auto-reload (ARR) is calculated to get a timebase period of 100ms,
meaning that initial timebase frequency is 10 Hz.
ARR = (TIM1 counter clock / timebase frequency) - 1
ARR = (TIM1 counter clock / 10) - 1

Update interrupts are enabled. Within the update interrupt service routine, pin LED3
(connected to LED3 on board NUCLEO-G031K8) is toggled. So the period of
blinking of LED3 = 2 * timebase period.

Put and remove a jumper between PA.15 (Arduino D2) and GND to update the
autoreload register (ARR). In up-counting update event is generated  at each counter overflow
(when the counter reaches the auto-reload value).

Finally the timebase frequency is calculated as follows:
timebase frequency = TIM1 counter clock /((PSC + 1)*(ARR + 1)*(RCR + 1))

@par Directory contents 

  - TIM/TIM_TimeBase_Init/Inc/stm32g0xx_it.h          Interrupt handlers header file
  - TIM/TIM_TimeBase_Init/Inc/main.h                  Header for main.c module
  - TIM/TIM_TimeBase_Init/Inc/stm32_assert.h          Template file to include assert_failed function
  - TIM/TIM_TimeBase_Init/Src/stm32g0xx_it.c          Interrupt handlers
  - TIM/TIM_TimeBase_Init/Src/main.c                  Main program
  - TIM/TIM_TimeBase_Init/Src/system_stm32g0xx.c      STM32G0xx system source file


@par Hardware and Software environment

  - This example runs on STM32G031K8Tx devices.
    
  - This example has been tested with NUCLEO-G031K8 board and can be
    easily tailored to any other supported device and development board.

@par How to use it ? 

In order to make the program work, you must do the following :
 - Open your preferred toolchain
 - Rebuild all files and load your image into target memory
 - Run the example

 * <h3><center>&copy; COPYRIGHT STMicroelectronics</center></h3>
 */
