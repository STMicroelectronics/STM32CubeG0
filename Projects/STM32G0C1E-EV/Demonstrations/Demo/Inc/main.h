/**
  ******************************************************************************
  * @file    main.h 
  * @author  MCD Application Team
  * @brief   Header for main.c file
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
