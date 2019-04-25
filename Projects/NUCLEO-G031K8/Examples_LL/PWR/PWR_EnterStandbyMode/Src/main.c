/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    Examples_LL/PWR/PWR_EnterStandbyMode/Src/main.c
  * @author  MCD Application Team
  * @brief   This example describes how to enter and exit the standby mode with
  *          a wakeup pin or external reset through the STM32G0xx PWR LL API.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2019 STMicroelectronics. 
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

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

#define BUTTON_MODE_GPIO  0
#define BUTTON_MODE_EXTI  1

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */

uint32_t LedSpeed = LED_BLINK_FAST;

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
/* USER CODE BEGIN PFP */

void     Configure_PWR(void);

void     LED_Blinking_5s(uint32_t Period);
void     EnterStandbyMode(void);

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */


  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  

  LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_SYSCFG);
  LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_PWR);

  /* System interrupt init*/

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  /* USER CODE BEGIN 2 */

  /* Configure Power IP */
  Configure_PWR();

  /* Led blinking in RUN mode during 5s */
  LED_Blinking_5s(LedSpeed);
  
  /* Configure and enter in Standby Mode */
  EnterStandbyMode();
  
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  /* HSI configuration and activation */
  LL_RCC_HSI_Enable();
  while(LL_RCC_HSI_IsReady() != 1)
  {
  };

  /* Set AHB prescaler*/
  LL_RCC_SetAHBPrescaler(LL_RCC_SYSCLK_DIV_1);

  /* Sysclk activation on the HSI */
  LL_RCC_SetSysClkSource(LL_RCC_SYS_CLKSOURCE_HSI);
  while(LL_RCC_GetSysClkSource() != LL_RCC_SYS_CLKSOURCE_STATUS_HSI)
  {
  };

  /* Set APB1 prescaler*/
  LL_RCC_SetAPB1Prescaler(LL_RCC_APB1_DIV_1);

  LL_Init1msTick(16000000);

  LL_SYSTICK_SetClkSource(LL_SYSTICK_CLKSOURCE_HCLK);
  /* Update CMSIS variable (which can be updated also through SystemCoreClockUpdate function) */
  LL_SetSystemCoreClock(16000000);
}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  LL_GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* GPIO Ports Clock Enable */
  LL_IOP_GRP1_EnableClock(LL_IOP_GRP1_PERIPH_GPIOC);

  /**/
  LL_GPIO_ResetOutputPin(LED3_GPIO_Port, LED3_Pin);

  /**/
  GPIO_InitStruct.Pin = LED3_Pin;
  GPIO_InitStruct.Mode = LL_GPIO_MODE_OUTPUT;
  GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_LOW;
  GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
  GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;
  LL_GPIO_Init(LED3_GPIO_Port, &GPIO_InitStruct);

}

/* USER CODE BEGIN 4 */
/**
  * @brief  Set LED3 to Blinking mode during 5s (toggle period based on value provided as input parameter).
  * @param  Period : Period of time (in ms) between each toggling of LED
  *   This parameter can be user defined values. Pre-defined values used in that example are :
  *     @arg LED_BLINK_FAST : Fast Blinking
  *     @arg LED_BLINK_SLOW : Slow Blinking
  *     @arg LED_BLINK_ERROR : Error specific Blinking
  * @retval None
  */
void LED_Blinking_5s(uint32_t Period)
{
  uint32_t iteration_max = 5000/Period;
  uint32_t i = 0;
  
  /* Toggle IO in during 5s (25*200ms) */
  for(i = 0; i < iteration_max; i++)
  {
    LL_GPIO_TogglePin(LED3_GPIO_Port, LED3_Pin);
    LL_mDelay(Period);
  }
}

/**
  * @brief  Function to configure and initialize PWR IP.
  * @param  None
  * @retval None
  */
void Configure_PWR(void)
{
  /* Enable Power Clock */
  LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_PWR);
  
  /* Check if the system was resumed from StandBy mode */
  if (LL_PWR_IsActiveFlag_SB() != 0)
  {
    /* Clear Standby flag */
    LL_PWR_ClearFlag_SB();
    
    /* Disable pull-up and pull-down configuration */
    LL_PWR_DisablePUPDCfg();

    /* Change LED speed to SLOW to indicate exit from Standby mode */
    LedSpeed = LED_BLINK_SLOW;
  }
  
  /* Check and Clear the Wakeup flag */
  if (LL_PWR_IsActiveFlag_WU1() != 0)
  {
    LL_PWR_ClearFlag_WU1();
  }
}

/**
  * @brief  Function to configure and enter in Standby Mode.
  * @param  None
  * @retval None
  */
void EnterStandbyMode(void)
{
  /* Turn-off LED */
  /* Note: LED state at this step depends on blinking state at the instant of user button is pressed. */
  LL_GPIO_ResetOutputPin(GPIOA, LL_GPIO_PIN_6);
   
  /* Enable pull up on wakeup pin */
  /* Note: Setting not mandatory but recommended to avoid wake-up pin floating */
  LL_PWR_EnableGPIOPullUp(LL_PWR_GPIO_A, LL_PWR_GPIO_BIT_0);
  
  /* Enable pull-up and pull-down configuration */
  LL_PWR_EnablePUPDCfg();
  
  /* Set wakeup pin polarity */
  LL_PWR_SetWakeUpPinPolarityLow(LL_PWR_WAKEUP_PIN1);

  /* Enable wakeup pin */
  LL_PWR_EnableWakeUpPin(LL_PWR_WAKEUP_PIN1);
  
  /* As default LL_PWR_WAKEUP_PIN1 state is high level, need to clear all wake up Flag again */
  LL_PWR_ClearFlag_WU();

  /** Request to enter Standby mode
    * Following procedure describe in STM32G0xx Reference Manual
    * See PWR part, section Low-power modes, Standby mode
    */
  /* Set Standby mode when CPU enters deepsleep */
  LL_PWR_SetPowerMode(LL_PWR_MODE_STANDBY);
  
  /* Set SLEEPDEEP bit of Cortex System Control Register */
  LL_LPM_EnableDeepSleep();
  
  /* This option is used to ensure that store operations are completed */
#if defined ( __CC_ARM)
  __force_stores();
#endif
  /* Request Wait For Interrupt */
  __WFI();
}

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
 
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{ 
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d", file, line) */

  /* Infinite loop */
  while (1)
  {
  }

  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
