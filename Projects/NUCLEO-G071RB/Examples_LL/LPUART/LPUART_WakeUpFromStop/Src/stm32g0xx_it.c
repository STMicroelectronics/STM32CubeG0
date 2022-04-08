/**
  ******************************************************************************
  * @file    Examples_LL/LPUART/LPUART_WakeUpFromStop/Src/stm32g0xx_it.c
  * @author  MCD Application Team
  * @brief   Main Interrupt Service Routines.
  *          This file provides template for all exceptions handler and
  *          peripherals interrupt service routine.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2018-2019 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "stm32g0xx_it.h"

/** @addtogroup STM32G0xx_LL_Examples
  * @{
  */

/** @addtogroup LPUART_WakeUpFromStop
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/

/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/******************************************************************************/
/*            >Cortex-M0+ Processor Exceptions Handlers                         */
/******************************************************************************/

/**
  * @brief  This function handles NMI exception.
  * @param  None
  * @retval None
  */
void NMI_Handler(void)
{
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
void SysTick_Handler(void)
{
}

/******************************************************************************/
/*                 STM32G0xx Peripherals Interrupt Handlers                   */
/*  Add here the Interrupt Handler for the used peripheral(s), for the        */
/*  available peripheral interrupt handler's name please refer to the startup */
/*  file (startup_stm32g0xx.s).                                               */
/******************************************************************************/

/**
  * Brief   This function handles LPUARTx Instance interrupt request.
  * Param   None
  * Retval  None
  */
void USART3_4_LPUART1_IRQHandler(void)
{
  /* Check WUF flag value in ISR register */
  if(LL_LPUART_IsActiveFlag_WKUP(LPUART1) && LL_LPUART_IsEnabledIT_WKUP(LPUART1))
  {
    /* Configure LPUART1 transfer interrupts : */
    /* Disable the UART Wake UP from stop mode Interrupt */
    LL_LPUART_DisableIT_WKUP(LPUART1);

    /* WUF flag clearing */
    LL_LPUART_ClearFlag_WKUP(LPUART1);
    
    /* Call function in charge of handling Character reception */
    LPUART_CharReception_Callback();
  }
  else
  {
    /* Call Error function */
    Error_Callback();
  }
	
}

/**
  * @}
  */

/**
  * @}
  */
