/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    FatFs/FatFs_MultiAccess_RTOS/Src/main.c
  * @author  MCD Application Team
  * @brief   Main program body
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
#include "main.h"
#include "cmsis_os.h"
#include "app_fatfs.h"
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
#define blckqSTACK_SIZE   configMINIMAL_STACK_SIZE
/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
osThreadId_t StartThreadHandle;
osThreadId_t ConcurrentThreadHandle;
static osThreadAttr_t attr = {
                        .priority = osPriorityNormal,
                        .stack_size = 8 * configMINIMAL_STACK_SIZE,
                      };

osMessageQId AppliEvent;
osMessageQId DiskEvent;
uint32_t   DiskQueueMsg = 0;
int32_t ProcessStatus = 0;
/* USER CODE BEGIN PV */
__IO uint32_t OsStatus = 0;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void StartThread(void *argument);
static void ConcurrentThread(void *argument);
void Success_Handler(void);
/* USER CODE BEGIN PFP */

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
  /* Configure LED_OK and LED_ERROR */
  BSP_LED_Init(LED_OK);
  BSP_LED_Init(LED_ERROR);
  
  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  /* USER CODE BEGIN 2 */
  /* Initialize the micro SD Card */
  BSP_SD_Init();

  /* USER CODE END 2 */
  if(MX_FATFS_Init() == APP_OK)
  {
    BSP_LED_Off(LED_ERROR);
    BSP_LED_Toggle(LED_OK);  
    
    HAL_Delay(200);
  }
  else
  {
    Error_Handler();
  }
  
  /* Init scheduler */
  osKernelInitialize();
  /* USER CODE BEGIN RTOS_MUTEX */

  /* USER CODE END RTOS_MUTEX */

  /* USER CODE BEGIN RTOS_SEMAPHORES */

  /* USER CODE END RTOS_SEMAPHORES */

  /* USER CODE BEGIN RTOS_TIMERS */

  /* USER CODE END RTOS_TIMERS */

  /* Create the thread(s) */
  /* definition and creation of USER_Thread */
  attr.name = "USER_Thread";
  StartThreadHandle = osThreadNew(StartThread, NULL, (const osThreadAttr_t *)&attr);
  /* definition and creation of USER_ConcurrentThread */
  attr.name = "USER_ConcurrentThread";
  ConcurrentThreadHandle = osThreadNew(ConcurrentThread, NULL, (const osThreadAttr_t *)&attr);
  /* USER CODE BEGIN RTOS_THREADS */

  /* USER CODE END RTOS_THREADS */

  /* USER CODE BEGIN RTOS_QUEUES */
  /* Create Disk Queue */
  DiskEvent = osMessageQueueNew (1U, sizeof(uint16_t), NULL);
  /* USER CODE END RTOS_QUEUES */


  /* Start scheduler */
  osKernelStart();

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

/**
  * @brief  System Clock Configuration
  *         The system Clock is configured as follow : 
  *            System Clock source            = PLL (HSI)
  *            SYSCLK(Hz)                     = 64000000
  *            HCLK(Hz)                       = 64000000
  *            AHB Prescaler                  = 1
  *            APB1 Prescaler                 = 1
  *            HSI Frequency(Hz)              = 16000000
  *            Flash Latency(WS)              = 2
  *            PLLM                           = 1
  *            PLLN                           = 8
  *            PLLP                           = 2
  *            PLLQ                           = 2
  *            PLLR                           = 2
  *            Flash Latency(WS)              = 2
  * @param  None
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the main internal regulator output voltage 
  */
  HAL_PWREx_ControlVoltageScaling(PWR_REGULATOR_VOLTAGE_SCALE1);
  /** Initializes the CPU, AHB and APB busses clocks 
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSIDiv = RCC_HSI_DIV1;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLM = RCC_PLLM_DIV1;
  RCC_OscInitStruct.PLL.PLLN = 8;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = RCC_PLLQ_DIV2;
  RCC_OscInitStruct.PLL.PLLR = RCC_PLLR_DIV2;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB busses clocks 
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

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/* USER CODE BEGIN Header_SemaphoreThread1 */
/**
  * @brief  Function implementing the StartThread thread.
  * @param  argument: Not used
  * @retval None
  */
/* USER CODE END Header_SemaphoreThread1 */
static void StartThread(void *argument)
{

  /* USER CODE BEGIN 5 */
  int32_t ffStatus ;
  /* Infinite loop */
  while (1)
  {
    ffStatus = MX_FATFS_Process();
    if( ffStatus == APP_SD_UNPLUGGED)
    {
      BSP_LED_Off(LED_OK);
      BSP_LED_Toggle(LED_ERROR);
      HAL_Delay(200);
    }
    else if (ffStatus == APP_ERROR)
    {
      Error_Handler();
    }
    else if (ffStatus ==APP_OK)
    {
      osThreadSuspend (StartThreadHandle);
    }
  }
  /* USER CODE END 5 */
}

/* USER CODE BEGIN ConcurrentThread */
/**
* @brief Function implementing the ConcurrentThread thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END ConcurrentThread */
static void ConcurrentThread(void *argument)
{
  /* USER CODE BEGIN ConcurrentThread */
  osStatus_t status;

  while (1)
  {
    status = osMessageQueueGet(DiskEvent, &DiskQueueMsg, NULL, osWaitForever);
    if(status == osOK)
    {
      switch(DiskQueueMsg)
      {
      case DISK_READY_EVENT:
      FS_File2Operations();
        break;

      case DISK_REMOVE_EVENT:
        /* Unlink the USB disk I/O driver */
        MX_FATFS_DeInit();
        Success_Handler();
        break;

      default:
        break;
      }
    }
  }
  /* USER CODE END ConcurrentThread */
}

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  BSP_LED_Off(LED_OK);
  BSP_LED_On(LED_ERROR);

  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

/**
  * @brief  This function is executed in case of success occurrence.
  * @retval None
  */
void Success_Handler(void)
{
  /* USER CODE BEGIN Success_Handler */
  BSP_LED_On(LED_OK);
  BSP_LED_Off(LED_ERROR);
  while (1)
  {
  }
  /* USER CODE END Success_Handler */
}

#ifdef  USE_FULL_ASSERT

/**
  * @brief  Reports the name of the source file and the source line number
  *   where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(char *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

  /* Infinite loop */
  while (1)
  {}
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
