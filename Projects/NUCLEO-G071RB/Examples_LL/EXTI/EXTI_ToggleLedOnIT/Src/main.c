/**
  ******************************************************************************
  * @file    Examples_LL/EXTI/EXTI_ToggleLedOnIT/Src/main.c
  * @author  MCD Application Team
  * @brief   This example describes how to configure the EXTI and use 
  *          GPIOs using the STM32G0xx LL API to toggles the available 
  *          users Leds on the board when User button is pressed.
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

/** @addtogroup EXTI_ToggleLedOnIT
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
void Configure_EXTI(void);
void LED_Init(void);

/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Main program
  * @param  None
  * @retval None
  */
int main(void)
{
  /* Configure the system clock to 16 MHz */
  SystemClock_Config();

  /* Initialize LED4 */
  LED_Init();
  
  /* Configure the EXTI Line on User Button */
  Configure_EXTI();

  /* Infinite loop */
  while (1)
  {
  }
}

/**
  * @brief  This function configures EXTI Line as Push-Button
  * @note   Peripheral configuration is minimal configuration from reset values.  
  * @param  None
  * @retval None
  */
void Configure_EXTI()
{
  /* -1- GPIO Config */
  /* Enable GPIO Clock (to be able to program the configuration registers) */
  USER_BUTTON_GPIO_CLK_ENABLE();
  /* Configure IO */
  LL_GPIO_SetPinMode(USER_BUTTON_GPIO_PORT, USER_BUTTON_PIN, LL_GPIO_MODE_INPUT);
  LL_GPIO_SetPinPull(USER_BUTTON_GPIO_PORT, USER_BUTTON_PIN, LL_GPIO_PULL_NO); 

  /* -2- Connect External Line to the GPIO*/
  USER_BUTTON_SYSCFG_SET_EXTI();
  
  /*-3- Enable a falling trigger EXTI line 13 Interrupt */
  USER_BUTTON_EXTI_LINE_ENABLE();
  USER_BUTTON_EXTI_FALLING_TRIG_ENABLE();
  
  /*-4- Configure NVIC for EXTI4_15_IRQn */
  NVIC_EnableIRQ(USER_BUTTON_EXTI_IRQn); 
  NVIC_SetPriority(USER_BUTTON_EXTI_IRQn,0);
}

/**
  * @brief  Initialize LED4.
  * @param  None
  * @retval None
  */
void LED_Init(void)
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
  * @brief  System Clock Configuration
  *         The system Clock is configured as follows :
  *            System Clock source            = HSI
  *            SYSCLK(Hz)                     = 16000000
  *            HCLK(Hz)                       = 16000000
  *            AHB Prescaler                  = 1
  *            APB1 Prescaler                 = 1
  *            HSI clock division factor      = 1  
  *            HSI Frequency(Hz)              = 16000000
  *            Flash Latency(WS)              = 0
  * @param  None
  * @retval None
  */
void SystemClock_Config(void)
{
  LL_FLASH_SetLatency(LL_FLASH_LATENCY_0);
  /* Set AHB prescaler*/
  LL_RCC_SetAHBPrescaler(LL_RCC_SYSCLK_DIV_1);

  /* Set APB1 prescaler*/
  LL_RCC_SetAPB1Prescaler(LL_RCC_APB1_DIV_1);

  /* HSI configuration and activation */
  LL_RCC_SetHSIDiv(LL_RCC_HSI_DIV_1);
  LL_RCC_HSI_Enable();
  while(LL_RCC_HSI_IsReady() != 1)
  {
  };

  LL_RCC_SetSysClkSource(LL_RCC_SYS_CLKSOURCE_HSI);
  while(LL_RCC_GetSysClkSource() != LL_RCC_SYS_CLKSOURCE_STATUS_HSI)
  {
  };

  /* Set systick to 1ms in using frequency set to 16MHz */
  LL_Init1msTick(16000000);

  /* Update CMSIS variable (which can be updated also through SystemCoreClockUpdate function) */
  LL_SetSystemCoreClock(16000000);
}

/******************************************************************************/
/*   USER IRQ HANDLER TREATMENT                                               */
/******************************************************************************/
/**
  * @brief  Function to manage IRQ Handler
  * @param  None
  * @retval None
  */
void UserButton_Callback(void)
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
