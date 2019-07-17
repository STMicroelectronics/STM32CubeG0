/**
  ******************************************************************************
  * @file    usbpd_hw_phy.h
  * @author  MCD Application Team
  * @brief   This file contains the headers of usbpd_hw_phy.h.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2017 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under Ultimate Liberty license
  * SLA0044, the "License"; You may not use this file except in compliance with
  * the License. You may obtain a copy of the License at:
  *                             www.st.com/SLA0044
  *
  ******************************************************************************
  */

#ifndef __USBPD_HW_PHY_H_
#define __USBPD_HW_PHY_H_

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "usbpd_phy.h"
  
/** @addtogroup STM32_USBPD_LIBRARY
  * @{
  */

/** @addtogroup USBPD_DEVICE
  * @{
  */

/** @addtogroup USBPD_DEVICE_PHY
  * @{
  */

/* Exported typedef ----------------------------------------------------------*/
/** @defgroup USBPD_PORT_HandleTypeDef USB PD handle Structure definition for USBPD_PHY_HW_IF
  * @brief  USBPD PORT handle Structure definition
  * @{
  */
typedef struct
{
  UCPD_TypeDef                *husbpd;          /*!< UCPD Handle parameters             */
  DMA_Channel_TypeDef         *hdmatx;          /*!< Tx DMA Handle parameters           */
  DMA_Channel_TypeDef         *hdmarx;          /*!< Rx DMA Handle parameters           */

  USBPD_SettingsTypeDef       *settings;
  USBPD_ParamsTypeDef         *params;
  const USBPD_PHY_Callbacks   *cbs;             /*!< USBPD_PHY__IF callbacks         */

  __IO uint32_t                PIN_CC1;         /*!< CC1 detection state               */
  __IO uint32_t                PIN_CC2;         /*!< CC2 detection state               */
    
  void (*USBPD_CAD_WakeUp)(void);               /*!< function used to wakeup cad task   */
  
  uint32_t  SupportedSOP;                       /*!<bit field SOP"Debug SOP'Debug SOP" SOP' SOP */
  
  CCxPin_TypeDef               CCx;             /*!< CC pin used for communication      */
  uint8_t                     *ptr_RxBuff;      /*!< Pointer to Raw Rx transfer Buffer  */
} USBPD_PORT_HandleTypeDef;

extern USBPD_PORT_HandleTypeDef Ports[USBPD_PORT_COUNT];

/**
  * @}
  */

/* Exported define -----------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported variables --------------------------------------------------------*/
/* Exported functions --------------------------------------------------------*/
/** @defgroup USBPD_DEVICE_PHY_Exported_functions USBPD DEVICE PHY Exported functions
  * @{
  */
USBPD_StatusTypeDef         PHY_PortInit(uint8_t PortNum, const USBPD_PHY_Callbacks *cbs, uint8_t *pRxBuffer, uint32_t SupportedSOP);
void                        PHY_BistCompleted(uint8_t PortNum, USBPD_BISTMsg_TypeDef bistmode);
void                        PHY_TxCompleted(uint8_t portnum);
void                        PHY_ResetCompleted(uint8_t PortNum, USBPD_SOPType_TypeDef Type);
USBPD_PHY_RX_Status_TypeDef PHY_Rx_Reset(uint8_t PortNum);
void                        PHY_Rx_HardReset(uint8_t PortNum);
void                        PHY_Rx_Completed(uint8_t PortNum, uint32_t MsgType);

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

#ifdef __cplusplus
}
#endif

#endif /* __USBPD_PHY_H_ */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/

