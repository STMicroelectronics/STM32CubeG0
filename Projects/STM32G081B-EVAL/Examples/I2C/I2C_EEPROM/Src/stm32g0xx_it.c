/**
  ******************************************************************************
  * @file    I2C/I2C_EEPROM/Src/stm32g0xx_it.c
  * @author  MCD Application Team
  * @brief   Main Interrupt Service Routines.
  *          This file provides template for all exceptions handler and
  *          peripherals interrupt service routine.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2018-2020 STMicroelectronics.
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

/** @addtogroup STM32G0xx_HAL_Examples
  * @{
  */

/** @addtogroup I2C_EEPROM
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* I2C handler declared in "main.c" file */
extern I2C_HandleTypeDef I2cHandle;

/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/******************************************************************************/
/*            Cortex-M0+ Processor Exceptions Handlers                         */
/******************************************************************************/
/**
  * @brief   This function handles NMI exception.
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
  HAL_IncTick();
}

/******************************************************************************/
/*                 STM32G0xx Peripherals Interrupt Handlers                  */
/*  Add here the Interrupt Handler for the used peripheral(s) (PPP), for the  */
/*  available peripheral interrupt handler's name please refer to the startup */
/*  file (startup_stm32g0xx.s).                                               */
/******************************************************************************/
/**
  * @brief This function handles DMA1 channel 1 interrupt.
  */
void I2Cx_DMA_TX_IRQHandler(void)
{
  HAL_DMA_IRQHandler(I2cHandle.hdmatx);
}

/**
  * @brief This function handles DMA1 channel 2 and channel 3 interrupts.
  */
void I2Cx_DMA_RX_IRQHandler(void)
{
  HAL_DMA_IRQHandler(I2cHandle.hdmarx);
}

/**
  * @brief This function handles I2C1 global interrupt.
  */
void I2Cx_IRQHandler(void)
{
  if (I2cHandle.Instance->ISR & (I2C_FLAG_BERR | I2C_FLAG_ARLO | I2C_FLAG_OVR)) {
    HAL_I2C_ER_IRQHandler(&I2cHandle);
  } else {
    HAL_I2C_EV_IRQHandler(&I2cHandle);
  }
}
