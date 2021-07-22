/**
  ******************************************************************************
  * @file    stm32g0xx_nucleo_32.h
  * @author  MCD Application Team
  * @brief   Header for stm32g0xx_nucleo32.c
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2019-2021 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef STM32G0XX_NUCLEO_32_H
#define STM32G0XX_NUCLEO_32_H

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32g0xx_hal.h"


/** @addtogroup BSP
  * @{
  */

/** @defgroup STM32G0XX_NUCLEO_32 STM32G0XX_NUCLEO_32
  * @brief This section contains the exported types, constants and functions
  *        required to use the Nucleo 32 board.
  * @{
  */

/** @defgroup STM32G0XX_NUCLEO_32_Exported_Types Exported Types
  * @{
  */

typedef enum
{
  LED3 = 0,
  LED_GREEN = LED3
} Led_TypeDef;

/**
  * @}
  */

/** @defgroup STM32G0XX_NUCLEO_32_Exported_Constants Exported Constants
  * @brief Define for STM32G0XX_NUCLEO_32 board
  * @{
  */

#if !defined (USE_STM32G0XX_NUCLEO_32)
 #define USE_STM32G0XX_NUCLEO_32
#endif

/** @defgroup STM32G0XX_NUCLEO_LED LED Constants
  * @{
  */

#define LEDn                               1

#define LED3_PIN                           GPIO_PIN_6
#define LED3_GPIO_PORT                     GPIOC
#define LED3_GPIO_CLK_ENABLE()             __HAL_RCC_GPIOC_CLK_ENABLE()
#define LED3_GPIO_CLK_DISABLE()            __HAL_RCC_GPIOC_CLK_DISABLE()

#define LEDx_GPIO_CLK_ENABLE(__INDEX__)    do {LED3_GPIO_CLK_ENABLE(); } while(0U)
#define LEDx_GPIO_CLK_DISABLE(__INDEX__)   LED3_GPIO_CLK_DISABLE())

/**
  * @}
  */

/** @defgroup STM32G0XX_NUCLEO_32_BUS BUS Constants
  * @{
  */

#if defined(HAL_I2C_MODULE_ENABLED)
/*##################### I2C2 ###################################*/
/* User can use this section to tailor I2Cx instance used and associated resources */
/* Definition for I2C2 Pins */
#define BSP_I2C2                        I2C2
#define BSP_I2C2_CLK_ENABLE()           __HAL_RCC_I2C2_CLK_ENABLE()
#define BSP_I2C2_CLK_DISABLE()          __HAL_RCC_I2C2_CLK_DISABLE()
#define BSP_I2C2_FORCE_RESET()          __HAL_RCC_I2C2_FORCE_RESET()
#define BSP_I2C2_RELEASE_RESET()        __HAL_RCC_I2C2_RELEASE_RESET()

#define BSP_I2C2_SCL_PIN                GPIO_PIN_11    /* PA.11 arduino A5 */
#define BSP_I2C2_SDA_PIN                GPIO_PIN_12    /* PA.12 arduino A4 */

#define BSP_I2C2_GPIO_PORT              GPIOA      /* GPIOA */
#define BSP_I2C2_GPIO_CLK_ENABLE()      __HAL_RCC_GPIOA_CLK_ENABLE()
#define BSP_I2C2_GPIO_CLK_DISABLE()     __HAL_RCC_GPIOA_CLK_DISABLE()
#define BSP_I2C2_SCL_SDA_AF             GPIO_AF6_I2C2

/* Maximum Timeout values for flags waiting loops. These timeouts are not based
   on accurate values, they just guarantee that the application will not remain
   stuck if the I2C communication is corrupted.
   You may modify these timeout values depending on CPU frequency and application
   conditions (interrupts routines ...). */
#define BSP_I2C2_TIMEOUT_MAX            1000

/* I2C TIMING is calculated in case of the I2C Clock source is the SYSCLK = 80 MHz */
/* Set 0x40E03E53 value to reach 100 KHz speed (Rise time = 640ns, Fall time = 20ns) */
#define I2C2_TIMING                     0x40E03E53

#endif /* HAL_I2C_MODULE_ENABLED */

/**
  * @}
  */

/**
  * @}
  */

/** @defgroup STM32G0XX_NUCLEO_32_Exported_Functions Exported Functions
  * @{
  */

uint32_t         BSP_GetVersion(void);

/** @defgroup STM32G0XX_NUCLEO_LED_Functions LED Functions
  * @{
  */ 

void             BSP_LED_Init(Led_TypeDef Led);
void             BSP_LED_DeInit(Led_TypeDef Led);
void             BSP_LED_On(Led_TypeDef Led);
void             BSP_LED_Off(Led_TypeDef Led);
void             BSP_LED_Toggle(Led_TypeDef Led);

/**
  * @}
  */

/**
  * @}
  */

/**
  * @}
  */

/** @defgroup STM32G0XX_NUCLEO_32_GRAVITECH_4DIGITS GRAVITECH 4 DIGITS
  * @brief This section contains the exported functions
  *        required to use Gravitech shield 7 Segment Display
  * @{
  */

/** @defgroup STM32_GRAVITECH_4DIGITS_Exported_Constants Exported Constants
  * @{
  */

#define DIGIT4_SEG7_RESET 10000
/**
  * @}
  */

/** @defgroup STM32_GRAVITECH_4DIGITS_Exported_Functions Exported Functions
  * @{
  */

HAL_StatusTypeDef BSP_DIGIT4_SEG7_Init(void);
HAL_StatusTypeDef BSP_DIGIT4_SEG7_Display(uint32_t Value);

/**
  * @}
  */

/**
  * @}
  */

/**
  * @}
  */

#ifdef __cplusplus
}
#endif

#endif /* STM32G0XX_NUCLEO_32_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/

