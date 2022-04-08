/**
  ******************************************************************************
  * @file    k_config.h
  * @author  MCD Application Team
  * @brief   Header for k_config.c file
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2020 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef K_CONFIG_H
#define K_CONFIG_H

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "main.h"
/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported macros -----------------------------------------------------------*/
#define EVAL_BOARD   "STM32G0C1E-EV"
#define DEMO_VERSION "Version 1.4.0"
#define DEMO_DATE    "03 - 11 - 2020"

#define DEMO_INFO1   "MCD Application Team"
#define DEMO_INFO2   "COPYRIGHT 2020"
#define DEMO_INFO3   "STMicroelectronics"

#define CHOOSEFILE_MAXLEN   255

typedef enum{
 MODULE_MAIN_APP,
 MODULE_LOWPOWER,
 MODULE_TSENSOR,
 MODULE_VIEWBMP,
 MODULE_FILESBRO,
 MODULE_SYSTEM_INFO,
 MODULE_APPLICATIONS,
 MODULE_AUDIO,
 MODULE_CALENDAR,
 MODULE_HELP,
 MODULE_MAX,
 MODULE_NONE = 0xFF
} MODULES_INFO;

#define countof(a) (sizeof(a) / sizeof(*(a)))
/* Exported functions ------------------------------------------------------- */

#ifdef __cplusplus
}
#endif

#endif /* K_LOG_H */
