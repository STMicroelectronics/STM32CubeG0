/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    stm32g071b_discovery_usbpd_pwr.h
  * @author  MCD Application Team
  * @brief   Header file for stm32g071b_discovery_usbpd_pwr.c file
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
/* USER CODE END Header */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef STM32G071B_DISCOVERY_USBPD_PWR_H
#define STM32G071B_DISCOVERY_USBPD_PWR_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32g0xx_ll_pwr.h"
#include "stm32g0xx_ll_exti.h"
#include "stm32g071b_discovery_pwrmon.h"

/** @addtogroup BSP
  * @{
  */

/** @addtogroup STM32G071B-DISCO
  * @{
  */

/** @defgroup STM32G071B_DISCOVERY_USBPD_PWR STM32G071B-DISCO USBPD POWER
  * @{
  */

/** @defgroup STM32G071B_DISCOVERY_USBPD_PWR_Exported_Types2 Exported Types based on BSP V2 secification 
  * @{
  */

/**
  * @brief  Power role
  */
typedef enum
{
  POWER_ROLE_SOURCE = 0,
  POWER_ROLE_SINK,
  POWER_ROLE_DUAL
}
USBPD_PWR_PowerRoleTypeDef;

/**
  * @brief  Voltage control mode
  */
typedef enum
{
  DCDC_CTRL_MODE_UNKNOWN = 0,
  DCDC_CTRL_MODE_GPIO,
  DCDC_CTRL_MODE_PWM,
} USBPD_PWR_DCDCCtrlModeTypeDef;

/**
  * @brief  VBUS connection status
  */
typedef enum
{
  VBUS_CONNECTED = 0,
  VBUS_NOT_CONNECTED
} USBPD_PWR_VBUSConnectionStatusTypeDef;

/**
  * @brief VBUS Detection Callback
  */
typedef void USBPD_PWR_VBUSDetectCallbackFunc(uint32_t Instance,
                                              USBPD_PWR_VBUSConnectionStatusTypeDef VBUSConnectionStatus);

/**
  * @}
  */

/** @defgroup STM32G071B_DISCOVERY_USBPD_POWER_Exported_Constants Exported Constants based on BSP V1 specification (keep for legacy)
  * @{
  */
/**
  * @brief  VBUS disconnection threshold values (in mV)
  */
#define BSP_PWR_HIGH_VBUS_THRESHOLD     USBPD_PWR_HIGH_VBUS_THRESHOLD
#define BSP_PWR_LOW_VBUS_THRESHOLD      USBPD_PWR_LOW_VBUS_THRESHOLD



/**
  * @brief  captive cable enable
  */
#define BSP_PWR_CAPTIVE_CABLE_ENABLED

/**
  * @}
  */

/** @defgroup STM32G071B_DISCOVERY_USBPD_PWR_Exported_Constants2 Exported Constants on BSP V2 specification
  * @{
  */
/* Defines error for BSP V2 (copy for legacy reasons) */
/* Common Error codes */
#ifndef BSP_ERROR_NONE
  #define BSP_ERROR_NONE                         0
#endif
#ifndef BSP_ERROR_NO_INIT
  #define BSP_ERROR_NO_INIT                     -1
#endif
#ifndef BSP_ERROR_WRONG_PARAM
  #define BSP_ERROR_WRONG_PARAM                 -2
#endif
#ifndef BSP_ERROR_PERIPH_FAILURE
  #define BSP_ERROR_PERIPH_FAILURE              -4
#endif
#ifndef BSP_ERROR_FEATURE_NOT_SUPPORTED
  #define BSP_ERROR_FEATURE_NOT_SUPPORTED       -11
#endif
  
/**
  * @brief  Number of TypeC ports
  */
#define USBPD_PWR_INSTANCES_NBR               2u

/**
  * @brief  Type-C port identifier
  */
#define USBPD_PWR_TYPE_C_PORT_1           0u
#define USBPD_PWR_TYPE_C_PORT_2           1u

/**
  * @brief  CC pin identifier
  */
#define USBPD_PWR_TYPE_C_CC1              1u
#define USBPD_PWR_TYPE_C_CC2              2u

/**
  * @brief  VBUS disconnection threshold values (in mV)
  */
#define USBPD_PWR_HIGH_VBUS_THRESHOLD     (1200u)
#define USBPD_PWR_LOW_VBUS_THRESHOLD       (100u)
#define USBPD_PWR_VBUS_THRESHOLD_5V        USBPD_PWR_HIGH_VBUS_THRESHOLD
#define USBPD_PWR_VBUS_THRESHOLD_9V        USBPD_PWR_HIGH_VBUS_THRESHOLD
#define USBPD_PWR_VBUS_THRESHOLD_15V       USBPD_PWR_HIGH_VBUS_THRESHOLD
#define USBPD_PWR_VBUS_THRESHOLD_20V       USBPD_PWR_HIGH_VBUS_THRESHOLD
#define USBPD_PWR_VBUS_THRESHOLD_APDO      USBPD_PWR_HIGH_VBUS_THRESHOLD

/**
  * @brief  VBUS discharge parameters
  */
#define USBPD_PWR_DISCHARGE_MARGIN        (500u)
#define USBPD_PWR_DISCHARGE_TIME            (6u)

/**
  * @brief  Calibration settings
  */
#define USBPD_PWR_DCDC_PRECISION          (40u)     /* DCDC output precision set to 40mV (Noise)*/
#define USBPD_PWR_CALIBRATION_ENABLED     (1u)
#define USBPD_PWR_CALIBRATION_DISABLED    (0u)

/**
  * @brief  Standard VBUS voltage levels
  */
#define USBPD_PWR_VBUS_5V                 5000u
#define USBPD_PWR_VBUS_9V                 9000u
#define USBPD_PWR_VBUS_15V                15000u

/**
  * @brief  power timeout
  */
#define USBPD_PWR_TIMEOUT_PDO             250u         /* Timeout for PDO to PDO or PDO to APDO at 250ms*/
#define USBPD_PWR_TIMEOUT_APDO            25u          /* Timeout for APDO to APDO at 25ms*/

/**
  * @brief  Invalid value set during issue with voltage setting
  */
#define USBPD_PWR_INVALID_VALUE           0xFFFFFFFFu

/**
  * @brief  captive cable enable
  */
#define USBPD_PWR_CAPTIVE_CABLE_ENABLED

/**
  * @}
  */

/** @defgroup STM32G071B_DISCOVERY_USBPD_PWR_Exported_Functions2 Exported Functions based on BSP V2 specification
  * @{
  */
/* Common functions */
int32_t BSP_USBPD_PWR_Init(uint32_t Instance);

int32_t BSP_USBPD_PWR_Deinit(uint32_t Instance);

int32_t BSP_USBPD_PWR_VBUSInit(uint32_t Instance);

int32_t BSP_USBPD_PWR_VBUSDeInit(uint32_t Instance);

int32_t BSP_USBPD_PWR_VBUSOn(uint32_t Instance);

int32_t BSP_USBPD_PWR_VBUSOff(uint32_t Instance);

int32_t BSP_USBPD_PWR_VBUSSetVoltage_Fixed(uint32_t Instance,
                                           uint32_t VbusTargetInmv,
                                           uint32_t OperatingCurrent,
                                           uint32_t MaxOperatingCurrent);

int32_t BSP_USBPD_PWR_VBUSSetVoltage_Variable(uint32_t Instance,
                                              uint32_t VbusTargetMaxInmv,
                                              uint32_t VbusTargetMinInmv,
                                              uint32_t OperatingCurrent,
                                              uint32_t MaxOperatingCurrent);

int32_t BSP_USBPD_PWR_VBUSSetVoltage_Battery(uint32_t Instance,
                                             uint32_t VbusTargetMin,
                                             uint32_t VbusTargetMax,
                                             uint32_t OperatingPower,
                                             uint32_t MaxOperatingPower);

int32_t BSP_USBPD_PWR_VBUSSetVoltage_APDO(uint32_t Instance,
                                          uint32_t VbusTargetInmv,
                                          uint32_t OperatingCurrent,
                                          int32_t Delta);

int32_t BSP_USBPD_PWR_VBUSGetVoltage(uint32_t Instance, uint32_t *pVoltage);

int32_t BSP_USBPD_PWR_VBUSGetCurrent(uint32_t Instance, int32_t *pCurrent);

int32_t BSP_USBPD_PWR_VCONNInit(uint32_t Instance,
                                uint32_t CCPinId);

int32_t BSP_USBPD_PWR_VCONNDeInit(uint32_t Instance,
                                  uint32_t CCPinId);

int32_t BSP_USBPD_PWR_VCONNOn(uint32_t Instance,
                              uint32_t CCPinId);

int32_t BSP_USBPD_PWR_VCONNOff(uint32_t Instance,
                               uint32_t CCPinId);

int32_t BSP_USBPD_PWR_SetVBUSDisconnectionThreshold(uint32_t Instance,
                                                    uint32_t VoltageThreshold);

int32_t BSP_USBPD_PWR_RegisterVBUSDetectCallback(uint32_t                      Instance,
                                                 USBPD_PWR_VBUSDetectCallbackFunc   *pfnVBUSDetectCallback);

int32_t BSP_USBPD_PWR_VBUSIsOn(uint32_t Instance, uint8_t *pState);

int32_t BSP_USBPD_PWR_VCONNIsOn(uint32_t Instance,
                                uint32_t CCPinId, uint8_t *pState);

/* Specific functions */
int32_t     BSP_USBPD_PWR_CaptiveCablePreConfig(uint8_t PortNum);

int32_t     BSP_USBPD_PWR_CaptiveCablePostConfig(uint8_t PortNum);

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

#endif /* STM32G071B_DISCOVERY_USBPD_PWR_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
