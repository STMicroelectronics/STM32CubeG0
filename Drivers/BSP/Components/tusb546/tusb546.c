/**
  ******************************************************************************
  * @file    tusb546.c
  * @author  MCD Application Team
  * @brief   This file provides a set of functions needed to manage the TUSB546
  *          (USB Type-C DP ALT Mode Linear Redriver Crosspoint Switch).
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
#include "tusb546.h"

/** @addtogroup BSP
  * @{
  */

/** @addtogroup Components
  * @{
  */

/** @defgroup TUSB546
  * @brief     This file provides a set of functions needed to drive the
  *            TUSB546 USB Type-C DP ALT Mode Linear Redriver Crosspoint Switch.
  * @{
  */

/** @defgroup TUSB546_Private_TypesDefinitions
  * @{
  */

/**
  * @}
  */

/** @defgroup TUSB546_Private_Defines
  * @{
  */

/**
  * @}
  */

/** @defgroup TUSB546_Private_Macros
  * @{
  */

/**
  * @}
  */

/** @defgroup TUSB546_Private_Variables
  * @{
  */

/* Type-C Crosspoint Switch Driver structure initialization */
TYPECSWITCH_Drv_t tusb546_drv_CrossSwitch =
{
  tusb546_CrossSwitch_Init,
  tusb546_CrossSwitch_DeInit,
  tusb546_CrossSwitch_PowerOn,
  tusb546_CrossSwitch_PowerOff,
  tusb546_CrossSwitch_SetMode,
  tusb546_CrossSwitch_IsSupportedMode
};

/* DisplayPort Linear Redriver driver structure initialization*/
DPREDRIVER_Drv_t tusb546_drv_LinearRedriver =
{
  tusb546_DPRedriver_Init,
  tusb546_DPRedriver_DeInit,
  tusb546_DPRedriver_PowerOn,
  tusb546_DPRedriver_PowerOff,
  tusb546_DPRedriver_SetEQGain,
  tusb546_DPRedriver_EnableChannel,
  tusb546_DPRedriver_DisableChannel
};

/* Supported USB Type-C pin assignments */
static const uint32_t tusb546_SupportedModes =
   ( 1 << USB_NORMAL                     |
     1 << USB_FLIPPED                    |
     1 << DFP_D_PIN_ASSIGNMENT_C_NORMAL  |
     1 << DFP_D_PIN_ASSIGNMENT_C_FLIPPED |
     1 << DFP_D_PIN_ASSIGNMENT_D_NORMAL  |
     1 << DFP_D_PIN_ASSIGNMENT_D_FLIPPED |
     1 << DFP_D_PIN_ASSIGNMENT_E_NORMAL  |
     1 << DFP_D_PIN_ASSIGNMENT_E_FLIPPED |
     1 << DFP_D_PIN_ASSIGNMENT_F_NORMAL  |
     1 << DFP_D_PIN_ASSIGNMENT_F_FLIPPED );

static uint8_t tusb546_IsInitialized_CrossSwitch = 0;
static uint8_t tusb546_IsInitialized_DPRedriver = 0;

/**
  * @}
  */



/** @defgroup TUSB546_Private_FunctionPrototypes
  * @{
  */
/**
  * @}
  */

/** @defgroup TUSB546_Private_Functions
  * @{
  */

/**
  * @brief  Initialize the TUSB546 and configure the needed hardware resources
  *          (Type-C cross switch part).
  * @param  Address TUSB546 address on communication Bus.
  * @retval 0: successful, else failed
  */
uint32_t tusb546_CrossSwitch_Init(uint16_t Address)
{
  uint32_t err_count = 0;

  tusb546_IsInitialized_CrossSwitch  = 1;

  if (!tusb546_IsInitialized_DPRedriver)
  {
    /*  Low level init */
    err_count += MUX_IO_Init();

    /* Restore TUSB546 registers reset values */
    err_count += MUX_IO_Write(Address, TUSB546_REG_CTRL,       0x01);
    err_count += MUX_IO_Write(Address, TUSB546_REG_DP_CTRL1,   0x00);
    err_count += MUX_IO_Write(Address, TUSB546_REG_DP_CTRL2,   0x00);
    err_count += MUX_IO_Write(Address, TUSB546_REG_DP_CTRL4,   0x00);
    err_count += MUX_IO_Write(Address, TUSB546_REG_USB3_CTRL1, 0x00);
    err_count += MUX_IO_Write(Address, TUSB546_REG_USB3_CTRL2, 0x00);
    err_count += MUX_IO_Write(Address, TUSB546_REG_USB3_CTRL3, 0x00);
  }

  return err_count;
}

/**
  * @brief  Release the hardware resources required to use the TUSB546
  *         (Type-C cross switch part).
  * @param  Address TUSB546 address on communication Bus.
  * @retval none
  */
void tusb546_CrossSwitch_DeInit(uint16_t Address)
{
  tusb546_IsInitialized_CrossSwitch  = 0;

  if ((!tusb546_IsInitialized_CrossSwitch) &&
      (!tusb546_IsInitialized_DPRedriver))
  {
    /* Restore TUSB546 registers reset values */
    MUX_IO_Write(Address, TUSB546_REG_CTRL,       0x01);
    MUX_IO_Write(Address, TUSB546_REG_DP_CTRL1,   0x00);
    MUX_IO_Write(Address, TUSB546_REG_DP_CTRL2,   0x00);
    MUX_IO_Write(Address, TUSB546_REG_DP_CTRL4,   0x00);
    MUX_IO_Write(Address, TUSB546_REG_USB3_CTRL1, 0x00);
    MUX_IO_Write(Address, TUSB546_REG_USB3_CTRL2, 0x00);
    MUX_IO_Write(Address, TUSB546_REG_USB3_CTRL3, 0x00);

    /*  Low level de-init */
    MUX_IO_DeInit();
  }
}

/**
  * @brief  Power on the TUSB546 (Type-C cross switch part).
  * @param  Address TUSB546 address on communication Bus.
  * @retval 0: successful, else failed
  */
uint32_t tusb546_CrossSwitch_PowerOn(uint16_t Address)
{
  uint32_t err_count = 0;

  /* Enable USB3x port */
  err_count += MUX_IO_Write(Address, TUSB546_REG_CTRL, TUSB546_REG_CTRL_CTLSEL_0);

  return err_count;
}

/**
  * @brief  Power down the TUSB546 (Type-C cross switch part).
  * @param  Address TUSB546 address on communication Bus.
  * @retval 0: successful, else failed
  */
uint32_t tusb546_CrossSwitch_PowerOff(uint16_t Address)
{
  uint32_t err_count = 0;

  /* All RX and TX for USB3 and DisplayPort are disabled */
  err_count += MUX_IO_Write(Address, TUSB546_REG_CTRL, 0);

  return err_count;
}

/**
  * @brief  Configure the TUSB546 according to the requested USB Type-C
  *         connector pin assignment.
  * @param  Address TUSB546 address on communication Bus.
  * @param  Mode    USB Type-C connector pin assignment
  * @retval 0: success, else error
  */
uint32_t tusb546_CrossSwitch_SetMode(uint16_t Address, TYPECSWITCH_Mode_t Mode)
{
  uint32_t err_count = 0;
  uint8_t flipsel;
  uint8_t ctlsel;
  uint8_t ctlreg;

  switch(Mode)
  {
  case USB_NORMAL:
    flipsel = 0;
    ctlsel = TUSB546_REG_CTRL_CTLSEL_0;
    break;
  case USB_FLIPPED:
    flipsel = TUSB546_REG_CTRL_FLIPSEL;
    ctlsel = TUSB546_REG_CTRL_CTLSEL_0;
    break;
  case DFP_D_PIN_ASSIGNMENT_C_NORMAL:
  case DFP_D_PIN_ASSIGNMENT_E_NORMAL:
    flipsel = 0;
    ctlsel = TUSB546_REG_CTRL_CTLSEL_1;
    break;
  case DFP_D_PIN_ASSIGNMENT_C_FLIPPED:
  case DFP_D_PIN_ASSIGNMENT_E_FLIPPED:
    flipsel = TUSB546_REG_CTRL_FLIPSEL;
    ctlsel = TUSB546_REG_CTRL_CTLSEL_1;
    break;
  case DFP_D_PIN_ASSIGNMENT_D_NORMAL:
  case DFP_D_PIN_ASSIGNMENT_F_NORMAL:
    flipsel = 0;
    ctlsel = TUSB546_REG_CTRL_CTLSEL;
    break;
  case DFP_D_PIN_ASSIGNMENT_D_FLIPPED:
  case DFP_D_PIN_ASSIGNMENT_F_FLIPPED:
    flipsel = TUSB546_REG_CTRL_FLIPSEL;
    ctlsel = TUSB546_REG_CTRL_CTLSEL;
    break;
  default:
    flipsel = 0;
    ctlsel = 0;
    break;
  }

  /* Get actual device configuration */
  err_count += MUX_IO_Read(Address, TUSB546_REG_CTRL, &ctlreg);
  ctlreg = (ctlreg & ~(uint8_t)(TUSB546_REG_CTRL_FLIPSEL | TUSB546_REG_CTRL_CTLSEL)) | (flipsel | ctlsel);

  /* Update device configuration */
  err_count += MUX_IO_Write(Address, TUSB546_REG_CTRL, ctlreg);

  return err_count;
}

/**
  * @brief  Indicate whether the requested USB Type-C connector pin assignment
  *         is supported by the TUSB546.
  * @param  Mode USB Type-C connector pin assignment
  * @retval 1: Mode supported, else Mode not supported
  */
uint32_t  tusb546_CrossSwitch_IsSupportedMode(TYPECSWITCH_Mode_t Mode)
{
  return (((1 << Mode) & tusb546_SupportedModes ) == 0) ? 0 : 1;
}

/**
  * @brief  Initialize the TUSB546 and configure the needed hardware resources
  *          (DisplayPort linear redriver part).
  * @param  Address TUSB546 address on communication Bus.
  * @retval 0: successful, else failed
  */
uint32_t tusb546_DPRedriver_Init(uint16_t Address)
{
  uint32_t err_count = 0;

  tusb546_IsInitialized_DPRedriver  = 1;

  if (!tusb546_IsInitialized_CrossSwitch)
  {
    /*  Low level init */
    err_count += MUX_IO_Init();
  }

  return err_count;
}

/**
  * @brief  Release the hardware resources required to use the TUSB546
  *         (DisplayPort linear redriver part).
  * @param  Address TUSB546 address on communication Bus.
  * @retval none
  */
void tusb546_DPRedriver_DeInit(uint16_t Address)
{
  tusb546_IsInitialized_DPRedriver  = 0;

  if ((!tusb546_IsInitialized_CrossSwitch) &&
      (!tusb546_IsInitialized_DPRedriver))
  {
    /* Restore TUSB546 registers reset values */
    MUX_IO_Write(Address, TUSB546_REG_CTRL,       0x01);
    MUX_IO_Write(Address, TUSB546_REG_DP_CTRL1,   0x00);
    MUX_IO_Write(Address, TUSB546_REG_DP_CTRL2,   0x00);
    MUX_IO_Write(Address, TUSB546_REG_DP_CTRL4,   0x00);
    MUX_IO_Write(Address, TUSB546_REG_USB3_CTRL1, 0x00);
    MUX_IO_Write(Address, TUSB546_REG_USB3_CTRL2, 0x00);
    MUX_IO_Write(Address, TUSB546_REG_USB3_CTRL3, 0x00);

    /*  Low level de-init */
    MUX_IO_DeInit();
  }
}

/**
  * @brief  Power on the TUSB546 (DisplayPort linear redriver part).
  * @param  Address TUSB546 address on communication Bus.
  * @retval 0: successful, else failed
  */
uint32_t tusb546_DPRedriver_PowerOn(uint16_t Address)
{
  return 0;
}

/**
  * @brief  Power down the TUSB546 (DisplayPort linear redriver part).
  * @param  Address TUSB546 address on communication Bus.
  * @retval 0: successful, else failed
  */
uint32_t tusb546_DPRedriver_PowerOff(uint16_t Address)
{
  return 0;
}

/**
  * @brief  Set the equalizer gain for a given channel.
  * @param  Address TUSB546 address on communication Bus.
  * @param  ChannelId Channel identifier.
  *         This parameter can be take one of the following values:
  *         CHANNEL_DP0
  *         CHANNEL_DP1
  *         CHANNEL_DP2
  *         CHANNEL_DP3
  *         CHANNEL_RX1
  *         CHANNEL_RX2
  *         CHANNEL_SSTX
  * @param  EQGain Equalizer gain.
  *         This parameter must be a value between 0x00 and 0x0F.
  * @retval 0: successful, else failed
  */
uint32_t tusb546_DPRedriver_SetEQGain(uint16_t               Address,
                                      DPREDRIVER_ChannelId_t ChannelId,
                                      uint8_t                EQGain)
{
  uint32_t err_count = 0;
  uint8_t  eqsel = 0;
  uint8_t  reg_offset = 0;
  uint8_t  ctlreg;
  uint8_t  mask = 0;

  switch(ChannelId)
  {
  case CHANNEL_DP0:
    reg_offset = TUSB546_REG_DP_CTRL1;
    mask       = TUSB546_REG_DP_CTRL1_DP0EQ_SEL_Msk;
    eqsel      = EQGain;
    break;
  case CHANNEL_DP1:
    reg_offset = TUSB546_REG_DP_CTRL1;
    mask       = TUSB546_REG_DP_CTRL1_DP1EQ_SEL_Msk;
    eqsel      = (EQGain << TUSB546_REG_DP_CTRL1_DP1EQ_SEL_Pos);
    break;
  case CHANNEL_DP2:
    reg_offset = TUSB546_REG_DP_CTRL2;
    mask       = TUSB546_REG_DP_CTRL2_DP2EQ_SEL_Msk;
    eqsel      = EQGain;
    break;
  case CHANNEL_DP3:
    reg_offset = TUSB546_REG_DP_CTRL2;
    mask       = TUSB546_REG_DP_CTRL2_DP3EQ_SEL_Msk;
    eqsel      = (EQGain << TUSB546_REG_DP_CTRL2_DP3EQ_SEL_Pos);
    break;
  case CHANNEL_RX1:
    reg_offset = TUSB546_REG_USB3_CTRL1;
    mask       = TUSB546_REG_USB3_CTRL1_EQ1_SEL_Msk;
    eqsel      = EQGain;
    break;
  case CHANNEL_RX2:
    reg_offset = TUSB546_REG_USB3_CTRL1;
    mask       = TUSB546_REG_USB3_CTRL1_EQ2_SEL_Msk;
    eqsel      = (EQGain << TUSB546_REG_USB3_CTRL1_EQ2_SEL_Pos);
    break;
  case CHANNEL_SSTX:
    reg_offset = TUSB546_REG_USB3_CTRL2;
    mask       = TUSB546_USB3_CTRL2_SSEQ_SEL_Msk;
    eqsel      = EQGain;
    break;
  }

  if (reg_offset != 0)
  {
    /* Set EQ_OVERRIDE to use EQ settings from registers instead of value sample from pins */
    err_count += MUX_IO_Write(Address, TUSB546_REG_CTRL, TUSB546_REG_CTRL_EQ_OVERRIDE);

    /* Get actual EQ configuration */
    err_count += MUX_IO_Read(Address, reg_offset, &ctlreg);
    ctlreg = (ctlreg & ~(mask)) | eqsel;

    /* Update EQ configuration */
    err_count += MUX_IO_Write(Address, reg_offset, ctlreg);
  }

  return err_count;
}

/**
  * @brief  Enable a DP channel.
  * @param  Address TUSB546 address on communication Bus.
  * @param  ChannelId Channel identifier.
  *         This parameter can be take one of the following values:
  *         CHANNEL_DP0
  *         CHANNEL_DP1
  *         CHANNEL_DP2
  *         CHANNEL_DP3
  * @retval 0: successful, else failed
  */
uint32_t  tusb546_DPRedriver_EnableChannel(uint16_t               Address,
                                           DPREDRIVER_ChannelId_t ChannelId)
{
  uint32_t err_count = 0;
  uint8_t  ctlreg;

  err_count += MUX_IO_Read(Address, TUSB546_REG_DP_CTRL4, &ctlreg);

  switch(ChannelId)
  {
  case CHANNEL_DP0:
    ctlreg &= ~TUSB546_REG_DP_CTRL4_DP0_DISABLE;
    break;
  case CHANNEL_DP1:
    ctlreg &= ~TUSB546_REG_DP_CTRL4_DP1_DISABLE;
    break;
  case CHANNEL_DP2:
    ctlreg &= ~TUSB546_REG_DP_CTRL4_DP2_DISABLE;
    break;
  case CHANNEL_DP3:
    ctlreg &= ~TUSB546_REG_DP_CTRL4_DP3_DISABLE;
    break;
  default:
    /* Nothing to do */
    break;
  }

  /* Enable the DP line */
  err_count += MUX_IO_Write(Address, TUSB546_REG_DP_CTRL4, ctlreg);

  return err_count;
}

/**
  * @brief  Disable a DP channel.
  * @param  Address TUSB546 address on communication Bus.
  * @param  ChannelId Channel identifier.
  *         This parameter can be take one of the following values:
  *         CHANNEL_DP0
  *         CHANNEL_DP1
  *         CHANNEL_DP2
  *         CHANNEL_DP3
  * @retval 0: successful, else failed
  */
uint32_t  tusb546_DPRedriver_DisableChannel(uint16_t               Address,
                                            DPREDRIVER_ChannelId_t ChannelId)
{
  uint32_t err_count = 0;
  uint8_t  ctlreg;

  err_count += MUX_IO_Read(Address, TUSB546_REG_DP_CTRL4, &ctlreg);

  switch(ChannelId)
  {
  case CHANNEL_DP0:
    ctlreg |= TUSB546_REG_DP_CTRL4_DP0_DISABLE;
    break;
  case CHANNEL_DP1:
    ctlreg |= TUSB546_REG_DP_CTRL4_DP1_DISABLE;
    break;
  case CHANNEL_DP2:
    ctlreg |= TUSB546_REG_DP_CTRL4_DP2_DISABLE;
    break;
  case CHANNEL_DP3:
    ctlreg |= TUSB546_REG_DP_CTRL4_DP3_DISABLE;
    break;
  default:
    /* Nothing to do */
    break;
  }

  /* Disable the DP line */
  err_count += MUX_IO_Write(Address, TUSB546_REG_DP_CTRL4, ctlreg);

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
