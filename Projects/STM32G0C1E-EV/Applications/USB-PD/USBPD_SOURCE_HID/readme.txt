/**
  @page USBPD_SOURCE_HID USB Source Host Human Interface (HID) application

  @verbatim
  ******************** (C) COPYRIGHT 2021 STMicroelectronics *******************
  * @file    readme.txt
  * @author  MCD Application Team
  * @brief   Description of the USB Host HID application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2021 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  @endverbatim

@par Application Description

This application is type C provider and USB Host using STM32Cube firmware. It demonstrates how to use
a type C provider in the case of an USB host application based on the Human Interface Class (HID) on the STM32G0XX devices.

  USB HID : This is a typical application on how to use the STM32G0xx USB DRD Host peripheral to interact with an USB
  HID Device such as a Mouse or a Keyboard.

  USBPD provider: This application initialize the type C port 1 in source mode with only one PDO at 5V.


The USB DRD IP in full speed mode USB module uses internally a 48-MHz clock which is coming from a specific output of PLL or HSI48.

When the application is started, the connected HID device (Mouse/Keyboard) is detected in HID mode and 
gets initialized. This application is based on interacting with a HID device (Mouse/Keyboard) through a HID routine.

The STM32 MCU behaves as a HID Host, it enumerates the device and extracts VID, PID, 
manufacturer name, Serial number and product name information and displays it on The UART Hyperterminal.

Connect UCPD cube Monitor on the VCP associated to our board (only available if USB cable is connected)
The UCPD Cube Monitor will be used to trace all the messages exchange between the ports partner.  

@note Care must be taken when using HAL_Delay(), this function provides accurate delay (in milliseconds)
      based on variable incremented in SysTick ISR. This implies that if HAL_Delay() is called from
      a peripheral ISR process, then the SysTick interrupt must have higher priority (numerically lower)
      than the peripheral interrupt. Otherwise the caller ISR process will be blocked.
      To change the SysTick interrupt priority you have to use HAL_NVIC_SetPriority() function.

@note The application needs to ensure that the SysTick time base is always set to 1 millisecond
      to have correct HAL operation.

@note The FreeRTOS heap size configTOTAL_HEAP_SIZE defined in FreeRTOSConfig.h is set accordingly to the 
      OS resources memory requirements of the application with +10% margin and rounded to the upper Kbyte boundary.

For more details about FreeRTOS implementation on STM32Cube, please refer to UM1722 "Developing Applications 
on STM32Cube with RTOS".

For more details about the STM32Cube USB Host library, please refer to UM1720
"STM32Cube USB Host library".

- @note In case of using an AZERTY keyboard, user should add "AZERTY_KEYBOARD" define to ensure correct 
displaying taped characters.

It is possible to fine tune needed USB Host features by modifying defines values in USBH configuration
file usbh_conf.h available under the project includes directory, in a way to fit the application
requirements, such as:
- Level of debug: USBH_DEBUG_LEVEL
                  0: No debug messages
                  1: Only User messages are shown
                  2: User and Error messages are shown
                  3: All messages and internal debug messages are shown
   By default debug messages are displayed on the debugger IO terminal; to redirect the Library
   messages to uart terminal, stm32g0xx_hal_uart.c driver needs to be added to the application sources.
   Debug messages are displayed on the uart terminal using ST-Link.



@par Keywords

Connectivity, USB_Host, USB, HID, Human Interface, Full Speed, High Speed, Mouse, Keyboard

USB_PD_Lib, UCPD, Type C, USBPD, FreeRTOS

@par Directory contents

  - USB-PD/USBPD_SOURCE_HID/Core/Src/main.c                         Main program
  - USB-PD/USBPD_SOURCE_HID/Core/Src/system_stm32g0xx.c             STM32G0xx  system clock configuration File
  - USB-PD/USBPD_SOURCE_HID/Core/Src/stm32g0xx_it.c                 Interrupt handlers
  - USB-PD/USBPD_SOURCE_HID/Core/Src/stm32g0xx_hal_msp.c            HAL MSP Module
  - USB-PD/USBPD_SOURCE_HID/Core/Inc/main.h                         Main program header File
  - USB-PD/USBPD_SOURCE_HID/Core/Inc/stm32g0xx_hal_conf.h           HAL configuration File
  - USB-PD/USBPD_SOURCE_HID/Core/Inc/stm32g0xx_it.h                 Interrupt handlers header File
  - USB-PD/USBPD_SOURCE_HID/USB_Host/App/usb_host.c                 HID State Machine
  - USB-PD/USBPD_SOURCE_HID/USB_Host/App/mouse.c                    HID mouse functions File
  - USB-PD/USBPD_SOURCE_HID/USB_Host/App/keyboard.c                 HID keyboard functions File
  - USB-PD/USBPD_SOURCE_HID/USB_Host/App/usb_host.h                 HID State Machine Header File
  - USB-PD/USBPD_SOURCE_HID/USB_Host/App/mouse.h                    HID mouse functions Header File
  - USB-PD/USBPD_SOURCE_HID/USB_Host/App/keyboard.h                 HID keyboard functions Header File
  - USB-PD/USBPD_SOURCE_HID/USB_Host/Target/usbh_conf.c             General low level driver configuration
  - USB-PD/USBPD_SOURCE_HID/USB_Host/Target/usbh_conf.h             USB Host driver configuration File
  - USB-PD/USBPD_SOURCE_HID/USBPD/Src/usbpd.c                       Ucpd init file
  - USB-PD/USBPD_SOURCE_HID/USBPD/Inc/usbpd.h                       header of Ucpd init file
  - USB-PD/USBPD_SOURCE_HID/USBPD/Inc/usbpd_devices_conf.h          UCPD device configuration file
  - USB-PD/USBPD_SOURCE_HID/USBPD/Inc/usbpd_dpm_conf.h              UCPD stack configuration file
  - USB-PD/USBPD_SOURCE_HID/USBPD/Inc/usbpd_dpm_core.h              device policy manager core header file
  - USB-PD/USBPD_SOURCE_HID/USBPD/Src/usbpd_dpm_core.c              device policy manager core file
  - USB-PD/USBPD_SOURCE_HID/USBPD/Inc/usbpd_dpm_user.h              device policy manager users header file
  - USB-PD/USBPD_SOURCE_HID/USBPD/Src/usbpd_dpm_user.c              device policy manager users file
  - USB-PD/USBPD_SOURCE_HID/USBPD/Inc/usbpd_pwr_if.h                power if management header file
  - USB-PD/USBPD_SOURCE_HID/USBPD/Src/usbpd_pwr_if.c                power if management file
  - USB-PD/USBPD_SOURCE_HID/USBPD/Inc/usbpd_pdo_defs.h              pdo definition header file
  - USB-PD/USBPD_SOURCE_HID/USBPD/Inc/usbpd_pwr_user.h              power management header file
  - USB-PD/USBPD_SOURCE_HID/USBPD/Inc/usbpd_vdm_users.h             vendor define message management header file

@par Hardware and Software environment

  - This application runs on STM32G0xx devices.

  - This application has been tested with STMicroelectronics STM32G0C1E-EV board
    and can be easily tailored to any other supported device and development board.

  - STM32G0C1E-EV Set-up

    - Plug the USB key into the STM32G0C1E-EV board through 'USB Type C-Male to A-Female' cable to the connector:CN7
    - Connect ST-Link cable to the PC USB port to display data on the CubeMx Monitor.

@note User shall remove the jumpers JP3 and JP4


@par How to use it ?

In order to make the program work, you must do the following :
 - As UCPD Monitor is using a dedicated flash area for storing USPBD port capabilities, please make sure to erase full flash content before starting. (With STM32CubeProgrammer for example)
 - Open your preferred toolchain
 - Rebuild all files and load your image into target memory
 - Run the application
 - Run CubeMx Monitor and open trace tools
 - Connect a type C sink device on the Type-C connector 1
 
@note
   The user has to check the list of the COM ports in Device Manager to find out the number of the
   COM ports that have been assigned (by OS) to the Stlink VCP .



 */
