/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    usbpd_dpm_user.c
  * @author  MCD Application Team
  * @brief   USBPD DPM user code
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

#define USBPD_DPM_USER_C
/* Includes ------------------------------------------------------------------*/
#include "stm32g0c1e_eval_pwr.h"
#include "main.h"
#include "usbpd_pwr_if.h"
#include "usbpd_core.h"
#include "usbpd_dpm_user.h"
#if defined(_TRACE)
#include "usbpd_trace.h"
#include "string.h"
#include "stdio.h"
#endif /* _TRACE */
#include "usbpd_usb_if.h"
    
/** @addtogroup STM32_USBPD_APPLICATION
  * @{
  */

/** @addtogroup STM32_USBPD_APPLICATION_DPM_USER
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN Private_Typedef */

/* USER CODE END Private_Typedef */

/* Private define ------------------------------------------------------------*/
/** @defgroup USBPD_USER_PRIVATE_DEFINES USBPD USER Private Defines
  * @{
  */
/* USER CODE BEGIN Private_Define */

/* USER CODE END Private_Define */

/**
  * @}
  */

/* Private macro -------------------------------------------------------------*/
/** @defgroup USBPD_USER_PRIVATE_MACROS USBPD USER Private Macros
  * @{
  */
#if defined(_TRACE)
#define DPM_USER_DEBUG_TRACE_SIZE       50u
#define DPM_USER_DEBUG_TRACE(_PORT_, ...)  do {                                                                \
      char _str[DPM_USER_DEBUG_TRACE_SIZE];                                                                    \
      uint8_t _size = snprintf(_str, DPM_USER_DEBUG_TRACE_SIZE, __VA_ARGS__);                                  \
      if (_size < DPM_USER_DEBUG_TRACE_SIZE)                                                                   \
        USBPD_TRACE_Add(USBPD_TRACE_DEBUG, (uint8_t)(_PORT_), 0, (uint8_t*)_str, strlen(_str));                \
      else                                                                                                     \
        USBPD_TRACE_Add(USBPD_TRACE_DEBUG, (uint8_t)(_PORT_), 0, (uint8_t*)_str, DPM_USER_DEBUG_TRACE_SIZE);   \
  } while(0)

#else
#define DPM_USER_DEBUG_TRACE(_PORT_, ...)
#endif /* _TRACE */
/* USER CODE BEGIN Private_Macro */

/* USER CODE END Private_Macro */
/**
  * @}
  */

/* Private variables ---------------------------------------------------------*/
/** @defgroup USBPD_USER_PRIVATE_VARIABLES USBPD USER Private Variables
  * @{
  */
/* USER CODE BEGIN Private_Variables */

/* USER CODE END Private_Variables */
/**
  * @}
  */

/* Private function prototypes -----------------------------------------------*/
/** @defgroup USBPD_USER_PRIVATE_FUNCTIONS USBPD USER Private Functions
  * @{
  */
/* USER CODE BEGIN USBPD_USER_PRIVATE_FUNCTIONS_Prototypes */

/* USER CODE END USBPD_USER_PRIVATE_FUNCTIONS_Prototypes */
/**
  * @}
  */

/* Exported functions ------- ------------------------------------------------*/
/** @defgroup USBPD_USER_EXPORTED_FUNCTIONS USBPD USER Exported Functions
  * @{
  */
/* USER CODE BEGIN USBPD_USER_EXPORTED_FUNCTIONS */

/* USER CODE END USBPD_USER_EXPORTED_FUNCTIONS */

/** @defgroup USBPD_USER_EXPORTED_FUNCTIONS_GROUP1 USBPD USER Exported Functions called by DPM CORE
  * @{
  */
/* USER CODE BEGIN USBPD_USER_EXPORTED_FUNCTIONS_GROUP1 */

/* USER CODE END USBPD_USER_EXPORTED_FUNCTIONS_GROUP1 */

/**
  * @brief  Initialize DPM (port power role, PWR_IF, CAD and PE Init procedures)
  * @retval USBPD Status
  */

/**
  * @brief  User processing time, it is recommended to avoid blocking task for long time
  * @param  argument  DPM User event
  * @retval None
  */
void USBPD_DPM_UserExecute(void const *argument)
{
/* USER CODE BEGIN USBPD_DPM_UserExecute */

/* USER CODE END USBPD_DPM_UserExecute */
}

/**
  * @brief  UserCableDetection reporting events on a specified port from CAD layer.
  * @param  PortNum The handle of the port
  * @param  State CAD state
  * @retval None
  */
void USBPD_DPM_UserCableDetection(uint8_t PortNum, USBPD_CAD_EVENT State)
{
/* USER CODE BEGIN USBPD_DPM_UserCableDetection */
  switch(State)
  {
  case USBPD_CAD_EVENT_ATTACHED:
  case USBPD_CAD_EVENT_ATTEMC:
    {
      printf("Type C attached\n");
      USBPD_PWR_IF_VBUSEnable(PortNum);
      break;
    }
  case USBPD_CAD_EVENT_DETACHED :
  case USBPD_CAD_EVENT_EMC :
  default :
    {
      printf("Type C detached\n");
      USBPD_PWR_IF_VBUSDisable(PortNum);
      break;
    }
  }
/* USER CODE END USBPD_DPM_UserCableDetection */
}
/**
  * @brief  Callback function called by PE to inform DPM about PE event.
  * @param  PortNum The current port number
  * @param  EventVal @ref USBPD_NotifyEventValue_TypeDef
  * @retval None
  */
void USBPD_DPM_Notification(uint8_t PortNum, USBPD_NotifyEventValue_TypeDef EventVal)
{
  switch(EventVal)
  {
      
    /*
     *  USB management
     */
    case USBPD_NOTIFY_USBSTACK_START:
      {
        /* start device stack */
        USBPD_USBIF_HostStart(PortNum);
        break;
      }
    case USBPD_NOTIFY_USBSTACK_STOP:
      {
        /* stop device stack */
        USBPD_USBIF_HostStop(PortNum);
        break;
      }
    /*************************** end USB management *****************************/

    default:
            break;
  }
}
/**
  * @}
  */

/** @addtogroup USBPD_USER_PRIVATE_FUNCTIONS
  * @{
  */

/* USER CODE BEGIN USBPD_USER_PRIVATE_FUNCTIONS */

/* USER CODE END USBPD_USER_PRIVATE_FUNCTIONS */

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
