/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2019-2020 STMicroelectronics.
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

#define AES_PAYLOAD_SIZE_GCM          128  /* in 32-bit long words */
#define AES_PAYLOAD_SIZE_CCM            8  /* in 32-bit long words */
#define AES_ACTUAL_PAYLOAD_SIZE_CCM    17  /* Plaintext_CCM[] message is actually 17-byte long */

#define AES_HEADER_SIZE_CCM             3  /* in 32-bit long words */

#define TAG_SIZE                        4  /* in 32-bit long words */

#define KEY_256_SIZE                    8  /* in 32-bit long words */
#define IV_SIZE                         4  /* in 32-bit long words */

#define TIMEOUT_VALUE                0xFF

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
CRYP_HandleTypeDef hcryp;
__ALIGN_BEGIN static const uint32_t pKeyAES[4] __ALIGN_END = {
                            0xC939CC13,0x397C1D37,0xDE6AE0E1,0xCB7C423C};
__ALIGN_BEGIN static const uint32_t pInitVectAES[4] __ALIGN_END = {
                            0xB3D8CC01,0x7CBB89B3,0x9E0F67E2,0x00000002};
__ALIGN_BEGIN static const uint32_t HeaderAES[4] __ALIGN_END = {
                            0x24825602,0xbd12a984,0xe0092d3e,0x448eda5f};

/* USER CODE BEGIN PV */
uint32_t suspension_requested;
uint32_t IT_call_nb;              /* Counter of CRYP interruptions, used
                                   to indicate suspension occurrence when
                                   low priority data are entered
                                   under interruption                      */

uint32_t AES256Key_CCM[KEY_256_SIZE] = {
                            0xD346D11A, 0x7117CE04, 0x08089570, 0x7778287C,
                            0x40F5F473, 0xA9A8F2B1, 0x570F6137, 0x4669751A};

/* Low priority message GCM enciphered ===================================================================*/

uint32_t Plaintext_GCM[AES_PAYLOAD_SIZE_GCM] = {
                         0xc3b3c41f,0x113a31b7,0x3d9a5cd4,0x32103069, 0xc3b3c41f,0x113a31b7,0x3d9a5cd4,0x32103069,
                         0x24825602,0xbd12a984,0xe0092d3e,0x448eda5f, 0x24825602,0xbd12a984,0xe0092d3e,0x448eda5f,
                         0xc3b3c41f,0x113a31b7,0x3d9a5cd4,0x32103069, 0xc3b3c41f,0x113a31b7,0x3d9a5cd4,0x32103069,
                         0x24825602,0xbd12a984,0xe0092d3e,0x448eda5f, 0x24825602,0xbd12a984,0xe0092d3e,0x448eda5f,
                         0xc3b3c41f,0x113a31b7,0x3d9a5cd4,0x32103069, 0xc3b3c41f,0x113a31b7,0x3d9a5cd4,0x32103069,
                         0x24825602,0xbd12a984,0xe0092d3e,0x448eda5f, 0x24825602,0xbd12a984,0xe0092d3e,0x448eda5f,
                         0xc3b3c41f,0x113a31b7,0x3d9a5cd4,0x32103069, 0xc3b3c41f,0x113a31b7,0x3d9a5cd4,0x32103069,
                         0x24825602,0xbd12a984,0xe0092d3e,0x448eda5f, 0x24825602,0xbd12a984,0xe0092d3e,0x448eda5f,
                         0xc3b3c41f,0x113a31b7,0x3d9a5cd4,0x32103069, 0xc3b3c41f,0x113a31b7,0x3d9a5cd4,0x32103069,
                         0x24825602,0xbd12a984,0xe0092d3e,0x448eda5f, 0x24825602,0xbd12a984,0xe0092d3e,0x448eda5f,
                         0xc3b3c41f,0x113a31b7,0x3d9a5cd4,0x32103069, 0xc3b3c41f,0x113a31b7,0x3d9a5cd4,0x32103069,
                         0x24825602,0xbd12a984,0xe0092d3e,0x448eda5f, 0x24825602,0xbd12a984,0xe0092d3e,0x448eda5f,
                         0xc3b3c41f,0x113a31b7,0x3d9a5cd4,0x32103069, 0xc3b3c41f,0x113a31b7,0x3d9a5cd4,0x32103069,
                         0x24825602,0xbd12a984,0xe0092d3e,0x448eda5f, 0x24825602,0xbd12a984,0xe0092d3e,0x448eda5f,
                         0xc3b3c41f,0x113a31b7,0x3d9a5cd4,0x32103069, 0xc3b3c41f,0x113a31b7,0x3d9a5cd4,0x32103069,
                         0x24825602,0xbd12a984,0xe0092d3e,0x448eda5f, 0x24825602,0xbd12a984,0xe0092d3e,0x448eda5f,
                         };

uint32_t Cyphertext_GCM[AES_PAYLOAD_SIZE_GCM] = {
                         0x93FE7D9E, 0x9BFD1034, 0x8A5606E5, 0xCAFA7354, 0x85024120, 0x9AA1BC64, 0xC39CD896, 0xEB18A82A,
                         0xC33F5D30, 0xBF02902A, 0xDBC21463, 0x32C90296, 0x882324E5, 0xE98A66DB, 0x715FBD6F, 0x3DB42967,
                         0x8D014905, 0x3606986C, 0xB25714C0, 0xA5AAE722, 0xBFD70067, 0xE6B5B095, 0x4C3CCC20, 0x0257E816,
                         0xB25B3A23, 0x7D55544C, 0xA5F150E1, 0xC8A5C0BA, 0x57AB2924, 0xE8A6B8CE, 0xE8241D95, 0x56D64CA7,
                         0x392AB175, 0x1DAE6DC6, 0x65A5C01E, 0xA49ECDF9, 0x4B45FE3C, 0xCACAFBED, 0x77147596, 0xE9B8B71D,
                         0xF02CCC6F, 0x487CA95E, 0xEA0A3A9D, 0x7E2B0B4C, 0xBA03A4CA, 0x4A15536C, 0x63132D68, 0x967C5A35,
                         0x27428AC5, 0x475CDA18, 0x9096453C, 0x02A3BB30, 0x5F0CD427, 0xB6951B64, 0x6F34ECBA, 0x08110FA0,
                         0x3759CFE7, 0x084B8290, 0xA8FA98AD, 0x7C26C8C7, 0x4801DE30, 0xC754EF2A, 0x0D883184, 0xEF3DF4FF,
                         0x90E4483B, 0x9141ACC3, 0x846F0ED5, 0x7D5C6148, 0xDFF8AEFC, 0x11047D7C, 0x7D62A8E5, 0xB8B08F99,
                         0x68802B45, 0x902541EF, 0xC0E5C2CF, 0xF4215D6E, 0x22F44D5B, 0x27805310, 0x9A7FD851, 0x52504CE9,
                         0xCD539D16, 0x0F35C690, 0x646FB8D5, 0x65985C5B, 0xB0DB7F7C, 0xF101660A, 0xF8DAE389, 0xB8C60C7A,
                         0x640B2285, 0x3FC9BA14, 0xEF4BBCB2, 0x7151C575, 0x95B4254B, 0x9B529CDA, 0x80A1751D, 0xA29A470E,
                         0xA2CFAB9D, 0x9975A70D, 0xDFE048B1, 0x14CBC5AB, 0x49B4785D, 0x3C093E23, 0xC7474633, 0x66CA3874,
                         0xFB34555E, 0xEC6C667C, 0x6F28FC93, 0x6DE2F3B1, 0x7E12A40D, 0x68F780AA, 0x78271B30, 0xD3CCA9C3,
                         0x02E7C630, 0xCF8BB78C, 0x54EDBE11, 0x9A61F878, 0xADD7F798, 0xB32DCD33, 0x63BA571D, 0x171CFB16,
                         0xC67C4E00, 0xE458F1C5, 0x014EE1E1, 0x44DFB727, 0x8103DD79, 0x9920D4BD, 0x7AE461FD, 0x3E03BEB6};

uint32_t ExpectedTAG_GCM[4]={0x7717B56A,0x6071EFFA,0xA6A974A4,0xB2821BB7};

/* High priority message CCM enciphered  ===================================================================*/

uint32_t BlockB0[IV_SIZE] = {0x7A05C8CC, 0x7732B3B4, 0x7F08AF1D, 0xAF000011};
uint32_t BlockB1[AES_HEADER_SIZE_CCM] = {0x00073421, 0x5F032567, 0x0B000000};

/* Actual payload is 17-byte long */
uint32_t Plaintext_CCM[AES_PAYLOAD_SIZE_CCM] = {
                             0xBBD88334, 0x000075F6, 0xF4E89F9D, 0xDA50F5EA,
                             0xB1000000, 0x00000000, 0x00000000, 0x00000000};

uint32_t Cyphertext_CCM[AES_PAYLOAD_SIZE_CCM] = {
                             0xA7B7653C, 0x5D600AF3, 0x9CA0DB48, 0x0F4F5CCE,
                             0x99000000, 0x00000000, 0x00000000, 0x00000000};

uint32_t ExpectedTAG_CCM[TAG_SIZE] = {
                             0x352C36D3, 0x935B8894, 0x0426A004, 0x3BBAB7EE};

/*============================================================================*/

/* Used to store the low-priority GCM-encrypted text */
uint32_t Encryptedtext_LowPrio[AES_PAYLOAD_SIZE_GCM]={0};

/* Used to store the high-priority CCM-encrypted text */
uint32_t Encryptedtext_CCM[AES_PAYLOAD_SIZE_CCM] ={0};

/* Used to store the computed tag */
uint32_t TAG[TAG_SIZE]={0};

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_AES_Init(void);
/* USER CODE BEGIN PFP */
static void data_cmp(uint32_t *EncryptedText, uint32_t *RefText, uint32_t Size);
static void HighPriorityMessage_Processing(void);
/* Private functions ---------------------------------------------------------*/
#if defined(__GNUC__)
extern void initialise_monitor_handles(void);
#endif
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
#if defined(__GNUC__)
  initialise_monitor_handles();
#endif

  /* STM32G0xx HAL library initialization:
       - Configure the Flash prefetch
       - Systick timer is configured by default as source of time base, but user
         can eventually implement his proper time base source (a general purpose
         timer for example or other time source), keeping in mind that Time base
         duration should be kept 1ms since PPP_TIMEOUT_VALUEs are defined and
         handled in milliseconds basis.
       - Low Level Initialization
     */
  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */
  /* Configure LEDs */
  BSP_LED_Init(LED_GREEN);
  BSP_LED_Init(LED_RED);
  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_AES_Init();
  /* USER CODE BEGIN 2 */
  /* Initialize interruption counter and suspension request */
  IT_call_nb = 0;
  suspension_requested = 0;

  /*==========================================================================*/
  /* Start GCM enciphering of low priority message                            */
  /*==========================================================================*/
  if (HAL_CRYP_Encrypt_IT(&hcryp, Plaintext_GCM, AES_PAYLOAD_SIZE_GCM, Encryptedtext_LowPrio) != HAL_OK)
  {
    Error_Handler();
  }

  /*===================================================*/
  /* Wait for the CRYP handle to end up in READY state */
  /* (indicates low priority data processing end)      */
  /*===================================================*/
  while (HAL_CRYP_GetState(&hcryp) != HAL_CRYP_STATE_READY)
  {
    /*==========================================================*/
    /* If low priority data processing suspension is requested  */
    /*==========================================================*/
    if (suspension_requested == 1)
    {
      /* Carry out low prio data processing suspension and context saving */
      suspension_requested = 0;
      if (HAL_OK != HAL_CRYP_Suspend(&hcryp))
      {
        Error_Handler();
      }

      /*================================================*/
      /* Process high priority data (in interrupt mode) */
      /*================================================*/
      HighPriorityMessage_Processing();

      /* When done, resume low prio data processing
         - Context restore
         - processing resume */
      if (HAL_OK != HAL_CRYP_Resume(&hcryp))
      {
        Error_Handler();
      }
    /*============================================================*/
    /* End of low priority data processing suspension/resumption  */
    /*============================================================*/
    }
  }

  /* Compare the low priority message decrypted text with the expected one *****/
  data_cmp(Encryptedtext_LowPrio, Cyphertext_GCM, AES_PAYLOAD_SIZE_GCM);

  /* Compute the authentication TAG */
  if (HAL_CRYPEx_AESGCM_GenerateAuthTAG(&hcryp, TAG, TIMEOUT_VALUE) != HAL_OK)
  {
    /* Processing Error */
    Error_Handler();
  }
  /* Compare the derived tag with the expected one *************************/
  data_cmp(TAG, ExpectedTAG_GCM, TAG_SIZE);

  /* No issue detected if code reaches this point */
  BSP_LED_On(LED_GREEN);
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
  * @brief AES Initialization Function
  * @param None
  * @retval None
  */
static void MX_AES_Init(void)
{

  /* USER CODE BEGIN AES_Init 0 */

  /* USER CODE END AES_Init 0 */

  /* USER CODE BEGIN AES_Init 1 */

  /* USER CODE END AES_Init 1 */
  hcryp.Instance = AES;
  hcryp.Init.DataType = CRYP_DATATYPE_32B;
  hcryp.Init.KeySize = CRYP_KEYSIZE_128B;
  hcryp.Init.pKey = (uint32_t *)pKeyAES;
  hcryp.Init.pInitVect = (uint32_t *)pInitVectAES;
  hcryp.Init.Algorithm = CRYP_AES_GCM_GMAC;
  hcryp.Init.Header = (uint32_t *)HeaderAES;
  hcryp.Init.HeaderSize = 4;
  hcryp.Init.DataWidthUnit = CRYP_DATAWIDTHUNIT_WORD;
  hcryp.Init.HeaderWidthUnit = CRYP_HEADERWIDTHUNIT_WORD;
  hcryp.Init.KeyIVConfigSkip = CRYP_KEYIVCONFIG_ALWAYS;
  if (HAL_CRYP_Init(&hcryp) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN AES_Init 2 */

  /* USER CODE END AES_Init 2 */

}

/* USER CODE BEGIN 4 */
/**
  * @brief  buffer data comparison
  * @param
  * @retval None
  */
static void data_cmp(uint32_t *EncryptedText, uint32_t *RefText, uint32_t Size)
{
  /*  Before starting a new process, you need to check the current state of the peripheral;
      if it’s busy you need to wait for the end of current transfer before starting a new one.
      For simplicity reasons, this example is just waiting till the end of the
      process, but application may perform other tasks while transfer operation
      is ongoing. */
  while (HAL_CRYP_GetState(&hcryp) != HAL_CRYP_STATE_READY)
  {
  }

  /*##-3- Check the encrypted text with the expected one #####################*/
  if(memcmp(EncryptedText, RefText, 4*Size) != 0)
  {
    Error_Handler();
  }
  else
  {
    /* Right encryption */
  }
}

/**
  * @brief  High priority message processing: CCM-enciphering with a 256-bit long key
  * @param
  * @retval None
  */
static void HighPriorityMessage_Processing(void)
{
  hcryp.Init.KeySize       = CRYP_KEYSIZE_256B;
  hcryp.Init.pKey          = (uint32_t *)AES256Key_CCM;
  hcryp.Init.Algorithm     = CRYP_AES_CCM;
  hcryp.Init.B0            = (uint32_t *)BlockB0;
  hcryp.Init.Header        = (uint32_t *)BlockB1;
  hcryp.Init.HeaderSize    = AES_HEADER_SIZE_CCM;
  hcryp.Init.DataWidthUnit = CRYP_DATAWIDTHUNIT_BYTE; /* for payload padding on a byte-basis */

  /* Set the CRYP parameters */
  if (HAL_CRYP_Init(&hcryp) != HAL_OK)
  {
    /* Initialization Error */
    Error_Handler();
  }

  if (HAL_CRYP_Encrypt_IT(&hcryp, Plaintext_CCM, AES_ACTUAL_PAYLOAD_SIZE_CCM, Encryptedtext_CCM) != HAL_OK)
  {
    /* Processing Error */
    Error_Handler();
  }
  while (HAL_CRYP_GetState(&hcryp) != HAL_CRYP_STATE_READY);

  /* Compare the encrypted text with the expected one *************************/
  Encryptedtext_CCM[(AES_ACTUAL_PAYLOAD_SIZE_CCM/4)] = Encryptedtext_CCM[(AES_ACTUAL_PAYLOAD_SIZE_CCM/4)]&0xFF000000; /* to compare only first byte of last word */
  data_cmp(Encryptedtext_CCM, Cyphertext_CCM, ((AES_ACTUAL_PAYLOAD_SIZE_CCM/4)+1));

  /* Compute the authentication TAG */
  if (HAL_CRYPEx_AESGCM_GenerateAuthTAG(&hcryp, TAG, TIMEOUT_VALUE) != HAL_OK)
  {
    /* Processing Error */
    Error_Handler();
  }

  /* Compare the derived tag with the expected one *************************/
  data_cmp(TAG, ExpectedTAG_CCM, TAG_SIZE);

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
  printf("\n\r Error Detected...\n ");

  /* Turn LED_RED on */
  BSP_LED_On(LED_RED);

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
