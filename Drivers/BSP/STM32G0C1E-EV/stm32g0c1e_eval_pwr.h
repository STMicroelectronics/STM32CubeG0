/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    stm32g0c1e_eval_pwr.h
  * @author  MCD Application Team
  * @brief   This file contains all the functions prototypes for the
  *          stm32g0c1e_eval_pwr.c firmware driver.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2020-2021 STMicroelectronics.
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
#ifndef STM32G0C1E_EVAL_POWER
#define STM32G0C1E_EVAL_POWER

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32g0xx.h"

/** @addtogroup BSP
  * @{
  */

/** @addtogroup STM32G0C1E_EVAL
  * @{
  */

/** @defgroup STM32G0C1E_EVAL_POWER STM32G0C1E_EVAL POWER
  * @{
  */

/** @defgroup STM32G0C1E_EVAL_POWER_Exported_Types Exported Types based on BSP V1 specification (keep for legacy)
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
#define PWR_PowerRoleTypeDef USBPD_PWR_PowerRoleTypeDef

/**
  * @brief  Voltage control mode
  */
#define PWR_DCDCCtrlModeTypeDef USBPD_PWR_DCDCCtrlModeTypeDef

/**
  * @brief  VBUS connection status
  */
#define PWR_VBUSConnectionStatusTypeDef USBPD_PWR_VBUSConnectionStatusTypeDef

/**
  * @}
  */

/** @defgroup STM32G0C1E_EVAL_POWER_Exported_Types2 Exported Types based on BSP V2 secification 
  *@{
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

/* Keep for Legacy BSP V1 */
/**
  * @brief VBUS Detection Callback
  */
typedef void PWR_VBUSDetectCallbackFunc(uint32_t Instance,
                                        PWR_VBUSConnectionStatusTypeDef VBUSConnectionStatus);

/**
  * @brief VBUS Detection Callback
  */
typedef void USBPD_PWR_VBUSDetectCallbackFunc(uint32_t Instance,
                                              USBPD_PWR_VBUSConnectionStatusTypeDef VBUSConnectionStatus);

/**
  * @}
  */

/** @defgroup STM32G0C1E_EVAL_POWER_Exported_Constants Exported Constants based on BSP V1 specification (keep for legacy)
  * @{
  */
/**
  * @brief  Number of TypeC ports
  */
#define PWR_TYPEC_PORT_NB USBPD_PWR_INSTANCES_NBR

/**
  * @brief  Type-C port identifier
  */
#define TYPE_C_PORT_1  USBPD_PWR_TYPE_C_PORT_1
#define TYPE_C_PORT_2  USBPD_PWR_TYPE_C_PORT_2

/**
  * @brief  CC pin identifier
  */
#define TYPE_C_CC1     USBPD_PWR_TYPE_C_CC1
#define TYPE_C_CC2     USBPD_PWR_TYPE_C_CC2


/**
  * @brief  VBUS disconnection threshold values (in mV)
  */
#define BSP_PWR_HIGH_VBUS_THRESHOLD     USBPD_PWR_HIGH_VBUS_THRESHOLD
#define BSP_PWR_LOW_VBUS_THRESHOLD      USBPD_PWR_LOW_VBUS_THRESHOLD

/**
  * @brief  VBUS discharge parameters
  */
#define BSP_PWR_DISCHARGE_MARGIN        USBPD_PWR_DISCHARGE_MARGIN
#define BSP_PWR_DISCHARGE_TIME          USBPD_PWR_DISCHARGE_TIME

/**
  * @brief  Calibration settings
  */
#define BSP_PWR_DCDC_PRECISION          USBPD_PWR_DCDC_PRECISION
#define BSP_PWR_CALIBRATION_ENABLED     USBPD_PWR_CALIBRATION_ENABLED
#define BSP_PWR_CALIBRATION_DISABLED    USBPD_PWR_CALIBRATION_DISABLED

/**
  * @brief  power timeout
  */
#define BSP_PWR_TIMEOUT_PDO             USBPD_PWR_TIMEOUT_PDO
#define BSP_PWR_TIMEOUT_APDO            USBPD_PWR_TIMEOUT_APDO

/**
  * @brief  Invalid value set during issue with voltage setting
  */
#define BSP_PWR_INVALID_VALUE           USBPD_PWR_INVALID_VALUE

/**
  * @}
  */

/** @defgroup STM32G0C1E_EVAL_USBPD_PWR_Exported_Constants2 Exported Constants on BSP V2 specification
  @{
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
#define USBPD_PWR_INSTANCES_NBR                 2u

/**
  * @brief  Type-C port identifier
  */
#define USBPD_PWR_TYPE_C_PORT_1                 0u
#define USBPD_PWR_TYPE_C_PORT_2                 1u

/**
  * @brief  CC pin identifier
  */
#define USBPD_PWR_TYPE_C_CC1                    1u
#define USBPD_PWR_TYPE_C_CC2                    2u

/**
  * @brief  VBUS disconnection threshold values (in mV)
  */
#define USBPD_PWR_HIGH_VBUS_THRESHOLD           (2800u)
#define USBPD_PWR_LOW_VBUS_THRESHOLD            (750U)
#define USBPD_PWR_VBUS_THRESHOLD_5V             (3900u)
#define USBPD_PWR_VBUS_THRESHOLD_9V             (7000u)
#define USBPD_PWR_VBUS_THRESHOLD_15V            (12500u)
#define USBPD_PWR_VBUS_THRESHOLD_20V            (17000u)
#define USBPD_PWR_VBUS_THRESHOLD_APDO           (2150u)

/**
  * @brief  VBUS discharge parameters
  */
#define USBPD_PWR_DISCHARGE_MARGIN              (500u)
#define USBPD_PWR_DISCHARGE_TIME                (6u)

/**
  * @brief  Calibration settings
  */
#define USBPD_PWR_DCDC_PRECISION                (20u)     /* DCDC output precision set to 20mV (Noise)*/
#define USBPD_PWR_CALIBRATION_ENABLED           (1u)
#define USBPD_PWR_CALIBRATION_DISABLED          (0u)

/**
  * @brief  Standard VBUS voltage levels
  */
#define USBPD_PWR_VBUS_5V                       5000u
#define USBPD_PWR_VBUS_9V                       9000u
#define USBPD_PWR_VBUS_15V                      15000u

/**
  * @brief  power timeout
  */
#define USBPD_PWR_TIMEOUT_PDO                   250u         /* Timeout for PDO to PDO or PDO to APDO at 250ms*/
#define USBPD_PWR_TIMEOUT_APDO                  25u          /* Timeout for APDO to APDO at 25ms*/

/**
  * @brief  Invalid value set during issue with voltage setting
  */
#define USBPD_PWR_INVALID_VALUE                 0xFFFFFFFFu

#define GPIO_SOURCE_EN_PIN                      GPIO_PIN_3  /* PD.03 */
#define GPIO_SOURCE_EN_PORT                     GPIOD
#define GPIO_SOURCE_EN_CLK_ENABLE()             __HAL_RCC_GPIOD_CLK_ENABLE()

#define GPIO_VBUS_DISCHARGE1_PIN                GPIO_PIN_13  /* PB.13 */
#define GPIO_VBUS_DISCHARGE1_PORT               GPIOB
#define GPIO_VBUS_DISCHARGE1_CLK_ENABLE()       __HAL_RCC_GPIOB_CLK_ENABLE()

#define GPIO_VBUS_DISCHARGE2_PIN                GPIO_PIN_14  /* PB.14 */
#define GPIO_VBUS_DISCHARGE2_PORT               GPIOB
#define GPIO_VBUS_DISCHARGE2_CLK_ENABLE()       __HAL_RCC_GPIOB_CLK_ENABLE()

#define GPIO_VCONN_EN1_PIN                      GPIO_PIN_4  /* PD.04 */
#define GPIO_VCONN_EN1_PORT                     GPIOD
#define GPIO_VCONN_EN1_CLK_ENABLE()             __HAL_RCC_GPIOD_CLK_ENABLE()

#define GPIO_VCONN_EN2_PIN                      GPIO_PIN_9  /* PB.09 */
#define GPIO_VCONN_EN2_PORT                     GPIOB
#define GPIO_VCONN_EN2_CLK_ENABLE()             __HAL_RCC_GPIOB_CLK_ENABLE()

#define GPIO_VCONN_DISCHARGE1_PIN               GPIO_PIN_2  /* PA.02 */
#define GPIO_VCONN_DISCHARGE1_PORT              GPIOA
#define GPIO_VCONN_DISCHARGE1_CLK_ENABLE()      __HAL_RCC_GPIOA_CLK_ENABLE()

#define GPIO_VCONN_DISCHARGE2_PIN               GPIO_PIN_0  /* PB.00 */
#define GPIO_VCONN_DISCHARGE2_PORT              GPIOB
#define GPIO_VCONN_DISCHARGE2_CLK_ENABLE()      __HAL_RCC_GPIOB_CLK_ENABLE()

#define GPIO_V_CTL2_PIN                         GPIO_PIN_1  /* PA.01 */
#define GPIO_V_CTL2_PORT                        GPIOA
#define GPIO_V_CTL2_CLK_ENABLE()                __HAL_RCC_GPIOA_CLK_ENABLE()

#define GPIO_V_CTL1_PIN                         GPIO_PIN_1  /* PC.01 */
#define GPIO_V_CTL1_PORT                        GPIOC
#define GPIO_V_CTL1_CLK_ENABLE()                __HAL_RCC_GPIOC_CLK_ENABLE()

/* Definition of ADCx clock resources */
#define ADCx                                    ADC1
#define ADCx_CLK_ENABLE()                       __HAL_RCC_ADC_CLK_ENABLE()
#define ADCx_CLK_DISABLE()                      __HAL_RCC_ADC_CLK_DISABLE()

#define ADCx_FORCE_RESET()                      __HAL_RCC_ADC_FORCE_RESET()
#define ADCx_RELEASE_RESET()                    __HAL_RCC_ADC_RELEASE_RESET()

/* Definition of ADCx channels */
#define ADCx_CHANNEL_VSENSE_1                   ADC_CHANNEL_9
#define ADCx_CHANNEL_ISENSE_1                   ADC_CHANNEL_11
#define ADCx_CHANNEL_VSENSE_2                   ADC_CHANNEL_3
#define ADCx_CHANNEL_ISENSE_2                   ADC_CHANNEL_16
#define ADCx_CHANNEL_VSENSE_DCDC                ADC_CHANNEL_15

/* Definition of ADCx channels */
#define ADCxChanneln 5u

#define ADCx_CHANNEL_VSENSE_1_GPIO_CLK_ENABLE() __HAL_RCC_GPIOB_CLK_ENABLE()
#define ADCx_CHANNEL_VSENSE_1_GPIO_PORT         GPIOB
#define ADCx_CHANNEL_VSENSE_1_GPIO_PIN          GPIO_PIN_1

#define ADCx_CHANNEL_ISENSE_1_GPIO_CLK_ENABLE() __HAL_RCC_GPIOB_CLK_ENABLE()
#define ADCx_CHANNEL_ISENSE_1_GPIO_PORT         GPIOB
#define ADCx_CHANNEL_ISENSE_1_GPIO_PIN          GPIO_PIN_10

#define ADCx_CHANNEL_VSENSE_2_GPIO_CLK_ENABLE() __HAL_RCC_GPIOA_CLK_ENABLE()
#define ADCx_CHANNEL_VSENSE_2_GPIO_PORT         GPIOA
#define ADCx_CHANNEL_VSENSE_2_GPIO_PIN          GPIO_PIN_3

#define ADCx_CHANNEL_ISENSE_2_GPIO_CLK_ENABLE() __HAL_RCC_GPIOB_CLK_ENABLE()
#define ADCx_CHANNEL_ISENSE_2_GPIO_PORT         GPIOB
#define ADCx_CHANNEL_ISENSE_2_GPIO_PIN          GPIO_PIN_12

#define ADCx_CHANNEL_VSENSE_DCDC_GPIO_CLK_ENABLE() __HAL_RCC_GPIOB_CLK_ENABLE()
#define ADCx_CHANNEL_VSENSE_DCDC_GPIO_PORT      GPIOB
#define ADCx_CHANNEL_VSENSE_DCDC_GPIO_PIN       GPIO_PIN_11

/* Definition of ADCx DMA resources */
#define ADCx_DMA_CLK_ENABLE()                   __HAL_RCC_DMA1_CLK_ENABLE()
#define ADCx_DMA                                DMA1_Channel1

/* Definition of DMA NVIC resources */
#define ADCx_DMA_IRQn                           DMA1_Channel1_IRQn
#define ADCx_DMA_IRQHandler                     DMA1_Channel1_IRQHandler

/* Definition of ADCx NVIC resources */
#define ADCx_IRQn                               ADC1_COMP_IRQn
#define ADCx_IRQHandler                         ADC1_COMP_IRQHandler

/**
  * @}
  */

/** @defgroup STM32G0C1E_EVAL_POWER_Exported_Functions Exported Functions based on BSP V1 specification (keep for legacy)
  * @{
  */
PWR_StatusTypeDef BSP_PWR_Init(uint32_t PortId);

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

/** @defgroup STM32G0C1E_EVAL_USBPD_POWER_Exported_Functions2 Exported Functions based on BSP V2 specification
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

int32_t BSP_USBPD_PWR_DCDCGetCtrlMode(uint32_t Instance, USBPD_PWR_DCDCCtrlModeTypeDef *pDCDCCtrl);

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

#endif /* STM32G0C1E_EVAL_POWER */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
