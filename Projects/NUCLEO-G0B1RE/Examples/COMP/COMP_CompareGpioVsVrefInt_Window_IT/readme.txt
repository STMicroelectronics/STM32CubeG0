/**
  @page COMP_CompareGpioVsVrefInt_Window_IT COMP example

  @verbatim
  ******************************************************************************
  * @file    COMP/COMP_CompareGpioVsVrefInt_Window_IT/readme.txt
  * @author  MCD Application Team
  * @brief   Description of the COMP_CompareGpioVsVrefInt_Window_IT example.
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

@par Example Description

How to use a pair of comparator peripherals to compare a voltage level applied on
a GPIO pin to two thresholds: the internal voltage reference (VREFINT) and a fraction
of the internal voltage reference (VREFINT/2), in interrupt mode.

Example configuration:
Comparator instances COMP1 and COMP2 are configured to work together
in window mode to compare 2 thresholds:
 - COMP1 input plus set to a GPIO pin (cf pin below) in analog mode.
   COMP2 input plus is not used (connected internally to the 
   same input plus as COMP1).
 - COMP1 input minus set to internal voltage reference VrefInt 
   (voltage level 1.2V, refer to device datasheet for min/typ/max values)
 - COMP2 input minus set to 1/2 of internal voltage reference VrefInt 
   (voltage level 0.6V)
Both Comparators interruption are enabled through EXTI lines
with trigger edge set to both edges rising and falling.

Example execution:
From the main program execution, comparator is enabled.
Each time the voltage level applied on GPIO pin (comparator input plus) 
is crossing VrefInt or 1/2 VrefInt voltage thresholds (comparator input minus),
the comparator with corresponding threshold generates an interruption.
LED4 is turned on or off depending on the pair of comparators output state:
turned on if comparator voltage level is inside the window (between VrefInt
and 1/2 VrefInt: between 1.2V and 0.6V), turned off otherwise.

Connection needed:
A voltage must be supplied to the analog input pin (cf pin below),
between Vdda=3.3V and 0V. Otherwise this pin can be let floating (but in this
case COMP comparison level will be undetermined).

To check comparator window thresholds, you must use an external power supply
to set voltage level below, above or within comparator thresholds.

Other peripheral used:
  1 GPIO for LED4
  1 GPIO for analog input of comparator: PA.01 (Arduino connector CN8 pin A1, Morpho connector CN7 pin 30)

@par Keywords

Comparator, COMP, analog, voltage, analog input, threshold, VrefInt, window

@par Directory contents
  - PWR/COMP_CompareGpioVsVrefInt_Window_IT/Inc/stm32g0xx_nucleo_conf.h BSP configuration file
  - PWR/COMP_CompareGpioVsVrefInt_Window_IT/Inc/stm32g0xx_hal_conf.h    HAL configuration file
  - PWR/COMP_CompareGpioVsVrefInt_Window_IT/Inc/stm32g0xx_it.h          Interrupt handlers header file
  - PWR/COMP_CompareGpioVsVrefInt_Window_IT/Inc/main.h                  Header for main.c module
  - PWR/COMP_CompareGpioVsVrefInt_Window_IT/Src/stm32g0xx_it.c          Interrupt handlers
  - PWR/COMP_CompareGpioVsVrefInt_Window_IT/Src/stm32g0xx_hal_msp.c     HAL MSP file
  - PWR/COMP_CompareGpioVsVrefInt_Window_IT/Src/main.c                  Main program
  - PWR/COMP_CompareGpioVsVrefInt_Window_IT/Src/system_stm32g0xx.c      STM32G0xx system source file


@par Hardware and Software environment
  - This example runs on STM32G0B1RETx devices.
  - This example has been tested with STMicroelectronics NUCLEO-G0B1RE
    board and can be easily tailored to any other supported device
    and development board.

@par How to use it ?
In order to make the program work, you must do the following :
 - Open your preferred toolchain
 - Rebuild all files and load your image into target memory
 - Run the example


 */

