/**
  ******************************************************************************
  * @file    app_lowpower.c
  * @author  MCD Application Team
  * @brief   Low power application implementation.
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
#ifndef __APP_LOWPOWER_C
#define __APP_LOWPOWER_C

/* Includes ------------------------------------------------------------------*/
#include <string.h>
#include "app_lowpower.h"
#include "main.h"
#include "stm32g0xx_hal.h"
#include "stm32g081b_eval_lcd.h"
#include "k_config.h"
#include "k_module.h"
#include "k_menu.h"
#include "k_window.h"

/* Private typedef ----------------------------------------------------------*/    
/* Private constants ----------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
KMODULE_RETURN _LowPowerDemoExec(void);
KMODULE_RETURN _LowPowerDemoConfig(void);
KMODULE_RETURN _LowPowerDemoUnConfig(void);

void LowPowerDemo(void);
void LowPowerUserAction(uint8_t sel);
void LowPowerStandbyWakeupPin(void);
void LowPowerStandbyRTCAlarm(void);
void LowPowerStopEXTI(void);
void LowPowerStopRTCAlarm(void);

static void LowPowerHandleAlarm(void);

/* Private Variable ----------------------------------------------------------*/
/* standby mode menu */
const tMenuItem StandbyModeMenuItems[] =
{
    {"Wakeup pin"    , 14, 30,    SEL_EXEC, MODULE_LOWPOWER, LowPowerStandbyWakeupPin, LowPowerUserAction, NULL, NULL, NULL  },
    {"RTC Alarm" , 14, 30,    SEL_EXEC, MODULE_LOWPOWER, LowPowerStandbyRTCAlarm, LowPowerUserAction, NULL, NULL, NULL  },
    {"RETURN"         ,  0,  0,    SEL_EXIT, MODULE_LOWPOWER, NULL, NULL, NULL, NULL, NULL  }
};

const tMenu StandbyModeMenu = {
  "Standby mode", StandbyModeMenuItems, countof(StandbyModeMenuItems), TYPE_TEXT, 1, 1
};


/* stop mode menu */
const tMenuItem StopModeMenuItems[] =
{
    {"EXTI pin"     , 14, 30,    SEL_EXEC, MODULE_LOWPOWER, LowPowerStopEXTI, LowPowerUserAction, NULL, NULL , NULL },
    {"RTC Alarm", 14, 30,    SEL_EXEC, MODULE_LOWPOWER, LowPowerStopRTCAlarm, LowPowerUserAction, NULL, NULL, NULL  },
    {"Return"        ,  0,  0,    SEL_EXIT, MODULE_LOWPOWER, NULL, NULL, NULL, NULL }
};

const tMenu StopModeMenu = {
  "Stop mode", StopModeMenuItems, countof(StopModeMenuItems), TYPE_TEXT, 1, 1
};


/* Main menu */
const tMenuItem LowPowerMenuItems[] =
{
    {"STOP mode"   , 14, 30, SEL_SUBMENU, MODULE_LOWPOWER, NULL, NULL, (const tMenu*)&StopModeMenu, NULL, NULL  },
    {"STANDBY mode", 14, 30, SEL_SUBMENU, MODULE_LOWPOWER, NULL, NULL, (const tMenu*)&StandbyModeMenu, NULL, NULL  },
    {"Return"      ,  0,  0, SEL_EXIT, MODULE_NONE, NULL, NULL, NULL, NULL }
};

const tMenu LowpowerMenu = {
  "Low power", LowPowerMenuItems, countof(LowPowerMenuItems), TYPE_TEXT, 1, 1};

/* used to exit application */
static __IO uint8_t user_event=0;
static __IO uint8_t user_action=0;

/* Private typedef -----------------------------------------------------------*/
/* External variables --------------------------------------------------------*/
const K_ModuleItem_Typedef ModuleLowPower =
{
  MODULE_LOWPOWER,
  _LowPowerDemoConfig,
  _LowPowerDemoExec,
  _LowPowerDemoUnConfig,
  NULL
};

/* RTC Handle variable */
extern RTC_HandleTypeDef RtcHandle;

/**
  * @brief  Configure the Lowpower application 
  * @param  None.
  * @note   run and display information about the lowpower feature.  
  * @retval None.
  */
KMODULE_RETURN _LowPowerDemoConfig(void)
{
  /*#### Disable all used wakeup sources ####*/
  HAL_RTC_DeactivateAlarm(&RtcHandle, RTC_ALARM_B);
  /* Disable all previous wake up interrupt */
  HAL_PWR_DisableWakeUpPin(PWR_WAKEUP_PIN1);

  /*#### Clear all related wakeup flags ####*/
  /* Clear the Alarm interrupt pending bit */
  __HAL_RTC_ALARM_CLEAR_FLAG(&RtcHandle,RTC_FLAG_ALRBF);
  /* Clear PWR wake up Flag */
  __HAL_PWR_CLEAR_FLAG(PWR_FLAG_WUF1 | PWR_FLAG_SB);

    return KMODULE_OK;
}

/**
  * @brief  un-Configure the Lowpower application 
  * @param  None.
  * @note   run and display information about the lowpower feature.
  * @retval None.
  */
KMODULE_RETURN _LowPowerDemoUnConfig(void)
{
  return KMODULE_OK;
}

/**
  * @brief  Run the Lowpower application 
  * @param  None.
  * @note   run and display information about the lowpower feature.  
  * @retval None.
  */
KMODULE_RETURN _LowPowerDemoExec(void)
{
  /* Prepare Execute the main MMI of lowpower application */
  kMenu_Execute(LowpowerMenu);
  return KMODULE_OK;
}

/**
  * @brief  Get User action 
  * @param  sel : User selection (JOY_SEL,...)
  * @note   This example is the only way to get user information.  
  * @retval None
  */
void LowPowerUserAction(uint8_t sel)
{
  if (user_action == 0)
  {
    user_action = 1;
    user_event = sel;
  }
}

/**
  * @brief  Run the Lowpower Standby mode Wakeup pin 
  * @param  None.
  * @note   run and display information about the lowpower feature.  
  * @retval None.
  */
void LowPowerStandbyWakeupPin(void)
{
  kWindow_Popup("STANDBY WAKEUP", LCD_COLOR_WHITE, LCD_COLOR_ST_BLUE,\
                "JOY sel will\nstart\nstandby mode\n",                  \
                LCD_COLOR_WHITE, LCD_COLOR_ST_BLUE );

  HAL_Delay(100);
  user_event = JOY_UP;
  while(user_event != JOY_SEL)
  {
    user_action = 0;
  };

  kWindow_Popup("", LCD_COLOR_WHITE, LCD_COLOR_WHITE,         \
                "\n\n\n\npress JOY SEL\nto exit\nand reset\n",\
                 LCD_COLOR_ST_BLUE, LCD_COLOR_WHITE );
  
  /* Enable WKUP pin */
  HAL_PWR_EnableWakeUpPin(PWR_WAKEUP_PIN1);
  
  /* Standby mode */
  HAL_PWR_EnterSTANDBYMode();
}

/**
  * @brief  Run the Lowpower Standby mode RTC Alarm
  * @param  None.
  * @note   run and display information about the lowpower feature.  
  * @retval None.
  */
void LowPowerStandbyRTCAlarm(void)
{
  kWindow_Popup("STANDBY Alarm", LCD_COLOR_WHITE, LCD_COLOR_ST_BLUE,\
                "\nset delay time\n",                            \
                LCD_COLOR_ST_BLUE, LCD_COLOR_WHITE );

  /* Set the alarm */
  LowPowerHandleAlarm();

  kWindow_Popup("", LCD_COLOR_WHITE, LCD_COLOR_WHITE, \
                "\n\nstandby mode\nstarted\nwait alarm\nto exit\nand reset\n", \
                LCD_COLOR_ST_BLUE, LCD_COLOR_WHITE );  

  /*#### Enter StandBy mode ####*/
  HAL_PWR_EnterSTANDBYMode();
  while(1);
}


/**
  * @brief  Enter in stop mode and exit by pressingf the tamper button
  * @param  None
  * @note   This example enter in stop mode.  
  * @retval None
  */
void LowPowerStopEXTI(void)
{
  kWindow_Popup("STOP EXTI", LCD_COLOR_WHITE, LCD_COLOR_ST_BLUE,\
                "\n\nPress JOY sel to\nstart\nstop mode\n",   \
                 LCD_COLOR_WHITE, LCD_COLOR_ST_BLUE );

  HAL_Delay(100);

  user_event = JOY_UP;
  while(user_event != JOY_SEL)
  {
    user_action = 0;
  };

  kWindow_Popup("STOP EXTI", LCD_COLOR_WHITE, LCD_COLOR_ST_BLUE,     \
                "\n\nstop mode\nstarted\npress tamper\nto exit\n",\
                 LCD_COLOR_ST_BLUE, LCD_COLOR_WHITE );  

  /* User push-button (EXTI_Line0) will be used to wakeup the system from STOP mode */
  BSP_PB_Init(BUTTON_TAMPER, BUTTON_MODE_EXTI);

  /* avoid wakeup from JOY event */
  BSP_JOY_DeInit();

  /* Clear PWR wake up Flag */
  __HAL_PWR_CLEAR_FLAG(PWR_FLAG_WUF);

  /* Enter Stop Mode */
  HAL_PWR_EnterSTOPMode(PWR_LOWPOWERREGULATOR_ON, PWR_STOPENTRY_WFI);

  /* Display info */
  LowPowerExitDisplay(STOP);

  /* Restore the clock configuration */
  SystemClock_Config();

  /* User push-button (EXTI_Line0) will be used to wakeup the system from STOP mode */
  BSP_PB_Init(BUTTON_TAMPER, BUTTON_MODE_GPIO);
  BSP_JOY_Init(JOY_MODE_EXTI);
}

/**
  * @brief  Enter in stop mode and exit by an alarm
  * @param  None
  * @note   This example enter in stop mode.  
  * @retval None
  */
void LowPowerStopRTCAlarm(void)
{
  kWindow_Popup("STOP Alarm", LCD_COLOR_WHITE, LCD_COLOR_ST_BLUE,\
                "\nset delay time\n",                          \
                LCD_COLOR_ST_BLUE, LCD_COLOR_WHITE ); 

  /* set the alarm */
  LowPowerHandleAlarm();

  kWindow_Popup("STOP Alarm", LCD_COLOR_WHITE, LCD_COLOR_ST_BLUE,  \
                "\n\nstop mode\nstarted\nwait alarm\nto exit\n",\
                LCD_COLOR_ST_BLUE, LCD_COLOR_WHITE );  

  /* De Init Joystick */
  BSP_JOY_DeInit();

  /* Enter Stop Mode */
  HAL_PWR_EnterSTOPMode(PWR_LOWPOWERREGULATOR_ON, PWR_STOPENTRY_WFI);

  /* Restore the clock configuration */
  SystemClock_Config();

  /* Display info */
  LowPowerExitDisplay(STOP);

  HAL_RTC_DeactivateAlarm(&RtcHandle, RTC_ALARM_B);

  /* Clear the Alarm interrupt pending bit */
  __HAL_RTC_ALARM_CLEAR_FLAG(&RtcHandle,RTC_FLAG_ALRBF);

  BSP_JOY_Init(JOY_MODE_EXTI);
}

/**
  * @brief  Get user info to setup an alarm
  * @param  RTC handle
  * @note   This function wait user info to setup the alarm.  
  * @retval None
  */
static void LowPowerHandleAlarm(void)
{
  enum {
    HOURS,
    MINUTES,
    SECONDS,
    END
  };

  RTC_DateTypeDef currentdate = {0};
  RTC_TimeTypeDef time = {0};
  RTC_TimeTypeDef currenttime = {0};
  RTC_AlarmTypeDef Alarm = {0};
  sFONT *font;
  uint8_t temp[16];
  uint8_t exit = 0;
  uint8_t index = SECONDS;
  uint8_t position;
  uint8_t statpos;

  /* get current font */
  font = BSP_LCD_GetFont();
  statpos = (BSP_LCD_GetXSize() >> 1) - (4 * font->Width);

  /* Get the alarm time from user */
  do
  {
    BSP_LCD_SetTextColor(LCD_COLOR_ST_BLUE);
    sprintf((char *)temp, "%.2d:%.2d:%.2d", time.Hours, time.Minutes, time.Seconds);
    BSP_LCD_DisplayStringAt(statpos, 5 * font->Height, temp, NO_MODE);
    BSP_LCD_SetTextColor(LCD_COLOR_ST_PINK);

    switch(index)
    {
    case HOURS :
      position = statpos;
      sprintf((char *)temp, "%.2d", time.Hours);
      break;
    case MINUTES:
      position = statpos + (3 * font->Width);
      sprintf((char *)temp, "%.2d", time.Minutes);
      break;
    case SECONDS :
      position = statpos + (6 * font->Width);
      sprintf((char *)temp, "%.2d", time.Seconds);
      break;
    }
    BSP_LCD_DisplayStringAt(position, 5 * font->Height, temp, NO_MODE);

    user_event = JOY_NONE;
    user_action = 0;
    while(user_event == JOY_NONE);
    switch(user_event)
    {
    case JOY_UP :
      if(index == HOURS)
      {
        if( time.Hours == 23 ) time.Hours = 0;
        else
          time.Hours++;
      }

      if(index == MINUTES)
      {
        if(time.Minutes == 59 ) time.Minutes= 0;
        else
          time.Minutes++;
      }
      if(index == SECONDS)
      {
        if(time.Seconds == 59 ) time.Seconds =0;
        else
          time.Seconds++;
      }
      break;
    case JOY_DOWN :
      if(index == HOURS)
      {
        if (time.Hours == 0 ) time.Hours = 23;
        else time.Hours--;
      }
      if(index == MINUTES)
      {
        if(time.Minutes == 0) time.Minutes=59;
        else 
          time.Minutes--;
      }
      if(index == SECONDS)
      {
        if(time.Seconds == 0) time.Seconds = 59;
        else
          time.Seconds--;
      }
      break;
    case JOY_RIGHT :
      if(index != SECONDS ) index++; 
      break;
    case JOY_LEFT :
      if(index != HOURS ) index--;
      break;
    case JOY_SEL :
      exit = 1;
      break;
    }
  } while(exit == 0);

  HAL_RTC_GetTime(&RtcHandle, &currenttime,  RTC_FORMAT_BIN);
  HAL_RTC_GetDate(&RtcHandle, &currentdate, RTC_FORMAT_BIN);
  time.SubSeconds = currenttime.SubSeconds;
  if((time.Seconds + currenttime.Seconds) > 60 )  time.Minutes++;
  time.Seconds = ((time.Seconds + currenttime.Seconds) % 60);

  if((time.Minutes + currenttime.Minutes) > 60 )  time.Hours++;
  time.Minutes = ((time.Minutes + currenttime.Minutes) % 60);

  time.Hours = ((time.Hours + currenttime.Hours) % 24);

  /* Set the alarm */
  Alarm.Alarm = RTC_ALARM_B;
  Alarm.AlarmDateWeekDay = RTC_WEEKDAY_MONDAY;
  Alarm.AlarmDateWeekDaySel = RTC_ALARMDATEWEEKDAYSEL_DATE;
  Alarm.AlarmMask = RTC_ALARMMASK_DATEWEEKDAY;
  Alarm.AlarmSubSecondMask = RTC_ALARMSUBSECONDMASK_ALL;
  Alarm.AlarmTime.TimeFormat = RTC_HOURFORMAT_24;
  Alarm.AlarmTime.Hours = time.Hours;
  Alarm.AlarmTime.Minutes = time.Minutes;
  Alarm.AlarmTime.Seconds = time.Seconds;
  if(HAL_RTC_SetAlarm_IT(&RtcHandle, &Alarm, RTC_FORMAT_BIN) != HAL_OK)
  {
    while(1);
  }
}


void LowPowerExitDisplay(uint32_t Mode)
{
  char msg[22] = "\n\nexit from\n";

  if(Mode == STANDBY)
  {
    strcat(msg, "STANDBY\n");
  }
  else
  {
    strcat(msg, "STOP\n");
  }

  kWindow_Popup("Low Power", LCD_COLOR_WHITE, LCD_COLOR_ST_BLUE,\
                msg, LCD_COLOR_ST_BLUE, LCD_COLOR_WHITE );

  HAL_Delay(500);
}

void HAL_RTCEx_AlarmBEventCallback(RTC_HandleTypeDef *hrtc)
{
  HAL_RTC_DeactivateAlarm(&RtcHandle, RTC_ALARM_B);
}


#undef __APP_LOWPOWER_C
#endif
/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
