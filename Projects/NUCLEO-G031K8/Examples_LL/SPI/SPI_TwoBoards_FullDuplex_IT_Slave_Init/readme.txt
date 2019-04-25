/**
  @page SPI_TwoBoards_FullDuplex_IT_Slave_Init SPI example
  
  @verbatim
  ******************************************************************************
  * @file    Examples_LL/SPI/SPI_TwoBoards_FullDuplex_IT_Slave_Init/readme.txt 
  * @author  MCD Application Team
  * @brief   Description of the SPI_TwoBoards_FullDuplex_IT_Slave_Init example.
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

Data buffer transmission and receptionvia SPI using Interrupt mode. This 
example is based on the STM32G0xx SPI LL API. The peripheral 
initialization uses LL unitary service functions for optimization purposes (performance and size).

The communication is done with 2 boards through SPI.
   _________________________                        _________________________
  |       ___________ ______|                      |__________________       |
  |      |SPI1              |                      |             SPI1 |      |
  |      |                  |                      |                  |      |
  |      |         CLK(PB3) |______________________|(PB3)CLK          |      |
  |      |                  |                      |                  |      |
  |      |         MISO(PB4)|______________________|(PB4)MISO         |      |
  |      |                  |                      |                  |      |
  |      |         MOSI(PB5)|______________________|(PB5)MOSI         |      |
  |      |                  |                      |                  |      |
  |      |__________________|                      |__________________|      |
  |      __                 |                      |                         |
  |     |__|                |                      |                         |
  |     USER                |                      |                         |
  |                      GND|______________________|GND                      |
  |                         |                      |                         |
  |_STM32G0xx Master _______|                      |_STM32G0xx Slave ________|

This example shows how to configure GPIO and SPI peripherals
to use a Full-Duplex communication using IT mode through the STM32G0xx COM_INSTANCE1_TYPE LL API.

This example is splitted in two projects, Master board and Slave board:

- Master Board
  SPI1 Peripheral is configured in Master mode.
  SPI1_IRQn activated. 
  RXNE and TXE Interrupts SPI peripheral activated.
  GPIO associated to PA.15 (Arduino D2) is linked with EXTI. 
  
- Slave Board
  SPI1 Peripheral is configured in Slave mode.
  SPI1_IRQn activated. 
  RXNE and TXE Interrupts SPI peripheral activated.


Example execution:
On BOARD MASTER, LED3 is blinking Fast (200ms) and wait user connects the PA.15 (Arduino D2) to GND.
Connect PA.15 (Arduino D2) to GND on BOARD to start a Full-Duplex communication through IT.
On MASTER side, Clock will be generated on SCK line, Transmission(MOSI Line) and reception (MISO Line) 
will be done at the same time. 
SLAVE SPI will received  the Clock (SCK Line), so Transmission(MISO Line) and reception (MOSI Line) will be done also.

LED3 is On on both boards if data is well received.

In case of errors, LED3 is blinking Slowly (1s).

@note You need to perform a reset on Master board, then perform it on Slave board
      to have the correct behaviour of this example.


@par Directory contents 

  - SPI/SPI_TwoBoards_FullDuplex_IT_Slave_Init/Inc/stm32g0xx_it.h          Interrupt handlers header file
  - SPI/SPI_TwoBoards_FullDuplex_IT_Slave_Init/Inc/main.h                  Header for main.c module
  - SPI/SPI_TwoBoards_FullDuplex_IT_Slave_Init/Inc/stm32_assert.h          Template file to include assert_failed function
  - SPI/SPI_TwoBoards_FullDuplex_IT_Slave_Init/Src/stm32g0xx_it.c          Interrupt handlers
  - SPI/SPI_TwoBoards_FullDuplex_IT_Slave_Init/Src/main.c                  Main program
  - SPI/SPI_TwoBoards_FullDuplex_IT_Slave_Init/Src/system_stm32g0xx.c      STM32G0xx system source file

@par Hardware and Software environment

  - This example runs on NUCLEO-G031K8 devices.

  - This example has been tested with NUCLEO-G031K8 board and can be
    easily tailored to any other supported device and development board.

  - NUCLEO-G031K8 Set-up
    - Connect Master board PB3 to Slave Board PB3 (connected to pin 15 of CN4 connector)
    - Connect Master board PB4 to Slave Board PB4 (connected to pin 15 of CN3 connector)
    - Connect Master board PB5 to Slave Board PB5 (connected to pin 14 of CN3 connector)
    - Connect Master board GND to Slave Board GND

@par How to use it ? 

In order to make the program work, you must do the following :
 - Open your preferred toolchain
 - Rebuild all files and load your image into target memory
    o Load the Master project in Master Board
    o Load the Slave project in Slave Board
 - Run the example

 * <h3><center>&copy; COPYRIGHT STMicroelectronics</center></h3>
 */
