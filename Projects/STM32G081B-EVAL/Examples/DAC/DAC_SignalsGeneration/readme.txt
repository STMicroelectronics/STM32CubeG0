/**
  @page DAC_SignalsGeneration DAC Signals generation example
  
  @verbatim
  ******************************************************************************
  * @file    DAC/DAC_SignalsGeneration/readme.txt 
  * @author  MCD Application Team
  * @brief   Description of the DAC Signals generation example.
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

How to use the DAC peripheral to generate several signals using the DMA
controller and the DAC internal wave generator.
For each press on Tamper push-button, 
A signal has been selected and can be monitored on  
the DAC channel one:
    - Triangle waveform (Channel 1), amplitute: ~1V, frequency: order of Hz.
    - Escalator waveform (Channel 1) using DMA transfer, amplitute: ~3V, frequency: order of KHz.

	
STM32G081B-EVAL board's LEDs can be used to monitor the process status:
  - LED3 is ON and example is stopped (using infinite loop)
  when there is an error during process.

@note Care must be taken when using HAL_Delay(), this function provides accurate delay (in milliseconds)
      based on variable incremented in SysTick ISR. This implies that if HAL_Delay() is called from
      a peripheral ISR process, then the SysTick interrupt must have higher priority (numerically lower)
      than the peripheral interrupt. Otherwise the caller ISR process will be blocked.
      To change the SysTick interrupt priority you have to use HAL_NVIC_SetPriority() function.
      
@note The application need to ensure that the SysTick time base is always set to 1 millisecond
      to have correct HAL operation.

@par Keywords

Analog, DAC, Signals generation, DMA, Triangle, Escalator, Waveform, Amplitude

@par Directory contents 

  - DAC/DAC_SignalsGeneration/Inc/stm32g0xx_hal_conf.h    HAL configuration file
  - DAC/DAC_SignalsGeneration/Inc/stm32g0xx_it.h          DAC interrupt handlers header file
  - DAC/DAC_SignalsGeneration/Inc/main.h                  Header for main.c module  
  - DAC/DAC_SignalsGeneration/Src/stm32g0xx_it.c          DAC interrupt handlers
  - DAC/DAC_SignalsGeneration/Src/main.c                  Main program
  - DAC/DAC_SignalsGeneration/Src/stm32g0xx_hal_msp.c     HAL MSP file
  - DAC/DAC_SignalsGeneration/Src/system_stm32g0xx.c      STM32G0xx system source file

@par Hardware and Software environment  
  - This example runs on STM32G081RBTx devices.
    
  - This example has been tested with STM32G081B-EVAL board and can be
    easily tailored to any other supported device and development board.

  - STM32G081B-EVAL Set-up :
      
      - Connect PA4 (DAC Channel1) (pin 6 in CN10) to an oscilloscope.
      
      - Use Tamper push-button connected to PC.13.

@par How to use it ? 

In order to make the program work, you must do the following :
 - Open your preferred toolchain 
 - Rebuild all files and load your image into target memory
 - Run the example
  
 * <h3><center>&copy; COPYRIGHT STMicroelectronics</center></h3>
 */

