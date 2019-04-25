/**
  @page ADC_Oversampling ADC example
  
  @verbatim
  ******************************************************************************
  * @file    Examples/ADC/ADC_Oversampling/readme.txt 
  * @author  MCD Application Team
  * @brief   Description of the ADC_Oversampling example.
  ******************************************************************************
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
Use ADC to convert a single channel but using oversampling feature to increase resolution. 

Example configuration:
ADC is configured to convert a single channel, in single conversion mode,
from SW trigger.
Oversampling is configured to perform 128 conversions
with a right shift of 3 before returning a result.

Example execution:
The ADC performs conversions of the selected channel in continuous mode from SW start
trigger.
Then, SW is waiting for conversion to complete. When done, it reads conversion data from
ADC data register, stores it into a variable, and convert it into mVolt in another one?

For debug: variables to monitor with debugger watch window:
 - "uhADCxConvertedData": ADC group regular conversion data
 - "uhADCxConvertedData_Voltage_mVolt": ADC conversion data computation to physical values

Connection needed:
None, if ADC channel and DAC channel are selected on the same GPIO.
Otherwise, connect a wire between DAC channel output and ADC input.

Other peripherals used:
  1 GPIO for LED
  1 GPIO for analog input: PA4 (connector CN10 pin 6)
  DAC
  1 GPIO for DAC channel output PA4 (connector CN10 pin 6)
  1 GPIO for push button

Board settings:
 - ADC is configured to convert ADC_CHANNEL_9 (connector CN10 pin 6).
 - The voltage input on ADC channel is provided from DAC (DAC1_CHANNEL_1).
   ADC input from pin PA4 and DAC ouput to pin PA4:
   If same pin is used no connection is required, it is done internally. Otherwise, user need to connect a wire between connector CN10 pin 6 and connector CN10 pin 6
 - Voltage is increasing at each click on Tamper push-button, from 0 to maximum range in 4 steps.
   Clicks on Tamper push-button follow circular cycles: At clicks counter maximum value reached, counter is set back to 0.


To observe voltage level applied on ADC channel through GPIO, connect a voltmeter on
pin PA4 (connector CN10 pin 6).

STM32G081B-EVAL board LED is be used to monitor the program execution status:
 - Normal operation: LED1 is toggling at each conversion.
 - Error: In case of error, LED1 is toggling twice at a frequency of 1Hz.

@par Keywords

Analog, ADC, Analog to Digital, single conversion , Software trigger, Over sampling.

@par Directory contents 

  - ADC/ADC_Oversampling/Inc/stm32g0xx_it.h          Interrupt handlers header file
  - ADC/ADC_Oversampling/Inc/main.h                        Header for main.c module  
  - ADC/ADC_Oversampling/Src/stm32g0xx_it.c          Interrupt handlers
  - ADC/ADC_Oversampling/Src/main.c                        Main program
  - ADC/ADC_Oversampling/Src/stm32g0xx_hal_msp.c     HAL MSP module
  - ADC/ADC_Oversampling/Src/system_stm32g0xx.c      STM32G0xx system source file


@par Hardware and Software environment

  - This example runs on STM32G081xx devices.
    
  - This example has been tested with STM32G081B-EVAL board and can be
    easily tailored to any other supported device and development board.


@par How to use it ? 

In order to make the program work, you must do the following :
 - Open your preferred toolchain
 - Rebuild all files and load your image into target memory
 - Run the example

 * <h3><center>&copy; COPYRIGHT STMicroelectronics</center></h3>
 */
