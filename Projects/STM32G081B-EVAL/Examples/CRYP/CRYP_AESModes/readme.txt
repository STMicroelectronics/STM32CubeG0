/**
  @page CRYP_AESModes  Encrypt and Decrypt data using AES Algo in ECB/CBC/CTR
  chaining modes with all possible key sizes.
  
  @verbatim
  ******************************************************************************
  * @file    CRYP/CRYP_AESModes/readme.txt 
  * @author  MCD Application Team
  * @brief   Description of the CRYP AES Algorithm in all modes and all key sizes
             Example
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

How to use the CRYP peripheral to encrypt and decrypt data using AES in chaining
modes (ECB, CBC, CTR).

In this example, the following key sizes are used: 128 or 256.

Ciphering/Deciphering with a 128-bit long key is used in polling mode with data type set to 8-bit (byte swapping).
Ciphering/Deciphering with a 256-bit long key is used in interrupt mode with data type set to 32-bit (no swapping).
For ECB and CBC, key derivation is carried out at the same time as decryption.

With the 256-bit long key processing, the ciphering output is used as deciphering input. 

This example successively carries out

1. ECB mode:
   - AES128 ECB encryption
   - AES256 ECB encryption
   - AES128 ECB decryption
   - AES128 ECB decryption and key derivation
   - AES256 ECB decryption and key derivation
2. CBC mode:
   - AES128 CBC encryption
   - AES256 CBC encryption
   - AES128 CBC decryption
   - AES128 CBC decryption and key derivation
   - AES256 CBC decryption and key derivation
3. CTR mode:
   - AES128 CTR encryption
   - AES256 CTR encryption
   - AES128 CTR decryption
   - AES256 CTR decryption

@par Keywords

Security, Cryptography, CRYPT, AES, ECB, CBC, CTR, MAC, USART

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
In case of ciphering or deciphering issue, LED3 is turned on. 


@par Directory contents 

  - CRYP/CRYP_AESModes/Inc/stm32g0xx_hal_conf.h    HAL configuration file
  - CRYP/CRYP_AESModes/Inc/stm32g0xx_it.h          Interrupt handlers header file
  - CRYP/CRYP_AESModes/Inc/main.h                  Header for main.c module  
  - CRYP/CRYP_AESModes/Src/stm32g0xx_it.c          Interrupt handlers
  - CRYP/CRYP_AESModes/Src/main.c                  Main program
  - CRYP/CRYP_AESModes/Src/stm32g0xx_hal_msp.c     HAL MSP module
  - CRYP/CRYP_AESModes/Src/system_stm32g0xx.c      STM32G0xx system source file


@par Hardware and Software environment

  - This example runs on STM32G081RBTx devices.

  - This example has been tested with STM32G081B-EVAL board with socket using STM32G081xx plugged-in  
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
