/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    stm32g0xx_it.c
  * @brief   Interrupt Service Routines.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2019-2020 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Private includes ----------------------------------------------------------*/
#include "main.h"
#include "stm32g0xx.h"
#include "demo_disco.h"

#include "stm32g0xx_it.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#if defined(_TRACE) ||defined(_GUI_INTERFACE)
#include "tracer_emb.h"
#endif
/* USER CODE END Includes */

/** @addtogroup STM32_USBPD_APPLICATION
  * @{
  */

/** @addtogroup STM32_USBPD_APPLICATION_IT
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN TD */

/* USER CODE END TD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
 
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN PFP */
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/* External variables --------------------------------------------------------*/
/* USER CODE BEGIN EV */

/* USER CODE END EV */

/******************************************************************************/
/*            Cortex-M0+ Processor  Interruption and Exception Handlers                         */
/******************************************************************************/
/**
  * @brief This function handles Non maskable interrupt.
  */
void NMI_Handler(void)
{
  /* USER CODE BEGIN NonMaskableInt_IRQn 0 */
  /* Go to infinite loop when Hard Fault exception occurs */
  while (1)
  {
  }
  /* USER CODE END NonMaskableInt_IRQn 0 */
  /* USER CODE BEGIN NonMaskableInt_IRQn 1 */

  /* USER CODE END NonMaskableInt_IRQn 1 */
}

/**
  * @brief This function handles Hard fault interrupt.
  */
void HardFault_Handler(void)
{
  /* USER CODE BEGIN HardFault_IRQn 0 */

  /* Go to infinite loop when Hard Fault exception occurs */
  /* USER CODE END HardFault_IRQn 0 */
  while (1)
  {
    /* USER CODE BEGIN W1_HardFault_IRQn 0 */
    /* USER CODE END W1_HardFault_IRQn 0 */
  }
}

/**
  * @brief  This function handles SysTick Handler.
  * @retval None
  */
void SysTick_Handler(void)
{
  /* USER CODE BEGIN SysTick_IRQn 0 */
  USBPD_DPM_TimerCounter();
#if defined(_GUI_INTERFACE)
  GUI_TimerCounter();
#endif /* _GUI_INTERFACE */
  /* USER CODE END SysTick_IRQn 0 */
  HAL_IncTick();
  /* USER CODE BEGIN SysTick_IRQn 1 */

  /* USER CODE END SysTick_IRQn 1 */
}

/******************************************************************************/
/*                 STM32G0xx Peripherals Interrupt Handlers                   */
/*  Add here the Interrupt Handler for the used peripheral(s) (PPP), for the  */
/*  available peripheral interrupt handler's name please refer to the startup */
/*  file (startup_stm32g0xx.s).                                               */
/******************************************************************************/
#if defined(_TRACE)||defined(_GUI_INTERFACE)
/**
  * @brief This function handles DMA1 channel 4, channel 5, channel 6, channel 7 and DMAMUX1 interrupts.
  */
void DMA1_Ch4_7_DMAMUX1_OVR_IRQHandler(void)
{
  /* USER CODE BEGIN DMA1_Ch4_7_DMAMUX1_OVR_IRQn 0 */
   TRACER_EMB_IRQHandlerDMA();
  /* USER CODE END DMA1_Ch4_7_DMAMUX1_OVR_IRQn 0 */
  
  /* USER CODE BEGIN DMA1_Ch4_7_DMAMUX1_OVR_IRQn 1 */

  /* USER CODE END DMA1_Ch4_7_DMAMUX1_OVR_IRQn 1 */
}
#endif /*_TRACE || _GUI_INTERFACE */

/**
  * @brief This function handles UCPD1 and UCPD2 interrupts / UCPD1 and UCPD2 wake-up interrupts through EXTI lines 32 and 33.
  */
void UCPD1_2_IRQHandler(void)
{
  /* USER CODE BEGIN UCPD1_2_IRQn 0 */
  if( DEMO_IsSpyMode())
  {
    DEMO_SPY_Handler();
  }
  else
  {
    USBPD_PORT0_IRQHandler();
  }
  /* USER CODE END UCPD1_2_IRQn 0 */
  /* USER CODE BEGIN UCPD1_2_IRQn 1 */

  /* USER CODE END UCPD1_2_IRQn 1 */
}

#if defined(_GUI_INTERFACE) || defined(_TRACE)
/**
  * @brief  This function handles USART 3 and 4 interrupts.
  * @retval None
  */
void USART3_4_LPUART1_IRQHandler(void)
{
  TRACER_EMB_IRQHandlerUSART();
}
#endif /*_GUI_INTERFACE || _TRACE */

/* USER CODE BEGIN 1 */

/**
  * @brief  This function handles EXTI 0 and 1 interrupts.
  * @retval None
  */
void EXTI0_1_IRQHandler(void)
{
  HAL_GPIO_EXTI_IRQHandler(LEFT_JOY_PIN);
  HAL_GPIO_EXTI_IRQHandler(SEL_JOY_PIN);
}

/**
  * @brief  This function handles EXTI 2 and 3 interrupts.
  * @retval None
  */
void EXTI2_3_IRQHandler(void)
{
  HAL_GPIO_EXTI_IRQHandler(DOWN_JOY_PIN);
  HAL_GPIO_EXTI_IRQHandler(RIGHT_JOY_PIN);
}

/**
  * @brief  This function handles EXTI 4 to 15 interrupts.
  * @retval None
  */
void EXTI4_15_IRQHandler(void)
{
  HAL_GPIO_EXTI_IRQHandler(UP_JOY_PIN);
}
/* USER CODE END 1 */

/**
  * @}
  */

/**
  * @}
  */
