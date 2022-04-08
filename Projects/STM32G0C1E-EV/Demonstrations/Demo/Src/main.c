/**
  ******************************************************************************
  * @file    main.c
  * @author  MCD Application Team
  * @brief   This file provides main program functions
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
#include <string.h>
#include "main.h"
#include "SubDemoLegacy.h"
#include "SubDemoUcpd.h"
#include "stm32g0c1e_eval.h"
#include "stm32g0c1e_eval_lcd.h"
#include "k_config.h"
#include "k_window.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/

/* Private variables ---------------------------------------------------------*/
SubDemoApi_t SubDemoApi = {NULL};

/* Exported variables --------------------------------------------------------*/
SubDemoId_e SubDemoId = SUB_DEMO_LEGACY;
uint16_t DaughterBoardDependentExtiLine = SD_DETECT_PIN;

RTC_HandleTypeDef hrtc = {0};
DAC_HandleTypeDef hdac = {0};
ADC_HandleTypeDef hadc1 = {0};
DMA_HandleTypeDef hdma_adc1 = {0};

/* Exported functions --------------------------------------------------------*/
/**
* @brief  Main program
* @param  None
* @retval int
*/
int main(void)
{
  DBId_TypeDef daughter_board_id;
  
  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* Configure the system clock @ 64 MHz (PLL ON) */
  SystemClock_Config_64MHz();

  /* LED init */
  BSP_LED_Init(LED1);
  BSP_LED_Init(LED2);
  BSP_LED_Init(LED3);
  BSP_LED_Init(LED4);

  /* Get the daugther board id */
  if (BSP_DB_GetId(&daughter_board_id) !=0)
  {
    BSP_LCD_Init();
    BSP_LCD_Clear(LCD_COLOR_WHITE);
    kWindow_Error("Failed to identify\ndaughter board !\n");
    Error_Handler();
  }

  /* Set the sub-demo API descriptor */
  switch (daughter_board_id)
  {
  case DB_ID_UCPD_AB:
  case DB_ID_UCPD_C:
    SubDemoId = SUB_DEMO_UCPD;
    DaughterBoardDependentExtiLine = DET_HPD_SOURCE_PIN;
    SubDemoApi = SubDemoApiUcpd;
    break;
  default:
    SubDemoId = SUB_DEMO_LEGACY;
    DaughterBoardDependentExtiLine = SD_DETECT_PIN;
    SubDemoApi = SubDemoApiLegacy;
    break;
  }
  
  /* Launch the sub-demo execution */
  SubDemoApi.pfnSubDemoExec();
  
  /* we should not reach here */
  while(1);
}

/**
  * @brief  System Clock Configuration
  *         The system Clock is configured as follow :
  *            System Clock source            = PLL (HSI)
  *            SYSCLK(Hz)                     = 64000000
  *            HCLK(Hz)                       = 64000000
  *            AHB Prescaler                  = 1
  *            APB1 Prescaler                 = 1
  *            HSI Frequency(Hz)              = 16000000
  *            PLLM                           = 4
  *            PLLN                           = 64
  *            PLLP                           = 16
  *            PLLQ                           = 4
  *            PLLR                           = 4
  *            Flash Latency(WS)              = 2
  * @param  None
  * @retval None
  */
void SystemClock_Config_64MHz(void)
{
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};

  /* Set HSI as PLL source to use it for max 64MHz frequency as SYSCLK source */
  RCC_OscInitStruct.OscillatorType      = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.HSIState            = RCC_HSI_ON;
  RCC_OscInitStruct.HSIDiv              = RCC_HSI_DIV1;
  RCC_OscInitStruct.PLL.PLLState        = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource       = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLM            = RCC_PLLM_DIV4;
  RCC_OscInitStruct.PLL.PLLN            = 64;
  RCC_OscInitStruct.PLL.PLLP            = RCC_PLLP_DIV16;
  RCC_OscInitStruct.PLL.PLLQ            = RCC_PLLQ_DIV4;
  RCC_OscInitStruct.PLL.PLLR            = RCC_PLLR_DIV4;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct)!= HAL_OK)
  {
    while(1);
  }

  /* Select PLL as system clock source and configure the HCLK, PCLK1 clocks dividers */
  RCC_ClkInitStruct.ClockType      = (RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_PCLK1);
  RCC_ClkInitStruct.SYSCLKSource   = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider  = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2)!= HAL_OK)
  {
    while(1);
  }
}

void Error_Handler(void)
{
  while(1)
  {
    HAL_Delay(100);
    BSP_LED_Toggle(LED1);
    BSP_LED_Toggle(LED2);
    BSP_LED_Toggle(LED3);
    BSP_LED_Toggle(LED4);
  }
}

#ifdef USE_FULL_ASSERT
/**
* @brief  assert_failed
*         Reports the name of the source file and the source line number
*         where the assert_param error has occurred.
* @param  File: pointer to the source file name
* @param  Line: assert_param error line source number
* @retval None
*/
void assert_failed(uint8_t* file, uint32_t line)
{
  /* User can add his own implementation to report the file name and line
  number,ex: printf("Wrong parameters value: file %s on line %d\r\n",
  file, line) */

  /* Infinite loop */
  while (1)
  {}
}

#endif

/* Private functions ---------------------------------------------------------*/
/**
  * @brief  EXTI line rising edge detection callback.
  * @param  GPIO_Pin Specifies the port pin connected to corresponding EXTI line.
  * @retval None
  */
void HAL_GPIO_EXTI_Rising_Callback(uint16_t GPIO_Pin)
{
  /* Launch the sub-demo execution */
  if (SubDemoApi.pfnSubDemoExtiRisingEdgeDetectionManagement != NULL)
  {
    SubDemoApi.pfnSubDemoExtiRisingEdgeDetectionManagement(GPIO_Pin);
  }
}

/**
  * @brief  EXTI line falling edge detection callback.
  * @param  GPIO_Pin Specifies the port pin connected to corresponding EXTI line.
  * @retval None
  */
void HAL_GPIO_EXTI_Falling_Callback(uint16_t GPIO_Pin)
{
  /* Launch the sub-demo execution */
  if (SubDemoApi.pfnSubDemoExtiFallingEdgeDetectionManagement != NULL)
  {
    SubDemoApi.pfnSubDemoExtiFallingEdgeDetectionManagement(GPIO_Pin);
  }
}
