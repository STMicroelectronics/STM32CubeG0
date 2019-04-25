/**
  ******************************************************************************
  * @file    ina230.c
  * @author  MCD Application Team
  * @brief   This file provides a set of functions needed to manage the INA230,
  *          Bidirectional CURRENT/POWER MONITOR.  
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2019 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */
/* Includes ------------------------------------------------------------------*/
#include "ina230.h"

/** @addtogroup BSP
  * @{
  */ 

/** @addtogroup Components
  * @{
  */ 

/** @addtogroup INA230
  * @{
  */

/** @defgroup INA230_Private_TypesDefinitions
  * @{
  */
  
/**
  * @}
  */

/** @defgroup INA230_Private_Defines
  * @{
  */
#define REG_MASK_ENABLE_FUNCTIONS_Msk (INA230_REG_MASK_ENABLE_SOL_Msk | \
                                       INA230_REG_MASK_ENABLE_SUL_Msk | \
                                       INA230_REG_MASK_ENABLE_BOL_Msk | \
                                       INA230_REG_MASK_ENABLE_BUL_Msk | \
                                       INA230_REG_MASK_ENABLE_POL_Msk)

#define REG_MASK_ENABLE_FLAGS_Msk (INA230_REG_MASK_ENABLE_AFF_Msk  | \
                                   INA230_REG_MASK_ENABLE_CVRF_Msk | \
                                   INA230_REG_MASK_ENABLE_OVF_Msk)

/* 1 LSB step size for VBus : 1.25 mV
 * VBus = [INA230_REG_VBUS] * 1.25 mV 
 * VBus = [INA230_REG_VBUS] * BUS_VOLTAGE_LSB/1000 
 */
#define BUS_VOLTAGE_LSB 1250

/* 1 LSB step size for  VShunt: 2.5 µV
 * VShunt = [INA230_REG_VSHUNT] * (2.5/1000) mV = ([INA230_REG_VSHUNT]/400) mV
 * VShunt = [INA230_REG_VSHUNT] * VSHUNT_DIV
 */
#define VSHUNT_DIV 400

/* 1 LSB step size for power : 25 mW (assuming Current LSB is 1 mA)
 * Power = [INA230_REG_PWR] * 25 mW 
 * Power = [INA230_REG_PWR] * POWER_LSB 
 */
#define POWER_LSB 25

/**
  * @}
  */

/** @defgroup INA230_Private_Macros
  * @{
  */
#define DIV_ROUND_CLOSEST(__DIVIDEND__, __DIVISOR__) (((__DIVIDEND__) + ((__DIVISOR__)/2)) / (__DIVISOR__))


/**
  * @}
  */ 
  
/** @defgroup INA230_Private_Variables
  * @{
  */
PWRMON_Drv_t ina230_drv =
{
  ina230_Init,
  ina230_DeInit,
  ina230_ReadId,
  ina230_Reset,
  ina230_SetCalibration,
  ina230_GetCalibration,
  ina230_SetAlertFunction,
  ina230_GetAlertFunction,
  ina230_AlertPinConfig,
  ina230_SetVBusThreshold,
  ina230_GetVBusThreshold,
  ina230_SetVShuntThreshold,
  ina230_GetVShuntThreshold,
  ina230_SetPowerThreshold,
  ina230_GetPowerThreshold,
  ina230_AlertThresholdEnableIT,
  ina230_AlertThresholdDisableIT,
  ina230_ConversionReadyEnableIT,
  ina230_ConversionReadyDisableIT,
  ina230_StartConversion,
  ina230_StopConversion,
  ina230_GetVBus,
  ina230_GetVShunt,
  ina230_GetPower,
  ina230_GetCurrent,
  ina230_GetFlag
};

static const uint16_t aMode[OPERATING_MODE_NB][VOLTAGE_INPUT_NB] = {
  {
    INA230_MODE_TRIGGERED_VSHUNT,
    INA230_MODE_TRIGGERED_VBUS,
    INA230_MODE_TRIGGERED_ALL
  },
  {
    INA230_MODE_CONTINOUS_VSHUNT,
    INA230_MODE_CONTINOUS_VBUS,
    INA230_MODE_CONTINOUS_ALL
  }
};

static const uint16_t aFlags[FLAG_NB] = 
{
  INA230_REG_MASK_ENABLE_AFF,
  INA230_REG_MASK_ENABLE_CVRF,
  INA230_REG_MASK_ENABLE_OVF
};

/**
  * @}
  */

/** @defgroup INA230_Private_FunctionPrototypes
  * @{
  */

/**
  * @brief  Read access to INA230 register 
  * @param  Address INA230 address on communication Bus.
  * @param  Reg INA230 register offset.
  * @retval Data Read INA230 register content
  */
static uint16_t ina230_ReadReg(uint16_t Address, uint8_t Reg)
{
  uint16_t data = 0;
  uint8_t dt[2];
  
  PWRMON_IO_Read(Address, Reg, dt);
  
  data = (dt[0] << 8) | dt[1];
  
  return data;
}

/**
  * @brief  Write access to INA230 register 
  * @param  Address INA230 address on communication Bus.
  * @param  Reg INA230 register offset.
  * @param  Data Data to write.
  * @retval None
  */
static void ina230_WriteReg(uint16_t Address, uint8_t Reg, uint16_t Data)
{
  uint8_t dt[2];

  dt[0] = Data >> 8;    /* MSB 1st */
  dt[1] = Data & 0xff;  /* LSB 2nd */
  
  PWRMON_IO_Write(Address, Reg, dt);
}

/**
  * @brief  Enable the ALERT interrupt
  * @note ALERT pin is asserted when when the alert function selected in the
  *       Mask/Enable register exceeds the value programmed into the Alert Limit
  *       register or when the conversion is ready or both.
  * @param  Address INA230 address on communication Bus.
  * @retval None
  */
static void ina230_EnableIT(uint16_t Address)
{
  uint16_t apol;
  
  /* Get actual alert polarity setting */
  apol = (ina230_ReadReg(Address, INA230_REG_MASK_ENABLE) & INA230_REG_MASK_ENABLE_APOL_Msk);
         
  if (apol == INA230_REG_MASK_ENABLE_APOL)
  {
    /* ALERT pin is active high */
    PWRMON_IO_EnableIT(Address, 1);
  }
  else
  {
    /* ALERT pin is active LOW (default) */
    PWRMON_IO_EnableIT(Address, 0);
  }
}

/**
  * @brief  Initialize the INA230 and configure the needed hardware resources 
  * @param  Address INA230 address on communication Bus.
  * @param  pConfig Pointer to the INA230 configuration structure.
  * @retval None
  */
void ina230_Init(uint16_t Address, PWRMON_Config_t * pConfig)
{
  uint16_t cfg = 0;
  
  /*  Low level init */
  PWRMON_IO_Init(Address);
  
  /* Reset all registers are to default values */
  ina230_Reset(Address);
  
  /* Set INA230 configuration */
  cfg = (cfg & ((uint16_t)~(INA230_REG_CFG_VSHUNTCT_Msk))) | (pConfig->ShuntConvertTime << INA230_REG_CFG_VSHUNTCT_Pos);
  cfg = (cfg & ((uint16_t)~(INA230_REG_CFG_VBUSCT_Msk))) | (pConfig->BusConvertTime << INA230_REG_CFG_VBUSCT_Pos);
  cfg = (cfg & ((uint16_t)~(INA230_REG_CFG_AVG_Msk))) | (pConfig->AveragingMode << INA230_REG_CFG_AVG_Pos);
  ina230_WriteReg(Address, INA230_REG_CONFIG, cfg);  
}

/**
  * @brief  Release the hardware resources required to use the INA230
  * @param  Address INA230 address on communication Bus.
  * @retval none
  */
void ina230_DeInit(uint16_t Address)
{
  /* Reset all registers are to default values */
  ina230_Reset(Address);
  
  /*  Low level de-init */
  PWRMON_IO_DeInit(Address);
}

/**
  * @brief  Read the INA230 die identifier
  * @param  Address INA230 address on communication Bus.
  * @retval Id INA230 die identified
  */
uint16_t ina230_ReadId(uint16_t Address)
{
  return ina230_ReadReg(Address, INA230_REG_ID);
}

/**
  * @brief  Generate a system reset
  * @note All registers are reset to default values
  * @param  Address INA230 address on communication Bus.
  * @retval None
  */
void ina230_Reset(uint16_t Address)
{
  uint16_t cfg = 0;
  
  /*  Read programmed configuration  */
  cfg = ina230_ReadReg(Address, INA230_REG_CONFIG);
  
  /* Set Reset bit bit */
  cfg |= INA230_REG_CFG_RESET;
  ina230_WriteReg(Address, INA230_REG_CONFIG, cfg);  
}


/**
  * @brief  Programm the INA230 calibration register
  * @note INA230 does not necessarily measure current or power. If the
  *       Calibration register is not programmed, the result of the current
  *       and power calculations is zero.
  * @param  Address INA230 address on communication Bus.
  * @param  Calibration Calibration value.
  * @retval None
  */
void ina230_SetCalibration(uint16_t Address, uint16_t Calibration)
{
  ina230_WriteReg(Address, INA230_REG_CALIBRATION, Calibration); 
}

/**
  * @brief  Read programmed calibration value
  * @param  Address INA230 address on communication Bus.
  * @retval Calibration Calibration value.
  */
uint16_t ina230_GetCalibration(uint16_t Address)
{
  return ina230_ReadReg(Address, INA230_REG_CALIBRATION);
}

/**
  * @brief  Set the alert function
  * @param  Address INA230 address on communication Bus.
  * @param  AlertFunction Monitored alert function.
  *         This parameter can be take one of the following values:
  *         ALERT_FUNCTION_NONE
  *         ALERT_FUNCTION_SOL
  *         ALERT_FUNCTION_SUL
  *         ALERT_FUNCTION_B0L
  *         ALERT_FUNCTION_BUL
  *         ALERT_FUNCTION_POL
  * @retval None.
  */
void ina230_SetAlertFunction(uint16_t Address, PWRMON_AlertFunction_t AlertFunction)
{
  uint16_t mask_en;
  
  /* Read programmed Alert configuration */
  mask_en = ina230_ReadReg(Address, INA230_REG_MASK_ENABLE);
  
  /* Set Alert function */
  switch(AlertFunction)
  {
  case ALERT_FUNCTION_NONE:
    mask_en = mask_en & ((uint16_t)~(REG_MASK_ENABLE_FUNCTIONS_Msk));
    break;
  case ALERT_FUNCTION_SOL:
    mask_en = INA230_REG_MASK_ENABLE_SOL;
    break;
  case ALERT_FUNCTION_SUL:  
    mask_en = INA230_REG_MASK_ENABLE_SUL;
    break;
  case ALERT_FUNCTION_BOL:  
    mask_en = INA230_REG_MASK_ENABLE_BOL;
    break;
  case ALERT_FUNCTION_BUL:  
    mask_en = INA230_REG_MASK_ENABLE_BUL;
    break;
  case ALERT_FUNCTION_POL:
    mask_en = INA230_REG_MASK_ENABLE_POL;
    break;
  default:
    mask_en = mask_en & ((uint16_t)~(REG_MASK_ENABLE_FUNCTIONS_Msk));
    break;
  }
  
  ina230_WriteReg(Address, INA230_REG_MASK_ENABLE, mask_en);
  
}

/**
  * @brief  Get the programmed alert function
  * @param  Address INA230 address on communication Bus.
  * @retval Function Monitored alert function.
  *         This parameter can be take one of the following values:
  *         ALERT_FUNCTION_NONE
  *         ALERT_FUNCTION_SOL
  *         ALERT_FUNCTION_SUL
  *         ALERT_FUNCTION_B0L
  *         ALERT_FUNCTION_BUL
  *         ALERT_FUNCTION_POL
  * @retval None.
  */
PWRMON_AlertFunction_t ina230_GetAlertFunction(uint16_t Address)
{
  PWRMON_AlertFunction_t alert_function;
  uint16_t                    mask_en;
  
  /* Read programmed Alert configuration */
  mask_en = ina230_ReadReg(Address, INA230_REG_MASK_ENABLE);
  
  if ((mask_en & INA230_REG_MASK_ENABLE_POL) == INA230_REG_MASK_ENABLE_POL)
  {
    alert_function = ALERT_FUNCTION_POL;
  }
  else if ((mask_en & INA230_REG_MASK_ENABLE_BUL) == INA230_REG_MASK_ENABLE_BUL)
  {
    alert_function = ALERT_FUNCTION_BUL;
  }
  else if ((mask_en & INA230_REG_MASK_ENABLE_BOL) == INA230_REG_MASK_ENABLE_BOL)
  {
    alert_function = ALERT_FUNCTION_BOL;
  }
  else if ((mask_en & INA230_REG_MASK_ENABLE_SUL) == INA230_REG_MASK_ENABLE_SUL)
  {
    alert_function = ALERT_FUNCTION_SUL;
  }
  else if ((mask_en & INA230_REG_MASK_ENABLE_SOL) == INA230_REG_MASK_ENABLE_SOL)
  {
    alert_function = ALERT_FUNCTION_SOL;
  }
  else
  {
    alert_function = ALERT_FUNCTION_NONE;
  }
  
  return alert_function;
}

/**
  * @brief  Configure the Alert pin behavior
  * @param  Address INA230 address on communication Bus.
  * @param  pAlertPinConfig Pointer to the ALERT pin configuration structure.
  * @retval None
  */
void ina230_AlertPinConfig(uint16_t Address, PWRMON_AlertPinConfig_t * pAlertPinConfig)
{
  uint16_t mask_en;
  
  /* Read programmed Alert configuration */
  mask_en = ina230_ReadReg(Address, INA230_REG_MASK_ENABLE);
  
  /* Update Alert configuration */
  mask_en = (mask_en & ((uint16_t)~(INA230_REG_MASK_ENABLE_APOL_Msk))) | (pAlertPinConfig->Polarity << INA230_REG_MASK_ENABLE_APOL_Pos);
  mask_en = (mask_en & ((uint16_t)~(INA230_REG_MASK_ENABLE_LEN_Msk))) | (pAlertPinConfig->LatchEnable << INA230_REG_MASK_ENABLE_LEN_Pos);
  
  ina230_WriteReg(Address, INA230_REG_MASK_ENABLE, mask_en); 
}

/**
  * @brief  Program the VBus threshold
  * @note Do not apply more than 28 V on the VBUS pin
  * @param  Address INA230 address on communication Bus.
  * @param  VBusThreshold VBus threshold value (in mV).
  * @retval None
  */
void ina230_SetVBusThreshold(uint16_t Address, uint16_t VBusThreshold)
{
  uint32_t val;
  val = (VBusThreshold * 1000);
  val = DIV_ROUND_CLOSEST(val, BUS_VOLTAGE_LSB);
  ina230_WriteReg(Address, INA230_REG_ALERT_LIMIT, (uint16_t)val);
}

/**
  * @brief  Get the VBUS threshold
  * @param  Address INA230 address on communication Bus.
  * @retval  VBusThreshold VBus threshold value (in mV).
  */
uint16_t ina230_GetVBusThreshold(uint16_t Address)
{
  uint16_t val;
  uint32_t vbus_threshold;
  val = ina230_ReadReg(Address, INA230_REG_ALERT_LIMIT);
  vbus_threshold = (val * BUS_VOLTAGE_LSB);
  vbus_threshold = DIV_ROUND_CLOSEST(vbus_threshold, 1000);
  return (uint16_t)vbus_threshold;
}

/**
  * @brief  Program the VShunt threshold
  * @param  Address INA230 address on communication Bus.
  * @param  VshuntThreshold VShunt threshold value (in mV).
  * @retval None
  */
void ina230_SetVShuntThreshold(uint16_t Address, int16_t VShuntThreshold)
{
  uint16_t val;
  val = VShuntThreshold * VSHUNT_DIV;
  ina230_WriteReg(Address, INA230_REG_ALERT_LIMIT, val);
}

/**
  * @brief  Get the VShunt threshold
  * @param  Address INA230 address on communication Bus.
  * @retval  VshuntThreshold VShunt threshold value (in mV).
  */
int16_t ina230_GetVShuntThreshold(uint16_t Address)
{
  int16_t val;
  int16_t vshunt_threshold;
  val = ina230_ReadReg(Address, INA230_REG_ALERT_LIMIT);
  vshunt_threshold = DIV_ROUND_CLOSEST(val, VSHUNT_DIV);
  return vshunt_threshold;
}

/**
  * @brief  Program the Power threshold
  * @param  Address INA230 address on communication Bus.
  * @param  PowerThreshold Power threshold value (in mW).
  * @retval None
  */
void ina230_SetPowerThreshold(uint16_t Address, uint32_t PowerThreshold)
{
  uint16_t val;
  val = (uint16_t)DIV_ROUND_CLOSEST(PowerThreshold, POWER_LSB);;
  ina230_WriteReg(Address, INA230_REG_ALERT_LIMIT, val);
}

/**
  * @brief  Get the Power threshold
  * @param  Address INA230 address on communication Bus.
  * @retval  PowerThreshold Power threshold value (in mW).
  */
uint32_t ina230_GetPowerThreshold(uint16_t Address)
{
  uint16_t val;
  uint32_t power_threshold;
  val = ina230_ReadReg(Address, INA230_REG_ALERT_LIMIT);
  power_threshold = (uint32_t)(val * POWER_LSB);
  return power_threshold;
}

/**
  * @brief  Enable the Voltage/Power threshold interrupt
  * @note Alert pin will be asserted when the alert function selected through 
  *       ina230_SetAlertFunction() exceeds the threshold programmed through
  *       ina230_SetVBusThreshold().
  * @param  Address INA230 address on communication Bus.
  * @retval None
  */
void ina230_AlertThresholdEnableIT(uint16_t Address)
{
  ina230_EnableIT(Address);
}

/**
  * @brief  Disable the Voltage/Power threshold interrupt
  * @note Alert pin will not be asserted when the alert function selected through 
  *       ina230_SetAlertFunction() exceeds the threshold programmed through
  *       ina230_SetVBusThreshold().
  * @param  Address INA230 address on communication Bus.
  * @retval None
  */
void ina230_AlertThresholdDisableIT(uint16_t Address)
{
  PWRMON_IO_DisableIT(Address);
}

/**
  * @brief  Enable the Conversion ready interrupt
  * @note Alert pin will be asserted when the Conversion Ready Flag is
  *       asserted, indicating that the device is ready for the next conversion.
  * @param  Address INA230 address on communication Bus.
  * @retval None
  */
void ina230_ConversionReadyEnableIT(uint16_t Address)
{
  uint16_t mask_en;
  
  /* Read mask enable register */
  mask_en = ina230_ReadReg(Address, INA230_REG_MASK_ENABLE);
  
  ina230_EnableIT(Address);

  /* Set the conversion ready bit */
  mask_en |= INA230_REG_MASK_ENABLE_CNVR;
  ina230_WriteReg(Address, INA230_REG_MASK_ENABLE, mask_en);
}
  
/**
  * @brief  Disable the Conversion ready interrupt
  * @note Alert pin will not be asserted when the Conversion Ready Flag is
  *       asserted.
  * @param  Address INA230 address on communication Bus.
  * @retval None
  */
void ina230_ConversionReadyDisableIT(uint16_t Address)
{
  uint16_t mask_en;
  
  /* Read mask enable register */
  mask_en = ina230_ReadReg(Address, INA230_REG_MASK_ENABLE);

  PWRMON_IO_DisableIT(Address);
  
  /* Reset the conversion ready bit */
  mask_en &= ~INA230_REG_MASK_ENABLE_CNVR;
  ina230_WriteReg(Address, INA230_REG_MASK_ENABLE, mask_en);
}

/**
  * @brief  Launch the conversion on the selected voltage input
  * @param  Address INA230 address on communication Bus.
  * @param  InputSignal Input signal to convert.
  *         This parameter can be take one of the following values:
  *         INPUT_SIGNAL_SHUNT
  *         INPUT_SIGNAL_BUS
  *         INPUT_SIGNAL_ALL
  * @param  Mode operating mode.
  *         This parameter can be take one of the following values:
  *         OPERATING_MODE_CONTINUOUS
  *         OPERATING_MODE_TRIGGERED
  * @retval None
  */
void ina230_StartConversion(uint16_t                Address, 
                            PWRMON_InputSignal_t    InputSignal, 
                            PWRMON_OperatingMode_t  Mode)
{
  uint16_t cfg = 0;
  
  /*  Read programmed configuration  */
  cfg = ina230_ReadReg(Address, INA230_REG_CONFIG);

  /* Update operating mode */
  cfg = (cfg & ((uint16_t)~(INA230_REG_CFG_MODE_Msk))) | aMode[Mode][InputSignal];

  ina230_WriteReg(Address, INA230_REG_CONFIG, cfg);  
}

/**
  * @brief  Stop the conversion on the selected voltage input
  * @note INA230 is put in power down mode.
  * @note INA230 remains in power down mode until ina230_StartConversion() is
  *       called again.
  * @param  Address INA230 address on communication Bus.
  * @retval None
  */
void ina230_StopConversion(uint16_t Address)
{
  uint16_t cfg = 0;
  
  /*  Read programmed configuration  */
  cfg = ina230_ReadReg(Address, INA230_REG_CONFIG);

  /* Update operating mode (Power-Down) */
  cfg = (cfg & ((uint16_t)~(INA230_REG_CFG_MODE_Msk)));

  ina230_WriteReg(Address, INA230_REG_CONFIG, cfg);  
}

/**
  * @brief  Read the bus voltage
  * @param  Address INA230 address on communication Bus.
  * @retval BusVoltage Bus voltage value (in mV)
  */
uint16_t ina230_GetVBus(uint16_t Address)
{
  uint16_t val;
  uint32_t vbus;
  val = ina230_ReadReg(Address, INA230_REG_VBUS);
  vbus = (val * BUS_VOLTAGE_LSB);
  vbus = DIV_ROUND_CLOSEST(vbus, 1000);
  return (uint16_t)vbus;
}

/**
  * @brief  Read the shunt voltage
  * @param  Address INA230 address on communication Bus.
  * @retval VShunt Shunt voltage value (in mV)
  */
int16_t ina230_GetVShunt(uint16_t Address)
{
  int16_t val;
  int16_t vshunt;
  val = ina230_ReadReg(Address, INA230_REG_VSHUNT);
  vshunt = DIV_ROUND_CLOSEST(val, VSHUNT_DIV);
  return vshunt;
}

/**
  * @brief  Read the power value
  * @param  Address INA230 address on communication Bus.
  * @retval Power Power value (in mW)
  */
uint16_t ina230_GetPower(uint16_t Address)
{
  uint16_t val;
  uint16_t power;
  val = ina230_ReadReg(Address, INA230_REG_PWR);
  power = val * POWER_LSB;
  return power;
}

/**
  * @brief  Read the current value
  * @param  Address INA230 address on communication Bus.
  * @retval Current Current value (in mA)
  */
int16_t ina230_GetCurrent(uint16_t Address)
{
  return (int16_t)ina230_ReadReg(Address, INA230_REG_CURRENT);
}

/**
  * @brief  Read the designed flag value
  * @param  Address INA230 address on communication Bus.
  * @param  Flag specifies the flag to check.
  *         This parameter can be one of the following values:
  *         FLAG_ALERT_FUNCTION
  *         FLAG_CONVERSION_READY
  *         FLAG_MATH_OVERFLOW
  * @retval Actual state of the flag (TRUE or FALSE).
  */
uint8_t ina230_GetFlag(uint16_t Address, PWRMON_Flag_t Flag)
{
  uint16_t flags;
  
  flags = ina230_ReadReg(Address, INA230_REG_MASK_ENABLE) & REG_MASK_ENABLE_FLAGS_Msk;
  
  return ((aFlags[Flag] & flags) == aFlags[Flag]);
}
/**
  * @}
  */

/** @defgroup INA230_Private_Functions
  * @{
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

/**
  * @}
  */ 

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/     
