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
  * Copyright (c) 2019 STMicroelectronics. All rights reserved.
  *
  * This software component is licensed by ST under Ultimate Liberty license
  * SLA0044, the "License"; You may not use this file except in compliance with
  * the License. You may obtain a copy of the License at:
  *                               www.st.com/SLA0044
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "stm32g0xx.h"
#if defined(_TRACE) ||defined(_GUI_INTERFACE)
#include "tracer_emb.h"
#endif
#include "demo_disco.h"

#include "stm32g0xx_it.h"
/** @addtogroup STM32F0xx_HAL_Examples
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

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
/*                 STM32F0xx Peripherals Interrupt Handlers                   */
/*  Add here the Interrupt Handler for the used peripheral(s) (PPP), for the  */
/*  available peripheral interrupt handler's name please refer to the startup */
/*  file (startup_stm32f0xx.s).                                               */
/******************************************************************************/
#if defined(_TRACE)||defined(_GUI_INTERFACE)
void DMA1_Ch4_7_DMAMUX1_OVR_IRQHandler(void)
{
   TRACER_EMB_IRQHandlerDMA();
}
#endif /*_TRACE || _GUI_INTERFACE */

/**
  * @brief  This function handles DMA Channel 2 and 3 interrupts.
  * @param  None
  * @retval None
  */
void UCPD1_2_IRQHandler(void)
{
  if( DEMO_IsSpyMode())
  {
    DEMO_SPY_Handler();
  }
  else
  {
    USBPD_PORT0_IRQHandler();
  }
}

#if defined(_GUI_INTERFACE)
extern void HW_TRACER_EMB_IRQHandlerUSART(void);
void USART3_4_LPUART1_IRQHandler(void)
{
  HW_TRACER_EMB_IRQHandlerUSART();
}
#endif /*_GUI_INTERFACE*/

void EXTI0_1_IRQHandler(void) /* Joystick select */
{
  HAL_GPIO_EXTI_IRQHandler(LEFT_JOY_PIN);
  HAL_GPIO_EXTI_IRQHandler(SEL_JOY_PIN);
}

void EXTI2_3_IRQHandler(void)
{
  HAL_GPIO_EXTI_IRQHandler(DOWN_JOY_PIN);
  HAL_GPIO_EXTI_IRQHandler(RIGHT_JOY_PIN);
}

void EXTI4_15_IRQHandler(void)
{
  HAL_GPIO_EXTI_IRQHandler(UP_JOY_PIN);
}

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
