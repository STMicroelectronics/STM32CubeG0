/**
  @page I2C_OneBoard_Communication_PollingAndIT_Init I2C example (Master Polling Mode)

  @verbatim
  ******************************************************************************
  * @file    Examples_LL/I2C/I2C_OneBoard_Communication_PollingAndIT_Init/readme.txt
  * @author  MCD Application Team
  * @brief   Description of the I2C_OneBoard_Communication_PollingAndIT_Init I2C example (Master Polling Mode).
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

How to transmit data bytes from an I2C master device using polling mode
to an I2C slave device using interrupt mode. The peripheral is initialized
with LL unitary service functions to optimize for performance and size.

This example guides you through the different configuration steps by mean of LL API
to configure GPIO and I2C peripherals using only one NUCLEO-G031K8.

I2C1 Peripheral is configured in Slave mode with EXTI (Clock 400Khz, Own address 7-bit enabled).
I2C2 Peripheral is configured in Master mode (Clock 400Khz).
GPIO associated to User push-button is linked with EXTI.

LED3 blinks quickly to wait PA.15 (Arduino D2) action (put then remove a jumper between PA.15 (Arduino D2) and GND).

Example execution:
Put and remove a jumper between PA.15 (Arduino D2) and GND to initiate a write request by Master through Handle_I2C_Master() routine.
This action will generate an I2C start condition with the Slave address and a write bit condition.
When address Slave match code is received on I2C1, an ADDR interrupt occurs.
I2C1 IRQ Handler routine is then checking Address Match Code and direction Write.
This will allow Slave to enter in receiver mode and then acknowledge Master to send the bytes.
When acknowledge is received on I2C2, a TXIS event occurs.
This will allow Master to transmit a byte to the Slave.
Each time a byte is received on I2C1 (Slave), an RXNE interrupt occurs until a STOP condition.
And so each time the Slave acknowledge the byte received a TXIS event occurs on Master side.
Master auto-generate a Stop condition when size of data to transmit is achieved.

The STOP condition generate a STOP interrupt and initiate the end of reception on Slave side.
I2C1 IRQ handler and Handle_I2C_Master() routine are then clearing the STOP flag in both side.

LED3 is On if data are well received.

In case of errors, LED3 is blinking slowly (1s).


@par Directory contents

  - I2C/I2C_OneBoard_Communication_PollingAndIT_Init/Inc/stm32g0xx_it.h          Interrupt handlers header file
  - I2C/I2C_OneBoard_Communication_PollingAndIT_Init/Inc/main.h                  Header for main.c module
  - I2C/I2C_OneBoard_Communication_PollingAndIT_Init/Inc/stm32_assert.h          Template file to include assert_failed function
  - I2C/I2C_OneBoard_Communication_PollingAndIT_Init/Src/stm32g0xx_it.c          Interrupt handlers
  - I2C/I2C_OneBoard_Communication_PollingAndIT_Init/Src/main.c                  Main program
  - I2C/I2C_OneBoard_Communication_PollingAndIT_Init/Src/system_stm32g0xx.c      STM32G0xx system source file

@par Hardware and Software environment

  - This example runs on STM32G031K8Tx devices.

  - This example has been tested with NUCLEO-G031K8 board and can be
    easily tailored to any other supported device and development board.

  - NUCLEO-G031K8 Set-up
    - Connect GPIOs connected to I2C1 SCL/SDA (PB.8 and PB.9)
    to respectively SCL and SDA pins of I2C2 (PA.11 and PA.12).
      - I2C1_SCL  PB.8 (CN3, pin 11 (D8)) : connected to I2C2_SCL PA.11 (CN4, pin 7 (A5))
      - I2C1_SDA  PB.9 (CN3, pin 13 (D10)) : connected to I2C2_SDA PA.12 (CN4, pin 8 (A4))

  - Launch the program.
  - Put and remove a jumper between PA.15 (Arduino D2) and GND to initiate a write request by Master
      then Slave receive bytes.

@par How to use it ?

In order to make the program work, you must do the following :
 - Open your preferred toolchain
 - Rebuild all files and load your image into target memory
 - Run the example

 * <h3><center>&copy; COPYRIGHT STMicroelectronics</center></h3>
 */
