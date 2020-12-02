/**
  ******************************************************************************
  * @file    sn65dp141.c
  * @author  MCD Application Team
  * @brief   This file provides a set of functions needed to manage the SN65DP141
  *          DisplayPort Linear Redriver.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2017 STMicroelectronics.
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
#include "sn65dp141.h"

/** @addtogroup BSP
  * @{
  */

/** @addtogroup Components
  * @{
  */

/** @defgroup SN65DP141
  * @brief     This file provides a set of functions needed to drive the
  *            SN65DP141 DisplayPort Linear Redriver.
  * @{
  */

/** @defgroup SN65DP141_Private_TypesDefinitions
  * @{
  */

/**
  * @}
  */

/** @defgroup SN65DP141_Private_Defines
  * @{
  */

/**
  * @}
  */

/** @defgroup SN65DP141_Private_Macros
  * @{
  */

/**
  * @}
  */

/** @defgroup SN65DP141_Private_Variables
  * @{
  */

/* DisplayPort Linear Redriver Driver structure initialization */
DPREDRIVER_Drv_t sn65dp141_drv =
{
  sn65dp141_Init,
  sn65dp141_DeInit,
  sn65dp141_PowerOn,
  sn65dp141_PowerOff,
  sn65dp141_SetEQGain,
  sn65dp141_EnableChannel,
  sn65dp141_DisableChannel
};

/**
  * @}
  */

/** @defgroup SN65DP141_Private_FunctionPrototypes
  * @{
  */

/**
  * @}
  */

/** @defgroup SN65DP141_Private_Functions
  * @{
  */

/**
  * @brief  Initialize the SN65DP141 and configure the needed hardware resources.
  * @param  Address Device address on communication Bus.
  * @retval None
  */
uint32_t sn65dp141_Init(uint16_t Address)
{
  uint32_t err_count = 0;

  /*  Low level init */
  err_count += MUX_IO_Init();

  /* Restore SN65DP141 registers reset values */
  err_count += MUX_IO_Write(Address, SN65DP141_REG_CFG,     0x00);
  err_count += MUX_IO_Write(Address, SN65DP141_REG_CHEN,    0x00);
  err_count += MUX_IO_Write(Address, SN65DP141_REG_CH0_CFG, 0x00);
  err_count += MUX_IO_Write(Address, SN65DP141_REG_CH0_EN,  0x00);
  err_count += MUX_IO_Write(Address, SN65DP141_REG_CH1_CFG, 0x00);
  err_count += MUX_IO_Write(Address, SN65DP141_REG_CH1_EN,  0x00);
  err_count += MUX_IO_Write(Address, SN65DP141_REG_CH2_CFG, 0x00);
  err_count += MUX_IO_Write(Address, SN65DP141_REG_CH2_EN,  0x00);
  err_count += MUX_IO_Write(Address, SN65DP141_REG_CH3_CFG, 0x00);
  err_count += MUX_IO_Write(Address, SN65DP141_REG_CH3_EN,  0x00);

  return err_count;
}

/**
  * @brief  Release the hardware resources required to use the SN65DP141
  * @param  Address SN65DP141 address on communication Bus.
  * @retval none
  */
void sn65dp141_DeInit(uint16_t Address)
{
  /* Restore SN65DP141 registers reset values */
  MUX_IO_Write(Address, SN65DP141_REG_CFG,     0x00);
  MUX_IO_Write(Address, SN65DP141_REG_CHEN,    0x00);
  MUX_IO_Write(Address, SN65DP141_REG_CH0_CFG, 0x00);
  MUX_IO_Write(Address, SN65DP141_REG_CH0_EN,  0x00);
  MUX_IO_Write(Address, SN65DP141_REG_CH1_CFG, 0x00);
  MUX_IO_Write(Address, SN65DP141_REG_CH1_EN,  0x00);
  MUX_IO_Write(Address, SN65DP141_REG_CH2_CFG, 0x00);
  MUX_IO_Write(Address, SN65DP141_REG_CH2_EN,  0x00);
  MUX_IO_Write(Address, SN65DP141_REG_CH3_CFG, 0x00);
  MUX_IO_Write(Address, SN65DP141_REG_CH3_EN,  0x00);

  /*  Low level de-init */
  MUX_IO_DeInit();
}

/**
  * @brief  Power on the SN65DP141.
  * @param  Address SN65DP141 address on communication Bus.
  * @retval 0: successful, else failed
  */
uint32_t sn65dp141_PowerOn(uint16_t Address)
{
  uint32_t err_count = 0;
  uint8_t cfg;

  /* Read General Device Settings register*/
  err_count += MUX_IO_Read(Address, SN65DP141_REG_CFG, &cfg);

  /* Clear PWRDOWN bit of General Device Settings register */
  cfg &= ~SN65DP141_REG_CFG_PWRDOWN;
  err_count += MUX_IO_Write(Address, SN65DP141_REG_CFG, cfg);

  return err_count;
}

/**
  * @brief  Power down the SN65DP141.
  * @param  Address SN65DP141 address on communication Bus.
  * @retval 0: successful, else failed
  */
uint32_t sn65dp141_PowerOff(uint16_t Address)
{
  uint32_t err_count = 0;
  uint8_t cfg;

  /* Read General Device Settings register*/
  err_count += MUX_IO_Read(Address, SN65DP141_REG_CFG, &cfg);

  /* Set PWRDOWN bit of General Device Settings register */
  cfg |= SN65DP141_REG_CFG_PWRDOWN;
  err_count += MUX_IO_Write(Address, SN65DP141_REG_CFG, cfg);

  return err_count;
}

/**
  * @brief  Set the equalizer gain for a given channel.
  * @param  Address   SN65DP141 address on communication Bus.
  * @param  ChannelId Channel identifier.
  *         This parameter can be take one of the following values:
  *         CHANNEL_DP0
  *         CHANNEL_DP1
  *         CHANNEL_DP2
  *         CHANNEL_DP3
  * @param  EQGain    Equalizer gain.
  *         This parameter must be a value between 0x00 and 0x07.
  * @retval 0: successful, else failed
  */
uint32_t sn65dp141_SetEQGain(uint16_t                    Address,
                             DPREDRIVER_ChannelId_t      ChannelId,
                             uint8_t                     EQGain)
{
  uint32_t err_count = 0;
  uint8_t chctrl;
  uint8_t cfg;

  switch(ChannelId)
  {
  case CHANNEL_DP0:
    cfg = SN65DP141_REG_CH0_CFG;
    break;
  case CHANNEL_DP1:
    cfg = SN65DP141_REG_CH1_CFG;
    break;
  case CHANNEL_DP2:
    cfg = SN65DP141_REG_CH2_CFG;
    break;
  case CHANNEL_DP3:
    cfg = SN65DP141_REG_CH3_CFG;
    break;
  default:
    cfg = SN65DP141_REG_CH0_CFG;
    break;
  }

  /* Read Channel x Control Settings register */
  err_count += MUX_IO_Read(Address, cfg, &chctrl);

  /* Set the equalizer gain bit field (EQ setting) for concerned channel */
  chctrl = (chctrl & (~(uint8_t)SN65DP141_REG_CHxCFG_EQ_SETTING_Msk)) | (EQGain << SN65DP141_REG_CHxCFG_EQ_SETTING_Pos);

  /* Enable Max gain for TX & RX */
  chctrl |= SN65DP141_REG_CHxCFG_RX_GAIN_1 | SN65DP141_REG_CHxCFG_EQ_DC_GAIN | SN65DP141_REG_CHxCFG_TX_GAIN;

  /* Update Channel x Control Settings register */
  err_count += MUX_IO_Write(Address, cfg, chctrl);

  return err_count;
}

/**
  * @brief  Enable a DP channel.
  * @param  Address   SN65DP141 address on communication Bus.
  * @param  ChannelId Channel identifier.
  *         This parameter can be take one of the following values:
  *         CHANNEL_DP0
  *         CHANNEL_DP1
  *         CHANNEL_DP2
  *         CHANNEL_DP3
  * @retval 0: successful, else failed
  */
uint32_t sn65dp141_EnableChannel(uint16_t               Address,
                                 DPREDRIVER_ChannelId_t ChannelId)
{
  uint32_t err_count = 0;
  uint8_t chen;

  /* Read Channel Enable register */
  err_count += MUX_IO_Read(Address, SN65DP141_REG_CHEN, &chen);

  /* Clear LN_EN_CHx bit of Channel Enable register */
  switch(ChannelId)
  {
  case CHANNEL_DP0:
    chen &= ~SN65DP141_REG_CHEN_LN_EN_CH0;
    break;
  case CHANNEL_DP1:
    chen &= ~SN65DP141_REG_CHEN_LN_EN_CH1;
    break;
  case CHANNEL_DP2:
    chen &= ~SN65DP141_REG_CHEN_LN_EN_CH2;
    break;
  case CHANNEL_DP3:
    chen &= ~SN65DP141_REG_CHEN_LN_EN_CH3;
    break;
  default:
    /* Nothing to do */
    break;
  }

  err_count += MUX_IO_Write(Address, SN65DP141_REG_CHEN, chen);

  return err_count;
}

/**
  * @brief  Disable a DP channel.
  * @param  Address   SN65DP141 address on communication Bus.
  * @param  ChannelId Channel identifier.
  *         This parameter can be take one of the following values:
  *         CHANNEL_DP0
  *         CHANNEL_DP1
  *         CHANNEL_DP2
  *         CHANNEL_DP3
  * @retval 0: successful, else failed
  */
uint32_t sn65dp141_DisableChannel(uint16_t               Address,
                                  DPREDRIVER_ChannelId_t ChannelId)
{
  uint32_t err_count = 0;
  uint8_t chen;

  /* Read Channel Enable register */
  err_count += MUX_IO_Read(Address, SN65DP141_REG_CHEN, &chen);

  /* Set LN_EN_CHx bit of Channel Enable register */
  switch(ChannelId)
  {
  case CHANNEL_DP0:
    chen |= SN65DP141_REG_CHEN_LN_EN_CH0;
    break;
  case CHANNEL_DP1:
    chen |= SN65DP141_REG_CHEN_LN_EN_CH1;
    break;
  case CHANNEL_DP2:
    chen |= SN65DP141_REG_CHEN_LN_EN_CH2;
    break;
  case CHANNEL_DP3:
    chen |= SN65DP141_REG_CHEN_LN_EN_CH3;
    break;
  default:
    /* Nothing to do */
    break;
  }

  err_count += MUX_IO_Write(Address, SN65DP141_REG_CHEN, chen);

  return err_count;
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

