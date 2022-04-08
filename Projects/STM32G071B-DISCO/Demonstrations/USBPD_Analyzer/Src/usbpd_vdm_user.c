/**
  ******************************************************************************
  * @file    usbpd_vdm_user.c
  * @author  MCD Application Team
  * @brief   USBPD provider demo file
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2019 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "usbpd_core.h"
#include "usbpd_dpm_conf.h"
#include "usbpd_vdm_user.h"
#include "usbpd_dpm_user.h"
/* USER CODE BEGIN Includes */
#include "stm32g0xx.h"
#if defined(_GUI_INTERFACE)
#include "gui_api.h"
#include "usbpd_gui_memmap.h"
#endif /* _GUI_INTERFACE */
#include "string.h"
#if defined(_DEBUG_TRACE)
#include "usbpd_trace.h"
#endif /* _DEBUG_TRACE */
/* USER CODE END Includes */

/** @addtogroup STM32_USBPD_APPLICATION
  * @{
  */

/** @addtogroup STM32_USBPD_APPLICATION_VDM_USER
  * @{
  */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN Private_define */
#define SVDM_DP_STATUS SVDM_SPECIFIC_1
#define SVDM_DP_CONFIG SVDM_SPECIFIC_2

/** 
  * @brief Type-C to Plug/Receptacle
  * @{
  */
typedef enum {
  CABLE_TO_PLUG        = 0, /*0b0*/
  CABLE_TO_RECEPTACLE  = 1, /*0b1*/
} USBPD_CableToPR;

/** 
  * @}
  */
#if defined(USBPDCORE_SNK_CAPA_EXT)
#define VDM_CABLE_INFO      USBPD_CORE_SNK_EXTENDED_CAPA + 1U
#else
#define VDM_CABLE_INFO      USBPD_CORE_UNSTRUCTURED_VDM + 1U
#endif /* USBPDCORE_SNK_CAPA_EXT */
/* USER CODE END Private_define */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN Private_typedef */
/* GNU Compiler */
#if defined   (__GNUC__)
/* ARM Compiler */
#elif defined   (__CC_ARM)
#pragma anon_unions
/* IAR Compiler */
#elif defined (__ICCARM__)
#endif
/* USER CODE END Private_typedef */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN Private_macro */
#if defined(_DEBUG_TRACE)
#define VDM_USER_DEBUG_TRACE(_PORT_, __MESSAGE__)  USBPD_TRACE_Add(USBPD_TRACE_DEBUG,    (_PORT_), 0u,(__MESSAGE__), sizeof(__MESSAGE__) - 1u)
#else
#define VDM_USER_DEBUG_TRACE(_PORT_, __MESSAGE__)
#endif /* _DEBUG_TRACE */
/* USER CODE END Private_macro */

/* Private function prototypes -----------------------------------------------*/
/* List of callbacks for VDM layer */

static void                USBPD_VDM_InformIdentity(uint8_t PortNum, USBPD_SOPType_TypeDef SOPType, USBPD_VDM_CommandType_Typedef status, USBPD_DiscoveryIdentity_TypeDef *pIdentity);
static void                USBPD_VDM_InformSVID(uint8_t PortNum, USBPD_SOPType_TypeDef SOPType, USBPD_VDM_CommandType_Typedef CommandStatus, USBPD_SVIDInfo_TypeDef *pListSVID);
static void                USBPD_VDM_InformMode(uint8_t PortNum, USBPD_SOPType_TypeDef SOPType, USBPD_VDM_CommandType_Typedef CommandStatus, USBPD_ModeInfo_TypeDef *pModesInfo);
static void                USBPD_VDM_InformModeEnter(uint8_t PortNum, USBPD_SOPType_TypeDef SOPType, USBPD_VDM_CommandType_Typedef CommandStatus, uint16_t SVID, uint32_t ModeIndex);
static void                USBPD_VDM_InformSpecific(uint8_t PortNum, USBPD_SOPType_TypeDef SOPType, USBPD_VDM_Command_Typedef VDMCommand, uint8_t *NbData, uint32_t *VDO);
static void                USBPD_VDM_SendSpecific(uint8_t PortNum, USBPD_SOPType_TypeDef SOPType, USBPD_VDM_Command_Typedef VDMCommand, uint8_t *NbData, uint32_t *VDO);
/* USER CODE BEGIN Private_prototypes */

/* USER CODE END Private_prototypes */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN Private_variables */
USBPD_VDM_SettingsTypeDef DPM_VDM_Settings[USBPD_PORT_COUNT] = 
{
  {
    .VDM_XID_SOP                = USBPD_XID,    /*!< A decimal number assigned by USB-IF before certification */
    .VDM_USB_VID_SOP            = USBPD_VID,      /*!< A decimal number assigned by USB-IF before certification */
    .VDM_PID_SOP                = USBPD_PID,    /*!< A unique number assigned by the Vendor ID holder identifying the product. */
    .VDM_ModalOperation         = MODAL_OPERATION_SUPPORTED, /*!< Product support Modes based on @ref USBPD_ModalOp_TypeDef */
    .VDM_bcdDevice_SOP          = 0xAAAAu,        /*!< A unique number assigned by the Vendor ID holder containing identity information relevant to the release version of the product. */
    .VDM_USBHostSupport         = USB_NOTCAPABLE, /*!< Indicates whether the UUT is capable of enumerating USB Host */
    .VDM_USBDeviceSupport       = USB_CAPABLE, /*!< Indicates whether the UUT is capable of enumerating USB Devices */
    .VDM_ProductTypeUFPorCP     = PRODUCT_TYPE_AMA, /*!< Product type UFP or CablePlug of the UUT based on @ref USBPD_ProductType_TypeDef */
    .VDM_ProductTypeDFP         = PRODUCT_TYPE_UNDEFINED, /*!< Product type DFP of the UUT based on @ref USBPD_ProductType_TypeDef */
  }
};

const USBPD_VDM_Callbacks vdmCallbacks =
{
  /* in case of DISCO : we do not manage the below requests */
  NULL,
  NULL,
  NULL,
  NULL,
  NULL,
  USBPD_VDM_InformIdentity,
  USBPD_VDM_InformSVID,
  USBPD_VDM_InformMode,
  USBPD_VDM_InformModeEnter,
  NULL,
  NULL,
  NULL,
  USBPD_VDM_SendSpecific,
  NULL,
  USBPD_VDM_InformSpecific,
};

extern USBPD_ParamsTypeDef DPM_Params[USBPD_PORT_COUNT];
uint8_t VDM_Mode_On[USBPD_PORT_COUNT];

USBPD_IDHeaderVDO_TypeDef IDHeaderVDO[USBPD_PORT_COUNT];

USBPD_DiscoveryIdentity_TypeDef sIdentity[USBPD_PORT_COUNT];

uint16_t Remote_CurrentSVID[USBPD_PORT_COUNT];
uint16_t Remote_SVID_Mode[USBPD_PORT_COUNT];

#if defined(_GUI_INTERFACE)
USBPD_DPStatus_TypeDef sDP_Status[USBPD_PORT_COUNT];
USBPD_DPConfig_TypeDef sDP_Config[USBPD_PORT_COUNT];
#endif /* _GUI_INTERFACE */
extern GUI_SAVE_INFO                 DPM_GUI_SaveInfo;
/* USER CODE END Private_variables */

/* Private functions ---------------------------------------------------------*/


/**
  * @brief  Inform identity callback
  * @note   Function is called to save Identity information received in Discovery identity from port partner
            (answer to SVDM discovery identity sent by device)
  * @param  PortNum       current port number
  * @param  SOPType       SOP type 
  * @param  CommandStatus Command status based on @ref USBPD_VDM_CommandType_Typedef
  * @param  pIdentity     Pointer on the discovery identity information based on @ref USBPD_DiscoveryIdentity_TypeDef
  * @retval None
*/
static void USBPD_VDM_InformIdentity(uint8_t PortNum, USBPD_SOPType_TypeDef SOPType, USBPD_VDM_CommandType_Typedef CommandStatus, USBPD_DiscoveryIdentity_TypeDef *pIdentity)
{
/* USER CODE BEGIN USBPD_VDM_InformIdentity */
  switch(CommandStatus)
  {
  case SVDM_RESPONDER_ACK :
    {
      uint8_t*  disco_ident;
      disco_ident = (uint8_t*)&DPM_Ports[PortNum].VDM_DiscoIdentify;
      memcpy(disco_ident, (uint8_t*)pIdentity, sizeof(USBPD_DiscoveryIdentity_TypeDef));

#if defined(_GUI_INTERFACE)
      if (USBPD_ENABLE != GUI_IsRunning())
#endif /* _GUI_INTERFACE */
      {
        /* Alternate mode presence */
        if (pIdentity->AMA_VDO_Presence == 1)
        {
          /* Request to get SVID */
          USBPD_PE_SVDM_RequestSVID(PortNum, USBPD_SOPTYPE_SOP);
        }
      }
      memset(&DPM_Ports[PortNum].VDM_SVIDPortPartner, 0, sizeof(USBPD_SVIDPortPartnerInfo_TypeDef));
      Remote_CurrentSVID[PortNum]                   = 0;
    }
    break;
  case SVDM_RESPONDER_NAK :
    /* Nothing to do */
    break;
  case SVDM_RESPONDER_BUSY :
    /* retry in 50ms */
    break;
  default :
    break;
  }
/* USER CODE END USBPD_VDM_InformIdentity */
}

/**
  * @brief  Inform SVID callback
  * @note   Function is called to save list of SVID received in Discovery SVID from port partner
            (answer to SVDM discovery SVID sent by device)
  * @param  PortNum       current port number
  * @param  SOPType       SOP type 
  * @param  CommandStatus Command status based on @ref USBPD_VDM_CommandType_Typedef
  * @param  pListSVID     Pointer of list of SVID based on @ref USBPD_SVIDInfo_TypeDef
  * @retval None
  */
static void USBPD_VDM_InformSVID(uint8_t PortNum, USBPD_SOPType_TypeDef SOPType, USBPD_VDM_CommandType_Typedef CommandStatus, USBPD_SVIDInfo_TypeDef *pListSVID)
{
/* USER CODE BEGIN USBPD_VDM_InformSVID */
  uint32_t index = 0;

  /* Manage only reception of SVID coming from SOP (SVID on cable to be implemented) */
  if (USBPD_SOPTYPE_SOP == SOPType)
  {
  switch(CommandStatus)
  {
  case SVDM_RESPONDER_ACK :
    if (1 == DPM_Ports[PortNum].VDM_SVIDPortPartner.FlagAllSVIDReceived)
    {
      /* New list of SVIDs is being received. */
      memset(&DPM_Ports[PortNum].VDM_SVIDPortPartner, 0, sizeof(USBPD_SVIDPortPartnerInfo_TypeDef));
    }
    /* Save the received SVIDs */
    for (index = 0; index < pListSVID->NumSVIDs; index++)
    {
      DPM_Ports[PortNum].VDM_SVIDPortPartner.SVIDs[DPM_Ports[PortNum].VDM_SVIDPortPartner.NumSVIDs++] = pListSVID->SVIDs[index];
    }

    /* Check if all the SVIDs have been received */
    if ((pListSVID->AllSVID_Received == 0) && (0 == (DPM_Ports[PortNum].VDM_SVIDPortPartner.NumSVIDs %12)))
    {
      /* Request a new SVID message */
      USBPD_PE_SVDM_RequestSVID(PortNum, USBPD_SOPTYPE_SOP);
    }
    else
    {
      /* All the SVIDs have been received, request discovery mode on 1st SVID available
      in the list */
      DPM_Ports[PortNum].VDM_SVIDPortPartner.FlagAllSVIDReceived = 1;
      /* Request a discovery mode */
      Remote_SVID_Mode[PortNum] = 0;
#if defined(_GUI_INTERFACE)
      if (USBPD_ENABLE != GUI_IsRunning())
#endif /* _GUI_INTERFACE */
      {
        USBPD_PE_SVDM_RequestMode(PortNum, USBPD_SOPTYPE_SOP, DPM_Ports[PortNum].VDM_SVIDPortPartner.SVIDs[0]);
      }
    }
    break;
  case SVDM_RESPONDER_NAK :
    /* Nothing to do */
    break;
  case SVDM_RESPONDER_BUSY :
    // retry in 50ms 
    break;
  default :
    break;
  }
}
/* USER CODE END USBPD_VDM_InformSVID */
}

/**
  * @brief  Inform Mode callback ( received in Discovery Modes ACK)
  * @note   Function is called to save list of modes linked to SVID received in Discovery mode from port partner
            (answer to SVDM discovery mode sent by device)
  * @param  PortNum         current port number
  * @param  SOPType         SOP type 
  * @param  CommandStatus   Command status based on @ref USBPD_VDM_CommandType_Typedef
  * @param  pModesInfo      Pointer of Modes info based on @ref USBPD_ModeInfo_TypeDef
  * @retval None
  */
static void USBPD_VDM_InformMode(uint8_t PortNum, USBPD_SOPType_TypeDef SOPType, USBPD_VDM_CommandType_Typedef CommandStatus, USBPD_ModeInfo_TypeDef *pModesInfo)
{
/* USER CODE BEGIN USBPD_VDM_InformMode */
  /* Manage only reception of Mode coming from SOP (Mode on cable to be implemented) */
  switch(CommandStatus)
  {
  case SVDM_RESPONDER_ACK :
    {
      DPM_Ports[PortNum].VDM_ModesPortPartner.NumModes = pModesInfo->NumModes;
      for (uint32_t index = 0; index < DPM_Ports[PortNum].VDM_ModesPortPartner.NumModes; index++)
      {
        DPM_Ports[PortNum].VDM_ModesPortPartner.Modes[index] = pModesInfo->Modes[index];
      }

      DPM_Ports[PortNum].VDM_ModesPortPartner.SVID = pModesInfo->SVID;

    }
    break;
  case SVDM_RESPONDER_NAK :
#if defined(_GUI_INTERFACE)
    if (USBPD_ENABLE != GUI_IsRunning())
#endif /* _GUI_INTERFACE */
    {
      /* All the SVIDs have been received, request discovery mode on next SVID available
      in the list */
      Remote_SVID_Mode[PortNum]++;
      /* Request a discovery mode */
      USBPD_PE_SVDM_RequestMode(PortNum, USBPD_SOPTYPE_SOP, DPM_Ports[PortNum].VDM_SVIDPortPartner.SVIDs[Remote_SVID_Mode[PortNum]]);
    }
    break;
  case SVDM_RESPONDER_BUSY :
    // retry in 50ms 
    break;
  default :
    break;
  }
/* USER CODE END USBPD_VDM_InformMode */
}

/**
  * @brief  Inform Mode enter callback
  * @note   Function is called to inform if port partner accepted or not to enter in the mode
  *         specified in the SVDM enter mode sent by the device
  * @param  PortNum   current port number
  * @param  SOPType       SOP type
  * @param  CommandStatus Command status based on @ref USBPD_VDM_CommandType_Typedef
  * @param  SVID      SVID ID
  * @param  ModeIndex Index of the mode to be entered
  * @retval None
  */
static void USBPD_VDM_InformModeEnter(uint8_t PortNum, USBPD_SOPType_TypeDef SOPType, USBPD_VDM_CommandType_Typedef CommandStatus, uint16_t SVID, uint32_t ModeIndex)
{
/* USER CODE BEGIN USBPD_VDM_InformModeEnter */
  /* Manage only reception of enter Mode coming from SOP (Enter Mode on cable to be implemented) */
  switch(CommandStatus)
  {
  case SVDM_RESPONDER_ACK :
    break;
  case SVDM_RESPONDER_NAK :
    /* Nothing to do */
    break;
  case SVDM_RESPONDER_BUSY :
    /* retry in 50ms */
    break;
  default :
    break;
  }
/* USER CODE END USBPD_VDM_InformModeEnter */
}

/**
  * @brief  VDM Fill DP Status
  * @param  PortNum    current port number
  * @param  pDP_Status Pointer on @ref USBPD_DPStatus_TypeDef
  * @retval None
  */
void USBPD_VDM_FillDPStatus(uint8_t PortNum, USBPD_DPStatus_TypeDef *pDP_Status)
{
#if defined(_GUI_INTERFACE)
  memcpy(&sDP_Status[PortNum], pDP_Status, sizeof(USBPD_DPStatus_TypeDef));
#endif /* _GUI_INTERFACE */
}

/**
  * @brief  VDM Fill DP Config
  * @param  PortNum    current port number
  * @param  pDP_Config Pointer on @ref USBPD_DPConfig_TypeDef
  * @retval None
  */
void USBPD_VDM_FillDPConfig(uint8_t PortNum, USBPD_DPConfig_TypeDef *pDP_Config)
{
#if defined(_GUI_INTERFACE)
  memcpy(&sDP_Config[PortNum], pDP_Config, sizeof(USBPD_DPConfig_TypeDef));
#endif /* _GUI_INTERFACE */
}

/**
  * @brief  VDM Send Specific message callback
  * @note   Function is called when device wants to send a SVDM specific init message to port partner
  *         (for instance DP status or DP configure can be filled through this function)
  * @param  PortNum    current port number
  * @param  SOPType    SOP type
  * @param  VDMCommand VDM command based on @ref USBPD_VDM_Command_Typedef
  * @param  pNbData    Pointer of number of VDO to send
  * @param  pVDO       Pointer of VDO to send
  * @retval None
  */
static void USBPD_VDM_SendSpecific(uint8_t PortNum, USBPD_SOPType_TypeDef SOPType, USBPD_VDM_Command_Typedef VDMCommand, uint8_t *pNbData, uint32_t *pVDO)
{
/* USER CODE BEGIN USBPD_VDM_SendSpecific */
    /* Manage Specific message sent to port partner */
  if (USBPD_PORTDATAROLE_DFP == DPM_Params[PortNum].PE_DataRole)
  {
    switch(VDMCommand)
    {
    case SVDM_DP_STATUS :
      {
        VDM_USER_DEBUG_TRACE(PortNum, "USBPD_VDM_SendSpecific(DP_STATUS)");
#if defined(_GUI_INTERFACE)
        *pVDO = sDP_Status[PortNum].d32;
        *pNbData = 1;
#else
        *pVDO = 0;
        *pNbData = 0;
#endif /* _GUI_INTERFACE */
      }
      break;
    case SVDM_DP_CONFIG :
      {
        VDM_USER_DEBUG_TRACE(PortNum, "USBPD_VDM_SendSpecific(DP_CONFIG)");

#if defined(_GUI_INTERFACE)
        *pVDO = sDP_Config[PortNum].d32;
#else
#endif /* _GUI_INTERFACE */
      }
      break;
    default :
      break;
    }
  }
  else
  {
    switch(VDMCommand)
    {
    case SVDM_DP_STATUS :
      {
        VDM_USER_DEBUG_TRACE(PortNum, "USBPD_VDM_SendSpecific(DP_STATUS)");
        *pVDO = 0;
        *pNbData = 0;
      }
      break;
    case SVDM_DP_CONFIG :
      VDM_USER_DEBUG_TRACE(PortNum, "USBPD_VDM_SendSpecific(DP_CONFIG)");
      *pVDO = 0;
      *pNbData = 0;
      break;

    default :
      break;
    }
  }
/* USER CODE END USBPD_VDM_SendSpecific */
}

/**
  * @brief  VDM Specific message callback to inform user of reception of VDM specific message
  * @note   Function is called when answer from SVDM specific init message has been received by the device
  *         (for instance, save DP status and DP configure data through this function)
  * @param  PortNum           current port number
  * @param  SOPType    SOP type
  * @param  VDMCommand VDM command based on @ref USBPD_VDM_Command_Typedef
  * @param  pNbData    Pointer of number of received VDO
  * @param  pVDO       Pointer of received VDO
  * @retval None
  */
static void USBPD_VDM_InformSpecific(uint8_t PortNum, USBPD_SOPType_TypeDef SOPType, USBPD_VDM_Command_Typedef VDMCommand, uint8_t *pNbData, uint32_t *pVDO)
{
/* USER CODE BEGIN USBPD_VDM_InformSpecific */
/* USER CODE END USBPD_VDM_InformSpecific */
}



/* USER CODE BEGIN Private_functions */

/* USER CODE END Private_functions */

/* Exported functions ---------------------------------------------------------*/
/**
  * @brief  VDM Initialization function
  * @param  PortNum     Index of current used port
  * @retval status
  */
USBPD_StatusTypeDef USBPD_VDM_UserInit(uint8_t PortNum)
{
/* USER CODE BEGIN USBPD_VDM_UserInit */
  USBPD_PE_InitVDM_Callback(PortNum, (USBPD_VDM_Callbacks *)&vdmCallbacks);

  return USBPD_OK;
/* USER CODE END USBPD_VDM_UserInit */
}

/**
  * @brief  VDM Reset function
  * @param  PortNum     Index of current used port
  * @retval status
  */
void USBPD_VDM_UserReset(uint8_t PortNum)
{
/* USER CODE BEGIN USBPD_VDM_UserReset */
  /* Reset Port Partner variables*/
  Remote_CurrentSVID[PortNum]       = 0;

  /* Disconnect received on Type-C */
  VDM_Mode_On[PortNum] = 0;
/* USER CODE END USBPD_VDM_UserReset */
}

/* USER CODE BEGIN Exported_functions */
/* USER CODE END Exported_functions */


/**
  * @}
  */

/**
  * @}
  */
