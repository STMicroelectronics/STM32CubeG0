/**
  ******************************************************************************
  * @file           USB_Device/DFU_Standalone/USB_Device/App/usbd_dfu_if.c
  * @author         MCD Application Team
  * @brief          Memory management layer
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

/* Includes ------------------------------------------------------------------*/
#include "usbd_dfu_if.h"
#include "openbl_usb_cmd.h"
#include "usb_interface.h"
#include "openbl_mem.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/

#define MEDIA_DESC_STR      "@Internal Flash   /0x08000000/8*02Ka,248*02Kg"
#define MEDIA_ERASE_TIME    (uint16_t)500
#define MEDIA_PROGRAM_TIME  (uint16_t)500

extern USBD_HandleTypeDef hUsbDeviceFS;

static uint16_t USB_DFU_If_Init(void);
static uint16_t USB_DFU_If_Erase(uint32_t Add);
static uint16_t USB_DFU_If_Write(uint8_t *pSrc, uint8_t *pDest, uint32_t Len);
static uint8_t *USB_DFU_If_Read(uint8_t *pSrc, uint8_t *pDest, uint32_t Len);
static uint16_t USB_DFU_If_DeInit(void);
static uint16_t USB_DFU_If_GetStatus(uint32_t Add, uint8_t Cmd, uint8_t *pBuffer);

#if defined ( __ICCARM__ ) /* IAR Compiler */
#pragma data_alignment=4
#endif
__ALIGN_BEGIN USBD_DFU_MediaTypeDef USBD_DFU_Media_fops __ALIGN_END =
{
  (uint8_t *)MEDIA_DESC_STR,
  USB_DFU_If_Init,
  USB_DFU_If_DeInit,
  USB_DFU_If_Erase,
  USB_DFU_If_Write,
  USB_DFU_If_Read,
  USB_DFU_If_GetStatus
};

/**
  * @brief  Memory initialization routine.
  * @retval USBD_OK if operation is successful, MAL_FAIL else.
  */
uint16_t USB_DFU_If_Init(void)
{
  return 0;
}

/**
  * @brief  De-Initializes Memory
  * @retval USBD_OK if operation is successful, MAL_FAIL else
  */
uint16_t USB_DFU_If_DeInit(void)
{
  return 0;
}

/**
  * @brief  Erase sector.
  * @param  Add: Address of sector to be erased.
  * @retval 0 if operation is successful, MAL_FAIL else.
  */
uint16_t USB_DFU_If_Erase(uint32_t Add)
{
  uint8_t status;

  if (OPENBL_MEM_GetReadOutProtectionStatus() != RESET)
  {
    status = OPENBL_USB_DnloadRdpNack(&hUsbDeviceFS);
  }
  else
  {
    if (OPENBL_MEM_GetAddressArea((uint32_t)Add) == AREA_ERROR)
    {
      status = OPENBL_USB_SendAddressNack(&hUsbDeviceFS);
    }
    else
    {
      status = OPENBL_USB_EraseMemory(Add);
    }
  }

  return status;
}

/**
  * @brief  Memory write routine.
  * @param  src: Pointer to the source buffer. Address to be written to.
  * @param  dest: Pointer to the destination buffer.
  * @param  Len: Number of data to be written (in bytes).
  * @retval USBD_OK if operation is successful, MAL_FAIL else.
  */
uint16_t USB_DFU_If_Write(uint8_t *pSrc, uint8_t *pDest, uint32_t Len)
{
  uint8_t status;

  if (OPENBL_MEM_GetReadOutProtectionStatus() != RESET)
  {
    status = OPENBL_USB_DnloadRdpNack(&hUsbDeviceFS);
  }
  else
  {
    if (OPENBL_MEM_GetAddressArea((uint32_t)pDest) == AREA_ERROR)
    {
      status = OPENBL_USB_SendAddressNack(&hUsbDeviceFS);
    }
    else
    {
      OPENBL_USB_WriteMemory(pSrc, pDest, Len);
      status = 0;
    }
  }

  return status;
}

/**
  * @brief  Memory read routine.
  * @param  src: Pointer to the source buffer. Address to be written to.
  * @param  dest: Pointer to the destination buffer.
  * @param  Len: Number of data to be read (in bytes).
  * @retval Pointer to the physical address where data should be read.
  */
uint8_t *USB_DFU_If_Read(uint8_t *pSrc, uint8_t *pDest, uint32_t Len)
{
  if (OPENBL_MEM_GetReadOutProtectionStatus() != RESET)
  {
    OPENBL_USB_UploadRdpNack(&hUsbDeviceFS);
  }

  return OPENBL_USB_ReadMemory(pSrc, pDest, Len);
}

/**
  * @brief  Get status routine
  * @param  Add: Address to be read from
  * @param  Cmd: Number of data to be read (in bytes)
  * @param  pBuffer: used for returning the time necessary for a program or an erase operation
  * @retval USBD_OK if operation is successful
  */
uint16_t USB_DFU_If_GetStatus(uint32_t Add, uint8_t Cmd, uint8_t *pBuffer)
{
  switch (Cmd)
  {
    case DFU_MEDIA_PROGRAM:
      pBuffer[1] = (uint8_t)MEDIA_PROGRAM_TIME;
      pBuffer[2] = (uint8_t)(MEDIA_PROGRAM_TIME << 8);
      pBuffer[3] = 0;
      break;

    case DFU_MEDIA_ERASE:
    default:
      pBuffer[1] = (uint8_t)MEDIA_ERASE_TIME;
      pBuffer[2] = (uint8_t)(MEDIA_ERASE_TIME << 8);
      pBuffer[3] = 0;
      break;
  }
  return (USBD_OK);
}
