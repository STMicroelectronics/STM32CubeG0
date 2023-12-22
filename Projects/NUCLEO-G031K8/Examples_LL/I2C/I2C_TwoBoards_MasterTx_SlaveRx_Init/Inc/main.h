/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    Examples_LL/I2C/I2C_TwoBoards_MasterTx_SlaveRx_Init/Inc/main.h
  * @author  MCD Application Team
  * @brief   Header for main.c module
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2019-2021 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
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
#include "stm32g0xx_ll_i2c.h"
#include "stm32g0xx_ll_rcc.h"
#include "stm32g0xx_ll_bus.h"
#include "stm32g0xx_ll_system.h"
#include "stm32g0xx_ll_exti.h"
#include "stm32g0xx_ll_cortex.h"
#include "stm32g0xx_ll_utils.h"
#include "stm32g0xx_ll_pwr.h"
#include "stm32g0xx_ll_dma.h"
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
/* IRQ Handler treatment functions */
#ifndef SLAVE_BOARD
void UserButton_Callback(void);
#endif /* MASTER_BOARD */
void Error_Callback(void);
/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define LED3_Pin LL_GPIO_PIN_6
#define LED3_GPIO_Port GPIOC

/* USER CODE BEGIN Private defines */

/**
  * @brief Toggle periods for various blinking modes
  */

#define LED_BLINK_FAST  200
#define LED_BLINK_SLOW  500
#define LED_BLINK_ERROR 1000

/**
  * @brief Slave settings
  */
#define SLAVE_OWN_ADDRESS                       180 /* This value is a left shift of a real 7 bits of a slave address
                                                        value which can find in a Datasheet as example: b0101101
                                                        mean in uint8_t equivalent at 0x2D and this value can be
                                                        seen in the OAR1 register in bits OA1[1:7] */

/* Uncomment this line to use the board as slave, if not it is used as master */
//#define SLAVE_BOARD


/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
