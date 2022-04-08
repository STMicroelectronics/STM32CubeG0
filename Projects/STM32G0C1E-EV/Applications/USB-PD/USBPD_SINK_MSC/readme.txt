/**
  @page USBPD_SINK_MSC USB Sink Device Mass Storage (MSC) application

  @verbatim
  ******************************************************************************
  * @file    readme.txt
  * @author  MCD Application Team
  * @brief   Description of the USB MSC application.
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

This application is a type C Consumer and USB Device using STM32Cube firmware. It demonstrates how to use
a type C Consumer in the case of an USB Device application based on the Mass Storage Class (MSC) on the STM32G0xx devices.

  USB MSC : This is a typical application on how to use the STM32G0xx USB Device peripheral to communicate with a PC
  Host using the Bulk Only Transfer (BOT) and Small Computer System Interface (SCSI) transparent commands,
  while the microSD card is used as storage media. The STM32 MCU is enumerated as a MSC device using the
  native PC Host MSC driver to which the STM32G0C1E-EV board is connected.
  
  USBPD Consumer.: This application initialize the type C port 1 in sink mode with only one PDO at 5V.

At the beginning of the main program the HAL_Init() function is called to reset all the peripherals,
initialize the Flash interface and the systick. The user is provided with the SystemClock_Config()
function to configure the system clock (SYSCLK). The Full Speed (FS) USB module uses
internally a 48-MHz clock, which is generated from an internal PLL.

When the application is started, the user has just to plug the USB cable into a PC host and the device
is automatically detected. A new removable drive appears in the system window and write/read/format
operations can be performed as with any other removable drive.

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

For more details about the STM32Cube USB Device library, please refer to UM1734
"STM32Cube USB Device library".

@par Keywords

Connectivity, USB_Device, USB, MSC, Full Speed, flash, microSD card

USB_PD_Lib, UCPD, Type C, USBPD, FreeRTOS

@par Directory contents

  - USB-PD/USBPD_SINK_MSC/Core/Src/main.c                     Main program
  - USB-PD/USBPD_SINK_MSC/Core/Src/system_stm32g0xx.c         stm32g0xx system clock configuration file
  - USB-PD/USBPD_SINK_MSC/Core/Src/stm32g0xx_it.c             Interrupt handlers
  - USB-PD/USBPD_SINK_MSC/Core/Src/stm32g0xx_hal_msp.c        HAL MSP Module
  - USB-PD/USBPD_SINK_MSC/USB_Device/App/usb_device.c         USB Device application code
  - USB-PD/USBPD_SINK_MSC/USB_Device/App/usb_desc.c           USB device descriptor
  - USB-PD/USBPD_SINK_MSC/USB_Device/App/usbd_storage_if.c    Internal flash memory management
  - USB-PD/USBPD_SINK_MSC/USB_Device/Target/usbd_conf.c       General low level driver configuration
  - USB-PD/USBPD_SINK_MSC/Core/Inc/main.h                     Main program header file
  - USB-PD/USBPD_SINK_MSC/Core/Inc/stm32g0xx_it.h             Interrupt handlers header file
  - USB-PD/USBPD_SINK_MSC/Core/Inc/stm32g0xx_hal_conf.h       HAL configuration file
  - USB-PD/USBPD_SINK_MSC/USB_Device/App/usb_device.h         USB Device application header file
  - USB-PD/USBPD_SINK_MSC/USB_Device/App/usbd_desc.h          USB device descriptor header file
  - USB-PD/USBPD_SINK_MSC/USB_Device/App/usbd_storage_if.h    Internal flash memory management header file
  - USB-PD/USBPD_SINK_MSC/USB_Device/Target/usbd_conf.h       USB device driver Configuration file
  - USB-PD/USBPD_SINK_MSC/USBPD/Src/usbpd.c                   Ucpd init file
  - USB-PD/USBPD_SINK_MSC/USBPD/Inc/usbpd.h                   header of Ucpd init file
  - USB-PD/USBPD_SINK_MSC/USBPD/Inc/usbpd_devices_conf.h      UCPD device configuration file
  - USB-PD/USBPD_SINK_MSC/USBPD/Inc/usbpd_dpm_conf.h          UCPD stack configuration file
  - USB-PD/USBPD_SINK_MSC/USBPD/Inc/usbpd_dpm_core.h          device policy manager core header file
  - USB-PD/USBPD_SINK_MSC/USBPD/Src/usbpd_dpm_core.c          device policy manager core file
  - USB-PD/USBPD_SINK_MSC/USBPD/Inc/usbpd_dpm_user.h          device policy manager users header file
  - USB-PD/USBPD_SINK_MSC/USBPD/Src/usbpd_dpm_user.c          device policy manager users file
  - USB-PD/USBPD_SINK_MSC/USBPD/Inc/usbpd_pwr_if.h            power if management header file
  - USB-PD/USBPD_SINK_MSC/USBPD/Src/usbpd_pwr_if.c            power if management file
  - USB-PD/USBPD_SINK_MSC/USBPD/Inc/usbpd_pdo_defs.h          pdo definition header file
  - USB-PD/USBPD_SINK_MSC/USBPD/Inc/usbpd_pwr_user.h          power management header file
  - USB-PD/USBPD_SINK_MSC/USBPD/Inc/usbpd_vdm_users.h         vendor define message management header file


@par Hardware and Software environment

  - This application runs on STM32G0xx devices.

  - This application has been tested with STMicroelectronics STM32G0C1E-EV board

  - STM32G0C1E-EV Set-up:
    - Plug STM32G0 EVAL USB-C daughter board (MB1352) on top of STM32G0C1E-EV mother board(MB1581)
    - Insert a microSD card into the STM32G0C1E-EV
    - Connect the STM32G0C1E-EV board CN7(on MB1352) to the PC through "TYPE-C" to "Standard A" cable

@note User shall remove the jumpers JP3 and JP4


@par How to use it ?

In order to make the program work, you must do the following :
 - As UCPD Monitor is using a dedicated flash area for storing USPBD port capabilities, please make sure to erase full flash content before starting. (With STM32CubeProgrammer for example)
 - Open your preferred toolchain
 - Rebuild all files and load your image into target memory
 - Run the application
 - Run CubeMx Monitor and open trace tools
 - Connect a type C source device on the Type-C connector 1
 

 */
