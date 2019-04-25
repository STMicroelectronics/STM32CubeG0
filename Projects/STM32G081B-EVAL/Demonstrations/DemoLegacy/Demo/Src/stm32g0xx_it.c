/**
  ******************************************************************************
  * @file    stm32g0xx_it.c
  * @author  MCD Application Team
  * @brief   This file includes the interrupt handlers for the application.
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
#include "stm32g0xx_it.h"
#include "stm32g0xx_hal.h"
#include "stm32g081b_eval.h"
#include "stm32g081b_eval_sd.h"
#include "app_audio.h"
#include "app_calendar.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
/* RTC Handle variable */
extern RTC_HandleTypeDef RtcHandle;

/******************************************************************************/
/*             Cortex-M Processor Exceptions Handlers                         */
/******************************************************************************/
/**
  * @brief  NMI_Handler
  *         This function handles NMI exception.
  * @param  None
  * @retval None
  */
void NMI_Handler(void)
{
  /* Go to infinite loop */
  while (1)
  {
  }
}

/**
  * @brief  HardFault_Handler
  *         This function handles Hard Fault exception.
  * @param  None
  * @retval None
  */
void HardFault_Handler(void)
{
  /* Go to infinite loop */
  while (1)
  {
  }
}

/**
  * @brief  SVC_Handler
  *         This function handles Supervisor Call exception.
  * @param  None
  * @retval None
  */
void SVC_Handler(void)
{
  /* Go to infinite loop */
  while (1)
  {
  }
}

/**
  * @brief  PendSV_Handler
  *         This function handles PendSV exception.
  * @param  None
  * @retval None
  */
void PendSV_Handler(void)
{
  /* Go to infinite loop */
  while (1)
  {
  }
}

void SysTick_Handler(void)
{
  HAL_IncTick();
}

/******************************************************************************/
/*                 STM32F0xx Peripherals Interrupt Handlers                   */
/*  Add here the Interrupt Handler for the used peripheral(s) (PPP), for the  */
/*  available peripheral interrupt handler's name please refer to the startup */
/*  file (startup_stm32fxxx.s).                                                */
/******************************************************************************/
/**
  * @brief  This function handles RTC interrupt request.
  * @param  None
  * @retval None
  */
void RTC_TAMP_IRQHandler(void)
{
    HAL_RTC_AlarmIRQHandler(&RtcHandle);
    HAL_RTCEx_WakeUpTimerIRQHandler(&RtcHandle);
}

/**
  * @brief  This function handles External lines 4_15 interrupt request.
  * @param  None
  * @retval None
  */
void EXTI4_15_IRQHandler(void)
{
  HAL_GPIO_EXTI_IRQHandler(SD_DETECT_PIN);
  HAL_GPIO_EXTI_IRQHandler(RIGHT_JOY_PIN);
  HAL_GPIO_EXTI_IRQHandler(LEFT_JOY_PIN);  
  HAL_GPIO_EXTI_IRQHandler(TAMPER_BUTTON_PIN);
}

/**
  * @brief  This function handles External lines 2_3 interrupt request.
  * @param  None
  * @retval None
  */
void EXTI2_3_IRQHandler(void)
{
  HAL_GPIO_EXTI_IRQHandler(DOWN_JOY_PIN);
  HAL_GPIO_EXTI_IRQHandler(UP_JOY_PIN);
}

/**
  * @brief  This function handles External lines 0_1 interrupt request.
  * @param  None
  * @retval None
  */
void EXTI0_1_IRQHandler(void)
{
  HAL_GPIO_EXTI_IRQHandler(SEL_JOY_PIN);
}
  
/**
  * @brief  DMA1 channel 1 interrupt handler.
  * @param  None
  * @retval None
  */
void DMA1_Channel1_IRQHandler(void)
{
  if (hdac.DMA_Handle1 != (DMA_HandleTypeDef *)NULL)
  {
    HAL_DMA_IRQHandler(hdac.DMA_Handle1);  
  }
  else if (hadc.DMA_Handle != (DMA_HandleTypeDef *)NULL)
  {
    HAL_DMA_IRQHandler(hadc.DMA_Handle); 
  }
}

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
