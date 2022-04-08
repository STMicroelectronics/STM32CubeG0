/**
  @page FatFs_MultiAccess_RTOS  FatFs with multiAccess RTOS application

  @verbatim
  ******************************************************************************
  * @file    FatFs/FatFs_MultiAccess_RTOS/readme.txt
  * @author  MCD Application Team
  * @brief   Description of the FatFs with multiAccess RTOS application
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2019 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  @endverbatim

@par Application Description

How to use STM32Cube firmware with FatFs middleware component as a generic FAT
file system module, FreeRTOS as an RTOS module based on using CMSIS-OS-V2 wrapping
layer common APIs.
This example develops an application exploiting FatFs features
with a FatFs disk drive in RTOS mode configuration.

At the beginning of the main program the HAL_Init() function is called to reset
all the peripherals, initialize the Flash interface and the systick.
Then the SystemClock_Config() function is used to configure the system clock
(SYSCLK) to run at 64 MHz.

The application is based on writing two text files to a drive.
Two threads are created to manage multiple access to the FAT volumes
through FatFs APIs as described in the following steps:

 - StartThread executing steps below:
   - Link the FatFs disk I/O driver;
   - Register the file system object (mount) to the FatFs module for the uSD drive;
   - Create and Open new text file object with write access;
   - Write data to the text file;
   - Close the open text file.

 - ConcurrentThread executing steps below:
   - Create and Open new text file object with write access;
   - Write data to the text file;
   - Close the open text file;
   - Unlink the FatFs  disk I/O driver.

It is worth noting that the application manages any error occurred during the
access to FAT volume, when using FatFs APIs. Otherwise, user can check if the
written text files are available on the uSD card.

It is possible to fine tune needed FatFs features by modifying defines values
in FatFs configuration file “ffconf.h” available under the project includes
directory, in a way to fit the application requirements.

It is possible to fine tune needed FatFs features by modifying defines values
in FatFs configuration file "ffconf.h" available under the project includes
directory, in a way to fit the application requirements.

It is possible to dynamically hot plug/unplug the uSD card, as the application will
handle the plug/unplug events.


@note: for some uSD's, replacing it  while the application is running makes the application
       fail. It is recommended to reset the board using the "Reset button" after replacing
       the uSD.

STM32G0C1E-EV's LED can be used to monitor the application status:
  - LED1 is ON when the application runs successfully.
  - LED3 is ON when any error occurs.
  - LED3 toggle when SD card is unplugged.

@note Care must be taken when using HAL_Delay(), this function provides accurate delay (in milliseconds)
      based on variable incremented in SysTick ISR. This implies that if HAL_Delay() is called from
      a peripheral ISR process, then the SysTick interrupt must have higher priority (numerically lower)
      than the peripheral interrupt. Otherwise the caller ISR process will be blocked.
      To change the SysTick interrupt priority you have to use HAL_NVIC_SetPriority() function.

@note The application needs to ensure that the SysTick time base is always set to 1 millisecond
      to have correct HAL operation.

@par Keywords

FatFs, FreeRTOS, SD Card, File system, FAT Volume, Format, Mount, Read, Write

@par Directory contents

  - FatFs/FatFs_MultiAccess_RTOS/Core/Inc/stm32g0xx_hal_conf.h               HAL configuration file
  - FatFs/FatFs_MultiAccess_RTOS/Core/Inc/stm32g0xx_it.h                     Interrupt handlers header file
  - FatFs/FatFs_MultiAccess_RTOS/Core/Inc/main.h                             Header for main.c module
  - FatFs/FatFs_MultiAccess_RTOS/Core/Src/stm32g0xx_it.c                     Interrupt handlers
  - FatFs/FatFs_MultiAccess_RTOS/Core/Src/main.c                             Main program
  - FatFs/FatFs_MultiAccess_RTOS/Core/Src/system_stm32g0xx.c                 STM32G0xx system source file
  - FatFs/FatFs_MultiAccess_RTOS/Src/stm32g0xx_hal_timebase_tim.c HAL timebase file
  - FatFs/FatFs_MultiAccess_RTOS/Inc/FreeRTOSConfig.h             FreeRTOS Configuration file
  - FatFs/FatFs_MultiAccess_RTOS/FATFS/Target/ffconf.h                       FAT file system module configuration file
  - FatFs/FatFs_MultiAccess_RTOS/FATFS/Target/sd_diskio.h                    uSD diskio header file
  - FatFs/FatFs_MultiAccess_RTOS/FATFS/Target/sd_diskio.c                    FatFs uSD diskio driver
  - FatFs/FatFs_MultiAccess_RTOS/FATFS/App/app_fatfs.h                       Header file for App_fatfs.c file
  - FatFs/FatFs_MultiAccess_RTOS/FATFS/App/app_fatfs.c                       FatFs application code


@par Hardware and Software environment

  - This application runs on STM32G0C1VETx devices

  - This application has been tested with STM32G0C1E-EV board and can be
    easily tailored to any other supported device and development board.

@par How to use it ?

In order to make the program work, you must do the following:

 - Insert a microSD card in the board appropriate slot
 - Open your preferred toolchain
 - Rebuild all files and load your image into target memory
 - Run the application


 */
