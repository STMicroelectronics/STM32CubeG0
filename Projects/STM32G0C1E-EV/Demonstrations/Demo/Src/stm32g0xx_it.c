/**
  ******************************************************************************
  * @file    stm32g0xx_it.c
  * @author  MCD Application Team
  * @brief   This file includes the interrupt handlers for the application.
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

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "stm32g0xx_it.h"
#include "stm32g0c1e_eval.h"
#include "usbpd_def.h"
#include "usbpd_dpm_core.h"
#include "usbpd_pwr_if.h"
#include "usbpd_hw_if.h"
#if defined(_GUI_INTERFACE)
#include "gui_api.h"
#endif /* _GUI_INTERFACE */
#if defined(_TRACE)
#include "tracer_emb.h"
#endif/*_TRACE */

/** @addtogroup CORE
  * @{
  */

/** @defgroup
  * @brief
  * @{
  */
  
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
  if (SubDemoId == SUB_DEMO_UCPD)
  {
    USBPD_PWR_IF_AlarmType(USBPD_PWR_IF_NMI);
  }
  
  /* Go to infinite loop when NMI occurs */
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
  if (SubDemoId == SUB_DEMO_UCPD)
  {
    USBPD_PWR_IF_AlarmType(USBPD_PWR_IF_HARD_FAULT);
  }

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
void SysTick_Handler (void)
{
  if (SubDemoId == SUB_DEMO_UCPD)
  {
    USBPD_DPM_TimerCounter();
#if defined(_GUI_INTERFACE)
    GUI_TimerCounter();
#endif /* _GUI_INTERFACE */
  }
  
  HAL_IncTick();
}
 
/******************************************************************************/
/*                 STM32G0xx Peripherals Interrupt Handlers                   */
/*  Add here the Interrupt Handler for the used peripheral(s) (PPP), for the  */
/*  available peripheral interrupt handler's name please refer to the startup */
/*  file (startup_stm32g0c1e.s).                                                */
/******************************************************************************/
/**
  * @brief  This function handles RTC interrupt request.
  * @param  None
  * @retval None
  */
void RTC_TAMP_IRQHandler(void)
{
    HAL_RTC_AlarmIRQHandler(&hrtc);
    HAL_RTCEx_WakeUpTimerIRQHandler(&hrtc);
}

/**
  * @brief  This function handles External lines 4_15 interrupt request.
  * @param  None
  * @retval None
  */
#if defined(USE_STM32G0C1E_EVAL_REVA)
void EXTI4_15_IRQHandler(void)
{
  HAL_GPIO_EXTI_IRQHandler(RIGHT_JOY_PIN);
  HAL_GPIO_EXTI_IRQHandler(DOWN_JOY_PIN);  
  HAL_GPIO_EXTI_IRQHandler(TAMPER_BUTTON_PIN);
  HAL_GPIO_EXTI_IRQHandler(DaughterBoardDependentExtiLine);
}
#else
void EXTI4_15_IRQHandler(void)
{
  HAL_GPIO_EXTI_IRQHandler(RIGHT_JOY_PIN);
  HAL_GPIO_EXTI_IRQHandler(LEFT_JOY_PIN);  
  HAL_GPIO_EXTI_IRQHandler(TAMPER_BUTTON_PIN);
  HAL_GPIO_EXTI_IRQHandler(DaughterBoardDependentExtiLine);
}
#endif

/**
  * @brief  This function handles External lines 2_3 interrupt request.
  * @param  None
  * @retval None
  */
#if defined(USE_STM32G0C1E_EVAL_REVA)
void EXTI2_3_IRQHandler(void)
{
  HAL_GPIO_EXTI_IRQHandler(LEFT_JOY_PIN);
  HAL_GPIO_EXTI_IRQHandler(UP_JOY_PIN);
}
#else
void EXTI2_3_IRQHandler(void)
{
  HAL_GPIO_EXTI_IRQHandler(DOWN_JOY_PIN);
  HAL_GPIO_EXTI_IRQHandler(UP_JOY_PIN);
}
#endif

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
  * @brief This function handles UCPD1 and UCPD2 interrupts / UCPD1 and UCPD2 wake-up interrupts through EXTI lines 32 and 33.
  */
void USB_UCPD1_2_IRQHandler(void)
{
  USBPD_PORT0_IRQHandler();

#if USBPD_PORT_COUNT == 2
  USBPD_PORT1_IRQHandler();
#endif
}

/**
  * @brief  This function handles debounce timer interrupt request.
  * @retval None
  */
void DEBOUNCE_TIM_IRQHandler(void)
{
  HAL_TIM_IRQHandler(&htim);
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
  else if (hadc1.DMA_Handle != (DMA_HandleTypeDef *)NULL)
  {
    HAL_DMA_IRQHandler(&hdma_adc1); 
  }
}

/**
  * @brief  This function handles ADC interrupt request.
  * @retval None
  */
void ADC1_COMP_IRQHandler(void)
{
  HAL_ADC_IRQHandler(&hadc1);
}

#if defined(_TRACE)
/**
  * @brief  This function handles USART 3 and 4 interrupts.
  * @retval None
  */
void TRACER_EMB_USART_IRQHANDLER(void)
{
  TRACER_EMB_IRQHandlerUSART();
}

/**
  * @brief  This function handles DMA channel 4 to 7 interrupt request.
  * @retval None
  */
void TRACER_EMB_TX_DMA_IRQHANDLER(void)
{
   TRACER_EMB_IRQHandlerDMA();
}
#endif /*_TRACE */

/**
  * @}
  */

/**
  * @}
  */
