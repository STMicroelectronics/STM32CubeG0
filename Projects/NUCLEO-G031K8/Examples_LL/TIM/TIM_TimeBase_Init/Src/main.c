/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    Examples_LL/TIM/TIM_TimeBase_Init/Src/main.c
  * @author  MCD Application Team
  * @brief   This example describes how to use a timer instance to generate a 
  *          time base using the STM32G0xx TIM LL API.
  *          Peripheral initialization done using LL unitary services functions.
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
/* Number of time base frequencies */
#define TIM_BASE_FREQ_NB 10
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
static uint32_t tim_prescaler = 0;
static uint32_t tim_period = 0;
static uint32_t TimOutClock = 1;

/* Actual autoreload value multiplication factor */
static uint8_t AutoreloadMult = 1;

/* Debounce delay */
static uint32_t Wait200ms = 0;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_TIM1_Init(void);
/* USER CODE BEGIN PFP */
static void Configure_EXTI(void);
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
  /* Set the pre-scaler value to have TIM1 counter clock equal to 10 kHz      */
  /*
   In this example TIM1 input clock TIM1CLK is set to APB2 clock (PCLK2),   
   since APB2 pre-scaler is equal to 1.                                     
      TIM1CLK = PCLK2                                                       
      PCLK1 = HCLK                                                          
      => TIM1CLK = SystemCoreClock (56 MHz)                                 
  */
  tim_prescaler = __LL_TIM_CALC_PSC(SystemCoreClock, 10000);

  /* TIM1CLK = SystemCoreClock / (APB prescaler & multiplier)              */
  TimOutClock = SystemCoreClock/1;
  tim_period = __LL_TIM_CALC_ARR(TimOutClock, tim_prescaler, 10);
  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_TIM1_Init();
  /* USER CODE BEGIN 2 */
 /* Configure the EXTI Line on User Button */
  Configure_EXTI();
  
  /* Clear the update flag */
  LL_TIM_ClearFlag_UPDATE(TIM1);

  /* Enable the update interrupt */
  LL_TIM_EnableIT_UPDATE(TIM1);

  /* Enable counter */
  LL_TIM_EnableCounter(TIM1);
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  /* Configure the Cortex-M SysTick source to have 1ms time base */ 
  LL_Init1msTick(SystemCoreClock);
  
  while (1)
  {
    if (Wait200ms == 1)
    {
      /* Wait 200 ms */
      LL_mDelay(200);
      
      /* Clear debounce flag */
      Wait200ms = 0;
    }
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
  if(LL_FLASH_GetLatency() != LL_FLASH_LATENCY_2)
  {
    Error_Handler();  
  };

  /* HSI configuration and activation */
  LL_RCC_HSI_Enable();
  while(LL_RCC_HSI_IsReady() != 1)
  {
  };

  /* Main PLL configuration and activation */
  LL_RCC_PLL_ConfigDomain_SYS(LL_RCC_PLLSOURCE_HSI, LL_RCC_PLLM_DIV_1, 8, LL_RCC_PLLR_DIV_2);
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

  LL_Init1msTick(64000000);

  LL_SYSTICK_SetClkSource(LL_SYSTICK_CLKSOURCE_HCLK);
  /* Update CMSIS variable (which can be updated also through SystemCoreClockUpdate function) */
  LL_SetSystemCoreClock(64000000);
  LL_RCC_SetTIMClockSource(LL_RCC_TIM1_CLKSOURCE_PCLK1);
}

/**
  * @brief TIM1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM1_Init(void)
{

  /* USER CODE BEGIN TIM1_Init 0 */

  /* USER CODE END TIM1_Init 0 */

  LL_TIM_InitTypeDef TIM_InitStruct = {0};
  LL_TIM_BDTR_InitTypeDef TIM_BDTRInitStruct = {0};

  /* Peripheral clock enable */
  LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_TIM1);

  /* TIM1 interrupt Init */
  NVIC_SetPriority(TIM1_BRK_UP_TRG_COM_IRQn, 0);
  NVIC_EnableIRQ(TIM1_BRK_UP_TRG_COM_IRQn);

  /* USER CODE BEGIN TIM1_Init 1 */

  /* USER CODE END TIM1_Init 1 */
  TIM_InitStruct.Prescaler = tim_prescaler;
  TIM_InitStruct.CounterMode = LL_TIM_COUNTERMODE_UP;
  TIM_InitStruct.Autoreload = tim_period;
  TIM_InitStruct.ClockDivision = LL_TIM_CLOCKDIVISION_DIV1;
  TIM_InitStruct.RepetitionCounter = 0;
  LL_TIM_Init(TIM1, &TIM_InitStruct);
  LL_TIM_DisableARRPreload(TIM1);
  LL_TIM_SetClockSource(TIM1, LL_TIM_CLOCKSOURCE_INTERNAL);
  LL_TIM_SetTriggerOutput(TIM1, LL_TIM_TRGO_RESET);
  LL_TIM_SetTriggerOutput2(TIM1, LL_TIM_TRGO2_RESET);
  LL_TIM_DisableMasterSlaveMode(TIM1);
  TIM_BDTRInitStruct.BreakAFMode = LL_TIM_BREAK_AFMODE_INPUT;
  TIM_BDTRInitStruct.Break2AFMode = LL_TIM_BREAK_AFMODE_INPUT;
  LL_TIM_BDTR_Init(TIM1, &TIM_BDTRInitStruct);
  /* USER CODE BEGIN TIM1_Init 2 */

  /* USER CODE END TIM1_Init 2 */

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
  * @brief  This function configures EXTI Line as Button
  * @note   Peripheral configuration is minimal configuration from reset values.  
  * @param  None
  * @retval None
  */
static void Configure_EXTI()
{
  /* -1- GPIO Config */
  /* Enable GPIO Clock (to be able to program the configuration registers) */
  VIRTUAL_BUTTON_GPIO_CLK_ENABLE();
  /* Configure IO */
  LL_GPIO_SetPinMode(VIRTUAL_BUTTON_GPIO_PORT, VIRTUAL_BUTTON_PIN, LL_GPIO_MODE_INPUT);
  LL_GPIO_SetPinPull(VIRTUAL_BUTTON_GPIO_PORT, VIRTUAL_BUTTON_PIN, LL_GPIO_PULL_UP); 

  /* -2- Connect External Line to the GPIO*/
  VIRTUAL_BUTTON_SYSCFG_SET_EXTI();

  /*-3- Enable a falling trigger External line 15 Interrupt */
  VIRTUAL_BUTTON_EXTI_LINE_ENABLE();
  VIRTUAL_BUTTON_EXTI_RISING_TRIG_ENABLE();
  
  /*-4- Configure NVIC for EXTI4_15_IRQn */
  NVIC_EnableIRQ(VIRTUAL_BUTTON_EXTI_IRQn); 
  NVIC_SetPriority(VIRTUAL_BUTTON_EXTI_IRQn,0);
}


/******************************************************************************/
/*   USER IRQ HANDLER TREATMENT                                               */
/******************************************************************************/
/**
  * @brief  Update the timer update event period
  * @param  None
  * @retval None
  */
void UserButton_Callback(void)
{
  if (Wait200ms == 0)
  {
    /* Set debounce flag */
    Wait200ms = 1;
    
    /* Change the update event period by modifying the autoreload value.        */
    /* In up-counting update event is generated at each counter overflow (when  */
    /* the counter reaches the auto-reload value).                              */
    /* Update event period is calculated as follows:                            */
    /*   Update_event = TIM1CLK /((PSC + 1)*(ARR + 1)*(RCR + 1))                */
    /*   where TIM1CLK is 64 MHz                                                */
    AutoreloadMult = AutoreloadMult % TIM_BASE_FREQ_NB;
    LL_TIM_SetAutoReload(TIM1, tim_period * (AutoreloadMult +1));
    
    /* Force update generation */
    LL_TIM_GenerateEvent_UPDATE(TIM1);
    
    AutoreloadMult++;
  }
}

/**
  * @brief  Timer update interrupt processing
  * @param  None
  * @retval None
  */
void TimerUpdate_Callback(void)
{
  LL_GPIO_TogglePin(LED3_GPIO_Port, LED3_Pin);  
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
     tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
