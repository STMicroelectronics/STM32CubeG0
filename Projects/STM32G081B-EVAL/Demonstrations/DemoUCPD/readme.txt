/**
  @page Demo   Demo UCPD STM32G081B-EVAL
 
  @verbatim
  ******************************************************************************
  * @file    readme.txt 
  * @author  MCD Application Team
  * @brief   Description of STM32G081B-EVAL USB-C Power Delivery Demonstration
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

This demonstration firmware is based on STM32Cube and describes how to use 
USB Power Delivery (USB-PD) feature based on STM32G081B-EVAL + MB1352 extension boards.

At the beginning of the main program the HAL_Init() function is called to reset 
all the peripherals, initialize the Flash interface and the systick.
Then the SystemClock_Config() function is used to configure the system clock
(SYSCLK) to run at 64 MHz.
The UCPD IP is used for USB-PD communication/detection.
 
The demonstration firmware aims to describe how USB-PD version PD3.0, has been
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

@par Hardware and Software environment

  - This demonstration runs only on STM32G081B-EVAL board(MB1350 rev B) with USB PD daughter board(MB1352) rev C.

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
  - MB1350 (rev C) configuration for USB PD :
        JP17=D5V
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
