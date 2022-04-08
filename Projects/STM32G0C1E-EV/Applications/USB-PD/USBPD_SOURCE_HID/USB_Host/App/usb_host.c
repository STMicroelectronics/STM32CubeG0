/**
  ******************************************************************************
  * @file    USB_Host/HID_RTOS/Src/usb_host.c
  * @author  MCD Application Team
  * @brief   This file implements Application Functions
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2020-2021 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------ */
#include "main.h"

/* Private typedef ----------------------------------------------------------- */
/* Private define ------------------------------------------------------------ */
/* Private macro ------------------------------------------------------------- */
/* Private variables --------------------------------------------------------- */
__IO HID_APP_State hid_app_state;
__IO uint32_t OsStatus = 0;
/* Exported variables --------------------------------------------------------- */
extern USBH_HandleTypeDef hUsbHostFS;
/** USER CODE BEGIN1 **/

/* Definitions for osSemaphore */
osSemaphoreId AppEvent;

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes ----------------------------------------------- */
extern void HID_MOUSE_App(USBH_HandleTypeDef *phost);
void USBH_HID_AppProcess(void const * argument);
extern void HID_KEYBRD_App(USBH_HandleTypeDef *phost);
/* Private functions --------------------------------------------------------- */

/**
  * @brief  Demo state machine.
  * @param  None
  * @retval None
  */
void HID_AppInit(void)
{
  /* Create Menu Semaphore */
  osSemaphoreDef(osSem);

  AppEvent = osSemaphoreCreate(osSemaphore(osSem), 1);

  /* Force menu to show Item 0 by default */
  osSemaphoreRelease(AppEvent);
   
  osThreadDef(Menu_Thread, USBH_HID_AppProcess, osPriorityHigh, 0,
              4 * configMINIMAL_STACK_SIZE);
  osThreadCreate(osThread(Menu_Thread), NULL);
}

/**
  * @brief  Updates the app state .
  * @param  None
  * @retval None
  */
void HID_UpdateApp(void)
{
  /* Force Sem to show Item 0 by default */
  hid_app_state = HID_APP_WAIT;
  OsStatus = osSemaphoreRelease(AppEvent);
  
}

/**
  * @brief  User task
  * @param  pvParameters not used
  * @retval None
  */
void USBH_HID_AppProcess(void const * argument)
{
  for (;;)
  {
    if (osSemaphoreWait(AppEvent, osWaitForever) == osOK)
    {
      switch(hid_app_state)
      {
        case HID_APP_WAIT:
          if(Appli_state == APPLICATION_READY)
          {
            if(USBH_HID_GetDeviceType(&hUsbHostFS) == HID_KEYBOARD)
            {
              hid_app_state = HID_APP_KEYBOARD;
              USBH_UsrLog("Use Keyboard to tape characters:");
              OsStatus = osSemaphoreRelease(AppEvent);
            }
            else if(USBH_HID_GetDeviceType(&hUsbHostFS) == HID_MOUSE)
            {
              hid_app_state = HID_APP_MOUSE;
              USBH_UsrLog("USB HID Host Mouse App...");
              OsStatus = osSemaphoreRelease(AppEvent);
            }
          }
          break;

      case HID_APP_MOUSE:
        if(Appli_state == APPLICATION_READY)
        {
          HID_MOUSE_App(&hUsbHostFS);
        }
        break;

      case HID_APP_KEYBOARD:
        if(Appli_state == APPLICATION_READY)
        {
          HID_KEYBRD_App(&hUsbHostFS);
        }
        break;

      default:
        break;
      }

      if(Appli_state == APPLICATION_DISCONNECT)
      {
        Appli_state = APPLICATION_IDLE;
        USBH_ErrLog("USB device disconnected !!! \n");
        hid_app_state = HID_APP_WAIT;
      }
    }
  }
}

/**
  * @brief  The function is a callback about HID Data events
  * @param  phost: Selected device
  * @retval None
  */
void USBH_HID_EventCallback(USBH_HandleTypeDef * phost)
{
  osSemaphoreRelease(AppEvent);
}
