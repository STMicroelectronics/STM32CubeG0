/**
  ******************************************************************************
  * @file    stm32g081b_eval_pwr.h
  * @author  MCD Application Team
  * @brief   This file contains all the functions prototypes for the
  *          stm32g081b_eval_pwr.c firmware driver.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT(c) 2018 STMicroelectronics</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef STM32G081B_EVAL_POWER
#define STM32G081B_EVAL_POWER

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32g0xx.h"

/** @addtogroup BSP
  * @{
  */

/** @addtogroup STM32G081B_EVAL
  * @{
  */

/** @defgroup STM32G081B_EVAL_POWER STM32G081B_EVAL POWER
  * @{
  */

/** @defgroup STM32G081B_EVAL_POWER_Exported_Types Exported Types
  * @{
  */

/**
  * @brief  POWER Status
  */
typedef enum
{
    PWR_OK = 0,
    PWR_ERROR
} PWR_StatusTypeDef;


/**
  * @brief  Power role
  */
typedef enum
{
    POWER_ROLE_SOURCE = 0,
    POWER_ROLE_SINK,
    POWER_ROLE_DUAL
} PWR_PowerRoleTypeDef;

/**
  * @brief  Voltage control mode
  */
typedef enum
{
    DCDC_CTRL_MODE_UNKNOWN = 0,
    DCDC_CTRL_MODE_GPIO,
    DCDC_CTRL_MODE_PWM,
} PWR_DCDCCtrlModeTypeDef;

/**
  * @brief  VBUS connection status
  */
typedef enum
{
    VBUS_CONNECTED = 0,
    VBUS_NOT_CONNECTED
} PWR_VBUSConnectionStatusTypeDef;

/**
  * @brief VBUS Detection Callback
  */
typedef void PWR_VBUSDetectCallbackFunc(uint32_t PortId,
                                        PWR_VBUSConnectionStatusTypeDef VBUSConnectionStatus);

/**
  * @}
  */

/** @defgroup STM32G081B_EVAL_POWER_Exported_Constants Exported Constants
  * @{
  */
/**
  * @brief  Number of TypeC ports
  */
#define PWR_TYPEC_PORT_NB 2

/**
  * @brief  Type-C port identifier
  */
#define TYPE_C_PORT_1  0
#define TYPE_C_PORT_2  1

/**
  * @brief  CC pin identifier
  */
#define TYPE_C_CC1     1
#define TYPE_C_CC2     2


/**
  * @brief  VBUS disconnection threshold values (in mV)
  */
#define BSP_PWR_HIGH_VBUS_THRESHOLD     (2800u)
#define BSP_PWR_LOW_VBUS_THRESHOLD       (800u)

/**
  * @brief  VBUS discharge parameters
  */
#define BSP_PWR_DISCHARGE_MARGIN        (500u)
#define BSP_PWR_DISCHARGE_TIME            (6u)

/**
  * @brief  Calibration settings
  */
#define BSP_PWR_DCDC_PRECISION          (20u)     /* DCDC output precision set to 20mV (Noise)*/
#define BSP_PWR_CALIBRATION_ENABLED     (1u)
#define BSP_PWR_CALIBRATION_DISABLED    (0u)

/**
  * @brief  power timeout
  */
#define BSP_PWR_TIMEOUT_PDO             250u
#define BSP_PWR_TIMEOUT_APDO             25u

/**
  * @brief  Invalid value set during issue with voltage setting
  */
#define BSP_PWR_INVALID_VALUE           0xFFFFFFFFu

/**
  * @}
  */


/** @defgroup STM32G081B_EVAL_POWER_Exported_Functions Exported Functions
  * @{
  */
PWR_StatusTypeDef BSP_PWR_VBUSInit(uint32_t PortId);

PWR_StatusTypeDef BSP_PWR_VBUSDeInit(uint32_t PortId);

void BSP_PWR_VBUSIsGPIO(uint32_t *voltage1, uint32_t *voltage2, uint32_t *voltage3);

PWR_StatusTypeDef BSP_PWR_VBUSOn(uint32_t PortId);

PWR_StatusTypeDef BSP_PWR_VBUSOff(uint32_t PortId);

PWR_StatusTypeDef BSP_PWR_VBUSSetVoltage_Fixed(uint32_t PortNum,
                                               uint32_t VbusTargetInmv,
                                               uint32_t OperatingCurrent,
                                               uint32_t MaxOperatingCurrent);

PWR_StatusTypeDef BSP_PWR_VBUSSetVoltage_Variable(uint32_t PortNum,
                                                  uint32_t VbusTargetMaxInmv,
                                                  uint32_t VbusTargetMinInmv,
                                                  uint32_t OperatingCurrent,
                                                  uint32_t MaxOperatingCurrent);

PWR_StatusTypeDef BSP_PWR_VBUSSetVoltage_Battery(uint32_t PortId,
                                                 uint32_t VbusTargetMin,
                                                 uint32_t VbusTargetMax,
                                                 uint32_t OperatingPower,
                                                 uint32_t MaxOperatingPower);
PWR_StatusTypeDef BSP_PWR_VBUSSetVoltage_APDO(uint32_t PortId,
                                              uint32_t VbusTargetInmv,
                                              uint32_t OperatingCurrent,
                                              int32_t Delta);
uint32_t BSP_PWR_VBUSGetVoltage(uint32_t PortId);

int32_t BSP_PWR_VBUSGetCurrent(uint32_t PortId);

PWR_StatusTypeDef BSP_PWR_VCONNInit(uint32_t PortId,
                                    uint32_t CCPinId);

PWR_StatusTypeDef BSP_PWR_VCONNDeInit(uint32_t PortId,
                                      uint32_t CCPinId);

PWR_StatusTypeDef BSP_PWR_VCONNOn(uint32_t PortId,
                                  uint32_t CCPinId);

PWR_StatusTypeDef BSP_PWR_VCONNOff(uint32_t PortId,
                                   uint32_t CCPinId);

void BSP_PWR_SetVBUSDisconnectionThreshold(uint32_t PortId,
                                           uint32_t VoltageThreshold);

PWR_StatusTypeDef BSP_PWR_RegisterVBUSDetectCallback(uint32_t                      PortId,
                                                     PWR_VBUSDetectCallbackFunc *  pfnVBUSDetectCallback);

uint8_t BSP_PWR_VBUSIsOn(uint32_t PortId);

uint8_t BSP_PWR_VCONNIsOn(uint32_t PortId,
                          uint32_t CCPinId);

uint32_t BSP_PWR_DCDCGetVoltage(uint32_t PortId);
PWR_DCDCCtrlModeTypeDef BSP_PWR_DCDCGetCtrlMode(uint32_t PortId);
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

#endif /* STM32G081B_EVAL_POWER */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
