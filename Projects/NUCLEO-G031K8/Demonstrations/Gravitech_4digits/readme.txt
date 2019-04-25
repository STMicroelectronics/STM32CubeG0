/**
  @page Demo   Demo NUCLEO-G031K8
 
  @verbatim
  ******************************************************************************
  * @file    readme.txt 
  * @author  MCD Application Team
  * @brief   Description of NUCLEO-G031K8 Demo
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
STM32NUCLEO_32 board.

At the beginning of the main program the HAL_Init() function is called to reset 
all the peripherals, initialize the Flash interface and the systick.
Then the SystemClock_Config() function is used to configure the system clock
(SYSCLK) to run at 64 MHz.
 
 
Below you find the sequence to discover the demonstration :

  This demo needs a Gravitech 4 digits 7 segment nano shield.
  You will find it here : http://www.gravitech.us/7sediforarna.html
  Plug the STM32NUCLEO_32 on top of this shield.
  The demo changes the power mode every 10 seconds and updates the display every second.
  The last digit corresponds to the time.
  The first digit corresponds to the power mode as is described below :
    100X = RUN Mode
    200X = SLEEP Mode
    300X = STOP Mode
    400X = STANDBY Mode
    500X = LPRUN Mode
    600X = LPSLEEP Mode
  For details about the low power modes, please look at the stm32g0xx reference manual.

@note Care must be taken when using HAL_Delay(), this function provides accurate
      delay (in milliseconds) based on variable incremented in SysTick ISR. 
      This implies that if HAL_Delay() is called from a peripheral ISR process, 
      then the SysTick interrupt must have higher priority (numerically lower)
      than the peripheral interrupt. Otherwise the caller ISR process will be blocked.
      To change the SysTick interrupt priority you have to use HAL_NVIC_SetPriority() function.
      
@note The application need to ensure that the SysTick time base is always set to
      1 millisecond to have correct HAL operation.

@par Directory contents
 
  - Gravitech_4digits/Inc/stm32g0xx_hal_conf.h    HAL configuration file
  - Gravitech_4digits/Inc/stm32g0xx_it.h          Interrupt handlers header file
  - Gravitech_4digits/Inc/main.h                  Header for main.c module
  - Gravitech_4digits/Src/stm32g0xx_it.c          Interrupt handlers
  - Gravitech_4digits/Src/main.c                  Main program
  - Gravitech_4digits/Src/system_stm32g0xx.c      STM32G0xx system source file


@par Hardware and Software environment

  - This example runs on STM32G031xx devices.
  - This demo has been tested with NUCLEO-G031K8 board and can be
    easily tailored to any other supported device and development board.
  - Gravitech 4 digits 7 segment nano shield is connected on Arduino nano connectors.

@par How to use it ? 

In order to make the program work, you must do the following :
  - Open your preferred toolchain 
  - Rebuild all files and load your image into target memory
  - Run the example
 
 * <h3><center>&copy; COPYRIGHT STMicroelectronics</center></h3>
 */
