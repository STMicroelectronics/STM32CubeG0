/**
  ******************************************************************************
  * @file    usbpd_usb_if.c
  * @author  MCD Application Team
  * @brief   This file contains the usb if functions.
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

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "usbpd_usb_if.h"
#include "usbpd_trace.h"
#include "usb_host.h"
#include "usbh_core.h"
#include "usbpd.h"
/* USB include files ----------------------------------------------------------*/

/** @addtogroup STM32_USBPD_USBIF
  * @{
  */

/* Private enums -------------------------------------------------------------*/
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Exported functions ---------------------------------------------------------*/
extern USBH_HandleTypeDef hUsbHostFS;
/**
  * @brief  Initialize billboard driver
  * @param  None
  * @retval status
  */
int32_t USBPD_USBIF_Init(void)
{
  USBPD_TRACE_Add(USBPD_TRACE_DEBUG, 0, 0, (uint8_t *)"USB_IF init", 11);
  return 0;
}


/** @addtogroup USBPD_CORE_USBIF_Exported_Functions
  * @{
  */
void USBPD_USBIF_DeviceStart(uint32_t PortNum)
{
}

void USBPD_USBIF_DeviceStop(uint32_t PortNum)
{
  USBPD_TRACE_Add(USBPD_TRACE_DEBUG, PortNum, 0,  (uint8_t *)"USBIF Device stop", 16);
}

void USBPD_USBIF_HostStart(uint32_t PortNum)
{
  USBPD_TRACE_Add(USBPD_TRACE_DEBUG, PortNum, 0, (uint8_t *) "USBIF host start", 16);
  /* Start Host Process */
  MX_USB_HOST_Init();
}

void USBPD_USBIF_HostStop(uint32_t PortNum)
{
  USBPD_TRACE_Add(USBPD_TRACE_DEBUG, PortNum, 0, (uint8_t *) "USBIF host stop", 14);
  USBH_Stop(&hUsbHostFS);
  USBH_DeInit(&hUsbHostFS);
}

void USBPD_USBIF_DeviceBillboard(uint32_t PortNum)
{
  USBPD_TRACE_Add(USBPD_TRACE_DEBUG, PortNum, 0,  (uint8_t *)"USBIF device billboard", 22);
}

void USBPD_USBIF_Swap2Host(uint32_t PortNum)
{
  USBPD_TRACE_Add(USBPD_TRACE_DEBUG, PortNum, 0,  (uint8_t *)"USBIF swap to host", 18);
}

void USBPD_USBIF_Swap2Device(uint32_t PortNum)
{
  USBPD_TRACE_Add(USBPD_TRACE_DEBUG, PortNum, 0,  (uint8_t *)"USBIF swap to device", 20);
}

void USBPD_USBIF_DeviceSetBOSInfo(uint32_t PortNum, void *DataPtr)
{
  USBPD_TRACE_Add(USBPD_TRACE_DEBUG, PortNum, 0,  (uint8_t *)"USBIF set BOS info", 18);
}

void USBPD_USBIF_DeviceSetVDMInfo(uint32_t PortNum, void *DataPtr)
{
  USBPD_TRACE_Add(USBPD_TRACE_DEBUG, PortNum, 0,  (uint8_t *)"USBIF set VDM info", 18);
}

/**
  * @}
  */

/** @addtogroup USBPD_CORE_USBIF_Private_Functions
  * @{
  */

/**
  * @}
  */
