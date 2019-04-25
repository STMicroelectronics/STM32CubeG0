/**
  @page RCC_ClockSwitch RCC Clock Switch example

  @verbatim
  ******************************************************************************
  * @file    RCC/RCC_ClockSwitch/readme.txt
  * @author  MCD Application Team
  * @brief   Description of the RCC Clock Switch example.
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

Switch of the system clock (SYSCLK) from Low frequency clock to high frequency clock, using the RCC HAL API.

In this example, after startup SYSCLK is configured to the max frequency using the PLL with
HSI as clock source, the Joystick Selection push-button (connected to EXTI_Line0_1) will be
used to change the system clock source:
- from LSI to PLL
- from PLL to LSI

Each time the Joystick Selection push-button is pressed EXTI_Line0_1 interrupt is generated and in the ISR
the System Clock source is checked using __HAL_RCC_GET_SYSCLK_SOURCE() macro:

- If PLL is selected as System clock source, the following steps will be followed to switch
   SYSCLK from PLL to LSI:
     a- Configure the SysTick to have interrupt in 100ms time basis
     b- Select the LSI as system clock source

- If the LSI oscillator is selected as System clock source, the following steps will be followed to switch
   SYSCLK from LSI to PLL:
     a- Select the PLL as system clock source
     b- Configure the SysTick to have interrupt in 1ms time basis

In this example the SYSCLK is outputed on the MCO1 pin(PA.08).
MCO1 pin may be outputed on PA.08 depending on SB configuration.

LED6 is toggling with a timing defined by HAL_IncTick() API depending on the system clock source.
LED4 will turn ON if any error occurs.

@note Care must be taken when using HAL_Delay(), this function provides accurate delay (in milliseconds)
      based on variable incremented in SysTick ISR. This implies that if HAL_Delay() is called from
      a peripheral ISR process, then the SysTick interrupt must have higher priority (numerically lower)
      than the peripheral interrupt. Otherwise the caller ISR process will be blocked.
      To change the SysTick interrupt priority you have to use HAL_NVIC_SetPriority() function.
      
@par Keywords

System, RCC, PLL, PLLCLK, SYSCLK, LSI, Swicth Clock, Oscillator,

@par Directory contents

  - RCC/RCC_ClockSwitch/Inc/stm32g0xx_hal_conf.h        HAL configuration file
  - RCC/RCC_ClockSwitch/Inc/stm32g0xx_it.h              Interrupt handlers header file
  - RCC/RCC_ClockSwitch/Inc/main.h                      Header for main.c module
  - RCC/RCC_ClockSwitch/Src/stm32g0xx_it.c              Interrupt handlers
  - RCC/RCC_ClockSwitch/Src/main.c                      Main program
  - RCC/RCC_ClockSwitch/Src/system_stm32g0xx.c          STM32G0xx system source file
  - RCC/RCC_ClockSwitch/Src/stm32g0xx_hal_msp.c   HAL MSP module

@par Hardware and Software environment

  - This example runs on STM32G071RB devices.

  - This example has been tested with STM32G071B-DISCO board and can be easily tailored to any other supported device
    and development board.


@par How to use it ?

In order to make the program work, you must do the following :
 - Open your preferred toolchain
 - Rebuild all files and load your image into target memory
 - Run the example

 * <h3><center>&copy; COPYRIGHT STMicroelectronics</center></h3>
 */
