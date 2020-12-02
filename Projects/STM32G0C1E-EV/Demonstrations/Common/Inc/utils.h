/**
  ******************************************************************************
  * @file    utils.h
  * @author  MCD Application Team
  * @brief   System information functions
  ******************************************************************************
  * Copyright (c) 2020 STMicroelectronics. All rights reserved.
  *
  * This software component is licensed by ST under Ultimate Liberty license
  * SLA0044, the "License"; You may not use this file except in compliance with
  * the License. You may obtain a copy of the License at:
  *                               www.st.com/SLA0044
  *
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __UTILS_H
#define __UTILS_H

#ifdef __cplusplus
extern "C" {
#endif

  /* Includes ----------------------------------------------------------------*/
#include <stdint.h>

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/  
/* Exported macros -----------------------------------------------------------*/

/* Exported functions --------------------------------------------------------*/
void      UTILS_DisplayBuildOption(uint32_t Timeout);
#ifdef __cplusplus
}
#endif

#endif /* __UTILS_H */


/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/

