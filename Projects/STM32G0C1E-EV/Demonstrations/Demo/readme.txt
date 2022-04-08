/**
  @page Demo   Demo STM32G0C1E-EV
 
  @verbatim
  ******************************************************************************
  * @file    readme.txt 
  * @author  MCD Application Team
  * @brief   Description of STM32G0C1E-EV Demo
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

@par Demo Description

This demonstration firmware provides a sub-set of applications supported by the STM32G0C1E-EV 
board(MB1581) equipped with either Legacy daughter board(MB1351) or USB-PD board(MB1352).

This demonstration firmware is based on STM32Cube and describes how to use 
USB Power Delivery (USB-PD) feature based on STM32G0C1E-EV (MB1581) + MB1352 extension board 
and helps you also to discover most of peripherals available on the STM32G0C1E device,
using STM32G0C1E-EV (MB1581) + MB1351 extension board.

At the beginning of the main program the HAL_Init() function is called to reset
all the peripherals, initialize the Flash interface and the systick.
Then the SystemClock_Config() function is used to configure the system clock
(SYSCLK) to run at 64 MHz.

Then the firmware will identify the daughter board connected on the daughter board connectors, thanks to 
voltage level measured at PB11 (ADC_IN15).
Depending on the returned daughter board Id, the firmware will either launch UCPD sub-demo menu 
or Legacy sub-demo menu.

The legacy part of the demonstration firmware provides a sub-set of applications
supported by the STM32G0C1E-EV board equipped with Legacy daughter board(MB1351).

Below you will find the different sub-set of "Legacy" applications available in this demonstration 
and supported by the STM32G0C1E-EV board when Legacy daughter board(MB1351) is plugged:

 - calendar : application to set date, time and alarm on a calendar
 - image viewer : displays the pictures present on the SD card (\USER).
 - audio player/recorder : application to play & record wave files.
   warning: Jumper on JP25 must be plugged for better audio quality.
 - thermometer & LDR : measure and display the ambiant temperature & Light level.
 - low power mode : allows to enter in low power mode(stop and standby)
                    and the wakeup can be triggered by an EXTI or an alarm.
 - file browser : application to browse on the SD card.
 - Help : indicates date & version of demonstration FW
 - About : describes HW configuration( jumper, etc..) to run demonstration FW

The USB-PD part of the demonstration firmware aims to describe how USB-PD version PD3.0, is
implemented in context of STM32G0xx devices, embedding UCPD IP :

 - Port 1 is Dual Role Power capable (DRP, i.e. meaning it can establish 
   a PD contract either in Provider/Source(SRC) or Consumer/Sink(SNK) mode).
 - Port 2 is only SNK capable and could only acts as a Sink/Consumer.
 - LCD is used to display current USB-PD state of each port.
 - Tamper button is used to trigger USB-PD action/request according to port capabilities
   (as Power Role Swap request on Port 1 for example)
 - Demonstration implementation is based on FreeRTOS.  

When no USB Type-C cables are connected, Port capabilities are displayed on LCD
(i.e. DRP for Port 1, and SNK for Port 2).
When a USB Type-C cable is connected to a port, and a Explicit Contract is established 
(agreement reached between a two ports as a result of the Power Delivery negotiation),
information displayed on LCD indicates the Port Role within the contract, i.e. SRC (Provider/Source)
or SNK (Consumer/Sink)

The user can connect to Port 1 receptacle an application which plays the Consumer 
role as well as the Provider role.
The user can connect to Port 2 receptacle an application which plays the Provider role.

When a Provider Only is connected to either Port 1 or Port 2:
 - The STM32 MCU behaves as a Consumer (Sink mode),it waits for Power Capabilities message 
   from the attached Provider. When a Source Capabilities message is received, the STM32 
   starts the evaluation of the received capabilities and check if one of the received power 
   objects can meet its power requirement.
   The STM32 shall send the Request message to request the new power level from the offered 
   Source Capabilities.
   Once the Explicit Contract is achieved (PS_Ready message received), information that Port 
   is then acting as Consumer (SNK) is displayed on LCD. 
   The CC line (Configuration Channel, CC1 or CC2) is also indicated.
When a Consumer Only is connected to Port 1:
 - The STM32 MCU behaves as a Provider (Source mode), it exchanges Power profiles with 
   the connected device and waits for Power Request message from the attached Consumer.
   If the requested power can be met, the communication will end with an Accept message followed 
   by a PS_RDY message.
 - Once a Explicit Contract is established, information that Port 1 is then acting as Provider (SRC)
   is displayed on LCD. The CC line (Configuration Channel, CC1 or CC2) is also indicated.
When a Consumer Only is connected to Port 2:
 - As Port 2 could only behaves as a Consumer (Sink mode), no Explicit Contract could be established.
When a DRP capable device is connected to Port 1:
 - As Port1 is able to act as a Source or Sink (DRP), when a DRP capable device is connected to Port 1, 
   an explicit contract could be established either with Port 1 acting as a Provider (Source mode), 
   or as a Consumer (SNK). Similar procedures than described above then take place.
   Once a Explicit Contract is established, information of Port 1 Role is displayed on LCD. 
   The CC line (Configuration Channel, CC1 or CC2) is also indicated.
When a DRP capable device is connected to Port 2:
 - As Port 2 could only behaves as a Consumer (Sink mode), connected device will have to behave
   as a Source in order to allow Explicit contract establishment.

The Power Role Swap feature is supported on Port 1 (as requiring Dual Role Power support):
 - When connected to an USB-C provider only device (source mode), the power role swap between the two boards is not possible,
   DRP board will act automatically as a sink.
 - When connected to an USB-C consumer only device (sink mode), the power role swap between the two boards is not possible,
   DRP board will act automatically as a source.
 - When connected to an USB-C with DRP, the power role swap could be triggered by
   pressing tamper button.

@note Care must be taken when using HAL_Delay(), this function provides accurate
      delay (in milliseconds) based on variable incremented in SysTick ISR. 
      This implies that if HAL_Delay() is called from a peripheral ISR process, 
      then the SysTick interrupt must have higher priority (numerically lower)
      than the peripheral interrupt. Otherwise the caller ISR process will be blocked.
      To change the SysTick interrupt priority you have to use HAL_NVIC_SetPriority() function.

@note The application needs to ensure that the SysTick time base is always set to
      1 millisecond to have correct HAL operation.
      
@par Keywords

Demonstrations, low power, calendar, FatFs, SD Card, File system, UCPD

@par Hardware and Software environment

UCPD sub-demo:
  - This demonstration runs only on STM32G0C1E-EV board(MB1581 rev A) with USB PD daughter board(MB1352 rev C) 

  - MB1352 shall always be powered using DC IN (19V) on CN3.

  - MB1352 (rev C) configuration (PWM Mode):
    front side:
        JP1=ENABLE, JP2=ENABLE
        JP3(VBUS_SEL)=+5V
        JP5=PORT1(1.2)
    back side:
        Set SB as explained by "PWM Mode" marking
        SB13=SB14=SB15=OFF
        SB2=SB3=SB23=SB26=ON
  - MB1581 (rev A) configuration for USB PD :
        JP17=D5V

Legacy Sub-demo: 

  - This demonstration runs on STM32G0C1E-EV mother board (MB1581 rev A)
    with Legacy daughter board (MB1351 rev A).

  - A SDSC microSD card (capacity up to 4GB), must contain all the resources files
    available under the FW package Binary/SD_card folder.

  - For a better user experience with audio player/recorder application,
    jumper on JP25 must be plugged.

@par How to use it ? 

In order to make the program work, you must do the following :
 - As UCPD Monitor is using a dedicated flash area for storing USPBD port capabilities, please make sure to erase full flash content before starting. (With STM32CubeProgrammer for example)
 - Open your preferred toolchain 
 - Rebuild all files and load your image into target memory
 - Run the example


 */
