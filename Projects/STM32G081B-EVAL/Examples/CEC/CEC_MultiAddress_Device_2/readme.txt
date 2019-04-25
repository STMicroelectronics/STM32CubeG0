/**
  @page CEC CEC_MultiAddress_Device_2 example
  
  @verbatim
  ******************************************************************************
  * @file    CEC/CEC_MultiAddress_Device_2/readme.txt 
  * @author  MCD Application Team
  * @brief   Description of the CEC Multiple Address communication example.
  ******************************************************************************
  *
  * Copyright (c) 2018 STMicroelectronics. All rights reserved.
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                       opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  @endverbatim

@par Example Description 

This example shows how to configure and use the CEC peripheral to receive and 
transmit messages in the case where one device supports two distinct logical
addresses at the same time.


- Hardware Description

To use this example, two STM32G081B-EVAL boards (called Device_1 and 
Device_2) are loaded with the matching software then connected through CEC lines
PB10 and GND.


@verbatim
Development board = STM32G081B-EVAL 
*------------------------------------------------------------------------------*
|         Device Address :0x01                    Device Address :0x03         |
|                                                 Device Address :0x05         |
|         ____________________                   ____________________          |
|        |                    |                 |                    |         |
|        |                    |                 |                    |         | 
|        |     __________     |                 |     __________     |         |
|        |    |   CEC    |____|____CECLine______|____|   CEC    |    |         |
|        |    | Device_1 |    |                 |    | Device_2 |    |         |
|        |    |__________|    |                 |    |__________|    |         |
|        |                    |                 |                    |         |
|        |  O LD1             |                 |  O LD1             |         |
|        |  O LD2    Joystick |                 |  O LD2    Joystick |         |
|        |  O LD3        _    |                 |  O LD3        _    |         |
|        |  O LD4       |_|   |                 |  O LD4       |_|   |         |
|        |                    |                 |                    |         |
|        |             GND O--|-----------------|--O GND             |         |
|        |____________________|                 |____________________|         |
|                                                                              |
|                                                                              |
*------------------------------------------------------------------------------*
@endverbatim


- Software Description

The test unrolls as follows.

On Device_1 TX side, three possible messages can be transmitted and are 
indicated as below on the transmitting board:
 - when Tamper push-button is pressed, a ping message (no payload) is addressed
  to Device_2 first logical address; LED1 toggles
 - when Joystick DOWN push-button is pressed, a ping message (no payload) is addressed
  to Device_2 second logical address; LED4 toggles  
 - when Joystick Selection push-button is pressed, a broadcast message (with empty
 payload) is sent by Device_1; LED2 toggles
 
When Joystick UP push-button is pressed, LED3 toggles (no message sent)  


Accordingly, the following happens on Device_2 RX side in case of successful
reception:
 - when Tamper push-button is pressed on TX side, 
     * LED1 is turned on
     * LED3 and LED4 are turned off 
 - when Joystick DOWN push-button is pressed on TX side, 
     * LED4 is turned on
     * LED1 and LED3 are turned off      
 - when Joystick Selection push-button is pressed on TX side, all LEDs are turned off 


Conversely, on Device_2 TX side, only ping messages addressed to the 
single Device_1 logical address or broadcast messages are sent:
 - when Tamper push-button is pressed, a ping message (no payload) is addressed
  to Device_1; LED1 toggles
 - when Joystick DOWN push-button is pressed, a ping message (no payload) is addressed
  to Device_1; LED4 toggles  
 - when Joystick Selection push-button is pressed, a broadcast message (with empty
 payload) is sent; LED2 toggles


Accordingly, the following happens on Device_1 RX side in case of successful
transmission:
 - when Tamper or Joystick DOWN push-button are pressed on TX side, 
      * LED1 and LED4 are turned on
      * LED3 is turned off     
 - when Joystick Selection push-button is pressed on TX side, all LEDs are turned off 


For Device_1 and _2, in case of transmission or reception error, 
LED3 is turned on.


Practically, 2 EXTI lines (EXTI_Line0_1 and EXTI_Line2_3) are configured to 
generate an interrupt on each falling or rising edge. 
A specific message is then transmitted by the CEC IP
and a LED connected to a specific GPIO pin is toggled.
    - EXTI_Line0_1 is mapped to PA.00
    - EXTI_Line2_3 is mapped to PC.13, PC.02 and PC.03 

Then, on TX side,
  - when rising edge is detected on EXTI_Line0_1, LED2 toggles
  - when falling edge is detected on EXTI_Line4_15 and EXTI line interrupt is detected
    on PC.13, LED1 toggles
  - when falling edge is detected on EXTI_Line2_3 and EXTI line interrupt is detected
    on PC.02, LED3 toggles
  - when falling edge is detected on EXTI_Line2_3 and EXTI line interrupt is detected
    on PC.03, LED4 toggles

In this example, HCLK is configured at 56 MHz.

@par Keywords

Connectivity, CEC, Transmission, Reception, joystick, Data exchange

@par Directory contents 

  - CEC/CEC_MultiAddress_Device_2/Inc/stm32g0xx_hal_conf.h    HAL configuration file
  - CEC/CEC_MultiAddress_Device_2/Inc/stm32g0xx_it.h          Interrupt handlers header file
  - CEC/CEC_MultiAddress_Device_2/Inc/main.h                  Header for main.c module  
  - CEC/CEC_MultiAddress_Device_2/Src/stm32g0xx_it.c          Interrupt handlers
  - CEC/CEC_MultiAddress_Device_2/Src/system_stm32g0xx.c      STM32G0xx system source file
  - CEC/CEC_MultiAddress_Device_2/Src/main.c                  Main program
  - CEC/CEC_MultiAddress_Device_2/Src/stm32g0xx_hal_msp.c     IP hardware resources initialization
  
@par Hardware and Software environment

  - This example runs on STM32G081RBTx Devices.
    
  - This example has been tested with STM32G081B-EVAL board and can be
    easily tailored to any other supported device and development board.      


@par How to use it ? 

In order to make the program work, you must do the following :
 - Open your preferred toolchain 
 - Rebuild all files (CEC_DataExchange_Device_1 project) and load your image into target memory
    o Load the project in Device_1 Board
 - Rebuild all files (CEC_DataExchange_Device_2 project) and load your image into target memory
    o Load the project in Device_2 Board
 - Be aware that PB10 pin is missing but PB10 is connected directly to it (SB23 is closed).
 - With a wire, connect PB10-PB10 between the 2 boards
 - Add a pull-up resistor of 27kohm between PB10 and V3.3
 - Connect the ground of the 2 boards
 - Run the example

 * <h3><center>&copy; COPYRIGHT STMicroelectronics</center></h3>
 */
