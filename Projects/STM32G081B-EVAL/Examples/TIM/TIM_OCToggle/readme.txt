/**
  @page TIM_OCToggle TIM_OCToggle example
  
  @verbatim
  ******************************************************************************
  * @file    TIM/TIM_OCToggle/readme.txt 
  * @author  MCD Application Team
  * @brief   This example shows how to configure the Timer to generate four different 
  *          signals with four different frequencies.
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

Configuration of the TIM peripheral to generate four different 
signals at four different frequencies.

  The TIM1 frequency is set to SystemCoreClock, and the objective is
  to get TIM1 counter clock at 1 MHz so the Prescaler is computed as following:
     - Prescaler = (TIM1CLK /TIM1 counter clock) - 1
   
  SystemCoreClock is set to 56 MHz for STM32G0xx Devices.

  The TIM1 CCR1 register value is equal to 625:
  CC1 update rate = TIM1 counter clock / CCR1_Val = 1600 Hz,
  so the TIM1 Channel 1 generates a periodic signal with a frequency equal to 800 Hz.

  The TIM1 CCR2 register value is equal to 1250:
  CC2 update rate = TIM1 counter clock / CCR2_Val = 800 Hz,
  so the TIM1 channel 2 generates a periodic signal with a frequency equal to 400 Hz.

  The TIM1 CCR3 register value is equal to 2500:
  CC3 update rate = TIM1 counter clock / CCR3_Val = 400 Hz,
  so the TIM1 channel 3 generates a periodic signal with a frequency equal to 200 Hz.

  The TIM1 CCR4 register value is equal to 5000:
  CC4 update rate = TIM1 counter clock / CCR4_Val =  200 Hz,
  so the TIM1 channel 4 generates a periodic signal with a frequency equal to 100 Hz.

@note PWM signal frequency values mentioned above are theoretical (obtained when the system clock frequency
      is exactly 56 MHz). Since the generated system clock frequency may vary from one board to another observed
      PWM signal frequency might be slightly different.
	  
@note Care must be taken when using HAL_Delay(), this function provides accurate delay (in milliseconds)
      based on variable incremented in SysTick ISR. This implies that if HAL_Delay() is called from
      a peripheral ISR process, then the SysTick interrupt must have higher priority (numerically lower)
      than the peripheral interrupt. Otherwise the caller ISR process will be blocked.
      To change the SysTick interrupt priority you have to use HAL_NVIC_SetPriority() function.
      
@note This example needs to ensure that the SysTick time base is always set to 1 millisecond
      to have correct HAL operation.

@par Keywords

Timer, Output, signals, Output compare toggle, PWM, Oscilloscope

@par Directory contents

  - TIM/TIM_OCToggle/Inc/stm32g0xx_hal_conf.h    HAL configuration file
  - TIM/TIM_OCToggle/Inc/stm32g0xx_it.h          Interrupt handlers header file
  - TIM/TIM_OCToggle/Inc/main.h                  Header for main.c module  
  - TIM/TIM_OCToggle/Src/stm32g0xx_it.c          Interrupt handlers
  - TIM/TIM_OCToggle/Src/main.c                  Main program
  - TIM/TIM_OCToggle/Src/stm32g0xx_hal_msp.c     HAL MSP file
  - TIM/TIM_OCToggle/Src/system_stm32g0xx.c      STM32G0xx system source file


@par Hardware and Software environment

  - This example runs on STM32G081RBTx devices.
    
  - This example has been tested with STMicroelectronics STM32G081B-EVAL 
    board and can be easily tailored to any other supported device 
    and development board.      

  - STM32G081B-EVAL Set-up
   Connect the following pins to an oscilloscope to monitor the different waveforms:
      - PA8: (TIM1_CH1) (pin 1 in CN5 connector)
      - PA9: (TIM1_CH2) (pin 3 in CN5 connector)
      - PA10: (TIM1_CH3) (pin 4 in CN5 connector)
      - PA11: (TIM1_CH4) (pin 4 in CN9 connector)

@par How to use it ? 

In order to make the program work, you must do the following :
 - Open your preferred toolchain
 - Rebuild all files and load your image into target memory
 - Run the example

 * <h3><center>&copy; COPYRIGHT STMicroelectronics</center></h3>
 */
