/**
  ******************************************************************************
  * @file    cbtl08gp053.h
  * @author  MCD Application Team
  * @brief   This file contains all the functions prototypes for the CBTL08GP053
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
  

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __CBTL08GP053_H
#define __CBTL08GP053_H

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include <stdint.h>
#include "../Common/usbtypecswitch.h"

/** @addtogroup BSP
  * @{
  */

/** @addtogroup Components
  * @{
  */

/** @addtogroup CBTL08GP053
  * @{
  */
  
#ifdef DBG_BSP_MUX
typedef struct
{
  uint8_t RESERVED1;      /*!< Address offset: 0x00 */
  uint8_t SYS_CTRL;       /*!< Address offset: 0x01 */
  uint8_t OP1_CTRL;       /*!< Address offset: 0x02 */
  uint8_t OP2_CTRL;       /*!< Address offset: 0x03 */
  uint8_t OP3_CTRL;       /*!< Address offset: 0x04 */
  uint8_t OP4_CTRL;       /*!< Address offset: 0x05 */
  uint8_t OP5_CTRL;       /*!< Address offset: 0x06 */
  uint8_t CROSS5_CTRL;    /*!< Address offset: 0x07 */
  uint8_t SW_CTRL;        /*!< Address offset: 0x08 */
  uint8_t REVISION;       /*!< Address offset: 0x09 */
} CBTL08GP053_TypeDef;

extern CBTL08GP053_TypeDef CBTL08GP053;
#endif /* DBG_BSP_MUX */

/** @defgroup CBTL08GP053_Exported_Constants
  * @{
  */
/******************************* Register Map  ********************************/
#define CBTL08GP053_REG_SYS_CTRL     0x01  /*!< */
#define CBTL08GP053_REG_OP1_CTRL     0x02  /*!< */
#define CBTL08GP053_REG_OP2_CTRL     0x03  /*!< */
#define CBTL08GP053_REG_OP3_CTRL     0x04  /*!< */
#define CBTL08GP053_REG_OP4_CTRL     0x05  /*!< */
#define CBTL08GP053_REG_OP5_CTRL     0x06  /*!< */
#define CBTL08GP053_REG_CROSS5_CTRL  0x07  /*!< */
#define CBTL08GP053_REG_SW_CTRL      0x08  /*!< */
#define CBTL08GP053_REG_REVISION     0x09  /*!< */

/*********************  Bit definition for SYS_CTRL register  *****************/
#define CBTL08GP053_REG_SYS_CTRL_SWITCH_EN_Pos         (7U)
#define CBTL08GP053_REG_SYS_CTRL_SWITCH_EN_Msk         (0x1U << CBTL08GP053_REG_SYS_CTRL_SWITCH_EN_Pos)  /*!< 0x80*/
#define CBTL08GP053_REG_SYS_CTRL_SWITCH_EN             CBTL08GP053_REG_SYS_CTRL_SWITCH_EN_Msk            /*!< */
   
/*********************  Bit definition for OP1_CTRL register  *****************/
#define CBTL08GP053_REG_OP1_CTRL_EN_IP1_Pos         (0U)
#define CBTL08GP053_REG_OP1_CTRL_EN_IP1_Msk         (0x1U << CBTL08GP053_REG_OP1_CTRL_EN_IP1_Pos)        /*!< 0x01 */
#define CBTL08GP053_REG_OP1_CTRL_EN_IP1             CBTL08GP053_REG_OP1_CTRL_EN_IP1_Msk                  /*!< */
#define CBTL08GP053_REG_OP1_CTRL_EN_IP2_Pos         (1U)
#define CBTL08GP053_REG_OP1_CTRL_EN_IP2_Msk         (0x1U << CBTL08GP053_REG_OP1_CTRL_EN_IP2_Pos)        /*!< 0x02 */
#define CBTL08GP053_REG_OP1_CTRL_EN_IP2             CBTL08GP053_REG_OP1_CTRL_EN_IP2_Msk                  /*!< */
#define CBTL08GP053_REG_OP1_CTRL_EN_IP3_Pos         (2U)
#define CBTL08GP053_REG_OP1_CTRL_EN_IP3_Msk         (0x1U << CBTL08GP053_REG_OP1_CTRL_EN_IP3_Pos)        /*!< 0x04 */
#define CBTL08GP053_REG_OP1_CTRL_EN_IP3             CBTL08GP053_REG_OP1_CTRL_EN_IP3_Msk                  /*!< */
   
/*********************  Bit definition for OP2_CTRL register  *****************/
#define CBTL08GP053_REG_OP2_CTRL_EN_IP1_Pos         (0U)
#define CBTL08GP053_REG_OP2_CTRL_EN_IP1_Msk         (0x1U << CBTL08GP053_REG_OP2_CTRL_EN_IP1_Pos)        /*!< 0x01 */
#define CBTL08GP053_REG_OP2_CTRL_EN_IP1             CBTL08GP053_REG_OP2_CTRL_EN_IP1_Msk                  /*!< */
#define CBTL08GP053_REG_OP2_CTRL_EN_IP2_Pos         (1U)
#define CBTL08GP053_REG_OP2_CTRL_EN_IP2_Msk         (0x1U << CBTL08GP053_REG_OP2_CTRL_EN_IP2_Pos)        /*!< 0x02 */
#define CBTL08GP053_REG_OP2_CTRL_EN_IP2             CBTL08GP053_REG_OP2_CTRL_EN_IP2_Msk                  /*!< */
#define CBTL08GP053_REG_OP2_CTRL_EN_IP3_Pos         (2U)
#define CBTL08GP053_REG_OP2_CTRL_EN_IP3_Msk         (0x1U << CBTL08GP053_REG_OP2_CTRL_EN_IP3_Pos)        /*!< 0x04 */
#define CBTL08GP053_REG_OP2_CTRL_EN_IP3             CBTL08GP053_REG_OP2_CTRL_EN_IP3_Msk                  /*!< */
   
/*********************  Bit definition for OP3_CTRL register  *****************/
#define CBTL08GP053_REG_OP3_CTRL_EN_IP4_Pos         (3U)
#define CBTL08GP053_REG_OP3_CTRL_EN_IP4_Msk         (0x1U << CBTL08GP053_REG_OP3_CTRL_EN_IP4_Pos)        /*!< 0x08 */
#define CBTL08GP053_REG_OP3_CTRL_EN_IP4             CBTL08GP053_REG_OP3_CTRL_EN_IP4_Msk                  /*!< */
#define CBTL08GP053_REG_OP3_CTRL_EN_IP5_Pos         (4U)
#define CBTL08GP053_REG_OP3_CTRL_EN_IP5_Msk         (0x1U << CBTL08GP053_REG_OP3_CTRL_EN_IP5_Pos)        /*!< 0x10 */
#define CBTL08GP053_REG_OP3_CTRL_EN_IP5             CBTL08GP053_REG_OP3_CTRL_EN_IP5_Msk                  /*!< */
#define CBTL08GP053_REG_OP3_CTRL_EN_IP6_Pos         (5U)
#define CBTL08GP053_REG_OP3_CTRL_EN_IP6_Msk         (0x1U << CBTL08GP053_REG_OP3_CTRL_EN_IP6_Pos)        /*!< 0x20 */
#define CBTL08GP053_REG_OP3_CTRL_EN_IP6             CBTL08GP053_REG_OP3_CTRL_EN_IP6_Msk                  /*!< */
   
/*********************  Bit definition for OP4_CTRL register  *****************/
#define CBTL08GP053_REG_OP4_CTRL_EN_IP4_Pos         (3U)
#define CBTL08GP053_REG_OP4_CTRL_EN_IP4_Msk         (0x1U << CBTL08GP053_REG_OP4_CTRL_EN_IP4_Pos)        /*!< 0x08 */
#define CBTL08GP053_REG_OP4_CTRL_EN_IP4             CBTL08GP053_REG_OP4_CTRL_EN_IP4_Msk                  /*!< */
#define CBTL08GP053_REG_OP4_CTRL_EN_IP5_Pos         (4U)
#define CBTL08GP053_REG_OP4_CTRL_EN_IP5_Msk         (0x1U << CBTL08GP053_REG_OP4_CTRL_EN_IP5_Pos)        /*!< 0x10 */
#define CBTL08GP053_REG_OP4_CTRL_EN_IP5             CBTL08GP053_REG_OP4_CTRL_EN_IP5_Msk                  /*!< */
#define CBTL08GP053_REG_OP4_CTRL_EN_IP6_Pos         (5U)
#define CBTL08GP053_REG_OP4_CTRL_EN_IP6_Msk         (0x1U << CBTL08GP053_REG_OP4_CTRL_EN_IP6_Pos)        /*!< 0x20 */
#define CBTL08GP053_REG_OP4_CTRL_EN_IP6             CBTL08GP053_REG_OP4_CTRL_EN_IP6_Msk                  /*!< */
   
/*********************  Bit definition for OP5_CTRL register  *****************/
#define CBTL08GP053_REG_OP5_CTRL_EN_IP7_Pos         (6U)
#define CBTL08GP053_REG_OP5_CTRL_EN_IP7_Msk         (0x1U << CBTL08GP053_REG_OP5_CTRL_EN_IP7_Pos)        /*!< 0x40 */
#define CBTL08GP053_REG_OP5_CTRL_EN_IP7             CBTL08GP053_REG_OP5_CTRL_EN_IP7_Msk                  /*!< */
#define CBTL08GP053_REG_OP5_CTRL_EN_IP8_Pos         (7U)
#define CBTL08GP053_REG_OP5_CTRL_EN_IP8_Msk         (0x1U << CBTL08GP053_REG_OP5_CTRL_EN_IP8_Pos)        /*!< 0x80 */
#define CBTL08GP053_REG_OP5_CTRL_EN_IP8             CBTL08GP053_REG_OP5_CTRL_EN_IP8_Msk                  /*!< */
   
/******************  Bit definition for CROSS5_CTRL register  *****************/
#define CBTL08GP053_REG_CROSS5_PASS_Pos             (0U)
#define CBTL08GP053_REG_CROSS5_PASS_Msk             (0x1U << CBTL08GP053_REG_CROSS5_PASS_Pos)            /*!< 0x01 */
#define CBTL08GP053_REG_CROSS5_PASS                 CBTL08GP053_REG_CROSS5_PASS_Msk                      /*!< */
#define CBTL08GP053_REG_CROSS5_CROSS_Pos            (1U)
#define CBTL08GP053_REG_CROSS5_CROSS_Msk            (0x1U << CBTL08GP053_REG_CROSS5_CROSS_Pos)           /*!< 0x02 */
#define CBTL08GP053_REG_CROSS5_CROSS                CBTL08GP053_REG_CROSS5_CROSS_Msk                     /*!< */

/*******************  Bit definition for SW_CTRL register  ********************/
#define CBTL08GP053_REG_SW_OP1_SET_Pos              (0U)
#define CBTL08GP053_REG_SW_OP1_SET_Msk              (0x1U << CBTL08GP053_REG_SW_OP1_SET_Pos)             /*!< 0x01 */
#define CBTL08GP053_REG_SW_OP1_SET                   CBTL08GP053_REG_SW_OP1_SET_Msk                       /*!< */
#define CBTL08GP053_REG_SW_OP2_SET_Pos              (1U)
#define CBTL08GP053_REG_SW_OP2_SET_Msk              (0x1U << CBTL08GP053_REG_SW_OP2_SET_Pos)             /*!< 0x02 */
#define CBTL08GP053_REG_SW_OP2_SET                   CBTL08GP053_REG_SW_OP2_SET_Msk                       /*!< */
#define CBTL08GP053_REG_SW_OP3_SET_Pos              (2U)
#define CBTL08GP053_REG_SW_OP3_SET_Msk              (0x1U << CBTL08GP053_REG_SW_OP3_SET_Pos)             /*!< 0x04 */
#define CBTL08GP053_REG_SW_OP3_SET                   CBTL08GP053_REG_SW_OP3_SET_Msk                       /*!< */
#define CBTL08GP053_REG_SW_OP4_SET_Pos              (3U)
#define CBTL08GP053_REG_SW_OP4_SET_Msk              (0x1U << CBTL08GP053_REG_SW_OP4_SET_Pos)             /*!< 0x08 */
#define CBTL08GP053_REG_SW_OP4_SET                   CBTL08GP053_REG_SW_OP4_SET_Msk                       /*!< */
#define CBTL08GP053_REG_SW_OP5_SET_Pos              (4U)
#define CBTL08GP053_REG_SW_OP5_SET_Msk              (0x1U << CBTL08GP053_REG_SW_OP5_SET_Pos)             /*!< 0x10 */
#define CBTL08GP053_REG_SW_OP5_SET                   CBTL08GP053_REG_SW_OP5_SET_Msk                       /*!< */
#define CBTL08GP053_REG_SW_X5_SET_Pos               (5U)
#define CBTL08GP053_REG_SW_X5_SET_Msk               (0x1U << CBTL08GP053_REG_SW_X5_SET_Pos)              /*!< 0x20 */
#define CBTL08GP053_REG_SW_X5_SET                    CBTL08GP053_REG_SW_X5_SET_Msk                        /*!< */
   
/********************  Bit definition for REVISION register  ******************/
#define CBTL08GP053_REVISION_REV_ID_Pos             (0U)
#define CBTL08GP053_REVISION_REV_ID_Msk             (0xFFU << CBTL08GP053_REVISION_REV_ID_Pos)           /*!< 0xFF */
#define CBTL08GP053_REVISION_REV_ID                  CBTL08GP053_REVISION_REV_ID_Msk                      /*!< */

/** @defgroup CBTL08GP053_DisplayPort_Alternate_Mode 
  * @{
  */
#define CBTL08GP053_DP_ALTMODE_DFP_D_C_E_NORMAL           0x00U
#define CBTL08GP053_DP_ALTMODE_DFP_D_C_E_FLIPPED          0x01U
#define CBTL08GP053_DP_ALTMODE_DFP_D_D_F_NORMAL           0x02U
#define CBTL08GP053_DP_ALTMODE_DFP_D_D_F_FLIPPED          0x03U
#define CBTL08GP053_DP_ALTMODE_UFP_D_C_NORMAL             0x04U
#define CBTL08GP053_DP_ALTMODE_UFP_D_C_FLIPPED            0x05U
#define CBTL08GP053_DP_ALTMODE_UFP_D_D_NORMAL             0x06U
#define CBTL08GP053_DP_ALTMODE_UFP_D_D_FLIPPED            0x07U
#define CBTL08GP053_DP_ALTMODE_UFP_D_E_NORMAL             0x08U
#define CBTL08GP053_DP_ALTMODE_UFP_D_E_FLIPPED            0x09U
/**
  * @}
  */

/** @defgroup CBTL08GP053_DisplayPort_Standby_Mode 
  * @{
  */
#define CBTL08GP053_DP_STANDBYMODE_SAFE               0x00U
#define CBTL08GP053_DP_STANDBYMODE_USB3_NORMAL        0x01U
#define CBTL08GP053_DP_STANDBYMODE_USB3_FLIPPED       0x02U

/**
  * @}
  */
   
/**
  * @}
  */
 
#if defined(CBTL08GP053_DEBUG)
/** @defgroup CBTL08GP053_Exported_Structure CBTL08GP053 Exported Structure
  * @{
  */

typedef union {
  uint8_t Register;
  struct {
    uint8_t RESERVED  :7;
    uint8_t SWITCH_EN :1;
  };
} CBTL08GP053_SysCtrlTypeDef;

typedef union {
  uint8_t Register;
  struct {
    uint8_t IP1       :1;
    uint8_t IP2       :1;
    uint8_t IP3       :1;
    uint8_t RESERVED  :5;
  };
} CBTL08GP053_Op1CtrlTypeDef;

typedef union {
  uint8_t Register;
  struct {
    uint8_t IP1       :1;
    uint8_t IP2       :1;
    uint8_t IP3       :1;
    uint8_t RESERVED  :5;
  };
} CBTL08GP053_Op2CtrlTypeDef;

typedef union {
  uint8_t Register;
  struct {
    uint8_t RESERVED1 :3;
    uint8_t IP4       :1;
    uint8_t IP5       :1;
    uint8_t IP6       :1;
    uint8_t RESERVED2 :2;
  };
} CBTL08GP053_Op3CtrlTypeDef;

typedef union {
  uint8_t Register;
  struct {
    uint8_t RESERVED1 :3;
    uint8_t IP4       :1;
    uint8_t IP5       :1;
    uint8_t IP6       :1;
    uint8_t RESERVED2 :2;
  };
} CBTL08GP053_Op4CtrlTypeDef;

typedef union {
  uint8_t Register;
  struct {
    uint8_t RESERVED  :6;
    uint8_t IP7       :1;
    uint8_t IP8       :1;
  };
} CBTL08GP053_Op5CtrlTypeDef;

typedef union {
  uint8_t Register;
  struct {
    uint8_t PASS      :1;
    uint8_t CROSS     :1;
    uint8_t RESERVED  :6;
  };
} CBTL08GP053_Cross5CtrlTypeDef;

typedef union {
  uint8_t Register;
  struct {
    uint8_t OP1_SET   :1;
    uint8_t OP2_SET   :1;
    uint8_t OP3_SET   :1;
    uint8_t OP4_SET   :1;
    uint8_t OP5_SET   :1;
    uint8_t X5_SET    :1;
    uint8_t RESERVED  :2;
  };
} CBTL08GP053_SwCtrlTypeDef;

typedef struct
{
  CBTL08GP053_SysCtrlTypeDef    SysCtrl;    /*!< SYS_CTRL register (0x01)     */
  CBTL08GP053_Op1CtrlTypeDef    Op1Ctrl;    /*!< OP1_CTRL Registers (0x02)    */
  CBTL08GP053_Op2CtrlTypeDef    Op2Ctrl;    /*!< OP2_CTRL Registers (0x03)    */
  CBTL08GP053_Op3CtrlTypeDef    Op3Ctrl;    /*!< OP3_CTRL Registers (0x04)    */
  CBTL08GP053_Op4CtrlTypeDef    Op4Ctrl;    /*!< OP4_CTRL Registers (0x05)    */
  CBTL08GP053_Op5CtrlTypeDef    Op5Ctrl;    /*!< OP5_CTRL Registers (0x06)    */
  CBTL08GP053_Cross5CtrlTypeDef Cross5Ctrl; /*!< CROSS5_CTRL Registers (0x07) */
  CBTL08GP053_SwCtrlTypeDef     SwCtrl;     /*!< SW_CTRL Registers (0x08)     */
  uint8_t                       Revision;   /*!< REVISION Registers (0x09)    */
} CBTL08GP053_RegistersTypeDef;
/**
  * @}
  */
#endif /* CBTL08GP053_DEBUG */

/** @defgroup CBTL08GP053_Exported_Functions
  * @{
  */
/* USB Type-C Crosspoint Switch management functions */
uint32_t  cbtl08gp053_Init(uint16_t Address);
void      cbtl08gp053_DeInit(uint16_t Address); 
uint32_t  cbtl08gp053_PowerOn(uint16_t Address);
uint32_t  cbtl08gp053_PowerOff(uint16_t Address);
uint32_t  cbtl08gp053_SetMode(uint16_t Address, TYPECSWITCH_Mode_t Mode);
uint32_t  cbtl08gp053_IsSupportedMode(TYPECSWITCH_Mode_t Mode);

/* MUX IO functions */
uint8_t   MUX_IO_Init(void);
void      MUX_IO_DeInit(void);
uint8_t   MUX_IO_Write(uint16_t Addr, uint16_t Reg, uint8_t Data);
uint8_t   MUX_IO_Read(uint16_t Addr, uint16_t Reg, uint8_t *pData);
uint32_t  MUX_IO_IsDeviceReady(uint16_t DevAddress, uint32_t Trials);

/**
  * @}
  */

/** @defgroup CBTL08GP053_Exported_Variables
  * @{
  */
/* Type-C Crosspoint Switch Driver */
extern TYPECSWITCH_Drv_t cbtl08gp053_drv;
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

#ifdef __cplusplus
 }
#endif
  
#endif /* __CBTL08GP053_H */


/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/ 
