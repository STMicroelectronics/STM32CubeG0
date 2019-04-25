/**
  ******************************************************************************
  * @file    Examples_LL/TIM/TIM_TimeBase/Src/main.c
  * @author  MCD Application Team
  * @brief   This example describes how to use a timer instance to generate a 
  *          time base using the STM32G0xx TIM LL API.
  *          Peripheral initialization done using LL unitary services functions.
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
#include "main.h"

/** @addtogroup STM32G0xx_LL_Examples
  * @{
  */

/** @addtogroup TIM_TimeBase
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define BUTTON_MODE_GPIO  0
#define BUTTON_MODE_EXTI  1

/* Number of time base frequencies */
#define TIM_BASE_FREQ_NB 10

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Initial autoreload value */
static uint32_t InitialAutoreload = 0;

/* Actual autoreload value multiplication factor */
static uint8_t AutoreloadMult = 1;

/* TIM1 Clock */
static uint32_t TimOutClock = 1;

/* Private function prototypes -----------------------------------------------*/
__STATIC_INLINE void     SystemClock_Config(void);
__STATIC_INLINE void     Configure_TIMTimeBase(void);
__STATIC_INLINE void     LED_Init(void);
__STATIC_INLINE void     UserButton_Init(void);

/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Main program
  * @param  None
  * @retval None
  */
int main(void)
{
  /* Configure the system clock to 56 MHz */
  SystemClock_Config();

  /* Initialize LED4 */
  LED_Init();

  /* Initialize button in EXTI mode */
  UserButton_Init();

  /* Configure the timer time base */
  Configure_TIMTimeBase();

  /* Infinite loop */
  while (1)
  {
  }
}

/**
  * @brief  Configures the timer as a time base.
  * @note   Peripheral configuration is minimal configuration from reset values.
  *         Thus, some useless LL unitary functions calls below are provided as
  *         commented examples - setting is default configuration from reset.
  * @param  None
  * @retval None
  */
__STATIC_INLINE void  Configure_TIMTimeBase(void)
{
  /* Enable the timer peripheral clock */
  LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_TIM1); 
  
  /* Set counter mode */
  /* Reset value is LL_TIM_COUNTERMODE_UP */
  //LL_TIM_SetCounterMode(TIM1, LL_TIM_COUNTERMODE_UP);

  /* Set the pre-scaler value to have TIM1 counter clock equal to 10 kHz      */
  /*
   In this example TIM1 input clock TIM1CLK is set to APB2 clock (PCLK2),   
   since APB2 pre-scaler is equal to 1.                                     
      TIM1CLK = PCLK2                                                       
      PCLK1 = HCLK                                                          
      => TIM1CLK = SystemCoreClock (56 MHz)                                 
  */
  LL_TIM_SetPrescaler(TIM1, __LL_TIM_CALC_PSC(SystemCoreClock, 10000));
  
  /* Set the auto-reload value to have an initial update event frequency of 10 Hz */
    /* TIM1CLK = SystemCoreClock / (APB prescaler & multiplier)                 */
  TimOutClock = SystemCoreClock/1;
  
  InitialAutoreload = __LL_TIM_CALC_ARR(TimOutClock, LL_TIM_GetPrescaler(TIM1), 10);
  LL_TIM_SetAutoReload(TIM1, InitialAutoreload);
  
  /* Enable the update interrupt */
  LL_TIM_EnableIT_UPDATE(TIM1);
  
  /* Configure the NVIC to handle TIM1 update interrupt */
  NVIC_SetPriority(TIM1_BRK_UP_TRG_COM_IRQn, 0);
  NVIC_EnableIRQ(TIM1_BRK_UP_TRG_COM_IRQn);
  
  /* Enable counter */
  LL_TIM_EnableCounter(TIM1);
  
  /* Force update generation */
  LL_TIM_GenerateEvent_UPDATE(TIM1);
}

/**
  * @brief  Initialize LED4.
  * @param  None
  * @retval None
  */
__STATIC_INLINE void LED_Init(void)
{
  /* Enable the LED4 Clock */
  LED4_GPIO_CLK_ENABLE();

  /* Configure IO in output push-pull mode to drive external LED4 */
  LL_GPIO_SetPinMode(LED4_GPIO_PORT, LED4_PIN, LL_GPIO_MODE_OUTPUT);
  /* Reset value is LL_GPIO_OUTPUT_PUSHPULL */
  //LL_GPIO_SetPinOutputType(LED4_GPIO_PORT, LED4_PIN, LL_GPIO_OUTPUT_PUSHPULL);
  /* Reset value is LL_GPIO_SPEED_FREQ_LOW */
  //LL_GPIO_SetPinSpeed(LED4_GPIO_PORT, LED4_PIN, LL_GPIO_SPEED_FREQ_LOW);
  /* Reset value is LL_GPIO_PULL_NO */
  //LL_GPIO_SetPinPull(LED4_GPIO_PORT, LED4_PIN, LL_GPIO_PULL_NO);
}

/**
  * @brief  Configures User push-button in GPIO or EXTI Line Mode.
  * @param  None
  * @retval None
  */
__STATIC_INLINE void UserButton_Init(void)
{
  /* Enable the BUTTON Clock */
  USER_BUTTON_GPIO_CLK_ENABLE();
  
  /* Configure GPIO for BUTTON */
  LL_GPIO_SetPinMode(USER_BUTTON_GPIO_PORT, USER_BUTTON_PIN, LL_GPIO_MODE_INPUT);
  LL_GPIO_SetPinPull(USER_BUTTON_GPIO_PORT, USER_BUTTON_PIN, USER_BUTTON_GPIO_PUPD);
  
  /* Connect External Line to the GPIO*/
  USER_BUTTON_SYSCFG_SET_EXTI();
    
  /* Enable a rising trigger EXTI line 13 Interrupt */
  USER_BUTTON_EXTI_LINE_ENABLE();
  USER_BUTTON_EXTI_FALLING_TRIG_ENABLE();
    
  /* Configure NVIC for USER_BUTTON_EXTI_IRQn */
  NVIC_EnableIRQ(USER_BUTTON_EXTI_IRQn); 
  NVIC_SetPriority(USER_BUTTON_EXTI_IRQn,0x03);  
}

/**
  * @brief  System Clock Configuration
  *         The system Clock is configured as follow : 
  *            System Clock source            = PLL (HSI)
  *            SYSCLK(Hz)                     = 56000000
  *            HCLK(Hz)                       = 56000000
  *            AHB Prescaler                  = 1
  *            APB1 Prescaler                 = 1
  *            HSI clock division factor      = 1  
  *            HSI Frequency(Hz)              = 16000000
  *            Flash Latency(WS)              = 2
  *            PLLM                           = 4
  *            PLLN                           = 70
  *            PLLR                           = 5
  * @param  None
  * @retval None
  */
void SystemClock_Config(void)
{
  LL_FLASH_SetLatency(LL_FLASH_LATENCY_2);
  /* HSI configuration and activation */
  LL_RCC_SetHSIDiv(LL_RCC_HSI_DIV_1);
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

  /* Sysclk activation on the main PLL */

  LL_RCC_SetSysClkSource(LL_RCC_SYS_CLKSOURCE_PLL);
  while(LL_RCC_GetSysClkSource() != LL_RCC_SYS_CLKSOURCE_STATUS_PLL)
  {
  };

  /* Set AHB prescaler*/
  LL_RCC_SetAHBPrescaler(LL_RCC_SYSCLK_DIV_1);

  /* Set APB1 prescaler*/
  LL_RCC_SetAPB1Prescaler(LL_RCC_APB1_DIV_1);

  /* Set systick to 1ms in using frequency set to 56MHz */
  /* This frequency can be calculated through LL RCC macro */
  /* ex: __LL_RCC_CALC_PLLCLK_FREQ(__LL_RCC_CALC_HSI_FREQ(),
                                  LL_RCC_PLLM_DIV_4, 70, LL_RCC_PLLR_DIV_5)*/
  LL_Init1msTick(56000000);

  /* Update CMSIS variable (which can be updated also through SystemCoreClockUpdate function) */
  LL_SetSystemCoreClock(56000000);
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
  /* Change the update event period by modifying the autoreload value.        */
  /* In up-counting update event is generated at each counter overflow (when  */
  /* the counter reaches the auto-reload value).                              */
  /* Update event period is calculated as follows:                            */
  /*   Update_event = TIM1CLK /((PSC + 1)*(ARR + 1)*(RCR + 1))                */
  /*   where TIM1CLK is 56 MHz                                                */
  AutoreloadMult = AutoreloadMult % TIM_BASE_FREQ_NB;
  LL_TIM_SetAutoReload(TIM1, InitialAutoreload * (AutoreloadMult +1));

  /* Force update generation */
  LL_TIM_GenerateEvent_UPDATE(TIM1);

  AutoreloadMult++;
}

/**
  * @brief  Timer update interrupt processing
  * @param  None
  * @retval None
  */
void TimerUpdate_Callback(void)
{
  LL_GPIO_TogglePin(LED4_GPIO_PORT, LED4_PIN);  
}

#ifdef  USE_FULL_ASSERT

/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(char *file, uint32_t line)
{
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d", file, line) */

  /* Infinite loop */
  while (1)
  {
  }
}
#endif

/**
  * @}
  */

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
