/**
  @page PWR_PVD PWR Programmable Voltage Detector (PVD) example
  
  @verbatim
  ******************************************************************************
  * @file    PWR/PWR_PVD/readme.txt 
  * @author  MCD Application Team
  * @brief   Description of the PWR Programmable Voltage Detector (PVD) example
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
How to configure the programmable voltage detector by using an external interrupt 
line. External DC supply must be used to supply Vdd.

In this example, EXTI line 16 is configured to generate an interrupt on each rising
or falling edge of the PVD output signal (which indicates that the Vdd voltage is
moving below or above the PVD threshold). As long as the voltage is above the 
target threshold (2.5V), LED4 is blinking with a 200 ms-period; when the voltage drops
below the threshold, LED4 stops blinking and remains constantly on (or appears
to be turned off if the voltage is getting really low); when the voltage moves back
above the target threshold, LED4 starts blinking again.

@note Care must be taken when using HAL_Delay(), this function provides accurate delay (in milliseconds)
      based on variable incremented in SysTick ISR. This implies that if HAL_Delay() is called from
      a peripheral ISR process, then the SysTick interrupt must have higher priority (numerically lower)
      than the peripheral interrupt. Otherwise the caller ISR process will be blocked.
      To change the SysTick interrupt priority you have to use HAL_NVIC_SetPriority() function.
      
@note The application needs to ensure that the SysTick time base is always set to 1 millisecond
      to have correct HAL operation.

@par Keywords

Power, PWR, EXTI, PVD, Interrupt, Wakeup, External reset

@par Directory contents 

  - PWR/PWR_PVD/Inc/stm32g0xx_hal_conf.h     HAL Configuration file
  - PWR/PWR_PVD/Inc/stm32g0xx_it.h           Header for stm32g0xx_it.c
  - PWR/PWR_PVD/Inc/main.h                         Header file for main.c
  - PWR/PWR_PVD/Src/system_stm32g0xx.c       STM32G0xx system clock configuration file
  - PWR/PWR_PVD/Src/stm32g0xx_it.c           Interrupt handlers
  - PWR/PWR_PVD/Src/stm32g0xx_hal_msp.c      HAL MSP module
  - PWR/PWR_PVD/Src/main.c                         Main program

@par Hardware and Software environment

  - This example runs on STM32G071xx devices
    
  - This example has been tested with STMicroelectronics NUCLEO-G071RB
    board and can be easily tailored to any other supported device
    and development board.

  - NUCLEO-G071RB Set-up :
    To apply an external power supply input, the user can
    - apply the E5V input in
      * setting V+ to Morpho connector CN7 pin 6 (pin 8 being used as GND)
      * moving jumper JP2 on E5V
      * removing jumper JP1
    - or apply the +3V3 input
      * setting V+ to Morpho connector CN7 pins 12 or Arduino connector CN6 pin 2
      * setting GND to Morpho connector CN7 pins 20 or Arduino connector CN6 pin 6
      * removing jumper JP2
    Next,         
    - apply an external voltage on the proper connector pins as selected above
    - check the board behaviour according to LED4.

@par How to use it ? 

In order to make the program work, you must do the following :
 - Open your preferred toolchain 
 - Rebuild all files and load your image into target memory
 - Run the example


 * <h3><center>&copy; COPYRIGHT STMicroelectronics</center></h3>
 */
