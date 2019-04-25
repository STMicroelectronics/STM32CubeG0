/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    CEC/CEC_ListenMode_Device_3/Src/stm32f0xx_hal_msp.c 
  * @author  MCD Application Team
  * @brief   HAL MSP module.
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
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "main.h"
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN TD */

/* USER CODE END TD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN Define */
 
/* USER CODE END Define */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN Macro */

/* USER CODE END Macro */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* External functions --------------------------------------------------------*/
/* USER CODE BEGIN ExternalFunctions */

/* USER CODE END ExternalFunctions */

/* USER CODE BEGIN 0 */

/* USER CODE END 0 */
/**
  * Initializes the Global MSP.
  */
void HAL_MspInit(void)
{
  /* USER CODE BEGIN MspInit 0 */

  /* USER CODE END MspInit 0 */

  __HAL_RCC_SYSCFG_CLK_ENABLE();
  __HAL_RCC_PWR_CLK_ENABLE();

  /* System interrupt init*/

  /* USER CODE BEGIN MspInit 1 */

  /* USER CODE END MspInit 1 */
}

/**
* @brief CEC MSP Initialization
* This function configures the hardware resources used in this example
* @param hcec: CEC handle pointer
* @retval None
*/
void HAL_CEC_MspInit(CEC_HandleTypeDef* hcec)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};
  if(hcec->Instance==CEC)
  {
  /* USER CODE BEGIN CEC_MspInit 0 */

  /* USER CODE END CEC_MspInit 0 */
    /* Peripheral clock enable */
    __HAL_RCC_CEC_CLK_ENABLE();
  
    __HAL_RCC_GPIOB_CLK_ENABLE();
    /**HDMI_CEC GPIO Configuration    
    PB10     ------> CEC 
    */
    GPIO_InitStruct.Pin = GPIO_PIN_10;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_OD;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF0_CEC;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

    /* CEC interrupt Init */
    HAL_NVIC_SetPriority(CEC_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(CEC_IRQn);
  /* USER CODE BEGIN CEC_MspInit 1 */

  /* USER CODE END CEC_MspInit 1 */
  }

}

/**
* @brief CEC MSP De-Initialization
* This function freeze the hardware resources used in this example
* @param hcec: CEC handle pointer
* @retval None
*/
void HAL_CEC_MspDeInit(CEC_HandleTypeDef* hcec)
{
  if(hcec->Instance==CEC)
  {
  /* USER CODE BEGIN CEC_MspDeInit 0 */

  /* USER CODE END CEC_MspDeInit 0 */
    /* Peripheral clock disable */
    __HAL_RCC_CEC_CLK_DISABLE();
  
    /**HDMI_CEC GPIO Configuration    
    PB10     ------> CEC 
    */
    HAL_GPIO_DeInit(GPIOB, GPIO_PIN_10);

    /* CEC interrupt DeInit */
    HAL_NVIC_DisableIRQ(CEC_IRQn);
  /* USER CODE BEGIN CEC_MspDeInit 1 */

  /* USER CODE END CEC_MspDeInit 1 */
  }

}

/* USER CODE BEGIN 1 */

/* USER CODE END 1 */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
