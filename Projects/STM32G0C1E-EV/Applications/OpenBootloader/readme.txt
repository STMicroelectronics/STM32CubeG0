/**
  @page OpenBootloader Open Bootloader IAP application
 
  @verbatim
  ******************************************************************************
  * @file    OpenBootloader/readme.txt
  * @author  MCD Application Team
  * @brief   Description of the consumer example.
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

This application exploits OpenBootloader Middleware to demonstrate how to develop an IAP application
and how use it.

At the beginning of the main program the HAL_Init() function is called to reset 
all the peripherals, initialize the Flash interface and the systick.
Then the SystemClock_Config() function is used to configure the system
clock (SYSCLK) to run at 64 MHz.

Then, these protocols and memories interfaces are configured
  - USART1
  - FDCAN1
  - I2C1
  - USB
  - FLASH interface
  - RAM interface
  - Option bytes interface
  - Engi bytes interface
  - OTP interface
  - System memory interface

Then, the OpenBootloader application will wait for incoming communication on one of the supported protocols.

Once a communication is established through one of the supported protocols,
the OpenBootloader application will wait for commands sent by the host.

@note In case of Mass Erase operation, the OpenBootloader FLASH area must be protected otherwise the OpenBootloader
      will be erased.

@par Keywords

OpenBootloader, USART, FDCAN, I2C, USB

@par Directory contents
     - Core/Src/main.c                                    Main program file
     - Core/Src/stm32g0xx_hal_msp.c                       MSP Initialization file
     - Core/Src/stm32g0xx_it.c                            Interrupt handlers file
     - Core/Src/system_stm32g0xx.c                        STM32G0xx system clock configuration file
     - Core/Inc/main.h                                    Main program header file
     - Core/Inc/stm32g0xx_hal_conf.h                      HAL Library Configuration file
     - Core/Inc/stm32g0xx_it.h                            Interrupt handlers header file
     - OpenBootloader/App/app_openbootloader.c            OpenBootloader application entry point
     - OpenBootloader/App/app_openbootloader.h            Header for OpenBootloader application entry file
     - OpenBootloader/Target/common_interface.c           Contains common functions used by different interfaces
     - OpenBootloader/Target/common_interface.h           Header for common functions file
     - OpenBootloader/Target/engibytes_interface.c        Contains ENGI Bytes interface
     - OpenBootloader/Target/engibytes_interface.h        Header of ENGI Bytes interface file
     - OpenBootloader/Target/fdcan_interface.c            Contains FDCAN interface
     - OpenBootloader/Target/fdcan_interface.h            Header of FDCAN interface file
     - OpenBootloader/Target/flash_interface.c            Contains FLASH interface
     - OpenBootloader/Target/flash_interface.h            Header of FLASH interface file
     - OpenBootloader/Target/i2c_interface.c              Contains I2C interface
     - OpenBootloader/Target/i2c_interface.h              Header of I2C interface file
     - OpenBootloader/Target/interfaces_conf.h            Contains Interfaces configuration
     - OpenBootloader/Target/iwdg_interface.c             Contains IWDG interface
     - OpenBootloader/Target/iwdg_interface.h             Header of IWDG interface file
     - OpenBootloader/Target/openbootloader_conf.h        Header file that contains OpenBootloader HW dependent configuration
     - OpenBootloader/Target/optionbytes_interface.c      Contains OptionBytes interface
     - OpenBootloader/Target/optionbytes_interface.h      Header of OptionBytes interface file
     - OpenBootloader/Target/otp_interface.c              Contains OTP interface
     - OpenBootloader/Target/otp_interface.h              Header of OTP interface file
     - OpenBootloader/Target/platform.h	                  Header for patterns
     - OpenBootloader/Target/ram_interface.c              Contains RAM interface
     - OpenBootloader/Target/ram_interface.h              Header of RAM interface file
     - OpenBootloader/Target/systemmemory_interface.c     Contains system memory interface
     - OpenBootloader/Target/systemmemory_interface.h     Header of system memory interface file
     - OpenBootloader/Target/usart_interface.c            Contains USART interface
     - OpenBootloader/Target/usart_interface.h            Header of USART interface file
     - OpenBootloader/Target/usb_interface.c              Contains USB interface
     - OpenBootloader/Target/usb_interface.h              Header of USB interface file
     - USB_Device/App/usb_device.c                        USB device application code
     - USB_Device/App/usb_device.h                        USB device application header file
     - USB_Device/App/usbd_desc.c                         USB device descriptor
     - USB_Device/App/usbd_desc.h                         USB device descriptor header file
     - USB_Device/App/usbd_dfu_if.c                       USB device functions interface
     - USB_Device/App/usbd_dfu_if.h                       USB device functions interface header file
     - USB_Device/Target/usbd_conf.c                      USB device driver Configuration file
     - USB_Device/Target/usbd_conf.h                      USB device driver Configuration header file

@par Hardware and Software environment

  - This example runs on STM32G0C1VExx devices.

  - This example has been tested with STM32G0C1E-EV board and can be
    easily tailored to any other supported device and development board.

  - STM32G0C1E-EV set-up to use FDCAN1:
    - Jumper JP9 => fitted
    - Connect the 2 points JP8 CAN connector (CAN-H and CAN-L) of FDCAN1
      instance to your FDCAN Host adapter

  - STM32G0C1E-EV set-up to use USART:
    - To use the USART1 for communication you have to connect:
      - Tx pin of your host adapter to PA10 pin of the STM32G0C1E-EV board
      - Rx pin of your host adapter to PA9 pin of the STM32G0C1E-EV board

  - STM32G0C1E-EV set-up to use I2C:
    - Close pin17 &18 of CN2 by jumpers
    - Close pin17 &18 of CN3 by jumpers
    - Set I2C address to 0x5D
    - To use the I2C1 for communication you have to connect:
      - SCL pin of your host adapter to PB6 pin of the STM32G0C1E-EV board
      - SDA pin of your host adapter to PB7 pin of the STM32G0C1E-EV board

  - STM32G0C1E-EV set-up to use USB:
    - Install the DFU driver available in "DfuSe Demonstrator" installation directory
      For Windows 8.1 and later : Update STM32 DFU device driver manually from Windows Device Manager.
      The install of required device driver is available under:
      "Program Files\STMicroelectronics\Software\DfuSe v3.0.5\Bin\Driver\Win8.1" directory.
    - It is also possible to use the ST host STM32Cuebeprogrammer: select USB then connect 
    - To use the USB for communication you have to connect:
      - The daughter board to the STM32G0C1E-EV board
      - CN7 connector to the PC through "TYPE-C" to "Standard A" cable.

@par How to use it ?

In order to make the program work, you must do the following:
 - Open your preferred toolchain
 - Rebuild all files and load your image into target memory
 - Run the application
 - Run STM32CubeProgrammer and connect to OpenBootloader using USART1
    Or
 - Connect your FDCAN host adapter and connect to OpenBootloader using FDCAN1
    Or
 - Connect your I2C host adapter and connect to OpenBootloader using I2C1
    Or
 - Connect your USB host adapter and connect to OpenBootloader using USB


 */
