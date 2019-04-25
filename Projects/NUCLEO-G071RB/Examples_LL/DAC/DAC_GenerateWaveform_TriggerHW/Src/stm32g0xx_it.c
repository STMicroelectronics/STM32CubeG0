/** 
  ******************************************************************************
  * @file    Examples_LL/DAC/DAC_GenerateWaveform_TriggerHW/Src/stm32g0xx_it.c
  * @author  MCD Application Team
  * @brief   Main Interrupt Service Routines.
  *          This file provides template for all exceptions handler and
  *          peripherals interrupt service routine.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2018 STMicroelectronics. 
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the 
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "stm32g0xx_it.h"

/** @addtogroup STM32G0xx_LL_Examples
  * @{
  */

/** @addtogroup DAC_GenerateWaveform_TriggerHW
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
/*  Add here the Interrupt Handler for the used peripheral(s) (PPP), for the  */
/*  available peripheral interrupt handler's name please refer to the startup */
/*  file (startup_stm32g0xx.s).                                               */
/******************************************************************************/

/**
  * @brief  This function handles external line 4_15 interrupt request.
  * @param  None
  * @retval None
  */
void USER_BUTTON_IRQHANDLER(void)
{
  /* Manage Flags */
  if(LL_EXTI_IsActiveFallingFlag_0_31(USER_BUTTON_EXTI_LINE) != RESET)
  {
    /* Call interruption treatment function */
    UserButton_Callback();
    
    /* Clear EXTI line flag */
    /* Note: Clear flag after callback function to minimize user button       */
    /*       switch debounce parasitics.                                      */
    LL_EXTI_ClearFallingFlag_0_31(USER_BUTTON_EXTI_LINE);
  }
}

/**
  * @brief  This function handles DAC1 interrupt.
  * @param  None
  * @retval None
  */
void TIM6_DAC_LPTIM1_IRQHandler(void)
{
  /* Check whether DAC channel1 underrun caused the DAC interruption */
  if(LL_DAC_IsActiveFlag_DMAUDR1(DAC1) != 0)
  {
    /* Clear flag DAC channel1 underrun */
    LL_DAC_ClearFlag_DMAUDR1(DAC1);
    
    /* Call interruption treatment function */
    DacUnderrunError_Callback();
  }
}

/**
  * @brief  This function handles DMA1 interrupt request.
  * @param  None
  * @retval None
  */
void DMA1_Channel2_3_IRQHandler(void)
{
  /* Check whether DMA transfer error caused the DMA interruption */
  if(LL_DMA_IsActiveFlag_TE3(DMA1) == 1)
  {
    /* Clear flag DMA transfer error */
    LL_DMA_ClearFlag_TE3(DMA1);
    
    /* Call interruption treatment function */
    DacDmaTransferError_Callback();
  }
}

/**
  * @}
  */

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
