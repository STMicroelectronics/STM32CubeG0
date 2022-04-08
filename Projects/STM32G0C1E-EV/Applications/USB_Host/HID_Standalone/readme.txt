/**
  @page HID_Standalone USB Host Human Interface (HID) application

  @verbatim
  ******************** (C) COPYRIGHT 2020 STMicroelectronics *******************
  * @file    USB_Host/HID_Standalone/readme.txt
  * @author  MCD Application Team
  * @brief   Description of the USB Host HID application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2020 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  @endverbatim

@par Application Description

This application is a part of the USB Host Library package using STM32Cube firmware. It describes how to use
USB host application based on the Human Interface Class (HID) on the STM32G0XX devices.

This is a typical application on how to use the STM32G0xx USB DRD Host peripheral to interact with an USB
HID Device such as a Mouse or a Keyboard.

At the beginning of the main program the HAL_Init() function is called to reset all the peripherals, initialize the Flash interface and the systick.
The SystemClock_Config() function is used to configure the system clock for STM32G0xx Devices.

The USB DRD IP in full speed mode USB module uses internally a 48-MHz clock which is coming from a specific output of PLL or HSI48.

When the application is started, the connected HID device (Mouse/Keyboard) is detected in HID mode and 
gets initialized. This application is based on interacting with a HID device (Mouse/Keyboard) through a HID routine.

The STM32 MCU behaves as a HID Host, it enumerates the device and extracts VID, PID, 
manufacturer name, Serial number and product name information and displays it on The UART Hyperterminal.


@note Care must be taken when using HAL_Delay(), this function provides accurate delay (in milliseconds)
      based on variable incremented in SysTick ISR. This implies that if HAL_Delay() is called from
      a peripheral ISR process, then the SysTick interrupt must have higher priority (numerically lower)
      than the peripheral interrupt. Otherwise the caller ISR process will be blocked.
      To change the SysTick interrupt priority you have to use HAL_NVIC_SetPriority() function.

@note The application needs to ensure that the SysTick time base is always set to 1 millisecond
      to have correct HAL operation.

For more details about the STM32Cube USB Host library, please refer to UM1720
"STM32Cube USB Host library".

- @note In case of using an AZERTY keyboard, user should add "AZERTY_KEYBOARD" define to ensure correct 
displaying taped characters.

It is possible to fine tune needed USB Host features by modifying defines values in USBH configuration
file “usbh_conf.h” available under the project includes directory, in a way to fit the application
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

@par Directory contents

  - USB_Host/HID_Standalone/Core/Src/main.c                         Main program
  - USB_Host/HID_Standalone/Core/Src/system_stm32g0xx.c             STM32G0xx  system clock configuration File
  - USB_Host/HID_Standalone/Core/Src/stm32g0xx_it.c                 Interrupt handlers
  - USB_Host/HIS_Standalone/Core/Src/stm32g0xx_hal_msp.c            HAL MSP Module
  - USB_Host/HID_Standalone/Core/Inc/main.h                         Main program header File
  - USB_Host/HID_Standalone/Core/Inc/stm32g0xx_hal_conf.h           HAL configuration File
  - USB_Host/HID_Standalone/Core/Inc/stm32g0xx_it.h                 Interrupt handlers header File
  - USB_Host/HID_Standalone/USB_Host/App/usb_host.c                 HID State Machine
  - USB_Host/HID_Standalone/USB_Host/App/mouse.c                    HID mouse functions File
  - USB_Host/HID_Standalone/USB_Host/App/keyboard.c                 HID keyboard functions File
  - USB_Host/HID_Standalone/USB_Host/App/usb_host.h                 HID State Machine Header File
  - USB_Host/HID_Standalone/USB_Host/App/mouse.h                    HID mouse functions Header File
  - USB_Host/HID_Standalone/USB_Host/App/keyboard.h                 HID keyboard functions Header File
  - USB_Host/HID_Standalone/USB_Host/Target/usbh_conf.c             General low level driver configuration
  - USB_Host/HID_Standalone/USB_Host/Target/usbh_conf.h             USB Host driver configuration File
  - USB_Host/MSC_Standalone/USB-PD/usbpd.c                          Ucpd init file
  - USB_Host/MSC_Standalone/USB-PD/usbpd.h                          header of Ucpd init file
  - USB_Host/MSC_Standalone/USB-PD/usbpd_devices_conf.h             UCPD device configuration file
  - USB_Host/MSC_Standalone/USB-PD/usbpd_dpm_conf.h                 UCPD stack configuration file
  - USB_Host/MSC_Standalone/USB-PD/usbpd_dpm_core.h                 device policy manager core header file
  - USB_Host/MSC_Standalone/USB-PD/usbpd_dpm_core.c                 device policy manager core file
  - USB_Host/MSC_Standalone/USB-PD/usbpd_dpm_user.h                 device policy manager users header file
  - USB_Host/MSC_Standalone/USB-PD/usbpd_dpm_user.c                 device policy manager users file
  - USB_Host/MSC_Standalone/USB-PD/usbpd_pwr_if.h                   power if management header file
  - USB_Host/MSC_Standalone/USB-PD/usbpd_pwr_if.c                   power if management file


@par Hardware and Software environment

  - This application runs on STM32G0xx devices.

  - This application has been tested with STMicroelectronics STM32G0C1E-EV board
    and can be easily tailored to any other supported device and development board.

  - This application has been tested with STMicroelectronics STM32G0C1E-EV
    boards and can be easily tailored to any other supported device
    and development board.

  - STM32G0C1E-EV Set-up

@note When the power source jumper JP24 is configured to D5V position (with this configuration the mother board is powered from the daughter board),
      there is no risk to get an over voltage on USB Vbus connector CN7. When the ST-Link is used as power source there is a risk to get an over voltage in connector CN7.
      In such power configuration, software safety protection is required to detect the overvoltage. Please refer to SAFETY PROTECTION CODE BEGIN section in main.c.
      

    - Plug the USB key into the STM32G0C1E-EV board through 'USB Type C-Male
      to A-Female' cable to the connector:CN7
    - Connect ST-Link cable to the PC USB port to display data on the HyperTerminal.

    A virtual COM port will then appear in the HyperTerminal:

     - Hyperterminal configuration:
      - Data Length = 8 Bits
      - One Stop Bit
      - No parity
      - BaudRate = 115200 baud
      - Flow control: None
	  
@note User shall remove the jumpers JP3 and JP4


@par How to use it ?

In order to make the program work, you must do the following :
 - Open your preferred toolchain
 - Rebuild all files and load your image into target memory
 - Open the configured uart hyperterminal in order to display debug messages.
 - Run the application
 - Open the configured uart hyperterminal in order to display debug messages.


@note
   The user has to check the list of the COM ports in Device Manager to find out the number of the
   COM ports that have been assigned (by OS) to the Stlink VCP .



 */
