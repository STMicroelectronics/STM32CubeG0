/**
  @page ADC_ContinuousConversion_TriggerSW ADC example

  @verbatim
  ******************************************************************************
  * @file    Examples/ADC/ADC_ContinuousConversion_TriggerSW/readme.txt 
  * @author  MCD Application Team
  * @brief   Description of the ADC_ContinuousConversion_TriggerSW example.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2019 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  @endverbatim

@par Example Description
This example provides a short description of how to use the ADC peripheral to
perform conversions in continuous mode.

Example configuration:
ADC is configured to convert a single channel, in continuous conversion mode,
from SW trigger.

Example execution:
After the first trigger (software start in this example), following ADC conversions
are launched successively automatically, indefinitely.
With data overwrite functionality enabled, ADC is continuously updating the ADC conversion data register.

For debug: variables to monitor with debugger watch window:
 - "uhADCxConvertedData": ADC group regular conversion data
 - "uhADCxConvertedData_Voltage_mVolt": ADC conversion data computation to physical values
 
Connection needed:
None.
Note: Optionally, a voltage can be supplied to the analog input pin (cf pin below),
      between 0V and Vdda=3.3V, to perform a ADC conversion on a determined
      voltage level.
      Otherwise, this pin can be let floating (in this case ADC conversion data 
      value will be undetermined).

Other peripherals used:
  1 GPIO for LED
  1 GPIO for analog input: PA4 (pin 10 on connector CN4 (Arduino A2))

Board settings:
 - ADC is configured to convert ADC_CHANNEL_4.
 - The voltage input on ADC channel must be provided by an external source connected to the selected GPIO analog input (pin 10 on connector CN4 (Arduino A2)).


To observe voltage level applied on ADC channel through GPIO, connect a voltmeter on
pin PA4 (pin 10 on connector CN3 (Arduino A2)).

NUCLEO-G031K8 board LED is be used to monitor the program execution status:
 - Normal operation: LED3 is toggling after each data read

@par Keywords

Analog, ADC, Analog to Digital, continuous mode, Continuous conversion, trigger software

@par Directory contents 

  - ADC/ADC_ContinuousConversion_TriggerSW/Inc/stm32g0xx_it.h          Interrupt handlers header file
  - ADC/ADC_ContinuousConversion_TriggerSW/Inc/main.h                  Header for main.c module  
  - ADC/ADC_ContinuousConversion_TriggerSW/Inc/stm32g0xx_hal_conf.h
  - ADC/ADC_ContinuousConversion_TriggerSW/Src/stm32g0xx_it.c          Interrupt handlers
  - ADC/ADC_ContinuousConversion_TriggerSW/Src/main.c                  Main program
  - ADC/ADC_ContinuousConversion_TriggerSW/Src/system_stm32g0xx.c      STM32G0xx system source file
  - ADC/ADC_ContinuousConversion_TriggerSW/Src/stm32g0xx_hal_msp.c

@par Hardware and Software environment

  - This example runs on STM32G031xx devices.
    
  - This example has been tested with NUCLEO-G031K8 board and can be
    easily tailored to any other supported device and development board.


@par How to use it ? 

In order to make the program work, you must do the following :
 - Open your preferred toolchain
 - Rebuild all files and load your image into target memory
 - Run the example


 */
