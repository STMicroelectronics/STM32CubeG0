/**
  ******************************************************************************
  * @file    main.h 
  * @author  MCD Application Team
  * @brief   Header for main.c file
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
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
 extern "C" {
#endif
   
/* Includes ------------------------------------------------------------------*/
#include <stdio.h>
#include <string.h>

#include "stm32g0xx.h"
  
/* Exported types ------------------------------------------------------------*/
typedef struct {
  uint32_t (*pfnSubDemoInit)(void);
  uint32_t (*pfnSubDemoDeInit)(void);
  void (*pfnSubDemoExec)(void);
  void (*pfnSubDemoExtiRisingEdgeDetectionManagement)(uint16_t GPIO_Pin);
  void (*pfnSubDemoExtiFallingEdgeDetectionManagement)(uint16_t GPIO_Pin);
} SubDemoApi_t;

typedef enum {
  SUB_DEMO_LEGACY = 0,
  SUB_DEMO_UCPD
} SubDemoId_e;
   
/* Exported variables --------------------------------------------------------*/
extern SubDemoId_e SubDemoId;
extern uint16_t DaughterBoardDependentExtiLine;
extern RTC_HandleTypeDef hrtc;
extern DAC_HandleTypeDef hdac;
extern ADC_HandleTypeDef hadc1;
extern DMA_HandleTypeDef hdma_adc1;

/* Exported constants --------------------------------------------------------*/
/* Exported macros -----------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */ 
void SystemClock_Config_64MHz(void);
void Error_Handler(void);



#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
