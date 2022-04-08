/**
  ******************************************************************************
  * @file    stm32g0xx_hal_msp.c
  * @author  MCD Application Team
  * @brief   This file provides code for the MSP Initialization
  *          and de-Initialization codes.
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
#include "stm32g0xx_hal.h"
#include "stm32g0xx_ll_bus.h"
#include "stm32g0xx_ll_gpio.h"
#include "interfaces_conf.h"

/**
  * Initializes the Global MSP.
  */
void HAL_MspInit(void)
{
  __HAL_RCC_SYSCFG_CLK_ENABLE();
  __HAL_RCC_PWR_CLK_ENABLE();
}


/**
  * @brief This function configures FDCAN used hardware resources
  * @param hfdcan: FDCAN handle pointer
  * @retval None
  */
void HAL_FDCAN_MspInit(FDCAN_HandleTypeDef *hfdcan)
{
  LL_GPIO_InitTypeDef GPIO_InitStruct;

  RCC_PeriphCLKInitTypeDef RCC_PeriphClkInit;

  /* Select PLL1Q as source of FDCANx clock */
  RCC_PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_FDCAN;
  RCC_PeriphClkInit.FdcanClockSelection  = RCC_FDCANCLKSOURCE_PLL;
  HAL_RCCEx_PeriphCLKConfig(&RCC_PeriphClkInit);

  /* Enable FDCANx clock */
  LL_AHB1_GRP1_EnableClock(LL_IOP_GRP1_PERIPH_GPIOD);
  LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_FDCAN);

  /*##-2- Configure peripheral GPIO ##########################################*/
  /* Configure FDCAN TX as alternate function */
  GPIO_InitStruct.Pin        = FDCANx_TX_PIN;
  GPIO_InitStruct.Mode       = LL_GPIO_MODE_ALTERNATE;
  GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
  GPIO_InitStruct.Speed      = LL_GPIO_SPEED_FREQ_HIGH;
  GPIO_InitStruct.Pull       = LL_GPIO_PULL_UP;
  GPIO_InitStruct.Alternate  = FDCANx_TX_AF;
  LL_GPIO_Init(FDCANx_TX_GPIO_PORT, &GPIO_InitStruct);

  /* Configure FDCAN RX as alternate function */
  GPIO_InitStruct.Pin = FDCANx_RX_PIN;
  LL_GPIO_Init(FDCANx_RX_GPIO_PORT, &GPIO_InitStruct);
}

/**
* @brief  DeInitializes the FDCAN MSP.
* @param  hfdcan: pointer to an FDCAN_HandleTypeDef structure that contains
*         the configuration information for the specified FDCAN.
* @retval None
*/
void HAL_FDCAN_MspDeInit(FDCAN_HandleTypeDef *hfdcan)
{
  /*##-1- Reset peripherals ##################################################*/
  FDCANx_FORCE_RESET();
  FDCANx_RELEASE_RESET();

  /*##-2- Disable peripherals and GPIO Clocks ################################*/
  /* Configure FDCANx Tx as alternate function */
  HAL_GPIO_DeInit(FDCANx_TX_GPIO_PORT, FDCANx_TX_PIN);

  /* Configure FDCANx Rx as alternate function */
  HAL_GPIO_DeInit(FDCANx_RX_GPIO_PORT, FDCANx_RX_PIN);
}
