/**
  @page MSC_Standalone USB Host Mass Storage (MSC) application

  @verbatim
  ******************** (C) COPYRIGHT 2020 STMicroelectronics *******************
  * @file    USB_Host/MSC_Standalone/readme.txt
  * @author  MCD Application Team
  * @brief   Description of the USB Host MSC application.
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
USB host application based on the Mass Storage Class (MSC) on the STM32G0XX devices.

This is a typical application on how to use the stm32g0xx USB DRD Host peripheral to operate with an USB
flash disk using the Bulk Only Transfer (BOT) and Small Computer System Interface (SCSI) transparent
commands combined with a file system FatFs (Middleware component).

At the beginning of the main program the HAL_Init() function is called to reset all the peripherals, initialize the Flash interface and the systick.
The SystemClock_Config() function is used to configure the system clock for stm32g0xx Devices :
The CPU at 64 MHz
The HCLK for D1 Domain AXI peripherals, D2 Domain AHB peripherals and D3 Domain AHB peripherals at 64 MHz.
The APB clock dividers for D1 Domain APB3 peripherals, D2 Domain APB1 and APB2 peripherals and D3 Domain APB4 peripherals to run at 64 MHz.

The Full speed DRD IP in full speed mode USB module uses internally a 48-MHz clock which is coming from a specific output of PLL.

When the application is started, the connected USB flash disk device is detected in MSC mode and gets
initialized. The STM32 MCU behaves as a MSC Host, it enumerates the device and extracts VID, PID,
manufacturer name, Serial no and product name information and displays it on the UART Hyperterminal.
This application is based on read/write file and explore the USB flash disk content through a MSC routine.

Debug messages are desplayed along with each new step of the application :
 - "File Operations" operation writes a small text file (less to 1 KB) on the USB flash disk.
 - "Explorer Disk" operation explores the USB flash disk content and displays it on the uart terminal.
    Display the whole USB flash disk content (recursion level 2).

@note Care must be taken when using HAL_Delay(), this function provides accurate delay (in milliseconds)
      based on variable incremented in SysTick ISR. This implies that if HAL_Delay() is called from
      a peripheral ISR process, then the SysTick interrupt must have higher priority (numerically lower)
      than the peripheral interrupt. Otherwise the caller ISR process will be blocked.
      To change the SysTick interrupt priority you have to use HAL_NVIC_SetPriority() function.

@note The application needs to ensure that the SysTick time base is always set to 1 millisecond
      to have correct HAL operation.

For more details about the STM32Cube USB Host library, please refer to UM1720
"STM32Cube USB Host library".


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

Connectivity, USB_Host, USB, MSC, Mass Storage, Full Speed, BOT, SCSI, Removable drive, FatFs,
File system, Write, Read, Format

@par Directory contents

   - USB_Host/MSC_Standalone/USB_Host/Target/usbh_conf.c           General low level driver configuration
   - USB_Host/MSC_Standalone/USB_Host/Target/usbh_conf.h           USB Host driver Configuration file
   - USB_Host/MSC_Standalone/USB_Host/App/explorer.c               Explore the USB flash disk content
   - USB_Host/MSC_Standalone/USB_Host/App/file_operations.c        Write/read file on the disk
   - USB_Host/MSC_Standalone/USB_Host/App/usbh_diskio.c            USB diskio interface for FatFs
   - USB_Host/MSC_Standalone/USB_Host/App/host.c                   MSC State Machine
   - USB_Host/MSC_Standalone/Core/Src/main.c                       Main program
   - USB_Host/MSC_Standalone/Core/Src/system_stm32g0xx.c           STM32G0xx  system clock configuration file
   - USB_Host/MSC_Standalone/Core/Src/stm32g0xx_it.c               Interrupt handlers
   - USB_Host/MSC_Standalone/Core/Src/stm32g0xx_hal_msp.c          HAL MSP Module
   - USB_Host/MSC_Standalone/Core/Inc/main.h                       Main program header file
   - USB_Host/MSC_Standalone/Core/Inc/stm32g0xx_it.h               Interrupt handlers header file
   - USB_Host/MSC_Standalone/Core/Inc/stm32g0xx_hal_conf.h         HAL configuration file
   - USB_Host/MSC_Standalone/Core/Inc/ffconf.h                     FatFs Module Configuration file
   - USB_Host/MSC_Standalone/USB-PD/usbpd.c                        Ucpd init file
   - USB_Host/MSC_Standalone/USB-PD/usbpd.h                        header of Ucpd init file
   - USB_Host/MSC_Standalone/USB-PD/usbpd_devices_conf.h           UCPD device configuration file
   - USB_Host/MSC_Standalone/USB-PD/usbpd_dpm_conf.h               UCPD stack configuration file
   - USB_Host/MSC_Standalone/USB-PD/usbpd_dpm_core.h               device policy manager core header file
   - USB_Host/MSC_Standalone/USB-PD/usbpd_dpm_core.c               device policy manager core file
   - USB_Host/MSC_Standalone/USB-PD/usbpd_dpm_user.h               device policy manager users header file
   - USB_Host/MSC_Standalone/USB-PD/usbpd_dpm_user.c               device policy manager users file
   - USB_Host/MSC_Standalone/USB-PD/usbpd_pwr_if.h                 power if management header file
   - USB_Host/MSC_Standalone/USB-PD/usbpd_pwr_if.c                 power if management file




@par Hardware and Software environment

  - This application runs on STM32G0xx  devices.

  - This application has been tested with STMicroelectronics STM32G0C1E-EV
    boards and can be easily tailored to any other supported device
    and development board.

  - STM32G0C1E-EV Set-up

@note When the power source jumper JP24 is configured to D5V position (with this configuration the mother board is powered from the daughter board),
      there is no risk to get an over voltage on USB Vbus connector CN7. When the ST-Link is used as power source there is a risk to get an over voltage in connector CN7.
      In such power configuration, software safety protection is required to detect the overvoltage. Please refer to SAFETY PROTECTION CODE BEGIN section in main.c.


     - Plug the USB key into the STM32G0C1E-EV board through 'USB micro Type C-Male
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
