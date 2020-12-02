/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    usbpd_dpm_user.h
  * @author  MCD Application Team
  * @brief   Header file for usbpd_dpm_user.c file
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2020 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under Ultimate Liberty license
  * SLA0044, the "License"; You may not use this file except in compliance with
  * the License. You may obtain a copy of the License at:
  *                             www.st.com/SLA0044
  *
  ******************************************************************************
  */
/* USER CODE END Header */

#ifndef __USBPD_DPM_USER_H_
#define __USBPD_DPM_USER_H_

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
/* USER CODE BEGIN Include */

/* USER CODE END Include */

/** @addtogroup STM32_USBPD_APPLICATION
  * @{
  */

/** @addtogroup STM32_USBPD_APPLICATION_DPM_USER
  * @{
  */

/* Exported typedef ----------------------------------------------------------*/
/* USER CODE BEGIN Typedef */

/* USER CODE END Typedef */

/* Exported define -----------------------------------------------------------*/
/* USER CODE BEGIN Define */

/* USER CODE END Define */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN Constant */

/* USER CODE END Constant */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN Macro */

/* USER CODE END Macro */

/* Exported variables --------------------------------------------------------*/
/* USER CODE BEGIN Private_Variables */

/* USER CODE END Private_Variables */

/* Exported functions --------------------------------------------------------*/
/** @addtogroup USBPD_USER_EXPORTED_FUNCTIONS
  * @{
  */
/** @addtogroup USBPD_USER_EXPORTED_FUNCTIONS_GROUP1
  * @{
  */
void                USBPD_DPM_UserExecute(void const *argument);
void                USBPD_DPM_UserCableDetection(uint8_t PortNum, USBPD_CAD_EVENT State);

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

#endif /* __USBPD_DPM_USER_H_ */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
