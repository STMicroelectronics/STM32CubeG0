/**
  ******************************************************************************
  * @file    stm32g0316_discovery.h
  * @author  MCD Application Team
  * @brief   Header for stm32g0316_discovery.c module
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
#ifndef STM32G0316_DISCOVERY_H
#define STM32G0316_DISCOVERY_H

#ifdef __cplusplus
extern "C" {
#endif

/** @addtogroup BSP
  * @{
  */

/** @defgroup STM32G0316-DISCO STM32G0316-DISCO
  * @{
  */

/* Includes ------------------------------------------------------------------*/
#include "stm32g0xx_hal.h"

/** @defgroup STM32G0316_DISCOVERY_Exported_Types Exported Types
  * @{
  */

/**
 * @brief LED Types Definition
 */
typedef enum
{
  LED2 = 0,
  LED_GREEN   = LED2,
}Led_TypeDef;

/**
  * @}
  */

/** @defgroup STM32G0316_DISCOVERY_Exported_Constants Exported Constants
  * @{
  */

/** @defgroup STM32G0316_DISCOVERY_LED LED Constants
  * @{
  */
#define LEDn                              1

#define LED2_PIN                          GPIO_PIN_12
#define LED2_GPIO_PORT                    GPIOA
#define LED2_GPIO_CLK_ENABLE()            __HAL_RCC_GPIOA_CLK_ENABLE()
#define LED2_GPIO_CLK_DISABLE()           __HAL_RCC_GPIOA_CLK_DISABLE()

#define LEDx_GPIO_CLK_ENABLE(__LED__)     do { if((__LED__) == 0) LED2_GPIO_CLK_ENABLE();} while(0)

#define LEDx_GPIO_CLK_DISABLE(__LED__)    do { if((__LED__) == 0) LED2_GPIO_CLK_DISABLE();} while(0)

/**
  * @}
  */

/**
  * @}
  */

/** @defgroup STM32G0316_DISCOVERY_Exported_Functions Exported Functions
  * @{
  */
uint32_t                BSP_GetVersion(void);

/** @defgroup STM32G0316_DISCOVERY_Exported_Functions_LED LED Functions
  * @{
  */
void                    BSP_LED_Init(Led_TypeDef Led);
void                    BSP_LED_DeInit(Led_TypeDef Led);
void                    BSP_LED_On(Led_TypeDef Led);
void                    BSP_LED_Off(Led_TypeDef Led);
void                    BSP_LED_Toggle(Led_TypeDef Led);
/**
  * @}
  */

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

#endif /* STM32G0316_DISCOVERY_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
