/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2020-2021 STMicroelectronics.
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
#include "stm32g0xx_hal.h"
#include "stm32g0xx_ll_dma.h"
#include "stm32g0xx_ll_rcc.h"
#include "stm32g0xx_ll_bus.h"
#include "stm32g0xx_ll_crs.h"
#include "stm32g0xx_ll_system.h"
#include "stm32g0xx_ll_exti.h"
#include "stm32g0xx_ll_cortex.h"
#include "stm32g0xx_ll_utils.h"
#include "stm32g0xx_ll_pwr.h"
#include "stm32g0xx_ll_ucpd.h"
#include "stm32g0xx_ll_usart.h"
#include "stm32g0xx_ll_gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "stm32g0c1e_eval.h"
#include "usbh_hid_mouse.h"
#include "usbh_hid.h"
#include "usbh_hid_parser.h"
/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

typedef enum {
  HID_DEMO_IDLE = 0,
  HID_DEMO_WAIT,  
  HID_DEMO_START,
  HID_DEMO_MOUSE,  
  HID_DEMO_KEYBOARD,
  HID_DEMO_REENUMERATE,
}HID_Demo_State;

typedef enum {
  HID_MOUSE_IDLE = 0,
  HID_MOUSE_WAIT, 
  HID_MOUSE_START,
}HID_mouse_State;

typedef enum {
  HID_KEYBOARD_IDLE = 0,
  HID_KEYBOARD_WAIT,  
  HID_KEYBOARD_START,    
}HID_keyboard_State;

typedef struct _DemoStateMachine {
  __IO HID_Demo_State     state;
  __IO HID_mouse_State    mouse_state;
  __IO HID_keyboard_State keyboard_state;   
  __IO uint8_t            select;
  __IO uint8_t            lock;
}HID_DEMO_StateMachine;
//---------------------------------------------
typedef enum {
  HID_APP_IDLE = 0,
  HID_APP_WAIT,
  HID_APP_MOUSE,
  HID_APP_KEYBOARD,
}HID_APP_State;

typedef enum {
  APPLICATION_IDLE = 0,
  APPLICATION_DISCONNECT,
  APPLICATION_START,
  APPLICATION_READY,
  APPLICATION_RUNNING,
}HID_ApplicationTypeDef;
//-------------------------------------------
/* USER CODE END ET */
/* Exported variables ------------------------------------------------------------*/
extern HID_ApplicationTypeDef Appli_state;
extern HID_MOUSE_Info_TypeDef mouse_info;
extern HID_DEMO_StateMachine hid_demo;

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

void HID_AppInit(void);
void HID_UpdateApp(void);
void MX_USB_HOST_Init(void);
/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define USBPD_FW_VERSION 0x16092020
#define VCONN_EN2_Pin LL_GPIO_PIN_9
#define VCONN_EN2_GPIO_Port GPIOB
#define V_CTL1_Pin LL_GPIO_PIN_2
#define V_CTL1_GPIO_Port GPIOC
#define VSENSE_2_Pin LL_GPIO_PIN_3
#define VSENSE_2_GPIO_Port GPIOA
#define VCONN_DISCHARGE2_Pin LL_GPIO_PIN_0
#define VCONN_DISCHARGE2_GPIO_Port GPIOB
#define VSENSE_1_Pin LL_GPIO_PIN_10
#define VSENSE_1_GPIO_Port GPIOB
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
void   MX_GPIO_Init(void);
void   MX_DMA_Init(void);
void   MX_UCPD1_Init(void);
void   MX_ADC1_Init(void);
void   MX_USART3_UART_Init(void);
void   MX_LPTIM1_Init(void);
/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
