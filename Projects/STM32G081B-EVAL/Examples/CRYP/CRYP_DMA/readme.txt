/**
  @page CRYP_DMA  Encrypt and Decrypt data using AES Algo in ECB chaining
   mode using DMA
  
  @verbatim
  ******************************************************************************
  * @file    CRYP/CRYP_DMA/readme.txt 
  * @author  MCD Application Team
  * @brief   Description of the CRYP AES Algorithm in ECB mode with DMA Example
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

How to use the CRYP peripheral to encrypt and decrypt data using the AES-128 
algorithm with ECB chaining mode in DMA mode.

DMA is used to transfer data from memory to the AES processor
input as well as to transfer data from AES processor output to memory.

64-byte buffers are ciphered and deciphered (4 AES blocks)
Ciphering/Deciphering with a 128-bit long key is used with data type set to 8-bit (byte swapping).

This example unrolls as follows: 
- AES Encryption (Plain Data --> Encrypted Data)
- AES Decryption with key derivation (Encrypted Data --> Decrypted Data)

LED3 is used to notify if the Encryption/Decryption failed

Depending of IDE, to watch content of Terminal I/O note that
 - When resorting to IAR Embedded Workbench IDE:
   plain data, encrypted and decrypted data are displayed on debugger Terminal I/O as follows: View --> Terminal I/O.
 - When resorting to MDK-ARM KEIL IDE:
   Command Code could not be displayed on debugger but user can use the Virtual Com port of the STM32G081B-EVAL
   to display Command Code on PC side using an hyperterminal. See below, information related to this possibility.
 - When resorting to AC6 SW4STM32 IDE:
 In Debug configuration window\ Startup, in addition to "monitor reset halt" add the command "monitor arm semihosting enable"
 Command Code is displayed on debugger as follows: Window--> Show View--> Console.

Other proposal to retrieve display of Command Code for all IDE is to use the Virtual Com.

In order to select use of Virtual Com port feature of STLINK for connection between STM32G081B-EVAL and PC,
User has to set USE_VCP_CONNECTION define to 1 in main.h file.
If so, please ensure that USART communication between the target MCU and ST-LINK MCU is properly enabled 
on HW board in order to support Virtual Com Port (Default HW SB configuration allows use of VCP)

When all ciphering and deciphering operations are successful, LED1 is turned on.
In case of ciphering or deciphering issue, LED3 toggles. 

@par Keywords

Security, Cryptography, CRYPT, AES, ECB, DMA, cipher, UART


@note Care must be taken when using HAL_Delay(), this function provides accurate
      delay (in milliseconds) based on variable incremented in SysTick ISR. This
      implies that if HAL_Delay() is called from a peripheral ISR process, then 
      the SysTick interrupt must have higher priority (numerically lower)
      than the peripheral interrupt. Otherwise the caller ISR process will be blocked.
      To change the SysTick interrupt priority you have to use HAL_NVIC_SetPriority() function.
      
@note The example needs to ensure that the SysTick time base is always set to 1 millisecond
      to have correct HAL operation.

@par Directory contents 

  - CRYP/CRYP_DMA/Inc/stm32g0xx_hal_conf.h    HAL configuration file
  - CRYP/CRYP_DMA/Inc/stm32g0xx_it.h          Interrupt handlers header file
  - CRYP/CRYP_DMA/Inc/main.h                  Header for main.c module  
  - CRYP/CRYP_DMA/Src/stm32g0xx_it.c          Interrupt handlers
  - CRYP/CRYP_DMA/Src/main.c                  Main program
  - CRYP/CRYP_DMA/Src/stm32g0xx_hal_msp.c     HAL MSP module
  - CRYP/CRYP_DMA/Src/system_stm32g0xx.c      STM32G0xx system source file

@par Hardware and Software environment

  - This example runs on STM32G081RBTx devices.

  - This example has been tested with STM32G081B-EVAL board with socket using STM32G081RBTx plugged-in  
    and can be easily tailored to any other supported device and development board.
  
  - Hyperterminal configuration:
    - BaudRate = 115200 baud  
    - Word Length = 8 Bits
    - One Stop Bit
    - No parity
    - Hardware flow control disabled (RTS and CTS signals)
    - Receive and transmit enabled

@par How to use it ? 

In order to make the program work, you must do the following:
 - Open your preferred toolchain 
 - Rebuild all files and load your image into target memory
 - Run the example


 * <h3><center>&copy; COPYRIGHT STMicroelectronics</center></h3>
 */
