 /**
  ******************************************************************************
  * @file    stm32g071b_discovery_pwrmon.h
  * @author  MCD Application Team
  * @brief   Header file for stm32g071b_discovery_pwrmon.c module
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
#ifndef STM32G071B_DISCOVERY_PWRMON_H
#define STM32G071B_DISCOVERY_PWRMON_H

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32g071b_discovery.h"
#include "../Components/ina230/ina230.h"

/** @addtogroup BSP
  * @{
  */

/** @addtogroup STM32G071B-DISCO
  * @{
  */

/** @addtogroup STM32G071B_DISCOVERY_PWRMON
  * @{
  */

/** @defgroup STM32G071B_DISCOVERY_PWRMON_Exported_Types  Exported Types
  * @{
  */

/**
  * @brief  PWRMON Status
  */
typedef enum
{
  PWRMON_OK = 0,
  PWRMON_ERROR
} PWRMON_StatusTypeDef;

/**
  * @brief  Alert Threshold type
  */
typedef enum {
  VOLTAGE_OVER_THRESHOLD = 0,
  VOLTAGE_UNDER_THRESHOLD
} PWRMON_Threshold_t;

/**
  * @brief Voltage out of range Callback
  */
typedef void PWRMON_VoltageOutOfRangeCallbackFunc(Alert_TypeDef Alert);

/**
  * @brief Conversion ready Callback
  */
typedef void PWRMON_ConversionReadyCallbackFunc(Alert_TypeDef Alert);

/**
  * @}
  */

/** @defgroup STM32G071B_DISCOVERY_PWRMON_Exported_Defines  Exported Defines
  * @{
  */
#define  ALERT_CONVERSION_READY      0x00000001U
#define  ALERT_VOLTAGE_OUT_OF_RANGE  0x00000002U

/**
  * @}
  */

/* Exported functions --------------------------------------------------------*/
/** @defgroup STM32G071B_DISCOVERY_PWRMON_Exported_Functions  Exported Functions
  * @{
  */
PWRMON_StatusTypeDef BSP_PWRMON_Init(Alert_TypeDef       Alert,
                                     PWRMON_Config_t   * pConfig);

PWRMON_StatusTypeDef BSP_PWRMON_DeInit(Alert_TypeDef Alert);

PWRMON_StatusTypeDef BSP_PWRMON_StartMeasure(Alert_TypeDef          Alert,
                                             PWRMON_OperatingMode_t Mode);

PWRMON_StatusTypeDef BSP_PWRMON_StopMeasure(Alert_TypeDef Alert);

uint8_t BSP_PWRMON_IsConversionReady(Alert_TypeDef Alert);

uint8_t BSP_PWRMON_IsVoltageOutOfRange(Alert_TypeDef Alert);

PWRMON_StatusTypeDef BSP_PWRMON_GetVoltage(Alert_TypeDef Alert,
                                           uint32_t *    pVoltage);

PWRMON_StatusTypeDef BSP_PWRMON_GetCurrent(Alert_TypeDef Alert,
                                           int32_t *     pCurrent);

PWRMON_StatusTypeDef BSP_PWRMON_GetPower(Alert_TypeDef Alert,
                                         uint32_t *    pPower);

PWRMON_StatusTypeDef BSP_PWRMON_SetVoltageThreshold(Alert_TypeDef        Alert,
                                                    uint32_t             Threshold,
                                                    PWRMON_Threshold_t   ThresholdType);

PWRMON_StatusTypeDef BSP_PWRMON_GetVoltageThreshold(Alert_TypeDef        Alert,
                                                    uint32_t *           pThreshold,
                                                    PWRMON_Threshold_t * pThresholdType);

PWRMON_StatusTypeDef BSP_PWRMON_ConfigIT(Alert_TypeDef          Alert,
                                         uint32_t               Mask,
                                         PWRMON_AlertPolarity_t Polarity);

PWRMON_StatusTypeDef BSP_PWRMON_EnableIT(Alert_TypeDef Alert);

PWRMON_StatusTypeDef BSP_PWRMON_DisableIT(Alert_TypeDef Alert);

PWRMON_StatusTypeDef BSP_PWRMON_RegisterVoltageOutOfRangeCallback(Alert_TypeDef                          Alert,
                                                                  PWRMON_VoltageOutOfRangeCallbackFunc * pfnVoltageOutOfRangeCallback);

PWRMON_StatusTypeDef BSP_PWRMON_RegisterConversionReadyCallback(Alert_TypeDef                        Alert,
                                                                PWRMON_ConversionReadyCallbackFunc * pfnConversionReadyCallback);

void BSP_PWRMON_AlertNotify(Alert_TypeDef Alert);

/* FOR DEBUG ONLY */
#if defined(INA230_DEBUG)
void BSP_PWRMON_DumpDeviceRegisters(Alert_TypeDef Alert, INA230_Registers_t * pRegisters);
#endif /* INA230_DEBUG */

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

#endif /* STM32G071B_DISCOVERY_PWRMON_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
