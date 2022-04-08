/**
  ******************************************************************************
  * @file    stm32g0xx_it.h 
  * @author  MCD Application Team
  * @brief   This file contains the headers of the interrupt handlers.
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
#ifndef STM32G0xx_IT_H
#define STM32G0xx_IT_H

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32g0c1e_eval_mux.h"
#if defined(_TRACE)
#include "tracer_emb_conf.h"
#endif/*_TRACE */

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */

void NMI_Handler(void);
void HardFault_Handler(void);
void SysTick_Handler(void);

void RTC_TAMP_IRQHandler(void);
void EXTI4_15_IRQHandler(void);
void EXTI2_3_IRQHandler(void);
void EXTI0_1_IRQHandler(void);
void USB_UCPD1_2_IRQHandler(void);
void DEBOUNCE_TIM_IRQHandler(void);
void DMA1_Channel1_IRQHandler(void);
void ADC1_COMP_IRQHandler(void);
#if defined(_TRACE)
void TRACER_EMB_USART_IRQHANDLER(void);
void TRACER_EMB_TX_DMA_IRQHANDLER(void);
#endif/*_TRACE */

#ifdef __cplusplus
}
#endif

#endif/* STM32G0xx_IT_H */
