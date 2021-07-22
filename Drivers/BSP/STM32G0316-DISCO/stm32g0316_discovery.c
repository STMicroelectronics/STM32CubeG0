/**
  ******************************************************************************
  * @file    stm32g0316_discovery.c
  * @author  MCD Application Team
  * @brief   This file provides a set of firmware functions to manage Leds,
  *          push-button of STM32G0316-DISCO board (MB1454)
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

/* Includes ------------------------------------------------------------------*/
#include "stm32g0316_discovery.h"

/** @addtogroup BSP
 * @{
 */

/** @addtogroup STM32G0316-DISCO
  * @brief This file provides firmware functions to manage Leds, push-buttons,
  *        available on STM32G0316-DISCO board from STMicroelectronics.
  * @{
  */

/** @defgroup STM32G0316_DISCOVERY_Private_Constants Private Constants
  * @{
  */

/**
 * @brief STM32G0316 DISCOVERY BSP Driver version number
   */
#define __STM32G0316_DISCOVERY_BSP_VERSION_MAIN   (0x01U) /*!< [31:24] main version */
#define __STM32G0316_DISCOVERY_BSP_VERSION_SUB1   (0x00U) /*!< [23:16] sub1 version */
#define __STM32G0316_DISCOVERY_BSP_VERSION_SUB2   (0x02U) /*!< [15:8]  sub2 version */
#define __STM32G0316_DISCOVERY_BSP_VERSION_RC     (0x00U) /*!< [7:0]  release candidate */
#define __STM32G0316_DISCOVERY_BSP_VERSION            ((__STM32G0316_DISCOVERY_BSP_VERSION_MAIN << 24)\
                                                       |(__STM32G0316_DISCOVERY_BSP_VERSION_SUB1 << 16)\
                                                       |(__STM32G0316_DISCOVERY_BSP_VERSION_SUB2 << 8 )\
                                                       |(__STM32G0316_DISCOVERY_BSP_VERSION_RC))
/**
  * @}
  */

/** @defgroup STM32G0316_DISCOVERY_Exported_Variables Exported Variables
  * @{
  */

/**
 * @brief LED variables
 */
GPIO_TypeDef*   LED_PORT[LEDn] =  {LED2_GPIO_PORT};

const uint16_t  LED_PIN[LEDn] =   {LED2_PIN};

/**
  * @}
  */

/** @addtogroup STM32G0316_DISCOVERY_Exported_Functions
  * @{
  */

/**
  * @brief  This method returns the STM32G0316 DISCOVERY BSP Driver revision
  * @retval version : 0xXYZR (8bits for each decimal, R for RC)
  */
uint32_t BSP_GetVersion(void)
{
  return __STM32G0316_DISCOVERY_BSP_VERSION;
}

/** @addtogroup STM32G0316_DISCOVERY_Exported_Functions_LED
  * @{
  */
/**
  * @brief  Configures LED GPIOs.
  * @param  Led Specifies the Led to be configured.
  *   This parameter can be one of following parameters:
  *     @arg LED2
  * @retval None
  */
void BSP_LED_Init(Led_TypeDef Led)
{
  GPIO_InitTypeDef  GPIO_InitStructure;

  /* Enable the GPIO_LED clock */
  LEDx_GPIO_CLK_ENABLE(Led);

  /* Configure the GPIO_LED pin */
  GPIO_InitStructure.Pin = LED_PIN[Led];
  GPIO_InitStructure.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStructure.Pull = GPIO_NOPULL;
  GPIO_InitStructure.Speed = GPIO_SPEED_FREQ_VERY_HIGH;

  HAL_GPIO_Init(LED_PORT[Led], &GPIO_InitStructure);

  HAL_GPIO_WritePin(LED_PORT[Led], GPIO_InitStructure.Pin, GPIO_PIN_RESET);
}

/**
  * @brief  Unconfigures LED GPIOs.
  * @param  Led Specifies the Led to be unconfigured.
  *   This parameter can be one of following parameters:
  *     @arg LED2
  * @retval None
  */
void BSP_LED_DeInit(Led_TypeDef Led)
{
  HAL_GPIO_DeInit(LED_PORT[Led], LED_PIN[Led]);
}

/**
  * @brief  Turns selected LED On.
  * @param  Led Specifies the Led to be set on.
  *   This parameter can be one of following parameters:
  *     @arg LED2
  * @retval None
  */
void BSP_LED_On(Led_TypeDef Led)
{
  HAL_GPIO_WritePin(LED_PORT[Led], LED_PIN[Led], GPIO_PIN_SET);
}

/**
  * @brief  Turns selected LED Off.
  * @param  Led Specifies the Led to be set off.
  *   This parameter can be one of following parameters:
  *     @arg LED2
  * @retval None
  */
void BSP_LED_Off(Led_TypeDef Led)
{
  HAL_GPIO_WritePin(LED_PORT[Led], LED_PIN[Led], GPIO_PIN_RESET);
}

/**
  * @brief  Toggles the selected LED.
  * @param  Led Specifies the Led to be toggled.
  *   This parameter can be one of following parameters:
  *     @arg LED2
  * @retval None
  */
void BSP_LED_Toggle(Led_TypeDef Led)
{
  HAL_GPIO_TogglePin(LED_PORT[Led], LED_PIN[Led]);
}

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

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
