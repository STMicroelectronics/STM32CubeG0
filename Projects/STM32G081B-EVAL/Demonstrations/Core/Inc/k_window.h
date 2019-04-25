/**
  ******************************************************************************
  * @file    k_window.h
  * @author  MCD Application Team
  * @brief   Header for k_window.c file
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
#ifndef _K_WINDOW_H
#define _K_WINDOW_H

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include <stdint.h>

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported variables --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/

   /* Exported functions ------------------------------------------------------- */
void kWindow_Error(char *msg);
void kWindow_Popup(char *title, uint16_t title_tc, uint16_t title_bc ,char *Msg, uint16_t msg_tc, uint16_t msg_bc);


#ifdef __cplusplus
}
#endif

#endif /* _K_WINDOW_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
