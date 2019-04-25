/**
  @page ADC_MultiChannelSingleConversion ADC example

  @verbatim
  ******************************************************************************
  * @file    Examples/ADC/ADC_MultiChannelSingleConversion/readme.txt 
  * @author  MCD Application Team
  * @brief   Description of the ADC_MultiChannelSingleConversion example.
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
Use ADC to convert several channels using sequencer in discontinuous mode, 
conversion data are transferred by DMA into an array, indefinitely (circular mode).


Example configuration:
ADC is configured to convert 3 channels (1 external channel on GPIO,
2 internal channels VrefInt and temperature sensor),
in discontinuous conversion mode from SW trigger.
DMA is configured to transfer conversion data in an array, in circular mode.

Example execution:
From the start, the ADC converts the selected channels in sequence, one by one
(discontinuous mode) at each trig from Tamper push-button click.
DMA transfers conversion data to the array, DMA transfer complete interruption occurs.
Results array is updated indefinitely (DMA in circular mode).
LED1 is turned on when the DMA transfer is completed (data array full)
and turned off at next DMA half-transfer (data array first half updated).
Note: If DMA buffer is full when user click on Tamper push-button, the buffer is reset
(to ease user to observe behavior).

For debug: variables to monitor with debugger watch window:
 - "aADCxConvertedData": ADC group regular conversion data (array of data)
 - "uhADCxConvertedData_VoltageGPIO_mVolt": Value of voltage on GPIO pin (on which is mapped ADC channel) calculated from ADC conversion data (unit: mV)
 - "uhADCxConvertedData_VrefInt_mVolt": Value of internal voltage reference VrefInt calculated from ADC conversion data (unit: mV)
 - "hADCxConvertedData_Temperature_DegreeCelsius": Value of temperature calculated from ADC conversion data (unit: degree Celcius)
 - "uhADCxConvertedData_VrefAnalog_mVolt": Value of analog reference voltage (Vref+), connected to analog voltage supply Vdda, calculated from ADC conversion data (unit: mV)

Connection needed:
None.
Note: Optionally, a voltage can be supplied to the analog input pin (cf pin below),
      between 0V and Vdda=3.3V, to perform a ADC conversion on a determined
      voltage level.
      Otherwise, this pin can be let floating (in this case ADC conversion data
      will be undetermined).

Other peripherals used:
  1 GPIO for LED
  1 GPIO for analog input: PA4 ( connector CN10 pin 6)
  1 GPIO for push button
  DMA

Board settings:
 - ADC is configured to convert one external channel ADC_CHANNEL_4
   ( connector CN10 pin 6) and the two internal ones ADC_CHANNEL_VREFINT,
   ADC_CHANNEL_TEMPSENSOR.
 - The voltage input on the ADC external channel can be provided by an external source
   connected to  connector CN10 pin 6.



STM32G081B-EVAL board LED is be used to monitor the program execution status:
 - Normal operation: LED1 is turned-on/off in function of ADC conversion
   result.
    - Toggling: "On" upon conversion completion (full DMA buffer filled)
                "Off" upon half conversion completion (half DMA buffer filled)
 - Error: In case of error, LED1 is toggling twice at a frequency of 1Hz.

@par Keywords

Analog, ADC, Analog to Digital, Single conversion, Multi channel, Software trigger

@par Directory contents 

  - ADC/ADC_MultiChannelSingleConversion/Inc/stm32g0xx_it.h          Interrupt handlers header file
  - ADC/ADC_MultiChannelSingleConversion/Inc/main.h                        Header for main.c module  
  - ADC/ADC_MultiChannelSingleConversion/Src/stm32g0xx_it.c          Interrupt handlers
  - ADC/ADC_MultiChannelSingleConversion/Src/stm32g0xx_hal_msp.c     HAL MSP module
  - ADC/ADC_MultiChannelSingleConversion/Src/main.c                        Main program
  - ADC/ADC_MultiChannelSingleConversion/Src/system_stm32g0xx.c      STM32G0xx system source file


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
