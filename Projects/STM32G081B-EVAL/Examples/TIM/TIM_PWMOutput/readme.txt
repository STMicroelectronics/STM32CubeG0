/**
  @page TIM_PWMOutput TIM PWM Output example
  
  @verbatim
  ******************************************************************************
  * @file    TIM/TIM_PWMOutput/readme.txt 
  * @author  MCD Application Team
  * @brief   Description of the PWM signals generation using TIM1
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

Configuration of the TIM peripheral in PWM (pulse width modulation) mode.

SystemCoreClock is set to 56 MHz for STM32G0xx Devices.

    In this example TIM1 input clock (TIM1CLK) is set to APB1 clock (PCLK1),
    since APB1 prescaler is equal to 1.
      TIM1CLK = PCLK1
      PCLK1 = HCLK
      => TIM1CLK = HCLK = SystemCoreClock

    To get TIM1 counter clock at SystemCoreClock, the prescaler is set to 0

    To get TIM1 output clock at 24 KHz, the period (ARR)) is computed as follows:
       ARR = (TIM1 counter clock / TIM1 output clock) - 1
           = 2295

    TIM1 Channel1 duty cycle = (TIM1_CCR1/ TIM1_ARR + 1)* 100 = 50%
    TIM1 Channel2 duty cycle = (TIM1_CCR2/ TIM1_ARR + 1)* 100 = 37.5%
    TIM1 Channel3 duty cycle = (TIM1_CCR3/ TIM1_ARR + 1)* 100 = 25%
    TIM1 Channel4 duty cycle = (TIM1_CCR4/ TIM1_ARR + 1)* 100 = 12.5%


The PWM waveforms can be displayed using an oscilloscope.

@note The duty cycles values mentioned above are theoretical (obtained when the system clock frequency is exactly 56 MHz).
      They might be slightly different depending on system clock frequency precision.

@note Care must be taken when using HAL_Delay(), this function provides accurate delay (in milliseconds)
      based on variable incremented in SysTick ISR. This implies that if HAL_Delay() is called from
      a peripheral ISR process, then the SysTick interrupt must have higher priority (numerically lower)
      than the peripheral interrupt. Otherwise the caller ISR process will be blocked.
      To change the SysTick interrupt priority you have to use HAL_NVIC_SetPriority() function.
      
@note This example needs to ensure that the SysTick time base is always set to 1 millisecond
      to have correct HAL operation.

@par Keywords

Timer, Output, signal, PWM, Oscilloscope, Frequency, Duty cycle, Waveform

@par Directory contents

  - TIM/TIM_PWMOutput/Inc/stm32g0xx_hal_conf.h    HAL configuration file
  - TIM/TIM_PWMOutput/Inc/stm32g0xx_it.h          Interrupt handlers header file
  - TIM/TIM_PWMOutput/Inc/main.h                  Header for main.c module  
  - TIM/TIM_PWMOutput/Src/stm32g0xx_it.c          Interrupt handlers
  - TIM/TIM_PWMOutput/Src/main.c                  Main program
  - TIM/TIM_PWMOutput/Src/stm32g0xx_hal_msp.c     HAL MSP file
  - TIM/TIM_PWMOutput/Src/system_stm32g0xx.c      STM32G0xx system source file


@par Hardware and Software environment

  - This example runs on STM32G081RBTx devices.
  - In this example, the clock is set to 56 MHz.
    
  - This example has been tested with STMicroelectronics STM32G081B-EVAL 
    board and can be easily tailored to any other supported device 
    and development board.      

  - STM32G081B-EVAL Set-up
   Connect the following pins to an oscilloscope to monitor the different waveforms:
        - TIM1_CH1 : PA.08 (pin 1 in CN5 connector)
        - TIM1_CH2 : PA.09 (pin 3 in CN5 connector)
        - TIM1_CH3 : PA.10 (pin 4 in CN5 connector)
        - TIM1_CH4 : PA.11(pin 4 in CN9 connector)


@par How to use it ? 

In order to make the program work, you must do the following :
 - Open your preferred toolchain
 - Rebuild all files and load your image into target memory
 - Run the example

 * <h3><center>&copy; COPYRIGHT STMicroelectronics</center></h3>
 */
