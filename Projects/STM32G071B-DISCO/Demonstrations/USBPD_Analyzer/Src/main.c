/**
  ******************************************************************************
  * @file    main.c
  * @author  MCD Application Team
  * @brief   USBPD demo main file
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2019 STMicroelectronics.
  * All rights reserved.</center></h2>
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
#include "stm32g0xx_ll_utils.h"
#include "stm32g0xx_ll_system.h"
#include "stm32g0xx_ll_rcc.h"
#include "stm32g0xx_ll_cortex.h"
#include "cmsis_os.h"
#include "demo_disco.h"
#include "usbpd_trace.h"
#include "stm32g071b_discovery.h"
#include "stm32g071b_discovery_pwrmon.h"
#include "stm32g0xx_ll_gpio.h"
#include "usbpd_dpm_user.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define _HSE_ENABLE 1

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/

/* Private function prototypes -----------------------------------------------*/
static void SystemClock_Config(void);

/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Main program
  * @param  None
  * @retval None
  */
int main(void)
{
  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();
  
  /* Configure the system clock */
  SystemClock_Config();

  HAL_NVIC_SetPriority(SysTick_IRQn, TICK_INT_PRIORITY ,0U);
  LL_SYSTICK_EnableIT();
  DOOR_SENSE_GPIO_CLK_ENABLE();
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  GPIO_InitStruct.Pin = DOOR_SENSE_PIN;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  HAL_GPIO_Init(DOOR_SENSE_PORT, &GPIO_InitStruct);

  BSP_LED_Init(LED4);
  BSP_LED_Init(LED5);
  BSP_LED_Init(LED6);
  BSP_LED_Init(LED7);
  HAL_Delay(1);

  if( GPIO_PIN_SET == HAL_GPIO_ReadPin(DOOR_SENSE_PORT, DOOR_SENSE_PIN)) /* we are in SPY mode ? */
  {
    BSP_LED_On(LED6);

    BSP_MOSFET_Init(MOSFET_ENCC1);
    BSP_MOSFET_Init(MOSFET_ENCC2);
    BSP_MOSFET_Init(MOSFET_RD_CC2);
    BSP_MOSFET_On(MOSFET_ENCC1);
    BSP_MOSFET_On(MOSFET_ENCC2);
    BSP_MOSFET_On(MOSFET_RD_CC1);
    /* Start in spy mode */
    /* Initialize spy MMI */
    USBPD_TRACE_Init();
    DEMO_InitBSP();
    DEMO_InitTask(MODE_SPY);
    /* Initialize the TRACE */
    /* Start the scheduler */
    osKernelStart();
  }

  /* Global Init of USBPD HW */
  USBPD_HW_IF_GlobalHwInit();

#if defined(_GUI_INTERFACE)
  /* Initialize GUI */
  GUI_Init(BSP_GetHWBoardVersionName, BSP_GetPDTypeName);
#endif /* _GUI_INTERFACE */

  /* Initialize the Device Policy Manager */
  if( USBPD_ERROR == USBPD_DPM_InitCore())
  {
    /* error the RTOS can't be started  */
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
    /* error the RTOS can't be started  */
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
  *            HSI clock division factor      = 1  
  *            HSI Frequency(Hz)              = 16000000
  *            Flash Latency(WS)              = 2
  *            PLLM                           = 1
  *            PLLN                           = 8
  *            PLLR                           = 2
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
  LL_RCC_PLL_ConfigDomain_SYS(LL_RCC_PLLSOURCE_HSI, LL_RCC_PLLM_DIV_1, 8, LL_RCC_PLLR_DIV_2);
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

  /* Set systick to 1ms in using frequency set to 64MHz */
  /* This frequency can be calculated through LL RCC macro */
  /* ex: __LL_RCC_CALC_PLLCLK_FREQ(__LL_RCC_CALC_HSI_FREQ(),
                                  LL_RCC_PLLM_DIV_1, 8, LL_RCC_PLLR_DIV_2)*/
  LL_Init1msTick(64000000);

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
void assert_failed(char* file, uint32_t line)
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
