/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : SubDemoUcpd.c
  * @brief          : This file implements the UCPD sub-demo API
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
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "SubDemoUcpd.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "main.h"
#include "demo_application.h"
#include "stm32g0xx.h"
#include "stm32g0xx_ll_bus.h"
#include "stm32g0xx_ll_utils.h"
#include "stm32g0xx_ll_system.h"
#include "stm32g0xx_ll_rcc.h"
#include "stm32g0c1e_eval.h"
#include "stm32g0c1e_eval_lcd.h"
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

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/

/* USER CODE BEGIN PFP */
static uint32_t SubDemoUcpdInit(void);
static uint32_t SubDemoUcpdDeInit(void);
static void SubDemoUcpdExec(void);
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

SubDemoApi_t SubDemoApiUcpd = 
{
  .pfnSubDemoInit         = SubDemoUcpdInit,
  .pfnSubDemoDeInit       = SubDemoUcpdDeInit,
  .pfnSubDemoExec         = SubDemoUcpdExec,
  .pfnSubDemoExtiRisingEdgeDetectionManagement  = DEMO_ExtiRisingEdgeDetection,
  .pfnSubDemoExtiFallingEdgeDetectionManagement = DEMO_ExtiFallingEdgeDetection
};

uint32_t SubDemoUcpdInit(void)
{
  return 0;
}

uint32_t SubDemoUcpdDeInit(void)
{
  return 0;
}

/**
  * @brief  The application entry point.
  * @retval uint32_t
  */
void SubDemoUcpdExec(void)
{
  /* USER CODE BEGIN 1 */
  /* USER CODE END 1 */

  /* USER CODE BEGIN Init */
  /* USER CODE END Init */

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* USER CODE BEGIN 2 */
  /* USER CODE END 2 */

  /* USBPD initialisation ---------------------------------*/
  /* Global Init of USBPD HW */
  USBPD_HW_IF_GlobalHwInit();

  /* Initialize the Device Policy Manager */
  if (USBPD_OK != USBPD_DPM_InitCore())
  {
    /* error on core init  */
    while(1);
  }

  /* Initialise the DPM application */
  if (USBPD_OK != USBPD_DPM_UserInit())
  {
    while(1);
  }

#if defined(_GUI_INTERFACE)
  /* Initialize GUI before retrieving PDO from RAM */
  GUI_Init(BSP_GetHWBoardVersionName, BSP_GetPDTypeName, HW_IF_PWR_GetVoltage, HW_IF_PWR_GetCurrent);
#endif /* _GUI_INTERFACE */

  /* USER CODE BEGIN RTOS_THREADS */
  /* add threads, ... */
  if (USBPD_OK != USBPD_DPM_InitOS())
  {
    /* error the RTOS can't be started  */
    while(1);
  }
  /* USER CODE END RTOS_THREADS */

  USBPD_DPM_Run();
  /* We should never get here as control is now taken by the scheduler */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
}
