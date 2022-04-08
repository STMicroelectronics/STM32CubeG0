/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    usbpd_dpm_conf.h
  * @author  MCD Application Team
  * @brief   Header file for stack/application settings file
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2018 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

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
#include "gui_api.h"
#include "usbpd_gui_memmap.h"
#endif /* _GUI_INTERFACE */
/* USER CODE BEGIN Includes */
/* Section where include file can be added */

/* USER CODE END Includes */

/* Define   ------------------------------------------------------------------*/
/* Define VID, PID,... manufacturer parameters */
#define USBPD_VID (0x0483u)     /*!< Vendor ID (assigned by the USB-IF)                     */
#define USBPD_PID (0x0002u)     /*!< Product ID (assigned by the manufacturer)              */
#define USBPD_XID (0x0000AAAAU) /*!< Value provided by the USB-IF assigned to the product   */

/* USER CODE BEGIN Define */
/* Section where Define can be added */

/* USER CODE END Define */

/* Exported typedef ----------------------------------------------------------*/
/* USER CODE BEGIN Typedef */
/* Section where Typedef can be added */

/* USER CODE END Typedef */

/* Private variables ---------------------------------------------------------*/
#ifndef __USBPD_DPM_CORE_C
extern USBPD_SettingsTypeDef            DPM_Settings[USBPD_PORT_COUNT];
extern USBPD_IdSettingsTypeDef          DPM_ID_Settings[USBPD_PORT_COUNT];
extern USBPD_USER_SettingsTypeDef       DPM_USER_Settings[USBPD_PORT_COUNT];
#else /* __USBPD_DPM_CORE_C */
USBPD_SettingsTypeDef       DPM_Settings[USBPD_PORT_COUNT] =
{
  {
    .PE_SupportedSOP = USBPD_SUPPORTED_SOP_SOP | USBPD_SUPPORTED_SOP_SOP1 | USBPD_SUPPORTED_SOP_SOP2, /* Supported SOP : SOP, SOP' SOP" SOP'Debug SOP"Debug      */
    .PE_SpecRevision = USBPD_SPECIFICATION_REV3,/* spec revision value                                     */
    .PE_DefaultRole = USBPD_PORTPOWERROLE_SNK,  /* Default port role                                       */
    .PE_RoleSwap = USBPD_TRUE,                  /* support port role swap                                  */
    .PE_VDMSupport = USBPD_TRUE,
    .PE_RespondsToDiscovSOP = USBPD_TRUE,       /*!< Can respond successfully to a Discover Identity */
    .PE_AttemptsDiscovSOP = USBPD_TRUE,         /*!< Can send a Discover Identity */
    .PE_PingSupport = USBPD_FALSE,              /* support Ping (only for PD3.0)                           */
    .PE_CapscounterSupport = USBPD_FALSE,       /* support caps counter                                    */
    .CAD_RoleToggle = USBPD_TRUE,               /* cad role toggle                                         */
    .CAD_TryFeature = USBPD_FALSE,              /* cad try feature                                         */
    .CAD_AccesorySupport = USBPD_FALSE,         /* cas accessory support                                   */
    .PE_PD3_Support.d =                         /*!< PD3 SUPPORT FEATURE                                              */
    {
      .PE_UnchunkSupport                = USBPD_FALSE,  /*!< Unchunked mode Support                */
      .PE_FastRoleSwapSupport           = USBPD_FALSE,  /* support fast role swap only spec revision 3.0            */
      .Is_GetPPSStatus_Supported        = USBPD_TRUE,   /*!< PPS message supported or not by DPM */
      .Is_SrcCapaExt_Supported          = USBPD_TRUE,   /*!< Source_Capabilities_Extended message supported or not by DPM */
      .Is_Alert_Supported               = USBPD_FALSE,  /*!< Alert message supported or not by DPM */
      .Is_GetStatus_Supported           = USBPD_TRUE,   /*!< Status message supported or not by DPM (Is_Alert_Supported should be enabled) */
      .Is_GetManufacturerInfo_Supported = USBPD_FALSE,  /*!< Manufacturer_Info message supported or not by DPM */
      .Is_GetCountryCodes_Supported     = USBPD_FALSE,  /*!< Country_Codes message supported or not by DPM */
      .Is_GetCountryInfo_Supported      = USBPD_FALSE,  /*!< Country_Info message supported or not by DPM */
      .Is_SecurityRequest_Supported     = USBPD_FALSE,  /*!< Security_Response message supported or not by DPM */
      .Is_FirmUpdateRequest_Supported   = USBPD_FALSE,  /*!< Firmware update response message supported by PE */
      .Is_GetBattery_Supported          = USBPD_FALSE,  /*!< Get Battery Capabitity and Status messages supported by PE */
    },
    .CAD_DefaultResistor = vRp_3_0A,
    .CAD_SRCToggleTime = 40,                    /* uint8_t CAD_SRCToggleTime; */
    .CAD_SNKToggleTime = 40,                    /* uint8_t CAD_SNKToggleTime; */
  },
  {
    .PE_SupportedSOP = USBPD_SUPPORTED_SOP_SOP, /* Supported SOP : SOP, SOP' SOP" SOP'Debug SOP"Debug      */
    .PE_SpecRevision = USBPD_SPECIFICATION_REV3,/* spec revision value                                     */
    .PE_DefaultRole = USBPD_PORTPOWERROLE_SNK,  /* Default port role                                       */
    .PE_RoleSwap = USBPD_FALSE,                 /* support port role swap                                  */
    .PE_VDMSupport = USBPD_TRUE,                /* support VDM                                             */
    .PE_RespondsToDiscovSOP = USBPD_TRUE,       /*!< Can respond successfully to a Discover Identity */
    .PE_AttemptsDiscovSOP = USBPD_TRUE,         /*!< Can send a Discover Identity */
    .PE_PingSupport = USBPD_FALSE,              /* support Ping (only for PD3.0)                           */
    .PE_CapscounterSupport = USBPD_FALSE,       /* support caps counter                                    */
    .CAD_RoleToggle = USBPD_FALSE,              /* cad role toggle                                         */
    .CAD_TryFeature = USBPD_FALSE,              /* cad try feature                                         */
    .CAD_AccesorySupport = USBPD_FALSE,         /* cas accessory support                                   */
    .PE_PD3_Support.d =                         /*!< PD3 SUPPORT FEATURE                                              */
    {
      .PE_UnchunkSupport                = USBPD_FALSE,  /*!< Unchunked mode Support                */
      .PE_FastRoleSwapSupport           = USBPD_FALSE,  /*!< support fast role swap only spec revision 3.0            */
      .Is_GetPPSStatus_Supported        = USBPD_TRUE,   /*!< PPS message supported or not by DPM */
      .Is_SrcCapaExt_Supported          = USBPD_TRUE,   /*!< Source_Capabilities_Extended message supported or not by DPM */
      .Is_Alert_Supported               = USBPD_FALSE,  /*!< Alert message supported or not by DPM */
      .Is_GetStatus_Supported           = USBPD_TRUE,   /*!< Status message supported or not by DPM (Is_Alert_Supported should be enabled) */
      .Is_GetManufacturerInfo_Supported = USBPD_FALSE,  /*!< Manufacturer_Info message supported or not by DPM */
      .Is_GetCountryCodes_Supported     = USBPD_FALSE,  /*!< Country_Codes message supported or not by DPM */
      .Is_GetCountryInfo_Supported      = USBPD_FALSE,  /*!< Country_Info message supported or not by DPM */
      .Is_SecurityRequest_Supported     = USBPD_FALSE,  /*!< Security_Response message supported or not by DPM */
      .Is_FirmUpdateRequest_Supported   = USBPD_FALSE,  /*!< Firmware update response message not supported by PE */
      .Is_GetBattery_Supported          = USBPD_FALSE,  /*!< Get Battery Capabitity and Status messages supported by PE */
    },
    .CAD_DefaultResistor = vRp_3_0A,
    .CAD_SRCToggleTime = 40,                    /* uint8_t CAD_SRCToggleTime; */
    .CAD_SNKToggleTime = 40,                    /* uint8_t CAD_SNKToggleTime; */
  }
};

/* USER CODE BEGIN Variable */
/* Section where Variable can be added */
USBPD_IdSettingsTypeDef          DPM_ID_Settings[USBPD_PORT_COUNT] =
{
  {
    .XID = USBPD_XID,     /*!< Value provided by the USB-IF assigned to the product   */
    .VID = USBPD_VID,     /*!< Vendor ID (assigned by the USB-IF)                     */
    .PID = USBPD_PID,     /*!< Product ID (assigned by the manufacturer)              */
      },
#if USBPD_PORT_COUNT >= 2
  {
    .XID = USBPD_XID,     /*!< Value provided by the USB-IF assigned to the product   */
    .VID = USBPD_VID,     /*!< Vendor ID (assigned by the USB-IF)                     */
    .PID = USBPD_PID,     /*!< Product ID (assigned by the manufacturer)              */
  }
#endif /* USBPD_PORT_COUNT >= 2 */
};

USBPD_USER_SettingsTypeDef       DPM_USER_Settings[USBPD_PORT_COUNT] =
{
  {
    .DPM_SNKRequestedPower =                        /*!< Requested Power by the sink board                                    */
    {
      .MaxOperatingCurrentInmAunits = USBPD_CORE_PDO_SNK_FIXED_MAX_CURRENT,
      .OperatingVoltageInmVunits    = USBPD_BOARD_REQUESTED_VOLTAGE_MV,
      .MaxOperatingVoltageInmVunits = USBPD_BOARD_MAX_VOLTAGE_MV,
      .MinOperatingVoltageInmVunits = USBPD_BOARD_MIN_VOLTAGE_MV_PORT0,
      .OperatingPowerInmWunits      = (USBPD_CORE_PDO_SNK_FIXED_MAX_CURRENT * USBPD_BOARD_REQUESTED_VOLTAGE_MV)/1000,
      .MaxOperatingPowerInmWunits   = (USBPD_CORE_PDO_SNK_FIXED_MAX_CURRENT * USBPD_BOARD_MAX_VOLTAGE_MV)/1000
    },
    .PE_DataSwap = USBPD_TRUE,                  /* support data swap                                       */
    .PE_DR_Swap_To_DFP = USBPD_TRUE,                  /*  Support of DR Swap to DFP                                  */
    .PE_DR_Swap_To_UFP = USBPD_TRUE,                  /*  Support of DR Swap to UFP                                  */
    .PE_VconnSwap = USBPD_TRUE,                 /* support VCONN swap                                  */
    .DPM_SRCExtendedCapa =                        /*!< SRC Extended Capability           */
      {
        .VID = USBPD_VID,      /*!< Vendor ID (assigned by the USB-IF)                   */
        .PID = USBPD_PID,      /*!< Product ID (assigned by the manufacturer)            */
        .XID = USBPD_XID,      /*!< Value provided by the USB-IF assigned to the product */
        .FW_revision = 1,      /*!< Firmware version number                              */
        .HW_revision = 2,      /*!< Hardware version number                              */
        .Voltage_regulation= 0,/*!< Voltage Regulation                                   */
        .Holdup_time = 0,      /*!< Holdup Time                                          */
        .Compliance = 0,       /*!< Compliance                                           */
        .TouchCurrent = 0,     /*!< Touch Current                                        */
        .PeakCurrent1 = 0,     /*!< Peak Current1                                        */
        .PeakCurrent2 = 0,     /*!< Peak Current2                                        */
        .PeakCurrent3 = 0,     /*!< Peak Current3                                        */
        .Touchtemp = 0,        /*!< Touch Temp                                           */
        .Source_inputs = 0,    /*!< Source Inputs                                        */
        .NbBatteries = 0,      /*!< Number of Batteries/Battery Slots                    */
        .SourcePDP = (uint8_t)USBPD_PDP_SRC_IN_WATTS,       /*!< Source PDP  5V*3A                                    */
      },
    .DPM_SNKExtendedCapa =                        /*!< SRC Extended Capability           */
      {
        .VID = USBPD_VID,      /*!< Vendor ID (assigned by the USB-IF)                   */
        .PID = USBPD_PID,      /*!< Product ID (assigned by the manufacturer)            */
        .XID = USBPD_XID,      /*!< Value provided by the USB-IF assigned to the product */
        .FW_revision        = 1,         /*!< Firmware version number                                        */
        .HW_revision        = 2,         /*!< Hardware version number                                        */
        .SKEDB_Version      = USBPD_SKEDB_VERSION_1P0, /*!< SKEDB Version (not the specification Version)
                                                            based on @ref USBPD_SKEDB_VERSION                */
        .LoadStep           = USBPD_SKEDB_LOADSTEP_150MA, /*!< Load Step based on @ref USBPD_SKEDB_LOADSTEP  */
        .SinkLoadCharac.b   =          /*!< Sink Load Characteristics                */
        {
          .PercentOverload  = 0,         /*!< Percent overload in 10% increments Values higher than 25
                                               (11001b) are clipped to 250%. 00000b is the default.    */
          .OverloadPeriod   = 0,         /*!< Overload period in 20ms when bits 0-4 non-zero.          */
          .DutyCycle        = 0,         /*!< Duty cycle in 5% increments when bits 0-4 are non-zero.  */
          .VBusVoltageDrop  = 0,         /*!< Can tolerate VBUS Voltage drop.                          */
        },
        .Compliance         = 0,         /*!< Compliance based on combination of @ref USBPD_SKEDB_COMPLIANCE */
        .Touchtemp          = USBPD_SKEDB_TOUCHTEMP_NA, /*!< Touch Temp based on @ref USBPD_SKEDB_TOUCHTEMP  */
        .BatteryInfo        = 0,         /*!< Battery info                                                   */
        .SinkModes          = 0,         /*!< Sink Modes based on combination of @ref USBPD_SKEDB_SINKMODES  */
        .SinkMinimumPDP     = 0,         /*!< The Minimum PDP required by the Sink to operate without
                                              consuming any power from its Battery(s) should it have one     */
        .SinkOperationalPDP = 0,         /*!< The PDP the Sink requires to operate normally. For Sinks with
                                              a Battery, it is the PDP Rating of the charger supplied with
                                              it or recommended for it.                                      */
        .SinkMaximumPDP     = 0,         /*!< The Maximum PDP the Sink can consume to operate and
                                              charge its Battery(s) should it have one.                      */
      },
    .DPM_ManuInfoPort =                      /*!< Manufacturer information used for the port            */
    {
      .VID = USBPD_VID,                      /*!< Vendor ID (assigned by the USB-IF)        */
      .PID = USBPD_PID,                      /*!< Product ID (assigned by the manufacturer) */
      .ManuString = "STMicroelectronics",    /*!< Vendor defined byte array                 */
    },
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
  {
    .DPM_SNKRequestedPower =                                             /*!< Requested Power by the sink board                                    */
    {
      .MaxOperatingCurrentInmAunits = USBPD_CORE_PDO_SNK_FIXED_MAX_CURRENT,
      .OperatingVoltageInmVunits    = USBPD_BOARD_REQUESTED_VOLTAGE_MV,
      .MaxOperatingVoltageInmVunits = USBPD_BOARD_MAX_VOLTAGE_MV,
      .MinOperatingVoltageInmVunits = USBPD_BOARD_MIN_VOLTAGE_MV_PORT1,
      .OperatingPowerInmWunits      = (USBPD_CORE_PDO_SNK_FIXED_MAX_CURRENT * USBPD_BOARD_REQUESTED_VOLTAGE_MV)/1000,
      .MaxOperatingPowerInmWunits   = (USBPD_CORE_PDO_SNK_FIXED_MAX_CURRENT * USBPD_BOARD_MAX_VOLTAGE_MV)/1000
    },
    .PE_DataSwap = USBPD_TRUE,                  /* support data swap                                       */
    .PE_DR_Swap_To_DFP = USBPD_TRUE,                  /*  Support of DR Swap to DFP                                  */
    .PE_DR_Swap_To_UFP = USBPD_TRUE,                  /*  Support of DR Swap to UFP                                  */
    .PE_VconnSwap = USBPD_FALSE,                /* support VCONN swap                                  */
    .DPM_SRCExtendedCapa =                /*!< SRC Extended Capability                              */
      {
        .VID                = USBPD_VID,  /*!< Vendor ID (assigned by the USB-IF)                   */
        .PID                = USBPD_PID,  /*!< Product ID (assigned by the manufacturer)            */
        .XID                = USBPD_XID,  /*!< Value provided by the USB-IF assigned to the product */
        .FW_revision        = 1,          /*!< Firmware version number                              */
        .HW_revision        = 2,          /*!< Hardware version number                              */
        .Voltage_regulation = 0,          /*!< Voltage Regulation                                   */
        .Holdup_time        = 0,          /*!< Holdup Time                                          */
        .Compliance         = 0,          /*!< Compliance                                           */
        .TouchCurrent       = 0,          /*!< Touch Current                                        */
        .PeakCurrent1       = 0,          /*!< Peak Current1                                        */
        .PeakCurrent2       = 0,          /*!< Peak Current2                                        */
        .PeakCurrent3       = 0,          /*!< Peak Current3                                        */
        .Touchtemp          = 0,          /*!< Touch Temp                                           */
        .Source_inputs      = 0,          /*!< Source Inputs                                        */
        .NbBatteries        = 0,          /*!< Number of Batteries/Battery Slots                    */
        .SourcePDP          = (uint8_t)USBPD_PDP_SRC_IN_WATTS, /*!< Source PDP  5V*3A               */
      },
    .DPM_SNKExtendedCapa =                        /*!< SRC Extended Capability           */
      {
        .VID                = USBPD_VID, /*!< Vendor ID (assigned by the USB-IF)                             */
        .PID                = USBPD_PID, /*!< Product ID (assigned by the manufacturer)                      */
        .XID                = USBPD_XID, /*!< Value provided by the USB-IF assigned to the product           */
        .FW_revision        = 1,         /*!< Firmware version number                                        */
        .HW_revision        = 2,         /*!< Hardware version number                                        */
        .SKEDB_Version      = USBPD_SKEDB_VERSION_1P0, /*!< SKEDB Version (not the specification Version)
                                                            based on @ref USBPD_SKEDB_VERSION                */
        .LoadStep           = USBPD_SKEDB_LOADSTEP_150MA, /*!< Load Step based on @ref USBPD_SKEDB_LOADSTEP  */
        .SinkLoadCharac.b   =          /*!< Sink Load Characteristics                */
        {
          .PercentOverload  = 0,         /*!< Percent overload in 10% increments Values higher than 25
                                               (11001b) are clipped to 250%. 00000b is the default.    */
          .OverloadPeriod   = 0,         /*!< Overload period in 20ms when bits 0-4 non-zero.          */
          .DutyCycle        = 0,         /*!< Duty cycle in 5% increments when bits 0-4 are non-zero.  */
          .VBusVoltageDrop  = 0,         /*!< Can tolerate VBUS Voltage drop.                          */
        },
        .Compliance         = 0,         /*!< Compliance based on combination of @ref USBPD_SKEDB_COMPLIANCE */
        .Touchtemp          = USBPD_SKEDB_TOUCHTEMP_NA, /*!< Touch Temp based on @ref USBPD_SKEDB_TOUCHTEMP  */
        .BatteryInfo        = 0,         /*!< Battery info                                                   */
        .SinkModes          = 0,         /*!< Sink Modes based on combination of @ref USBPD_SKEDB_SINKMODES  */
        .SinkMinimumPDP     = 0,         /*!< The Minimum PDP required by the Sink to operate without
                                              consuming any power from its Battery(s) should it have one     */
        .SinkOperationalPDP = 0,         /*!< The PDP the Sink requires to operate normally. For Sinks with
                                              a Battery, it is the PDP Rating of the charger supplied with
                                              it or recommended for it.                                      */
        .SinkMaximumPDP     = 0,         /*!< The Maximum PDP the Sink can consume to operate and
                                              charge its Battery(s) should it have one.                      */
      },
    .DPM_ManuInfoPort =                      /*!< Manufacturer information used for the port            */
    {
      .VID = USBPD_VID,                      /*!< Vendor ID (assigned by the USB-IF)        */
      .PID = USBPD_PID,                      /*!< Product ID (assigned by the manufacturer) */
      .ManuString = "STMicroelectronics",    /*!< Vendor defined byte array                 */
    },
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
  }
};
#endif /* !__USBPD_DPM_CORE_C */
/* USER CODE END Variable */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN Constant */
/* Section where Constant can be added */

/* USER CODE END Constant */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN Macro */
/* Section where Macro can be added */

/* USER CODE END Macro */

#ifdef __cplusplus
}
#endif

#endif /* __USBPD_DPM_CONF_H_ */
