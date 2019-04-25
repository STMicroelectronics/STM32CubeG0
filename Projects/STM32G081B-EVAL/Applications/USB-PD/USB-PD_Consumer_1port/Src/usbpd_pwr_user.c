/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    usbpd_pwr_user.c
  * @author  MCD Application Team
  * @brief   USBPD PWR user code
  ******************************************************************************
  *
  * Copyright (c) 2018 STMicroelectronics. All rights reserved.
  *
  * This software component is licensed by ST under Ultimate Liberty license
  * SLA0044, the "License"; You may not use this file except in compliance with
  * the License. You may obtain a copy of the License at:
  *                               www.st.com/SLA0044
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "usbpd_pwr_user.h"
#include "stm32g0xx_hal.h"

/* USER CODE BEGIN include */
#include "stm32g0xx_ll_lptim.h"
/* USER CODE END include */

/** @addtogroup BSP
  * @{
  */

/** @addtogroup POWER
  * @{
  */
/** @defgroup POWER_Private_Typedef Private Typedef
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
  VBUS_5_V         = 0,
  VBUS_9_V         = 1,
  VBUS_15_V        = 2,
  VBUS_18_V        = 3,
  VBUS_PPS_V       = 4,
  VBUS_LEVEL_NB    = 5
} VBUS_Level_TypeDef;

typedef enum
{
    ADC_VSENSE_1       = 0,   /*< PB1  ADC used to get VBUS1 voltage level     */
    ADC_ISENSE_1       = 1,   /*< PB10 ADC used to get VBUS1 curent value      */
    ADC_VSENSE_2       = 2,   /*< PA3  ADC used to get VBUS2 voltage level     */
    ADC_ISENSE_2       = 3,   /*< PB12 ADC used to get VBUS2 curent value      */
    ADC_VSENSE_DCDC    = 4    /*< PB11 ADC used to get DCDC voltage level      */
} ADC_ChannelId_TypeDef;

typedef enum
{
    GPIO_SOURCE_EN        = 0,   /*< PC6  Handle Source power Mos                                    */
    GPIO_VBUS_DISCHARGE1  = 1,   /*< PB13 Handle Discharge Mos on VBUS1                              */
    GPIO_VBUS_DISCHARGE2  = 2,   /*< PB14 Handle Discharge Mos on VBUS2                              */
    GPIO_VCONN_EN1        = 3,   /*< PD4  Handle Vconn on VBUS1                                      */
    GPIO_VCONN_EN2        = 4,   /*< PB9  Handle Vconn on VBUS2                                      */
    GPIO_VCONN_DISCHARGE1 = 5,   /*< PA2  Handle Fast Role Swap on VBUS1 CC1 line or Vconn Discharge */
    GPIO_VCONN_DISCHARGE2 = 6,   /*< PPB0 Handle Fast Role Swap on VBUS1 CC2 line or Vconn Discharge */
    GPIO_V_CTL1           = 7,   /*< PC1   VBUS voltage level control                                */
    GPIO_V_CTL2           = 8    /*< PB5 (Rev. A/B) or PA1 (Rev. C) VBUS voltage level control       */
} GPIO_Id_TypeDef;

typedef enum {
    PORT_NOT_INITIALIZED = 0,
    PORT_INITIALIZED
} PWR_PortStateTypedef;

typedef enum {
  POWER_NONE,
  POWER_FIXED,
  POWER_VARIABLE,
  POWER_APDO
} PWR_Type;


typedef struct {
    PWR_PortStateTypedef            State;
    PWR_PowerRoleTypeDef            Role;
    uint32_t                        VBUSDisconnectionThreshold;
    PWR_VBUSConnectionStatusTypeDef VBUSConnectionStatus;
    PWR_VBUSDetectCallbackFunc *    pfnVBUSDetectCallback;
    PWR_Type                        Type;
    uint32_t                        VBUSVoltage;
} PortInfoTypeDef;


typedef struct {
    uint8_t                         ADCRefCount;
    PWR_DCDCCtrlModeTypeDef         DCDCCtrlMode;
    PortInfoTypeDef                 PortInfo[PWR_TYPEC_PORT_NB];
} ContextTypeDef;

/* USER CODE END POWER_Private_Typedef */
/**
  * @}
  */

/** @defgroup POWER_Private_Constants Private Constants
* @{
*/
/* USER CODE BEGIN POWER_Private_Constants */
#define GPIO_SOURCE_EN_PIN                    GPIO_PIN_3  /* PD.03 */
#define GPIO_SOURCE_EN_PORT                   GPIOD
#define GPIO_SOURCE_EN_CLK_ENABLE()           __HAL_RCC_GPIOD_CLK_ENABLE()

#define GPIO_VBUS_DISCHARGE1_PIN              GPIO_PIN_13  /* PB.13 */
#define GPIO_VBUS_DISCHARGE1_PORT             GPIOB
#define GPIO_VBUS_DISCHARGE1_CLK_ENABLE()     __HAL_RCC_GPIOB_CLK_ENABLE()

#define GPIO_VBUS_DISCHARGE2_PIN              GPIO_PIN_14  /* PB.14 */
#define GPIO_VBUS_DISCHARGE2_PORT             GPIOB
#define GPIO_VBUS_DISCHARGE2_CLK_ENABLE()     __HAL_RCC_GPIOB_CLK_ENABLE()

#define GPIO_VCONN_EN1_PIN                    GPIO_PIN_4  /* PD.04 */
#define GPIO_VCONN_EN1_PORT                   GPIOD
#define GPIO_VCONN_EN1_CLK_ENABLE()           __HAL_RCC_GPIOD_CLK_ENABLE()

#define GPIO_VCONN_EN2_PIN                    GPIO_PIN_9  /* PB.09 */
#define GPIO_VCONN_EN2_PORT                   GPIOB
#define GPIO_VCONN_EN2_CLK_ENABLE()           __HAL_RCC_GPIOB_CLK_ENABLE()

#define GPIO_VCONN_DISCHARGE1_PIN             GPIO_PIN_2  /* PA.02 */
#define GPIO_VCONN_DISCHARGE1_PORT            GPIOA
#define GPIO_VCONN_DISCHARGE1_CLK_ENABLE()    __HAL_RCC_GPIOA_CLK_ENABLE()

#define GPIO_VCONN_DISCHARGE2_PIN             GPIO_PIN_0  /* PB.00 */
#define GPIO_VCONN_DISCHARGE2_PORT            GPIOB
#define GPIO_VCONN_DISCHARGE2_CLK_ENABLE()    __HAL_RCC_GPIOB_CLK_ENABLE()

#define GPIO_V_CTL2_PIN                       GPIO_PIN_1  /* PA.01 */
#define GPIO_V_CTL2_PORT                      GPIOA
#define GPIO_V_CTL2_CLK_ENABLE()              __HAL_RCC_GPIOA_CLK_ENABLE()

#define GPIO_V_CTL1_PIN                       GPIO_PIN_1  /* PC.01 */
#define GPIO_V_CTL1_PORT                      GPIOC
#define GPIO_V_CTL1_CLK_ENABLE()              __HAL_RCC_GPIOC_CLK_ENABLE()

/* Definition of ADCx clock resources */
#define ADCx                            ADC1
#define ADCx_CLK_ENABLE()               __HAL_RCC_ADC_CLK_ENABLE()
#define ADCx_CLK_DISABLE()              __HAL_RCC_ADC_CLK_DISABLE()

#define ADCx_FORCE_RESET()              __HAL_RCC_ADC_FORCE_RESET()
#define ADCx_RELEASE_RESET()            __HAL_RCC_ADC_RELEASE_RESET()

/* Definition of ADCx channels */
#define ADCx_CHANNEL_VSENSE_1           ADC_CHANNEL_9
#define ADCx_CHANNEL_ISENSE_1           ADC_CHANNEL_11
#define ADCx_CHANNEL_VSENSE_2           ADC_CHANNEL_3
#define ADCx_CHANNEL_ISENSE_2           ADC_CHANNEL_16
#define ADCx_CHANNEL_VSENSE_DCDC        ADC_CHANNEL_15

/* Definition of ADCx channels */
#define ADCxChanneln 5

#define ADCx_CHANNEL_VSENSE_1_GPIO_CLK_ENABLE() __HAL_RCC_GPIOB_CLK_ENABLE()
#define ADCx_CHANNEL_VSENSE_1_GPIO_PORT         GPIOB
#define ADCx_CHANNEL_VSENSE_1_GPIO_PIN          GPIO_PIN_1

#define ADCx_CHANNEL_ISENSE_1_GPIO_CLK_ENABLE() __HAL_RCC_GPIOB_CLK_ENABLE()
#define ADCx_CHANNEL_ISENSE_1_GPIO_PORT         GPIOB
#define ADCx_CHANNEL_ISENSE_1_GPIO_PIN          GPIO_PIN_10

#define ADCx_CHANNEL_VSENSE_2_GPIO_CLK_ENABLE() __HAL_RCC_GPIOA_CLK_ENABLE()
#define ADCx_CHANNEL_VSENSE_2_GPIO_PORT         GPIOA
#define ADCx_CHANNEL_VSENSE_2_GPIO_PIN          GPIO_PIN_3

#define ADCx_CHANNEL_ISENSE_2_GPIO_CLK_ENABLE() __HAL_RCC_GPIOB_CLK_ENABLE()
#define ADCx_CHANNEL_ISENSE_2_GPIO_PORT         GPIOB
#define ADCx_CHANNEL_ISENSE_2_GPIO_PIN          GPIO_PIN_12

#define ADCx_CHANNEL_VSENSE_DCDC_GPIO_CLK_ENABLE() __HAL_RCC_GPIOB_CLK_ENABLE()
#define ADCx_CHANNEL_VSENSE_DCDC_GPIO_PORT         GPIOB
#define ADCx_CHANNEL_VSENSE_DCDC_GPIO_PIN          GPIO_PIN_11

/* Definition of ADCx DMA resources */
#define ADCx_DMA_CLK_ENABLE()           __HAL_RCC_DMA1_CLK_ENABLE()
#define ADCx_DMA                        DMA1_Channel1

/* Definition of DMA NVIC resources */
#define ADCx_DMA_IRQn                   DMA1_Channel1_IRQn
#define ADCx_DMA_IRQHandler             DMA1_Channel1_IRQHandler

/* Definition of ADCx NVIC resources */
#define ADCx_IRQn                       ADC1_COMP_IRQn
#define ADCx_IRQHandler                 ADC1_COMP_IRQHandler

/* VDD voltage (in mv)*/
#define VDD 3300

/* Maximum digital value of the ADC output (12 Bits resolution)
   To converting ADC measurement to an absolute voltage value:
   VCHANNELx = ADCx_DATA x (VDD/ADC_FULL_SCALE)
*/
#define ADC_FULL_SCALE 0x0FFF

/* VBUS connection detection voltage threshold (in mV) */
#define VBUS_CONNECTION_THRESHOLD 3000

/* VBUS disconnection detection voltage threshold (in mV) */
#define VBUS_DISCONNECTION_THRESHOLD 2200

/* Resistor voltage divider */
#define RA 330000
#define RB  49900

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

/** @defgroup POWER_Private_Macros Private Macros
  * @{
  */
/* USER CODE BEGIN POWER_Private_Macros */
#define GPIOx_CLK_ENABLE(__GPIO__)   do { if((__GPIO__) == GPIO_SOURCE_EN)   GPIO_SOURCE_EN_CLK_ENABLE(); else \
                                          if((__GPIO__) == GPIO_VBUS_DISCHARGE1) GPIO_VBUS_DISCHARGE1_CLK_ENABLE(); else \
                                          if((__GPIO__) == GPIO_VBUS_DISCHARGE2) GPIO_VBUS_DISCHARGE2_CLK_ENABLE(); else \
                                          if((__GPIO__) == GPIO_VCONN_EN1)   GPIO_VCONN_EN1_CLK_ENABLE(); else \
                                          if((__GPIO__) == GPIO_VCONN_EN2)   GPIO_VCONN_EN2_CLK_ENABLE(); else \
                                          if((__GPIO__) == GPIO_VCONN_DISCHARGE1)     GPIO_VCONN_DISCHARGE1_CLK_ENABLE(); else \
                                          if((__GPIO__) == GPIO_VCONN_DISCHARGE2)     GPIO_VCONN_DISCHARGE2_CLK_ENABLE(); else \
                                          if((__GPIO__) == GPIO_V_CTL1)      GPIO_V_CTL1_CLK_ENABLE(); else \
                                          if((__GPIO__) == GPIO_V_CTL2)      GPIO_V_CTL2_CLK_ENABLE();} while(0)

#define ADCx_CHANNEL_GPIO_CLK_ENABLE(__ADC_CHANNEL__) \
  do { if ((__ADC_CHANNEL__) == ADCx_CHANNEL_VSENSE_1) ADCx_CHANNEL_VSENSE_1_GPIO_CLK_ENABLE(); else \
       if ((__ADC_CHANNEL__) == ADCx_CHANNEL_ISENSE_1) ADCx_CHANNEL_ISENSE_1_GPIO_CLK_ENABLE(); else \
       if ((__ADC_CHANNEL__) == ADCx_CHANNEL_VSENSE_2) ADCx_CHANNEL_VSENSE_2_GPIO_CLK_ENABLE(); else \
       if ((__ADC_CHANNEL__) == ADCx_CHANNEL_ISENSE_2) ADCx_CHANNEL_VSENSE_2_GPIO_CLK_ENABLE(); else \
       if ((__ADC_CHANNEL__) == ADCx_CHANNEL_VSENSE_DCDC) ADCx_CHANNEL_VSENSE_DCDC_GPIO_CLK_ENABLE();} while(0)

#define ABS(__VAL__) (((int32_t)(__VAL__)) < 0 ? - ((int32_t)(__VAL__)) : ((int32_t)(__VAL__)))

/* USER CODE END POWER_Private_Macros */
/**
  * @}
  */

/** @defgroup POWER_Private_Variables Private Variables
  * @{
  */
/* USER CODE BEGIN POWER_Private_Variables */
/**
* @brief GPIO variables
*/
/* GPIO Id. / GPIO port cross table */
static GPIO_TypeDef * GPIO_PORT[] = {
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
static const uint16_t GPIO_PIN[] = {
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

/* V/I Sense / ADC Channel cross table */
static const uint32_t ADC_CHANNEL[ADCxChanneln] =
{
    ADCx_CHANNEL_VSENSE_1,
    ADCx_CHANNEL_ISENSE_1,
    ADCx_CHANNEL_VSENSE_2,
    ADCx_CHANNEL_ISENSE_2,
    ADCx_CHANNEL_VSENSE_DCDC
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
   aADCxConvertedValues[0]: VSENSE_2
   aADCxConvertedValues[1]: VSENSE_1
   aADCxConvertedValues[2]: ISENSE_1
   aADCxConvertedValues[3]: ISENSE_2
*/
static __IO uint16_t   aADCxConvertedValues[4] = {0};

/* BSP PWR contextual data */
static ContextTypeDef Context =
{
    .ADCRefCount         = 0,
    .DCDCCtrlMode        = DCDC_CTRL_MODE_UNKNOWN,
    .PortInfo =
    {
        /* TYPE_C_PORT_1 */
        {
            .State                      = PORT_NOT_INITIALIZED,
            .Role                       = POWER_ROLE_SOURCE,
            .VBUSDisconnectionThreshold = VBUS_DISCONNECTION_THRESHOLD,
            .VBUSConnectionStatus       = VBUS_NOT_CONNECTED,
            .pfnVBUSDetectCallback      = (PWR_VBUSDetectCallbackFunc *)NULL,
            .VBUSVoltage                = 0,
        },
        /* TYPE_C_PORT_2 */
        {
            .State                      = PORT_NOT_INITIALIZED,
            .Role                       = POWER_ROLE_SINK,
            .VBUSDisconnectionThreshold = VBUS_DISCONNECTION_THRESHOLD,
            .VBUSConnectionStatus       = VBUS_NOT_CONNECTED,
            .pfnVBUSDetectCallback      = (PWR_VBUSDetectCallbackFunc *)NULL,
            .VBUSVoltage                = 0,
        }
    }
};

/* VBUS sensing completed flag */
uint8_t VBUSSensingCompleted = 0;

/* Power source calibration related variables */
uint16_t aPWMDutyCycleLUT[VBUS_LEVEL_NB] = {
  0x00B0,   /* 5V */
  0x0200,   /* 9V */
  0x0300,   /* 15V */
  0x0400,   /* 18V */
  0x0000    /* Will be used to store latest PPS request */
};

uint8_t aPWMDutyCalibrated[VBUS_LEVEL_NB] = {
  0x0,   /* 5V */
  0x0,   /* 9V */
  0x0,   /* 15V */
  0x0,   /* 18V */
  0x0    /* Will be used to store latest PPS request */
};

uint16_t aPWMDutyCycleLUT_PPS[PWM_MAX_VALUES_LUT_PPS] =
{
/* 3300, 3320, 3340, 3360, 3380, 3400, 3420, 3440, 3460, 3480 */
   48,   52,   56,   61,   65,   70,   74,   79,   83,   88,
/* 3500, 3520, 3540, 3560, 3580, 3600, 3620, 3640, 3660, 3680 */
   92,   97,   101,   106,   110,   115,   119,   124,   128,   133,
/* 3700, 3720, 3740, 3760, 3780, 3800, 3820, 3840, 3860, 3880 */
   137,   142,   146,   151,   155,   160,   164,   169,   173,   177,
/* 3900, 3920, 3940, 3960, 3980, 4000, 4020, 4040, 4060, 4080 */
   182,   186,   191,   195,   200,   204,   209,   213,   218,   222,
/* 4100, 4120, 4140, 4160, 4180, 4200, 4220, 4240, 4260, 4280 */
   227,   231,   236,   240,   245,   249,   254,   258,   263,   267,
/* 4300, 4320, 4340, 4360, 4380, 4400, 4420, 4440, 4460, 4480 */
   272,   276,   281,   285,   290,   294,   299,   303,   307,   312,
/* 4500, 4520, 4540, 4560, 4580, 4600, 4620, 4640, 4660, 4680 */
   316,   321,   325,   330,   334,   339,   343,   348,   352,   357,
/* 4700, 4720, 4740, 4760, 4780, 4800, 4820, 4840, 4860, 4880 */
   361,   366,   370,   375,   379,   384,   388,   393,   397,   402,
/* 4900, 4920, 4940, 4960, 4980, 5000, 5020, 5040, 5060, 5080 */
   406,   411,   415,   420,   424,   429,   433,   437,   442,   446,
/* 5100, 5120, 5140, 5160, 5180, 5200, 5220, 5240, 5260, 5280 */
   451,   455,   460,   464,   469,   473,   478,   482,   487,   491,
/* 5300, 5320, 5340, 5360, 5380, 5400, 5420, 5440, 5460, 5480 */
   496,   500,   505,   509,   514,   518,   523,   527,   532,   536,
/* 5500, 5520, 5540, 5560, 5580, 5600, 5620, 5640, 5660, 5680 */
   541,   545,   550,   554,   558,   563,   567,   572,   576,   581,
/* 5700, 5720, 5740, 5760, 5780, 5800, 5820, 5840, 5860, 5880 */
   585,   590,   594,   599,   603,   608,   612,   617,   621,   626,
/* 5900 */
   630
};

uint8_t aPWMDutyCalibrated_PPS[PWM_MAX_VALUES_LUT_PPS] = { 0 };

/* USER CODE END POWER_Private_Variables */
/**
  * @}
  */

/** @defgroup POWER_Private_Functions Private Functions
  * @{
  */
/* USER CODE BEGIN POWER_Private_Prototypes */
static uint8_t  PWR_InitSource(uint32_t PortId);
static uint8_t  PWR_DeInitSource(uint32_t PortId);
static uint8_t  PWR_InitSink(uint32_t PortId);
static uint8_t  PWR_DeInitSink(uint32_t PortId);
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
static uint32_t PWR_DCDCPreChargeC57(uint32_t PortId);
static PWR_StatusTypeDef PWR_VBUSSetVoltage(uint32_t PortId,
                                         uint32_t VBusInmV,
                                         uint16_t Precision);
uint32_t PWR_DCDCGetVoltage(uint32_t PortId);
static void ADCx_MspInit(ADC_HandleTypeDef *hadc);
static void ADCx_MspDeInit(ADC_HandleTypeDef *hadc);
static void LPTIMx_MspInit(LPTIM_HandleTypeDef *hlptim);
static void LPTIMx_MspDeInit(LPTIM_HandleTypeDef *hlptim);
static void     PWR_VBUSDischarge(uint32_t PortId,
                           uint32_t VbusStopInmV,
                           uint32_t VbusErrorInmV,
                           uint16_t Delay);
/* USER CODE END POWER_Private_Prototypes */
/**
  * @}
  */

/** @defgroup POWER_Exported_Variables Exported Variables
  * @{
  */
/* USER CODE BEGIN POWER_Exported_Variables */
extern ADC_HandleTypeDef hadc1;
extern DMA_HandleTypeDef hdma_adc1;
LPTIM_HandleTypeDef hlptim  = {0};
/* USER CODE END POWER_Exported_Variables */
/**
  * @}
  */

/** @addtogroup POWER_Exported_Functions
  * @{
  */
/**
  * @brief  Initialize the hardware resources used by the Type-C power delivery (PD)
  *         controller.
  * @param  PortId Type-C port identifier
  *         This parameter can be take one of the following values:
  *         @arg TYPE_C_PORT_1
  *         @arg TYPE_C_PORT_2
  * @retval PD controller status
  */
PWR_StatusTypeDef BSP_PWR_VBUSInit(uint32_t               PortId)
{
/* USER CODE BEGIN BSP_PWR_VBUSInit */
  uint32_t ret = 0;
  PWR_PowerRoleTypeDef   role;

  if (Context.PortInfo[PortId].State == PORT_NOT_INITIALIZED)
  {
    if (PortId == TYPE_C_PORT_1)
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
      ret += PWR_InitSink(PortId);

      if (Context.ADCRefCount == 1)
      {
        ret += PWR_StartVBusSensing();
      }
    }

    if ((role == POWER_ROLE_SOURCE) || (role == POWER_ROLE_DUAL))
    {
      ret += PWR_InitSource(PortId);
    }

    /* Update PWR context */
    Context.PortInfo[PortId].State = PORT_INITIALIZED;
    Context.PortInfo[PortId].Role = role;
  }

  return (ret == 0) ? PWR_OK : PWR_ERROR;
/* USER CODE END BSP_PWR_VBUSInit */
}

/**
  * @brief  Release the hardware resources used by the Type-C power delivery (PD)
  *         controller.
  * @param  PortId Type-C port identifier
  *         This parameter can be take one of the following values:
  *         @arg TYPE_C_PORT_1
  *         @arg TYPE_C_PORT_2
  * @retval PD controller status
  */
PWR_StatusTypeDef BSP_PWR_VBUSDeInit(uint32_t PortId)
{
/* USER CODE BEGIN BSP_PWR_VBUSDeInit */
  uint32_t ret = 0;

  if (Context.PortInfo[PortId].State == PORT_INITIALIZED)
  {
    if (PortId == TYPE_C_PORT_1)
    {
      PWR_GPIO_DeInit(GPIO_VCONN_EN1);
      PWR_GPIO_DeInit(GPIO_VCONN_EN2);
    }

    if ((Context.PortInfo[PortId].Role == POWER_ROLE_SOURCE) ||
        (Context.PortInfo[PortId].Role == POWER_ROLE_DUAL))
    {
      ret += PWR_DeInitSource(PortId);
    }

    if ((Context.PortInfo[PortId].Role == POWER_ROLE_SINK) ||
        (Context.PortInfo[PortId].Role == POWER_ROLE_DUAL))
    {
      if (Context.ADCRefCount == 1)
      {
        ret += PWR_StopVBusSensing();
      }
      ret += PWR_DeInitSink(PortId);
    }

    /* Update PWR context */
    Context.PortInfo[PortId].State = PORT_NOT_INITIALIZED;
  }

  return (ret == 0) ? PWR_OK : PWR_ERROR;
/* USER CODE END BSP_PWR_VBUSDeInit */
}

/**
  * @brief  Enable power supply over VBUS.
  * @param  PortId Type-C port identifier
  *         This parameter can be take one of the following values:
  *         @arg TYPE_C_PORT_1
  * @retval PD controller status
  */
PWR_StatusTypeDef BSP_PWR_VBUSOn(uint32_t PortId)
{
/* USER CODE BEGIN BSP_PWR_VBUSOn */
  PWR_StatusTypeDef retr = PWR_OK;

  if (PortId == TYPE_C_PORT_1)
  {
    /* Start VBUS Discharge */
    PWR_GPIO_On(GPIO_VBUS_DISCHARGE1);

    /* safety control to avoid spike when enabling the power */
    /* issue detected during board reset */
    uint32_t _tickstart = HAL_GetTick();
    uint32_t _dcdc;
    uint32_t _delta = PWR_DCDCGetVoltage(PortId);

    do {
      _dcdc = PWR_DCDCGetVoltage(PortId);

      /* check after 2 second check if the power has start to decrease */
      if((((HAL_GetTick() - _tickstart) > 2000)) && ((_delta - _dcdc) < 500))
      {
  return PWR_ERROR;
      }

      /* Timeout management */
      if((HAL_GetTick() - _tickstart) > 15000)
      {
        return PWR_ERROR;
      }
    }
    while (_dcdc > 5500);

    /* Switch on VBUS */
    PWR_GPIO_On(GPIO_SOURCE_EN);

    /* Set VBUS to its default voltage level */
    retr = PWR_VBUSSetVoltage(PortId, VBUS_VOLTAGE_5V_IN_MV, 200); /* 5% max = 250mV max */

    /* Stop VBUS Discharge */
    PWR_GPIO_Off(GPIO_VBUS_DISCHARGE1);
  }
  else
  {
    retr = PWR_ERROR;
  }

  /* Save the power context */
  Context.PortInfo[PortId].VBUSVoltage = VBUS_VOLTAGE_5V_IN_MV;
  Context.PortInfo[PortId].Type = POWER_FIXED;

  return retr;
/* USER CODE END BSP_PWR_VBUSOn */
}

/**
  * @brief  Disable power supply over VBUS.
  * @param  PortId Type-C port identifier
  *         This parameter can be take one of the following values:
  *         @arg TYPE_C_PORT_1
  * @retval PD controller status
  */
PWR_StatusTypeDef BSP_PWR_VBUSOff(uint32_t PortId)
{
/* USER CODE BEGIN BSP_PWR_VBUSOff */
  uint32_t ret = 0;

  if (PortId == TYPE_C_PORT_1)
  {
    /* Go back to original VBUS value */
    PWR_VBUSSetVoltage(0, VBUS_VOLTAGE_0V_IN_MV, 0);

    /* Discharge VBUS */
    PWR_VBUSDischarge(PortId, BSP_PWR_LOW_VBUS_THRESHOLD, 0, 0);

  }
  else
  {
    ret++;
  }

   /* Save the power context */
  Context.PortInfo[PortId].VBUSVoltage = VBUS_VOLTAGE_0V_IN_MV;
  Context.PortInfo[PortId].Type = POWER_NONE;

  return (ret == 0) ? PWR_OK : PWR_ERROR;
/* USER CODE END BSP_PWR_VBUSOff */
}

/**
  * @brief  Set a fixed/variable PDO and manage the power control.
  * @param  PortId Type-C port identifier
  *         This parameter can be take one of the following values:
  *         @arg TYPE_C_PORT_1
  * @param  VbusTargetInmv the vbus Target (in mV)
  * @param  OperatingCurrent the Operating Current (in mA)
  * @param  MaxOperatingCurrent the Max Operating Current (in mA)
  * @retval PD controller status
  */
PWR_StatusTypeDef BSP_PWR_VBUSSetVoltage_Fixed(uint32_t PortId,
                                               uint32_t VbusTargetInmv,
                                               uint32_t OperatingCurrent,
                                               uint32_t MaxOperatingCurrent)
{
/* USER CODE BEGIN BSP_PWR_VBUSSetVoltage_Fixed */
  PWR_StatusTypeDef retr = PWR_OK;

  if(Context.PortInfo[PortId].VBUSVoltage != VbusTargetInmv)
  {
    /* Start VBUS Discharge, done to increase the decrease speed */
    PWR_GPIO_On(GPIO_VBUS_DISCHARGE1);

    /* Set the power, the precision must be at 5% */
    retr = PWR_VBUSSetVoltage(PortId, VbusTargetInmv, VbusTargetInmv * 5 / 100);

    /* Wait the power stabilization */
    PWR_GPIO_Off(GPIO_VBUS_DISCHARGE1);
  }

  /* Upate the context */
  Context.PortInfo[PortId].VBUSVoltage = VbusTargetInmv;
  Context.PortInfo[PortId].Type = POWER_FIXED;

  /* Set the current limitation */
  /* not implemented */

  return retr;
/* USER CODE END BSP_PWR_VBUSSetVoltage_Fixed */
}

/**
  * @brief  Set a fixed/variable PDO and manage the power control.
  * @param  PortId Type-C port identifier
  *         This parameter can be take one of the following values:
  *         @arg TYPE_C_PORT_1
  * @param  VbusTargetMinInmv the vbus Target min (in mV)
  * @param  VbusTargetMaxInmv the vbus Target max (in mV)
  * @param  OperatingCurrent the Operating Current (in mA)
  * @param  MaxOperatingCurrent the Max Operating Current (in mA)
  * @retval PD controller status
  */
PWR_StatusTypeDef BSP_PWR_VBUSSetVoltage_Variable(uint32_t PortId,
                                                  uint32_t VbusTargetMinInmv,
                                                  uint32_t VbusTargetMaxInmv,
                                                  uint32_t OperatingCurrent,
                                                  uint32_t MaxOperatingCurrent)
{
/* USER CODE BEGIN BSP_PWR_VBUSSetVoltage_Variable */
  return PWR_ERROR;
/* USER CODE BEGIN BSP_PWR_VBUSSetVoltage_Variable */
}

/**
  * @brief  Set a Battery PDO and manage the power control.
  * @param  PortId Type-C port identifier
  *         This parameter can be take one of the following values:
  *         @arg TYPE_C_PORT_1
  * @param  VbusTargetMin the vbus Target min (in mV)
  * @param  VbusTargetMax the vbus Target max (in mV)
  * @param  OperatingPower the Operating Power (in mW)
  * @param  MaxOperatingPower the Max Operating Power (in mW)
  * @retval PD controller status
  */
PWR_StatusTypeDef BSP_PWR_VBUSSetVoltage_Battery(uint32_t PortId,
                                                 uint32_t VbusTargetMin,
                                                 uint32_t VbusTargetMax,
                                                 uint32_t OperatingPower,
                                                 uint32_t MaxOperatingPower)
{
/* USER CODE BEGIN BSP_PWR_VBUSSetVoltage_Battery */
  return PWR_ERROR;
/* USER CODE BEGIN BSP_PWR_VBUSSetVoltage_Battery */
}

/**
  * @brief  Set a APDO and manage the power control.
  * @param  PortId            Type-C port identifier
  *         This parameter can be take one of the following values:
  *         @arg TYPE_C_PORT_1
  * @param  VbusTargetInmv    the vbus Target (in mV)
  * @param  OperatingCurrent  the Operating current (in mA)
  * @param  Delta             Delta between with previous APDO (in mV), 0 means APDO start
  * @retval PD controller status
  */
PWR_StatusTypeDef BSP_PWR_VBUSSetVoltage_APDO(uint32_t PortId,
                                              uint32_t VbusTargetInmv,
                                              uint32_t OperatingCurrent,
                                              int32_t Delta)
{
/* USER CODE BEGIN BSP_PWR_VBUSSetVoltage_APDO */
  return PWR_ERROR;
/* USER CODE BEGIN BSP_PWR_VBUSSetVoltage_APDO */
}

/**
  * @brief  Get actual voltage level measured on the VBUS line.
  * @param  PortId Type-C port identifier
  *         This parameter can be take one of the following values:
  *         @arg TYPE_C_PORT_1
  *         @arg TYPE_C_PORT_2
  * @retval Voltage measured voltage level (in mV)
  */
uint32_t  BSP_PWR_VBUSGetVoltage(uint32_t PortId)
{
/* USER CODE BEGIN BSP_PWR_VBUSGetVoltage */
  uint32_t voltage = 0;

  if (PortId == TYPE_C_PORT_1)
  {
    voltage = PWR_ConvertADCDataToVoltage(aADCxConvertedValues[1]);
  }
  else if (PortId == TYPE_C_PORT_2)
  {
    voltage = PWR_ConvertADCDataToVoltage(aADCxConvertedValues[0]);
  }
  return voltage;
/* USER CODE END BSP_PWR_VBUSGetVoltage */
}

/**
  * @brief  Get actual current level measured on the VBUS line.
  * @param  PortId Type-C port identifier
  *         This parameter can be take one of the following values:
  *         @arg TYPE_C_PORT_1
  *         @arg TYPE_C_PORT_2
  * @retval Current measured current level (in mA)
  */
int32_t BSP_PWR_VBUSGetCurrent(uint32_t PortId)
{
/* USER CODE BEGIN BSP_PWR_VBUSGetCurrent */
  int32_t current = 0;

  if (PortId == TYPE_C_PORT_1)
  {
    current = PWR_ConvertADCDataToCurrent(aADCxConvertedValues[2]);
  }
  else if (PortId == TYPE_C_PORT_2)
  {
    current = PWR_ConvertADCDataToCurrent(aADCxConvertedValues[3]);
  }

  return (current);
/* USER CODE END BSP_PWR_VBUSGetCurrent */
}

/**
  * @brief  Initialize VCONN sourcing.
  * @param  PortId Type-C port identifier
  *         This parameter can be take one of the following values:
  *         @arg TYPE_C_PORT_1
  *         @arg TYPE_C_PORT_2
  * @param  CCPinId Type-C CC pin identifier
  *         This parameter can be take one of the following values:
  *         @arg TYPE_C_CC1
  *         @arg TYPE_C_CC2
  * @retval PD controller status
  */
PWR_StatusTypeDef BSP_PWR_VCONNInit(uint32_t PortId,
                                    uint32_t CCPinId)
{
/* USER CODE BEGIN BSP_PWR_VCONNInit */
  PWR_StatusTypeDef ret = PWR_ERROR;

  if (PortId == TYPE_C_PORT_1)
  {
    switch(CCPinId)
    {
    case TYPE_C_CC1 :
      {
        PWR_GPIO_Init(GPIO_VCONN_DISCHARGE1);
        ret = PWR_OK;
      }
      break;
    case TYPE_C_CC2 :
      {
        PWR_GPIO_Init(GPIO_VCONN_DISCHARGE2);
        ret = PWR_OK;
      }
      break;
    default:
      break;
    }
  }
  return ret;
/* USER CODE END BSP_PWR_VCONNInit */
}

/**
  * @brief  Un-Initialize VCONN sourcing.
  * @param  PortId Type-C port identifier
  *         This parameter can be take one of the following values:
  *         @arg TYPE_C_PORT_1
  *         @arg TYPE_C_PORT_2
  * @param  CCPinId Type-C CC pin identifier
  *         This parameter can be take one of the following values:
  *         @arg TYPE_C_CC1
  *         @arg TYPE_C_CC2
  * @retval PD controller status
  */
PWR_StatusTypeDef BSP_PWR_VCONNDeInit(uint32_t PortId,
                                      uint32_t CCPinId)
{
/* USER CODE BEGIN BSP_PWR_VCONNDeInit */
  PWR_StatusTypeDef ret = PWR_ERROR;

  if (PortId == TYPE_C_PORT_1)
  {
    switch(CCPinId)
    {
    case TYPE_C_CC1 :
      {
        PWR_GPIO_DeInit(GPIO_VCONN_DISCHARGE1);
        ret = PWR_OK;
      }
      break;
    case TYPE_C_CC2 :
      {
        PWR_GPIO_DeInit(GPIO_VCONN_DISCHARGE2);
        ret = PWR_OK;
      }
      break;
    default:
      break;
    }
  }
  return ret;
/* USER CODE END BSP_PWR_VCONNDeInit */
}

/**
  * @brief  Enable VCONN sourcing.
  * @param  PortId Type-C port identifier
  *         This parameter can be take one of the following values:
  *         @arg TYPE_C_PORT_1
  *         @arg TYPE_C_PORT_2
  * @param  CCPinId Type-C CC pin identifier
  *         This parameter can be take one of the following values:
  *         @arg TYPE_C_CC1
  *         @arg TYPE_C_CC2
  * @retval PD controller status
  */
PWR_StatusTypeDef BSP_PWR_VCONNOn(uint32_t PortId,
                                  uint32_t CCPinId)
{
/* USER CODE BEGIN BSP_PWR_VCONNOn */
  PWR_StatusTypeDef ret = PWR_ERROR;

  if (PortId == TYPE_C_PORT_1)
  {
    if (CCPinId == TYPE_C_CC1)
    {
      PWR_GPIO_On(GPIO_VCONN_EN1);
      ret = PWR_OK;
    }
    else if (CCPinId == TYPE_C_CC2)
    {
      PWR_GPIO_On(GPIO_VCONN_EN2);
      ret = PWR_OK;
    }
  }

  return ret;
/* USER CODE END BSP_PWR_VCONNOn */
}

/**
  * @brief  Disable VCONN sourcing.
  * @param  PortId Type-C port identifier
  *         This parameter can be take one of the following values:
  *         @arg TYPE_C_PORT_1
  *         @arg TYPE_C_PORT_2
  * @param  CCPinId CC pin identifier
  *         This parameter can be take one of the following values:
  *         @arg TYPE_C_CC1
  *         @arg TYPE_C_CC2
  * @retval PD controller status
  */
PWR_StatusTypeDef BSP_PWR_VCONNOff(uint32_t PortId,
                                   uint32_t CCPinId)
{
/* USER CODE BEGIN BSP_PWR_VCONNOff */
  PWR_StatusTypeDef ret = PWR_ERROR;

  if (PortId == TYPE_C_PORT_1)
  {
    if (CCPinId == TYPE_C_CC1)
    {
      /* switch off Vconn */
      PWR_GPIO_Off(GPIO_VCONN_EN1);
      PWR_GPIO_On(GPIO_VCONN_EN1);
      PWR_GPIO_Off(GPIO_VCONN_EN1);

      /* Dicharge VCONN */
      PWR_GPIO_On(GPIO_VCONN_DISCHARGE1);
      HAL_Delay(1);
      PWR_GPIO_Off(GPIO_VCONN_DISCHARGE1);
      ret = PWR_OK;
    }
    else if (CCPinId == TYPE_C_CC2)
    {
      /* switch off Vconn */
      PWR_GPIO_Off(GPIO_VCONN_EN2);
      PWR_GPIO_On(GPIO_VCONN_EN2);
      PWR_GPIO_Off(GPIO_VCONN_EN2);

      /* Dicharge VCONN */
      PWR_GPIO_On(GPIO_VCONN_DISCHARGE2);
      HAL_Delay(1);
      PWR_GPIO_Off(GPIO_VCONN_DISCHARGE2);
      ret = PWR_OK;
    }
  }

  return ret;
/* USER CODE END BSP_PWR_VCONNOff */
}

/**
  * @brief  Set the VBUS disconnection voltage threshold.
  * @note   Callback funtion registered through BSP_PWR_RegisterVBUSDetectCallback
  *         function call is invoked when VBUS falls below programmed threshold.
  * @note   By default VBUS disconnection threshold is set to 3.3V
  * @param  PortId Type-C port identifier
  *         This parameter can be take one of the following values:
  *         @arg TYPE_C_PORT_1
  *         @arg TYPE_C_PORT_2
  * @param  VoltageThreshold: VBUS disconnection voltage threshold (in mV)
  * @retval PD controller status
  */
void BSP_PWR_SetVBUSDisconnectionThreshold(uint32_t PortId,
                                           uint32_t VoltageThreshold)
{
/* USER CODE BEGIN BSP_PWR_SetVBUSDisconnectionThreshold */
  /* Update PWR context */
  if (VoltageThreshold <= VBUS_CONNECTION_THRESHOLD)
  {
    Context.PortInfo[PortId].VBUSDisconnectionThreshold = VoltageThreshold;
  }
/* USER CODE END BSP_PWR_SetVBUSDisconnectionThreshold */
}

/**
  * @brief  Register USB Type-C Current callback function.
  * @note   Callback function invoked when VBUS rises above 4V (VBUS present) or
  *         when VBUS falls below programmed threshold (VBUS absent).
  * @note   Callback funtion is un-registered when callback function pointer
  *         argument is NULL.
  * @param  PortId Type-C port identifier
  *         This parameter can be take one of the following values:
  *         @arg TYPE_C_PORT_1
  *         @arg TYPE_C_PORT_2
  * @param  pfnVBUSDetectCallback callback function pointer
  * @retval 0 success else fail
  */
PWR_StatusTypeDef BSP_PWR_RegisterVBUSDetectCallback(uint32_t                       PortId,
                                                     PWR_VBUSDetectCallbackFunc *   pfnVBUSDetectCallback)
{
/* USER CODE BEGIN BSP_PWR_RegisterVBUSDetectCallback */
  return PWR_ERROR;
/* USER CODE END BSP_PWR_RegisterVBUSDetectCallback */
}

/**
  * @brief  Get actual VBUS status.
  * @param  PortId Type-C port identifier
  *         This parameter can be take one of the following values:
  *         @arg TYPE_C_PORT_1
  * @retval VBUS status (1: On, 0: Off)
  */
uint8_t BSP_PWR_VBUSIsOn(uint32_t PortId)
{
/* USER CODE BEGIN BSP_PWR_VBUSIsOn */
  if (PortId == TYPE_C_PORT_1)
  {
    return (READ_BIT(GPIO_PORT[GPIO_SOURCE_EN]->ODR, GPIO_PIN[GPIO_SOURCE_EN]) == GPIO_PIN[GPIO_SOURCE_EN]) ? 1 : 0;
  }
  else
  {
    return 0;
  }
/* USER CODE END BSP_PWR_VBUSIsOn */
}

/**
  * @brief  Get actual VCONN status.
  * @param  PortId Type-C port identifier
  *         This parameter can be take one of the following values:
  *         @arg TYPE_C_PORT_1
  * @param  CCPinId Type-C CC pin identifier
  *         This parameter can be take one of the following values:
  *         @arg TYPE_C_CC1
  *         @arg TYPE_C_CC2
  * @retval VCONN status (1: On, 0: Off)
  */
  uint8_t BSP_PWR_VCONNIsOn(uint32_t PortId,
                          uint32_t CCPinId)
{
/* USER CODE BEGIN BSP_PWR_VCONNIsOn */
  GPIO_Id_TypeDef gpio;

  if (PortId == TYPE_C_PORT_1)
  {
    if (CCPinId == TYPE_C_CC1)
    {
      gpio = GPIO_VCONN_EN1;
    }
    else
    {
      gpio = GPIO_VCONN_EN2;
    }

    return (READ_BIT(GPIO_PORT[gpio]->ODR, GPIO_PIN[gpio]) == GPIO_PIN[gpio]);
  }
  else
  {
    return 0;
  }
/* USER CODE END BSP_PWR_VCONNIsOn */
}

/**
  * @}
  */

/** @addtogroup POWER_Private_Functions
  * @{
  */

/* USER CODE BEGIN POWER_Private_Functions */
/**
  * @brief  Initialize the hardware resources used by a port acting as a source.
  * @param  PortId Type-C port identifier
  *         This parameter can be take one of the following values:
  *         @arg TYPE_C_PORT_1
  * @retval 0 success else fail
  */
static uint8_t PWR_InitSource(uint32_t PortId)
{
  uint8_t ret = 0;

  if (PortId == TYPE_C_PORT_1)
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
 * @param  PortId Type-C port identifier
 *         This parameter can take following value:
 *         @arg TYPE_C_PORT_1
* @retval 0 success else fail
 */
static uint8_t PWR_DeInitSource(uint32_t PortId)
{
  if (PortId == TYPE_C_PORT_1)
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
 * @param  PortId Type-C port identifier
 *         This parameter can take one of the following values:
 *         @arg TYPE_C_PORT_1
 *         @arg TYPE_C_PORT_2
* @retval 0 success else fail
 */
static uint8_t PWR_InitSink(uint32_t PortId)
{
  uint32_t ret = 0;

  ret += PWR_ADC_SetConfig();

  Context.ADCRefCount++;

  return (ret == 0) ? PWR_OK : PWR_ERROR;
}

/**
 * @brief  Release the hardware resources used by a port acting as a sink.
 * @param  PortId Type-C port identifier
 *         This parameter can be take one of the following values:
 *         @arg TYPE_C_PORT_1
 *         @arg TYPE_C_PORT_2
* @retval 0 success else fail
 */
static uint8_t PWR_DeInitSink(uint32_t PortId)
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
*     @arg   GPIO_SOURCE_EN
*     @arg   GPIO_VBUS_DISCHARGE1
*     @arg   GPIO_VBUS_DISCHARGE2
*     @arg   GPIO_VCONN_EN1
*     @arg   GPIO_VCONN_EN2
*     @arg   GPIO_VCONN_DISCHARGE1
*     @arg   GPIO_VCONN_DISCHARGE2
*     @arg   GPIO_V_CTL2
*     @arg   GPIO_V_CTL1
* @retval None
  */
static void PWR_GPIO_Init(GPIO_Id_TypeDef gpio)
{
  GPIO_InitTypeDef  GPIO_InitStruct = {0};

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
  }
}

/**
* @brief  Reset  GPIO configuration.
* @param  gpio Specifies the GPIO Pin .
*   This parameter can be one of following parameters:
*     @arg   GPIO_SOURCE_EN
*     @arg   GPIO_VBUS_DISCHARGE1
*     @arg   GPIO_VBUS_DISCHARGE2
*     @arg   GPIO_VCONN_EN1
*     @arg   GPIO_VCONN_EN2
*     @arg   GPIO_VCONN_DISCHARGE1
*     @arg   GPIO_VCONN_DISCHARGE2
*     @arg   GPIO_V_CTL2
*     @arg   GPIO_V_CTL1
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
*     @arg   GPIO_SOURCE_EN
*     @arg   GPIO_VBUS_DISCHARGE1
*     @arg   GPIO_VBUS_DISCHARGE2
*     @arg   GPIO_VCONN_EN1
*     @arg   GPIO_VCONN_EN2
*     @arg   GPIO_VCONN_DISCHARGE1
*     @arg   GPIO_VCONN_DISCHARGE2
*     @arg   GPIO_V_CTL2
*     @arg   GPIO_V_CTL1
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
      GPIO_InitTypeDef  GPIO_InitStruct = {0};

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
*     @arg   GPIO_SOURCE_EN
*     @arg   GPIO_VBUS_DISCHARGE1
*     @arg   GPIO_VBUS_DISCHARGE2
*     @arg   GPIO_VCONN_EN1
*     @arg   GPIO_VCONN_EN2
*     @arg   GPIO_VCONN_DISCHARGE1
*     @arg   GPIO_VCONN_DISCHARGE2
*     @arg   GPIO_V_CTL2
*     @arg   GPIO_V_CTL1
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
      GPIO_InitTypeDef  GPIO_InitStruct = {0};

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
  uint8_t ret = 0;

  /* Initialize ADC (if not already used) */
  if (Context.ADCRefCount == 0)
  {
    __HAL_ADC_RESET_HANDLE_STATE(&hadc1);

    /* Configuration of hadc init structure: ADC parameters and regular group */
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
  if (Context.ADCRefCount == 0)
  {
    /* Stop ADCx conversions */
    HAL_ADC_Stop_DMA(&hadc1);

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
*     @arg   ADC_VSENSE_1
*     @arg   ADC_VSENSE_2
*     @arg   ADC_ISENSE_1
*     @arg   ADC_ISENSE_2
*     @arg   ADC_VSENSE_DCDC (MB1352C only)
* @retval 0 success else fail
*/
static uint8_t PWR_ADC_SetChannelConfig(ADC_ChannelId_TypeDef ADCChannel)
{
  uint8_t                  ret = 0;
  GPIO_InitTypeDef         gpio_config = {0};
  ADC_ChannelConfTypeDef   adc_channel_config = {0};

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

  if (HAL_ADC_ConfigChannel(&hadc1, &adc_channel_config) != HAL_OK) ret++;

  return ret;
}

/**
* @brief  Reset ADC channel configuration.
* @param  ADCChannel Specifies the ADC channel
*     @arg   ADC_VSENSE_1
*     @arg   ADC_VSENSE_2
*     @arg   ADC_ISENSE_1
*     @arg   ADC_ISENSE_2
*     @arg   ADC_VSENSE_DCDC (MB1352C only)
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
  uint32_t voltage = 0;
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
  uint32_t threshold = 0;
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
  int32_t current = 0;

  current = ((ADCData * VDD) >> 10) - 6600;

  return current;
}

/**
 * @brief  Start I/V sense on both Type-C ports.
 * @retval 0 success else fail
 */
static uint8_t PWR_StartVBusSensing(void)
{
  uint8_t ret = 0;

  /* Start ADCx conversions */
  if (HAL_ADC_Start_DMA(&hadc1, (uint32_t *)aADCxConvertedValues, 4) != HAL_OK)  ret++;

  return ret;
}

/**
 * @brief  Stop I/V sense on both Type-C ports.
 * @retval 0 success else fail
 */
static uint8_t PWR_StopVBusSensing(void)
{
  uint8_t ret = 0;

  /* Stop ADCx conversions */
  if (HAL_ADC_Stop_DMA(&hadc1) != HAL_OK)  ret++;

  VBUSSensingCompleted = 0;

  return ret;
}

/**
 * @brief  Pause I/V sense on both Type-C ports.
 * @retval 0 success else fail
 */
static uint8_t PWR_PauseVBusSensing(void)
{
  uint8_t ret = 0;

  /* Stop on-going conversions */
  if (HAL_ADC_Stop_DMA(&hadc1) != HAL_OK)  ret++;

  /* Reset Channel selection register */
  hadc1.Instance->CHSELR &= ~(ADC_CHSELR_SQ_ALL);

  VBUSSensingCompleted = 0;

  return ret;
}

/**
 * @brief  Resume I/V sense on both Type-C ports.
 * @retval 0 success else fail
 */
static uint8_t PWR_ResumeVBusSensing(void)
{
  uint8_t  ret = 0;
  uint32_t tickstart = 0;

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
  if (HAL_ADC_Start_DMA(&hadc1, (uint32_t *)aADCxConvertedValues, 4) != HAL_OK)  ret++;

  /* Wait until 1st DMA conversion is completed */
  tickstart = HAL_GetTick();

  while (VBUSSensingCompleted == 0)
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
  HAL_DMA_Init(&hdma_adc1);

  /* Associate the initialized DMA handle to the ADC handle */
  __HAL_LINKDMA(hadc, DMA_Handle, hdma_adc1);

  /* NVIC configuration for DMA interrupt (transfer completion or error) */
  HAL_NVIC_SetPriority(ADCx_DMA_IRQn, 1, 0);
  HAL_NVIC_EnableIRQ(ADCx_DMA_IRQn);

  /* NVIC configuration for ADCx interrupt */
  HAL_NVIC_SetPriority(ADC1_COMP_IRQn, 0, 0);
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
  ADC_AnalogWDGConfTypeDef adc_awd_config = {0};

  if (Context.PortInfo[TYPE_C_PORT_1].pfnVBUSDetectCallback != (PWR_VBUSDetectCallbackFunc *)NULL)
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
      adc_awd_config.LowThreshold  = 0;
    }

    /* Update AWD Low/High thresholds to detect VBUS disconnection */
    adc_awd_config.WatchdogNumber = ADC_ANALOGWATCHDOG_1;
    adc_awd_config.WatchdogMode   = ADC_ANALOGWATCHDOG_SINGLE_REG;
    adc_awd_config.Channel        = ADCx_CHANNEL_VSENSE_1;
    adc_awd_config.ITMode         = ENABLE;

    /* Update AWD configuration */
    HAL_ADC_AnalogWDGConfig(hadc, &adc_awd_config);

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
  ADC_AnalogWDGConfTypeDef adc_awd_config = {0};

  if (Context.PortInfo[TYPE_C_PORT_2].pfnVBUSDetectCallback != (PWR_VBUSDetectCallbackFunc *)NULL)
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
      adc_awd_config.LowThreshold  = 0;
    }

    /* Update AWD2 Low/High thresholds to detect next VBUS connection/disconnection */
    adc_awd_config.WatchdogNumber = ADC_ANALOGWATCHDOG_2;
    adc_awd_config.WatchdogMode   = ADC_ANALOGWATCHDOG_SINGLE_REG;
    adc_awd_config.Channel        = ADCx_CHANNEL_VSENSE_2;
    adc_awd_config.ITMode         = ENABLE;

    /* Update AWD2 configuration */
    HAL_ADC_AnalogWDGConfig(hadc, &adc_awd_config);

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
  RCC_PeriphCLKInitTypeDef clk_config = {0};
  GPIO_InitTypeDef         gpio_config = {0};

  /* Select the PCLK clock as LPTIM peripheral clock */
  clk_config.PeriphClockSelection = RCC_PERIPHCLK_LPTIM1;
  clk_config.Lptim1ClockSelection = RCC_LPTIM1CLKSOURCE_PCLK1;
  HAL_RCCEx_PeriphCLKConfig(&clk_config);

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
  VBUSSensingCompleted = 1;
}

/**
  * @brief  Initialize VBUS voltage control (VBUS defaults at 5V)
  * @retval 0 success else fail
  */
static uint8_t PWR_InitPowerSource(void)
{
  uint8_t ret = 0;

  /* LPTIM1 initialization */
  hlptim.Instance = LPTIM1;
  hlptim.Init.Clock.Source    = LPTIM_CLOCKSOURCE_APBCLOCK_LPOSC;
  hlptim.Init.CounterSource   = LPTIM_COUNTERSOURCE_INTERNAL;
  hlptim.Init.Clock.Prescaler = LPTIM_PRESCALER_DIV1;
  hlptim.Init.Trigger.Source  = LPTIM_TRIGSOURCE_SOFTWARE;
  hlptim.Init.OutputPolarity  = LPTIM_OUTPUTPOLARITY_HIGH;
  hlptim.Init.UpdateMode      = LPTIM_UPDATE_IMMEDIATE;

  LPTIMx_MspDeInit(&hlptim);
  HAL_LPTIM_DeInit(&hlptim);
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
  HAL_LPTIM_DeInit(&hlptim);
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
  return READ_REG(hlptim.Instance->CMP);
}

/**
  * @brief  Precharge C57 .
  * @param  PortId Type-C port identifier
  *         This parameter can be take one of the following values:
  *         @arg TYPE_C_PORT_1
  * @retval Measured DCDC voltage
  */
static uint32_t PWR_DCDCPreChargeC57(uint32_t PortId)
{
  uint32_t voltage = 0;

  if (PortId == TYPE_C_PORT_1)
  {
    /* Make sure SOURCE_EN is not OFF */
    PWR_GPIO_Off(GPIO_SOURCE_EN);

    {
      GPIO_InitTypeDef  GPIO_InitStruct = {0};
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
       Wait C57 charge for 30ms... */
    HAL_Delay(30);
    PWR_GPIO_On(GPIO_V_CTL2); /* DCDC_EN = 1, so DCDC output is on */
    /* Wait C57 charge for 30ms... */
    HAL_Delay(30);
    voltage = PWR_DCDCGetVoltage(PortId);

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
    voltage = PWR_DCDCGetVoltage(PortId);
  }

  return voltage;
}

/**
 * @brief  Get actual DCDC voltage level.
 * @param  PortId Type-C port identifier
 *         This parameter can be take one of the following values:
 *         @arg TYPE_C_PORT_1
 * @retval DCDC voltage level (in mV)
 */
uint32_t PWR_DCDCGetVoltage(uint32_t PortId)
{
  uint32_t voltage = 0;

  /* DCDC measure is possible only if the port operates as a source */
  if (Context.PortInfo[PortId].Role != POWER_ROLE_SINK)
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
 * @brief  Set VBUS voltage level
 * @param  PortId Type-C port identifier
 *         This parameter can be take one of the following values:
 *         @arg TYPE_C_PORT_1
 *         @arg TYPE_C_PORT_2
 * @param  VBusInmV  VBUS voltage level setpoint(in mV)
 * @param  Calibration Calibration control
 *         This parameter can be take one of the following values:
 *         0: :calibration disabled
 *         1: :calibration enabled
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
static PWR_StatusTypeDef PWR_VBUSSetVoltage(uint32_t PortId,
                                         uint32_t VBusInmV,
                                                uint16_t Precision)
{
  PWR_StatusTypeDef _retr = PWR_OK;
  static uint16_t          duty_cycle;
  uint8_t           duty_calibrated = 0;
  uint32_t          vbus_measured;

  if (PortId == TYPE_C_PORT_1)
  {
    duty_cycle =  PWR_GetPWMDutyCycle();
    switch (VBusInmV)
    {
    case VBUS_VOLTAGE_0V_IN_MV:
      {
      duty_cycle    = 0x10;
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
      if (1 == duty_calibrated )
      {
        duty_cycle = aPWMDutyCycleLUT[VBUS_9_V];
      }
      break;
    case VBUS_VOLTAGE_15V_IN_MV:
      duty_calibrated = aPWMDutyCalibrated[VBUS_15_V];
      if (1 == duty_calibrated )
      {
        duty_cycle = aPWMDutyCycleLUT[VBUS_15_V];
      }
      break;
    case VBUS_VOLTAGE_18V_IN_MV:
      duty_cycle      = aPWMDutyCycleLUT[VBUS_18_V];
      if (1 == duty_calibrated )
      {
        duty_cycle = aPWMDutyCycleLUT[VBUS_18_V];
      }
      break;
    default:
      duty_cycle    = aPWMDutyCycleLUT[VBUS_5_V];
#if _PPS==USBPD_TRUE
      if (VBusInmV <= 5900)
      {
        duty_cycle      = aPWMDutyCycleLUT_PPS[(VBusInmV - 3300) / 20];
        duty_calibrated = aPWMDutyCalibrated_PPS[(VBusInmV - 3300) / 20];
      }
#endif /* _PPS */
      break;
    }


    if (duty_calibrated == 1)
    {
      uint32_t _tickstart = HAL_GetTick();
      PWR_SetPWMDutyCycle(duty_cycle);

      /* Check if the voltage is at the expected level */
      do
      {
          vbus_measured  = BSP_PWR_VBUSGetVoltage(PortId);
          for(int32_t index=0; index < 2000; index++){ __DSB();};
          /* timeout to avoid infinite loop in case of power default */
          if((HAL_GetTick() - _tickstart) > 300)
          {
            /* discard the VBUS and return an error */
            BSP_PWR_VBUSOff(PortId);
            return PWR_ERROR;
          }

     } while(ABS(vbus_measured -  VBusInmV)> 200);
    }
    else
    {
      int32_t           vbus_error;
      int32_t           vbus_previous;
      int32_t           pwm;
      int32_t           pwm_step_current;
      int32_t           pwm_step_previous=0;
      int32_t           pwm_error = 0;
      int32_t           pwm_error_previous;
      uint8_t            nb_pll_loop;
      pwm_step_previous = 0;
      pwm_step_current = duty_cycle;
      vbus_previous = vbus_measured = 0;
      nb_pll_loop = 0;

      do
      {
        pwm_step_previous = pwm_step_current;
        vbus_previous = vbus_measured;
        pwm_error_previous = pwm_error;
        pwm_error = 0;
        pwm = 0;

        PWR_SetPWMDutyCycle(duty_cycle);

        /* We can not avoid this DCDC takes time to lock .... */
        for(int32_t index=0; index < 7000; index++){ __DSB();};

        /* Measure actual VBUS voltage level */
        vbus_measured  = BSP_PWR_VBUSGetVoltage(PortId);
        /* Protection to handle the case where power is not present */
        if(vbus_measured < 300)
        {
          /* discard the VBUS and return an error */
          BSP_PWR_VBUSOff(PortId);
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
          else
          {
            /* Error case: Go back to 5V or 0V and break? */
          }
        }

        pwm_step_current = duty_cycle;

        if (    (ABS((int16_t)pwm_step_current-(int16_t)pwm_step_previous) <= BSP_PWR_DCDC_MIN_PWM_STEP)
             && ((ABS(((int32_t)vbus_measured - (int32_t)vbus_previous))) <= BSP_PWR_DCDC_PRECISION)
             && (ABS(vbus_error) <= Precision))

        {
          nb_pll_loop++;
        }
        else
        {
          nb_pll_loop = 0;
        }
      } while (nb_pll_loop < 1);

      /* save the PWM value for the next settings */
      switch (VBusInmV)
      {
      case VBUS_VOLTAGE_0V_IN_MV:
        break;
      case VBUS_VOLTAGE_5V_IN_MV:
        aPWMDutyCycleLUT[VBUS_5_V]  = duty_cycle;
        aPWMDutyCalibrated[VBUS_5_V] = 1;
        break;
      case VBUS_VOLTAGE_9V_IN_MV:
        aPWMDutyCycleLUT[VBUS_9_V]  = duty_cycle;
        aPWMDutyCalibrated[VBUS_9_V] = 1;
        break;
      case VBUS_VOLTAGE_15V_IN_MV:
        aPWMDutyCycleLUT[VBUS_15_V] = duty_cycle;
        aPWMDutyCalibrated[VBUS_15_V] = 1;
        break;
      case VBUS_VOLTAGE_18V_IN_MV:
        aPWMDutyCycleLUT[VBUS_18_V] = duty_cycle;
        aPWMDutyCalibrated[VBUS_18_V] = 1;
        break;
      default:
        aPWMDutyCycleLUT[VBUS_PPS_V] = duty_cycle;
#if _PPS==USBPD_TRUE
        if (VBusInmV <= 5900)
        {
          int32_t _diff_duty_cycle;
          uint32_t _index;
          _diff_duty_cycle = duty_cycle - aPWMDutyCycleLUT_PPS[(VBusInmV - 3300) / 20];
          /* Calibrate the other values */
          for (_index = 0; _index < PWM_MAX_VALUES_LUT_PPS; _index++)
            aPWMDutyCycleLUT_PPS[_index] += _diff_duty_cycle;
          aPWMDutyCycleLUT_PPS[(VBusInmV - 3300) / 20]   = duty_cycle;
          aPWMDutyCalibrated_PPS[(VBusInmV - 3300) / 20] = duty_calibrated;
        }
#endif /* _PPS */
        break;
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
 * @brief  Force the VBUS discharge.
 * @param  PortId Type-C port identifier
 *         This parameter can be take one of the following values:
 *         @arg TYPE_C_PORT_1
 * @param  VbusStopInmV Low VBUS threshold (in mV)
 * @param  VbusErrorInmV VBUS margin (in mV)
 * @param  Delay Discharge delay (in ms)
 * @retval None
 */
void PWR_VBUSDischarge(uint32_t PortId,
                           uint32_t VbusStopInmV,
                           uint32_t VbusErrorInmV,
                           uint16_t Delay)
{
/* USER CODE BEGIN BSP_PWR_VBUSDischarge */
  uint32_t vbus;
  uint8_t vbus_disable = 0;

  if (PortId == TYPE_C_PORT_1)
  {

    PWR_GPIO_On(GPIO_VBUS_DISCHARGE1);
    /* Disable DCDC_EN bit*/
    PWR_GPIO_Off(GPIO_V_CTL2);

    /* Wait Tempo or Voltage level */
    if (Delay > 0)
    {
      HAL_Delay(Delay);
    }
    else /* if (VbusErrorInmV > 0) */
    {

      do{
        HAL_Delay(1);
        vbus = BSP_PWR_VBUSGetVoltage(PortId);
        if ((0 == vbus_disable) && (vbus < 5000))
        {
          /* Switch off VBUS */
          PWR_GPIO_Off(GPIO_SOURCE_EN);
          vbus_disable = 1;
        }
      } while(vbus >= (VbusStopInmV + VbusErrorInmV));
    }

    /* Enable DCDC_EN bit*/
    PWR_GPIO_On(GPIO_V_CTL2);
    PWR_GPIO_Off(GPIO_VBUS_DISCHARGE1);
    /* Allow to restart with correct duty cycle */
    PWR_SetPWMDutyCycle(aPWMDutyCycleLUT[VBUS_5_V]);
  }
/* USER CODE END BSP_PWR_VBUSDischarge */
}

/* USER CODE END POWER_Private_Functions */

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

