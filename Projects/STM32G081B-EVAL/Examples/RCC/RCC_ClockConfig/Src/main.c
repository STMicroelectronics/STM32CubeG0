/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    RCC/RCC_ClockConfig/Src/main.c
  * @author  MCD Application Team
  * @brief   This example describes how to use the RCC HAL API to configure the
  *          system clock (SYSCLK) and modify the clock settings on run time.
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

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
/* Private variables ---------------------------------------------------------*/
__IO FlagStatus SwitchClock = RESET;
__IO uint32_t  ClockSourceStatus = 0;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
/* USER CODE BEGIN PFP */
/* Private function prototypes -----------------------------------------------*/
static void SystemClockHSI_Config(void);
static void SystemClockHSE_Config(void);
static void SwitchSystemClock(void);
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
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  /* USER CODE BEGIN 2 */

  /* Configure LED1 and LED3 */
  BSP_LED_Init(LED1);
  BSP_LED_Init(LED3);

  /* Initialize Tamper push-button, will be used to trigger an interrupt each time it's pressed.*/
  BSP_PB_Init(BUTTON_TAMPER, BUTTON_MODE_EXTI);

  __HAL_RCC_SYSCFG_CLK_ENABLE();
  /* Perform UCPD Dead Battery strobe signal to remove 5K pull down on CC lines */
  HAL_SYSCFG_StrobeDBattpinsConfig(SYSCFG_CFGR1_UCPD1_STROBE | SYSCFG_CFGR1_UCPD2_STROBE);
  __HAL_RCC_SYSCFG_CLK_DISABLE();

  
  /* Output SYSCLK  on MCO1 pin(PA.08) */
  HAL_RCC_MCOConfig(RCC_MCO1, RCC_MCO1SOURCE_SYSCLK, RCC_MCODIV_1);

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
    /* check if Tamper push-button has been pressed to switch clock config */
    if (SwitchClock != RESET)
    {
      SwitchSystemClock();
    }

    /* Toggle LED1 in an infinite loop */
    BSP_LED_Toggle(LED1);
    HAL_Delay(100);
  }

  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the main internal regulator output voltage
  */
  HAL_PWREx_ControlVoltageScaling(PWR_REGULATOR_VOLTAGE_SCALE1);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSIDiv = RCC_HSI_DIV1;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLM = RCC_PLLM_DIV4;
  RCC_OscInitStruct.PLL.PLLN = 70;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV10;
  RCC_OscInitStruct.PLL.PLLQ = RCC_PLLQ_DIV5;
  RCC_OscInitStruct.PLL.PLLR = RCC_PLLR_DIV5;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};
/* USER CODE BEGIN MX_GPIO_Init_1 */
/* USER CODE END MX_GPIO_Init_1 */

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOA_CLK_ENABLE();

  /*Configure GPIO pin : PA8 */
  GPIO_InitStruct.Pin = GPIO_PIN_8;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  GPIO_InitStruct.Alternate = GPIO_AF0_MCO;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

/* USER CODE BEGIN MX_GPIO_Init_2 */
/* USER CODE END MX_GPIO_Init_2 */
}

/* USER CODE BEGIN 4 */
/**
  * @brief  EXTI line detection callbacks.
  * @param  GPIO_Pin: Specifies the pins connected EXTI line
  * @retval None
  */
void HAL_GPIO_EXTI_Falling_Callback(uint16_t GPIO_Pin)
{
  if (GPIO_Pin == TAMPER_BUTTON_PIN)
  {
    SwitchClock = SET;
  }
}


/**
  * @brief  switch in system clock out of ISR context.
  * @retval None
  */
static void SwitchSystemClock(void)
{
  if (__HAL_RCC_GET_PLL_OSCSOURCE() == RCC_PLLSOURCE_HSI)
  {
    /* PLL source is HSI oscillator */
    /* Set SYSCLK frequency to 56000000 Hz, coming from the PLL which is clocked by HSE  */
    SystemClockHSE_Config();
  }
  else if (__HAL_RCC_GET_PLL_OSCSOURCE() == RCC_PLLSOURCE_HSE)
  {
    /* PLL source is HSE  oscillator */
    /* Set SYSCLK frequency to 16000000 Hz, coming from the PLL which is clocked by HSI */
    SystemClockHSI_Config();
  }

  /* reset global variable */
  SwitchClock = RESET;
}

/**
  * @brief  Switch the PLL source from HSI to HSE , and select the PLL as SYSCLK
  *         source.
  *         The system Clock is configured as follows :
  *            System Clock source            = PLL (HSE )
  *            SYSCLK(Hz)                     = 56000000
  *            HCLK(Hz)                       = 56000000
  *            AHB Prescaler                  = 1
  *            APB1 Prescaler                 = 1
  *            HSE Frequency(Hz)              = 8000000
  *            PLL_M                          = RCC_PLLM_DIV1
  *            PLL_N                          = 28
  *            PLL_P                          = RCC_PLLP_DIV8
  *            PLL_Q                          = RCC_PLLQ_DIV4
  *            PLL_R                          = RCC_PLLR_DIV4
  *            Flash Latency(WS)              = 2
  * @param  None
  * @retval None
  */
static void SystemClockHSE_Config(void)
{
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};

  /* -1- Select HSI as system clock source to allow modification of the PLL configuration */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_SYSCLK;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;
  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
  {
    /* Initialization Error */
    Error_Handler();
  }

  /* -2- Enable HSE  Oscillator, select it as PLL source and finally activate the PLL */
  RCC_OscInitStruct.OscillatorType        = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState              = RCC_HSE_ON;

  RCC_OscInitStruct.PLL.PLLSource         = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLState          = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLM              = RCC_PLLM_DIV1;
  RCC_OscInitStruct.PLL.PLLN              = 28;
  RCC_OscInitStruct.PLL.PLLP              = RCC_PLLP_DIV8;
  RCC_OscInitStruct.PLL.PLLQ              = RCC_PLLQ_DIV4;
  RCC_OscInitStruct.PLL.PLLR              = RCC_PLLR_DIV4;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    /* Initialization Error */
    Error_Handler();
  }

  /* -3- Select the PLL as system clock source and configure the HCLK and PCLK1 clocks dividers */
  RCC_ClkInitStruct.ClockType       = (RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_PCLK1);
  RCC_ClkInitStruct.SYSCLKSource    = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider   = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider  = RCC_HCLK_DIV1;
  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    /* Initialization Error */
    Error_Handler();
  }

  /* -4- Optional: Disable HSI Oscillator (if the HSI is no more needed by the application)*/
  RCC_OscInitStruct.OscillatorType  = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState        = RCC_HSI_OFF;
  RCC_OscInitStruct.PLL.PLLState    = RCC_PLL_NONE;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    /* Initialization Error */
    Error_Handler();
  }
  ClockSourceStatus = 1;
}

/**
  * @brief  Switch the PLL source from HSE  to HSI, and select the PLL as SYSCLK
  *         source.
  *         The system Clock is configured as follows :
  *            System Clock source            = PLL (HSI)
  *            SYSCLK(Hz)                     = 16000000
  *            HCLK(Hz)                       = 16000000
  *            AHB Prescaler                  = 1
  *            APB1 Prescaler                 = 1
  *            APB2 Prescaler                 = 
  *            Flash Latency(WS)              = 2
  * @param  None
  * @retval None
  */
static void SystemClockHSI_Config(void)
{
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};

  /* -1- Select HSE  as system clock source to allow modification of the PLL configuration */
  RCC_ClkInitStruct.ClockType       = RCC_CLOCKTYPE_SYSCLK;
  RCC_ClkInitStruct.SYSCLKSource    = RCC_SYSCLKSOURCE_HSE;
  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
  {
    /* Initialization Error */
    Error_Handler();
  }

  /* -2- Enable HSI Oscillator, select it as PLL source and finally activate the PLL */
  RCC_OscInitStruct.OscillatorType       = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState             = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue  = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState         = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource        = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLM       = RCC_PLLM_DIV4;
  RCC_OscInitStruct.PLL.PLLN       = 70;
  RCC_OscInitStruct.PLL.PLLP       = RCC_PLLP_DIV10;
  RCC_OscInitStruct.PLL.PLLQ       = RCC_PLLQ_DIV5;
  RCC_OscInitStruct.PLL.PLLR       = RCC_PLLR_DIV5;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    /* Initialization Error */
    Error_Handler();
  }

  /* -3- Select the PLL as system clock source and configure the HCLK and PCLK1 clocks dividers */
  RCC_ClkInitStruct.ClockType       = (RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_PCLK1);
  RCC_ClkInitStruct.SYSCLKSource    = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider   = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider  = RCC_HCLK_DIV1;
  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    /* Initialization Error */
    Error_Handler();
  }

  /* -4- Optional: Disable HSE  Oscillator (if the HSE  is no more needed by the application) */
  RCC_OscInitStruct.OscillatorType  = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState        = RCC_HSE_OFF;
  RCC_OscInitStruct.PLL.PLLState    = RCC_PLL_NONE;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    /* Initialization Error */
    Error_Handler();
  }

  ClockSourceStatus = 2;

}

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  ClockSourceStatus = 0xE;
  /* Turn LED3 on */
  BSP_LED_On(LED3);
  /* User can add his own implementation to report the HAL error return state */
  while (1)
  {
  }
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
