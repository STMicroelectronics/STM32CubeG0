/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    Examples_MIX/ADC/ADC_SingleConversion_TriggerSW_IT/Inc/main.h
  * @author  MCD Application Team
  * @brief   Header for main.c module
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
/* USER CODE END Header */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32g0xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "stm32g0xx_ll_adc.h"
#include "stm32g0xx_ll_bus.h"
#include "stm32g0xx_ll_rcc.h"
#include "stm32g0xx_ll_system.h"
#include "stm32g0xx_ll_utils.h"
#include "stm32g0xx_ll_pwr.h"
#include "stm32g0xx_nucleo.h"
#include "stm32g0xx_ll_dac.h"
/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* Waveform voltage generation for test:                                      */
/*  - If this literal is defined: For this example purpose, generates a       */
/*    waveform voltage on a spare DAC channel, so user has just to connect    */
/*    a wire between DAC channel output and ADC input to run this example.    */
/*    (this avoid to user the need of an external signal voltage generator).  */
/*  - If this literal is not defined: User has to connect an external signal  */
/*    voltage generator on the selected ADC input to run this example.        */
#define WAVEFORM_VOLTAGE_GENERATION_FOR_TEST


/* User can use this section to tailor ADCx instance under use and associated
   resources */

/* ## Definition of ADC related resources ################################### */
/* Definition of ADCx clock resources */
#define ADCx                            ADC1
#define ADCx_CLK_ENABLE()               __HAL_RCC_ADC_CLK_ENABLE()

#define ADCx_FORCE_RESET()              __HAL_RCC_ADC_FORCE_RESET()
#define ADCx_RELEASE_RESET()            __HAL_RCC_ADC_RELEASE_RESET()

/* Definition of ADCx channels */
#define ADCx_CHANNELa                   ADC_CHANNEL_4

/* Definition of ADCx NVIC resources */
#define ADCx_IRQn                       ADC1_COMP_IRQn
#define ADCx_IRQHandler                 ADC1_COMP_IRQHandler

/* Definition of ADCx channels pins */
#define ADCx_CHANNELa_GPIO_CLK_ENABLE() __HAL_RCC_GPIOA_CLK_ENABLE()
#define ADCx_CHANNELa_GPIO_PORT         GPIOA
#define ADCx_CHANNELa_PIN               GPIO_PIN_4


/* ## Definition of DAC related resources ################################### */
/* Note: Definition will be used only if literal "WAVEFORM_VOLTAGE_GENERATION_FOR_TEST" is defined */
/* Definition of DACx clock resources */
#define DACx                            DAC
#define DACx_CLK_ENABLE()               __HAL_RCC_DAC1_CLK_ENABLE()
#define DACx_CHANNEL_GPIO_CLK_ENABLE()  __HAL_RCC_GPIOA_CLK_ENABLE()

#define DACx_FORCE_RESET()              __HAL_RCC_DAC1_FORCE_RESET()
#define DACx_RELEASE_RESET()            __HAL_RCC_DAC1_RELEASE_RESET()

/* Definition of DACx channels */
#define DACx_CHANNEL_TO_ADCx_CHANNELa            DAC_CHANNEL_1

/* Definition of DACx channels pins */
#define DACx_CHANNEL_TO_ADCx_CHANNELa_PIN        GPIO_PIN_4
#define DACx_CHANNEL_TO_ADCx_CHANNELa_GPIO_PORT  GPIOA

/* Definition of DACx NVIC resources */
#define DACx_IRQn                       TIM6_DAC_LPTIM1_IRQn
#define DACx_IRQHandler                 TIM6_DAC_LPTIM1_IRQHandler

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* IRQ Handler treatment */
void AdcGrpRegularUnitaryConvComplete_Callback(void);
void AdcGrpRegularOverrunError_Callback(void);

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
