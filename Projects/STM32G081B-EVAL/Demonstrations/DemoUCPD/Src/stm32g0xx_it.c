/**
  ******************************************************************************
  * @file    stm32g0xx_it.c
  * @author  MCD Application Team
  * @brief   Main Interrupt Service Routines.
  *          This file provides template for all exceptions handler and
  *          peripherals interrupt service routine.
  ******************************************************************************
   * @attention
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

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "stm32g0xx.h"
#include "stm32g0xx_hal.h"
#include "usbpd_pwr_if.h"
#include "stm32g081b_eval.h"
#include "demo_application.h"
#include "stm32g081b_eval_mux.h"
#include "tracer_emb.h"
#include "stm32g0xx_it.h"

/** @addtogroup STM32G0xx_HAL_Examples
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
extern ADC_HandleTypeDef    hadc;

/******************************************************************************/
/*            Cortex-M0 Processor Exceptions Handlers                         */
/******************************************************************************/

/**
  * @brief  This function handles NMI exception.
  * @param  None
  * @retval None
  */
void NMI_Handler(void)
{
  /* Go to infinite loop when Hard Fault exception occurs */
  USBPD_PWR_IF_Alarm();
  while (1)
  {
  }
}

/**
  * @brief  This function handles Hard Fault exception.
  * @param  None
  * @retval None
  */
void HardFault_Handler(void)
{
  /* Go to infinite loop when Hard Fault exception occurs */
  USBPD_PWR_IF_Alarm();
  while (1)
  {
  }
}

/**
  * @brief  This function handles SysTick Handler.
  * @param  None
  * @retval None
  */
void SysTick_Handler(void)
{
  HAL_IncTick();
  USBPD_DPM_TimerCounter();
}

/******************************************************************************/
/*                 STM32G0xx Peripherals Interrupt Handlers                   */
/*  Add here the Interrupt Handler for the used peripheral(s) (PPP), for the  */
/*  available peripheral interrupt handler's name please refer to the startup */
/*  file (startup_stm32g0xx.s).                                               */
/******************************************************************************/
/**
  * @brief  This function handles ADC interrupt request.
  * @param  None
  * @retval None
  */
void ADC1_COMP_IRQHandler(void)
{
  HAL_ADC_IRQHandler(&hadc);
}

/**
* @brief  This function handles DMA interrupt request.
* @param  None
* @retval None
*/
void DMA1_Channel1_IRQHandler(void)
{
  HAL_DMA_IRQHandler(hadc.DMA_Handle);
}

/**
* @brief  This function handles DMA channel 4 to 7 interrupt request.
* @param  None
* @retval None
*/
void DMA1_Ch4_7_DMAMUX1_OVR_IRQHandler(void)
{
   TRACER_EMB_IRQHandlerDMA();
}


/**
  * @brief  This function handles UCPD interrupt.
  * @param  None
  * @retval None
  */
void UCPD1_2_IRQHandler(void)
{
  USBPD_PORT0_IRQHandler();
#if USBPD_PORT_COUNT == 2
  USBPD_PORT1_IRQHandler();
#endif
}

/**
  * @brief  This function handles USART 3 and 4 interrupts.
  * @param  None
  * @retval None
  */
void USART3_4_LPUART1_IRQHandler(void)
{
  TRACER_EMB_IRQHandlerUSART();
}

/**
  * @brief  This function handles EXTI 0 and 1 interrupts.
  * @param  None
  * @retval None
  */
void EXTI0_1_IRQHandler(void)
{
  HAL_GPIO_EXTI_IRQHandler(SEL_JOY_PIN);
}

/**
  * @brief  This function handles EXTI 2 and 3 interrupts.
  * @param  None
  * @retval None
  */
void EXTI2_3_IRQHandler(void)
{
  HAL_GPIO_EXTI_IRQHandler(UP_JOY_PIN);
  HAL_GPIO_EXTI_IRQHandler(DOWN_JOY_PIN);
}

/**
  * @brief  This function handles EXTI 4 to 15 interrupts.
  * @param  None
  * @retval None
  */
void EXTI4_15_IRQHandler(void)
{
  HAL_GPIO_EXTI_IRQHandler(LEFT_JOY_PIN);
  HAL_GPIO_EXTI_IRQHandler(RIGHT_JOY_PIN);
  HAL_GPIO_EXTI_IRQHandler(TAMPER_BUTTON_PIN);
  HAL_GPIO_EXTI_IRQHandler(DET_HPD_SOURCE_PIN);
}

/**
  * @brief  This function handles debounce timer interrupt request.
  * @param  None
  * @retval None
  */
void DEBOUNCE_TIM_IRQHandler(void)
{
  HAL_TIM_IRQHandler(&htim);
}

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
