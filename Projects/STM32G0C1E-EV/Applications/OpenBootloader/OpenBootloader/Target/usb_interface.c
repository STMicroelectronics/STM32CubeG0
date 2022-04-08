/**
  ******************************************************************************
  * @file    usb_interface.c
  * @author  MCD Application Team
  * @brief   Contains USB protocol commands
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

/* Includes ------------------------------------------------------------------*/
#include "usb_interface.h"
#include "usbd_core.h"
#include "stm32g0xx.h"
#include "stm32g0xx_hal.h"
#include "usbd_def.h"
#include "usbd_dfu.h"
#include "usbd_ioreq.h"
#include "usbd_dfu_if.h"
#include "usbd_desc.h"
#include "usb_device.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Exported variables --------------------------------------------------------*/

uint8_t USB_Detection = 0;

/* Private function prototypes -----------------------------------------------*/
/* Exported functions --------------------------------------------------------*/

/**
 * @brief  This function is used to configure USB pins and then initialize the used USB instance.
 * @retval None.
 */
void OPENBL_USB_Configuration(void)
{
  HAL_PWREx_EnableVddUSB();
  /* Init USB device Library, add supported class and start the library */
  MX_USB_Device_Init();
}

/**
 * @brief  This function is used to detect if there is any activity on USB protocol.
 * @retval None.
 */
uint8_t OPENBL_USB_ProtocolDetection(void)
{
  uint8_t detected;

  if (USB_Detection == 1)
  {
    detected = 1;
  }
  else
  {
    detected = 0;
  }

  return detected;
}

/**
  * @brief  This function is used to send a nack when the address is not valid by changing
  * the state of the usb to dfu_error.
  * @retval Returns USBD_FAIL.
  */
uint16_t OPENBL_USB_SendAddressNack(USBD_HandleTypeDef *pDev)
{
  USBD_DFU_HandleTypeDef *hdfu;
  hdfu = (USBD_DFU_HandleTypeDef *)pDev->pClassData;
  if (hdfu->dev_state == DFU_STATE_DNLOAD_BUSY)
  {
    hdfu->dev_state = DFU_ERROR_TARGET;
    hdfu->dev_status[0] = DFU_ERROR_TARGET;
    hdfu->dev_status[1] = 0;
    hdfu->dev_status[2] = 0;
    hdfu->dev_status[3] = 0;
    hdfu->dev_status[4] = DFU_STATE_ERROR;
    hdfu->dev_status[5] = 0;
  }
  return USBD_FAIL;
}

/**
  * @brief  This function is used to send a nack when the RDP level of download command is egale
  * to 1 by changing the state of the usb to dfu_error.
  * @retval Returns USBD_FAIL.
  */
uint16_t OPENBL_USB_DnloadRdpNack(USBD_HandleTypeDef *pDev)
{
  USBD_DFU_HandleTypeDef *hdfu;
  hdfu = (USBD_DFU_HandleTypeDef *)pDev->pClassData;
  if (hdfu->dev_state == DFU_STATE_DNLOAD_BUSY)
  {
    hdfu->dev_state = DFU_ERROR_VENDOR;
    hdfu->dev_status[0] = DFU_ERROR_VENDOR;
    hdfu->dev_status[1] = 0;
    hdfu->dev_status[2] = 0;
    hdfu->dev_status[3] = 0;
    hdfu->dev_status[4] = DFU_STATE_ERROR;
    hdfu->dev_status[5] = 0;
  }
  return (uint8_t)USBD_FAIL;
}

/**
  * @brief  This function is used to send a nack when the RDP level of upload command is egale
  * to 1 by changing the state of the usb to dfu_error.
  * @retval Returns USBD_FAIL.
  */
void OPENBL_USB_UploadRdpNack(USBD_HandleTypeDef *pDev)
{
  USBD_DFU_HandleTypeDef *hdfu;
  hdfu = (USBD_DFU_HandleTypeDef *)pDev->pClassData;

  if ((hdfu->dev_state == DFU_STATE_IDLE) || (hdfu->dev_state == DFU_STATE_UPLOAD_IDLE))
  {
    if (hdfu->wblock_num > 1U)
    {
      /* Call the error management function (command will be nacked */
      hdfu->dev_state = DFU_ERROR_VENDOR;
      hdfu->dev_status[0] = DFU_ERROR_VENDOR;
      hdfu->dev_status[1] = 0;
      hdfu->dev_status[2] = 0;
      hdfu->dev_status[3] = 0;
      hdfu->dev_status[4] = DFU_STATE_ERROR;
      hdfu->dev_status[5] = 0;
      USBD_CtlSendData(pDev, NULL, 0x0);
    }
  }
}
