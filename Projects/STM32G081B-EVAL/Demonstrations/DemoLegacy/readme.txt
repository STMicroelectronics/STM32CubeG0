/**
  @page Demo   Demo STM32G081B-EVAL
 
  @verbatim
  ******************************************************************************
  * @file    readme.txt
  * @author  MCD Application Team
  * @brief   Description of STM32G081B-EVAL Demo legacy
  ******************************************************************************
  *
  * Copyright (c) 2018 STMicroelectronics. All rights reserved.
  *
  * This software component is licensed by ST under Ultimate Liberty license
  * SLA0044, the "License"; You may not use this file except in compliance with
  * the License. You may obtain a copy of the License at:
  *                               www.st.com/SLA0044
  *
  ******************************************************************************
  @endverbatim

@par Demo Description

The provided demonstration "Legacy" firmware based on STM32Cube helps you to discover STM32
Cortex-M devices that can be plugged on a STM32G081B-EVAL board.

At the beginning of the main program the HAL_Init() function is called to reset
all the peripherals, initialize the Flash interface and the systick.
Then the SystemClock_Config() function is used to configure the system clock
(SYSCLK) to run at 64 MHz.

The legacy part of the demonstration firmware provides a sub-set of applications
supported by the STM32081B-EVAL board equiped with Legacy daughter board(MB1351).

Below you find the different items of demonstration :

 - calendar : application to set date, time and alarm on a calendar
 - image viewer : displays the pictures present on the SD card (\USER).
 - audio player/recorder : application to play & record wave files.
 - thermometer & LDR : displays the temperature & Light level.
 - low power mode : allows to enter in low power mode(stop and standby)
                    and the wakeup can be ordered by an EXTI or an alarm.
 - file browser : aplication to browse on the SD card.
 - Help : indicates date & version of demonstration FW
 - About : describes HW configuration( jumper, etc..) to run demonstration FW

@note Care must be taken when using HAL_Delay(), this function provides accurate
      delay (in milliseconds) based on variable incremented in SysTick ISR.
      This implies that if HAL_Delay() is called from a peripheral ISR process,
      then the SysTick interrupt must have higher priority (numerically lower)
      than the peripheral interrupt. Otherwise the caller ISR process will be blocked.
      To change the SysTick interrupt priority you have to use HAL_NVIC_SetPriority() function.
      
@note The application needs to ensure that the SysTick time base is always set to
      1 millisecond to have correct HAL operation.


@par Hardware and Software environment

  - This demonstration runs only on STM32G081B-EVAL mother board (MB1350, rev B)
    with Legacy daughter board (MB1351, rev A).

  - A SDSC microSD card (capacity up to 4GB), must contain all the ressources files
    available under the FW package Binary/SD_card folder.

@par How to use it ?

In order to make the program work, you must do the following :
 - Open your preferred toolchain
 - Rebuild all files and load your image into target memory
 - Run the example

@note Each software development environment provides two configurations
      1. Standalone: once built, the legacy demonstration software is flashed directly on the
      board and executed right after. In that mode user has to make sure that the legacy daughter
      board (MB1351) or no daughter board is connected to the extension connector of the mother
      board (MB1350). No binary file is provided for standalone mode. It has to be rebuild based
      on the user's preferred IDE.
      2. Relocate: once built, the legacy demonstration software executable must be copied in a
      specific folder of the SD card. It is copied from SD card to the flash memory by the
      demonstration loader if the legacy daughter board (MB1351) or no daughter board is
      connected to the extension connector of the mother board (MB1350).

 * <h3><center>&copy; COPYRIGHT STMicroelectronics</center></h3>
 */
