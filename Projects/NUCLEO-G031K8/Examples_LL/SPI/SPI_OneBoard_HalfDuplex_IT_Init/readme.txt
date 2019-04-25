/**
  @page SPI_OneBoard_HalfDuplex_IT_Init SPI example
  
  @verbatim
  ******************************************************************************
  * @file    Examples_LL/SPI/SPI_OneBoard_HalfDuplex_IT_Init/readme.txt 
  * @author  MCD Application Team
  * @brief   Description of the SPI_OneBoard_HalfDuplex_IT_Init example.
  ******************************************************************************
  *
  * Copyright (c) 2019 STMicroelectronics. All rights reserved.
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                       opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  @endverbatim

@par Example Description

Configuration of GPIO and SPI peripherals to transmit bytes 
from an SPI Master device to an SPI Slave device in Interrupt mode. This example
is based on the STM32G0xx SPI LL API. The peripheral initialization uses 
LL unitary service functions for optimization purposes (performance and size).

This example works with only one NUCLEO-G031K8.

SPI1 Peripheral is configured in Master mode Half-Duplex Tx.
SPI2 Peripheral is configured in Slave mode Half-Duplex Rx.
GPIO associated to User push-button is linked with EXTI. 

Example execution:

LED3 is blinking Fast (200ms) and wait user connects the PA.15 (Arduino D2) to GND.
Connect PA.15 (Arduino D2) to GND on BOARD start a Half-Duplex communication through IT.
On MASTER side (SPI1), Clock will be generated on SCK line, Transmission done on MOSI Line.
On SLAVE side (SPI2) reception is done through the MISO Line.

LED3 is On if data is well received.

In case of errors, LED3 is blinking Slowly (1s).

@par Directory contents 

  - SPI/SPI_OneBoard_HalfDuplex_IT_Init/Inc/stm32g0xx_it.h          Interrupt handlers header file
  - SPI/SPI_OneBoard_HalfDuplex_IT_Init/Inc/main.h                  Header for main.c module
  - SPI/SPI_OneBoard_HalfDuplex_IT_Init/Inc/stm32_assert.h          Template file to include assert_failed function
  - SPI/SPI_OneBoard_HalfDuplex_IT_Init/Src/stm32g0xx_it.c          Interrupt handlers
  - SPI/SPI_OneBoard_HalfDuplex_IT_Init/Src/main.c                  Main program
  - SPI/SPI_OneBoard_HalfDuplex_IT_Init/Src/system_stm32g0xx.c      STM32G0xx system source file

@par Hardware and Software environment

  - This example runs on NUCLEO-G031K8 devices.

  - This example has been tested with NUCLEO-G031K8 board and can be
    easily tailored to any other supported device and development board.

  - NUCLEO-G031K8 Set-up
    - Connect Master SCK  PB3 to Slave SCK  PB8
    - Connect Master MOSI PB5 to Slave MISO PB2

Relation with Board connector:
  PB3  is connected to pin 15 of CN4 connector
  PB5  is connected to pin 14 of CN3 connector
  PB8 is connected to pin 11 of CN3 connector
  PB2 is connected to pin 10 of CN3 connector

@par How to use it ? 

In order to make the program work, you must do the following :
 - Open your preferred toolchain
 - Rebuild all files and load your image into target memory
 - Run the example

 * <h3><center>&copy; COPYRIGHT STMicroelectronics</center></h3>
 */
