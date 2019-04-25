/**
  ******************************************************************************
  * @file    main.c
  * @author  MCD Application Team
  * @brief   This file provides main program functions
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
#include "stm32g0316_discovery.h"

/* Private typedef -----------------------------------------------------------*/
typedef enum
{
  DEMO_MODE_NRST = 0,
  DEMO_MODE_GPIO
}DemoMode_t;

typedef enum
{
  DEMO_SUBMODE_NRST_BLINK_WAIT = 0,
  DEMO_SUBMODE_NRST_BLINK_OBL,
  DEMO_SUBMODE_NRST_BLINK_IWDG,
  DEMO_SUBMODE_GPIO_BLINK_WAIT,
  DEMO_SUBMODE_GPIO_BLINK_OBL,
  DEMO_SUBMODE_GPIO_BLINK_IWDG,
  DEMO_SUBMODE_GPIO_BLINK_WAKEUP,
  DEMO_SUBMODE_NB
}DemoSubMode_t;

typedef enum
{
  KEYPRESS_NONE,
  KEYPRESS_SHORT,
  KEYPRESS_LONG,
  KEYPRESS_DOUBLE,
}KeyPress_t;

typedef struct
{
  uint32_t OnPeriod;
  uint32_t OffPeriod;
  uint32_t BlinkNb;
  uint32_t Cycle;
} LedBlinkScheme_t;

/* Private define ------------------------------------------------------------*/
#define BUTTON_MULTIBONDING_NB              4
#define KEYPRESS_OVERALL_SAMPLING           800u
#define KEYPRESS_DEBOUNCE_DELAY             40u
#define LED_OFF                             0u
#define LED_ON                              1u

#define NRST_BLINKING_PERIOD 2000
#define GPIO_BLINKING_PERIOD 4000
#define LED_BOOTCHECK_TEMPO  5000

/* Private macro -------------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
static void           SystemClockConfig(DemoMode_t DemoMode);
static void           RtcConfig(void);
static void           BackupDomainAccessEnable(void);
static void           CheckBootReason(void);
static void           ExecuteBootReason(DemoMode_t DemoMode);

static void           MainNrstDemoMode(void);
static void           MainGpioDemoMode(void);
static void           GpioDemoModeShortPress(void);
static void           GpioDemoModeLongPress(void);
static void           GpioDemoModeDoublePress(void);

static void           LedBlinkSchemeOn(DemoSubMode_t DemoSubMode);
static void           LedBlinkSchemeOff(void);

static void           PushButtonConfig(void);
static KeyPress_t     PushButtonInputScan(void);

static DemoMode_t     GetDemoMode(void);
static void           SetDemoMode(DemoMode_t DemoMode);

static void           ErrorHandler(void);

/* Private variables ---------------------------------------------------------*/

__IO uint32_t toto =0;
static __IO uint32_t IoFalling;
static uint32_t RccBootFlags = 0;
static uint32_t PwrSBFlag = 0;
static uint32_t LedOn = 0;
static uint32_t LedOnPeriod = 0;
static uint32_t LedOffPeriod = 0;
static uint32_t LedCycle = 0;
static uint32_t LedBlinkNb = 0;
static uint32_t LedBlinkCount = 0;
static uint16_t ButtonPinBonding[BUTTON_MULTIBONDING_NB] = {GPIO_PIN_2, GPIO_PIN_0, GPIO_PIN_1, GPIO_PIN_2};
static GPIO_TypeDef *ButtonPort;

static LedBlinkScheme_t LedBlinkScheme[DEMO_SUBMODE_NB] = 
{
  {NRST_BLINKING_PERIOD/2,  NRST_BLINKING_PERIOD/2 , 1, NRST_BLINKING_PERIOD}, /* DEMO_SUBMODE_NRST_BLINK_WAIT   */
  {NRST_BLINKING_PERIOD/10, NRST_BLINKING_PERIOD/10, 2, NRST_BLINKING_PERIOD}, /* DEMO_SUBMODE_NRST_BLINK_OBL    */
  {NRST_BLINKING_PERIOD/10, NRST_BLINKING_PERIOD/10, 3, NRST_BLINKING_PERIOD}, /* DEMO_SUBMODE_NRST_BLINK_IWDG   */
  {GPIO_BLINKING_PERIOD/2,  GPIO_BLINKING_PERIOD/2,  1, GPIO_BLINKING_PERIOD}, /* DEMO_SUBMODE_GPIO_BLINK_WAIT   */
  {NRST_BLINKING_PERIOD/10, NRST_BLINKING_PERIOD/10, 2, GPIO_BLINKING_PERIOD}, /* DEMO_SUBMODE_GPIO_BLINK_OBL    */
  {NRST_BLINKING_PERIOD/10, NRST_BLINKING_PERIOD/10, 3, GPIO_BLINKING_PERIOD}, /* DEMO_SUBMODE_GPIO_BLINK_IWDG   */
  {NRST_BLINKING_PERIOD/10, NRST_BLINKING_PERIOD/10, 4, GPIO_BLINKING_PERIOD}, /* DEMO_SUBMODE_GPIO_BLINK_WAKEUP */
};

/* Exported variables --------------------------------------------------------*/
uint16_t ButtonPin;
RTC_HandleTypeDef RtcHandle = {0};

/* Exported functions --------------------------------------------------------*/
/**
* @brief  Main program
* @param  None
* @retval int
*/
int main(void)
{
  DemoMode_t demomode;

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* Enable Backup Domain access */
  BackupDomainAccessEnable();

  /* Check Boot reason */
  CheckBootReason();

  /* LED init */
  BSP_LED_Init(LED2);

  /* Get Demo mode */
  demomode = GetDemoMode();

  /* Configure the system clock */
  SystemClockConfig(demomode);

  /* RTC config */
  RtcConfig();
  
  /* Different execution depending on boot reason */
  ExecuteBootReason(demomode);

  /* Switch to correct sub-demo mode */
  switch(demomode)
  {
    case DEMO_MODE_NRST:
      MainNrstDemoMode();
      break;

    case DEMO_MODE_GPIO:
      MainGpioDemoMode();
      break;

    default:
      ErrorHandler();
      break;
  }

  /* Infinite loop */
  while(1);
}


/** @brief  Main function of PF2-NRST DemoMode
  * @param  None
  * @retval None
  */
static void MainNrstDemoMode(void)
{
  IWDG_HandleTypeDef hiwdg;

  /* Check what was boot reason : if coming from PWR, IWDG or OBL reset keep
     normal sub-demo mode */
  if(RccBootFlags == RCC_CSR_PINRSTF)
  {
    /* Switch to GPIO sub-demo mode */
    SetDemoMode(DEMO_MODE_GPIO);
  }
  else
  {
    LedBlinkSchemeOn(DEMO_SUBMODE_NRST_BLINK_WAIT);
    hiwdg.Instance = IWDG;
    hiwdg.Init.Prescaler = IWDG_PRESCALER_256;
    hiwdg.Init.Reload = 0x0FFF/4;
    hiwdg.Init.Window = IWDG_WINDOW_DISABLE;
    HAL_IWDG_Init(&hiwdg);
  }
}


/** @brief  Main function of PF2-GPIO DemoMode
  * @param  None
  * @retval None
  */
static void MainGpioDemoMode(void)
{
  KeyPress_t keypress = KEYPRESS_NONE;

  PushButtonConfig();

  LedBlinkSchemeOn(DEMO_SUBMODE_GPIO_BLINK_WAIT);

  while(keypress == KEYPRESS_NONE)
  {
    /* Scan nb of key press */
    keypress = PushButtonInputScan();
  }

  switch(keypress)
  {
    case KEYPRESS_SHORT:
      GpioDemoModeShortPress();
      break;

    case KEYPRESS_LONG:
      GpioDemoModeLongPress();
      break;

    case KEYPRESS_DOUBLE:
      GpioDemoModeDoublePress();
      break;

    case KEYPRESS_NONE:
    default:
      ErrorHandler();
      break;
  }

}


/** @brief  Gpio Sub Mode short press handler.
  *         We will enter in standby mode with wakeup pin 1
  * @param  None
  * @retval None
  */
static void GpioDemoModeShortPress(void)
{
  /* Disable User Button */
  HAL_GPIO_DeInit(ButtonPort, ButtonPin);

  /* Desactivate Blinking scheme */
  LedBlinkSchemeOff();

  HAL_Delay(100);

  /* Enable gpio pin 0 / wake up pin 1 with pull UP in standby mode */
  HAL_PWREx_EnableGPIOPullUp(PWR_GPIO_A, PWR_GPIO_BIT_0);
  HAL_PWREx_EnablePullUpPullDownConfig();

  /* Configure Wake up pin 1 with falling edge detection */
  HAL_PWR_EnableWakeUpPin(PWR_WAKEUP_PIN1_LOW);
  /* Clear Wakeup Pin 1 flag before entering standby */
  __HAL_PWR_CLEAR_FLAG(PWR_FLAG_WUF1);

  /* Enable Ultra Low Power mode */
  HAL_PWREx_EnablePORMonitorSampling();

  /* Enter standby mode */
  HAL_SuspendTick();
  HAL_PWR_EnterSTANDBYMode();

  /* We should not reach here */
  while(1);
}


/** @brief  Gpio Sub Mode long press handler.
  *         We will switch to NRST Sub Mode
  * @param  None
  * @retval None
  */
static void GpioDemoModeLongPress(void)
{
  /* Switch to NRST sub-demo mode */
  SetDemoMode(DEMO_MODE_NRST);
}


/** @brief  Gpio Sub Mode double press handler.
  *         We will perform immediat system reset through IWDG
  * @param  None
  * @retval None
  */
static void GpioDemoModeDoublePress(void)
{
  IWDG_HandleTypeDef hiwdg;

  hiwdg.Instance = IWDG;
  hiwdg.Init.Prescaler = IWDG_PRESCALER_4;
  hiwdg.Init.Reload = 0x0001;
  hiwdg.Init.Window = IWDG_WINDOW_DISABLE;
  HAL_IWDG_Init(&hiwdg);
}

/* Private functions ---------------------------------------------------------*/
/**
  * @brief  System Clock Configuration
  *         The system Clock is configured as follow:
  *         in case og NRST Mode:
  *           System Clock source            = HSI
  *           SYSCLK(Hz)                     = 16000000
  *           HCLK(Hz)                       = 16000000
  *           AHB Prescaler                  = 1
  *           APB1 Prescaler                 = 1
  *           Flash Latency(WS)              = 1
  *         in case GPIO Mode:
  *            System Clock source            = PLL (HSI)
  *            SYSCLK(Hz)                     = 64000000
  *            HCLK(Hz)                       = 64000000
  *            AHB Prescaler                  = 1
  *            APB1 Prescaler                 = 1
  *            HSI Frequency(Hz)              = 16000000
  *            PLLM                           = 4
  *            PLLN                           = 64
  *            PLLP                           = 16
  *            PLLQ                           = 5
  *            PLLR                           = 2
  *            Flash Latency(WS)              = 2
  * @param  None
  * @retval None
  */
static void SystemClockConfig(DemoMode_t DemoMode)
{
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  uint32_t flashlatency;

  /* Set common parameters depending on subdemo mode */
  RCC_OscInitStruct.OscillatorType      = (RCC_OSCILLATORTYPE_HSI | RCC_OSCILLATORTYPE_LSI);
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.HSIState            = RCC_HSI_ON;
  RCC_OscInitStruct.HSIDiv              = RCC_HSI_DIV1;
  RCC_OscInitStruct.LSIState            = RCC_LSI_ON;

  RCC_ClkInitStruct.ClockType      = (RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_PCLK1);
  RCC_ClkInitStruct.AHBCLKDivider  = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;

  if(DemoMode == DEMO_MODE_GPIO)
  {
    /* Set HSI as PLL source to use it for max 64MHz frequency */
    RCC_OscInitStruct.PLL.PLLState        = RCC_PLL_ON;
    RCC_OscInitStruct.PLL.PLLSource       = RCC_PLLSOURCE_HSI;
    RCC_OscInitStruct.PLL.PLLM            = RCC_PLLM_DIV4;
    RCC_OscInitStruct.PLL.PLLN            = 64;
    RCC_OscInitStruct.PLL.PLLP            = RCC_PLLP_DIV16;
    RCC_OscInitStruct.PLL.PLLQ            = RCC_PLLQ_DIV2;
    RCC_OscInitStruct.PLL.PLLR            = RCC_PLLR_DIV4;

    /* Set PLL as SYSCLK source and Flash latency */
    RCC_ClkInitStruct.SYSCLKSource   = RCC_SYSCLKSOURCE_PLLCLK;
    flashlatency = FLASH_LATENCY_2;

    /* Increase volatage scaling : set range 1 (default value) */
    if(HAL_PWREx_ControlVoltageScaling(PWR_REGULATOR_VOLTAGE_SCALE1) != HAL_OK)
    {
      ErrorHandler();
    }
  }
  else
  {
    /* Set HSI as SYSCLK source and Flash latency */
    RCC_ClkInitStruct.SYSCLKSource   = RCC_SYSCLKSOURCE_HSI;
    flashlatency = FLASH_LATENCY_0;
  }

  /* Configure oscillator */
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct)!= HAL_OK)
  {
    ErrorHandler();
  }

  /* Configure clocks */
  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, flashlatency)!= HAL_OK)
  {
    ErrorHandler();
  }

  if(DemoMode == DEMO_MODE_NRST)
  {
    /* Decrease volatage scaling : set range 2 */
    if(HAL_PWREx_ControlVoltageScaling(PWR_REGULATOR_VOLTAGE_SCALE2) != HAL_OK)
    {
      ErrorHandler();
    }
  }
}


/**
  * @brief  Configure RTC
  * @param  None
  * @retval None
  */
static void RtcConfig(void)
{
  RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};

  /* Set Rtc instance to Handle */
  RtcHandle.Instance = RTC;
  if((RccBootFlags & (RCC_CSR_PWRRSTF | RCC_CSR_SFTRSTF)) != 0x00u)
  {
    /* enable RTC clock */
    PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_RTC;
    PeriphClkInit.RTCClockSelection = RCC_RTCCLKSOURCE_LSI;
    if(HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
    {
      ErrorHandler();
    }

    /* Enable RTC peripheral Clocks */
    __HAL_RCC_RTC_ENABLE();
  }
  else
  {
    /* There potentially wakeup timer auto-reloaded : desactivate it */
    HAL_RTCEx_DeactivateWakeUpTimer(&RtcHandle);
    __HAL_RTC_CLEAR_FLAG(&RtcHandle, RTC_CLEAR_WUTF);
  }

  /* Configure NVIC */
  HAL_NVIC_SetPriority(RTC_TAMP_IRQn, 0x03u, 0x00u);
  HAL_NVIC_EnableIRQ(RTC_TAMP_IRQn);
}


/**
  * @brief  Configure blinking led scheme
  * @param  None
  * @retval None
  */
static void LedBlinkSchemeOn(DemoSubMode_t DemoSubMode)
{
  uint32_t counter;

  /* Save new led configuration */
  LedOnPeriod = LedBlinkScheme[DemoSubMode].OnPeriod;
  LedOffPeriod = LedBlinkScheme[DemoSubMode].OffPeriod;
  LedCycle = LedBlinkScheme[DemoSubMode].Cycle;
  LedBlinkNb = LedBlinkScheme[DemoSubMode].BlinkNb;
  LedBlinkCount = LedBlinkNb;
  LedOn = LED_ON;

  counter = (LedOnPeriod * (LSI_VALUE >> 2)) / 1000;

  BSP_LED_On(LED2);
  HAL_RTCEx_SetWakeUpTimer_IT(&RtcHandle, counter, RTC_WAKEUPCLOCK_RTCCLK_DIV2);
}

/**
  * @brief  Turn off blinking scheme
  * @param  None
  * @retval None
  */
static void LedBlinkSchemeOff(void)
{
  /* There potentially wakeup timer auto-reloaded : desactivate it */
  HAL_RTCEx_DeactivateWakeUpTimer(&RtcHandle);
  __HAL_RTC_CLEAR_FLAG(&RtcHandle, RTC_CLEAR_WUTF);

  /* turn Led Off */
  BSP_LED_Off(LED2);
  LedOn = LED_OFF;
}

/**
  * @brief  Configure push button
  * @param  None
  * @retval None
  */
static void PushButtonConfig(void)
{
  uint32_t index;
  GPIO_InitTypeDef gpio;
  IRQn_Type irq;

  index = TAMP->BKP0R;

  if(index != 0)
  {
    __HAL_RCC_GPIOA_CLK_ENABLE();
    ButtonPort = GPIOA;
  }
  else
  {
    __HAL_RCC_GPIOF_CLK_ENABLE();
    ButtonPort = GPIOF;
  }

  ButtonPin = ButtonPinBonding[index];

  /* increase index */
  index++;
  if(index >= BUTTON_MULTIBONDING_NB)
  {
    index = 0;
  }
  TAMP->BKP0R = index;

  /* PF02 is used as user button: add pull-up as no external one and use
  falling trigger */
  gpio.Pin = ButtonPin;
  gpio.Mode = GPIO_MODE_IT_FALLING;
  gpio.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(ButtonPort, &gpio);

  if(ButtonPin != GPIO_PIN_2)
  {
    irq = EXTI0_1_IRQn;
  }
  else
  {
    irq = EXTI2_3_IRQn;
  }

  HAL_NVIC_SetPriority(irq, 0x00, 0x00);
  HAL_NVIC_EnableIRQ(irq);
}


/**
  * @brief  Scan push button input signal in order to determine short, long or
  *         double key press
  * @param  None
  * @retval Return short, long or sdoucle key pressed.
  */
static KeyPress_t PushButtonInputScan(void)
{
  KeyPress_t code = KEYPRESS_NONE;
  uint32_t press;
  uint32_t release;
  uint32_t tickstart;

  /* If push button interruption has been detected */
  if(IoFalling != 0x00u)
  {
    tickstart = HAL_GetTick();
    press = 0;
    release = 0;

    /* for an overall sampling time */
    while(HAL_GetTick() - tickstart < KEYPRESS_OVERALL_SAMPLING)
    {
      HAL_Delay(KEYPRESS_DEBOUNCE_DELAY);
      if(GPIO_PIN_RESET == HAL_GPIO_ReadPin(ButtonPort, ButtonPin))
      {
        press++;
      }
      else
      {
        release++;
      }
    }

    if(IoFalling == 2)
    {
      code = KEYPRESS_DOUBLE;
    }
    else if (press > 5)
    {
      code = KEYPRESS_LONG;
    }
    else
    {
      code = KEYPRESS_SHORT;
    }
    IoFalling = 0x00u;
  }
  return code;
}


/**
  * @brief  Get current demo sub-mode
  * @param  None
  * @retval Demo sub-mode
  */
static DemoMode_t GetDemoMode(void)
{
  /* return sub-mode */
  if((FLASH->OPTR & FLASH_OPTR_NRST_MODE) == OB_RESET_MODE_GPIO)
  {
    return DEMO_MODE_GPIO;
  }
  else
  {
    return DEMO_MODE_NRST;
  }
}


/**
  * @brief  Set demo sub-mode
  * @param  SubMode select one of 2 different demo modes.
  *         This parameter can be one of the following values:
  *           @arg @ref SUBDEMO_MODE_GPIO
  *           @arg @ref SUBDEMO_MODE_NRST
  *         Target will reboot with option byte reset reason
  * @retval None
  */
static void SetDemoMode(DemoMode_t DemoMode)
{
  uint32_t nrstmode;

  /* Enable Flash access anyway */
  __HAL_RCC_FLASH_CLK_ENABLE();

  /* Unlock flash */
  FLASH->KEYR = FLASH_KEY1;
  FLASH->KEYR = FLASH_KEY2;
  while((FLASH->CR & FLASH_CR_LOCK) != 0x00);

  /* unlock option byte registers */
  FLASH->OPTKEYR = 0x08192A3B;
  FLASH->OPTKEYR = 0x4C5D6E7F;
  while((FLASH->CR & FLASH_CR_OPTLOCK) == FLASH_CR_OPTLOCK);

  /* get current user option bytes */
  nrstmode = (FLASH->OPTR & ~FLASH_OPTR_NRST_MODE);
  /* Select sub demo mode */
  if(DemoMode == DEMO_MODE_GPIO)
  {
    nrstmode |= OB_RESET_MODE_GPIO;
  }
  else
  {
    nrstmode |= OB_RESET_MODE_INPUT_OUTPUT;
  }

  /* Program option bytes */
  FLASH->OPTR = nrstmode;

  /* Write operation */
  FLASH->CR |= FLASH_CR_OPTSTRT;
  while((FLASH->SR & FLASH_SR_BSY1) != 0);

  /* Force OB Load */
  FLASH->CR |= FLASH_CR_OBL_LAUNCH;

  while(1);
}


/**
  * @brief  Enable Backup domain access
  * @param  None
  * @retval None
  */
static void BackupDomainAccessEnable(void)
{
  /* enable backup register access */
  __HAL_RCC_PWR_CLK_ENABLE();
  HAL_PWR_EnableBkUpAccess();

  /* RTCAPB clock enable */
  __HAL_RCC_RTCAPB_CLK_ENABLE();
}


/**
  * @brief  Check boot reason and save them
  * @param  None
  * @retval None
  */
static void CheckBootReason(void)
{
  /* Get reset flags */
  RccBootFlags = (RCC->CSR & (RCC_CSR_OBLRSTF | RCC_CSR_PINRSTF | RCC_CSR_PWRRSTF
  | RCC_CSR_SFTRSTF |  RCC_CSR_IWDGRSTF |  RCC_CSR_WWDGRSTF | RCC_CSR_LPWRRSTF));

  /* Clear reset flags */
  __HAL_RCC_CLEAR_RESET_FLAGS();

  /* Get Standby flag */
  PwrSBFlag = PWR->SR1;

  /* Clear Standby flag */
  __HAL_PWR_CLEAR_FLAG(PWR_FLAG_SB | PWR_FLAG_WUF1);
}


/**
  * @brief  Show to end user boot reason
  * @param  None
  * @retval None
  */
static void ExecuteBootReason(DemoMode_t DemoMode)
{
  DemoSubMode_t demosubmode = DEMO_SUBMODE_NRST_BLINK_WAIT;

  /* Check boot reason and warn user in case of Option Byte Reload reset, IWDG
  reset and stand by exit */
  if(((RccBootFlags & (RCC_CSR_PWRRSTF | RCC_CSR_SFTRSTF)) == 0x00u) && (RccBootFlags != RCC_CSR_PINRSTF))
  {
    if((RccBootFlags & RCC_CSR_OBLRSTF) != 0x00u)
    {
      if (DemoMode == DEMO_MODE_NRST)
      {
        demosubmode = DEMO_SUBMODE_NRST_BLINK_OBL;
      }
      else
      {
        demosubmode = DEMO_SUBMODE_GPIO_BLINK_OBL;
      }
    }
    else if((RccBootFlags & RCC_CSR_IWDGRSTF) != 0x00u)
    {
      if (DemoMode == DEMO_MODE_NRST)
      {
        demosubmode = DEMO_SUBMODE_NRST_BLINK_IWDG;
      }
      else
      {
        demosubmode = DEMO_SUBMODE_GPIO_BLINK_IWDG;
      }
    }
    else if((PwrSBFlag & PWR_SR1_SBF) != 0x00u)
    {
      /* Disable GPIO pull up in standby */
      HAL_PWREx_DisablePullUpPullDownConfig();

      demosubmode = DEMO_SUBMODE_GPIO_BLINK_WAKEUP;
    }

    /* Launch led blink scheme */
    LedBlinkSchemeOn(demosubmode);

    /* Let user see this boot reason */
    HAL_Delay(LED_BOOTCHECK_TEMPO);

    /* Desactivate Blinking scheme */
    LedBlinkSchemeOff();
  }
}


/**
  * @brief  EXTI line detection callback.
  * @param  GPIO_Pin Specifies the port pin connected to corresponding EXTI line.
  * @retval None
  */
void HAL_GPIO_EXTI_Falling_Callback(uint16_t GPIO_Pin)
{
  if(GPIO_Pin == ButtonPin)
  {
    IoFalling++;
  }
}


/**
  * @brief  Wake Up Timer callback.
  * @param  hrtc RTC handle
  * @retval None
  */
void HAL_RTCEx_WakeUpTimerEventCallback(RTC_HandleTypeDef * hrtc)
{
  uint32_t counter;
  
  HAL_RTCEx_DeactivateWakeUpTimer(&RtcHandle);

  if(LedBlinkCount == 0x00u)
  {
    LedBlinkCount = LedBlinkNb;
    counter = ((LedCycle - ((LedOnPeriod + LedOffPeriod) * LedBlinkNb)) * (LSI_VALUE >> 2)) / 1000;
  }
  else
  {
    if(LedOn != LED_OFF)
    {
      LedBlinkCount--;
      BSP_LED_Off(LED2);
      counter = (LedOffPeriod * (LSI_VALUE >> 2)) / 1000;
      LedOn = LED_OFF;
    }
    else
    {
      BSP_LED_On(LED2);
      counter = (LedOnPeriod * (LSI_VALUE >> 2)) / 1000;
      LedOn = LED_ON;
    }
  }

  HAL_RTCEx_SetWakeUpTimer_IT(&RtcHandle, counter, RTC_WAKEUPCLOCK_RTCCLK_DIV2);
}



/**
  * @brief  Functionnal error report to user: LED stay on forever
  * @param  None
  * @retval None
  */
static void ErrorHandler(void)
{
  /* Desactivate Blinking scheme */
  LedBlinkSchemeOff();

  /* Turn LED On */
  BSP_LED_On(LED2);

  /* Infinite loop */
  while(1)
  {
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

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
