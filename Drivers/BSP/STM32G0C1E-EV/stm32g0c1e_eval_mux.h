/**
  ******************************************************************************
  * @file    stm32g0c1e_eval_mux.h
  * @author  MCD Application Team
  * @brief   This file contains all the functions prototypes for the
  *          stm32g0c1e_eval_mux.c firmware driver.
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

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef STM32G0C1E_EVAL_MUX_H
#define STM32G0C1E_EVAL_MUX_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32g0xx.h"
#include "../Components/sn65dp141/sn65dp141.h"
#include "../Components/cbtl08gp053/cbtl08gp053.h"
#include "../Components/tusb546/tusb546.h"

/** @addtogroup BSP
  * @{
  */

/** @addtogroup STM32G0C1E_EVAL
  * @{
  */

/** @defgroup STM32G0C1E_EVAL_MUX STM32G0C1E_EVAL MUX
  * @{
  */

/** @defgroup STM32G0C1E_EVAL_MUX_Exported_Defines Exported Defines
  * @{
  */

#define MUX_1_EQGAIN_MAX  TUSB546_EQGAIN_15
#define MUX_2_EQGAIN_MAX  SN65DP141_EQGAIN_7

/**
  * @}
  */

/** @defgroup STM32G0C1E_EVAL_MUX_Exported_Types Exported Types
  * @{
  */

/**
  * @brief  MUX Status
  */
typedef enum
{
    MUX_OK = 0,
    MUX_ERROR
} MUX_StatusTypeDef;

/**
  * @brief  Type-C MUX identifier
  */
typedef enum
{
    TYPE_C_MUX_1 = 0,
    TYPE_C_MUX_2,
    TYPE_C_MUX_NB
} MUX_TypeCMuxIdTypeDef;

/**
  * @brief  Type-C connector pin assignments
  */
typedef enum
{
    DFP_PIN_ASSIGNMMENT_A = 0, /*!< USB Type-C to USB Type-C or Protocol Converter */
    DFP_PIN_ASSIGNMMENT_B,     /*!< USB Type-C to USB Type-C or Protocol Converter */
    DFP_PIN_ASSIGNMMENT_C,     /*!< USB Type-C to USB Type-C or Protocol Converter */
    DFP_PIN_ASSIGNMMENT_D,     /*!< USB Type-C to USB Type-C or Protocol Converter */
    DFP_PIN_ASSIGNMMENT_E,     /*!< USB Type-C to DisplayPort                      */
    DFP_PIN_ASSIGNMMENT_F,     /*!< USB Type-C to DisplayPort                      */
    UFP_PIN_ASSIGNMMENT_A,     /*!< USB Type-C to USB Type-C or Protocol Converter */
    UFP_PIN_ASSIGNMMENT_B,     /*!< USB Type-C to USB Type-C or Protocol Converter */
    UFP_PIN_ASSIGNMMENT_C,     /*!< USB Type-C to USB Type-C or Protocol Converter */
    UFP_PIN_ASSIGNMMENT_D,     /*!< USB Type-C to USB Type-C or Protocol Converter */
    UFP_PIN_ASSIGNMMENT_E,     /*!< USB Type-C to USB Type-C or Protocol Converter */
    UFP_PIN_ASSIGNMMENT_F,     /*!< USB Type-C to USB Type-C or Protocol Converter */
    USB_ONLY_PIN_ASSIGNMMENT   /*!< USB 3.1 Only                                   */
} MUX_TypeCConnectorPinAssignmentTypeDef;

/**
  * @brief  Type-C port plug orientation
  */
typedef enum
{
    PLUG_ORIENTATION_NORMAL = 0,
    PLUG_ORIENTATION_FLIPPED
} MUX_TypeCPlugOrientationTypeDef;

/**
  * @brief  HDP (Hot Plug Detection) state
  */
typedef enum
{
    HPD_STATE_LOW = 0,
    HPD_STATE_HIGH
} MUX_HPDStateTypeDef;

/**
  * @brief HPD Callback Function Pointer
  */
typedef void MUX_HPDCallbackFuncTypeDef(MUX_TypeCMuxIdTypeDef TypeCMuxId, MUX_HPDStateTypeDef HPDState);

/**
  * @}
  */

/** @defgroup STM32G0C1E_EVAL_MUX_Exported_Constants Exported Constants
  * @{
  */
#define DETn                                  2

#define DET_HPD_SOURCE_PIN                    GPIO_PIN_6  /* PC.06 */
#define DET_HPD_SOURCE_GPIO_PORT              GPIOC
#define DET_HPD_SOURCE_GPIO_CLK_ENABLE()      __HAL_RCC_GPIOC_CLK_ENABLE()
#define DET_HPD_SOURCE_GPIO_CLK_DISABLE()     __HAL_RCC_GPIOC_CLK_DISABLE()

#define DET_EXTI_IRQn                          EXTI4_15_IRQn
#define DET_EXTI_IRQHandler                    EXTI4_15_IRQHandler

#define SELn                                  1

#define SEL_HPDIN_PIN                         GPIO_PIN_5  /* PB.05 */
#define SEL_HPDIN_GPIO_PORT                   GPIOB
#define SEL_HPDIN_GPIO_CLK_ENABLE()           __HAL_RCC_GPIOB_CLK_ENABLE()
#define SEL_HPDIN_GPIO_CLK_DISABLE()          __HAL_RCC_GPIOB_CLK_DISABLE()

#define DEBOUNCE_TIM_INSTANCE                 TIM6
#define DEBOUNCE_TIM_CLK_ENABLE()             __HAL_RCC_TIM6_CLK_ENABLE()
#define DEBOUNCE_TIM_CLK_DISABLE()            __HAL_RCC_TIM6_CLK_DISABLE()
#define DEBOUNCE_TIM_COUNTER_CLK_FREQ()       HAL_RCC_GetPCLK1Freq()
#define DEBOUNCE_TIM_IRQn                     TIM6_DAC_LPTIM1_IRQn
#define DEBOUNCE_TIME                         1000u                        /* 1ms */
#define DEBOUNCE_TIM_IRQHandler               TIM6_DAC_LPTIM1_IRQHandler
/**
  * @}
  */

/** @defgroup STM32G0C1E_EVAL_MUX_Exported_Variables Exported Variables
  * @{
  */
extern TIM_HandleTypeDef htim;
/**
  * @}
  */

/** @defgroup STM32G0C1E_EVAL_MUX_Exported_Functions Exported Functions
  * @{
  */
MUX_StatusTypeDef BSP_MUX_Init(MUX_TypeCMuxIdTypeDef TypeCMuxId);

MUX_StatusTypeDef BSP_MUX_DeInit(MUX_TypeCMuxIdTypeDef TypeCMuxId);

MUX_StatusTypeDef BSP_MUX_Enable(MUX_TypeCMuxIdTypeDef TypeCMuxId);

MUX_StatusTypeDef BSP_MUX_Disable(MUX_TypeCMuxIdTypeDef TypeCMuxId);

MUX_StatusTypeDef BSP_MUX_SetDPPinAssignment(MUX_TypeCMuxIdTypeDef                  TypeCMuxId,
                                             MUX_TypeCPlugOrientationTypeDef        TypeCPlugOrientation,
                                             MUX_TypeCConnectorPinAssignmentTypeDef TypeCConnectorPinAssignment);

MUX_StatusTypeDef BSP_MUX_SetEQGain(MUX_TypeCMuxIdTypeDef                  TypeCMuxId,
                                             uint8_t                     EQGain);

MUX_StatusTypeDef BSP_MUX_SetHPDState(MUX_TypeCMuxIdTypeDef  TypeCMuxId,
                                      MUX_HPDStateTypeDef    HPDState);

MUX_StatusTypeDef BSP_MUX_GetHPDState(MUX_TypeCMuxIdTypeDef  TypeCMuxId,
                                      MUX_HPDStateTypeDef *  pHPDState);

MUX_StatusTypeDef BSP_MUX_HPDIRQ(MUX_TypeCMuxIdTypeDef  TypeCMuxId);

void              BSP_MUX_Detect_HPD(void);

MUX_StatusTypeDef BSP_MUX_RegisterHPDCallbackFunc(MUX_TypeCMuxIdTypeDef        TypeCMuxId,
                                                  MUX_HPDCallbackFuncTypeDef * pHPDCallbackFunc);

#if defined(TUSB546_DEBUG) || defined(CBTL08GP053_DEBUG) || defined(SN65DP141_DEBUG)
#define DEVICE_CBTL08GP053 0x00000001U
#define DEVICE_SN65DP141   0x00000002U
#define DEVICE_TUSB546     0x00000004U
void BSP_MUX_DumpDeviceRegisters(uint32_t Device);
#endif /* TUSB546_DEBUG || CBTL08GP053_DEBUG || SN65DP141_DEBUG */

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

#endif /* STM32G0C1E_EVAL_MUX_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
