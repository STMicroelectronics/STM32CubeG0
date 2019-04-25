/**
  ******************************************************************************
  * @file    app_thermometer_ldr.c
  * @author  MCD Application Team
  * @brief   Thermometer application implementation.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2018 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under Ultimate Liberty license
  * SLA0044, the "License"; You may not use this file except in compliance with
  * the License. You may obtain a copy of the License at:
  *                             www.st.com/SLA0044
  *
  ******************************************************************************
  */
#define __APP_THERMOMETER_C

/* Includes ------------------------------------------------------------------*/
#include "stm32g081b_eval_lcd.h"
#include "stm32g081b_eval_tsensor.h"
#include "k_config.h"
#include "k_module.h"
#include "k_menu.h"
#include "k_storage.h"

/* Private typedef ----------------------------------------------------------*/    
/* Private constants ----------------------------------------------------------*/

/* Private function prototypes -----------------------------------------------*/
KMODULE_RETURN _ThermometerLDRDemoExec(void);
KMODULE_RETURN _ThermometerConfig(void);
KMODULE_RETURN _LightDependentResistorConfig(void);

void ThermometerLDRDemo(void);
void ThermometerLDRMenuUserAction(uint8_t sel);
void ThermometerLDRUserHeader(void);
void ThermometerUserInformation(float tempvalue);
void LDRUserInformation(uint16_t LightDisplay);

/* Private Variable ----------------------------------------------------------*/
const tMenuItem ThermometerLDRMenuMenuItems[] =
{
    {"Press down to exit", 14, 30, TYPE_EXEC, MODULE_NONE, ThermometerLDRDemo, ThermometerLDRMenuUserAction, NULL, NULL, NULL  },
};

const tMenu ThermometerLDRMenu = {
  "Temperature/Light", ThermometerLDRMenuMenuItems, countof(ThermometerLDRMenuMenuItems), TYPE_EXEC, 1, 1 };

/* used to exit application */
static __IO uint8_t user_stop=0;

/* Daughter board identifier */
DBId_TypeDef DBId = DB_ID_NONE;
  

/* Private typedef -----------------------------------------------------------*/
/* External variables --------------------------------------------------------*/
const K_ModuleItem_Typedef ModuleTsensor =
{
  MODULE_TSENSOR,
  _ThermometerConfig,
  _LightDependentResistorConfig,
  _ThermometerLDRDemoExec,
  NULL
};  

ADC_HandleTypeDef    AdcHandle;

/**
  * @brief  setup the HW for the 8 uart application 
  * @param  None.
  * @note   set the memeory + Hw initialisation.  
  * @retval None.
  */
KMODULE_RETURN _LightDependentResistorConfig(void)
{
  ADC_ChannelConfTypeDef sConfig;
  
  if (DBId == DB_ID_LEGACY)
  {
    /*##-1- Configure the ADC peripheral #######################################*/
    AdcHandle.Instance          = ADC1;
    
    if (HAL_ADC_DeInit(&AdcHandle) != HAL_OK)
    {
      /* ADC de-initialization Error */
      return KMODULE_ERROR_EXEC;
    }
    
    AdcHandle.Init.ClockPrescaler        = ADC_CLOCK_SYNC_PCLK_DIV4;
    AdcHandle.Init.Resolution            = ADC_RESOLUTION_12B;
    AdcHandle.Init.DataAlign             = ADC_DATAALIGN_RIGHT;
    AdcHandle.Init.ScanConvMode          = DISABLE;                       /* Sequencer disabled (ADC conversion on only 1 channel: channel set on rank 1) */
    AdcHandle.Init.EOCSelection          = ADC_EOC_SINGLE_CONV;
    AdcHandle.Init.LowPowerAutoWait      = DISABLE;
    AdcHandle.Init.LowPowerAutoPowerOff  = DISABLE;
    AdcHandle.Init.ContinuousConvMode    = ENABLE;                       /* Continuous mode disabled to have only 1 conversion at each conversion trig */
    AdcHandle.Init.NbrOfConversion       = 1;
    AdcHandle.Init.DiscontinuousConvMode = DISABLE;                       /* Parameter discarded because sequencer is disabled */
    AdcHandle.Init.ExternalTrigConv      = ADC_SOFTWARE_START;            /* Software start to trig the 1st conversion manually, without external event */
    AdcHandle.Init.ExternalTrigConvEdge  = ADC_EXTERNALTRIGCONVEDGE_NONE;
    AdcHandle.Init.DMAContinuousRequests = ENABLE;
    AdcHandle.Init.Overrun               = ADC_OVR_DATA_OVERWRITTEN;
    AdcHandle.Init.SamplingTimeCommon1   = ADC_SAMPLETIME_19CYCLES_5;
    AdcHandle.Init.OversamplingMode      = DISABLE;
    
    if (HAL_ADC_Init(&AdcHandle) != HAL_OK)
    {
      /* ADC initialization Error */
      return KMODULE_ERROR_EXEC;
    }
    
    /*##-2- Configure ADC regular channel ######################################*/
    sConfig.Channel      = ADC_CHANNEL_1; 
    sConfig.Rank         = ADC_REGULAR_RANK_1;
    sConfig.SamplingTime = ADC_SAMPLINGTIME_COMMON_1;
    
    
    if (HAL_ADC_ConfigChannel(&AdcHandle, &sConfig) != HAL_OK)
    {
      /* Channel Configuration Error */
      return KMODULE_ERROR_EXEC;
    }
    
    /*##-3- Start the conversion process #######################################*/
    if (HAL_ADC_Start(&AdcHandle) != HAL_OK)
    {
      /* Start Conversation Error */
      return KMODULE_ERROR_EXEC;
    }
    
    /* Wait for the first convertion */
    HAL_ADC_PollForConversion(&AdcHandle, 10);
  }
  
  return KMODULE_OK;
}

/**
  * @brief  setup the HW for the 8 uart application 
  * @param  None.
  * @note   set the memeory + Hw initialisation.  
  * @retval None.
  */
KMODULE_RETURN _ThermometerConfig(void)
{
  BSP_DB_GetId(&DBId);
  
  if (DBId == DB_ID_LEGACY)
  {
    BSP_TSENSOR_Init();
    _LightDependentResistorConfig();
  }

  return KMODULE_OK;
}

/**
  * @brief  Run the 8 uart application 
  * @param  None.
  * @note   run and display information about the uart transaction.  
  * @retval None.
  */
KMODULE_RETURN _ThermometerLDRDemoExec(void)
{
  /* Prepare the main MMI */
  BSP_LCD_Clear(LCD_COLOR_WHITE);
  kMenu_Execute(ThermometerLDRMenu);
  
  /* Execute the app 8uart */
  /* App initialization    */
  return KMODULE_OK;
}


void ThermometerLDRDemo(void)
{
  float    TempValueNew = 0;
  float    TempValueOld = 0;  
  uint16_t LDRValueNew  = 0;
  uint16_t LDRValueOld  = 0;
  
  if (DBId == DB_ID_LEGACY)
  {
    /* Display user information */
    ThermometerLDRUserHeader();
    
    /* Get first temperature value */     
    TempValueNew = BSP_TSENSOR_ReadTemp();
    
    /* Display first Temperature Informations */
    ThermometerUserInformation(TempValueNew);
    
    /* Init Old Temp value at first start */
    TempValueOld = TempValueNew;
    
    /* Get LDR value */           
    LDRValueNew = ((4095- (uint16_t)HAL_ADC_GetValue(&AdcHandle))/40);      
    
    /* Display Temperature Informations */      
    LDRUserInformation(LDRValueNew);
    
    /* Init Old LDR value at first start */
    LDRValueNew = LDRValueOld;
    
    /* Display temperature until user event */
    user_stop = 0;
    while(user_stop == 0) 
    {
      /* Get temperature value */     
      TempValueNew = BSP_TSENSOR_ReadTemp();
      
      if(TempValueNew != TempValueOld)
      {
        /* ReInit Old Temp value */
        TempValueOld = TempValueNew;
        
        /* Display Temperature Informations */
        ThermometerUserInformation(TempValueNew);
      }
      
      /* Get LDR value */           
      LDRValueNew = ((4095- (uint16_t)HAL_ADC_GetValue(&AdcHandle))/40);      
      
      if(LDRValueNew != LDRValueOld)
      {
        /* ReInit Old LDR value */
        LDRValueOld = LDRValueNew;
        
        /* Display Temperature Informations */      
        LDRUserInformation(LDRValueNew);
      }
    }
  }
  else
  {
    /* Set the Back Color */
    BSP_LCD_SetBackColor(LCD_COLOR_WHITE);
    
    /* Set the Text Color */
    BSP_LCD_SetTextColor(LCD_COLOR_ST_PINK);
    
    BSP_LCD_SetFont(&Font24);
    BSP_LCD_DisplayStringAt(1, LINE(2), (uint8_t*)"CAUTION:", CENTER_MODE);  
    BSP_LCD_DisplayStringAt(1, LINE(4), (uint8_t*)"Please mount", CENTER_MODE);  
    BSP_LCD_DisplayStringAt(1, LINE(5), (uint8_t*)"MB1351 to run", CENTER_MODE);  
    BSP_LCD_DisplayStringAt(1, LINE(6), (uint8_t*)"this demo!", CENTER_MODE);  
    
    user_stop = 0;
    while(user_stop == 0);
  }
}

/**
  * @brief  Display uart transaction state 
  * @param  None.
  * @note   This display information about the uart transaction.  
  * @retval None.
  */
void ThermometerLDRUserHeader(void)
{
  /* Display the Thermometer icon */
  kStorage_OpenFileDrawPixel(28, 6, (uint8_t *)"STFILES/ICON15.bmp");

  /* Set default font */
  BSP_LCD_SetFont(&Font20);
    
  /* Set the Back Color */
  BSP_LCD_SetBackColor(LCD_COLOR_WHITE);

  /* Set the Text Color */
  BSP_LCD_SetTextColor(LCD_COLOR_ST_PINK);

  /* Set LDR comment */  
  BSP_LCD_DisplayStringAt(140,LINE(8), (uint8_t *)"LDR:", LEFT_MODE);
}  


/**
  * @brief  Display uart transaction state 
  * @param  None.
  * @note   This display information about the uart transaction.  
  * @retval None.
  */
void ThermometerUserInformation(float temperature)
{
  float    TempCelsiusDisplay = 0;
  float    TempFahrenheitDisplay = 0;
  uint8_t  LCDStrCelsius[8];
  uint8_t  LCDStrFahrenheit[8];
  uint16_t ThermometerDisplay = 0;

  /* Thermometer update information *******************************************/
  if ((uint16_t)temperature & 0x8000)
  {
    /* Display negative temperature */
    BSP_LCD_SetTextColor(LCD_COLOR_ST_BLUE);
    TempCelsiusDisplay = -(((uint16_t)temperature & 0x7FFF)>>1);
    if ((uint16_t)temperature & 0x0001)
    {
      TempCelsiusDisplay += 0.5;
    }
  }
  else
  {
    /* Display positive temperature */
    BSP_LCD_SetTextColor(LCD_COLOR_ST_PINK);
    TempCelsiusDisplay = (((uint16_t)temperature & 0x7FFF)>>1);
    if ((uint16_t)temperature & 0x0001)
    {
      TempCelsiusDisplay += 0.5;
    }
  }
  /* Convert Temperature in Celsius to temperature in Fahrenheit */
  TempFahrenheitDisplay = (1.8 * TempCelsiusDisplay) + 32;

  sprintf((char*)LCDStrCelsius,    "%5.1f C", TempCelsiusDisplay);
  sprintf((char*)LCDStrFahrenheit, " %2.1f F", TempFahrenheitDisplay);  
  BSP_LCD_DisplayStringAt(64,LINE(2), (uint8_t*)LCDStrCelsius, LEFT_MODE);
  BSP_LCD_DisplayStringAt(64,LINE(3), (uint8_t*)LCDStrFahrenheit, LEFT_MODE);
  
  ThermometerDisplay = (uint16_t)TempFahrenheitDisplay;

  /* Dynamical thermmeter is available only in range 57 to 86 Fahrenheit Degres*/
  if((ThermometerDisplay> 57)&&(ThermometerDisplay < 86))
  {
    /* Set the LCD White Color */
    BSP_LCD_SetTextColor(LCD_COLOR_WHITE);
    BSP_LCD_FillRect(26, 40,  9, 141-((ThermometerDisplay-57)*5));
    
    BSP_LCD_SetTextColor(LCD_COLOR_ST_PINK);
    BSP_LCD_FillRect(26, 181-((ThermometerDisplay-57)*5), 9 ,((ThermometerDisplay-57)*5) );
  }
}  


/**
  * @brief  Display LDR Icon and percentage 
  * @param  LightDisplay (percentage).
  * @note   This display icon related to LDR information.  
  * @retval None.
  */
void LDRUserInformation(uint16_t LightDisplay)
{
  uint8_t  LDRStrToDraw[6];
  
  /* Light Dependent Resistor (LDR) update information ************************/
  if(LightDisplay > 86)
  {
      kStorage_OpenFileDrawPixel(120, 210, (uint8_t *)"STFILES/LDR10.bmp");
  }
  else if(LightDisplay > 72)
  {    
      kStorage_OpenFileDrawPixel(120, 210, (uint8_t *)"STFILES/LDR9.bmp");
  }
  else if(LightDisplay > 64)
  {    
      kStorage_OpenFileDrawPixel(120, 210, (uint8_t *)"STFILES/LDR8.bmp");
  }
  else if(LightDisplay > 52)
  {    
      kStorage_OpenFileDrawPixel(120, 210, (uint8_t *)"STFILES/LDR7.bmp");
  }
  else if(LightDisplay > 42)
  {    
      kStorage_OpenFileDrawPixel(120, 210, (uint8_t *)"STFILES/LDR6.bmp");
  }
  else if(LightDisplay > 32)
  {    
      kStorage_OpenFileDrawPixel(120, 210, (uint8_t *)"STFILES/LDR5.bmp");
  }
  else if(LightDisplay > 24)
  {    
      kStorage_OpenFileDrawPixel(120, 210, (uint8_t *)"STFILES/LDR4.bmp");
  }
  else if(LightDisplay > 16)
  {    
      kStorage_OpenFileDrawPixel(120, 210, (uint8_t *)"STFILES/LDR3.bmp");
  }
  else if(LightDisplay > 10)
  {    
      kStorage_OpenFileDrawPixel(120, 210, (uint8_t *)"STFILES/LDR2.bmp");
  }
  else if(LightDisplay > 4)
  {    
      kStorage_OpenFileDrawPixel(120, 210, (uint8_t *)"STFILES/LDR1.bmp");
  }
  else if(LightDisplay <= 8)
  {    
      kStorage_OpenFileDrawPixel(120, 210, (uint8_t *)"STFILES/LDR0.bmp");
  }

  sprintf((char*)LDRStrToDraw, "%3d", LightDisplay);
  BSP_LCD_DisplayStringAt(140,LINE(9), (uint8_t*)LDRStrToDraw, LEFT_MODE);  
}
  
/**
  * @brief  Get User action 
  * @param  sel : User selection (JOY_SEL,...)
  * @note   This example is the only way to get user information.  
  * @retval None
  */
void ThermometerLDRMenuUserAction(uint8_t sel)
{
  switch(sel)
  {
  case JOY_DOWN : 
       user_stop = 1;
    break;
  default :
    break;
  }
}

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
