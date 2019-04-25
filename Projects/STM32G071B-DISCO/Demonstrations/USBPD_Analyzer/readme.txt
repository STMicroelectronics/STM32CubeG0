/**
  @page USB-C Power Delivery USBPD_Analyzer application
  
  @verbatim
  ******************************************************************************
  * @file    USBPD_Analyzer/readme.txt 
  * @author  MCD Application Team
  * @brief   Description of the USB-C Power Delivery USBPD_Analyzer application.
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
  
@par Application Description

This demonstration firmware is based on STM32Cube and describes how to use 
USB Power Delivery (USB-PD) feature based on STM32G071B-DISCO boards.

At the beginning of the main program the HAL_Init() function is called to reset 
all the peripherals, initialize the Flash interface and the systick.
Then the SystemClock_Config() function is used to configure the system clock
(SYSCLK) to run at 64 MHz.
The UCPD IP is used for USB-PD communication/detection.

The demonstration firmware aims to either emulate a Type C PD3 standalone sink, 
or to spy messages between two type C devices, depending on the door position.

It has been implemented in context of STM32G071xx devices, embedding UCPD IP:

 - When Disco is in spy mode, no action can be executed, but traces of exchanges 
   can be seen using STM32CubeMonitor tool.

 - LCD is used to display current USBPD informations, and user can navigate through menus.
   Check the Discovery user manual for more details on the menus.
 
 - Joystick is used to navigate through the LCD information. 
   Right and left to change the menus, and center to select action. 
   When available you can use select Joy-Up and Joy-Down to scroll trough actions or profile information.

 - Demonstration implementation is based on FreeRTOS.  

@note Care must be taken when using HAL_Delay(), this function provides accurate delay (in milliseconds)
      based on variable incremented in SysTick ISR. This implies that if HAL_Delay() is called from
      a peripheral ISR process, then the SysTick interrupt must have higher priority (numerically lower)
      than the peripheral interrupt. Otherwise the caller ISR process will be blocked.
      To change the SysTick interrupt priority you have to use HAL_NVIC_SetPriority() function.

@note The application needs to ensure that the SysTick time base is always set to 1 millisecond
      to have correct operation.

@par Directory contents

  - USBPD_Analyzer/Src/main.c                      Main program
  - USBPD_Analyzer/Src/system_stm32g0xx.c          STM32G0xx system clock configuration file
  - USBPD_Analyzer/Src/stm32g0xx_it.c              Interrupt handlers
  - USBPD_Analyzer/Src/usbpd_dpm_user.c            DPM layer implementation
  - USBPD_Analyzer/Src/usbpd_pwr_if.c              General power interface configuration
  - USBPD_Analyzer/Src/demo_disco.c                Demonstration code for DISCO
  - USBPD_Analyzer/Src/usbpd_vdm_user.c            VDM User code
  - USBPD_Analyzer/Inc/main.h                      Main program header file
  - USBPD_Analyzer/Inc/stm32g0xx_it.h              Interrupt handlers header file
  - USBPD_Analyzer/Inc/stm32g0xx_hal_conf.h        HAL configuration file
  - USBPD_Analyzer/Inc/usbpd_pwr_if.h              General power interface header file
  - USBPD_Analyzer/Inc/usbpd_dpm_conf.h            USB-C Power Delivery application Configuration file
  - USBPD_Analyzer/Inc/usbpd_dpm_user.h            DPM Layer header file
  - USBPD_Analyzer/Inc/usbpd_pdo_defs.h            PDO definition central header file
  - USBPD_Analyzer/Inc/FreeRTOSConfig.h            FreeRTOS module configuration file
  - USBPD_Analyzer/Inc/usbpd_vdm_user.h            VDM User header file
  - USBPD_Analyzer/Inc/demo_disco.h                Demonstration header file for DISCO
  - USBPD_Analyzer/Inc/tracer_emb_conf.h           Embedded Trace configuration file
  - USBPD_Analyzer/Inc/usbpd_gui_memmap.h          STM32CubeMonitor USB-PD configuration file
  - USBPD_Analyzer/Inc/usbpd_devices_conf.h        Devices configuration file


@par Hardware and Software environment

  - This demonstration runs only on STM32G071B-DISCO board(MB1378 rev C)

  - This application runs on STM32G071 devices.

  - This application can be used together with STM32CubeMonitor (Spy mode).

  - To select Standalone mode, mechanical door shall be closed.
  
  - To select Spy mode, mechanical door shall be opened.
  
@par How to use it ?
We recommend to have always the micro USB cable plugged in to power the G0 discovery kit.

In order to make the program work, you must do the following:
  - Open your preferred toolchain 
  - Rebuild all files and load your image into target memory
  - Run the application
  
Below is the LED meaning :  
  - Reference   Color   Name          Function 
  - LD1         Green   POWER 5V      5V present onto the board 
  - LD4         Orange  SINK mode     Attached to a power source 
  - LD5         Orange  SOURCE mode   Attached to a device as source 
  - LD6         Green   SPY mode      SPY mode active 

For example : the mode selection can be confirmed by the LED 6. If LD6 is on then SPY Mode, else Sink Mode.
In Sink Mode, the LD 4 is ON when the USB-PD is attached to a power source.

 
 * <h3><center>&copy; COPYRIGHT STMicroelectronics</center></h3>
 */
 