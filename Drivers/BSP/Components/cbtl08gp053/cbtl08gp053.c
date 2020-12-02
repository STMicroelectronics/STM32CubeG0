/**
  ******************************************************************************
  * @file    cbtl08gp053.c
  * @author  MCD Application Team
  * @brief   This file provides a set of functions needed to manage the CBTL08GP053
  *          (Crossbar switch device for USB Type-C systems).
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT(c) 2017 STMicroelectronics</center></h2>
  *
  * Redistribution and use in source and binary forms, with or without modification,
  * are permitted provided that the following conditions are met:
  *   1. Redistributions of source code must retain the above copyright notice,
  *      this list of conditions and the following disclaimer.
  *   2. Redistributions in binary form must reproduce the above copyright notice,
  *      this list of conditions and the following disclaimer in the documentation
  *      and/or other materials provided with the distribution.
  *   3. Neither the name of STMicroelectronics nor the names of its contributors
  *      may be used to endorse or promote products derived from this software
  *      without specific prior written permission.
  *
  * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
  * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
  * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
  * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
  * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
  * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
  * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
  * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
  * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
  * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "cbtl08gp053.h"

/** @addtogroup BSP
  * @{
  */

/** @addtogroup Components
  * @{
  */

/** @defgroup CBTL08GP053
  * @brief     This file provides a set of functions needed to drive the 
  *            CBTL08GP053 Crossbar switch device for USB Type-C systems.
  * @{
  */

/** @defgroup CBTL08GP053_Private_TypesDefinitions
  * @{
  */

/**
  * @}
  */

/** @defgroup CBTL08GP053_Private_Defines
  * @{
  */

/**
  * @}
  */

/** @defgroup CBTL08GP053_Private_Macros
  * @{
  */

/**
  * @}
  */

/** @defgroup CBTL08GP053_Private_Variables
  * @{
  */ 
#ifdef DBG_BSP_MUX
CBTL08GP053_TypeDef CBTL08GP053 =
{
  .SYS_CTRL    = 0x00,
  .OP1_CTRL    = 0x00,
  .OP2_CTRL    = 0x00,
  .OP3_CTRL    = 0x00,
  .OP4_CTRL    = 0x00,
  .OP5_CTRL    = 0x00,
  .CROSS5_CTRL = 0x01,
  .SW_CTRL     = 0x00,
  .SW_CTRL     = 0x00,
  .REVISION    = 0xA0
};
#endif /* DBG_BSP_MUX */

/* Type-C Crosspoint Switch Driver structure initialization */
TYPECSWITCH_Drv_t cbtl08gp053_drv = {
  cbtl08gp053_Init,
  cbtl08gp053_DeInit,
  cbtl08gp053_PowerOn,
  cbtl08gp053_PowerOff,
  cbtl08gp053_SetMode,
  cbtl08gp053_IsSupportedMode
};

/* Supported USB Type-C pin assignments */
static const uint32_t cbtl08gp053_SupportedModes = 
   ( 1 << USB_NORMAL                     |
     1 << USB_FLIPPED                    |
     1 << DFP_D_PIN_ASSIGNMENT_C_NORMAL  |
     1 << DFP_D_PIN_ASSIGNMENT_C_FLIPPED |
     1 << DFP_D_PIN_ASSIGNMENT_D_NORMAL  |
     1 << DFP_D_PIN_ASSIGNMENT_D_FLIPPED |
     1 << DFP_D_PIN_ASSIGNMENT_E_NORMAL  |
     1 << DFP_D_PIN_ASSIGNMENT_E_FLIPPED |
     1 << DFP_D_PIN_ASSIGNMENT_F_NORMAL  |
     1 << DFP_D_PIN_ASSIGNMENT_F_FLIPPED |
     1 << UFP_D_PIN_ASSIGNMENT_C_NORMAL  |
     1 << UFP_D_PIN_ASSIGNMENT_C_FLIPPED |
     1 << UFP_D_PIN_ASSIGNMENT_D_NORMAL  |
     1 << UFP_D_PIN_ASSIGNMENT_D_FLIPPED |
     1 << UFP_D_PIN_ASSIGNMENT_E_NORMAL  |
     1 << UFP_D_PIN_ASSIGNMENT_E_FLIPPED );

/**
  * @}
  */

/** @defgroup CBTL08GP053_Private_FunctionPrototypes
  * @{
  */

/**
  * @}
  */

/** @defgroup CBTL08GP053_Private_Functions
  * @{
  */

/**
  * @brief  Initialize the CBTL08GP053 and configure the needed hardware resources.
  * @param  Address CBTL08GP053 address on communication Bus.
  * @retval 0: successful, else failed
  */
uint32_t cbtl08gp053_Init(uint16_t Address)
{
  uint32_t err_count = 0;
  
  /*  Low level init */
  err_count += MUX_IO_Init();
  
  /* Restore CBTL08GP053 registers reset values */
  MUX_IO_Write(Address, CBTL08GP053_REG_SYS_CTRL,    0x00);
  MUX_IO_Write(Address, CBTL08GP053_REG_OP1_CTRL,    0x00);
  MUX_IO_Write(Address, CBTL08GP053_REG_OP2_CTRL,    0x00);
  MUX_IO_Write(Address, CBTL08GP053_REG_OP3_CTRL,    0x00);
  MUX_IO_Write(Address, CBTL08GP053_REG_OP4_CTRL,    0x00);
  MUX_IO_Write(Address, CBTL08GP053_REG_OP5_CTRL,    0x00);
  MUX_IO_Write(Address, CBTL08GP053_REG_CROSS5_CTRL, 0x01);
  MUX_IO_Write(Address, CBTL08GP053_REG_SW_CTRL,     0x00);

  return err_count;
}


/**
  * @brief  Release the hardware resources required to use the CBTL08GP053.
  * @param  Address CBTL08GP053 address on communication Bus.
  * @retval none
  */
void cbtl08gp053_DeInit(uint16_t Address)
{
  /* Restore CBTL08GP053 registers reset values */
  MUX_IO_Write(Address, CBTL08GP053_REG_SYS_CTRL,    0x00);
  MUX_IO_Write(Address, CBTL08GP053_REG_OP1_CTRL,    0x00);
  MUX_IO_Write(Address, CBTL08GP053_REG_OP2_CTRL,    0x00);
  MUX_IO_Write(Address, CBTL08GP053_REG_OP3_CTRL,    0x00);
  MUX_IO_Write(Address, CBTL08GP053_REG_OP4_CTRL,    0x00);
  MUX_IO_Write(Address, CBTL08GP053_REG_OP5_CTRL,    0x00);
  MUX_IO_Write(Address, CBTL08GP053_REG_CROSS5_CTRL, 0x01);
  MUX_IO_Write(Address, CBTL08GP053_REG_SW_CTRL,     0x00);
  
  /*  Low level de-init */
  MUX_IO_DeInit();
}
 
/**
  * @brief  Power on the CBTL08GP053.
  * @param  Address CBTL08GP053 address on communication Bus.
  * @retval 0: successful, else failed
  */
uint32_t cbtl08gp053_PowerOn(uint16_t Address)
{
  uint32_t err_count = 0;
  
  /* Set SYS_CTRL.SWTICH_EN bit */
  err_count += MUX_IO_Write(Address, CBTL08GP053_REG_SYS_CTRL, CBTL08GP053_REG_SYS_CTRL_SWITCH_EN);
  
  return err_count;
}

/**
  * @brief  Power down the CBTL08GP053.
  * @param  Address CBTL08GP053 address on communication Bus.
  * @retval 0: successful, else failed
  */
uint32_t cbtl08gp053_PowerOff(uint16_t Address)
{
  uint32_t err_count = 0;
  
  /* Clear SYS_CTRL.SWTICH_EN bit */
  err_count += MUX_IO_Write(Address, CBTL08GP053_REG_SYS_CTRL, 0x00);
  
  return err_count;
}

/**
  * @brief  Configure the CBTL08GP053 according to the requested USB Type-C
  *         connector pin assignment.
  * @param  Address CBTL08GP053 address on communication Bus.
  * @param  Mode     USB Type-C connector pin assignment
  * @retval 0: success, else error
  */
uint32_t cbtl08gp053_SetMode(uint16_t Address, TYPECSWITCH_Mode_t Mode)
{
  uint32_t err_count = 0;
  uint8_t sw_ctrl = (CBTL08GP053_REG_SW_X5_SET  |
                     CBTL08GP053_REG_SW_OP1_SET |
                     CBTL08GP053_REG_SW_OP2_SET |
                     CBTL08GP053_REG_SW_OP3_SET |
                     CBTL08GP053_REG_SW_OP4_SET |
                     CBTL08GP053_REG_SW_OP5_SET);
  
  /* Transition to safe mode prior updating cross switch configuration */
  err_count += MUX_IO_Write(Address, CBTL08GP053_REG_OP1_CTRL,    0x00);
  err_count += MUX_IO_Write(Address, CBTL08GP053_REG_OP2_CTRL,    0x00);
  err_count += MUX_IO_Write(Address, CBTL08GP053_REG_OP3_CTRL,    0x00);
  err_count += MUX_IO_Write(Address, CBTL08GP053_REG_OP4_CTRL,    0x00);
  err_count += MUX_IO_Write(Address, CBTL08GP053_REG_OP5_CTRL,    0x00);
  err_count += MUX_IO_Write(Address, CBTL08GP053_REG_CROSS5_CTRL, 0x00);

  /* Configure cross switch */
  switch(Mode)
  {
  case USB_NORMAL:
    err_count += MUX_IO_Write(Address, CBTL08GP053_REG_OP1_CTRL,    CBTL08GP053_REG_OP1_CTRL_EN_IP1);
    err_count += MUX_IO_Write(Address, CBTL08GP053_REG_OP2_CTRL,    0x00);
    err_count += MUX_IO_Write(Address, CBTL08GP053_REG_OP3_CTRL,    CBTL08GP053_REG_OP3_CTRL_EN_IP4);
    err_count += MUX_IO_Write(Address, CBTL08GP053_REG_OP4_CTRL,    0x00);
    err_count += MUX_IO_Write(Address, CBTL08GP053_REG_OP5_CTRL,    0x00);
    err_count += MUX_IO_Write(Address, CBTL08GP053_REG_CROSS5_CTRL, 0x00);
    break;
  case USB_FLIPPED:
    err_count += MUX_IO_Write(Address, CBTL08GP053_REG_OP1_CTRL,    0x00);
    err_count += MUX_IO_Write(Address, CBTL08GP053_REG_OP2_CTRL,    CBTL08GP053_REG_OP1_CTRL_EN_IP1);
    err_count += MUX_IO_Write(Address, CBTL08GP053_REG_OP3_CTRL,    0x00);
    err_count += MUX_IO_Write(Address, CBTL08GP053_REG_OP4_CTRL,    CBTL08GP053_REG_OP3_CTRL_EN_IP4);
    err_count += MUX_IO_Write(Address, CBTL08GP053_REG_OP5_CTRL,    0x00);
    err_count += MUX_IO_Write(Address, CBTL08GP053_REG_CROSS5_CTRL, 0x00);
    break;
  case DFP_D_PIN_ASSIGNMENT_C_NORMAL:
  case DFP_D_PIN_ASSIGNMENT_E_NORMAL:
    err_count += MUX_IO_Write(Address, CBTL08GP053_REG_OP1_CTRL,    CBTL08GP053_REG_OP1_CTRL_EN_IP2);
    err_count += MUX_IO_Write(Address, CBTL08GP053_REG_OP2_CTRL,    CBTL08GP053_REG_OP2_CTRL_EN_IP3);
    err_count += MUX_IO_Write(Address, CBTL08GP053_REG_OP3_CTRL,    CBTL08GP053_REG_OP3_CTRL_EN_IP5);
    err_count += MUX_IO_Write(Address, CBTL08GP053_REG_OP4_CTRL,    CBTL08GP053_REG_OP4_CTRL_EN_IP6);
    err_count += MUX_IO_Write(Address, CBTL08GP053_REG_OP5_CTRL,    CBTL08GP053_REG_OP5_CTRL_EN_IP7);
    err_count += MUX_IO_Write(Address, CBTL08GP053_REG_CROSS5_CTRL, CBTL08GP053_REG_CROSS5_PASS);
    break;
  case DFP_D_PIN_ASSIGNMENT_C_FLIPPED:
  case DFP_D_PIN_ASSIGNMENT_E_FLIPPED:
    err_count += MUX_IO_Write(Address, CBTL08GP053_REG_OP1_CTRL,    CBTL08GP053_REG_OP1_CTRL_EN_IP3);
    err_count += MUX_IO_Write(Address, CBTL08GP053_REG_OP2_CTRL,    CBTL08GP053_REG_OP2_CTRL_EN_IP2);
    err_count += MUX_IO_Write(Address, CBTL08GP053_REG_OP3_CTRL,    CBTL08GP053_REG_OP3_CTRL_EN_IP6);
    err_count += MUX_IO_Write(Address, CBTL08GP053_REG_OP4_CTRL,    CBTL08GP053_REG_OP4_CTRL_EN_IP5);
    err_count += MUX_IO_Write(Address, CBTL08GP053_REG_OP5_CTRL,    CBTL08GP053_REG_OP5_CTRL_EN_IP7);
    err_count += MUX_IO_Write(Address, CBTL08GP053_REG_CROSS5_CTRL, CBTL08GP053_REG_CROSS5_CROSS);
    break;
  case DFP_D_PIN_ASSIGNMENT_D_NORMAL:
  case DFP_D_PIN_ASSIGNMENT_F_NORMAL:
    err_count += MUX_IO_Write(Address, CBTL08GP053_REG_OP1_CTRL,    CBTL08GP053_REG_OP1_CTRL_EN_IP1);
    err_count += MUX_IO_Write(Address, CBTL08GP053_REG_OP2_CTRL,    CBTL08GP053_REG_OP2_CTRL_EN_IP3);
    err_count += MUX_IO_Write(Address, CBTL08GP053_REG_OP3_CTRL,    CBTL08GP053_REG_OP3_CTRL_EN_IP4);
    err_count += MUX_IO_Write(Address, CBTL08GP053_REG_OP4_CTRL,    CBTL08GP053_REG_OP4_CTRL_EN_IP6);
    err_count += MUX_IO_Write(Address, CBTL08GP053_REG_OP5_CTRL,    CBTL08GP053_REG_OP5_CTRL_EN_IP7);
    err_count += MUX_IO_Write(Address, CBTL08GP053_REG_CROSS5_CTRL, CBTL08GP053_REG_CROSS5_PASS);
    break;
  case DFP_D_PIN_ASSIGNMENT_D_FLIPPED:
  case DFP_D_PIN_ASSIGNMENT_F_FLIPPED:
    err_count += MUX_IO_Write(Address, CBTL08GP053_REG_OP1_CTRL,    CBTL08GP053_REG_OP1_CTRL_EN_IP3);
    err_count += MUX_IO_Write(Address, CBTL08GP053_REG_OP2_CTRL,    CBTL08GP053_REG_OP2_CTRL_EN_IP1);
    err_count += MUX_IO_Write(Address, CBTL08GP053_REG_OP3_CTRL,    CBTL08GP053_REG_OP3_CTRL_EN_IP6);
    err_count += MUX_IO_Write(Address, CBTL08GP053_REG_OP4_CTRL,    CBTL08GP053_REG_OP4_CTRL_EN_IP4);
    err_count += MUX_IO_Write(Address, CBTL08GP053_REG_OP5_CTRL,    CBTL08GP053_REG_OP5_CTRL_EN_IP7);
    err_count += MUX_IO_Write(Address, CBTL08GP053_REG_CROSS5_CTRL, CBTL08GP053_REG_CROSS5_CROSS);
    break;
  case UFP_D_PIN_ASSIGNMENT_C_NORMAL:
    err_count += MUX_IO_Write(Address, CBTL08GP053_REG_OP1_CTRL,    CBTL08GP053_REG_OP1_CTRL_EN_IP2);
    err_count += MUX_IO_Write(Address, CBTL08GP053_REG_OP2_CTRL,    CBTL08GP053_REG_OP2_CTRL_EN_IP3);
    err_count += MUX_IO_Write(Address, CBTL08GP053_REG_OP3_CTRL,    CBTL08GP053_REG_OP3_CTRL_EN_IP5);
    err_count += MUX_IO_Write(Address, CBTL08GP053_REG_OP4_CTRL,    CBTL08GP053_REG_OP4_CTRL_EN_IP6);
    err_count += MUX_IO_Write(Address, CBTL08GP053_REG_OP5_CTRL,    CBTL08GP053_REG_OP5_CTRL_EN_IP7);
    err_count += MUX_IO_Write(Address, CBTL08GP053_REG_CROSS5_CTRL, CBTL08GP053_REG_CROSS5_CROSS);
    break;
  case UFP_D_PIN_ASSIGNMENT_C_FLIPPED:
    err_count += MUX_IO_Write(Address, CBTL08GP053_REG_OP1_CTRL,    CBTL08GP053_REG_OP1_CTRL_EN_IP3);
    err_count += MUX_IO_Write(Address, CBTL08GP053_REG_OP2_CTRL,    CBTL08GP053_REG_OP2_CTRL_EN_IP2);
    err_count += MUX_IO_Write(Address, CBTL08GP053_REG_OP3_CTRL,    CBTL08GP053_REG_OP3_CTRL_EN_IP6);
    err_count += MUX_IO_Write(Address, CBTL08GP053_REG_OP4_CTRL,    CBTL08GP053_REG_OP4_CTRL_EN_IP5);
    err_count += MUX_IO_Write(Address, CBTL08GP053_REG_OP5_CTRL,    CBTL08GP053_REG_OP5_CTRL_EN_IP7);
    err_count += MUX_IO_Write(Address, CBTL08GP053_REG_CROSS5_CTRL, CBTL08GP053_REG_CROSS5_PASS);
    break;
  case UFP_D_PIN_ASSIGNMENT_D_NORMAL:
    err_count += MUX_IO_Write(Address, CBTL08GP053_REG_OP1_CTRL,    CBTL08GP053_REG_OP1_CTRL_EN_IP1);
    err_count += MUX_IO_Write(Address, CBTL08GP053_REG_OP2_CTRL,    CBTL08GP053_REG_OP2_CTRL_EN_IP3);
    err_count += MUX_IO_Write(Address, CBTL08GP053_REG_OP3_CTRL,    CBTL08GP053_REG_OP3_CTRL_EN_IP4);
    err_count += MUX_IO_Write(Address, CBTL08GP053_REG_OP4_CTRL,    CBTL08GP053_REG_OP4_CTRL_EN_IP6);
    err_count += MUX_IO_Write(Address, CBTL08GP053_REG_OP5_CTRL,    CBTL08GP053_REG_OP5_CTRL_EN_IP7);
    err_count += MUX_IO_Write(Address, CBTL08GP053_REG_CROSS5_CTRL, CBTL08GP053_REG_CROSS5_CROSS);
    break;
  case UFP_D_PIN_ASSIGNMENT_D_FLIPPED:
    err_count += MUX_IO_Write(Address, CBTL08GP053_REG_OP1_CTRL,    CBTL08GP053_REG_OP1_CTRL_EN_IP3);
    err_count += MUX_IO_Write(Address, CBTL08GP053_REG_OP2_CTRL,    CBTL08GP053_REG_OP2_CTRL_EN_IP1);
    err_count += MUX_IO_Write(Address, CBTL08GP053_REG_OP3_CTRL,    CBTL08GP053_REG_OP3_CTRL_EN_IP6);
    err_count += MUX_IO_Write(Address, CBTL08GP053_REG_OP4_CTRL,    CBTL08GP053_REG_OP4_CTRL_EN_IP4);
    err_count += MUX_IO_Write(Address, CBTL08GP053_REG_OP5_CTRL,    CBTL08GP053_REG_OP5_CTRL_EN_IP7);
    err_count += MUX_IO_Write(Address, CBTL08GP053_REG_CROSS5_CTRL, CBTL08GP053_REG_CROSS5_PASS);
    break;
  case UFP_D_PIN_ASSIGNMENT_E_NORMAL:
    err_count += MUX_IO_Write(Address, CBTL08GP053_REG_OP1_CTRL,    CBTL08GP053_REG_OP1_CTRL_EN_IP2);
    err_count += MUX_IO_Write(Address, CBTL08GP053_REG_OP2_CTRL,    CBTL08GP053_REG_OP2_CTRL_EN_IP3);
    err_count += MUX_IO_Write(Address, CBTL08GP053_REG_OP3_CTRL,    CBTL08GP053_REG_OP3_CTRL_EN_IP5);
    err_count += MUX_IO_Write(Address, CBTL08GP053_REG_OP4_CTRL,    CBTL08GP053_REG_OP4_CTRL_EN_IP6);
    err_count += MUX_IO_Write(Address, CBTL08GP053_REG_OP5_CTRL,    CBTL08GP053_REG_OP5_CTRL_EN_IP7);
    err_count += MUX_IO_Write(Address, CBTL08GP053_REG_CROSS5_CTRL, CBTL08GP053_REG_CROSS5_CROSS);
    break;
  case UFP_D_PIN_ASSIGNMENT_E_FLIPPED:
    err_count += MUX_IO_Write(Address, CBTL08GP053_REG_OP1_CTRL,    CBTL08GP053_REG_OP1_CTRL_EN_IP3);
    err_count += MUX_IO_Write(Address, CBTL08GP053_REG_OP2_CTRL,    CBTL08GP053_REG_OP2_CTRL_EN_IP2);
    err_count += MUX_IO_Write(Address, CBTL08GP053_REG_OP3_CTRL,    CBTL08GP053_REG_OP3_CTRL_EN_IP6);
    err_count += MUX_IO_Write(Address, CBTL08GP053_REG_OP4_CTRL,    CBTL08GP053_REG_OP4_CTRL_EN_IP5);
    err_count += MUX_IO_Write(Address, CBTL08GP053_REG_OP5_CTRL,    CBTL08GP053_REG_OP5_CTRL_EN_IP7);
    err_count += MUX_IO_Write(Address, CBTL08GP053_REG_CROSS5_CTRL, CBTL08GP053_REG_CROSS5_PASS);
    break;
  default:
    err_count += MUX_IO_Write(Address, CBTL08GP053_REG_OP1_CTRL,    0x00);
    err_count += MUX_IO_Write(Address, CBTL08GP053_REG_OP2_CTRL,    0x00);
    err_count += MUX_IO_Write(Address, CBTL08GP053_REG_OP3_CTRL,    0x00);
    err_count += MUX_IO_Write(Address, CBTL08GP053_REG_OP4_CTRL,    0x00);
    err_count += MUX_IO_Write(Address, CBTL08GP053_REG_OP5_CTRL,    0x00);
    err_count += MUX_IO_Write(Address, CBTL08GP053_REG_CROSS5_CTRL, 0x00);
    err_count += MUX_IO_Write(Address, CBTL08GP053_REG_SW_CTRL,     sw_ctrl);
    break;
  }

  /* Enable cross switch configuration */
  err_count += MUX_IO_Write(Address, CBTL08GP053_REG_SW_CTRL, sw_ctrl);

  return err_count;
}

/**
  * @brief  Indicate whether the requested USB Type-C connector pin assignment
  *         is supported by the CBTL08GP053.
  * @param  Mode USB Type-C connector pin assignment
  * @retval 0: success, else error
  */
uint32_t  cbtl08gp053_IsSupportedMode(TYPECSWITCH_Mode_t Mode)
{
  return (((1 << Mode) & cbtl08gp053_SupportedModes ) == 0) ? 0 : 1;
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
