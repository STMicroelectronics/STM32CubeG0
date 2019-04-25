/**
  ******************************************************************************
  * @file    stm32g0xx_it.c
  * @author  MCD Application Team
  * @brief   This file includes the interrupt handlers for the application.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2019 STMicroelectronics.
  * All rights reserved.</center></h2>
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
#include "main.h"

/** @addtogroup
  * @{
  */

/** @defgroup
  * @brief
  * @{
  */

/* External variables --------------------------------------------------------*/
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

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
  /* Go to infinite loop when Hard Fault exception occurs */
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
  /* Go to infinite loop when Hard Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles SVCall exception.
  * @param  None
  * @retval None
  */
void SVC_Handler(void)
{
}

/**
  * @brief  This function handles PendSVC exception.
  * @param  None
  * @retval None
  */
void PendSV_Handler(void)
{
}

/**
  * @brief  This function handles SysTick Handler.
  * @param  None
  * @retval None
  */
void SysTick_Handler (void)
{
  HAL_IncTick();
}
 
/**
  * @}
  */


/**
  * @brief  This function handles RTC & TAMP interrupt request.
  * @param  None
  * @retval None
  */
void RTC_TAMP_IRQHandler(void)
{
  HAL_RTCEx_WakeUpTimerIRQHandler(&RtcHandle);
}


/**
  * @brief  This function handles External lines 0_1 interrupt request.
  * @param  None
  * @retval None
  */
void EXTI0_1_IRQHandler(void)
{
  HAL_GPIO_EXTI_IRQHandler(ButtonPin);
}

/**
  * @brief  This function handles External lines 2_3 interrupt request.
  * @param  None
  * @retval None
  */
void EXTI2_3_IRQHandler(void)
{
  HAL_GPIO_EXTI_IRQHandler(ButtonPin);
}

/**
  * @}
  */
  
  
/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
