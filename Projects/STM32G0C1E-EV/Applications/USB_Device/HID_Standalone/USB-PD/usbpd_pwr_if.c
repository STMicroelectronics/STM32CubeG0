/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    usbpd_pwr_if.c
  * @author  MCD Application Team
  * @brief   This file contains power interface control functions.
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

#define __USBPD_PWR_IF_C

/* Includes ------------------------------------------------------------------*/
#include "usbpd_pwr_if.h"
#include "usbpd_hw_if.h"
#include "usbpd_dpm_core.h"
#include "usbpd_dpm_conf.h"
#include "usbpd_core.h"
/* USER CODE BEGIN Include */

/* USER CODE END Include */

/** @addtogroup STM32_USBPD_APPLICATION
  * @{
  */

/** @addtogroup STM32_USBPD_APPLICATION_POWER_IF
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/** @addtogroup STM32_USBPD_APPLICATION_POWER_IF_Private_TypeDef
  * @{
  */
/* USER CODE BEGIN Private_Typedef */

/* USER CODE END Private_Typedef */
/**
  * @}
  */

/* Private define ------------------------------------------------------------*/
/** @addtogroup STM32_USBPD_APPLICATION_POWER_IF_Private_Defines
  * @{
  */
/* USER CODE BEGIN Private_Define */

/* USER CODE END Private_Define */
/**
  * @}
  */

/* Private macros ------------------------------------------------------------*/
/** @addtogroup STM32_USBPD_APPLICATION_POWER_IF_Private_Macros
  * @{
  */
/* USER CODE BEGIN Private_Macro */

/* USER CODE END Private_Macro */
/**
  * @}
  */

/* Private variables ---------------------------------------------------------*/
/** @addtogroup STM32_USBPD_APPLICATION_POWER_IF_Private_Variables
  * @{
  */
/* USER CODE BEGIN Private_Variables */
/**
  * @brief  USBPD Port PDO Storage array declaration
  */

/* USER CODE END Private_Variables */
/**
  * @}
  */

/* Private function prototypes -----------------------------------------------*/
/** @addtogroup STM32_USBPD_APPLICATION_POWER_IF_Private_Functions
  * @{
  */
/* USER CODE BEGIN USBPD_USER_PRIVATE_FUNCTIONS_Prototypes */

/* USER CODE END USBPD_USER_PRIVATE_FUNCTIONS_Prototypes */
/**
  * @}
  */

/** @addtogroup STM32_USBPD_APPLICATION_POWER_IF_Exported_Functions
  * @{
  */
/**
  * @brief Function is called to get VBUS power status.
  * @param PortNum Port number
  * @param PowerTypeStatus  Power type status based on @ref USBPD_VBUSPOWER_STATUS
  * @retval UBBPD_TRUE or USBPD_FALSE
  */
uint8_t USBPD_PWR_IF_GetVBUSStatus(uint8_t PortNum, USBPD_VBUSPOWER_STATUS PowerTypeStatus)
{
/* USER CODE BEGIN USBPD_PWR_IF_GetVBUSStatus */
  uint8_t _status = USBPD_FALSE;
  uint32_t _vbus = HW_IF_PWR_GetVoltage(PortNum);

  switch(PowerTypeStatus)
  {
  case USBPD_PWR_BELOWVSAFE0V :
    if (_vbus < USBPD_PWR_LOW_VBUS_THRESHOLD) _status = USBPD_TRUE;
    break;
  case USBPD_PWR_VSAFE5V :
    if (_vbus >= USBPD_PWR_HIGH_VBUS_THRESHOLD) _status = USBPD_TRUE;
    break;
  case USBPD_PWR_SNKDETACH:
    if (_vbus < USBPD_PWR_HIGH_VBUS_THRESHOLD) _status = USBPD_TRUE;
    break;
  default :
    break;
  }
  return _status;
/* USER CODE END USBPD_PWR_IF_GetVBUSStatus */
}

/**
  * @brief Function is called to set the VBUS threshold when a request has been accepted.
  * @param PortNum Port number
  * @retval None
  */
void USBPD_PWR_IF_UpdateVbusThreshold(uint8_t PortNum)
{
/* USER CODE BEGIN USBPD_PWR_IF_UpdateVbusThreshold */
/* USER CODE END USBPD_PWR_IF_UpdateVbusThreshold */
}

/**
  * @brief Function is called to reset the VBUS threshold when there is a power reset.
  * @param PortNum Port number
  * @retval None
  */
void USBPD_PWR_IF_ResetVbusThreshold(uint8_t PortNum)
{
/* USER CODE BEGIN USBPD_PWR_IF_ResetVbusThreshold */
/* USER CODE END USBPD_PWR_IF_ResetVbusThreshold */
}

/**
  * @}
  */

/** @addtogroup STM32_USBPD_APPLICATION_POWER_IF_Private_Functions
  * @{
  */
/* USER CODE BEGIN USBPD_USER_PRIVATE_FUNCTIONS_Definition */

/* USER CODE END USBPD_USER_PRIVATE_FUNCTIONS_Definition */
/**
  * @}
  */

/**
  * @}
  */

/**
  * @}
  */
