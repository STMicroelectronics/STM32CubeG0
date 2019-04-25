/**
  ******************************************************************************
  * @file    main.c
  * @author  MCD Application Team
  * @brief   USBPD demo main file
  ******************************************************************************
   * @attention
  *
  * Copyright (c) 2018 STMicroelectronics. All rights reserved.
  *
  * This software component is licensed by ST under Ultimate Liberty license
  * SLA0044, the "License"; You may not use this file except in compliance with
  * the License. You may obtain a copy of the License at:
  *                             www.st.com/SLA0044
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "stm32g0xx.h"
#include "stm32g0xx_ll_bus.h"
#include "stm32g0xx_ll_utils.h"
#include "stm32g0xx_ll_system.h"
#include "stm32g0xx_ll_rcc.h"
#include "stm32g081b_eval.h"
#include "stm32g081b_eval_lcd.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define _HSE_ENABLE 1

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);

/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Main program
  * @param  None
  * @retval None
  */
int main(void)
{
  /* Get back current CPU clock config */
  SystemCoreClockUpdate();

  if(SystemCoreClock != 64000000)
  {
    /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
    HAL_Init();

    /* Configure the system clock @ CPU max */
    SystemClock_Config();
  }

  /* ## Backup register access ## */
  RCC->APBENR1 |= RCC_APBENR1_PWREN;
  LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_CRC);

  /* Global Init of USBPD HW */
  USBPD_HW_IF_GlobalHwInit();

#if defined(_GUI_INTERFACE)
  /* Initialize GUI */
  GUI_Init(BSP_GetHWBoardVersionName, BSP_GetPDTypeName);
#endif /* _GUI_INTERFACE */

  /* Initialize the Device Policy Manager */
  if( USBPD_ERROR == USBPD_DPM_InitCore())
  {
    /* error on core init  */
    while(1);
  }

  /* Initialise the DPM application */
  if (USBPD_OK != USBPD_DPM_UserInit())
  {
    while(1);
  }

  /* Initialize the Device Policy Manager */
  if( USBPD_ERROR == USBPD_DPM_InitOS())
  {
    /* error the OS init  */
    while(1);
  }

  USBPD_DPM_Run();
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
  *            PLLR                           = 4
  *            Flash Latency(WS)              = 2
  * @param  None
  * @retval None
  */
void SystemClock_Config(void)
{
  /* HSI configuration and activation */
  LL_RCC_HSI_Enable();
  while(LL_RCC_HSI_IsReady() != 1)
  {
  };

  /* Main PLL configuration and activation */
  LL_RCC_PLL_ConfigDomain_SYS(LL_RCC_PLLSOURCE_HSI, LL_RCC_PLLM_DIV_4, 64, LL_RCC_PLLR_DIV_4);
  LL_RCC_PLL_Enable();
  LL_RCC_PLL_EnableDomain_SYS();
  while(LL_RCC_PLL_IsReady() != 1)
  {
  };

  LL_FLASH_SetLatency(LL_FLASH_LATENCY_2);
  /* Sysclk activation on the main PLL */
  LL_RCC_SetSysClkSource(LL_RCC_SYS_CLKSOURCE_PLL);
  while(LL_RCC_GetSysClkSource() != LL_RCC_SYS_CLKSOURCE_STATUS_PLL)
  {
  };

  /* Set AHB prescaler*/
  LL_RCC_SetAHBPrescaler(LL_RCC_SYSCLK_DIV_1);

  /* Set APB1 prescaler*/
  LL_RCC_SetAPB1Prescaler(LL_RCC_APB1_DIV_1);

  /* Set systick to 1ms in using frequency set to 64MHz */
  /* This frequency can be calculated through LL RCC macro */
  /* ex: __LL_RCC_CALC_PLLCLK_FREQ(__LL_RCC_CALC_HSI_FREQ(),
  LL_RCC_PLLM_DIV_4, 64, LL_RCC_PLLR_DIV_4)*/
  LL_Init1msTick(64000000);
  SysTick->CTRL|= SysTick_CTRL_TICKINT_Msk;
  HAL_NVIC_SetPriority(SysTick_IRQn, TICK_INT_PRIORITY ,0U);

  /* Update CMSIS variable (which can be updated also through SystemCoreClockUpdate function) */
  LL_SetSystemCoreClock(64000000);
}

#ifdef  USE_FULL_ASSERT

/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t* file, uint32_t line)
{
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

  /* Infinite loop */
  while (1)
  {
  }
}
#endif

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
