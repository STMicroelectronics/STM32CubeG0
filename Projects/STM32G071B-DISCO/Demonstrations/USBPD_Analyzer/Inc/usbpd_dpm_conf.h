/**
  ******************************************************************************
  * @file    usbpd_dpm_core.h
  * @author  MCD Application Team
  * @brief   Header file for usbpd_dpm_core.c file
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

#ifndef __USBPD_DPM_CONF_H_
#define __USBPD_DPM_CONF_H_

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "usbpd_pdo_defs.h"
#include "usbpd_vdm_user.h"
#include "usbpd_dpm_user.h"
#if defined(_GUI_INTERFACE)
#include "usbpd_gui_memmap.h"
#endif /* _GUI_INTERFACE */

/* Define   ------------------------------------------------------------------*/
/* Define VID, PID,... manufacturer parameters */
#define USBPD_VID (0x0483u)     /*!< Vendor ID (assigned by the USB-IF)                     */
#define USBPD_PID (0x0002u)     /*!< Product ID (assigned by the manufacturer)              */
#define USBPD_XID (0xF0000003u) /*!< Value provided by the USB-IF assigned to the product   */

/* Exported typedef ----------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
#ifndef __USBPD_DPM_CORE_C
extern USBPD_SettingsTypeDef      DPM_Settings[USBPD_PORT_COUNT];
extern USBPD_USER_SettingsTypeDef DPM_USER_Settings[USBPD_PORT_COUNT];
#else /* __USBPD_DPM_CORE_C */
USBPD_SettingsTypeDef       DPM_Settings[USBPD_PORT_COUNT] =
{
  {
    .PE_SupportedSOP = USBPD_SUPPORTED_SOP_SOP, /* Supported SOP : SOP      */
    .PE_SpecRevision = USBPD_SPECIFICATION_REV3,/* spec revision value                                     */
    .PE_DefaultRole = USBPD_PORTPOWERROLE_SNK,  /* Default port role                                       */
    .PE_RoleSwap = USBPD_FALSE,                 /* support port role swap                                  */
    .PE_VDMSupport = USBPD_TRUE,                /* support VDM                                             */
    .PE_RespondsToDiscovSOP = USBPD_TRUE,       /*!< Can respond successfully to a Discover Identity */
    .PE_AttemptsDiscovSOP = USBPD_TRUE,         /*!< Can send a Discover Identity */
    .PE_PingSupport = USBPD_FALSE,              /* support Ping (only for PD3.0)                           */
    .PE_CapscounterSupport = USBPD_FALSE,       /* support caps counter                                    */
    .CAD_RoleToggle = USBPD_FALSE,              /* cad role toogle                                         */
    .CAD_TryFeature = USBPD_FALSE,              /* cad try feature                                         */
    .CAD_AccesorySupport = USBPD_FALSE,         /* cas accessory support                                   */
    .PE_PD3_Support.d =                           /*!< PD3 SUPPORT FEATURE                                              */
    {
      .PE_UnchunkSupport                = USBPD_FALSE,  /*!< Unchunked mode Support                */
      .PE_FastRoleSwapSupport           = USBPD_FALSE,  /* support fast role swap only spec revsion 3.0            */
      .Is_GetPPSStatus_Supported        = USBPD_TRUE,   /*!< PPS message supported or not by DPM */
      .Is_SrcCapaExt_Supported          = USBPD_TRUE,   /*!< Source_Capabilities_Extended message supported or not by DPM */
      .Is_Alert_Supported               = USBPD_FALSE,  /*!< Alert message supported or not by DPM */
      .Is_GetStatus_Supported           = USBPD_FALSE,  /*!< Status message supported or not by DPM */
      .Is_GetManufacturerInfo_Supported = USBPD_FALSE,  /*!< Manufacturer_Info message supported or not by DPM */
      .Is_GetCountryCodes_Supported     = USBPD_FALSE,  /*!< Country_Codes message supported or not by DPM */
      .Is_GetCountryInfo_Supported      = USBPD_FALSE,  /*!< Country_Info message supported or not by DPM */
      .Is_SecurityRequest_Supported     = USBPD_FALSE,  /*!< Security_Response message supported or not by DPM */
      .Is_FirmUpdateRequest_Supported   = USBPD_FALSE,  /*!< Firmware update response message not supported by PE */
      .Is_SnkCapaExt_Supported          = USBPD_FALSE,  /*!< Sink_Capabilities_Extended message supported or not by DPM */
    },

    .CAD_SRCToggleTime          = 40,                    /* uint8_t CAD_SRCToggleTime; */
    .CAD_SNKToggleTime          = 40,                    /* uint8_t CAD_SNKToggleTime; */
  }
};

USBPD_USER_SettingsTypeDef DPM_USER_Settings[USBPD_PORT_COUNT] =
{
  {
    .DPM_SNKRequestedPower =                                             /*!< Requested Power by the sink board                                    */
    {
      .MaxOperatingCurrentInmAunits = USBPD_CORE_PDO_SNK_FIXED_MAX_CURRENT,
      .OperatingVoltageInmVunits    = USBPD_BOARD_REQUESTED_VOLTAGE_MV,
      .MaxOperatingVoltageInmVunits = USBPD_BOARD_MAX_VOLTAGE_MV,
      .MinOperatingVoltageInmVunits = USBPD_BOARD_MIN_VOLTAGE_MV,
      .OperatingPowerInmWunits      = (USBPD_CORE_PDO_SNK_FIXED_MAX_CURRENT * USBPD_BOARD_REQUESTED_VOLTAGE_MV)/1000,
      .MaxOperatingPowerInmWunits   = (USBPD_CORE_PDO_SNK_FIXED_MAX_CURRENT * USBPD_BOARD_MAX_VOLTAGE_MV)/1000
    },
    .PE_DataSwap = USBPD_TRUE,                  /* support data swap    */
    .PE_VconnSwap = USBPD_FALSE,                /* support VCONN swap   */
#if defined(_GUI_INTERFACE)
    .PWR_AccessoryDetection     = USBPD_FALSE,  /*!< It enables or disables powered accessory detection */
    .PWR_AccessoryTransition    = USBPD_FALSE,  /*!< It enables or disables transition from Powered.accessory to Try.SNK */
    .PWR_UnconstrainedPower     = USBPD_CORE_PDO_NOT_EXT_POWERED, /*!< UUT has an external power source available that is sufficient to adequately power the system while charging external devices or the UUT’s primary function is to charge external devices. */
    .PWR_RpResistorValue        = vRd_3_0A,     /*!< RP resitor value based on @ref CAD_SNK_Source_Current_Adv_Typedef */
    .USB_Support                = USBPD_CORE_PDO_USBCOMM_NOT_CAPABLE, /*!< USB_Comms_Capable, is the UUT capable of enumerating as a USB host or device? */
    .USB_Device                 = USBPD_FALSE,  /*!< Type_C_Can_Act_As_Device, Indicates whether the UUT can communicate with USB 2.0 or USB 3.1 as a device or as the Upstream Facing Port of a hub. */
    .USB_Host                   = USBPD_FALSE,  /*!<  Type_C_Can_Act_As_Host, Indicates whether the UUT can communicate with USB 2.0 or USB 3.1 as a host or as the Downstream Facing Port of a hub */
    .USB_SuspendSupport         = USBPD_CORE_PDO_USBSUSP_NOT_SUPPORTED, /*!<  USB Suspend support values in PDO definition (Source) */
    .CAD_tDRP                   = 80,           /*!<  Type_C_Can_Act_As_Host, Indicates whether the UUT can communicate with USB 2.0 or USB 3.1 as a host or as the Downstream Facing Port of a hub */
    .CAD_dcSRC_DRP              = 50,           /*!<  USB Suspend support values in PDO definition (Source) */
#endif /* _GUI_INTERFACE */
  },
};
#endif /* !__USBPD_DPM_CORE_C */

/* Exported define -----------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported variables --------------------------------------------------------*/
/* Exported functions --------------------------------------------------------*/

#ifdef __cplusplus
}
#endif

#endif /* __USBPD_DPM_CONF_H_ */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
