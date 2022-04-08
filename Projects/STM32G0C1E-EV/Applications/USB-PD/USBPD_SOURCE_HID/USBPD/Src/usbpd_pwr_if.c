/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    usbpd_pwr_if.c
  * @author  MCD Application Team
  * @brief   This file contains power interface control functions.
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
/* USER CODE END Header */

#define __USBPD_PWR_IF_C

/* Includes ------------------------------------------------------------------*/
#include "usbpd_pwr_if.h"
#include "usbpd_hw_if.h"
#include "usbpd_dpm_core.h"
#include "usbpd_dpm_conf.h"
#include "usbpd_pdo_defs.h"
#include "usbpd_core.h"
#if defined(_TRACE)
#include "usbpd_trace.h"
#endif /* _TRACE */
#include "string.h"
#include "gui_api.h"
/* USER CODE BEGIN Include */

/* USER CODE END Include */

/** @addtogroup STM32_USBPD_APPLICATION
  * @{
  */

/** @addtogroup STM32_USBPD_APPLICATION_POWER_IF
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/** @addtogroup STM32_USBPD_APPLICATION_POWER_IF_Private_TypeDef
  * @{
  */
/* USER CODE BEGIN Private_Typedef */

/* USER CODE END Private_Typedef */
/**
  * @}
  */

/* Private define ------------------------------------------------------------*/
/** @addtogroup STM32_USBPD_APPLICATION_POWER_IF_Private_Defines
  * @{
  */
/* USER CODE BEGIN Private_Define */

/* USER CODE END Private_Define */
/**
  * @}
  */

/* Private macros ------------------------------------------------------------*/
/** @addtogroup STM32_USBPD_APPLICATION_POWER_IF_Private_Macros
  * @{
  */
#if defined(_TRACE)
#define PWR_IF_DEBUG_TRACE(_PORT_, __MESSAGE__)  USBPD_TRACE_Add(USBPD_TRACE_DEBUG, (_PORT_), 0u, (uint8_t*)(__MESSAGE__), sizeof(__MESSAGE__) - 1u)
#else
#define PWR_IF_DEBUG_TRACE(_PORT_, __MESSAGE__)
#endif /* _TRACE */
/* USER CODE BEGIN Private_Macro */

/* USER CODE END Private_Macro */
/**
  * @}
  */

/* Private variables ---------------------------------------------------------*/
/** @addtogroup STM32_USBPD_APPLICATION_POWER_IF_Private_Variables
  * @{
  */
/* USER CODE BEGIN Private_Variables */
/**
  * @brief  USBPD Port PDO Storage array declaration
  */

/**** PDO ****/
USBPD_PWR_Port_PDO_Storage_TypeDef PWR_Port_PDO_Storage[USBPD_PORT_COUNT];
/* USER CODE END Private_Variables */
/**
  * @}
  */

/* Private function prototypes -----------------------------------------------*/
/** @addtogroup STM32_USBPD_APPLICATION_POWER_IF_Private_Functions
  * @{
  */
/* USER CODE BEGIN USBPD_USER_PRIVATE_FUNCTIONS_Prototypes */

/* USER CODE END USBPD_USER_PRIVATE_FUNCTIONS_Prototypes */
/**
  * @}
  */

/** @addtogroup STM32_USBPD_APPLICATION_POWER_IF_Exported_Functions
  * @{
  */

/**
  * @brief  Initialize structures and variables related to power board profiles
  *         used by Sink and Source, for all available ports.
  * @retval USBPD status
  */
USBPD_StatusTypeDef USBPD_PWR_IF_Init(void)
{
/* USER CODE BEGIN USBPD_PWR_IF_Init */
  USBPD_StatusTypeDef _status = USBPD_OK;

  /* Set links to PDO values and number for Port 0 (defined in PDO arrays in H file).
   */
  PWR_Port_PDO_Storage[USBPD_PORT_0].SourcePDO.ListOfPDO = (uint32_t *) PORT0_PDO_ListSRC;
  PWR_Port_PDO_Storage[USBPD_PORT_0].SourcePDO.NumberOfPDO = &USBPD_NbPDO[1];

  return _status;
/* USER CODE END USBPD_PWR_IF_Init */
}

/**
  * @brief  Sets the required power profile, now it works only with Fixed ones
  * @param  PortNum Port number
  * @retval USBPD status
*/
USBPD_StatusTypeDef USBPD_PWR_IF_SetProfile(uint8_t PortNum)
{
/* USER CODE BEGIN USBPD_PWR_IF_SetProfile */
  USBPD_StatusTypeDef      _ret = USBPD_ERROR;
  USBPD_PDO_TypeDef        _pdo;
  USBPD_SNKRDO_TypeDef     _rdo;

  _rdo.d32 = DPM_Ports[PortNum].DPM_RcvRequestDOMsg;
  if (0 == PortNum)
    _pdo.d32 = PORT0_PDO_ListSRC[0];
  else
    _pdo.d32 = PORT0_PDO_ListSRC[0];

  if(PWR_OK == BSP_USBPD_PWR_VBUSSetVoltage_Fixed(PortNum, _pdo.SRCFixedPDO.VoltageIn50mVunits * 50,
                                                     (_rdo.FixedVariableRDO.OperatingCurrentIn10mAunits * 10),
                                                     (_rdo.FixedVariableRDO.MaxOperatingCurrent10mAunits * 10)))
  {
     _ret = USBPD_OK;
  }

  return _ret;
/* USER CODE END USBPD_PWR_IF_SetProfile */
}

/**
  * @brief  Checks if the power on a specified port is ready
  * @param  PortNum Port number
  * @param  Vsafe   Vsafe status based on @ref USBPD_VSAFE_StatusTypeDef
  * @retval USBPD status
  */
USBPD_StatusTypeDef USBPD_PWR_IF_SupplyReady(uint8_t PortNum, USBPD_VSAFE_StatusTypeDef Vsafe)
{
/* USER CODE BEGIN USBPD_PWR_IF_SupplyReady */
  return USBPD_ERROR;
/* USER CODE END USBPD_PWR_IF_SupplyReady */
}

/**
  * @brief  Enables VBUS power on a specified port
  * @param  PortNum Port number
  * @retval USBPD status
  */
USBPD_StatusTypeDef USBPD_PWR_IF_VBUSEnable(uint8_t PortNum)
{
/* USER CODE BEGIN USBPD_PWR_IF_VBUSEnable */
  USBPD_StatusTypeDef _status = (USBPD_StatusTypeDef)HW_IF_PWR_Enable(PortNum, USBPD_ENABLE, CCNONE, USBPD_FALSE, USBPD_PORTPOWERROLE_SRC);
  return _status;
/* USER CODE END USBPD_PWR_IF_VBUSEnable */
}

/**
  * @brief  Disable VBUS/VCONN the power on a specified port
  * @param  PortNum Port number
  * @retval USBPD status
  */
USBPD_StatusTypeDef USBPD_PWR_IF_VBUSDisable(uint8_t PortNum)
{
/* USER CODE BEGIN USBPD_PWR_IF_VBUSDisable */
  USBPD_StatusTypeDef _status = (USBPD_StatusTypeDef)HW_IF_PWR_Enable(PortNum, USBPD_DISABLE, CCNONE, USBPD_FALSE, USBPD_PORTPOWERROLE_SRC);
  return _status;
/* USER CODE END USBPD_PWR_IF_VBUSDisable */
}

/**
  * @brief  Checks if the power on a specified port is enabled
  * @param  PortNum Port number
  * @retval USBPD_ENABLE or USBPD_DISABLE
  */
USBPD_FunctionalState USBPD_PWR_IF_VBUSIsEnabled(uint8_t PortNum)
{
  /* Get the Status of the port */
  return USBPD_PORT_IsValid(PortNum) ? (USBPD_FunctionalState)HW_IF_PWR_VBUSIsEnabled(PortNum) : USBPD_DISABLE;
}

/**
  * @brief  Reads the voltage and the current on a specified port
  * @param  PortNum Port number
  * @param  pVoltage: The Voltage in mV
  * @param  pCurrent: The Current in mA
  * @retval USBPD_ERROR or USBPD_OK
  */
USBPD_StatusTypeDef USBPD_PWR_IF_ReadVA(uint8_t PortNum, uint16_t *pVoltage, uint16_t *pCurrent)
{
/* USER CODE BEGIN USBPD_PWR_IF_ReadVA */
  return USBPD_ERROR;
/* USER CODE END USBPD_PWR_IF_ReadVA */
}

/**
  * @brief  Enables the VConn on the port.
  * @param  PortNum Port number
  * @param  CC      Specifies the CCx to be selected based on @ref CCxPin_TypeDef structure
  * @retval USBPD status
  */
USBPD_StatusTypeDef USBPD_PWR_IF_Enable_VConn(uint8_t PortNum, CCxPin_TypeDef CC)
{
/* USER CODE BEGIN USBPD_PWR_IF_Enable_VConn */
  return USBPD_ERROR;
/* USER CODE END USBPD_PWR_IF_Enable_VConn */
}

/**
  * @brief  Disable the VConn on the port.
  * @param  PortNum Port number
  * @param  CC      Specifies the CCx to be selected based on @ref CCxPin_TypeDef structure
  * @retval USBPD status
  */
USBPD_StatusTypeDef USBPD_PWR_IF_Disable_VConn(uint8_t PortNum, CCxPin_TypeDef CC)
{
/* USER CODE BEGIN USBPD_PWR_IF_Disable_VConn */
  return USBPD_ERROR;
/* USER CODE END USBPD_PWR_IF_Disable_VConn */
}

/**
  * @brief  Allow PDO data reading from PWR_IF storage.
  * @param  PortNum Port number
  * @param  DataId Type of data to be read from PWR_IF
  *         This parameter can be one of the following values:
  *           @arg @ref USBPD_CORE_DATATYPE_SRC_PDO Source PDO reading requested
  *           @arg @ref USBPD_CORE_DATATYPE_SNK_PDO Sink PDO reading requested
  * @param  Ptr Pointer on address where PDO values should be written (u8 pointer)
  * @param  Size Pointer on nb of u32 written by PWR_IF (nb of PDOs)
  * @retval None
  */
void USBPD_PWR_IF_GetPortPDOs(uint8_t PortNum, USBPD_CORE_DataInfoType_TypeDef DataId, uint8_t *Ptr, uint32_t *Size)
{
    if (DataId == USBPD_CORE_DATATYPE_SRC_PDO)
    {
      *Size = USBPD_NbPDO[1];
      memcpy(Ptr,PORT0_PDO_ListSRC, sizeof(uint32_t) * USBPD_NbPDO[1]);
    }
/* USER CODE BEGIN USBPD_PWR_IF_GetPortPDOs */
  uint32_t   nbpdo, index, nb_valid_pdo = 0;
  uint32_t   *ptpdoarray = NULL;
  USBPD_PDO_TypeDef pdo_first;
  USBPD_PDO_TypeDef pdo;
  
  /* Check if valid port */
  if (USBPD_PORT_IsValid(PortNum))
  {
    /* According to type of PDO to be read, set pointer on values and nb of elements */
    switch(DataId)
    {
    case USBPD_CORE_DATATYPE_SRC_PDO :
      nbpdo = *PWR_Port_PDO_Storage[PortNum].SourcePDO.NumberOfPDO;
      ptpdoarray = PWR_Port_PDO_Storage[PortNum].SourcePDO.ListOfPDO;
      /* Save the 1st PDO */
      pdo_first.d32 = *ptpdoarray;
      /* Reset unchunked bit if current revision is PD2.0*/
      if (USBPD_SPECIFICATION_REV2 == DPM_Params[PortNum].PE_SpecRevision)
      {
        pdo_first.SRCFixedPDO.UnchunkedExtendedMessage  = USBPD_PDO_SRC_FIXED_UNCHUNK_NOT_SUPPORTED;
      }
      break;
    default :
      nbpdo = 0;
      break;
    }
    
    /* Copy PDO data in output buffer */
    for (index = 0; index < nbpdo; index++)
    {
      pdo.d32 = *ptpdoarray;
      /* Copy only PDO (and not APDO in case of current revision is PD2.0) */
      if ((USBPD_SPECIFICATION_REV2 == DPM_Params[PortNum].PE_SpecRevision)
          && (pdo.GenericPDO.PowerObject == USBPD_CORE_PDO_TYPE_APDO))
      {
      }
      else
      {
        /* Copy 1st PDO as potentially FRS or UNCHUNKED bits have been reset */
        if (0 == index)
        {
          (void)memcpy(Ptr, (uint8_t*)&pdo_first.d32, 4u);
        }
        else
        {
          (void)memcpy((Ptr + (nb_valid_pdo * 4u)), (uint8_t*)ptpdoarray, 4u);
        }
        nb_valid_pdo++;
      }
      ptpdoarray++;
    }
    /* Set nb of read PDO (nb of u32 elements); */
    *Size = nb_valid_pdo;
  }
/* USER CODE END USBPD_PWR_IF_GetPortPDOs */
}

/**
  * @brief  Find out SRC PDO pointed out by a position provided in a Request DO (from Sink).
  * @param  PortNum Port number
  * @param  RdoPosition RDO Position in list of provided PDO
  * @param  Pdo Pointer on PDO value pointed out by RDO position (u32 pointer)
  * @retval Status of search
  *         USBPD_OK : Src PDO found for requested DO position (output Pdo parameter is set)
  *         USBPD_FAIL : Position is not compliant with current Src PDO for this port (no corresponding PDO value)
  */
USBPD_StatusTypeDef USBPD_PWR_IF_SearchRequestedPDO(uint8_t PortNum, uint32_t RdoPosition, uint32_t *Pdo)
{
/* USER CODE BEGIN USBPD_PWR_IF_SearchRequestedPDO */
  if((RdoPosition == 0) || (RdoPosition > *PWR_Port_PDO_Storage[PortNum].SourcePDO.NumberOfPDO))
  {
    /* Invalid PDO index */
  return USBPD_FAIL;
  }

  *Pdo = PWR_Port_PDO_Storage[PortNum].SourcePDO.ListOfPDO[RdoPosition - 1];
  return USBPD_OK;
/* USER CODE END USBPD_PWR_IF_SearchRequestedPDO */
}

/**
  * @brief  the function is called in case of critical issue is detected to switch in safety mode.
  * @retval None
  */
void USBPD_PWR_IF_Alarm()
{
/* USER CODE BEGIN USBPD_PWR_IF_Alarm */

/* USER CODE END USBPD_PWR_IF_Alarm */
}

/**
  * @brief Function is called to get VBUS power status.
  * @param PortNum Port number
  * @param PowerTypeStatus  Power type status based on @ref USBPD_VBUSPOWER_STATUS
  * @retval UBBPD_TRUE or USBPD_FALSE
  */
uint8_t USBPD_PWR_IF_GetVBUSStatus(uint8_t PortNum, USBPD_VBUSPOWER_STATUS PowerTypeStatus)
{
/* USER CODE BEGIN USBPD_PWR_IF_GetVBUSStatus */
  uint8_t _status = USBPD_FALSE;
  uint32_t _vbus = HW_IF_PWR_GetVoltage(PortNum);

  switch(PowerTypeStatus)
  {
  case USBPD_PWR_BELOWVSAFE0V :
    if (_vbus < USBPD_PWR_LOW_VBUS_THRESHOLD) _status = USBPD_TRUE;
    break;
  case USBPD_PWR_VSAFE5V :
    if (_vbus >= USBPD_PWR_HIGH_VBUS_THRESHOLD) _status = USBPD_TRUE;
    break;
  case USBPD_PWR_SNKDETACH:
    if (_vbus < USBPD_PWR_HIGH_VBUS_THRESHOLD) _status = USBPD_TRUE;
    break;
  default :
    break;
  }
  return _status;
/* USER CODE END USBPD_PWR_IF_GetVBUSStatus */
}

/**
  * @brief Function is called to set the VBUS threshold when a request has been accepted.
  * @param PortNum Port number
  * @retval None
  */
void USBPD_PWR_IF_UpdateVbusThreshold(uint8_t PortNum)
{
/* USER CODE BEGIN USBPD_PWR_IF_UpdateVbusThreshold */
/* USER CODE END USBPD_PWR_IF_UpdateVbusThreshold */
}

/**
  * @brief Function is called to reset the VBUS threshold when there is a power reset.
  * @param PortNum Port number
  * @retval None
  */
void USBPD_PWR_IF_ResetVbusThreshold(uint8_t PortNum)
{
/* USER CODE BEGIN USBPD_PWR_IF_ResetVbusThreshold */
/* USER CODE END USBPD_PWR_IF_ResetVbusThreshold */
}

/**
  * @}
  */

/** @addtogroup STM32_USBPD_APPLICATION_POWER_IF_Private_Functions
  * @{
  */
/* USER CODE BEGIN USBPD_USER_PRIVATE_FUNCTIONS_Definition */

/* USER CODE END USBPD_USER_PRIVATE_FUNCTIONS_Definition */
/**
  * @}
  */

/**
  * @}
  */

/**
  * @}
  */
