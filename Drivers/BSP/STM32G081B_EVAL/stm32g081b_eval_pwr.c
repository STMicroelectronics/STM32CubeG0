/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    stm32g081b_eval_pwr.c
  * @author  MCD Application Team
  * @brief   This file provides a set of functions needed to manage the Type-C
  *          Power Delivery (PD):
  *            - VBUS control
  *            - VBUS voltage/current measurement
  *            - VCONN control
  *            - VBUS presence detection
  *            - DCDC voltage measurement (MB1352C only)
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2018-2021 STMicroelectronics.
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
#if defined(_TRACE)||defined(_GUI_INTERFACE)
#define PWR_DEBUG
#endif /* _TRACE || _GUI_INTERFACE*/
#include "stm32g081b_eval.h"
#include "stm32g081b_eval_pwr.h"
#include "stm32g0xx_hal.h"
#include "stm32g0xx_ll_lptim.h"
#include "string.h"
#ifdef PWR_DEBUG
#include "usbpd_core.h"
#include "usbpd_trace.h"
#endif
/* USER CODE BEGIN include */
/* USER CODE END include */


/** @addtogroup BSP
  * @{
  */

/** @addtogroup STM32G081B_EVAL
  * @{
  */

/** @addtogroup STM32G081B_EVAL_POWER
  * @{
  */
/** @defgroup STM32G081B_EVAL_POWER_Private_Typedef Private Typedef
  * @{
  */
/* USER CODE BEGIN POWER_Private_Typedef */
/* Vref voltage level (in mV) */
#define VREFANALOG_VOLTAGE 3300

/**
  * @brief USB PD PPS values
  */
typedef enum
{
  VBUS_5_V         = 0u,
  VBUS_9_V         = 1u,
  VBUS_15_V        = 2u,
  VBUS_18_V        = 3u,
  VBUS_PPS_V       = 4u,
  VBUS_LEVEL_NB    = 5u
} VBUS_Level_TypeDef;

typedef enum
{
  ADC_VSENSE_1       = 0u,   /*< PB1  ADC used to get VBUS1 voltage level     */
  ADC_ISENSE_1       = 1u,   /*< PB10 ADC used to get VBUS1 curent value      */
  ADC_VSENSE_2       = 2u,   /*< PA3  ADC used to get VBUS2 voltage level     */
  ADC_ISENSE_2       = 3u,   /*< PB12 ADC used to get VBUS2 curent value      */
  ADC_VSENSE_DCDC    = 4u    /*< PB11 ADC used to get DCDC voltage level      */
} ADC_ChannelId_TypeDef;

typedef enum
{
  GPIO_SOURCE_EN        = 0u,   /*< PC6  Handle Source power Mos                                    */
  GPIO_VBUS_DISCHARGE1  = 1u,   /*< PB13 Handle Discharge Mos on VBUS1                              */
  GPIO_VBUS_DISCHARGE2  = 2u,   /*< PB14 Handle Discharge Mos on VBUS2                              */
  GPIO_VCONN_EN1        = 3u,   /*< PD4  Handle Vconn on VBUS1                                      */
  GPIO_VCONN_EN2        = 4u,   /*< PB9  Handle Vconn on VBUS2                                      */
  GPIO_VCONN_DISCHARGE1 = 5u,   /*< PA2  Handle Fast Role Swap on VBUS1 CC1 line or Vconn Discharge */
  GPIO_VCONN_DISCHARGE2 = 6u,   /*< PPB0 Handle Fast Role Swap on VBUS1 CC2 line or Vconn Discharge */
  GPIO_V_CTL1           = 7u,   /*< PC1   VBUS voltage level control                                */
  GPIO_V_CTL2           = 8u    /*< PB5 (Rev. A/B) or PA1 (Rev. C) VBUS voltage level control       */
} GPIO_Id_TypeDef;

typedef enum
{
  PORT_NOT_INITIALIZED = 0u,
    PORT_INITIALIZED
} PWR_PortStateTypedef;

typedef enum
{
  POWER_NONE,
  POWER_FIXED,
  POWER_VARIABLE,
  POWER_APDO
} PWR_Type;


typedef struct
{
    PWR_PortStateTypedef            State;
    USBPD_PWR_PowerRoleTypeDef      Role;
    uint32_t                        VBUSDisconnectionThreshold;
    USBPD_PWR_VBUSConnectionStatusTypeDef VBUSConnectionStatus;
    USBPD_PWR_VBUSDetectCallbackFunc *    pfnVBUSDetectCallback;
    PWR_Type                        Type;
    uint32_t                        VBUSVoltage;
} PortInfoTypeDef;


typedef struct
{
    uint8_t                         ADCRefCount;
    USBPD_PWR_DCDCCtrlModeTypeDef   DCDCCtrlMode;
    PortInfoTypeDef                 PortInfo[PWR_TYPEC_PORT_NB];
} ContextTypeDef;

/* USER CODE END POWER_Private_Typedef */
/**
  * @}
  */

/** @defgroup STM32G081B_EVAL_POWER_Private_Constants Private Constants
  * @{
  */
/* USER CODE BEGIN POWER_Private_Constants */

/* VDD voltage (in mv)*/
#define VDD 3300

/* Maximum digital value of the ADC output (12 Bits resolution)
   To converting ADC measurement to an absolute voltage value:
   VCHANNELx = ADCx_DATA x (VDD/ADC_FULL_SCALE)
*/
#define ADC_FULL_SCALE 0x0FFFu

/* VBUS connection detection voltage threshold (in mV) */
#define VBUS_CONNECTION_THRESHOLD 3000u

/* VBUS disconnection detection voltage threshold (in mV) */
#define VBUS_DISCONNECTION_THRESHOLD 2200u

/* Resistor voltage divider */
#define RA 330000u
#define RB  49900u

/* VBUS power source calibration parameters */
#define BSP_PWR_DCDC_POLL_TIME          1        /* DCDC polling periode in mS */
#define BSP_PWR_DCDC_MAX_PWM_VALUE      (3200u)   /* DCDC Vref: Max pwm min value 3.8V on RevB */
#define BSP_PWR_DCDC_MIN_PWM_VALUE      (1)      /* DCDC Vref: Max pwm max value 18V on RevB */
#define BSP_PWR_DCDC_MAX_PWM_STEP       (55)     /* DCDC Vref: Max pwm step = Full scale/25mS */
#define BSP_PWR_DCDC_MIN_PWM_STEP       (1)      /* DCDC Vref: Min pwm step */
#define BSP_PWR_DCDC_PLL_P_N            (9)      /* DCDC Coef P (0.xx)*/
#define BSP_PWR_DCDC_PLL_P_D            (9)      /* DCDC Coef P */
#define BSP_PWR_DCDC_PLL_D_N            (7)      /* DCDC Coef D (0.xx) */
#define BSP_PWR_DCDC_PLL_D_D            (8)      /* DCDC Coef D */

#define PWM_MAX_VALUES_LUT_PPS          131u

/* VBUS voltage level values (in mV) */
#define VBUS_VOLTAGE_0V_IN_MV   0u
#define VBUS_VOLTAGE_5V_IN_MV   5000u
#define VBUS_VOLTAGE_9V_IN_MV   9000u
#define VBUS_VOLTAGE_15V_IN_MV  15000u
#define VBUS_VOLTAGE_18V_IN_MV  18000u

/* VBUS sensing resume timout value (in ms)*/
#define VBUS_SENSING_RESUME_TIMEOUT     (1U)
/* USER CODE END POWER_Private_Constants */
/**
  * @}
  */

/** @defgroup STM32G081B_EVAL_POWER_Private_Macros Private Macros
  * @{
  */
/* USER CODE BEGIN POWER_Private_Macros */
#define GPIOx_CLK_ENABLE(__GPIO__)   do { if((__GPIO__) == GPIO_SOURCE_EN)   {GPIO_SOURCE_EN_CLK_ENABLE();} else \
                                          if((__GPIO__) == GPIO_VBUS_DISCHARGE1) {GPIO_VBUS_DISCHARGE1_CLK_ENABLE();} else \
                                          if((__GPIO__) == GPIO_VBUS_DISCHARGE2) {GPIO_VBUS_DISCHARGE2_CLK_ENABLE();} else \
                                          if((__GPIO__) == GPIO_VCONN_EN1)   {GPIO_VCONN_EN1_CLK_ENABLE();} else \
                                          if((__GPIO__) == GPIO_VCONN_EN2)   {GPIO_VCONN_EN2_CLK_ENABLE();} else \
                                          if((__GPIO__) == GPIO_VCONN_DISCHARGE1)     {GPIO_VCONN_DISCHARGE1_CLK_ENABLE();} else \
                                          if((__GPIO__) == GPIO_VCONN_DISCHARGE2)     {GPIO_VCONN_DISCHARGE2_CLK_ENABLE();} else \
                                          if((__GPIO__) == GPIO_V_CTL1)      {GPIO_V_CTL1_CLK_ENABLE();} else \
                                          if((__GPIO__) == GPIO_V_CTL2)      {GPIO_V_CTL2_CLK_ENABLE();}} while(0u)

#define ADCx_CHANNEL_GPIO_CLK_ENABLE(__ADC_CHANNEL__) \
                                            do {  if ((__ADC_CHANNEL__) == ADCx_CHANNEL_VSENSE_1) {ADCx_CHANNEL_VSENSE_1_GPIO_CLK_ENABLE();} else \
                                                  if ((__ADC_CHANNEL__) == ADCx_CHANNEL_ISENSE_1) {ADCx_CHANNEL_ISENSE_1_GPIO_CLK_ENABLE();} else \
                                                  if ((__ADC_CHANNEL__) == ADCx_CHANNEL_VSENSE_2) {ADCx_CHANNEL_VSENSE_2_GPIO_CLK_ENABLE();} else \
                                                  if ((__ADC_CHANNEL__) == ADCx_CHANNEL_ISENSE_2) {ADCx_CHANNEL_VSENSE_2_GPIO_CLK_ENABLE();} else \
                                                  if ((__ADC_CHANNEL__) == ADCx_CHANNEL_VSENSE_DCDC) {ADCx_CHANNEL_VSENSE_DCDC_GPIO_CLK_ENABLE();}} while(0u)

#define ABS(__VAL__) (((int32_t)(__VAL__)) < 0 ? - ((int32_t)(__VAL__)) : ((int32_t)(__VAL__)))

/* USER CODE END POWER_Private_Macros */
/**
  * @}
  */

/** @defgroup STM32G081B_EVAL_POWER_Private_Variables Private Variables
  * @{
  */
/* USER CODE BEGIN POWER_Private_Variables */
/**
* @brief GPIO variables
*/
/* GPIO Id. / GPIO port cross table */
static GPIO_TypeDef *GPIO_PORT[] =
{
    GPIO_SOURCE_EN_PORT,
    GPIO_VBUS_DISCHARGE1_PORT,
    GPIO_VBUS_DISCHARGE2_PORT,
    GPIO_VCONN_EN1_PORT,
    GPIO_VCONN_EN2_PORT,
    GPIO_VCONN_DISCHARGE1_PORT,
    GPIO_VCONN_DISCHARGE2_PORT,
    GPIO_V_CTL1_PORT,
    GPIO_V_CTL2_PORT
};

/* GPIO Id. / GPIO pin cross table */
static const uint16_t GPIO_PIN[] =
{
    GPIO_SOURCE_EN_PIN,
    GPIO_VBUS_DISCHARGE1_PIN,
    GPIO_VBUS_DISCHARGE2_PIN,
    GPIO_VCONN_EN1_PIN,
    GPIO_VCONN_EN2_PIN,
    GPIO_VCONN_DISCHARGE1_PIN,
    GPIO_VCONN_DISCHARGE2_PIN,
    GPIO_V_CTL1_PIN,
    GPIO_V_CTL2_PIN
};

/* ADC Channel / GPIO port cross table */
static GPIO_TypeDef *  ADC_CHANNEL_PORT[ADCxChanneln] =
{
    ADCx_CHANNEL_VSENSE_1_GPIO_PORT,
    ADCx_CHANNEL_ISENSE_1_GPIO_PORT,
    ADCx_CHANNEL_VSENSE_2_GPIO_PORT,
    ADCx_CHANNEL_ISENSE_2_GPIO_PORT,
    ADCx_CHANNEL_VSENSE_DCDC_GPIO_PORT
};

/* ADC Channel / GPIO pin cross table */
const uint32_t ADC_CHANNEL_PIN[ADCxChanneln] =
{
    ADCx_CHANNEL_VSENSE_1_GPIO_PIN,
    ADCx_CHANNEL_ISENSE_1_GPIO_PIN,
    ADCx_CHANNEL_VSENSE_2_GPIO_PIN,
    ADCx_CHANNEL_ISENSE_2_GPIO_PIN,
    ADCx_CHANNEL_VSENSE_DCDC_GPIO_PIN
};

/* Variable containing ADC conversions results
   aADCxConvertedValues[0u]: VSENSE_2
   aADCxConvertedValues[1u]: VSENSE_1
   aADCxConvertedValues[2u]: ISENSE_1
   aADCxConvertedValues[3u]: ISENSE_2
*/
static __IO uint16_t   aADCxConvertedValues[4] = {0};

/* BSP PWR contextual data */
static ContextTypeDef Context =
{
    .ADCRefCount         = 0u,
    .DCDCCtrlMode        = DCDC_CTRL_MODE_UNKNOWN,
    .PortInfo =
    {
        /* TYPE_C_PORT_1 */
        {
            .State                      = PORT_NOT_INITIALIZED,
            .Role                       = POWER_ROLE_SOURCE,
            .VBUSDisconnectionThreshold = VBUS_DISCONNECTION_THRESHOLD,
            .VBUSConnectionStatus       = VBUS_NOT_CONNECTED,
            .pfnVBUSDetectCallback      = NULL,
            .VBUSVoltage                = 0u,
        },
        /* TYPE_C_PORT_2 */
        {
            .State                      = PORT_NOT_INITIALIZED,
            .Role                       = POWER_ROLE_SINK,
            .VBUSDisconnectionThreshold = VBUS_DISCONNECTION_THRESHOLD,
            .VBUSConnectionStatus       = VBUS_NOT_CONNECTED,
            .pfnVBUSDetectCallback      = NULL,
            .VBUSVoltage                = 0u,
        }
    }
};

/* VBUS sensing completed flag */
static uint8_t VBUSSensingCompleted = 0u;

/* Power source calibration related variables */
static uint16_t aPWMDutyCycleLUT[VBUS_LEVEL_NB] =
{
  0x00B0,   /* 5V */
  0x0200,   /* 9V */
  0x0300,   /* 15V */
  0x0400,   /* 18V */
  0x0000    /* Will be used to store latest PPS request */
};

/* LPTIM handle */
static LPTIM_HandleTypeDef hlptim  = {0u};

#if defined(DBG_LOG)
#define DBG_LOG_SIZE 10
uint8_t _DbgLogIndex = 0;

uint32_t _vbusout[DBG_LOG_SIZE];
int32_t  _Err[DBG_LOG_SIZE];
uint32_t _Pwm[DBG_LOG_SIZE];
int32_t  _PwmE[DBG_LOG_SIZE];
#endif /* DBG_LOG */

/* USER CODE END POWER_Private_Variables */
/**
  * @}
  */

/** @defgroup STM32G081B_EVAL_POWER_Private_Functions Private Functions
  * @{
  */
/* USER CODE BEGIN POWER_Private_Prototypes */
static uint8_t  PWR_InitSource(uint32_t Instance);
static uint8_t  PWR_DeInitSource(uint32_t Instance);
static uint8_t  PWR_InitSink(void);
static uint8_t  PWR_DeInitSink(uint32_t Instance);
static void     PWR_GPIO_Init(GPIO_Id_TypeDef gpio);
static void     PWR_GPIO_DeInit(GPIO_Id_TypeDef gpio);
static void     PWR_GPIO_On(GPIO_Id_TypeDef gpio);
static void     PWR_GPIO_Off(GPIO_Id_TypeDef gpio);
static uint8_t  PWR_ADC_SetConfig(void);
static void     PWR_ADC_ResetConfig(void);
static uint8_t  PWR_ADC_SetChannelConfig(ADC_ChannelId_TypeDef ADCChannel);
static void     PWR_ADC_ResetChannelConfig(ADC_ChannelId_TypeDef ADCChannel);
static uint32_t PWR_ConvertADCDataToVoltage(uint32_t ADCData);
static uint32_t PWR_ConvertVoltageToThreshold(uint32_t PWR_ConvertVoltageToThreshold);
static int32_t  PWR_ConvertADCDataToCurrent(uint32_t ADCData);
static uint8_t  PWR_StartVBusSensing(void);
static uint8_t  PWR_StopVBusSensing(void);
static uint8_t  PWR_PauseVBusSensing(void);
static uint8_t  PWR_ResumeVBusSensing(void);
static uint8_t  PWR_InitPowerSource(void);
static void     PWR_DeInitPowerSource(void);
static void     PWR_SetPWMDutyCycle(uint16_t PWMDutyCycle);
static uint16_t PWR_GetPWMDutyCycle(void);

static uint32_t PWR_DCDCPreChargeC57(uint32_t Instance);

static void     PWR_VBUSDischarge(uint32_t Instance,
                           uint32_t VbusStopInmV,
                           uint32_t VbusErrorInmV,
                           uint16_t Delay);

static PWR_StatusTypeDef PWR_VBUSSetVoltage(uint32_t Instance,
                                         uint32_t VBusInmV,
                                         uint16_t Precision);

static void ADCx_MspInit(ADC_HandleTypeDef *hadc);
static void ADCx_MspDeInit(ADC_HandleTypeDef *hadc);
static void LPTIMx_MspInit(LPTIM_HandleTypeDef *hlptim);
static void LPTIMx_MspDeInit(LPTIM_HandleTypeDef *hlptim);
/* USER CODE END POWER_Private_Prototypes */
/**
  * @}
  */

/** @defgroup STM32G081B_EVAL_POWER_Exported_Variables Exported Variables
  * @{
  */
extern ADC_HandleTypeDef   hadc1;
extern DMA_HandleTypeDef hdma_adc1;
/**
  * @}
  */

/** @addtogroup STM32G081B_EVAL_POWER_Exported_Functions
  * @{
  */
/**
  * @brief  Get the VSENSE_DCDC measurement with  DCDC_EN = 0 and = 1
  *         controller.
  * @param  voltageDCDCOff voltage DCDC Off.
  * @param  voltageDCDCOnT0  voltage DCDC Off at T0.
  * @param  voltageDCDCOnT10  voltage DCDC Off at T10 = 100ms.
  * @retval none
  */
void BSP_PWR_VBUSIsGPIO(uint32_t *voltageDCDCOff, uint32_t *voltageDCDCOnT0, uint32_t *voltageDCDCOnT10)
{
  uint8_t                  error = 0;
  GPIO_InitTypeDef         gpio_config = {0};
  ADC_HandleTypeDef        hadc = {0};
  ADC_ChannelConfTypeDef   adc_channel_config = {0};
  GPIO_InitTypeDef         GPIO_InitStruct;

  *voltageDCDCOff = *voltageDCDCOnT0 = *voltageDCDCOnT10 = 0;

  /* Enable GPIO clock */
  DB_DETECT_GPIO_CLK_ENABLE();

  /* Configure GPIO pin of the selected ADC channel */
  gpio_config.Pin = DB_DETECT_PIN;
  gpio_config.Mode = GPIO_MODE_ANALOG;
  gpio_config.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(DB_DETECT_GPIO_PORT, &gpio_config);

  /* Enable ADC clock */
  DB_ADC_CLK_ENABLE();

  /* Initialize ADC */
  hadc.Instance = DB_ADC;

  ADCx_MspDeInit(&hadc);
  if (HAL_ADC_DeInit(&hadc) != HAL_OK)
  {
    /* ADC de-initialization Error */
    error++;
  }

  hadc.Init.ClockPrescaler        = ADC_CLOCK_SYNC_PCLK_DIV4;
  hadc.Init.Resolution            = ADC_RESOLUTION_12B;
  hadc.Init.DataAlign             = ADC_DATAALIGN_RIGHT;
  hadc.Init.ScanConvMode          = ADC_SCAN_DIRECTION_FORWARD;
  hadc.Init.EOCSelection          = ADC_EOC_SINGLE_CONV;
  hadc.Init.LowPowerAutoWait      = DISABLE;
  hadc.Init.LowPowerAutoPowerOff  = DISABLE;
  hadc.Init.ContinuousConvMode    = ENABLE;
  hadc.Init.DiscontinuousConvMode = DISABLE;
  hadc.Init.ExternalTrigConv      = ADC_SOFTWARE_START;
  hadc.Init.ExternalTrigConvEdge  = ADC_EXTERNALTRIGCONVEDGE_NONE;
  hadc.Init.DMAContinuousRequests = DISABLE;
  hadc.Init.Overrun               = ADC_OVR_DATA_OVERWRITTEN;
  hadc.Init.SamplingTimeCommon1   = ADC_SAMPLETIME_39CYCLES_5;
  hadc.Init.OversamplingMode      = DISABLE;

  ADCx_MspInit(&hadc);
  if (HAL_ADC_Init(&hadc) != HAL_OK)
  {
    error++;
  }

  /* Run the ADC calibration */
  if (HAL_ADCEx_Calibration_Start(&hadc) != HAL_OK)
  {
    error++;
  }

  /* Configure the ADC channel used to identify the daughter board */
  adc_channel_config.Channel      = DB_ADC_CHANNEL;
  adc_channel_config.Rank         = ADC_RANK_CHANNEL_NUMBER;
  adc_channel_config.SamplingTime = ADC_SAMPLINGTIME_COMMON_1;

  if (HAL_ADC_ConfigChannel(&hadc, &adc_channel_config) != HAL_OK)
  {
    error++;
  }

  /* Initialise DCDC_EN = 0 */
  GPIO_V_CTL2_CLK_ENABLE();
  HAL_GPIO_DeInit(GPIO_PORT[GPIO_V_CTL2],GPIO_PIN[GPIO_V_CTL2]);
  GPIO_InitStruct.Pin   = GPIO_PIN[GPIO_V_CTL2];
  GPIO_InitStruct.Mode  = GPIO_MODE_OUTPUT_OD;
  GPIO_InitStruct.Pull  = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
  HAL_GPIO_Init(GPIO_PORT[GPIO_V_CTL2], &GPIO_InitStruct);
  HAL_GPIO_WritePin(GPIO_PORT[GPIO_V_CTL2], GPIO_PIN[GPIO_V_CTL2], GPIO_PIN_RESET);

  /* Start conversion */
  if (HAL_ADC_Start(&hadc) != HAL_OK)
  {
    error++;
  }

  /* Wait for conversion completion */
  if (HAL_ADC_PollForConversion(&hadc, 10) != HAL_OK)
  {
    error++;
  }

  /* Translate ADC conversion into voltage level */
  *voltageDCDCOff = __HAL_ADC_CALC_DATA_TO_VOLTAGE(VREFANALOG_VOLTAGE,
                                             HAL_ADC_GetValue(&hadc),
                                             hadc.Init.Resolution);
  /* stop conversion */
  if (HAL_ADC_Stop(&hadc) != HAL_OK)
  {
    error++;
  }

  /* Initialise DCDC_EN = 1 */
  HAL_GPIO_DeInit(GPIO_PORT[GPIO_V_CTL2], GPIO_PIN[GPIO_V_CTL2]);
  GPIO_InitStruct.Pin   = GPIO_PIN[GPIO_V_CTL2];
  GPIO_InitStruct.Mode  = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull  = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
  HAL_GPIO_Init(GPIO_PORT[GPIO_V_CTL2], &GPIO_InitStruct);
  HAL_GPIO_WritePin(GPIO_PORT[GPIO_V_CTL2], GPIO_PIN[GPIO_V_CTL2], GPIO_PIN_SET);

  HAL_GPIO_DeInit(GPIO_PORT[GPIO_V_CTL1], GPIO_PIN[GPIO_V_CTL1]);
  GPIO_InitStruct.Pin   = GPIO_PIN[GPIO_V_CTL1];
  GPIO_InitStruct.Mode  = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull  = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
  HAL_GPIO_Init(GPIO_PORT[GPIO_V_CTL1], &GPIO_InitStruct);
  HAL_GPIO_WritePin(GPIO_PORT[GPIO_V_CTL1], GPIO_PIN[GPIO_V_CTL1], GPIO_PIN_RESET);

  /* Start conversion */
  if (HAL_ADC_Start(&hadc) != HAL_OK)
  {
    error++;
  }

  /* Wait for conversion completion */
  if (HAL_ADC_PollForConversion(&hadc, 10) != HAL_OK)
  {
    error++;
  }

  /* Translate ADC conversion into voltage level */
  *voltageDCDCOnT0 = __HAL_ADC_CALC_DATA_TO_VOLTAGE(VREFANALOG_VOLTAGE,
                                             HAL_ADC_GetValue(&hadc),
                                             hadc.Init.Resolution);

  /* Start conversion */
  if (HAL_ADC_Stop(&hadc) != HAL_OK)
  {
    error++;
  }

  HAL_Delay(100);

  /* Start conversion */
  if (HAL_ADC_Start(&hadc) != HAL_OK)
  {
    error++;
  }

  /* Wait for conversion completion */
  if (HAL_ADC_PollForConversion(&hadc, 10) != HAL_OK)
  {
    error++;
  }

  /* Translate ADC conversion into voltage level */
  *voltageDCDCOnT10 = __HAL_ADC_CALC_DATA_TO_VOLTAGE(VREFANALOG_VOLTAGE,
                                             HAL_ADC_GetValue(&hadc),
                                             hadc.Init.Resolution);

  /* Uninit GPIO PIN */
  HAL_GPIO_DeInit(GPIO_PORT[GPIO_V_CTL2], GPIO_PIN[GPIO_V_CTL2]);
  HAL_GPIO_DeInit(GPIO_PORT[GPIO_V_CTL1], GPIO_PIN[GPIO_V_CTL1]);

  /* De-initialize ADC& */
  ADCx_MspDeInit(&hadc);
  HAL_ADC_DeInit(&hadc);

  /* Disable ADC clock */
  DB_ADC_CLK_DISABLE();

  /* Reset GPIO configuration */
  HAL_GPIO_DeInit(DB_DETECT_GPIO_PORT, gpio_config.Pin);
}

/**
  * @brief  Initialize the hardware resources used by the Type-C power delivery (PD)
  *         controller.
  * @param  Instance Type-C port identifier
  *         This parameter can be take one of the following values:
  *         @arg TYPE_C_PORT_1
  *         @arg TYPE_C_PORT_2
  * @retval PD controller status
  */
PWR_StatusTypeDef BSP_PWR_VBUSInit(uint32_t Instance)
{
  return ((BSP_ERROR_NONE == BSP_USBPD_PWR_VBUSInit(Instance)) ? PWR_OK : PWR_ERROR);
}

/**
  * @brief  Release the hardware resources used by the Type-C power delivery (PD)
  *         controller.
  * @param  Instance Type-C port identifier
  *         This parameter can be take one of the following values:
  *         @arg TYPE_C_PORT_1
  *         @arg TYPE_C_PORT_2
  * @retval PD controller status
  */
PWR_StatusTypeDef BSP_PWR_VBUSDeInit(uint32_t Instance)
{
  return ((BSP_ERROR_NONE == BSP_USBPD_PWR_VBUSDeInit(Instance)) ? PWR_OK : PWR_ERROR);
}

/**
  * @brief  Enable power supply over VBUS.
  * @param  Instance Type-C port identifier
  *         This parameter can be take one of the following values:
  *         @arg TYPE_C_PORT_1
  * @retval PD controller status
  */
PWR_StatusTypeDef BSP_PWR_VBUSOn(uint32_t Instance)
{
  return ((BSP_ERROR_NONE == BSP_USBPD_PWR_VBUSOn(Instance)) ? PWR_OK : PWR_ERROR);
}

/**
  * @brief  Disable power supply over VBUS.
  * @param  Instance Type-C port identifier
  *         This parameter can be take one of the following values:
  *         @arg TYPE_C_PORT_1
  * @retval PD controller status
  */
PWR_StatusTypeDef BSP_PWR_VBUSOff(uint32_t Instance)
{
  return ((BSP_ERROR_NONE == BSP_USBPD_PWR_VBUSOff(Instance)) ? PWR_OK : PWR_ERROR);
}

/**
  * @brief  Set a fixed/variable PDO and manage the power control.
  * @param  Instance Type-C port identifier
  *         This parameter can be take one of the following values:
  *         @arg TYPE_C_PORT_1
  * @param  VbusTargetInmv the vbus Target (in mV)
  * @param  OperatingCurrent the Operating Current (in mA)
  * @param  MaxOperatingCurrent the Max Operating Current (in mA)
  * @retval PD controller status
  */
PWR_StatusTypeDef BSP_PWR_VBUSSetVoltage_Fixed(uint32_t Instance,
                                               uint32_t VbusTargetInmv,
                                               uint32_t OperatingCurrent,
                                               uint32_t MaxOperatingCurrent)
{
  return ((BSP_ERROR_NONE == BSP_USBPD_PWR_VBUSSetVoltage_Fixed(Instance, VbusTargetInmv, OperatingCurrent, MaxOperatingCurrent)) ? PWR_OK : PWR_ERROR);
}

/**
  * @brief  Set a fixed/variable PDO and manage the power control.
  * @param  Instance Type-C port identifier
  *         This parameter can be take one of the following values:
  *         @arg TYPE_C_PORT_1
  * @param  VbusTargetMinInmv the vbus Target min (in mV)
  * @param  VbusTargetMaxInmv the vbus Target max (in mV)
  * @param  OperatingCurrent the Operating Current (in mA)
  * @param  MaxOperatingCurrent the Max Operating Current (in mA)
  * @retval PD controller status
  */
PWR_StatusTypeDef BSP_PWR_VBUSSetVoltage_Variable(uint32_t Instance,
                                                  uint32_t VbusTargetMinInmv,
                                                  uint32_t VbusTargetMaxInmv,
                                                  uint32_t OperatingCurrent,
                                                  uint32_t MaxOperatingCurrent)
{
  return ((BSP_ERROR_NONE == BSP_USBPD_PWR_VBUSSetVoltage_Variable(Instance, VbusTargetMinInmv, VbusTargetMaxInmv, OperatingCurrent, MaxOperatingCurrent)) ? PWR_OK : PWR_ERROR);
}

/**
  * @brief  Set a Battery PDO and manage the power control.
  * @param  Instance Type-C port identifier
  *         This parameter can be take one of the following values:
  *         @arg TYPE_C_PORT_1
  * @param  VbusTargetMin the vbus Target min (in mV)
  * @param  VbusTargetMax the vbus Target max (in mV)
  * @param  OperatingPower the Operating Power (in mW)
  * @param  MaxOperatingPower the Max Operating Power (in mW)
  * @retval PD controller status
  */
PWR_StatusTypeDef BSP_PWR_VBUSSetVoltage_Battery(uint32_t Instance,
                                                 uint32_t VbusTargetMin,
                                                 uint32_t VbusTargetMax,
                                                 uint32_t OperatingPower,
                                                 uint32_t MaxOperatingPower)
{
  return ((BSP_ERROR_NONE == BSP_USBPD_PWR_VBUSSetVoltage_Battery(Instance, VbusTargetMin, VbusTargetMax, OperatingPower, MaxOperatingPower)) ? PWR_OK : PWR_ERROR);
}

/**
  * @brief  Set a APDO and manage the power control.
  * @param  Instance Type-C port identifier
  *         This parameter can be take one of the following values:
  *         @arg TYPE_C_PORT_1
  * @param  VbusTargetInmv the vbus Target (in mV)
  * @param  OperatingCurrent the Operating current (in mA)
  * @param  Delta Delta between with previous APDO (in mV), 0 means APDO start
  * @retval PD controller status
  */
PWR_StatusTypeDef BSP_PWR_VBUSSetVoltage_APDO(uint32_t Instance,
                                              uint32_t VbusTargetInmv,
                                              uint32_t OperatingCurrent,
                                              int32_t Delta)
{
  return ((BSP_ERROR_NONE == BSP_USBPD_PWR_VBUSSetVoltage_APDO(Instance, VbusTargetInmv, OperatingCurrent, Delta)) ? PWR_OK : PWR_ERROR);
}

/**
  * @brief  Get actual voltage level measured on the VBUS line.
  * @param  Instance Type-C port identifier
  *         This parameter can be take one of the following values:
  *         @arg TYPE_C_PORT_1
  *         @arg TYPE_C_PORT_2
  * @retval Voltage measured voltage level (in mV)
  */
uint32_t  BSP_PWR_VBUSGetVoltage(uint32_t Instance)
{
  uint32_t voltage = 0;

  (void)BSP_USBPD_PWR_VBUSGetVoltage(Instance, &voltage);

  return voltage;
}

/**
  * @brief  Get actual current level measured on the VBUS line.
  * @param  Instance Type-C port identifier
  *         This parameter can be take one of the following values:
  *         @arg TYPE_C_PORT_1
  *         @arg TYPE_C_PORT_2
  * @retval Current measured current level (in mA)
  */
int32_t BSP_PWR_VBUSGetCurrent(uint32_t Instance)
{
  int32_t current = 0;

  (void)BSP_USBPD_PWR_VBUSGetCurrent(Instance, &current);

  return current;
}

/**
  * @brief  Initialize VCONN sourcing.
  * @param  Instance Type-C port identifier
  *         This parameter can be take one of the following values:
  *         @arg TYPE_C_PORT_1
  *         @arg TYPE_C_PORT_2
  * @param  CCPinId Type-C CC pin identifier
  *         This parameter can be take one of the following values:
  *         @arg TYPE_C_CC1
  *         @arg TYPE_C_CC2
  * @retval PD controller status
  */
PWR_StatusTypeDef BSP_PWR_VCONNInit(uint32_t Instance,
                                    uint32_t CCPinId)
{
  return ((BSP_ERROR_NONE == BSP_USBPD_PWR_VCONNInit(Instance, CCPinId)) ? PWR_OK : PWR_ERROR);
}

/**
  * @brief  Un-Initialize VCONN sourcing.
  * @param  Instance Type-C port identifier
  *         This parameter can be take one of the following values:
  *         @arg TYPE_C_PORT_1
  *         @arg TYPE_C_PORT_2
  * @param  CCPinId Type-C CC pin identifier
  *         This parameter can be take one of the following values:
  *         @arg TYPE_C_CC1
  *         @arg TYPE_C_CC2
  * @retval PD controller status
  */
PWR_StatusTypeDef BSP_PWR_VCONNDeInit(uint32_t Instance,
                                      uint32_t CCPinId)
{
  return ((BSP_ERROR_NONE == BSP_USBPD_PWR_VCONNDeInit(Instance, CCPinId)) ? PWR_OK : PWR_ERROR);
}

/**
  * @brief  Enable VCONN sourcing.
  * @param  Instance Type-C port identifier
  *         This parameter can be take one of the following values:
  *         @arg TYPE_C_PORT_1
  *         @arg TYPE_C_PORT_2
  * @param  CCPinId Type-C CC pin identifier
  *         This parameter can be take one of the following values:
  *         @arg TYPE_C_CC1
  *         @arg TYPE_C_CC2
  * @retval PD controller status
  */
PWR_StatusTypeDef BSP_PWR_VCONNOn(uint32_t Instance,
                                  uint32_t CCPinId)
{
  return ((BSP_ERROR_NONE == BSP_USBPD_PWR_VCONNOn(Instance, CCPinId)) ? PWR_OK : PWR_ERROR);
}

/**
  * @brief  Disable VCONN sourcing.
  * @param  Instance Type-C port identifier
  *         This parameter can be take one of the following values:
  *         @arg TYPE_C_PORT_1
  *         @arg TYPE_C_PORT_2
  * @param  CCPinId CC pin identifier
  *         This parameter can be take one of the following values:
  *         @arg TYPE_C_CC1
  *         @arg TYPE_C_CC2
  * @retval PD controller status
  */
PWR_StatusTypeDef BSP_PWR_VCONNOff(uint32_t Instance,
                                   uint32_t CCPinId)
{
  return ((BSP_ERROR_NONE == BSP_USBPD_PWR_VCONNOff(Instance, CCPinId)) ? PWR_OK : PWR_ERROR);
}

/**
  * @brief  Set the VBUS disconnection voltage threshold.
  * @note   Callback funtion registered through BSP_PWR_RegisterVBUSDetectCallback
  *         function call is invoked when VBUS falls below programmed threshold.
  * @note   By default VBUS disconnection threshold is set to 3.3V
  * @param  Instance Type-C port identifier
  *         This parameter can be take one of the following values:
  *         @arg TYPE_C_PORT_1
  *         @arg TYPE_C_PORT_2
  * @param  VoltageThreshold: VBUS disconnection voltage threshold (in mV)
  * @retval PD controller status
  */
void BSP_PWR_SetVBUSDisconnectionThreshold(uint32_t Instance,
                                           uint32_t VoltageThreshold)
{
  (void)BSP_USBPD_PWR_SetVBUSDisconnectionThreshold(Instance, VoltageThreshold);
  return;
}

/**
  * @brief  Register USB Type-C Current callback function.
  * @note   Callback function invoked when VBUS rises above 4V (VBUS present) or
  *         when VBUS falls below programmed threshold (VBUS absent).
  * @note   Callback funtion is un-registered when callback function pointer
  *         argument is NULL.
  * @param  Instance Type-C port identifier
  *         This parameter can be take one of the following values:
  *         @arg TYPE_C_PORT_1
  *         @arg TYPE_C_PORT_2
  * @param  pfnVBUSDetectCallback callback function pointer
  * @retval 0 success else fail
  */
PWR_StatusTypeDef BSP_PWR_RegisterVBUSDetectCallback(uint32_t                       Instance,
                                                     PWR_VBUSDetectCallbackFunc *   pfnVBUSDetectCallback)
{
  return ((BSP_ERROR_NONE == BSP_USBPD_PWR_RegisterVBUSDetectCallback(Instance, (USBPD_PWR_VBUSDetectCallbackFunc*)pfnVBUSDetectCallback)) ? PWR_OK : PWR_ERROR);
}


/**
  * @brief  Get actual VBUS status.
  * @param  Instance Type-C port identifier
  *         This parameter can be take one of the following values:
  *         @arg TYPE_C_PORT_1
  * @retval VBUS status (1: On, 0: Off)
  */
uint8_t BSP_PWR_VBUSIsOn(uint32_t Instance)
{
  uint8_t state = 0;
  
  BSP_USBPD_PWR_VBUSIsOn(Instance, &state);

  return state;
}

/**
  * @brief  Get actual VCONN status.
  * @param  Instance Type-C port identifier
  *         This parameter can be take one of the following values:
  *         @arg TYPE_C_PORT_1
  * @param  CCPinId Type-C CC pin identifier
  *         This parameter can be take one of the following values:
  *         @arg TYPE_C_CC1
  *         @arg TYPE_C_CC2
  * @retval VCONN status (1: On, 0: Off)
  */
uint8_t BSP_PWR_VCONNIsOn(uint32_t Instance,
                          uint32_t CCPinId)
{
  uint8_t state = 0;
  
  BSP_USBPD_PWR_VCONNIsOn(Instance, CCPinId, &state);

  return state;
}

/**
  * @brief  Get actual DCDC voltage level.
  * @param  Instance Type-C port identifier
  *         This parameter can be take one of the following values:
  *         @arg TYPE_C_PORT_1
  * @retval DCDC voltage level (in mV)
  */
uint32_t BSP_PWR_DCDCGetVoltage(uint32_t Instance)
{
  uint32_t voltage = 0;

  /* DCDC measure is possible only if the port operates as a source */
  if (Context.PortInfo[Instance].Role != POWER_ROLE_SINK)
  {
    /* Pause VBUS sensing */
    PWR_PauseVBusSensing();

    /* Configure the ADC channel used to sense DCDC voltage */
    if (PWR_ADC_SetChannelConfig(ADC_VSENSE_DCDC) == 0)
    {
      /* Start conversion */
      if (HAL_ADC_Start(&hadc1) != HAL_OK)
      {
        goto end_of_DCDC_voltage_measure;
      }

      /* Wait for conversion completion */
      if (HAL_ADC_PollForConversion(&hadc1, 10) != HAL_OK)
      {
        goto end_of_DCDC_voltage_measure;
      }

      /* Translate ADC conversion into voltage level */
      voltage = PWR_ConvertADCDataToVoltage(HAL_ADC_GetValue(&hadc1));

      /* Stop conversion */
      if (HAL_ADC_Stop(&hadc1) != HAL_OK)
      {
        goto end_of_DCDC_voltage_measure;
      }

      /* Reset ADC channel configuration */
      PWR_ADC_ResetChannelConfig(ADC_VSENSE_DCDC);
    }

    /* Resume VBUS senging */
    PWR_ResumeVBusSensing();
  }

end_of_DCDC_voltage_measure:
  return voltage;
}

/**
  * @brief  Get DCDC control mode.
  * @param  Instance Type-C port identifier
  *         This parameter can be take one of the following values:
  *         @arg TYPE_C_PORT_1
  * @retval DCDC control mode
  *         Returned value can take one of the following values:
  *         @arg DCDC_CTRL_MODE_UNKNOWN
  *         @arg DCDC_CTRL_MODE_GPIO
  *         @arg DCDC_CTRL_MODE_PWM
  */
PWR_DCDCCtrlModeTypeDef BSP_PWR_DCDCGetCtrlMode(uint32_t Instance)
{
  USBPD_PWR_DCDCCtrlModeTypeDef dcdc_ctrl = DCDC_CTRL_MODE_UNKNOWN;

  (void)BSP_USBPD_PWR_DCDCGetCtrlMode(Instance, &dcdc_ctrl);

  return (PWR_DCDCCtrlModeTypeDef)dcdc_ctrl;
}

/**
  * @}
  */

/** @addtogroup STM32G081B_EVAL_USBPD_POWER_Exported_Functions2
  * @{
  */

/**
  * @brief  Global initialization of PWR resource used by USB-PD
  * @param  Instance Type-C port identifier
  *         This parameter can be take one of the following values:
  *         @arg @ref USBPD_PWR_TYPE_C_PORT_1
  *         @arg @ref USBPD_PWR_TYPE_C_PORT_2
  * @retval BSP status
  */
int32_t BSP_USBPD_PWR_Init(uint32_t Instance)
{
  /* USER CODE BEGIN BSP_USBPD_PWR_Init */
  /* Check if instance is valid       */
  int32_t ret = BSP_ERROR_NONE;

  if (Instance >= USBPD_PWR_INSTANCES_NBR)
  {
    ret = BSP_ERROR_WRONG_PARAM;
  }
  return ret;
  /* USER CODE END BSP_USBPD_PWR_Init */
}

/**
  * @brief  Global de-initialization of PWR resource used by USB-PD
  * @param  Instance Type-C port identifier
  *         This parameter can be take one of the following values:
  *         @arg @ref USBPD_PWR_TYPE_C_PORT_1
  *         @arg @ref USBPD_PWR_TYPE_C_PORT_2
  * @retval BSP status
  */
int32_t BSP_USBPD_PWR_Deinit(uint32_t Instance)
{
  /* USER CODE BEGIN BSP_USBPD_PWR_Deinit */
  /* Check if instance is valid       */
  int32_t ret = BSP_ERROR_NONE;

  if (Instance >= USBPD_PWR_INSTANCES_NBR)
  {
    ret = BSP_ERROR_WRONG_PARAM;
  }
  return ret;
  /* USER CODE END BSP_USBPD_PWR_Deinit */
}

/**
  * @brief  Initialize the hardware resources used by the Type-C power delivery (PD)
  *         controller.
  * @param  Instance Type-C port identifier
  *         This parameter can be take one of the following values:
  *         @arg @ref USBPD_PWR_TYPE_C_PORT_1
  *         @arg @ref USBPD_PWR_TYPE_C_PORT_2
  * @retval BSP status
  */
int32_t BSP_USBPD_PWR_VBUSInit(uint32_t Instance)
{
  /* USER CODE BEGIN BSP_USBPD_PWR_VBUSInit */
  /* Check if instance is valid       */
  int32_t ret = BSP_ERROR_NONE;

  if (Instance >= USBPD_PWR_INSTANCES_NBR)
  {
    ret = BSP_ERROR_WRONG_PARAM;
  }
  else
  {
    USBPD_PWR_PowerRoleTypeDef   role;

    if (Context.PortInfo[Instance].State == PORT_NOT_INITIALIZED)
    {
      if (Instance == USBPD_PWR_TYPE_C_PORT_1)
      {
        role = POWER_ROLE_DUAL;
        PWR_GPIO_Init(GPIO_VCONN_EN1);
        PWR_GPIO_Init(GPIO_VCONN_EN2);
      }
      else
      {
        role = POWER_ROLE_SINK;
      }

      /* SINK / SOURCE initialization order is important: in case of dual role
         SINK must be initialized before SOURCE. */
      if ((role == POWER_ROLE_SINK) || (role == POWER_ROLE_DUAL))
      {
        if (0u == PWR_InitSink())
        {
          if (Context.ADCRefCount == 1u)
          {
            if (0u != PWR_StartVBusSensing())
            {
              ret = BSP_ERROR_PERIPH_FAILURE;
            }
          }
        }
        else
        {
          ret = BSP_ERROR_PERIPH_FAILURE;
        }
      }

      if ((BSP_ERROR_NONE == ret) &&
          ((role == POWER_ROLE_SOURCE) || (role == POWER_ROLE_DUAL)))
      {
        if (0u != PWR_InitSource(Instance))
        {
          ret = BSP_ERROR_PERIPH_FAILURE;
        }
      }

      /* Update PWR context */
      Context.PortInfo[Instance].State = PORT_INITIALIZED;
      Context.PortInfo[Instance].Role = role;
    }
  }
  return ret;
  /* USER CODE END BSP_USBPD_PWR_VBUSInit */
}

/**
  * @brief  Release the hardware resources used by the Type-C power delivery (PD)
  *         controller.
  * @param  Instance Type-C port identifier
  *         This parameter can be take one of the following values:
  *         @arg @ref USBPD_PWR_TYPE_C_PORT_1
  *         @arg @ref USBPD_PWR_TYPE_C_PORT_2
  * @retval BSP status
  */
int32_t BSP_USBPD_PWR_VBUSDeInit(uint32_t Instance)
{
  /* USER CODE BEGIN BSP_USBPD_PWR_VBUSDeInit */
  /* Check if instance is valid       */
  int32_t ret = BSP_ERROR_NONE;

  if (Instance >= USBPD_PWR_INSTANCES_NBR)
  {
    ret = BSP_ERROR_WRONG_PARAM;
  }
  else
  {
    if (Context.PortInfo[Instance].State == PORT_INITIALIZED)
    {
      if (Instance == USBPD_PWR_TYPE_C_PORT_1)
      {
        PWR_GPIO_DeInit(GPIO_VCONN_EN1);
        PWR_GPIO_DeInit(GPIO_VCONN_EN2);
      }

      if ((Context.PortInfo[Instance].Role == POWER_ROLE_SOURCE) ||
          (Context.PortInfo[Instance].Role == POWER_ROLE_DUAL))
      {
        if (0u != PWR_DeInitSource(Instance))
        {
          ret = BSP_ERROR_PERIPH_FAILURE;
        }
      }

      if ((Context.PortInfo[Instance].Role == POWER_ROLE_SINK) ||
          (Context.PortInfo[Instance].Role == POWER_ROLE_DUAL))
      {
        if (Context.ADCRefCount == 1u)
        {
          if (0u != PWR_StopVBusSensing())
          {
            ret = BSP_ERROR_PERIPH_FAILURE;
          }
        }
        if (BSP_ERROR_NONE == ret)
        {
          if (0u != PWR_DeInitSink(Instance))
          {
            ret = BSP_ERROR_PERIPH_FAILURE;
          }
        }
      }

      /* Update PWR context */
      Context.PortInfo[Instance].State = PORT_NOT_INITIALIZED;
    }
  }
  return ret;
  /* USER CODE END BSP_USBPD_PWR_VBUSDeInit */
}

/**
  * @brief  Enable power supply over VBUS.
  * @param  Instance Type-C port identifier
  *         This parameter can be take one of the following values:
  *         @arg @ref USBPD_PWR_TYPE_C_PORT_1
  * @retval BSP status
  */
int32_t BSP_USBPD_PWR_VBUSOn(uint32_t Instance)
{
  /* USER CODE BEGIN BSP_USBPD_PWR_VBUSOn */
  /* Check if instance is valid       */
  int32_t ret;

  if (Instance >= USBPD_PWR_INSTANCES_NBR)
  {
    ret = BSP_ERROR_WRONG_PARAM;
  }
  else
  {
    if (Instance == USBPD_PWR_TYPE_C_PORT_1)
    {
      /* Start VBUS Discharge */
      PWR_GPIO_On(GPIO_VBUS_DISCHARGE1);

      /* safety control to avoid spike when enabling the power */
      /* issue detected during board reset */
      uint32_t _tickstart = HAL_GetTick();
      uint32_t _dcdc;
      uint32_t _delta = BSP_PWR_DCDCGetVoltage(Instance);

      do
      {
        _dcdc = BSP_PWR_DCDCGetVoltage(Instance);

        /* check after 2 second check if the power has start to decrease */
        if ((((HAL_GetTick() - _tickstart) > 2000u)) && ((_delta - _dcdc) < 500u))
        {
#if defined(_TRACE)
          uint8_t _str[20];
          sprintf((char *)_str,"V:%ld-%ld", _dcdc, _delta);
          USBPD_TRACE_Add(USBPD_TRACE_DEBUG, 0, 0, _str, strlen((char *)_str));
          USBPD_TRACE_Add(USBPD_TRACE_DEBUG, 0, 0, (uint8_t *) "V:decrease error", 16);
#endif
          return BSP_ERROR_PERIPH_FAILURE;
        }

        /* Timeout management */
        if ((HAL_GetTick() - _tickstart) > 15000u)
        {
#if defined(_TRACE)
          USBPD_TRACE_Add(USBPD_TRACE_DEBUG, 0, 0, (uint8_t *) "V:timeout", 9);
#endif
          return BSP_ERROR_PERIPH_FAILURE;
        }
      }
      while (_dcdc > 5500u);

      /* Switch on VBUS */
      PWR_GPIO_On(GPIO_SOURCE_EN);

      /* Set VBUS to its default voltage level */
      ret = PWR_VBUSSetVoltage(Instance, VBUS_VOLTAGE_5V_IN_MV, 200u); /* 5% max = 250mV max */

      /* Stop VBUS Discharge */
      PWR_GPIO_Off(GPIO_VBUS_DISCHARGE1);
    }
    else
    {
      ret = BSP_ERROR_PERIPH_FAILURE;
    }

    /* Save the power context */
    Context.PortInfo[Instance].VBUSVoltage = VBUS_VOLTAGE_5V_IN_MV;
    Context.PortInfo[Instance].Type = POWER_FIXED;

  }
  return ret;
  /* USER CODE END BSP_USBPD_PWR_VBUSOn */
}

/**
  * @brief  Disable power supply over VBUS.
  * @param  Instance Type-C port identifier
  *         This parameter can be take one of the following values:
  *         @arg @ref USBPD_PWR_TYPE_C_PORT_1
  * @retval BSP status
  */
int32_t BSP_USBPD_PWR_VBUSOff(uint32_t Instance)
{
  /* USER CODE BEGIN BSP_USBPD_PWR_VBUSOff */
  /* Check if instance is valid       */
  int32_t ret = BSP_ERROR_NONE;

  if (Instance >= USBPD_PWR_INSTANCES_NBR)
  {
    ret = BSP_ERROR_WRONG_PARAM;
  }
  else
  {
    if (Instance == USBPD_PWR_TYPE_C_PORT_1)
    {
      /* Go back to original VBUS value */
      ret = PWR_VBUSSetVoltage(0u, VBUS_VOLTAGE_0V_IN_MV, 0u);

      /* Discharge VBUS */
      PWR_VBUSDischarge(Instance, USBPD_PWR_LOW_VBUS_THRESHOLD, 0u, 0u);
    }

    /* Save the power context */
    Context.PortInfo[Instance].VBUSVoltage = VBUS_VOLTAGE_0V_IN_MV;
    Context.PortInfo[Instance].Type = POWER_NONE;
  }
  return ret;
  /* USER CODE END BSP_USBPD_PWR_VBUSOff */
}

/**
  * @brief  Set a fixed/variable PDO and manage the power control.
  * @param  Instance Type-C port identifier
  *         This parameter can be take one of the following values:
  *         @arg @ref USBPD_PWR_TYPE_C_PORT_1
  * @param  VbusTargetInmv the vbus Target (in mV)
  * @param  OperatingCurrent the Operating Current (in mA)
  * @param  MaxOperatingCurrent the Max Operating Current (in mA)
  * @retval BSP status
  */
int32_t BSP_USBPD_PWR_VBUSSetVoltage_Fixed(uint32_t Instance,
                                           uint32_t VbusTargetInmv,
                                           uint32_t OperatingCurrent,
                                           uint32_t MaxOperatingCurrent)
{
  /* USER CODE BEGIN BSP_USBPD_PWR_VBUSSetVoltage_Fixed */
  /* Check if instance is valid       */
  int32_t ret = BSP_ERROR_NONE;
  UNUSED(MaxOperatingCurrent);
  UNUSED(OperatingCurrent);

  if (Instance >= USBPD_PWR_INSTANCES_NBR)
  {
    ret = BSP_ERROR_WRONG_PARAM;
  }
  else
  {
    if (Context.PortInfo[Instance].VBUSVoltage != VbusTargetInmv)
    {
      /* Start VBUS Discharge, done to increase the decrease speed */
      PWR_GPIO_On(GPIO_VBUS_DISCHARGE1);

      /* Set the power, the precision must be at 5% */
      ret = PWR_VBUSSetVoltage(Instance, VbusTargetInmv, (uint16_t)(VbusTargetInmv * 5u / 100u));

      /* Wait the power stabilization */
      PWR_GPIO_Off(GPIO_VBUS_DISCHARGE1);
    }

    /* Upate the context */
    Context.PortInfo[Instance].VBUSVoltage = VbusTargetInmv;
    Context.PortInfo[Instance].Type = POWER_FIXED;

    /* Set the current limitation */
    /* not implemented */

  }
  return ret;
  /* USER CODE END BSP_USBPD_PWR_VBUSSetVoltage_Fixed */
}

/**
  * @brief  Set a fixed/variable PDO and manage the power control.
  * @param  Instance Type-C port identifier
  *         This parameter can be take one of the following values:
  *         @arg @ref USBPD_PWR_TYPE_C_PORT_1
  * @param  VbusTargetMinInmv the vbus Target min (in mV)
  * @param  VbusTargetMaxInmv the vbus Target max (in mV)
  * @param  OperatingCurrent the Operating Current (in mA)
  * @param  MaxOperatingCurrent the Max Operating Current (in mA)
  * @retval BSP status
  */
int32_t BSP_USBPD_PWR_VBUSSetVoltage_Variable(uint32_t Instance,
                                              uint32_t VbusTargetMinInmv,
                                              uint32_t VbusTargetMaxInmv,
                                              uint32_t OperatingCurrent,
                                              uint32_t MaxOperatingCurrent)
{
  /* USER CODE BEGIN BSP_USBPD_PWR_VBUSSetVoltage_Variable */
  /* Check if instance is valid       */
  int32_t ret;
  UNUSED(MaxOperatingCurrent);
  UNUSED(OperatingCurrent);
  UNUSED(VbusTargetMaxInmv);

  if (Instance >= USBPD_PWR_INSTANCES_NBR)
  {
    ret = BSP_ERROR_WRONG_PARAM;
  }
  else
  {
    uint32_t vbus_target_inmv = (VbusTargetMinInmv + VbusTargetMinInmv) / 2u;

    /* Set the power, the precision must be at 5% */
    ret = PWR_VBUSSetVoltage(Instance, vbus_target_inmv, (uint16_t)(vbus_target_inmv * 5u / 100u));

    /* Set the current limitation */
    /* not implemented */

  }
  return ret;
  /* USER CODE BEGIN BSP_USBPD_PWR_VBUSSetVoltage_Variable */
}

/**
  * @brief  Set a Battery PDO and manage the power control.
  * @param  Instance Type-C port identifier
  *         This parameter can be take one of the following values:
  *         @arg @ref USBPD_PWR_TYPE_C_PORT_1
  * @param  VbusTargetMin the vbus Target min (in mV)
  * @param  VbusTargetMax the vbus Target max (in mV)
  * @param  OperatingPower the Operating Power (in mW)
  * @param  MaxOperatingPower the Max Operating Power (in mW)
  * @retval BSP status
  */
int32_t BSP_USBPD_PWR_VBUSSetVoltage_Battery(uint32_t Instance,
                                             uint32_t VbusTargetMin,
                                             uint32_t VbusTargetMax,
                                             uint32_t OperatingPower,
                                             uint32_t MaxOperatingPower)
{
  /* USER CODE BEGIN BSP_USBPD_PWR_VBUSSetVoltage_Battery */
  /* Check if instance is valid       */
  int32_t ret;
  UNUSED(OperatingPower);
  UNUSED(VbusTargetMax);
  UNUSED(VbusTargetMin);
  UNUSED(MaxOperatingPower);

  if (Instance >= USBPD_PWR_INSTANCES_NBR)
  {
    ret = BSP_ERROR_WRONG_PARAM;
  }
  else
  {
    /* Set the power, the precision must be at 5% */
    /* Set the current limitation */
    /* not implemented */

    ret = BSP_ERROR_FEATURE_NOT_SUPPORTED;
  }
  return ret;
  /* USER CODE BEGIN BSP_USBPD_PWR_VBUSSetVoltage_Battery */
}

/**
  * @brief  Set a APDO and manage the power control.
  * @param  Instance Type-C port identifier
  *         This parameter can be take one of the following values:
  *         @arg @ref USBPD_PWR_TYPE_C_PORT_1
  * @param  VbusTargetInmv the vbus Target (in mV)
  * @param  OperatingCurrent the Operating current (in mA)
  * @param  Delta Delta between with previous APDO (in mV), 0 means APDO start
  * @retval BSP status
  */
int32_t BSP_USBPD_PWR_VBUSSetVoltage_APDO(uint32_t Instance,
                                          uint32_t VbusTargetInmv,
                                          uint32_t OperatingCurrent,
                                          int32_t Delta)
{
  /* USER CODE BEGIN BSP_USBPD_PWR_VBUSSetVoltage_APDO */
  /* Check if instance is valid       */
  int32_t ret = BSP_ERROR_NONE;
  UNUSED(Delta);
  UNUSED(OperatingCurrent);

  if (Instance >= USBPD_PWR_INSTANCES_NBR)
  {
    ret = BSP_ERROR_WRONG_PARAM;
  }
  else
  {
    /* set the new value with the current value */
    if (POWER_APDO != Context.PortInfo[Instance].Type)
    {
      switch (Context.PortInfo[Instance].VBUSVoltage)
      {
        case VBUS_VOLTAGE_0V_IN_MV :
        case VBUS_VOLTAGE_5V_IN_MV :
          aPWMDutyCycleLUT[VBUS_PPS_V] = aPWMDutyCycleLUT[VBUS_5_V];
          break;
        case VBUS_VOLTAGE_9V_IN_MV :
          aPWMDutyCycleLUT[VBUS_PPS_V] = aPWMDutyCycleLUT[VBUS_9_V];
          break;
        case VBUS_VOLTAGE_15V_IN_MV :
          aPWMDutyCycleLUT[VBUS_PPS_V] = aPWMDutyCycleLUT[VBUS_15_V];
          break;
        case VBUS_VOLTAGE_18V_IN_MV :
          aPWMDutyCycleLUT[VBUS_PPS_V] = aPWMDutyCycleLUT[VBUS_18_V];
          break;
        default:
          break;
      }
    }

    if (Context.PortInfo[Instance].VBUSVoltage != VbusTargetInmv)
    {
      /* Start VBUS Discharge, done to increase the decrease speed */
      PWR_GPIO_On(GPIO_VBUS_DISCHARGE1);

      ret = PWR_VBUSSetVoltage(Instance, VbusTargetInmv, USBPD_PWR_DCDC_PRECISION);
      /* Wait the power stabilization */
      PWR_GPIO_Off(GPIO_VBUS_DISCHARGE1);

      /* Upate the context */
      Context.PortInfo[Instance].VBUSVoltage = VbusTargetInmv;
      Context.PortInfo[Instance].Type = POWER_APDO;
    }

    /* Set the current limitation */
    /* not implemented */
  }
  return ret;
  /* USER CODE BEGIN BSP_USBPD_PWR_VBUSSetVoltage_APDO */
}

/**
  * @brief  Get actual voltage level measured on the VBUS line.
  * @param  Instance Type-C port identifier
  *         This parameter can be take one of the following values:
  *         @arg @ref USBPD_PWR_TYPE_C_PORT_1
  *         @arg @ref USBPD_PWR_TYPE_C_PORT_2
  * @param  pVoltage Pointer on measured voltage level (in mV)
  * @retval BSP status
  */
int32_t BSP_USBPD_PWR_VBUSGetVoltage(uint32_t Instance, uint32_t *pVoltage)
{
  /* USER CODE BEGIN BSP_USBPD_PWR_VBUSGetVoltage */
  /* Check if instance is valid       */
  int32_t ret = BSP_ERROR_NONE;

  if ((Instance >= USBPD_PWR_INSTANCES_NBR) || (NULL == pVoltage))
  {
    ret = BSP_ERROR_WRONG_PARAM;
  }
  else
  {
    uint32_t voltage = 0u;


    if (Instance == USBPD_PWR_TYPE_C_PORT_1)
    {
      voltage = PWR_ConvertADCDataToVoltage(aADCxConvertedValues[1u]);
    }
    else
    {
      if (Instance == USBPD_PWR_TYPE_C_PORT_2)
      {
        voltage = PWR_ConvertADCDataToVoltage(aADCxConvertedValues[0u]);
      }
    }

    *pVoltage = voltage;
  }
  return ret;
  /* USER CODE END BSP_USBPD_PWR_VBUSGetVoltage */
}

/**
  * @brief  Get actual current level measured on the VBUS line.
  * @param  Instance Type-C port identifier
  *         This parameter can be take one of the following values:
  *         @arg @ref USBPD_PWR_TYPE_C_PORT_1
  *         @arg @ref USBPD_PWR_TYPE_C_PORT_2
  * @param  pCurrent Pointer on measured current level (in mA)
  * @retval BSP status
  */
int32_t BSP_USBPD_PWR_VBUSGetCurrent(uint32_t Instance, int32_t *pCurrent)
{
  /* USER CODE BEGIN BSP_USBPD_PWR_VBUSGetCurrent */
  /* Check if instance is valid       */
  int32_t ret = BSP_ERROR_NONE;

  if ((Instance >= USBPD_PWR_INSTANCES_NBR) || (NULL == pCurrent))
  {
    ret = BSP_ERROR_WRONG_PARAM;
  }
  else
  {
    int32_t current = 0;

    if (Instance == USBPD_PWR_TYPE_C_PORT_1)
    {
      current = PWR_ConvertADCDataToCurrent(aADCxConvertedValues[2u]);
    }
    else
    {
      if (Instance == USBPD_PWR_TYPE_C_PORT_2)
      {
        current = PWR_ConvertADCDataToCurrent(aADCxConvertedValues[3u]);
      }
    }

    *pCurrent = current;
  }
  return ret;
  /* USER CODE END BSP_USBPD_PWR_VBUSGetCurrent */
}

/**
  * @brief  Initialize VCONN sourcing.
  * @param  Instance Type-C port identifier
  *         This parameter can be take one of the following values:
  *         @arg @ref USBPD_PWR_TYPE_C_PORT_1
  *         @arg @ref USBPD_PWR_TYPE_C_PORT_2
  * @param  CCPinId Type-C CC pin identifier
  *         This parameter can be take one of the following values:
  *         @arg @ref USBPD_PWR_TYPE_C_CC1
  *         @arg @ref USBPD_PWR_TYPE_C_CC2
  * @retval BSP status
  */
int32_t BSP_USBPD_PWR_VCONNInit(uint32_t Instance,
                                uint32_t CCPinId)
{
  /* USER CODE BEGIN BSP_USBPD_PWR_VCONNInit */
  /* Check if instance is valid       */
  int32_t ret;

  if (Instance >= USBPD_PWR_INSTANCES_NBR)
  {
    ret = BSP_ERROR_WRONG_PARAM;
  }
  else
  {
    ret = BSP_ERROR_NO_INIT;

    if (Instance == USBPD_PWR_TYPE_C_PORT_1)
    {
      switch (CCPinId)
      {
        case USBPD_PWR_TYPE_C_CC1 :
        {
          PWR_GPIO_Init(GPIO_VCONN_DISCHARGE1);
          ret = BSP_ERROR_NONE;
          break;
        }
        case USBPD_PWR_TYPE_C_CC2 :
        {
          PWR_GPIO_Init(GPIO_VCONN_DISCHARGE2);
          ret = BSP_ERROR_NONE;
          break;
        }
        default:
          break;
      }
    }
  }
  return ret;
  /* USER CODE END BSP_USBPD_PWR_VCONNInit */
}

/**
  * @brief  Un-Initialize VCONN sourcing.
  * @param  Instance Type-C port identifier
  *         This parameter can be take one of the following values:
  *         @arg @ref USBPD_PWR_TYPE_C_PORT_1
  *         @arg @ref USBPD_PWR_TYPE_C_PORT_2
  * @param  CCPinId Type-C CC pin identifier
  *         This parameter can be take one of the following values:
  *         @arg @ref USBPD_PWR_TYPE_C_CC1
  *         @arg @ref USBPD_PWR_TYPE_C_CC2
  * @retval BSP status
  */
int32_t BSP_USBPD_PWR_VCONNDeInit(uint32_t Instance,
                                  uint32_t CCPinId)
{
  /* USER CODE BEGIN BSP_USBPD_PWR_VCONNDeInit */
  /* Check if instance is valid       */
  int32_t ret;

  if (Instance >= USBPD_PWR_INSTANCES_NBR)
  {
    ret = BSP_ERROR_WRONG_PARAM;
  }
  else
  {
    ret = BSP_ERROR_NO_INIT;

    if (Instance == USBPD_PWR_TYPE_C_PORT_1)
    {
      switch (CCPinId)
      {
        case USBPD_PWR_TYPE_C_CC1 :
        {
          PWR_GPIO_DeInit(GPIO_VCONN_DISCHARGE1);
          ret = BSP_ERROR_NONE;
          break;
        }
        case USBPD_PWR_TYPE_C_CC2 :
        {
          PWR_GPIO_DeInit(GPIO_VCONN_DISCHARGE2);
          ret = BSP_ERROR_NONE;
          break;
        }
        default:
          break;
      }
    }
  }
  return ret;
  /* USER CODE END BSP_USBPD_PWR_VCONNDeInit */
}

/**
  * @brief  Enable VCONN sourcing.
  * @param  Instance Type-C port identifier
  *         This parameter can be take one of the following values:
  *         @arg @ref USBPD_PWR_TYPE_C_PORT_1
  *         @arg @ref USBPD_PWR_TYPE_C_PORT_2
  * @param  CCPinId Type-C CC pin identifier
  *         This parameter can be take one of the following values:
  *         @arg @ref USBPD_PWR_TYPE_C_CC1
  *         @arg @ref USBPD_PWR_TYPE_C_CC2
  * @retval BSP status
  */
int32_t BSP_USBPD_PWR_VCONNOn(uint32_t Instance,
                              uint32_t CCPinId)
{
  /* USER CODE BEGIN BSP_USBPD_PWR_VCONNOn */
  /* Check if instance is valid       */
  int32_t ret;

  if (Instance >= USBPD_PWR_INSTANCES_NBR)
  {
    ret = BSP_ERROR_WRONG_PARAM;
  }
  else
  {
    ret = BSP_ERROR_PERIPH_FAILURE;

    if (Instance == USBPD_PWR_TYPE_C_PORT_1)
    {
      if (CCPinId == USBPD_PWR_TYPE_C_CC1)
      {
        PWR_GPIO_On(GPIO_VCONN_EN1);
        ret = BSP_ERROR_NONE;
      }
      else
      {
        if (CCPinId == USBPD_PWR_TYPE_C_CC2)
        {
          PWR_GPIO_On(GPIO_VCONN_EN2);
          ret = BSP_ERROR_NONE;
        }
      }
    }
  }
  return ret;
  /* USER CODE END BSP_USBPD_PWR_VCONNOn */
}

/**
  * @brief  Disable VCONN sourcing.
  * @param  Instance Type-C port identifier
  *         This parameter can be take one of the following values:
  *         @arg @ref USBPD_PWR_TYPE_C_PORT_1
  *         @arg @ref USBPD_PWR_TYPE_C_PORT_2
  * @param  CCPinId CC pin identifier
  *         This parameter can be take one of the following values:
  *         @arg @ref USBPD_PWR_TYPE_C_CC1
  *         @arg @ref USBPD_PWR_TYPE_C_CC2
  * @retval BSP status
  */
int32_t BSP_USBPD_PWR_VCONNOff(uint32_t Instance,
                               uint32_t CCPinId)
{
  /* USER CODE BEGIN BSP_USBPD_PWR_VCONNOff */
  /* Check if instance is valid       */
  int32_t ret;

  if (Instance >= USBPD_PWR_INSTANCES_NBR)
  {
    ret = BSP_ERROR_WRONG_PARAM;
  }
  else
  {
    ret = BSP_ERROR_PERIPH_FAILURE;

    if (Instance == USBPD_PWR_TYPE_C_PORT_1)
    {
      if (CCPinId == USBPD_PWR_TYPE_C_CC1)
      {
        /* switch off Vconn */
        PWR_GPIO_Off(GPIO_VCONN_EN1);
        PWR_GPIO_On(GPIO_VCONN_EN1);
        PWR_GPIO_Off(GPIO_VCONN_EN1);

        /* Dicharge VCONN */
        PWR_GPIO_On(GPIO_VCONN_DISCHARGE1);
        HAL_Delay(1u);
        PWR_GPIO_Off(GPIO_VCONN_DISCHARGE1);
        ret = BSP_ERROR_NONE;
      }
      else
      {
        if (CCPinId == USBPD_PWR_TYPE_C_CC2)
        {
          /* switch off Vconn */
          PWR_GPIO_Off(GPIO_VCONN_EN2);
          PWR_GPIO_On(GPIO_VCONN_EN2);
          PWR_GPIO_Off(GPIO_VCONN_EN2);

          /* Dicharge VCONN */
          PWR_GPIO_On(GPIO_VCONN_DISCHARGE2);
          HAL_Delay(1u);
          PWR_GPIO_Off(GPIO_VCONN_DISCHARGE2);
          ret = BSP_ERROR_NONE;
        }
      }
    }
  }
  return ret;
  /* USER CODE END BSP_USBPD_PWR_VCONNOff */
}

/**
  * @brief  Get actual VCONN status.
  * @param  Instance Type-C port identifier
  *         This parameter can be take one of the following values:
  *         @arg @ref USBPD_PWR_TYPE_C_PORT_1
  * @param  CCPinId Type-C CC pin identifier
  *         This parameter can be take one of the following values:
  *         @arg @ref USBPD_PWR_TYPE_C_CC1
  *         @arg @ref USBPD_PWR_TYPE_C_CC2
  * @param  pState VCONN status (1: On, 0: Off)
  * @retval BSP status
  */
int32_t BSP_USBPD_PWR_VCONNIsOn(uint32_t Instance,
                                uint32_t CCPinId, uint8_t *pState)
{
  /* USER CODE BEGIN BSP_USBPD_PWR_VCONNIsOn */
  /* Check if instance is valid       */
  int32_t ret = BSP_ERROR_NONE;

  if ((Instance >= USBPD_PWR_INSTANCES_NBR) || (NULL == pState))
  {
    ret = BSP_ERROR_WRONG_PARAM;
  }
  else
  {
    GPIO_Id_TypeDef gpio;

    if (Instance == USBPD_PWR_TYPE_C_PORT_1)
    {
      if (CCPinId == USBPD_PWR_TYPE_C_CC1)
      {
        gpio = GPIO_VCONN_EN1;
      }
      else
      {
        gpio = GPIO_VCONN_EN2;
      }
      ((READ_BIT(GPIO_PORT[gpio]->ODR, GPIO_PIN[gpio]) == GPIO_PIN[gpio]) ? (*pState = 1u) : (*pState = 0u));
    }
    else
    {
      ret = BSP_ERROR_FEATURE_NOT_SUPPORTED;;
    }
  }
  return ret;
  /* USER CODE END BSP_USBPD_PWR_VCONNIsOn */
}

/**
  * @brief  Set the VBUS disconnection voltage threshold.
  * @note   Callback funtion registered through BSP_USBPD_PWR_RegisterVBUSDetectCallback
  *         function call is invoked when VBUS falls below programmed threshold.
  * @note   By default VBUS disconnection threshold is set to 3.3V
  * @param  Instance Type-C port identifier
  *         This parameter can be take one of the following values:
  *         @arg @ref USBPD_PWR_TYPE_C_PORT_1
  *         @arg @ref USBPD_PWR_TYPE_C_PORT_2
  * @param  VoltageThreshold VBUS disconnection voltage threshold (in mV)
  * @retval BSP status
  */
int32_t BSP_USBPD_PWR_SetVBUSDisconnectionThreshold(uint32_t Instance,
                                                    uint32_t VoltageThreshold)
{
  /* USER CODE BEGIN BSP_USBPD_PWR_SetVBUSDisconnectionThreshold */
  /* Check if instance is valid       */
  int32_t ret = BSP_ERROR_NONE;

  if (Instance >= USBPD_PWR_INSTANCES_NBR)
  {
    ret = BSP_ERROR_WRONG_PARAM;
  }
  else
  {
    /* Update PWR context */
    if (VoltageThreshold <= VBUS_CONNECTION_THRESHOLD)
    {
      Context.PortInfo[Instance].VBUSDisconnectionThreshold = VoltageThreshold;
    }
  }
  return ret;
  /* USER CODE END BSP_USBPD_PWR_SetVBUSDisconnectionThreshold */
}

/**
  * @brief  Register USB Type-C Current callback function.
  * @note   Callback function invoked when VBUS rises above 4V (VBUS present) or
  *         when VBUS falls below programmed threshold (VBUS absent).
  * @note   Callback funtion is un-registered when callback function pointer
  *         argument is NULL.
  * @param  Instance Type-C port identifier
  *         This parameter can be take one of the following values:
  *         @arg @ref USBPD_PWR_TYPE_C_PORT_1
  *         @arg @ref USBPD_PWR_TYPE_C_PORT_2
  * @param  pfnVBUSDetectCallback callback function pointer
  * @retval BSP status
  */
int32_t BSP_USBPD_PWR_RegisterVBUSDetectCallback(uint32_t                       Instance,
                                                 USBPD_PWR_VBUSDetectCallbackFunc    *pfnVBUSDetectCallback)
{
  /* USER CODE BEGIN BSP_USBPD_PWR_RegisterVBUSDetectCallback */
  /* Check if instance is valid       */
  int32_t ret = BSP_ERROR_NONE;

  if ((Instance >= USBPD_PWR_INSTANCES_NBR) || (NULL == pfnVBUSDetectCallback))
  {
    ret = BSP_ERROR_WRONG_PARAM;
  }
  else
  {
    ADC_AnalogWDGConfTypeDef adc_awd_config;

    /* Configure analog watchdow */
    if (Instance == USBPD_PWR_TYPE_C_PORT_1)
    {
      adc_awd_config.WatchdogNumber = ADC_ANALOGWATCHDOG_1;
      adc_awd_config.Channel        = ADCx_CHANNEL_VSENSE_1;
    }
    else 
    { 
      adc_awd_config.WatchdogNumber = ADC_ANALOGWATCHDOG_2;
      adc_awd_config.Channel        = ADCx_CHANNEL_VSENSE_2;
    }
    /* Update PWR context */
    Context.PortInfo[Instance].pfnVBUSDetectCallback = pfnVBUSDetectCallback;

    if (pfnVBUSDetectCallback == NULL)
    {
      adc_awd_config.WatchdogMode  = ADC_ANALOGWATCHDOG_NONE;
      adc_awd_config.ITMode        = DISABLE;
      adc_awd_config.HighThreshold = 0xFFF;
      adc_awd_config.LowThreshold  = 0u;
    }
    else
    {
      adc_awd_config.WatchdogMode  = ADC_ANALOGWATCHDOG_SINGLE_REG;
      adc_awd_config.ITMode        = ENABLE;
      adc_awd_config.HighThreshold = PWR_ConvertVoltageToThreshold(VBUS_CONNECTION_THRESHOLD);
      adc_awd_config.LowThreshold  = 0u;
    }

    /* Stop on-going conversions */
    if (HAL_ADC_Stop_DMA(&hadc1) != HAL_OK)
    {
      ret = BSP_ERROR_PERIPH_FAILURE;
    }
    else
    {
      /* Set AWD configuration */
      if (HAL_ADC_AnalogWDGConfig(&hadc1, &adc_awd_config) != HAL_OK)
      {
        ret = BSP_ERROR_PERIPH_FAILURE;
      }
      else
      {
        /* Resume conversions */
        if (HAL_ADC_Start_DMA(&hadc1, (uint32_t *)aADCxConvertedValues, 4u) != HAL_OK)
        {
          ret = BSP_ERROR_PERIPH_FAILURE;
        }
      }
    }
  }
  return ret;
  /* USER CODE END BSP_USBPD_PWR_RegisterVBUSDetectCallback */
}

/**
  * @brief  Get actual VBUS status.
  * @param  Instance Type-C port identifier
  *         This parameter can be take one of the following values:
  *         @arg @ref USBPD_PWR_TYPE_C_PORT_1
  * @param  pState VBUS status (1: On, 0: Off)
  * @retval BSP status
  */
int32_t BSP_USBPD_PWR_VBUSIsOn(uint32_t Instance, uint8_t *pState)
{
  /* USER CODE BEGIN BSP_USBPD_PWR_VBUSIsOn */
  /* Check if instance is valid       */
  int32_t ret = BSP_ERROR_NONE;

  if (Instance >= USBPD_PWR_INSTANCES_NBR)
  {
    ret = BSP_ERROR_WRONG_PARAM;
  }
  else
  {
    if (Instance == USBPD_PWR_TYPE_C_PORT_1)
    {
      *pState = ((READ_BIT(GPIO_PORT[GPIO_SOURCE_EN]->ODR, GPIO_PIN[GPIO_SOURCE_EN]) == GPIO_PIN[GPIO_SOURCE_EN]) ? 1u : 0u);
    }
    else
    {
      ret = BSP_ERROR_FEATURE_NOT_SUPPORTED;;
    }
  }
  return ret;
  /* USER CODE END BSP_USBPD_PWR_VBUSIsOn */
}

/**
  * @brief  Get DCDC control mode.
  * @param  Instance Type-C port identifier
  *         This parameter can be take one of the following values:
  *         @arg @ref USBPD_PWR_TYPE_C_PORT_1
  * @param  pDCDCCtrl Pointer on DCDC control mode
  *         Returned value can take one of the following values:
  *         @arg @ref DCDC_CTRL_MODE_UNKNOWN
  *         @arg @ref DCDC_CTRL_MODE_GPIO
  *         @arg @ref DCDC_CTRL_MODE_PWM
  * @retval BSP status
  */
int32_t BSP_USBPD_PWR_DCDCGetCtrlMode(uint32_t Instance, USBPD_PWR_DCDCCtrlModeTypeDef *pDCDCCtrl)
{
  /* USER CODE BEGIN BSP_USBPD_PWR_DCDCGetCtrlMode */
  /* Check if instance is valid       */
  int32_t ret = BSP_ERROR_NONE;

  if ((Instance >= USBPD_PWR_INSTANCES_NBR) || (NULL == pDCDCCtrl))
  {
    ret = BSP_ERROR_WRONG_PARAM;
  }
  else
  {
    if (Instance == USBPD_PWR_TYPE_C_PORT_1)
    {
      *pDCDCCtrl = Context.DCDCCtrlMode;
    }
  }
  return ret;
  /* USER CODE END BSP_USBPD_PWR_DCDCGetCtrlMode */
}

/**
  * @}
  */

/** @addtogroup STM32G081B_EVAL_POWER_Private_Functions
  * @{
  */

/* USER CODE BEGIN POWER_Private_Functions */
/**
 * @brief  Initialize the hardware resources used by a port acting as a source.
 * @param  Instance Type-C port identifier
 *         This parameter can be take one of the following values:
 *         @arg TYPE_C_PORT_1
* @retval 0 success else fail
 */
static uint8_t PWR_InitSource(uint32_t Instance)
{
  uint8_t ret = 0;

  if (Instance == TYPE_C_PORT_1)
  {
    /* Initialize the Type-C port 1 related GPIOs */
    PWR_GPIO_Init(GPIO_VBUS_DISCHARGE1);
    PWR_GPIO_Init(GPIO_SOURCE_EN);
    ret = PWR_InitPowerSource();
    PWR_GPIO_Init(GPIO_V_CTL2);

    if (ret == 0)
    {
      /* Pre-charge C57 to avoid a high voltage overshoot on VSOURCE when DCDC is enabled. */
      PWR_DCDCPreChargeC57(TYPE_C_PORT_1);

      /* Determine DCDC voltage control mode (GPIO or PWM) */
      Context.DCDCCtrlMode = DCDC_CTRL_MODE_PWM;
    }
    return ret;
  }
  else
  {
    return 1;
  }
}

/**
 * @brief  Release the hardware resources used by a port acting as a source.
 * @param  Instance Type-C port identifier
 *         This parameter can take following value:
 *         @arg TYPE_C_PORT_1
* @retval 0 success else fail
 */
static uint8_t PWR_DeInitSource(uint32_t Instance)
{
  if (Instance == TYPE_C_PORT_1)
  {
    /* De-initialize the Type-C port 1 related GPIOs */
    PWR_DeInitPowerSource();
    PWR_GPIO_DeInit(GPIO_V_CTL2);
    PWR_GPIO_DeInit(GPIO_SOURCE_EN);
    PWR_GPIO_DeInit(GPIO_VBUS_DISCHARGE1);

    return 0;
  }
  else
  {
    return 1;
  }
}

/**
 * @brief  Initialize the hardware resources used by a port acting as a sink.
 * @retval 0 success else fail
 */
static uint8_t PWR_InitSink(void)
{
  uint32_t ret = 0;

  ret += PWR_ADC_SetConfig();

  Context.ADCRefCount++;

  return (ret == 0) ? PWR_OK : PWR_ERROR;
}

/**
 * @brief  Release the hardware resources used by a port acting as a sink.
 * @param  Instance Type-C port identifier
 *         This parameter can be take one of the following values:
 *         @arg TYPE_C_PORT_1
 *         @arg TYPE_C_PORT_2
* @retval 0 success else fail
 */
static uint8_t PWR_DeInitSink(uint32_t Instance)
{
  uint32_t ret = 0;

  Context.ADCRefCount--;

  PWR_ADC_ResetConfig();

  return (ret == 0) ? PWR_OK : PWR_ERROR;
}


/**
* @brief  Configures GPIO.
* @param  gpio Specifies the GPIO Pin to be configured.
*   This parameter can be one of following parameters:
*     @arg @ref   GPIO_SOURCE_EN
*     @arg @ref   GPIO_VBUS_DISCHARGE1
*     @arg @ref   GPIO_VBUS_DISCHARGE2
*     @arg @ref   GPIO_VCONN_EN1
*     @arg @ref   GPIO_VCONN_EN2
*     @arg @ref   GPIO_VCONN_DISCHARGE1
*     @arg @ref   GPIO_VCONN_DISCHARGE2
*     @arg @ref   GPIO_V_CTL2
*     @arg @ref   GPIO_V_CTL1
* @retval None
*/
static void PWR_GPIO_Init(GPIO_Id_TypeDef gpio)
{
  GPIO_InitTypeDef  GPIO_InitStruct;

  /* Enable the GPIO clock */
  GPIOx_CLK_ENABLE(gpio);

  /* Configure the GPIO pin */
  switch (gpio)
  {
  case GPIO_SOURCE_EN:
  case GPIO_VBUS_DISCHARGE1:
  case GPIO_VBUS_DISCHARGE2:
    GPIO_InitStruct.Pin   = GPIO_PIN[gpio];
    GPIO_InitStruct.Mode  = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull  = GPIO_PULLDOWN;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;

    HAL_GPIO_Init(GPIO_PORT[gpio], &GPIO_InitStruct);

    /* Set GPIO to inactive state */
    HAL_GPIO_WritePin(GPIO_PORT[gpio], GPIO_PIN[gpio], GPIO_PIN_RESET);
    break;

  case GPIO_VCONN_EN1:
  case GPIO_VCONN_EN2:
  case GPIO_V_CTL2:
  case GPIO_V_CTL1:
    GPIO_InitStruct.Pin   = GPIO_PIN[gpio];
    GPIO_InitStruct.Mode  = GPIO_MODE_OUTPUT_OD;
    GPIO_InitStruct.Pull  = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;

    HAL_GPIO_Init(GPIO_PORT[gpio], &GPIO_InitStruct);

    if(gpio == GPIO_V_CTL2)
    {
      /* Disable DCDC */
      HAL_GPIO_WritePin(GPIO_PORT[gpio], GPIO_PIN[gpio], GPIO_PIN_RESET);
    }
    else
    {
      /* Set GPIO to inactive state (High level is HI-Z) */
      HAL_GPIO_WritePin(GPIO_PORT[gpio], GPIO_PIN[gpio], GPIO_PIN_SET);
    }
    break;

  case GPIO_VCONN_DISCHARGE1:
  case GPIO_VCONN_DISCHARGE2:
    GPIO_InitStruct.Pin   = GPIO_PIN[gpio];
    GPIO_InitStruct.Mode  = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull  = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;

    HAL_GPIO_Init(GPIO_PORT[gpio], &GPIO_InitStruct);

    /* Set GPIO to inactive state */
    HAL_GPIO_WritePin(GPIO_PORT[gpio], GPIO_PIN[gpio], GPIO_PIN_RESET);
    break;
  default:
    break;
  }
}

/**
* @brief  Reset  GPIO configuration.
* @param  gpio Specifies the GPIO Pin .
*   This parameter can be one of following parameters:
  *     @arg @ref   GPIO_SOURCE_EN
  *     @arg @ref   GPIO_VBUS_DISCHARGE1
  *     @arg @ref   GPIO_VBUS_DISCHARGE2
  *     @arg @ref   GPIO_VCONN_EN1
  *     @arg @ref   GPIO_VCONN_EN2
  *     @arg @ref   GPIO_VCONN_DISCHARGE1
  *     @arg @ref   GPIO_VCONN_DISCHARGE2
  *     @arg @ref   GPIO_V_CTL2
  *     @arg @ref   GPIO_V_CTL1
* @retval None
*/
static void PWR_GPIO_DeInit(GPIO_Id_TypeDef gpio)
{
  HAL_GPIO_DeInit(GPIO_PORT[gpio], GPIO_PIN[gpio]);
}

/**
* @brief  Turns selected GPIO On.
* @param  gpio Specifies the GPIO Pin to be configured.
*   This parameter can be one of following parameters:
*     @arg @ref   GPIO_SOURCE_EN
*     @arg @ref   GPIO_VBUS_DISCHARGE1
*     @arg @ref   GPIO_VBUS_DISCHARGE2
*     @arg @ref   GPIO_VCONN_EN1
*     @arg @ref   GPIO_VCONN_EN2
*     @arg @ref   GPIO_VCONN_DISCHARGE1
*     @arg @ref   GPIO_VCONN_DISCHARGE2
*     @arg @ref   GPIO_V_CTL2
*     @arg @ref   GPIO_V_CTL1
* @retval None
*/
static void PWR_GPIO_On(GPIO_Id_TypeDef gpio)
{
  /* Update GPIO pin state */
  if ((gpio == GPIO_VCONN_EN1)   ||
      (gpio == GPIO_VCONN_EN2)   ||
      (gpio == GPIO_V_CTL1) ||
      (gpio == GPIO_V_CTL2))
{
    if (gpio == GPIO_V_CTL2)
    {
      GPIO_InitTypeDef  GPIO_InitStruct;

      PWR_GPIO_DeInit(GPIO_V_CTL2);

      GPIO_InitStruct.Pin   = GPIO_PIN[gpio];
      GPIO_InitStruct.Mode  = GPIO_MODE_OUTPUT_PP;
      GPIO_InitStruct.Pull  = GPIO_NOPULL;
      GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
      HAL_GPIO_Init(GPIO_PORT[gpio], &GPIO_InitStruct);
      HAL_GPIO_WritePin(GPIO_PORT[gpio], GPIO_PIN[gpio], GPIO_PIN_SET);
    }
    else
  {
    /* In open-drain mode only the low level is driven (high level is HI-Z) */
      HAL_GPIO_WritePin(GPIO_PORT[gpio], GPIO_PIN[gpio], GPIO_PIN_RESET);
    }
  }
  else
  {
    HAL_GPIO_WritePin(GPIO_PORT[gpio], GPIO_PIN[gpio], GPIO_PIN_SET);
  }
}

/**
* @brief  Turns selected GPIO Off.
* @param  gpio Specifies the GPIO Pin to be configured.
*   This parameter can be one of following parameters:
*     @arg @ref   GPIO_SOURCE_EN
*     @arg @ref   GPIO_VBUS_DISCHARGE1
*     @arg @ref   GPIO_VBUS_DISCHARGE2
*     @arg @ref   GPIO_VCONN_EN1
*     @arg @ref   GPIO_VCONN_EN2
*     @arg @ref   GPIO_VCONN_DISCHARGE1
*     @arg @ref   GPIO_VCONN_DISCHARGE2
*     @arg @ref   GPIO_V_CTL2
*     @arg @ref   GPIO_V_CTL1
* @retval None
*/
static void PWR_GPIO_Off(GPIO_Id_TypeDef gpio)
{
  /* Update GPIO pin state */
  if ((gpio == GPIO_VCONN_EN1)   ||
      (gpio == GPIO_VCONN_EN2)   ||
      (gpio == GPIO_V_CTL1) ||
      (gpio == GPIO_V_CTL2))
{
  if (gpio == GPIO_V_CTL2)
  {
      GPIO_InitTypeDef  GPIO_InitStruct;

      PWR_GPIO_DeInit(GPIO_V_CTL2);

      GPIO_InitStruct.Pin   = GPIO_PIN[gpio];
      GPIO_InitStruct.Mode  = GPIO_MODE_OUTPUT_OD;
      GPIO_InitStruct.Pull  = GPIO_NOPULL;
      GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
      HAL_GPIO_Init(GPIO_PORT[gpio], &GPIO_InitStruct);
      HAL_GPIO_WritePin(GPIO_PORT[gpio], GPIO_PIN[gpio], GPIO_PIN_RESET);
  }
  else
  {
      /* Set GPIO to inactive state (High level is HI-Z) */
      HAL_GPIO_WritePin(GPIO_PORT[gpio], GPIO_PIN[gpio], GPIO_PIN_SET);
    }
  }
  else
  {
    HAL_GPIO_WritePin(GPIO_PORT[gpio], GPIO_PIN[gpio], GPIO_PIN_RESET);
  }
}

/**
* @brief  Set ADC configuration.
* @retval 0 success else fail
*/
static uint8_t PWR_ADC_SetConfig(void)
{
  uint8_t ret = 0u;

  /* Initialize ADC (if not already used) */
  if (Context.ADCRefCount == 0u)
  {
    __HAL_ADC_RESET_HANDLE_STATE(&hadc1);

    /* Configuration of hadc1 init structure: ADC parameters and regular group */
    hadc1.Instance = ADCx;

    hadc1.Init.ClockPrescaler        = ADC_CLOCK_SYNC_PCLK_DIV4;
    hadc1.Init.Resolution            = ADC_RESOLUTION_12B;
    hadc1.Init.DataAlign             = ADC_DATAALIGN_RIGHT;
    hadc1.Init.ScanConvMode          = ADC_SCAN_DIRECTION_FORWARD;
    hadc1.Init.EOCSelection          = ADC_EOC_SINGLE_CONV;
    hadc1.Init.LowPowerAutoWait      = DISABLE;
    hadc1.Init.LowPowerAutoPowerOff  = DISABLE;
    hadc1.Init.ContinuousConvMode    = ENABLE;
    hadc1.Init.DiscontinuousConvMode = DISABLE;
    hadc1.Init.ExternalTrigConv      = ADC_SOFTWARE_START;
    hadc1.Init.ExternalTrigConvEdge  = ADC_EXTERNALTRIGCONVEDGE_NONE;
    hadc1.Init.DMAContinuousRequests = ENABLE;
    hadc1.Init.Overrun               = ADC_OVR_DATA_OVERWRITTEN;
    hadc1.Init.SamplingTimeCommon1   = ADC_SAMPLETIME_39CYCLES_5;
    hadc1.Init.OversamplingMode      = DISABLE;

    ADCx_MspInit(&hadc1);
    if (HAL_ADC_Init(&hadc1) != HAL_OK) ret++;

    /* Run the ADC calibration */
    if (HAL_ADCEx_Calibration_Start(&hadc1) != HAL_OK) ret++;

    /* Configure the ADC channel used to sense VBUS1 voltage */
    ret += PWR_ADC_SetChannelConfig(ADC_VSENSE_1);

    /* Configure the ADC channel used to sense VBUS1 current */
    ret += PWR_ADC_SetChannelConfig(ADC_ISENSE_1);

    /* Configure the ADC channel used to sense VBUS2 voltage */
    ret += PWR_ADC_SetChannelConfig(ADC_VSENSE_2);

    /* Configure the ADC channel used to sense VBUS2 current */
    ret += PWR_ADC_SetChannelConfig(ADC_ISENSE_2);
  }

  return ret;
}

/**
* @brief  Reset ADC configuration.
* @retval None
*/
static void PWR_ADC_ResetConfig(void)
{
  /* Stop conversion and de-initialize ADC (if not used any more ) */
  if (Context.ADCRefCount == 0u)
  {
    /* Stop ADCx conversions */
    (void)HAL_ADC_Stop_DMA(&hadc1);

    /* Reset ADC channel configuration */
    PWR_ADC_ResetChannelConfig(ADC_VSENSE_1);
    PWR_ADC_ResetChannelConfig(ADC_ISENSE_1);
    PWR_ADC_ResetChannelConfig(ADC_VSENSE_2);
    PWR_ADC_ResetChannelConfig(ADC_ISENSE_2);

    /* De-initialize ADCx */
    ADCx_MspDeInit(&hadc1);
    HAL_ADC_DeInit(&hadc1);
  }
}

/**
* @brief  Set ADC channel configuration.
* @param  ADCChannel Specifies the ADC channel
*     @arg @ref   ADC_VSENSE_1
*     @arg @ref   ADC_VSENSE_2
*     @arg @ref   ADC_ISENSE_1
*     @arg @ref   ADC_ISENSE_2
*     @arg @ref   ADC_VSENSE_DCDC (MB1352C only)
* @retval 0 success else fail
*/
static uint8_t PWR_ADC_SetChannelConfig(ADC_ChannelId_TypeDef ADCChannel)
{
  /* V/I Sense / ADC Channel cross table */
  static const uint32_t ADC_CHANNEL[ADCxChanneln] =
  {
    ADCx_CHANNEL_VSENSE_1,
    ADCx_CHANNEL_ISENSE_1,
    ADCx_CHANNEL_VSENSE_2,
    ADCx_CHANNEL_ISENSE_2,
    ADCx_CHANNEL_VSENSE_DCDC
  };

  uint8_t                  ret = 0u;
  GPIO_InitTypeDef         gpio_config = {0u};
  ADC_ChannelConfTypeDef   adc_channel_config = {0u};

  /* Enable GPIO clock */
  ADCx_CHANNEL_GPIO_CLK_ENABLE(ADC_CHANNEL[ADCChannel]);

  /* Configure GPIO pin of the selected ADC channel */
  gpio_config.Pin = ADC_CHANNEL_PIN[ADCChannel];
  gpio_config.Mode = GPIO_MODE_ANALOG;
  gpio_config.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(ADC_CHANNEL_PORT[ADCChannel], &gpio_config);

  /* Configure the ADC channel */
  adc_channel_config.Channel = ADC_CHANNEL[ADCChannel];
  adc_channel_config.Rank    = ADC_RANK_CHANNEL_NUMBER;
  adc_channel_config.SamplingTime = ADC_SAMPLINGTIME_COMMON_1;

  if (HAL_ADC_ConfigChannel(&hadc1, &adc_channel_config) != HAL_OK)
  {
    ret++;
  }

  return ret;
}

/**
* @brief  Reset ADC channel configuration.
* @param  ADCChannel Specifies the ADC channel
*     @arg @ref   ADC_VSENSE_1
*     @arg @ref   ADC_VSENSE_2
*     @arg @ref   ADC_ISENSE_1
*     @arg @ref   ADC_ISENSE_2
*     @arg @ref   ADC_VSENSE_DCDC (MB1352C only)
* @retval None
*/
static void PWR_ADC_ResetChannelConfig(ADC_ChannelId_TypeDef ADCChannel)
{
  /* Reset GPIO configuration */
  HAL_GPIO_DeInit(ADC_CHANNEL_PORT[ADCChannel], ADC_CHANNEL_PIN[ADCChannel]);
}

/**
  * @brief  Calculate the VBUS voltage level corresponding to ADC raw converted data.
  * @note   Voltage level is measured though a voltage divider
  *
  *  Voltage -- Ra ----.-   ^
  *                    |    |
  *                    Rb   | vadc = VBUS * Rb /(Ra + Rb)
  *                    |    |
  *                   GND   |
  * vadc = raw_data * (ADC_FULL_SCALE / VDD)
  * Ra = 330 kOhms
  * Rb = 49.9 kOhms
  * @param  ADCData  ADC raw converted data (resolution 12 bits)
  * @retval analog voltage (unit: mV)
  */
static uint32_t PWR_ConvertADCDataToVoltage(uint32_t ADCData)
{
  uint32_t voltage;
  uint32_t vadc;

  vadc = (ADCData * VDD)/ADC_FULL_SCALE;

  voltage = vadc * (RA + RB) / RB;

  return voltage;
}

/**
  * @brief  Calculate the analog watchdog threshold corresponding to a given VBUS voltage level.
  * @note   Voltage level is measured though a voltage divider
  *
  *  VBUS ----- Ra ----.-   ^
  *                    |    |
  *                    Rb   | vadc = VBUS * Rb /(Ra + Rb)
  *                    |    |
  *                   GND   |
  * vadc = Threshold * (ADC_FULL_SCALE / VDD)
  * Ra = 330 kOhms
  * Rb = 49.9 kOhms
  * @param  Voltage  VBUS analog voltage (unit: mV)
  * @retval analog watchdog threshold value
  */
static uint32_t PWR_ConvertVoltageToThreshold(uint32_t Voltage)
{
  uint32_t threshold;
  uint32_t vadc;

  vadc = Voltage * RB/(RA + RB);

  threshold = vadc * ADC_FULL_SCALE / VDD;

  return threshold;
}

/**
  * @brief  Calculate the VBUS current level corresponding to ADC raw converted data.
  * @note   Current level is measured though INA199x1 output voltage level.
  * @note   INA199x1 measures currents through a resistive shunt (0.005 Ohms) in
  *         two directions.
  * @note   VDD corresponds to the zero-input level state. The Vout responds
  *         by increasing above VDD for positive differential signals (relative
  *         to the IN– pin) and responds by decreasing below VDD for negative
  *        differential signals.
  *
  *   VBUS -----. Rs .-....
  *             |    |  .------------.
  *             |    |  |  INA199x1  |
  *             |    |  |            |
  *             |    '--> IN+        > Vout (GAIN * Vs)
  *             |  Vs ^ |            | 0 <= Vout < VDD   -  negative current
  *             |     | |            | Vout = VDD        -  zero-input
  *             '-------> IN-        | Vout > VDD        -  positive current
  *                     |            |
  *              Vref   >            |
  *             (Vdd/2) |            |
  *                     '------------'
  *
  * Is = Vout/(GAIN * Rs) = (Vout - 2*Vref)/(Gain * Rs)
  *    where Vout = ADCData * (VDD/ADC_FULL_SCALE)
  * Is = (ADCData * Vdd)/(ADC_FULL_SCALE * Gain * Rs) - Vdd/(Gain * Rs)
  *   Vdd = 3300 mV, ADC_FULL_SCALE = 4095, Gain = 50, Rs = 5/1000
  * @param  ADCData  ADC raw converted data (resolution 12 bits)
  * @retval VBUS analog current (unit: mA)
  */
static int32_t PWR_ConvertADCDataToCurrent(uint32_t ADCData)
{
  int32_t current;

  current = ((ADCData * VDD) >> 10u) - 6600u;

  return current;
}

/**
   * @brief  Force the VBUS discharge.
   * @param  Instance Type-C port identifier
   *         This parameter can be take one of the following values:
   *         @arg TYPE_C_PORT_1
   * @param  VbusStopInmV Low VBUS threshold (in mV)
   * @param  VbusErrorInmV VBUS margin (in mV)
   * @param  Delay Discharge delay (in ms)
   * @retval None
   */

static void PWR_VBUSDischarge(uint32_t Instance,
                              uint32_t VbusStopInmV,
                              uint32_t VbusErrorInmV,
                              uint16_t Delay)
{
  uint32_t vbus;
  uint8_t vbus_disable = 0;

  if (Instance == TYPE_C_PORT_1)
  {

    PWR_GPIO_On(GPIO_VBUS_DISCHARGE1);
    /* Disable DCDC_EN bit*/
    PWR_GPIO_Off(GPIO_V_CTL2);

    /* Wait Tempo or Voltage level */
    if (Delay > 0u)
    {
      HAL_Delay(Delay);
    }
    else /* if (VbusErrorInmV > 0) */
    {

      do{
        HAL_Delay(1);
        vbus = BSP_PWR_VBUSGetVoltage(Instance);
        if ((0 == vbus_disable) && (vbus < 5000))
        {
          /* Switch off VBUS */
          PWR_GPIO_Off(GPIO_SOURCE_EN);
          vbus_disable = 1u;
        }
      }
      while (vbus >= (VbusStopInmV + VbusErrorInmV));
    }

    /* Enable DCDC_EN bit*/
    PWR_GPIO_On(GPIO_V_CTL2);
    PWR_GPIO_Off(GPIO_VBUS_DISCHARGE1);
    /* Allow to restart with correct duty cycle */
    PWR_SetPWMDutyCycle(aPWMDutyCycleLUT[VBUS_5_V]);
  }
}

/**
   * @brief  Set VBUS voltage level
   * @param  Instance Type-C port identifier
   *         This parameter can be take one of the following values:
   *         @arg TYPE_C_PORT_1
   *         @arg TYPE_C_PORT_2
   * @param  VBusInmV  VBUS voltage level setpoint(in mV)
   * @param  Precision Requested precision (in mV)
   * @note  Calibration and Precision paremeters aren't relevant when the VBUS
   *        voltage level isn't controlled by a PWM signal.
   * @note  When VBUS voltage level is controlled by a PWM signal, VBUS voltage is
   *        controlled by a PD (Proportional-Derivative) control loop.
   *        The PD control implementation is ilustrated by the block diagram below:
   *
   *                                              .-------------------.
   *                 .-------.              .------> PROPORTIONAL (Kp) >-.   .-------.
   *                 |  ADD  | pwm_error    |      '-------------------'  |  |  ADD  |  pwm  .---------------.
   *    VBusInmV --> +       >--------------|                             '--> +     >-------> PWM generator >-.
   *                 |       |              |     .------------------.       |       |       '---------------' |
   * vbus_measured .-> -     |              '----->  DERIVATIVE (Kd) >-------> +     |                         |
   *               | '-------'  pwm_error_previous ->                  |       '-------'                         |
   *               |                              '------------------'                                         |
   *               '-------------------------------------------------------------------------------------------'
   *
   *   pwm = Kp * pwm_error + (Kd * der)
   *
   * where:
   *  pwm_error is calulated from vbus_error
   *  Kp = Proptional Constant.
   *  Kd = Derivative Constant.
   *  der  = pwm_error - pwm_error_previous
  *  (der: differential error)
  */
static PWR_StatusTypeDef PWR_VBUSSetVoltage(uint32_t Instance,
                                                uint32_t VBusInmV,
                                                uint16_t Precision)
{
  static uint8_t aPWMDutyCalibrated[VBUS_LEVEL_NB] =
  {
    0x0,   /* 5V */
    0x0,   /* 9V */
    0x0,   /* 15V */
    0x0,   /* 18V */
    0x0    /* Will be used to store latest PPS request */
  };
  
  PWR_StatusTypeDef _retr = PWR_OK;
  static uint16_t          duty_cycle;
  uint8_t           duty_calibrated = 0;
  uint32_t          vbus_measured;
#if defined(_TRACE)
  char _str[20];
#endif
  
  if (Instance == TYPE_C_PORT_1)
  {
    duty_cycle =  PWR_GetPWMDutyCycle();
    switch (VBusInmV)
    {
    case VBUS_VOLTAGE_0V_IN_MV:
      {
        duty_cycle    = 0x10u;
        /* Set PWM duty cycle to its initial value */
        PWR_SetPWMDutyCycle(duty_cycle);
        return PWR_OK;
      }
      
    case VBUS_VOLTAGE_5V_IN_MV:
      duty_cycle      = aPWMDutyCycleLUT[VBUS_5_V];
      duty_calibrated = aPWMDutyCalibrated[VBUS_5_V];
      break;
    case VBUS_VOLTAGE_9V_IN_MV:
      duty_calibrated = aPWMDutyCalibrated[VBUS_9_V];
      if (1u == duty_calibrated )
      {
        duty_cycle = aPWMDutyCycleLUT[VBUS_9_V];
      }
      break;
    case VBUS_VOLTAGE_15V_IN_MV:
      duty_calibrated = aPWMDutyCalibrated[VBUS_15_V];
      if (1u == duty_calibrated )
      {
        duty_cycle = aPWMDutyCycleLUT[VBUS_15_V];
      }
      break;
    case VBUS_VOLTAGE_18V_IN_MV:
      duty_cycle      = aPWMDutyCycleLUT[VBUS_18_V];
      if (1u == duty_calibrated )
      {
        duty_cycle = aPWMDutyCycleLUT[VBUS_18_V];
      }
      break;
    default:
      duty_cycle    = aPWMDutyCycleLUT[VBUS_PPS_V];
      duty_calibrated = 0u;
      break;
    }
    
#if defined(_TRACE)
    sprintf(_str,"==%d::%d",duty_cycle,duty_calibrated);
    USBPD_TRACE_Add(USBPD_TRACE_DEBUG,0,0,(uint8_t*)_str, strlen(_str));
#endif
    
#if defined(DBG_LOG)
    _DbgLogIndex=0;
#endif /* DBG_LOG */
    
    if (duty_calibrated == 1u)
    {
      uint32_t _tickstart = HAL_GetTick();
      PWR_SetPWMDutyCycle(duty_cycle);
      
      /* Check if the voltage is at the expected level */
      do
      {
        vbus_measured  = BSP_PWR_VBUSGetVoltage(Instance);
        for(int32_t index=0; index < 2000; index++)
        {
          __DSB();
        };
#ifdef PWR_DEBUG
        sprintf(_str,"svcc:%ld:%d", vbus_measured, duty_cycle);
        USBPD_TRACE_Add(USBPD_TRACE_DEBUG, Instance, 0, (uint8_t*)_str, strlen(_str));
#endif
        /* timeout to avoid infinite loop in case of power default */
        if ((HAL_GetTick() - _tickstart) > 300u)
        {
          /* discard the VBUS and return an error */
          BSP_PWR_VBUSOff(Instance);
          return PWR_ERROR;
        }
      }
      while ((ABS(vbus_measured - VBusInmV)) > 200u);
    }
    else
    {
      int32_t           vbus_error;
      int32_t           vbus_previous;
      int32_t           pwm;
      int32_t           pwm_step_current;
      int32_t           pwm_step_previous;
      int32_t           pwm_error = 0u;
      int32_t           pwm_error_previous;
      uint8_t            nb_pll_loop;
      pwm_step_previous = 0u;
      pwm_step_current = duty_cycle;
      vbus_previous = vbus_measured = 0u;
      nb_pll_loop = 0u;
      
      do
      {
        pwm_step_previous = pwm_step_current;
        vbus_previous = vbus_measured;
        pwm_error_previous = pwm_error;
        pwm_error = 0;
        pwm = 0;
        
        PWR_SetPWMDutyCycle(duty_cycle);
        
        /* We can not avoid this DCDC takes time to lock .... */
        for(int32_t index=0; index < 7000; index++)
        {
          __DSB();
        };
        
        /* Measure actual VBUS voltage level */
        vbus_measured  = BSP_PWR_VBUSGetVoltage(Instance);
#if 0
        sprintf(_str,"sv:%d:%d", vbus_measured, duty_cycle);
        USBPD_TRACE_Add(USBPD_TRACE_DEBUG, Instance, 0, (uint8_t*)_str, strlen(_str));
#endif
        /* Protection to handle the case where power is not present */
        if(vbus_measured < 300u)
        {
          /* discard the VBUS and return an error */
          BSP_PWR_VBUSOff(Instance);
          return PWR_ERROR;
        }
        
        /* Calculate VBUS error = target - the measure */
        vbus_error = (int32_t)(VBusInmV) - (int32_t)(vbus_measured);
        
        /* Convert VBUS error into PWM error (pwm_error) */
        if ((ABS(vbus_error) >= BSP_PWR_DCDC_PRECISION))
        {
          /* Calculate PWM error */
          pwm_error = (1000 * duty_cycle) / vbus_measured;
          pwm_error *= vbus_error;
          pwm_error /= 1000;
          
          /* Calculate new PWM setpoint */
          pwm = pwm_error*(int32_t)BSP_PWR_DCDC_PLL_P_N;
          pwm /= (int32_t)BSP_PWR_DCDC_PLL_P_D;
          pwm += (((pwm_error)-(pwm_error_previous))*(int32_t)BSP_PWR_DCDC_PLL_D_N)/((int32_t)BSP_PWR_DCDC_PLL_D_D);
        }
        
        /* Set PWM setpoint to its min value (if min DCDC capabilities is reached) */
        if (pwm == 0)
        {
          if (vbus_error > 0)
          {
            pwm = (int32_t)BSP_PWR_DCDC_MIN_PWM_STEP;
          }
          else
          {
            pwm = -(int32_t)BSP_PWR_DCDC_MIN_PWM_STEP;
          }
        }
        /* Limit PWM setpoint to max value (if required) */
        else if (ABS(pwm) >= (uint32_t)BSP_PWR_DCDC_MAX_PWM_STEP)
        {
          if (pwm > 0)
          {
            pwm = (int32_t)BSP_PWR_DCDC_MAX_PWM_STEP;
          }
          else
          {
            pwm = -(int32_t)BSP_PWR_DCDC_MAX_PWM_STEP;
          }
        }
        
        /* Calculate duty cycle setpoint from PWM setpoint */
        if (pwm > 0)
        {
          if ((duty_cycle + (uint32_t)(pwm)) <= (uint32_t)BSP_PWR_DCDC_MAX_PWM_VALUE)
          {
            duty_cycle += (uint32_t)ABS(pwm);
          }
          else
          {
            /* Error case: Go back to 5V or 0V and break? */
          }
        }
        else
        {
          if ((duty_cycle - (uint32_t)(pwm)) > (uint32_t)BSP_PWR_DCDC_MIN_PWM_VALUE)
          {
            duty_cycle -= (uint32_t)ABS(pwm);
          }
        }
        
        pwm_step_current = duty_cycle;
        
#if defined(DBG_LOG)
        _Err[_DbgLogIndex]=vbus_error;
        _Pwm[_DbgLogIndex]=duty_cycle;
        _PwmE[_DbgLogIndex]=pwm;
        _DbgLogIndex++;
        if (_DbgLogIndex >= sizeof(_vbusout) / 4)
        {
          _DbgLogIndex=0;
        }
#endif /* DBG_LOG*/
        
        if (    (ABS((int16_t)pwm_step_current-(int16_t)pwm_step_previous) <= BSP_PWR_DCDC_MIN_PWM_STEP)
            && ((ABS(((int32_t)vbus_measured - (int32_t)vbus_previous))) <= BSP_PWR_DCDC_PRECISION)
              && (ABS(vbus_error) <= Precision))
          
        {
          nb_pll_loop++;
        }
        else
        {
          nb_pll_loop = 0u;
        }
      }
      while (nb_pll_loop < 1u);
      
      /* save the PWM value for the next settings */
      switch (VBusInmV)
      {
      case VBUS_VOLTAGE_0V_IN_MV:
        break;
      case VBUS_VOLTAGE_5V_IN_MV:
        {
          aPWMDutyCycleLUT[VBUS_5_V]  = duty_cycle;
          aPWMDutyCalibrated[VBUS_5_V] = 1u;
#if defined(_TRACE)
          sprintf(_str, "ca5V:%d", duty_cycle);
          USBPD_TRACE_Add(USBPD_TRACE_DEBUG, 0, 0, (uint8_t*)_str, strlen(_str));
#endif
          break;
        }
      case VBUS_VOLTAGE_9V_IN_MV:
        {
          aPWMDutyCycleLUT[VBUS_9_V]  = duty_cycle;
          aPWMDutyCalibrated[VBUS_9_V] = 1u;
#if defined(_TRACE)
          sprintf(_str, "ca9V:%d", duty_cycle);
          USBPD_TRACE_Add(USBPD_TRACE_DEBUG, 0, 0, (uint8_t*)_str, strlen(_str));
#endif
          break;
        }
      case VBUS_VOLTAGE_15V_IN_MV:
        {
          aPWMDutyCycleLUT[VBUS_15_V] = duty_cycle;
          aPWMDutyCalibrated[VBUS_15_V] = 1u;
#if defined(_TRACE)
          sprintf(_str, "ca15V:%d", duty_cycle);
          USBPD_TRACE_Add(USBPD_TRACE_DEBUG, 0, 0, (uint8_t*)_str, strlen(_str));
#endif
          break;
        }
      case VBUS_VOLTAGE_18V_IN_MV:
        {
          aPWMDutyCycleLUT[VBUS_18_V] = duty_cycle;
          aPWMDutyCalibrated[VBUS_18_V] = 1u;
#if defined(_TRACE)
          sprintf(_str, "ca18V:%d", duty_cycle);
          USBPD_TRACE_Add(USBPD_TRACE_DEBUG, 0, 0, (uint8_t*)_str, strlen(_str));
#endif
          break;
        }
      default:
        {
          aPWMDutyCycleLUT[VBUS_PPS_V] = duty_cycle;
#if defined(_TRACE)
          sprintf(_str, "ca1PPS:%d", duty_cycle);
          USBPD_TRACE_Add(USBPD_TRACE_DEBUG, 0, 0, (uint8_t*)_str, strlen(_str));
#endif        
          break;
        }
      }
    }
  }
  else
  {
    _retr = PWR_ERROR;
  }
  
  return _retr;
}

/**
 * @brief  Start I/V sense on both Type-C ports.
 * @retval 0 success else fail
 */
static uint8_t PWR_StartVBusSensing(void)
{
  uint8_t ret = 0u;

  /* Start ADCx conversions */
  if (HAL_ADC_Start_DMA(&hadc1, (uint32_t *)aADCxConvertedValues, 4u) != HAL_OK)
  {
    ret++;
  }

  return ret;
}

/**
 * @brief  Stop I/V sense on both Type-C ports.
 * @retval 0 success else fail
 */
static uint8_t PWR_StopVBusSensing(void)
{
  uint8_t ret = 0u;

  /* Stop ADCx conversions */
  if (HAL_ADC_Stop_DMA(&hadc1) != HAL_OK)
  {
    ret++;
  }

  VBUSSensingCompleted = 0u;

  return ret;
}

/**
 * @brief  Pause I/V sense on both Type-C ports.
 * @retval 0 success else fail
 */
static uint8_t PWR_PauseVBusSensing(void)
{
  uint8_t ret = 0u;

  /* Stop on-going conversions */
  if (HAL_ADC_Stop_DMA(&hadc1) != HAL_OK)
  {
    ret++;
  }

  /* Reset Channel selection register */
  hadc1.Instance->CHSELR &= ~(ADC_CHSELR_SQ_ALL);

  VBUSSensingCompleted = 0u;

  return ret;
}

/**
 * @brief  Resume I/V sense on both Type-C ports.
 * @retval 0 success else fail
 */
static uint8_t PWR_ResumeVBusSensing(void)
{
  uint8_t  ret = 0u;
  uint32_t tickstart;

  /* Reset Channel selection register */
  hadc1.Instance->CHSELR &= ~(ADC_CHSELR_SQ_ALL);

  /* Configure the ADC channel used to sense VBUS1 voltage */
  ret += PWR_ADC_SetChannelConfig(ADC_VSENSE_1);

  /* Configure the ADC channel used to sense VBUS1 current */
  ret += PWR_ADC_SetChannelConfig(ADC_ISENSE_1);

  /* Configure the ADC channel used to sense VBUS2 voltage */
  ret += PWR_ADC_SetChannelConfig(ADC_VSENSE_2);

  /* Configure the ADC channel used to sense VBUS2 current */
  ret += PWR_ADC_SetChannelConfig(ADC_ISENSE_2);

  /* Start ADCx conversions */
  if (HAL_ADC_Start_DMA(&hadc1, (uint32_t *)aADCxConvertedValues, 4u) != HAL_OK)
  {
    ret++;
  }

  /* Wait until 1st DMA conversion is completed */
  tickstart = HAL_GetTick();

  while (VBUSSensingCompleted == 0u)
  {
    if((HAL_GetTick() - tickstart) >= VBUS_SENSING_RESUME_TIMEOUT)
    {
      /* Exit loop */
      break;
    }
  }


  return ret;
}

/**
  * @brief ADC MSP initialization
  * @param hadc ADC handle
  * @retval None
  */
static void ADCx_MspInit(ADC_HandleTypeDef *hadc)
{
  /* Enable ADCx clock */
  ADCx_CLK_ENABLE();

  /* Enable clock of DMA associated to the peripheral */
  ADCx_DMA_CLK_ENABLE();

  /* Configure DMA parameters */
  hdma_adc1.Instance = ADCx_DMA;
  hdma_adc1.Init.Request             = DMA_REQUEST_ADC1;
  hdma_adc1.Init.Direction           = DMA_PERIPH_TO_MEMORY;
  hdma_adc1.Init.PeriphInc           = DMA_PINC_DISABLE;
  hdma_adc1.Init.MemInc              = DMA_MINC_ENABLE;
  hdma_adc1.Init.PeriphDataAlignment = DMA_PDATAALIGN_HALFWORD;
  hdma_adc1.Init.MemDataAlignment    = DMA_MDATAALIGN_HALFWORD;
  hdma_adc1.Init.Mode                = DMA_CIRCULAR;
  hdma_adc1.Init.Priority            = DMA_PRIORITY_HIGH;

  /* Initialize the DMA */
  (void)HAL_DMA_Init(&hdma_adc1);

  /* Associate the initialized DMA handle to the ADC handle */
  __HAL_LINKDMA(hadc, DMA_Handle, hdma_adc1);

  /* NVIC configuration for DMA interrupt (transfer completion or error) */
  HAL_NVIC_SetPriority(ADCx_DMA_IRQn, 1u, 0u);
  HAL_NVIC_EnableIRQ(ADCx_DMA_IRQn);

  /* NVIC configuration for ADCx interrupt */
  HAL_NVIC_SetPriority(ADC1_COMP_IRQn, 0u, 0u);
  HAL_NVIC_EnableIRQ(ADC1_COMP_IRQn);
}

/**
  * @brief ADC MSP de-initialization
  * @param hadc ADC handle
  * @retval None
  */
static void ADCx_MspDeInit(ADC_HandleTypeDef *hadc)
{
  /* Reset ADC peripheral */
  ADCx_FORCE_RESET();
  ADCx_RELEASE_RESET();

  /* Disable ADCx clock */
  ADCx_CLK_DISABLE();

  /* Disable the NVIC configuration for ADCx interrupt */
  HAL_NVIC_DisableIRQ(ADC1_COMP_IRQn);

  /* Disable NVIC configuration for DMA interrupt */
  HAL_NVIC_DisableIRQ(ADCx_DMA_IRQn);

  /* De-Initialize the DMA */
  HAL_DMA_DeInit(hadc->DMA_Handle);
}

/**
  * @brief  Analog watchdog 1 callback
  * @param hadc ADC handle
  * @retval None
  */
void HAL_ADC_LevelOutOfWindowCallback(ADC_HandleTypeDef* hadc)
{
  ADC_AnalogWDGConfTypeDef adc_awd_config = {0u};

  if (Context.PortInfo[TYPE_C_PORT_1].pfnVBUSDetectCallback != (USBPD_PWR_VBUSDetectCallbackFunc *)NULL)
  {
    if (Context.PortInfo[TYPE_C_PORT_1].VBUSConnectionStatus == VBUS_NOT_CONNECTED)
    {
      /* PORT1: VBUS_NOT_CONNECTED to VBUS_CONNECTED transition */
      Context.PortInfo[TYPE_C_PORT_1].VBUSConnectionStatus = VBUS_CONNECTED;
      adc_awd_config.HighThreshold = 0xFFF;
      adc_awd_config.LowThreshold  = PWR_ConvertVoltageToThreshold(Context.PortInfo[TYPE_C_PORT_1].VBUSDisconnectionThreshold);
    }
    else
    {
      /* PORT1: VBUS_CONNECTED to VBUS_NOT_CONNECTED transition */
      Context.PortInfo[TYPE_C_PORT_1].VBUSConnectionStatus = VBUS_NOT_CONNECTED;
      adc_awd_config.HighThreshold = PWR_ConvertVoltageToThreshold(VBUS_CONNECTION_THRESHOLD);
      adc_awd_config.LowThreshold  = 0u;
    }

    /* Update AWD Low/High thresholds to detect VBUS disconnection */
    adc_awd_config.WatchdogNumber = ADC_ANALOGWATCHDOG_1;
    adc_awd_config.WatchdogMode   = ADC_ANALOGWATCHDOG_SINGLE_REG;
    adc_awd_config.Channel        = ADCx_CHANNEL_VSENSE_1;
    adc_awd_config.ITMode         = ENABLE;

    /* Update AWD configuration */
    (void)HAL_ADC_AnalogWDGConfig(hadc, &adc_awd_config);

    /* Call registered callback function */
    Context.PortInfo[TYPE_C_PORT_1].pfnVBUSDetectCallback(TYPE_C_PORT_1,
                                                          Context.PortInfo[TYPE_C_PORT_1].VBUSConnectionStatus);
  }
}

/**
  * @brief  Analog watchdog 2 callback
  * @param hadc ADC handle
  * @retval None
  */
void HAL_ADCEx_LevelOutOfWindow2Callback(ADC_HandleTypeDef* hadc)
{
  ADC_AnalogWDGConfTypeDef adc_awd_config = {0u};

  if (Context.PortInfo[TYPE_C_PORT_2].pfnVBUSDetectCallback != (USBPD_PWR_VBUSDetectCallbackFunc *)NULL)
  {
    if (Context.PortInfo[TYPE_C_PORT_2].VBUSConnectionStatus == VBUS_NOT_CONNECTED)
    {
      /* PORT2: VBUS_NOT_CONNECTED to VBUS_CONNECTED transition */
      Context.PortInfo[TYPE_C_PORT_2].VBUSConnectionStatus = VBUS_CONNECTED;
      adc_awd_config.HighThreshold = 0xFFF;
      adc_awd_config.LowThreshold  = PWR_ConvertVoltageToThreshold(Context.PortInfo[TYPE_C_PORT_2].VBUSDisconnectionThreshold);
    }
    else
    {
      /* PORT2: VBUS_CONNECTED to VBUS_NOT_CONNECTED transition */
      Context.PortInfo[TYPE_C_PORT_2].VBUSConnectionStatus = VBUS_NOT_CONNECTED;
      adc_awd_config.HighThreshold = PWR_ConvertVoltageToThreshold(VBUS_CONNECTION_THRESHOLD);
      adc_awd_config.LowThreshold  = 0u;
    }

    /* Update AWD2 Low/High thresholds to detect next VBUS connection/disconnection */
    adc_awd_config.WatchdogNumber = ADC_ANALOGWATCHDOG_2;
    adc_awd_config.WatchdogMode   = ADC_ANALOGWATCHDOG_SINGLE_REG;
    adc_awd_config.Channel        = ADCx_CHANNEL_VSENSE_2;
    adc_awd_config.ITMode         = ENABLE;

    /* Update AWD2 configuration */
    (void)HAL_ADC_AnalogWDGConfig(hadc, &adc_awd_config);

    /* Call registered callback function */
    Context.PortInfo[TYPE_C_PORT_2].pfnVBUSDetectCallback(TYPE_C_PORT_2,
                                                          Context.PortInfo[TYPE_C_PORT_2].VBUSConnectionStatus);
  }
}

/**
  * @brief  Initialize the LPTIM MSP.
  * @param  hlptim LPTIM handle
  * @retval None
  */
static void LPTIMx_MspInit(LPTIM_HandleTypeDef *hlptim)
{
  RCC_PeriphCLKInitTypeDef clk_config = {0u};
  GPIO_InitTypeDef         gpio_config = {0u};

  /* Select the PCLK clock as LPTIM peripheral clock */
  clk_config.PeriphClockSelection = RCC_PERIPHCLK_LPTIM1;
  clk_config.Lptim1ClockSelection = RCC_LPTIM1CLKSOURCE_PCLK1;
  (void)HAL_RCCEx_PeriphCLKConfig(&clk_config);

  /* Enable LPTIM1 clock */
  __HAL_RCC_LPTIM1_CLK_ENABLE();

  /* Force & Release LPTIM1 Clock Reset  */
  __HAL_RCC_LPTIM1_FORCE_RESET();
  __HAL_RCC_LPTIM1_RELEASE_RESET();

  /* Enable GPIOC clock  */
  GPIO_V_CTL1_CLK_ENABLE();

  /* Configure PC1 (LPTIM1_IN1) in alternate function (AF0) */
  gpio_config.Pin       = GPIO_V_CTL1_PIN;
  gpio_config.Mode      = GPIO_MODE_AF_OD;
  gpio_config.Pull      = GPIO_NOPULL;
  gpio_config.Speed     = GPIO_SPEED_FREQ_HIGH;
  gpio_config.Alternate = GPIO_AF0_LPTIM1;
  HAL_GPIO_Init(GPIO_V_CTL1_PORT, &gpio_config);
}

/**
  * @brief  Initialize the LPTIM MSP.
  * @param  hlptim LPTIM handle
  * @retval None
  */
static void LPTIMx_MspDeInit(LPTIM_HandleTypeDef *hlptim)
{
  /* Reset PC1 configuration */
  HAL_GPIO_DeInit(GPIO_V_CTL1_PORT, GPIO_V_CTL1_PIN);

  /* Disable LPTIM1 clock */
  __HAL_RCC_LPTIM1_CLK_DISABLE();
}

/**
  * @brief  Conversion complete callback in non-blocking mode.
  * @param hadc ADC handle
  * @retval None
  */
void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef* hadc)
{
  UNUSED(hadc);
  VBUSSensingCompleted = 1u;
}

/**
  * @brief  Initialize VBUS voltage control (VBUS defaults at 5V)
  * @retval 0 success else fail
  */
static uint8_t PWR_InitPowerSource(void)
{
  uint8_t ret = 0u;

  /* LPTIM1 initialization */
  hlptim.Instance = LPTIM1;
  hlptim.Init.Clock.Source    = LPTIM_CLOCKSOURCE_APBCLOCK_LPOSC;
  hlptim.Init.CounterSource   = LPTIM_COUNTERSOURCE_INTERNAL;
  hlptim.Init.Clock.Prescaler = LPTIM_PRESCALER_DIV1;
  hlptim.Init.Trigger.Source  = LPTIM_TRIGSOURCE_SOFTWARE;
  hlptim.Init.OutputPolarity  = LPTIM_OUTPUTPOLARITY_HIGH;
  hlptim.Init.UpdateMode      = LPTIM_UPDATE_IMMEDIATE;

  LPTIMx_MspInit(&hlptim);
  if (HAL_LPTIM_Init(&hlptim) != HAL_OK) ret++;

  /* Start PWM signal generation (VBUS voltage defaults at 5V)*/
  /* Period = (F(APB1) / 20K) + 1 */
  if (HAL_LPTIM_PWM_Start(&hlptim, (BSP_PWR_DCDC_MAX_PWM_VALUE + 1), 0x10) != HAL_OK) ret++;

  return ret;
}

/**
  * @brief  De-initialize VBUS voltage control
  * @retval None
  */
static void PWR_DeInitPowerSource(void)
{
  HAL_LPTIM_PWM_Stop(&hlptim);
  LPTIMx_MspDeInit(&hlptim);
}

/**
  * @brief  Set the PWM signal duty cycle
  * @param  PWMDutyCycle PWM signal duty cycle
  * @retval None
  */
static void PWR_SetPWMDutyCycle(uint16_t PWMDutyCycle)
{
  __HAL_LPTIM_COMPARE_SET(&hlptim , PWMDutyCycle);
  LL_LPTIM_SetCompare(hlptim.Instance, PWMDutyCycle);
}

/**
  * @brief SGet the PWM signal duty cycle
  * @retval PWMDutyCycle PWM signal duty cycle
  */
static uint16_t PWR_GetPWMDutyCycle(void)
{
  return (uint16_t)READ_REG(hlptim.Instance->CMP);
}


/**
  * @brief  Precharge C57 .
  * @param  Instance Type-C port identifier
  *         This parameter can be take one of the following values:
  *         @arg TYPE_C_PORT_1
  * @retval Measured DCDC voltage
  */
static uint32_t PWR_DCDCPreChargeC57(uint32_t Instance)
{
  uint32_t voltage = 0;

  if (Instance == TYPE_C_PORT_1)
  {
    /* Make sure SOURCE_EN is not OFF */
    PWR_GPIO_Off(GPIO_SOURCE_EN);

    {
      GPIO_InitTypeDef  GPIO_InitStruct;
      GPIOx_CLK_ENABLE(GPIO_V_CTL1);
      GPIO_InitStruct.Pin   = GPIO_PIN[GPIO_V_CTL1];
      GPIO_InitStruct.Mode  = GPIO_MODE_OUTPUT_PP;
      GPIO_InitStruct.Pull  = GPIO_NOPULL;
      GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
      HAL_GPIO_Init(GPIO_PORT[GPIO_V_CTL1], &GPIO_InitStruct);
      HAL_GPIO_WritePin(GPIO_PORT[GPIO_V_CTL1], GPIO_PIN[GPIO_V_CTL1], GPIO_PIN_SET);
    }


    /* Step 1 ====================================================================
    Step 1: DCDC_EN detects its voltage to ensure PSU 19V exist
    Wait C57 pre-charge for 20ms... */
    HAL_Delay(20);
    PWR_GPIO_On(GPIO_V_CTL2); /* DCDC_EN = 1, so DCDC output is on */
    /* Wait stabilization time for 10ms... */
    HAL_Delay(10);
    voltage = BSP_PWR_DCDCGetVoltage(Instance);

    /* Step 2 ====================================================================
       Step 2: PWM_CTL provides about 0.8V level before DCDC_EN=1                  */

    /*  Equivalent 950mV PWM_CTL voltage (in Push Pull mode) should be provided,
    This leads to 800mV feedback voltage for TI 54540 (pin 5, feedback pin),
    equivalent 950mV should be Provided short time (I suggest 30ms) before and
    after  DCDC_EN=1, then PWM_CTL can be Configured in OD mode, and set
    PWM_CTL with proper duty cycle to get desired VSOURCE voltage (5V in general).
    */

    /* Step 3 ====================================================================
       Step 3: DCDC_EN=1, then PWM_CTL changes to provide PWM waveform by 0 and open-drain
       Step 4: go back to working mode... */
    PWR_GPIO_Init(GPIO_V_CTL1);
    PWR_InitPowerSource();
    HAL_Delay(20);
    voltage = BSP_PWR_DCDCGetVoltage(Instance);
  }

  return voltage;
}

/**
  * @}
  */


/**
  * @}
  */


/**
  * @}
  */

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
