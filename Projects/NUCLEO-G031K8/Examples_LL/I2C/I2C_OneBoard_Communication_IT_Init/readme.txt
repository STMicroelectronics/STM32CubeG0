/**
  @page I2C_OneBoard_Communication_IT_Init I2C example (IT Mode)

  @verbatim
  ******************************************************************************
  * @file    Examples_LL/I2C/I2C_OneBoard_Communication_IT_Init/readme.txt
  * @author  MCD Application Team
  * @brief   Description of the I2C_OneBoard_Communication_IT_Init I2C example (IT Mode).
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

How to handle the reception of one data byte from an I2C slave device
by an I2C master device. Both devices operate in interrupt mode. The peripheral is initialized
with LL initialization function to demonstrate LL init usage.

This example guides you through the different configuration steps by mean of LL API
to configure GPIO and I2C peripherals using only one NUCLEO-G031K8.

The user can disable internal pull-up through "#define EXTERNAL_PULL_UP_AVAILABLE"
This help for an integration of this example inside an ecosystem board with external pull-up */

I2C1 Peripheral is configured in Slave mode with EXTI (Clock 400Khz, Own address 7-bit enabled).
I2C2 Peripheral is configured in Master mode with EXTI (Clock 400Khz).
GPIO associated to User push-button is linked with EXTI.

LED3 blinks quickly to wait PA.15 (Arduino D2) action (put then remove a jumper between PA.15 (Arduino D2) and GND).

Example execution:
Put and remove a jumper between PA.15 (Arduino D2) and GND to initiate a read request by Master.
This action will generate an I2C start condition with the Slave address and a read bit condition.
When address Slave match code is received on I2C1, an ADDR interrupt occurs.
I2C1 IRQ Handler routine is then checking Address Match Code and direction Read.
This will allow Slave to enter in transmitter mode and then send a byte when TXIS interrupt occurs.
When byte is received on I2C2, an RXNE interrupt occurs.
When RXDR register is read, Master auto-generate a NACK and STOP condition
to inform the Slave that the transfer is finished.
The NACK condition generate a NACK interrupt in Slave side treated in the I2C1 IRQ handler routine by a clear of NACK flag.
The STOP condition generate a STOP interrupt in both side (Slave and Master). I2C1 and I2C2 IRQ handler routine are then
clearing the STOP flag in both side.

LED3 is On if data is well received.

In case of errors, LED3 is blinking slowly (1s).


@par Directory contents

  - I2C/I2C_OneBoard_Communication_IT_Init/Inc/stm32g0xx_it.h          Interrupt handlers header file
  - I2C/I2C_OneBoard_Communication_IT_Init/Inc/main.h                  Header for main.c module
  - I2C/I2C_OneBoard_Communication_IT_Init/Inc/stm32_assert.h          Template file to include assert_failed function
  - I2C/I2C_OneBoard_Communication_IT_Init/Src/stm32g0xx_it.c          Interrupt handlers
  - I2C/I2C_OneBoard_Communication_IT_Init/Src/main.c                  Main program
  - I2C/I2C_OneBoard_Communication_IT_Init/Src/system_stm32g0xx.c      STM32G0xx system source file

@par Hardware and Software environment

  - This example runs on STM32G031K8Tx devices.

  - This example has been tested with NUCLEO-G031K8 board and can be
    easily tailored to any other supported device and development board.

  - NUCLEO-G031K8 Set-up
    - Connect GPIOs connected to I2C1 SCL/SDA (PB.8 and PB.9)
    to respectively SCL and SDA pins of I2C2 (PA.11 and PA.12).
      - I2C1_SCL  PB.8 (CN3, pin 11 (D8)) : connected to I2C2_SCL PA.11 (CN4, pin 7 (A5))
      - I2C1_SDA  PB.9 (CN3, pin 13 (D10)) : connected to I2C2_SDA PA.12 (CN4, pin 8 (A4))

  - Launch the program
  - Put and remove a jumper between PA.15 (Arduino D2) and GND to initiate a read request by Master
      then Slave send a byte.

@par How to use it ?

In order to make the program work, you must do the following :
 - Open your preferred toolchain
 - Rebuild all files and load your image into target memory
 - Run the example

 * <h3><center>&copy; COPYRIGHT STMicroelectronics</center></h3>
 */
