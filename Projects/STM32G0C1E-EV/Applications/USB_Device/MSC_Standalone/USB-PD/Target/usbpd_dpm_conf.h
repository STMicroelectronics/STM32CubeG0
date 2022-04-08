/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    usbpd_dpm_conf.h
  * @author  MCD Application Team
  * @brief   Header file for stack/application settings file
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2018 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

#ifndef __USBPD_DPM_CONF_H_
#define __USBPD_DPM_CONF_H_

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/

/* USER CODE BEGIN Includes */
/* Section where include file can be added */

/* USER CODE END Includes */

/* Define   ------------------------------------------------------------------*/

/* Exported typedef ----------------------------------------------------------*/
/* USER CODE BEGIN Typedef */
/* Section where Typedef can be added */

/* USER CODE END Typedef */

/* Private variables ---------------------------------------------------------*/
#ifndef __USBPD_DPM_CORE_C
extern USBPD_SettingsTypeDef            DPM_Settings[USBPD_PORT_COUNT];
#else /* __USBPD_DPM_CORE_C */
const USBPD_SettingsTypeDef       DPM_Settings[USBPD_PORT_COUNT] =
{
  {
    .PE_DefaultRole = USBPD_PORTPOWERROLE_SNK,  /* Default port role                                       */
  }
};
#endif /* !__USBPD_DPM_CORE_C */

/* USER CODE END Variable */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN Constant */
/* Section where Constant can be added */

/* USER CODE END Constant */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN Macro */
/* Section where Macro can be added */

/* USER CODE END Macro */

#ifdef __cplusplus
}
#endif

#endif /* __USBPD_DPM_CONF_H_ */
