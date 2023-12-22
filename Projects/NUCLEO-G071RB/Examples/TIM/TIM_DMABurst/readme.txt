/**
  @page TIM_DMABurst TIM_DMABurst example
  
  @verbatim
  ******************************************************************************
  * @file    TIM/TIM_DMABurst/readme.txt 
  * @author  MCD Application Team
  * @brief   Description of the TIM DMA Burst example.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2018 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  @endverbatim

@par Example Description 

How to update the TIMER channel 1 period and duty cycle using the TIMER DMA burst feature.

Every update DMA request, the DMA will do 3 transfers of half words into Timer 
registers beginning from ARR register.
On the DMA update request, 0x0FFF will be transferred into ARR, 0x0000 
will be transferred into RCR (if supported), 0x0555 will be transferred into CCR1. 

The TIM2CLK frequency is set to SystemCoreClock, to get TIM2 counter
clock at 8 MHz the Prescaler is computed as following:
- Prescaler = (TIM2CLK / TIM2 counter clock) - 1

SystemCoreClock is set to 56 MHz.

The TIM2 Frequency = TIM2 counter clock/(ARR + 1)
                   = 8 MHz / 4096 = 1.95 KHz

The TIM2 CCR1 register value is equal to 0x555, so the TIM2 Channel 1 generates a 
PWM signal with a frequency equal to 1.95 KHz and a duty cycle equal to 33.33%:
TIM2 Channel1 duty cycle = (TIM2_CCR1/ TIM2_ARR + 1)* 100 = 33.33%

The PWM waveform can be displayed using an oscilloscope.

@note Care must be taken when using HAL_Delay(), this function provides accurate
      delay (in milliseconds) based on variable incremented in SysTick ISR. This
      implies that if HAL_Delay() is called from a peripheral ISR process, then 
      the SysTick interrupt must have higher priority (numerically lower)
      than the peripheral interrupt. Otherwise the caller ISR process will be blocked.
      To change the SysTick interrupt priority you have to use HAL_NVIC_SetPriority() function.
      
@note This example needs to ensure that the SysTick time base is always set to 1 millisecond
      to have correct HAL operation.

@note In this examples, the output channel is configured in PWM1 mode meaning that in up-counting mode,
      channel 1 is active as long as TIMx_CNT < TIMx_CCR1. As, in the HAL_TIM_PWM_Start* function,
      the output channel is enabled prior enabling the timer's counter a timing difference might be observed
      on the first generated PWM pulse.
      This difference corresponds to the delay - in term of number of CPU cycles - between the channel enable
      and the counter enable in the HAL_TIM_PWM_Start* function.

@par Keywords

Timer, DMA, Burst mode, Duty Cycle, Waveform, Oscilloscope, Output, Signal, PWM

@par Directory contents

  - TIM/TIM_DMABurst/Inc/stm32g0xx_hal_conf.h    HAL configuration file
  - TIM/TIM_DMABurst/Inc/stm32g0xx_it.h          Interrupt handlers header file
  - TIM/TIM_DMABurst/Inc/main.h                  Header for main.c module  
  - TIM/TIM_DMABurst/Src/stm32g0xx_it.c          Interrupt handlers
  - TIM/TIM_DMABurst/Src/main.c                  Main program
  - TIM/TIM_DMABurst/Src/stm32g0xx_hal_msp.c     HAL MSP file
  - TIM/TIM_DMABurst/Src/system_stm32g0xx.c      STM32G0xx system source file

@par Hardware and Software environment

  - This example runs on STM32G071RBTx devices.
  - In this example, the clock is set to 56 MHz.
    
  - This example has been tested with NUCLEO-G071RB 
    board and can be easily tailored to any other supported device 
    and development board.

  - NUCLEO-G071RB Set-up
    - Connect the TIM2 output channel to an oscilloscope to monitor the different waveforms: 
    - TIM2 CH1 (PA.00 (pin 28 in CN7 connector))

@par How to use it ? 

In order to make the program work, you must do the following :
 - Open your preferred toolchain 
 - Rebuild all files and load your image into target memory
 - Run the example


 */
