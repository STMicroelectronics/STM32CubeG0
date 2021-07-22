/**
  ******************************************************************************
  * @file    stm32g071b_discovery_pwrmon.c
  * @author  MCD Application Team
  * @brief   This file provides a set of firmware functions to manage the 
  *          Power Monitor driver for STM32G071B-DISCO board.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2019-2021 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "stm32g071b_discovery.h"
#include "stm32g071b_discovery_pwrmon.h"
#include "../Components/ina230/ina230.h"

/** @addtogroup BSP
  * @{
  */

/** @addtogroup STM32G071B-DISCO
  * @{
  */

/** @defgroup STM32G071B_DISCOVERY_PWRMON STM32G071B-DISCO PWRMON
  * @brief This file includes the pwrmon driver for STM32G071B-DISCO board.
  *        It allows user to perform voltage, current and power measurements.
  * @{
  */

/** @defgroup STM32G071B_DISCOVERY_PWRMON_Private_Typedef Private Typedef
  * @{
  */
typedef enum {
  PWRMON_NOT_INITIALIZED = 0,
  PWRMON_INITIALIZED
} PWRMON_StateTypedef;

typedef struct {
  PWRMON_StateTypedef         State;
  uint32_t                    Mask;
  PWRMON_AlertPolarity_t Polarity;
} AlertInfoTypeDef;

typedef struct {
  PWRMON_VoltageOutOfRangeCallbackFunc * pfnVoltageOutOfRangeCallback;
  PWRMON_ConversionReadyCallbackFunc *   pfnConversionReadyCallback;
  AlertInfoTypeDef                       AlertInfo[ALERTn];
} ContextTypeDef;

/**
  * @}
  */

/** @defgroup STM32G071B_DISCOVERY_PWRMON_Private_Macros Private Macros
  * @{
  */
/* Divide positive dividend by divisor and round to closest integer. */
#define DIV_ROUND_CLOSEST(__DIVIDEND__, __DIVISOR__) (((__DIVIDEND__) + ((__DIVISOR__)/2)) / (__DIVISOR__))

/**
  * @}
  */


/** @defgroup STM32G071B_DISCOVERY_PWRMON_Private_Variables Private Variables
  * @{
  */
static ContextTypeDef Context =
{
  .pfnVoltageOutOfRangeCallback = (PWRMON_VoltageOutOfRangeCallbackFunc *)NULL,
  .pfnConversionReadyCallback   = (PWRMON_ConversionReadyCallbackFunc *)NULL,
  .AlertInfo =
  {
    /* ALERT_VBUS */
    {
      .State    = PWRMON_NOT_INITIALIZED,
      .Mask     = 0,
      .Polarity = ALERT_POLARITY_NORMAL
    },
    /* ALERT_CC1 */
    {
      .State    = PWRMON_NOT_INITIALIZED,
      .Mask     = 0,
      .Polarity = ALERT_POLARITY_NORMAL
    },
    /* ALERT_CC2 */
    {
      .State    = PWRMON_NOT_INITIALIZED,
      .Mask     = 0,
      .Polarity = ALERT_POLARITY_NORMAL
    },
  }
};

static const uint32_t aAddress[ALERTn] =
{
  VBUS_SENSING_I2C_ADDRESS,
  CC1_SENSING_I2C_ADDRESS,
  CC2_SENSING_I2C_ADDRESS
};

/**
  * @}
  */

/** @defgroup STM32G071B_DISCOVERY_PWRMON_Private_Functions Private Functions
  * @{
  */
static uint16_t CalculateCalibrationValue(uint32_t RShunt);
/**
  * @}
  */

/** @defgroup STM32G071B_DISCOVERY_PWRMON_Exported_Functions Exported Functions
  * @{
  */

/**
 * @brief  Initialize the hardware resources used by the power monitor
 *         device assigned to a given Alert pin.
 * @note When the triggered operating mode is selected, ConversionReadyCallback
 *       callback will be called whenever the device has completed a conversion.
 * @param  Alert Alert pin identifier
 *         This parameter can be take one of the following values:
 *         @arg   ALERT_VBUS
 *         @arg   ALERT_CC1
 *         @arg   ALERT_CC2
 * @param  pConfig Pointer to the power monitor device configuration structure
 * @retval pwrmon status
 */
PWRMON_StatusTypeDef BSP_PWRMON_Init(Alert_TypeDef       Alert,
                                     PWRMON_Config_t   * pConfig)
{
  uint32_t ret = 0;
  uint16_t cal;

  if (Context.AlertInfo[Alert].State == PWRMON_NOT_INITIALIZED)
  {
    /* Initialize INA230 */
    ina230_Init(aAddress[Alert], pConfig);

    /* Set calibration value (required to measure current) */
    cal = CalculateCalibrationValue(SHUNT_RESISTOR_VALUE);
    ina230_SetCalibration(aAddress[Alert], cal);

    /* Update Context */
    Context.AlertInfo[Alert].State = PWRMON_INITIALIZED;
  }

  return (ret == 0) ? PWRMON_OK : PWRMON_ERROR;
}

/**
 * @brief  Release the hardware resources used by the power monitor
 *         device assigned to a given Alert pin.
 * @param  Alert Alert pin identifier
 *         This parameter can be take one of the following values:
 *         @arg   ALERT_VBUS
 *         @arg   ALERT_CC1
 *         @arg   ALERT_CC2
 * @retval pwrmon status
 */
PWRMON_StatusTypeDef BSP_PWRMON_DeInit(Alert_TypeDef  Alert)
{
  uint32_t ret = 0;

  if (Context.AlertInfo[Alert].State == PWRMON_INITIALIZED)
  {
    /* De-initialize INA230 */
    ina230_DeInit(aAddress[Alert]);

    /* Update Context */
    Context.AlertInfo[Alert].State = PWRMON_NOT_INITIALIZED;
  }

  return (ret == 0) ? PWRMON_OK : PWRMON_ERROR;
}


/**
 * @brief  Start measures on a given Alert pin.
 * @param  Alert Alert pin identifier
 *         This parameter can be take one of the following values:
 *         @arg   ALERT_VBUS
 *         @arg   ALERT_CC1
 *         @arg   ALERT_CC2
 * @param  Mode Operating mode
 *         This parameter can be take one of the following values:
 *         @arg   OPERATING_MODE_CONTINUOUS
 *         @arg   OPERATING_MODE_TRIGGERED
 * @retval pwrmon status
 */
PWRMON_StatusTypeDef BSP_PWRMON_StartMeasure(Alert_TypeDef          Alert,
                                             PWRMON_OperatingMode_t Mode)
{
  uint32_t ret = 0;

  ina230_StartConversion(aAddress[Alert], VOLTAGE_INPUT_ALL, Mode);

  return (ret == 0) ? PWRMON_OK : PWRMON_ERROR;
}

/**
 * @brief  Stop the measures on a given Alert pin.
 * @param  Alert Alert pin identifier
 *         This parameter can be take one of the following values:
 *         @arg   ALERT_VBUS
 *         @arg   ALERT_CC1
 *         @arg   ALERT_CC2
 * @retval pwrmon status
 */
PWRMON_StatusTypeDef BSP_PWRMON_StopMeasure(Alert_TypeDef Alert)
{
  uint32_t ret = 0;

  ina230_StopConversion(aAddress[Alert]);

  return (ret == 0) ? PWRMON_OK : PWRMON_ERROR;
}

/**
 * @brief  Retrieve the conversion ready state of the power monitor device
 *         assigned to a given Alert pin.
 * @param  Alert Alert pin identifier
 *         This parameter can be take one of the following values:
 *         @arg   ALERT_VBUS
 *         @arg   ALERT_CC1
 *         @arg   ALERT_CC2
 * @retval conversion ready state
 */
uint8_t BSP_PWRMON_IsConversionReady(Alert_TypeDef Alert)
{
  return ina230_GetFlag(aAddress[Alert], FLAG_CONVERSION_READY);
}

/**
 * @brief  Retrieve the Alert Function Flag of the power monitor device
 *         assigned to a given Alert pin.
 * @param  Alert Alert pin identifier
 *         This parameter can be take one of the following values:
 *         @arg   ALERT_VBUS
 *         @arg   ALERT_CC1
 *         @arg   ALERT_CC2
* @retval conversion ready state
 */
uint8_t BSP_PWRMON_IsVoltageOutOfRange(Alert_TypeDef Alert)
{
  return ina230_GetFlag(aAddress[Alert], FLAG_ALERT_FUNCTION);
}

/**
 * @brief  Get actual voltage level on a given Alert pin.
 * @param  Alert Alert pin identifier
 *         This parameter can be take one of the following values:
 *         @arg   ALERT_VBUS
 *         @arg   ALERT_CC1
 *         @arg   ALERT_CC2
 * @param pVoltage Measured voltage level (in mV)
 * @retval pwrmon status
 */
PWRMON_StatusTypeDef BSP_PWRMON_GetVoltage(Alert_TypeDef Alert,
                                           uint32_t *    pVoltage)
{
  uint32_t ret = 0;

  *pVoltage = (uint32_t)ina230_GetVBus(aAddress[Alert]);

  return (ret == 0) ? PWRMON_OK : PWRMON_ERROR;
}

/**
 * @brief  Get actual current level on a given Alert pin.
 * @param  Alert Alert pin identifier
 *         This parameter can be take one of the following values:
 *         @arg   ALERT_VBUS
 *         @arg   ALERT_CC1
 *         @arg   ALERT_CC2
 * @param pCurrent Measured current level (in mA)
 * @retval pwrmon status
 */
PWRMON_StatusTypeDef BSP_PWRMON_GetCurrent(Alert_TypeDef Alert,
                                           int32_t *     pCurrent)
{
  uint32_t ret = 0;

  *pCurrent = (int32_t)ina230_GetCurrent(aAddress[Alert]);

  return (ret == 0) ? PWRMON_OK : PWRMON_ERROR;
}

/**
 * @brief  Get calculated power value on a given Alert pin.
 * @param  Alert Alert pin identifier
 *         This parameter can be take one of the following values:
 *         @arg   ALERT_VBUS
 *         @arg   ALERT_CC1
 *         @arg   ALERT_CC2
 * @param pPower Calculated power value (in mW)
 * @retval pwrmon status
 */
PWRMON_StatusTypeDef BSP_PWRMON_GetPower(Alert_TypeDef Alert,
                                         uint32_t *    pPower)
{
  uint32_t ret = 0;

  *pPower = (uint32_t)ina230_GetPower(aAddress[Alert]);

  return (ret == 0) ? PWRMON_OK : PWRMON_ERROR;
}

/**
 * @brief  Set the upper or lower voltage threshold on a given Alert pin.
 * @param  Alert Alert pin identifier
 *         This parameter can be take one of the following values:
 *         @arg   ALERT_VBUS
 *         @arg   ALERT_CC1
 *         @arg   ALERT_CC2
 * @param Threshold Voltage threshold (in mV)
 * @param ThresholdType Type of voltage threshold
 *         This parameter can be take one of the following values:
 *         @arg   VOLTAGE_OVER_THRESHOLD
 *         @arg   VOLTAGE_UNDER_THRESHOLD
 * @retval pwrmon status
 */
PWRMON_StatusTypeDef BSP_PWRMON_SetVoltageThreshold(Alert_TypeDef        Alert,
                                                    uint32_t             Threshold,
                                                    PWRMON_Threshold_t   ThresholdType)
{
  uint32_t ret = 0;
  PWRMON_AlertFunction_t alert_function;

  /* Set the alert function */
  if (ThresholdType == VOLTAGE_OVER_THRESHOLD)
  {
    /* Bus Voltage Over Limit */
    alert_function = ALERT_FUNCTION_BOL;
  }
  else
  {
    /* Bus Voltage Under Limit */
    alert_function = ALERT_FUNCTION_BUL;
  }
  ina230_SetAlertFunction(aAddress[Alert], alert_function);

  /* Set the VBus threshold */
  ina230_SetVBusThreshold(aAddress[Alert], (uint16_t)Threshold);

  return (ret == 0) ? PWRMON_OK : PWRMON_ERROR;
}

/**
 * @brief  Get actual voltage threshold on a given Alert pin.
 * @param  Alert Alert pin identifier
 *         This parameter can be take one of the following values:
 *         @arg   ALERT_VBUS
 *         @arg   ALERT_CC1
 *         @arg   ALERT_CC2
 * @param pThreshold Threshold value (in mV)
 * @param pThresholdType Type of voltage threshold
 *         This parameter can be take one of the following values:
 *         @arg   VOLTAGE_OVER_THRESHOLD
 *         @arg   VOLTAGE_UNDER_THRESHOLD
 * @retval pwrmon status
 */
PWRMON_StatusTypeDef BSP_PWRMON_GetVoltageThreshold(Alert_TypeDef        Alert,
                                                    uint32_t *           pThreshold,
                                                    PWRMON_Threshold_t * pThresholdType)
{
  uint32_t ret = 0;
  PWRMON_AlertFunction_t alert_function;

  /* Get the VBus threshold */
  *pThreshold = (uint32_t)ina230_GetVBusThreshold(aAddress[Alert]);

   /* Get the threshold type */
  alert_function = ina230_GetAlertFunction(aAddress[Alert]);

  if (alert_function == ALERT_FUNCTION_BOL)
  {
    *pThresholdType = VOLTAGE_OVER_THRESHOLD;
  }
  else
  {
    *pThresholdType = VOLTAGE_UNDER_THRESHOLD;
  }

  return (ret == 0) ? PWRMON_OK : PWRMON_ERROR;
}

/**
 * @brief Configure the ALERT pin behavior
 * @param  Alert Alert pin identifier
 *         This parameter can be take one of the following values:
 *         @arg   ALERT_VBUS
 *         @arg   ALERT_CC1
 *         @arg   ALERT_CC2
 * @param Mask Interrupt mask
 *         Channels This parameter can be a combination of the following values:
 *         @arg ALERT_CONVERSION_READY
 *         @arg ALERT_VOLTAGE_OUT_OF_RANGE
 * @param Polarity Alert pin polarity
 *         This parameter can be take one of the following values:
 *         @arg   ALERT_POLARITY_LOW
 *         @arg   ALERT_POLARITY_HIGH
 * @retval None
 */
PWRMON_StatusTypeDef BSP_PWRMON_ConfigIT(Alert_TypeDef               Alert,
                                         uint32_t                    Mask,
                                         PWRMON_AlertPolarity_t Polarity)
{
  uint32_t ret = 0;
  PWRMON_AlertPinConfig_t alert_pin_cfg;

  /* Configure the Alert pin */
  alert_pin_cfg.Polarity    = Polarity;
  alert_pin_cfg.LatchEnable = ALERT_LATCH_DISABLE;
  ina230_AlertPinConfig(aAddress[Alert], &alert_pin_cfg);

  /* Update Context */
  Context.AlertInfo[Alert].Mask     = Mask;
  Context.AlertInfo[Alert].Polarity = Polarity;

  return (ret == 0) ? PWRMON_OK : PWRMON_ERROR;
}

/**
 * @brief Enable the ALERT pin interrupt
 * @param  Alert Alert pin identifier
 *         This parameter can be take one of the following values:
 *         @arg   ALERT_VBUS
 *         @arg   ALERT_CC1
 *         @arg   ALERT_CC2
 * @retval None
 */
PWRMON_StatusTypeDef BSP_PWRMON_EnableIT(Alert_TypeDef Alert)
{
  uint32_t ret = 0;

  /* Enable the conversion ready interrupt (if required) */
  if ((Context.AlertInfo[Alert].Mask & ALERT_CONVERSION_READY) == ALERT_CONVERSION_READY)
  {
    ina230_ConversionReadyEnableIT(aAddress[Alert]);
  }

  /* Enable the Alert threshold interrupt (if required) */
  if ((Context.AlertInfo[Alert].Mask & ALERT_VOLTAGE_OUT_OF_RANGE) == ALERT_VOLTAGE_OUT_OF_RANGE)
  {
    ina230_AlertThresholdEnableIT(aAddress[Alert]);
  }

  return (ret == 0) ? PWRMON_OK : PWRMON_ERROR;
}

/**
 * @brief Disable the ALERT pin interrupt
 * @param  Alert Alert pin identifier
 *         This parameter can be take one of the following values:
 *         @arg   ALERT_VBUS
 *         @arg   ALERT_CC1
 *         @arg   ALERT_CC2
 * @retval None
 */
PWRMON_StatusTypeDef BSP_PWRMON_DisableIT(Alert_TypeDef Alert)
{
  uint32_t ret = 0;

  /* Disable the conversion ready interrupt (if required) */
  if ((Context.AlertInfo[Alert].Mask & ALERT_CONVERSION_READY) == ALERT_CONVERSION_READY)
  {
    ina230_ConversionReadyDisableIT(aAddress[Alert]);
  }

  /* Disable the Alert threshold interrupt (if required) */
  if ((Context.AlertInfo[Alert].Mask & ALERT_VOLTAGE_OUT_OF_RANGE) == ALERT_VOLTAGE_OUT_OF_RANGE)
  {
    ina230_AlertThresholdDisableIT(aAddress[Alert]);
  }

  return (ret == 0) ? PWRMON_OK : PWRMON_ERROR;
}

/**
 * @brief ALERT pin state change notification
 * @note This function must be called whenever the ALERT pin in asserted
 * @param  Alert Alert pin identifier
 *         This parameter can be take one of the following values:
 *         @arg   ALERT_VBUS
 *         @arg   ALERT_CC1
 *         @arg   ALERT_CC2
 * @retval None
 */
void BSP_PWRMON_AlertNotify(Alert_TypeDef Alert)
{
  /* Call conversion ready callback function (if required)*/
  if (((Context.AlertInfo[Alert].Mask & ALERT_CONVERSION_READY) == ALERT_CONVERSION_READY) &&
      (BSP_PWRMON_IsConversionReady(Alert) == 1)                                           &&
      (Context.pfnConversionReadyCallback != (PWRMON_ConversionReadyCallbackFunc *)NULL))
  {
    Context.pfnConversionReadyCallback(Alert);
  }

  /* Call Voltage out of range callback function (if required) */
  if (((Context.AlertInfo[Alert].Mask & ALERT_VOLTAGE_OUT_OF_RANGE) == ALERT_VOLTAGE_OUT_OF_RANGE) &&
      (BSP_PWRMON_IsVoltageOutOfRange(Alert) == 1)                                                 &&
      (Context.pfnVoltageOutOfRangeCallback != (PWRMON_VoltageOutOfRangeCallbackFunc *)NULL))
  {
    Context.pfnVoltageOutOfRangeCallback(Alert);
  }
}

/**
 * @brief  Register the callback function called when the voltage level
 *         exceeds the voltage limit.
 * @param  Alert Alert pin identifier
 *         This parameter can be take one of the following values:
 *         @arg   ALERT_VBUS
 *         @arg   ALERT_CC1
 *         @arg   ALERT_CC2
 * @param pfnVoltageOutOfRangeCallback Callback function pointer
 * @retval pwrmon status
 */
PWRMON_StatusTypeDef BSP_PWRMON_RegisterVoltageOutOfRangeCallback(Alert_TypeDef                            Alert,
                                                                  PWRMON_VoltageOutOfRangeCallbackFunc *   pfnVoltageOutOfRangeCallback)
{
  uint32_t ret = 0;

  /* Update Context */
  Context.pfnVoltageOutOfRangeCallback = pfnVoltageOutOfRangeCallback;

  return (ret == 0) ? PWRMON_OK : PWRMON_ERROR;
}

/**
 * @brief  Register the callback function called when the conversion is
 *         completed.
 * @param  Alert Alert pin identifier
 *         This parameter can be take one of the following values:
 *         @arg   ALERT_VBUS
 *         @arg   ALERT_CC1
 *         @arg   ALERT_CC2
 * @param pfnConversionReadyCallback Callback function pointer
 * @retval pwrmon status
 */
PWRMON_StatusTypeDef BSP_PWRMON_RegisterConversionReadyCallback(Alert_TypeDef                        Alert,
                                                                PWRMON_ConversionReadyCallbackFunc * pfnConversionReadyCallback)
{
  uint32_t ret = 0;

  /* Update Context */
  Context.pfnConversionReadyCallback = pfnConversionReadyCallback;

  return (ret == 0) ? PWRMON_OK : PWRMON_ERROR;
}

#if defined(INA230_DEBUG)
/**
 * @brief  Dump the register content of a device.
 * @param  Alert Alert pin identifier
 *         This parameter can be take one of the following values:
 *         @arg   ALERT_VBUS
 *         @arg   ALERT_CC1
 *         @arg   ALERT_CC2
 * @param pRegisters pointer to the image of the ina230 register set.
 * @retval none
 */
void BSP_PWRMON_DumpDeviceRegisters(Alert_TypeDef Alert, INA230_Registers_t * pRegisters)
{
  uint8_t dt[2];

  PWRMON_IO_Read(aAddress[Alert], INA230_REG_CONFIG,      dt); pRegisters->Config.Register = (dt[0] << 8) | dt[1];
  PWRMON_IO_Read(aAddress[Alert], INA230_REG_VSHUNT,      dt); pRegisters->VShunt = (dt[0] << 8) | dt[1];
  PWRMON_IO_Read(aAddress[Alert], INA230_REG_VBUS,        dt); pRegisters->VBus = (dt[0] << 8) | dt[1];
  PWRMON_IO_Read(aAddress[Alert], INA230_REG_PWR,         dt); pRegisters->Power = (dt[0] << 8) | dt[1];
  PWRMON_IO_Read(aAddress[Alert], INA230_REG_CURRENT,     dt); pRegisters->Current = (dt[0] << 8) | dt[1];
  PWRMON_IO_Read(aAddress[Alert], INA230_REG_CALIBRATION, dt); pRegisters->Calibration = (dt[0] << 8) | dt[1];
  PWRMON_IO_Read(aAddress[Alert], INA230_REG_MASK_ENABLE, dt); pRegisters->MaskEnable.Register = (dt[0] << 8) | dt[1];
  PWRMON_IO_Read(aAddress[Alert], INA230_REG_ALERT_LIMIT ,dt); pRegisters->AlertLimit = (dt[0] << 8) | dt[1];
  PWRMON_IO_Read(aAddress[Alert], INA230_REG_ID,          dt); pRegisters->DieId = (dt[0] << 8) | dt[1];
}
#endif /* INA230_DEBUG */

/**
  * @}
  */

/** @defgroup STM32G071B_DISCOVERY_PWRMON_Private_Functions Private Functions
  * @{
  */

/**
  * @brief  Calculate the calibration value to get a 1 mA resolution for
  *         current measures.
  *         CAL = 0.00512 / (RShunt * CurrentLsb)
  *         where CurrentLsb (1 mA) must be in the following range:
  *         (MaxExpectedCurrent/2^15) <= CurrentLsb <= (MaxExpectedCurrent/2^15)
  */
static uint16_t CalculateCalibrationValue(uint32_t RShunt)
{
  return (DIV_ROUND_CLOSEST(5120000, (RShunt*1000)));
}
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

