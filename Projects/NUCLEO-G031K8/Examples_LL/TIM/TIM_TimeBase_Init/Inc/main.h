/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    Examples_LL/TIM/TIM_TimeBase_Init/Inc/main.h
  * @author  MCD Application Team
  * @brief   Header for main.c module
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2019 STMicroelectronics. 
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the 
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
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
#include "stm32g0xx_ll_rcc.h"
#include "stm32g0xx_ll_bus.h"
#include "stm32g0xx_ll_system.h"
#include "stm32g0xx_ll_exti.h"
#include "stm32g0xx_ll_cortex.h"
#include "stm32g0xx_ll_utils.h"
#include "stm32g0xx_ll_pwr.h"
#include "stm32g0xx_ll_dma.h"
#include "stm32g0xx_ll_tim.h"
#include "stm32g0xx_ll_gpio.h"

#if defined(USE_FULL_ASSERT)
#include "stm32_assert.h"
#endif /* USE_FULL_ASSERT */

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */
/**
  * @brief User Button
  */
#define VIRTUAL_BUTTON_PIN                         LL_GPIO_PIN_15
#define VIRTUAL_BUTTON_GPIO_PORT                   GPIOA
#define VIRTUAL_BUTTON_GPIO_CLK_ENABLE()           LL_IOP_GRP1_EnableClock(LL_IOP_GRP1_PERIPH_GPIOA)
#define VIRTUAL_BUTTON_EXTI_LINE                   LL_EXTI_LINE_15
#define VIRTUAL_BUTTON_EXTI_IRQn                   EXTI4_15_IRQn
#define VIRTUAL_BUTTON_EXTI_LINE_ENABLE()          LL_EXTI_EnableIT_0_31(VIRTUAL_BUTTON_EXTI_LINE)
#define VIRTUAL_BUTTON_EXTI_RISING_TRIG_ENABLE()   LL_EXTI_EnableRisingTrig_0_31(VIRTUAL_BUTTON_EXTI_LINE)
#define VIRTUAL_BUTTON_EXTI_IS_ACTIVE_FLAG()       LL_EXTI_IsActiveRisingFlag_0_31(VIRTUAL_BUTTON_EXTI_LINE)
#define VIRTUAL_BUTTON_EXTI_CLEAR_FLAG()           LL_EXTI_ClearRisingFlag_0_31(VIRTUAL_BUTTON_EXTI_LINE)
#define VIRTUAL_BUTTON_SYSCFG_SET_EXTI()           do {\
                                                  LL_EXTI_SetEXTISource(LL_EXTI_CONFIG_PORTA, LL_EXTI_CONFIG_LINE15);\
                                                } while(0)
#define VIRTUAL_BUTTON_IRQHANDLER                  EXTI4_15_IRQHandler

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */
/* IRQ Handler treatment.*/
void UserButton_Callback(void);

/* TIM1 update interrupt processing */
void TimerUpdate_Callback(void);
/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define LED3_Pin LL_GPIO_PIN_6
#define LED3_GPIO_Port GPIOC

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
