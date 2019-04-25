/**
  ******************************************************************************
  * @file    stm32g071b_discovery_pwr.h
  * @author  MCD Application Team
  * @brief   This file contains BSP PWR interface control functions.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT(c) 2019 STMicroelectronics</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */

#ifndef STM32G71B_DISCOVERY_PWR_H
#define STM32G71B_DISCOVERY_PWR_H
/* Includes ------------------------------------------------------------------*/
#include "stm32g071b_discovery.h"
#include "stm32g071b_discovery_pwrmon.h"

/** @addtogroup BSP
  * @{
  */

/** @addtogroup STM32G071B-DISCO
  * @{
  */

/** @defgroup STM32G071B_DISCOVERY_POWER STM32G071B-DISCO POWER
  * @{
  */

/* Exported defines   ---------------------------------------------------------*/
/** @defgroup STM32G071B_DISCOVERY_POWER_Exported_Constants Exported Constants
  * @{
  */
/**
  * @brief  VBUS disconnection threshold values (in mV)
  */
#define BSP_PWR_HIGH_VBUS_THRESHOLD     (1200u)
#define BSP_PWR_LOW_VBUS_THRESHOLD       (100u)

/**
  * @brief  captive cable enable
  */
#define BSP_PWR_CAPTIVE_CABLE_ENABLED

/**
  * @}
  */


/* Exported functions ---------------------------------------------------------*/

/** @defgroup STM32G071B_DISCOVERY_POWER_Exported_Functions Exported Functions
  * @{
  */
void     BSP_PWR_VBUSInit(uint8_t PortCount);
void     BSP_PWR_VBUSDeInit(uint8_t PortCount);
uint32_t BSP_PWR_VBUSGetVoltage(uint8_t PortCount);
int32_t  BSP_PWR_VBUSGetCurrent(uint8_t PortCount);
void     BSP_PWR_CaptiveCablePreConfig(uint8_t PortNum);
void     BSP_PWR_CaptiveCablePostConfig(uint8_t PortNum);
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

#endif /* STM32G71B_DISCOVERY_PWR_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/

