/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    Examples_LL/RTC/RTC_Tamper_Init/Src/main.c
  * @author  MCD Application Team
  * @brief   This sample code shows how to use STM32G0xx RTC LL API
  *          to write/read data to/from RTC Backup data registers and demonstrates
  *          the Tamper detection feature.
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

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

#define BACKUP_COUNT 5
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */

__IO FlagStatus TamperStatus = RESET;

/* Backup registers table */
uint32_t aBKPDataReg[BACKUP_COUNT] =
{
  LL_RTC_BKP_DR0, LL_RTC_BKP_DR1, LL_RTC_BKP_DR2,
  LL_RTC_BKP_DR3, LL_RTC_BKP_DR4
};

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_RTC_Init(void);
/* USER CODE BEGIN PFP */
void     LED_On(void);
void     LED_Blinking(uint32_t Period);
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

  register uint32_t index = 0;


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
  MX_RTC_Init();
  /* USER CODE BEGIN 2 */

  /* Write Data on the Back Up registers  */
  for (index = 0; index < BACKUP_COUNT; index++)
  {
    LL_RTC_BKP_SetRegister(TAMP, aBKPDataReg[index], 0xDF59 + (index * 0x5A));
  }

  /* Check Data is stored on the Back Up registers */
  for (index = 0; index < BACKUP_COUNT; index++)
  {
    if (LL_RTC_BKP_GetRegister(TAMP, aBKPDataReg[index]) != (0xDF59 + (index * 0x5A)))
    {
      LED_Blinking(LED_BLINK_ERROR);
    }
  }

  /* Wait for Tamper detection */
  while (TamperStatus != SET)
  {
    LL_GPIO_TogglePin(LED4_GPIO_Port, LED4_Pin);
    LL_mDelay(LED_BLINK_FAST);
  }

  /* LED4 On: Tamper button pressed */
  LED_On();

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
  LL_FLASH_SetLatency(LL_FLASH_LATENCY_2);

  /* HSI configuration and activation */
  LL_RCC_HSI_Enable();
  while(LL_RCC_HSI_IsReady() != 1)
  {
  };

  /* Main PLL configuration and activation */
  LL_RCC_PLL_ConfigDomain_SYS(LL_RCC_PLLSOURCE_HSI, LL_RCC_PLLM_DIV_4, 70, LL_RCC_PLLR_DIV_5);
  LL_RCC_PLL_Enable();
  LL_RCC_PLL_EnableDomain_SYS();
  while(LL_RCC_PLL_IsReady() != 1)
  {
  };

  /* Set AHB prescaler*/
  LL_RCC_SetAHBPrescaler(LL_RCC_SYSCLK_DIV_1);

  /* Sysclk activation on the main PLL */
  LL_RCC_SetSysClkSource(LL_RCC_SYS_CLKSOURCE_PLL);
  while(LL_RCC_GetSysClkSource() != LL_RCC_SYS_CLKSOURCE_STATUS_PLL)
  {
  };

  /* Set APB1 prescaler*/
  LL_RCC_SetAPB1Prescaler(LL_RCC_APB1_DIV_1);

  LL_Init1msTick(56000000);

  /* Update CMSIS variable (which can be updated also through SystemCoreClockUpdate function) */
  LL_SetSystemCoreClock(56000000);
}

/**
  * @brief RTC Initialization Function
  * @param None
  * @retval None
  */
static void MX_RTC_Init(void)
{

  /* USER CODE BEGIN RTC_Init 0 */

  /* USER CODE END RTC_Init 0 */

  LL_RTC_InitTypeDef RTC_InitStruct = {0};

  /* Peripheral clock enable */
  LL_RCC_EnableRTC();

  /* USER CODE BEGIN RTC_Init 1 */

  /* Enables the PWR Clock and Enables access to the backup domain */
  LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_PWR);

  LL_PWR_EnableBkUpAccess();
  /* Enable RTC APB clock  */
  LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_RTC);
  /* USER CODE END RTC_Init 1 */
  RTC_InitStruct.HourFormat = LL_RTC_HOURFORMAT_24HOUR;
  RTC_InitStruct.AsynchPrescaler = 127;
  RTC_InitStruct.SynchPrescaler = 255;
  LL_RTC_Init(RTC, &RTC_InitStruct);
  /** Enable the RTC Tamper 1 
  */
  LL_RTC_TAMPER_Enable(TAMP, LL_RTC_TAMPER_1);
  LL_RTC_TAMPER_DisableActiveLevel(TAMP, LL_RTC_TAMPER_ACTIVELEVEL_TAMP1);
  LL_RTC_TAMPER_DisableMask(TAMP, LL_RTC_TAMPER_MASK_TAMPER1);
  LL_RTC_TAMPER_EnableEraseBKP(TAMP, LL_RTC_TAMPER_NOERASE_TAMPER1);
  LL_RTC_TAMPER_SetFilterCount(TAMP, LL_RTC_TAMPER_FILTER_DISABLE);
  LL_RTC_TAMPER_SetPrecharge(TAMP, LL_RTC_TAMPER_DURATION_1RTCCLK);
  LL_RTC_TAMPER_SetSamplingFreq(TAMP, LL_RTC_TAMPER_SAMPLFREQDIV_32768);
  LL_RTC_TAMPER_EnablePullUp(TAMP);
  LL_RTC_TS_EnableOnTamper(RTC);
  /* USER CODE BEGIN RTC_Init 2 */
  LL_RTC_EnableIT_TAMP1(TAMP);

  /* Configure the NVIC for RTC Tamper */
  NVIC_SetPriority(RTC_TAMP_IRQn, 0x0F);
  NVIC_EnableIRQ(RTC_TAMP_IRQn);

  /* RTC Tamper Interrupt Configuration: EXTI configuration */
  LL_EXTI_EnableIT_0_31(LL_EXTI_LINE_21);
  LL_EXTI_EnableRisingTrig_0_31(LL_EXTI_LINE_21);

  /* Clear the Tamper interrupt pending bit */
  LL_RTC_ClearFlag_TAMP1(TAMP);
  /* USER CODE END RTC_Init 2 */

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
  LL_IOP_GRP1_EnableClock(LL_IOP_GRP1_PERIPH_GPIOA);

  /**/
  LL_GPIO_ResetOutputPin(LED4_GPIO_Port, LED4_Pin);

  /**/
  GPIO_InitStruct.Pin = LED4_Pin;
  GPIO_InitStruct.Mode = LL_GPIO_MODE_OUTPUT;
  GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_LOW;
  GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
  GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;
  LL_GPIO_Init(LED4_GPIO_Port, &GPIO_InitStruct);

}

/* USER CODE BEGIN 4 */


/**
  * @brief  Turn-on LED4.
  * @param  None
  * @retval None
  */
void LED_On(void)
{
  /* Turn LED4 on */
  LL_GPIO_SetOutputPin(LED4_GPIO_Port, LED4_Pin);
}

/**
  * @brief  Set LED4 to Blinking mode for an infinite loop (toggle period based on value provided as input parameter).
  * @param  Period : Period of time (in ms) between each toggling of LED
  *   This parameter can be user defined values. Pre-defined values used in that example are :
  *     @arg LED_BLINK_FAST : Fast Blinking
  *     @arg LED_BLINK_SLOW : Slow Blinking
  *     @arg LED_BLINK_ERROR : Error specific Blinking
  * @retval None
  */
void LED_Blinking(uint32_t Period)
{
  /* Toggle IO in an infinite loop */
  while (1)
  {
    LL_GPIO_TogglePin(LED4_GPIO_Port, LED4_Pin);
    LL_mDelay(Period);
  }
}

/******************************************************************************/
/*   USER IRQ HANDLER TREATMENT                                               */
/******************************************************************************/
/**
  * @brief  Tamper event callback function
  * @param  None
  * @retval None
  */
void Tamper_Callback(void)
{
  register uint32_t index = 0;

  /* Deactivate the tamper */
  LL_RTC_TAMPER_Disable(TAMP, LL_RTC_TAMPER_1);

  /* Check Data is cleared on the Back Up registers */
  for (index = 0; index < BACKUP_COUNT; index++)
  {
    if (LL_RTC_BKP_GetRegister(TAMP, aBKPDataReg[index]) != 0x00)
    {
      LED_Blinking(LED_BLINK_ERROR);
    }
  }
  TamperStatus = SET;
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
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

  /* Infinite loop */
  while (1)
  {
  }
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
