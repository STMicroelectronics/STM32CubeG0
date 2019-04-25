/**
  ******************************************************************************
  * @file    usbpd_vdm_user.c
  * @author  MCD Application Team
  * @brief   USBPD provider demo file
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2019 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under Ultimate Liberty license
  * SLA0044, the "License"; You may not use this file except in compliance with
  * the License. You may obtain a copy of the License at:
  *                             www.st.com/SLA0044
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "usbpd_core.h"
#include "usbpd_dpm_conf.h"
#include "usbpd_vdm_user.h"
#include "usbpd_dpm_user.h"
#include "stm32g0xx.h"
#if defined(_GUI_INTERFACE)
#include "gui_api.h"
#include "usbpd_gui_memmap.h"
#include "string.h"
#endif /* _GUI_INTERFACE */
#include "string.h"

/* Private define ------------------------------------------------------------*/
#define SVDM_DP_STATUS SVDM_SPECIFIC_1
#define SVDM_DP_CONFIG SVDM_SPECIFIC_2

#define MAX_SVID_USER   1

/*
 * DP Pin assignement
 */
#define  DP_PIN_ASSIGNMENT_NONE 0x00            /*!< De-select pin assignment.  */
#define  DP_PIN_ASSIGNMENT_A    0x01            /*!< Select Pin Assignment A    */
#define  DP_PIN_ASSIGNMENT_B    0x02            /*!< Select Pin Assignment B    */
#define  DP_PIN_ASSIGNMENT_C    0x04            /*!< Select Pin Assignment C    */
#define  DP_PIN_ASSIGNMENT_D    0x08            /*!< Select Pin Assignment D    */
#define  DP_PIN_ASSIGNMENT_E    0x10            /*!< Select Pin Assignment E    */
#define  DP_PIN_ASSIGNMENT_F    0x20            /*!< Select Pin Assignment F    */

/* Pin configs B/D/F support multi-function */
#define MODE_DP_PIN_MF_MASK 0x2a
/* Pin configs A/B support BR2 signaling levels */
#define MODE_DP_PIN_BR2_MASK 0x3
/* Pin configs C/D/E/F support DP signaling levels */
#define MODE_DP_PIN_DP_MASK 0x3c

#define MODE_DP_V13  0x1
#define MODE_DP_GEN2 0x2

#define MODE_DP_NUMBER    2u
#define MODE_DP_MODE_SNK  0x1
#define MODE_DP_MODE_SRC  0x2
#define MODE_DP_MODE_BOTH 0x3

#define MODE_DP_STATUS_CONNECT_NO     0x0    /*!< no (DFP|UFP)_D is connected or disabled */
#define MODE_DP_STATUS_CONNECT_DFP_D  0x1    /*!< DFP_D connected                         */
#define MODE_DP_STATUS_CONNECT_UFP_D  0x2    /*!< UFP_D connected                         */
#define MODE_DP_STATUS_CONNECT_BOTH   0x3    /*!< DFP_D & UFP_D connected                 */

#define DISPLAY_PORT_SVID       0xFF01U          /*!< SVID For Display Port              */
#define PRODUCT_VDO             0xAAAAAAAAU      /*!< Device version + USB Product ID    */

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

/* Private typedef -----------------------------------------------------------*/
/* GNU Compiler */
#if defined   (__GNUC__)
/* ARM Compiler */
#elif defined   (__CC_ARM)
#pragma anon_unions
/* IAR Compiler */
#elif defined (__ICCARM__)
#endif
/*
 * DisplayPort modes capabilities
 */

typedef union
{
  uint32_t d32;
  struct
  {
    uint32_t   SignalDirection  : 2;    /*!< signal direction ( 00b=rsv, 01b=sink, 10b=src 11b=both ) */
    uint32_t   Supports         : 4;    /*!< xxx1: Supports DPv1.3, xx1x Supports USB Gen 2 signaling
                                             Other bits are reserved.                                 */
    uint32_t   PlugOrRecept     : 1;    /*!< Plug | Receptacle (0b == plug, 1b == receptacle)         */
    uint32_t   USB20            : 1;    /*!< USB 2.0 signaling (0b=yes, 1b=no)                        */
    uint32_t   DFP_D_Pin        : 8;    /*!< DFP_D pin assignment supported                           */
    uint32_t   UFP_D_Pin        : 8;    /*!< UFP_D pin assignment supported                           */
    uint32_t   Reserved         : 8;    /*!< Reserved                                                 */
  }d;
}USBPD_DPModeTypeDef;

/*
 * Structure to SVID supported by the devices
 */
typedef struct {
  uint32_t  NumSVIDs;
  uint16_t  SVIDs[MAX_SVID_USER];
}USBPD_SVIDUSerInfo_TypeDef;

/* Private macro -------------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* List of callbacks for VDM layer */

static void                USBPD_VDM_InformSVID(uint8_t PortNum, USBPD_SOPType_TypeDef SOPType, USBPD_VDM_CommandType_Typedef CommandStatus, USBPD_SVIDInfo_TypeDef *pListSVID);
static void                USBPD_VDM_InformMode(uint8_t PortNum, USBPD_SOPType_TypeDef SOPType, USBPD_VDM_CommandType_Typedef CommandStatus, USBPD_ModeInfo_TypeDef *pModesInfo);
static void                USBPD_VDM_InformModeEnter(uint8_t PortNum, USBPD_SOPType_TypeDef SOPType, USBPD_VDM_CommandType_Typedef CommandStatus, uint16_t SVID, uint32_t ModeIndex);
static void                USBPD_VDM_SendAttention(uint8_t PortNum, uint8_t *NbData, uint32_t *VDO);
static void                USBPD_VDM_ReceiveAttention(uint8_t PortNum, uint8_t NbData, uint32_t VDO);
static void                USBPD_VDM_SendSpecific(uint8_t PortNum, USBPD_VDM_Command_Typedef VDMCommand, uint8_t *NbData, uint32_t *VDO);
static USBPD_StatusTypeDef USBPD_VDM_ReceiveSpecific(uint8_t PortNum, USBPD_VDM_Command_Typedef VDMCommand, uint8_t *NbData, uint32_t *VDO);
static void                USBPD_VDM_InformSpecific(uint8_t PortNum, USBPD_VDM_Command_Typedef VDMCommand, uint8_t *NbData, uint32_t *VDO);
static void                USBPD_VDM_InformIdentity(uint8_t PortNum, USBPD_SOPType_TypeDef SOPType, USBPD_VDM_CommandType_Typedef status, USBPD_DiscoveryIdentity_TypeDef *pIdentity);
#if defined(_UVDM)
static void                USBPD_VDM_SendUVDM(uint8_t PortNum, USBPD_UVDMHeader_TypeDef *pUVDM_Header, uint8_t *pNbData, uint32_t *pVDO);
static USBPD_StatusTypeDef USBPD_VDM_ReceiveUVDM(uint8_t PortNum, USBPD_UVDMHeader_TypeDef UVDM_Header, uint8_t *pNbData, uint32_t *pVDO);
#endif /* _UVDM */


/* Private variables ---------------------------------------------------------*/
USBPD_VDM_SettingsTypeDef DPM_VDM_Settings[USBPD_PORT_COUNT] = 
{
  {
    .VDM_XID_SOP                = 0x0000AAAAu,    /*!< A decimal number assigned by USB-IF before certification */
    .VDM_USB_VID_SOP            = USBPD_VID,      /*!< A decimal number assigned by USB-IF before certification */
    .VDM_PID_SOP                = 0xAAAAu,        /*!< A unique number assigned by the Vendor ID holder identifying the product. */
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
  USBPD_VDM_SendAttention,
  USBPD_VDM_ReceiveAttention,
  USBPD_VDM_SendSpecific,
  USBPD_VDM_ReceiveSpecific,
  USBPD_VDM_InformSpecific,
#if defined(_UVDM)
  USBPD_VDM_SendUVDM,
  USBPD_VDM_ReceiveUVDM,
#endif /* _UVDM */
};

extern USBPD_ParamsTypeDef DPM_Params[USBPD_PORT_COUNT];

uint8_t VDM_Mode_On[USBPD_PORT_COUNT];

USBPD_IDHeaderVDO_TypeDef IDHeaderVDO[USBPD_PORT_COUNT];

USBPD_DiscoveryIdentity_TypeDef sIdentity[USBPD_PORT_COUNT];

uint16_t Remote_CurrentSVID[USBPD_PORT_COUNT];
uint16_t Remote_SVID_Mode[USBPD_PORT_COUNT];

USBPD_SVIDUSerInfo_TypeDef SVIDInfo[USBPD_PORT_COUNT];
USBPD_ModeInfo_TypeDef sModesInfo[USBPD_PORT_COUNT];

const USBPD_DPModeTypeDef vdo_dp_modes[USBPD_PORT_COUNT][MODE_DP_NUMBER] =
{
  {
    {
      .d.UFP_D_Pin        = DP_PIN_ASSIGNMENT_NONE,              /* UFP pin cfg supported : none         */
      .d.DFP_D_Pin        = DP_PIN_ASSIGNMENT_C | DP_PIN_ASSIGNMENT_E, /* DFP pin cfg supported */
      .d.USB20            = 1,              /* USB2.0 signalling even in AMode      */
      .d.PlugOrRecept     = CABLE_TO_PLUG,  /* its a plug                           */
      .d.Supports         = MODE_DP_V13,    /* DPv1.3 Support, no Gen2              */
      .d.SignalDirection  = MODE_DP_MODE_SNK  /* Its a sink only                      */
    },
#if MODE_DP_NUMBER==2u
    {
      .d.UFP_D_Pin        = DP_PIN_ASSIGNMENT_NONE,                  /* UFP pin cfg supported : none     */
      .d.DFP_D_Pin        = DP_PIN_ASSIGNMENT_C | DP_PIN_ASSIGNMENT_D, /* DFP pin cfg supported */
      .d.USB20            = 0,                  /* USB2.0 signalling even in AMode  */
      .d.PlugOrRecept     = CABLE_TO_RECEPTACLE,/* its a receptacle                 */
      .d.Supports         = MODE_DP_V13,        /* DPv1.3 Support, no Gen2          */
      .d.SignalDirection  = MODE_DP_MODE_BOTH   /* Its a sink / src                 */
    }
#endif /* MODE_DP_NUMBER==2u */
  },
};

USBPD_DPStatus_TypeDef sDP_Status[USBPD_PORT_COUNT] = 
{
  {
    .d.Reserved       = 0,
    .d.IRQ_HPD        = 0,
    .d.HPDState       = 0,
    .d.ExitDPMode     = 0,
    .d.USBConfig      = 0,
    .d.MultiFunction  = 1,
    .d.Enable         = 1,
    .d.PowerLow       = 0,
    .d.ConnectStatus  = MODE_DP_STATUS_CONNECT_DFP_D,
  },
};

#if defined(_GUI_INTERFACE)
USBPD_DPConfig_TypeDef sDP_Config[USBPD_PORT_COUNT] =
{
  {
    .d.SelectConfiguration  = MODE_DP_CONFIG_SELECT_UFP_U_AS_DFP_D,
    .d.Signaling            = MODE_DP_V13, /* Support of DPv1.3 */
    .d.Reserved1            = 0,
    .d.UFP_U_Pin            = DP_PIN_ASSIGNMENT_C,
    .d.Reserved2            = 0,
  },
};
#endif /* _GUI_INTERFACE */

/* Private functions ---------------------------------------------------------*/

/**
  * @brief  VDM Send Specific message callback
  * @param  PortNum    current port number
  * @param  NbData     Pointer of number of VDO to send
  * @param  VDO        Pointer of VDO to send
  * @retval None
  */
static void USBPD_VDM_SendAttention(uint8_t PortNum, uint8_t *NbData, uint32_t *VDO)
{
  if (USBPD_PORTDATAROLE_DFP == DPM_Params[PortNum].PE_DataRole)
  {
    USBPD_DPStatus_TypeDef dp_status = {0};
    dp_status.d.ConnectStatus = MODE_DP_STATUS_CONNECT_DFP_D;
    
    *VDO = dp_status.d32;
    *NbData = 1;
  }
  else
  {
    sDP_Status[PortNum].d.ConnectStatus = MODE_DP_STATUS_CONNECT_UFP_D;
    
    *VDO = sDP_Status[PortNum].d32;
    *NbData = 1;
  }
}

/**
  * @brief  VDM Attention callback to forward information from PE stack
  * @param  PortNum   current port number
  * @param  NbData    Number of received VDO
  * @param  VDO       Received VDO
  * @retval None
  */
static void USBPD_VDM_ReceiveAttention(uint8_t PortNum, uint8_t NbData, uint32_t VDO)
{
}

/**
  * @brief  VDM Discovery identity callback (answer after sending a Discover Identity req message)
  * @param  PortNum       current port number
  * @param  SOPType       SOP type 
  * @param  CommandStatus Command status based on @ref USBPD_VDM_CommandType_Typedef
  * @param  pIdentity     Pointer on the discovery identity information based on @ref USBPD_DiscoveryIdentity_TypeDef
  * @retval None
*/
static void USBPD_VDM_InformIdentity(uint8_t PortNum, USBPD_SOPType_TypeDef SOPType, USBPD_VDM_CommandType_Typedef CommandStatus, USBPD_DiscoveryIdentity_TypeDef *pIdentity)
{
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
}

/**
  * @brief  Inform SVID callback
  * @param  PortNum       current port number
  * @param  SOPType       SOP type 
  * @param  CommandStatus Command status based on @ref USBPD_VDM_CommandType_Typedef
  * @param  pListSVID     Pointer of list of SVID based on @ref USBPD_SVIDInfo_TypeDef
  * @retval None
  */
static void USBPD_VDM_InformSVID(uint8_t PortNum, USBPD_SOPType_TypeDef SOPType, USBPD_VDM_CommandType_Typedef CommandStatus, USBPD_SVIDInfo_TypeDef *pListSVID)
{
  uint32_t index = 0;

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

/**
  * @brief  Inform Mode callback
  * @param  PortNum         current port number
  * @param  SOPType         SOP type 
  * @param  CommandStatus   Command status based on @ref USBPD_VDM_CommandType_Typedef
  * @param  pModesInfo      Pointer of Modes info based on @ref USBPD_ModeInfo_TypeDef
  * @retval None
  */
static void USBPD_VDM_InformMode(uint8_t PortNum, USBPD_SOPType_TypeDef SOPType, USBPD_VDM_CommandType_Typedef CommandStatus, USBPD_ModeInfo_TypeDef *pModesInfo)
{
  switch(CommandStatus)
  {
  case SVDM_RESPONDER_ACK :
    {
      USBPD_DPModeTypeDef dp_mode;
      DPM_Ports[PortNum].VDM_ModesPortPartner.NumModes = pModesInfo->NumModes;
      for (uint32_t index = 0; index < DPM_Ports[PortNum].VDM_ModesPortPartner.NumModes; index++)
      {
        DPM_Ports[PortNum].VDM_ModesPortPartner.Modes[index] = pModesInfo->Modes[index];
      }

      DPM_Ports[PortNum].VDM_ModesPortPartner.SVID = pModesInfo->SVID;

      dp_mode.d32 = (DPM_Ports[PortNum].VDM_ModesPortPartner.Modes[0]);

#if defined(_GUI_INTERFACE)
      if (USBPD_ENABLE != GUI_IsRunning())
#endif /* _GUI_INTERFACE */
      {
        /* Enter in the mode only if DFP_D and UFP_D are supported */
        if ((dp_mode.d.SignalDirection == MODE_DP_MODE_BOTH) || (MODE_DP_MODE_SNK == dp_mode.d.SignalDirection))
        {
          /* Request to enter in the 1st mode */
          USBPD_PE_SVDM_RequestModeEnter(PortNum, SOPType, pModesInfo->SVID, 1);
        }
      }
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
}

/**
  * @brief  Inform Mode enter callback
  * @param  PortNum   current port number
  * @param  SVID      SVID ID
  * @param  ModeIndex Index of the mode to be entered
  * @retval None
  */
static void USBPD_VDM_InformModeEnter(uint8_t PortNum, USBPD_SOPType_TypeDef SOPType, USBPD_VDM_CommandType_Typedef CommandStatus, uint16_t SVID, uint32_t ModeIndex)
{
  switch(CommandStatus)
  {
  case SVDM_RESPONDER_ACK :
#if defined(_GUI_INTERFACE)
    if (USBPD_ENABLE != GUI_IsRunning())
#endif /* _GUI_INTERFACE */
    {
      if (DPM_Ports[PortNum].VDM_SVIDPortPartner.SVIDs[(ModeIndex - 1)] == SVID)
      {
        VDM_Mode_On[PortNum] = 1;
        
        USBPD_PE_SVDM_RequestSpecific(PortNum, USBPD_SOPTYPE_SOP, SVDM_DP_STATUS, DISPLAY_PORT_SVID);
      }
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
  * @param  pDP_Status Pointer on @ref USBPD_DPConfig_TypeDef
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
  * @param  PortNum            current port number
  * @param  VDMCommand VDM command based on @ref USBPD_VDM_Command_Typedef
  * @param  NbData     Pointer of number of VDO to send
  * @param  VDO        Pointer of VDO to send
  * @retval None
  */
static void USBPD_VDM_SendSpecific(uint8_t PortNum, USBPD_VDM_Command_Typedef VDMCommand, uint8_t *NbData, uint32_t *VDO)
{
  if (USBPD_PORTDATAROLE_DFP == DPM_Params[PortNum].PE_DataRole)
  {
    switch(VDMCommand)
    {
    case SVDM_DP_STATUS :
      {
#if defined(_GUI_INTERFACE)
        *VDO = sDP_Status[PortNum].d32;
#else
        USBPD_DPStatus_TypeDef dp_status = {0};
        dp_status.d.ConnectStatus = MODE_DP_STATUS_CONNECT_DFP_D;

        *VDO = dp_status.d32;
#endif /* _GUI_INTERFACE */
        *NbData = 1;
      }
      break;
    case SVDM_DP_CONFIG :
      {
        USBPD_DPModeTypeDef dp_mode;
        dp_mode.d32 = (DPM_Ports[PortNum].VDM_ModesPortPartner.Modes[0]);
        
        if (DP_PIN_ASSIGNMENT_C == (dp_mode.d.DFP_D_Pin & DP_PIN_ASSIGNMENT_C))
        {
#if defined(_GUI_INTERFACE)
          *VDO = sDP_Config[PortNum].d32;
#else
          USBPD_DPConfig_TypeDef vdo_config = {0};
          vdo_config.d.SelectConfiguration = MODE_DP_CONFIG_SELECT_UFP_U_AS_DFP_D;
          /* Support of DPv1.3 */
          vdo_config.d.Signaling = 0x1;
          vdo_config.d.UFP_U_Pin = DP_PIN_ASSIGNMENT_C;
          *VDO = vdo_config.d32;
#endif /* _GUI_INTERFACE */
          *NbData = 1;
        }
        else
        {
          /* Config not supported */
        }
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
        sDP_Status[PortNum].d.ConnectStatus = MODE_DP_STATUS_CONNECT_UFP_D;

        *VDO = sDP_Status[PortNum].d32;
        *NbData = 1;
      }
      break;
    case SVDM_DP_CONFIG :
      *VDO = 0;
      *NbData = 0;
      break;

    default :
      break;
    }
  }
}

/**
  * @brief  VDM Receive Specific message callback
  * @param  PortNum     Current port number
  * @param  VDMCommand  VDM command based on @ref USBPD_VDM_Command_Typedef
  * @param  pNbData     Pointer of number of received VDO and used for the answer
  * @param  pVDO        Pointer of received VDO and use for the answer
  * @retval USBPD status
  */
static USBPD_StatusTypeDef USBPD_VDM_ReceiveSpecific(uint8_t PortNum, USBPD_VDM_Command_Typedef VDMCommand, uint8_t *pNbData, uint32_t *pVDO)
{
  USBPD_StatusTypeDef status = USBPD_NAK;
  return status;
}

/**
  * @brief  VDM Inform Specific callback
  * @param  PortNum           current port number
  * @param  VDMCommand VDM command based on @ref USBPD_VDM_Command_Typedef
  * @param  NbData     Pointer of number of received VDO
  * @param  VDO        Pointer of received VDO
  * @retval status
  */
static void USBPD_VDM_InformSpecific(uint8_t PortNum, USBPD_VDM_Command_Typedef VDMCommand, uint8_t *NbData, uint32_t *VDO)
{
}

#if defined(_UVDM)
static void USBPD_VDM_SendUVDM(uint8_t PortNum, USBPD_UVDMHeader_TypeDef *pUVDM_Header, uint8_t *pNbData, uint32_t *pVDO)
{
  pUVDM_Header->b.VID = USBPD_VID;
  *pNbData            = 0;
}

static USBPD_StatusTypeDef USBPD_VDM_ReceiveUVDM(uint8_t PortNum, USBPD_UVDMHeader_TypeDef UVDM_Header, uint8_t *pNbData, uint32_t *pVDO)
{
  USBPD_StatusTypeDef _status = USBPD_OK;

  if (USBPD_VID != UVDM_Header.b.VID)
  {
    _status = USBPD_ERROR;
  }
  return _status;
}
#endif /* _UVDM */


/* Exported functions ---------------------------------------------------------*/
/**
  * @brief  VDM Initialization function
  * @param  PortNum     Index of current used port
  * @param  pSettings   Pointer on @ref USBPD_VDM_SettingsTypeDef structure
  * @param  pParams     Pointer on @ref USBPD_ParamsTypeDef structure
  * @retval status
  */
USBPD_StatusTypeDef USBPD_VDM_UserInit(uint8_t PortNum)
{
  uint32_t index = 0;
  
   /* Initialize SVID supported by consumer */
  SVIDInfo[PortNum].NumSVIDs = MAX_SVID_USER;
  
  for (index = 0; index < MAX_SVID_USER; index++)
  {
    SVIDInfo[PortNum].SVIDs[index] = DISPLAY_PORT_SVID + index;
  }

  USBPD_PE_InitVDM_Callback(PortNum, (USBPD_VDM_Callbacks *)&vdmCallbacks);

  return USBPD_OK;
}

/**
  * @brief  VDM Reset function
  * @retval status
  */
void USBPD_VDM_UserReset(uint8_t PortNum)
{
  /* Reset Port Partner variables*/
  Remote_CurrentSVID[PortNum]       = 0;

  /* Disconnect received on Type-C */
  VDM_Mode_On[PortNum] = 0;
}


/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/

