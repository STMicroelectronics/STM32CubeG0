/**
  @page BSP_Example  Example on how to use the BSP drivers
  
  @verbatim
  ******************************************************************************
  * @file    BSP/BSP_Example/readme.txt 
  * @author  MCD Application Team
  * @brief   Description of the BSP example.
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

This example provides a description of how to use the different BSP drivers. 

At the beginning of the main program the HAL_Init() function is called to reset 
all the peripherals, initialize the Flash interface and the systick.
Then the SystemClock_Config() function is used to configure the system
clock (SYSCLK) to run at 64 MHz.

This example shows how to use the different functionalities of components 
available on the board by switching between all tests using Tamper push-button button. 

 ** Push the User button to start first Test.  
Blue Led (LD4) will blink between each test. Press Tamper push-button to start another test:

 ** JOYSTICK **
Use the joystick button to move a pointer inside a rectangle 
(up/down/right/left) and change the pointer color(select).

 ** LCD **
This example shows how to use the different LCD features to display string
with different fonts, to display different shapes and to draw a bitmap.

 ** SD **
This example shows how to erase, write and read the SD card and also 
how to detect the presence of the card.

 ** TSENSOR **
This example show how to read a Temperature using the temperature sensor.
Temperature sensor is phycicaly present on daughter board MB1315.
Please make sure that MB1351 daughter board is mounted on top of EVAL board.

 ** LCD LOG **
This example show how to use the LCD log features. 

@note Care must be taken when using HAL_Delay(), this function provides accurate delay (in milliseconds)
      based on variable incremented in HAL time base ISR. This implies that if HAL_Delay() is called from
      a peripheral ISR process, then the HAL time base interrupt must have higher priority (numerically lower)
      than the peripheral interrupt. Otherwise the caller ISR process will be blocked.
      To change the HAL time base interrupt priority you have to use HAL_NVIC_SetPriority() function.
      
@note The application needs to ensure that the HAL time base is always set to 1 millisecond
      to have correct HAL operation.

@par Keywords

BSP,

@par Directory contents 

  - BSP/Src/main.c                 Main program
  - BSP/Src/system_stm32g0xx.c     STM32G0xx system clock configuration file
  - BSP/Src/stm32g0xx_it.c         Interrupt handlers 
  - BSP/Src/joystick.c             joystick feature
  - BSP/Src/lcd.c                  LCD drawing features
  - BSP/Src/log.c                  LCD Log firmware functions
  - BSP/Src/sd.c                   SD features
  - BSP/Src/temperature_sensor.c   Temperature Sensor features
  - BSP/Inc/main.h                 Main program header file  
  - BSP/Inc/stm32g0xx_conf.h       HAL Configuration file
  - BSP/Inc/stm32g0xx_it.h         Interrupt handlers header file
  - BSP/Inc/lcd_log_conf.h         lcd_log configuration template file
  - BSP/Inc/stlogo.h               Image used for BSP example

@par Hardware and Software environment

  - This example runs on STM32G0C1VExx devices.
    
  - This example has been tested with STM32G0C1E-EV board and can be
    easily tailored to any other supported device and development board.

@par How to use it ? 

In order to make the program work, you must do the following :
 - Open your preferred toolchain 
 - Rebuild all files and load your image into target memory
 - Run the example


 */
