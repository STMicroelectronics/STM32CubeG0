/**
  ******************************************************************************
  * @file    k_widgets.h
  * @author  MCD Application Team
  * @brief   Widgets interface
  ******************************************************************************
  * Copyright (c) 2018 STMicroelectronics. All rights reserved.
  *
  * This software component is licensed by ST under Ultimate Liberty license
  * SLA0044, the "License"; You may not use this file except in compliance with
  * the License. You may obtain a copy of the License at:
  *                               www.st.com/SLA0044
  *
  ******************************************************************************
  */ 

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef _K_WIDGETS_H
#define _K_WIDGETS_H

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include <stdint.h>

/* Exported types ------------------------------------------------------------*/
/* Progress Bar descriptor */
typedef struct {
  uint16_t textColor;
  uint16_t backgroungColor;
  uint16_t xpos;
  uint16_t ypos;
  uint16_t width;
  uint16_t height;  
} kWidgetsProgressBar_t;

/* Exported constants --------------------------------------------------------*/
/* Exported variables --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
void * kWidgets_ProgressBarCreate(kWidgetsProgressBar_t * pProgressBar, uint32_t MaxValue);
void kWidgets_ProgressBarDestroy(void * hwidgetProgressBar);
void kWidgets_ProgressBarUpdate(void * hwidgetProgressBar, uint32_t Progress);
void kWidgets_ProgressBarReset(void * hwidgetProgressBar);

#ifdef __cplusplus
}
#endif

#endif /* _K_MEM_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
