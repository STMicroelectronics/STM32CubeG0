/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  *
  * Copyright (c) 2018 STMicroelectronics. All rights reserved.
  *
  * This software component is licensed by ST under Ultimate Liberty license
  * SLA0044, the "License"; You may not use this file except in compliance with
  * the License. You may obtain a copy of the License at:
  *                             www.st.com/SLA0044
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32g0xx_hal.h"
#include "stm32g0xx_ll_ucpd.h"
#include "stm32g0xx_ll_usart.h"
#include "stm32g0xx_ll_rcc.h"
#include "stm32g0xx_ll_system.h"
#include "stm32g0xx_ll_gpio.h"
#include "stm32g0xx_ll_exti.h"
#include "stm32g0xx_ll_bus.h"
#include "stm32g0xx_ll_cortex.h"
#include "stm32g0xx_ll_utils.h"
#include "stm32g0xx_ll_pwr.h"
#include "stm32g0xx_ll_dma.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define V_CTL1_Pin LL_GPIO_PIN_2
#define V_CTL1_GPIO_Port GPIOC
#define V_CTL2_Pin LL_GPIO_PIN_1
#define V_CTL2_GPIO_Port GPIOA
#define VCONN_DISCHARGE1_Pin LL_GPIO_PIN_2
#define VCONN_DISCHARGE1_GPIO_Port GPIOA
#define VSENSE_2_Pin LL_GPIO_PIN_3
#define VSENSE_2_GPIO_Port GPIOA
#define VCONN_DISCHARGE2_Pin LL_GPIO_PIN_0
#define VCONN_DISCHARGE2_GPIO_Port GPIOB
#define VSENSE_1_Pin LL_GPIO_PIN_1
#define VSENSE_1_GPIO_Port GPIOB
#define ISENSE_1_Pin LL_GPIO_PIN_10
#define ISENSE_1_GPIO_Port GPIOB
#define VSENSE_DCDC_Pin LL_GPIO_PIN_11
#define VSENSE_DCDC_GPIO_Port GPIOB
#define ISENSE_2_Pin LL_GPIO_PIN_12
#define ISENSE_2_GPIO_Port GPIOB
#define VBUS_DISCHARGE1_Pin LL_GPIO_PIN_13
#define VBUS_DISCHARGE1_GPIO_Port GPIOB
#define VBUS_DISCHARGE2_Pin LL_GPIO_PIN_14
#define VBUS_DISCHARGE2_GPIO_Port GPIOB
#define SOURCE_EN_Pin LL_GPIO_PIN_3
#define SOURCE_EN_GPIO_Port GPIOD
#define VCONN_EN1_Pin LL_GPIO_PIN_4
#define VCONN_EN1_GPIO_Port GPIOD
#define VCONN_EN2_Pin LL_GPIO_PIN_9
#define VCONN_EN2_GPIO_Port GPIOB
/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
