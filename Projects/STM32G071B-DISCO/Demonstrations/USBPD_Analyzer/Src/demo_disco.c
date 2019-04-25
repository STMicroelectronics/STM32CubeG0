/**
  ******************************************************************************
  * @file    demo_disco.c
  * @author  MCD Application Team
  * @brief   demo application code
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
#include "usbpd_trace.h"
#include "usbpd_dpm_core.h"
#include "usbpd_dpm_conf.h"
#include "usbpd_pwr_if.h"
#include "stm32g071b_discovery.h"
#include "stm32g071b_discovery_lcd.h"
#include "stm32g071b_discovery_pwrmon.h"
#include "usbpd_pwr_if.h"
#include "stm32g0xx_ll_ucpd.h"
#include "usbpd_hw.h"
#include "demo_disco.h"
#include "string.h"
#include "stdio.h"
#include "cmsis_os.h"


/* Exported variables --------------------------------------------------------*/
/* Private typedef -----------------------------------------------------------*/
typedef enum {
  DETACHED,
  ATTACHED,
} DEMO_STATE;


typedef struct {
  uint8_t SOPx;
  uint8_t size;
  uint8_t data[30];
} USBPD_Msg;

/** @defgroup USBPD_MsgHeaderStructure_definition USB PD Message header Structure definition
  * @brief USB PD Message header Structure definition
  * @{
  */
typedef union
{
  uint16_t d16;
  struct
  {
uint16_t MessageType :                  /*!< Message Header's message Type                      */
    5;
    uint16_t PortDataRole :                 /*!< Message Header's Port Data Role                    */
    1;
    uint16_t SpecificationRevision :        /*!< Message Header's Spec Revision                     */
    2;
    uint16_t PortPowerRole_CablePlug :      /*!< Message Header's Port Power Role/Cable Plug field  */
    1;
    uint16_t MessageID :                    /*!< Message Header's message ID                        */
    3;
    uint16_t NumberOfDataObjects :          /*!< Message Header's Number of data object             */
    3;
    uint16_t Extended :                     /*!< Reserved                                           */
    1;
  }
  b;
} DEMO_MsgHeader_TypeDef;

/* Private define ------------------------------------------------------------*/
#define STR_SIZE_MAX 18

#define LCD_ALARMBOX_MESSAGES_MAX         30u

/* Format of the different kind of message */
/*   - 31-28  4 bit for the message type   */
#define DEMO_MSG_TYPE_POS                 28u
#define DEMO_MSG_TYPE_MSK                 (0xFu << DEMO_MSG_TYPE_POS)
#define DEMO_MSG_MMI                      (0u   << DEMO_MSG_TYPE_POS)
#define DEMO_MSG_CAD                      (1u   << DEMO_MSG_TYPE_POS)
#define DEMO_MSG_PENOTIFY                 (2u   << DEMO_MSG_TYPE_POS)
#define DEMO_MSG_GETINFO                  (3u   << DEMO_MSG_TYPE_POS)

/*  CAD
     - 27-26 2 bit for CC Pin
     - 25-24 2 bit port num
     - 15-00 16 bit CAD event
*/
#define DEMO_CAD_CC_NUM_Pos               (26u)
#define DEMO_CAD_CC_NUM_Msk               (0x3u << DEMO_CAD_CC_NUM_Pos)
#define DEMO_CAD_CC_NUM_NONE              (0u   << DEMO_CAD_CC_NUM_Pos)
#define DEMO_CAD_CC_NUM_CC1               (1u   << DEMO_CAD_CC_NUM_Pos)
#define DEMO_CAD_CC_NUM_CC2               (2u   << DEMO_CAD_CC_NUM_Pos)

#define DEMO_CAD_PORT_NUM_Pos             (24u)
#define DEMO_CAD_PORT_NUM_Msk             (0x3u << DEMO_CAD_PORT_NUM_Pos)
#define DEMO_CAD_PORT_NUM_0               (0x0u << DEMO_CAD_PORT_NUM_Pos)
#define DEMO_CAD_PORT_NUM_1               (0x1u << DEMO_CAD_PORT_NUM_Pos)

#define DEMO_CAD_STATE_Pos                (0u)
#define DEMO_CAD_STATE_Msk                (0xFFu << DEMO_CAD_STATE_Pos)

/*  PE
     - 27-26 2 bit none
     - 25-24 2 bit port num
     - 15-00 16 bit PE event
*/
#define DEMO_MSG_PORT_NUM_Pos             (24u)
#define DEMO_MSG_PORT_NUM_Msk             (0x3u << DEMO_MSG_PORT_NUM_Pos)
#define DEMO_MSG_PORT_NUM_0               (0x0u << DEMO_MSG_PORT_NUM_Pos)
#define DEMO_MSG_PORT_NUM_1               (0x1u << DEMO_MSG_PORT_NUM_Pos)

#define DEMO_MSG_DATA_Pos                (0u)
#define DEMO_MSG_DATA_Msk                (0xFFu << DEMO_MSG_DATA_Pos)

/* GET INFO Message */
#define DEMO_MSG_GETINFO_SNKCAPA         (0x1u << DEMO_MSG_DATA_Pos)
#define DEMO_MSG_GETINFO_SVID            (0x2u << DEMO_MSG_DATA_Pos)

#define MAX_LINE_PDO      4u
#define MAX_LINE_COMMAND  4u
#define MAX_LINE_EXTCAPA  3u


typedef enum {
  MENU_STLOGO,
  MENU_MAIN,
  MENU_COMMAND,
  MENU_START,
  MENU_PD_SPEC,
  MENU_POWER,
  MENU_DUAL_ROLE_POWER,
  MENU_DATA_ROLE_SWAP,
  MENU_UNCHUNKED_MODE,
  MENU_DISPLAY_PORT,
  MENU_MEASURE,
  MENU_SELECT_SOURCECAPA,
  MENU_SINKCAPA_RECEIVED,
  MENU_EXTCAPA_RECEIVED,
  MENU_SPY_MESSAGE,
  MENU_VERSION
} DEMO_MENU;

typedef enum {
  COMMAND_HARDRESET,
  COMMAND_CONTROLMSG_GET_SRC_CAP,
  COMMAND_CONTROLMSG_GET_SNK_CAP,
  COMMAND_CONTROLMSG_DR_SWAP,
  COMMAND_CONTROLMSG_SOFT_RESET,
  COMMAND_REQUEST_VDM_DISCOVERY,
//  COMMAND_CONTROLMSG_GET_SRC_CAPEXT,
  COMMAND_MAX,
} DEMO_COMMAND;


typedef struct
{
  uint8_t commandid;
  uint8_t commandstr[23];
} typedef_COMMANDE;

/* Private macro -------------------------------------------------------------*/
#define DEMO_MAX_COMMAND  12u

/* Private variables ---------------------------------------------------------*/
uint8_t DP_info_received ;
static uint8_t indexAPDO = 0;
static DEMO_MENU next_menu;
static uint8_t pe_disabled = 0; /* 0 by default, 1 if PE is disabled (in case of no PD device attached) */

/*
 * Definition of control message
 */
const int8_t _tab_control_msg[][18] = {
  "",
  "GOODCRC",          /* 0x01 */
  "GOTOMIN",          /* 0x02 */
  "ACCEPT",           /* 0x03 */
  "REJECT",           /* 0x04 */
  "PING",             /* 0x05 */
  "PS_RDY",           /* 0x06 */
  "GET_SRC_CAP",      /* 0x07 */
  "GET_SNK_CAP",      /* 0x08 */
  "DR_SWAP",          /* 0x09 */
  "PR_SWAP",          /* 0x0A */
  "VCONN_SWAP",       /* 0x0B */
  "WAIT",             /* 0x0C */
  "SOFT_RESET",       /* 0x0D */
  "CTRL_UNKNOW1",     /* 0x0E */
  "CTRL_UNKNOW2",     /* 0x0F */
  "NOT_SUPPORTED",    /* 0x10 */
  "GET_SRC_CAPEXT",   /* 0x11 */
  "GET_STATUS",       /* 0x12 */
  "GET_PPS_STATUS",   /* 0x13 */
  "GET_COUNTRY_CODES" /* 0x14 */
};

/*
 * Definition of data msg
 */
const int8_t _tab_data_msg[][19] = {
  "",
  "SRC_CAPABILITIES", /* 0x01 */
  "REQUEST",          /* 0x02 */
  "BIST",             /* 0x03 */
  "SNK_CAPABILITIES", /* 0x04 */
  "BATTERY_STATUS",   /* 0x05 */
  "ALERT",            /* 0x06 */
  "GET_COUNTRY_CODE", /* 0x07 */
  "DATA_UNKNOW1",     /* 0x08 */
  "DATA_UNKNOW2",     /* 0x09 */
  "DATA_UNKNOW3",     /* 0x0A */
  "DATA_UNKNOW4",     /* 0x0B */
  "DATA_UNKNOW5",     /* 0x0C */
  "DATA_UNKNOW6",     /* 0x0D */
  "DATA_UNKNOW7",     /* 0x0E */
  "VENDOR_DEFINED"    /* 0x0F */
};

const int8_t _tab_extdata_msg[][19] = {
  "E_NONE",//                  = USBPD_EXTENDED_MESSAGE,
  "E_SRC_CAPABILITIES",//   = (USBPD_EXTENDED_MESSAGE | 0x01),  /*!< sent by Source or Dual-Role Power    - SOP only  */
  "E_STATUS",//                = (USBPD_EXTENDED_MESSAGE | 0x02),  /*!< sent by Source                       - SOP only  */
  "E_GET_BAT_CAP",//       = (USBPD_EXTENDED_MESSAGE | 0x03),  /*!< sent by Source or Sink               - SOP only  */
  "E_GET_BAT_STATUS",//    = (USBPD_EXTENDED_MESSAGE | 0x04),  /*!< sent by Source or Sink               - SOP only  */
  "E_BAT_CAPABILITIES",//  = (USBPD_EXTENDED_MESSAGE | 0x05),  /*!< sent by Source or Sink               - SOP only  */
  "E_GET_MANU_INFO",// = (USBPD_EXTENDED_MESSAGE | 0x06),  /*!< sent by Source or Sink or Cable Plug - SOP*      */
  "E_MANU_INFO",//     = (USBPD_EXTENDED_MESSAGE | 0x07),  /*!< sent by Source or Sink or Cable Plug - SOP*      */
  "E_SEC_REQUEST",//      = (USBPD_EXTENDED_MESSAGE | 0x08),  /*!< sent by Source or Sink               - SOP*      */
  "E_SEC_RESPONSE",//     = (USBPD_EXTENDED_MESSAGE | 0x09),  /*!< sent by Source or Sink or Cable Plug - SOP*      */
  "E_FIRMUP_REQUEST",//   = (USBPD_EXTENDED_MESSAGE | 0x0A),  /*!< sent by Source or Sink               - SOP*      */
  "E_FIRMUP_RESPONSE",//  = (USBPD_EXTENDED_MESSAGE | 0x0B),  /*!< sent by Source or Sink or Cable Plug - SOP*      */
  "E_PPS_STATUS",//            = (USBPD_EXTENDED_MESSAGE | 0x0C),  /*!< sent by Source                       - SOP only  */
  "E_COUNTRY_INFO",//          = (USBPD_EXTENDED_MESSAGE | 0x0D),  /*!< sent by Source or Sink or Cable Plug - SOP*      */
  "E_COUNTRY_CODES",//         = (USBPD_EXTENDED_MESSAGE | 0x0E),  /*!< sent by Source or Sink or Cable Plug - SOP*      */
  "E_UNKNOW1",
};



const typedef_COMMANDE g_tab_command_global[COMMAND_MAX] = {
  { COMMAND_HARDRESET                   ,"HARD RESET        " },
  { COMMAND_CONTROLMSG_GET_SRC_CAP      ,"GET SRC CAPA      " },
  { COMMAND_CONTROLMSG_GET_SNK_CAP      ,"GET SNK CAPA      " },
  { COMMAND_CONTROLMSG_DR_SWAP          ,"DATA ROLE SWAP    " },
  { COMMAND_CONTROLMSG_SOFT_RESET       ,"SOFT RESET        " },
  { COMMAND_REQUEST_VDM_DISCOVERY       ,"REQUEST VDM DISCO " }
//  { COMMAND_CONTROLMSG_GET_SRC_CAPEXT   ,"GET SRCE EXT CAPA " }
};

/*  contains the id of the selected item inside a menu*/
static uint8_t   g_tab_menu_sel = { 0 };
/*  contains the menu selected position  */
static  int8_t   g_tab_menu_pos = { 0 };

/* demo mode */
static DEMO_MODE hmode;

/* Timer handler */
TimerHandle_t xTimers;


/* Private variables ---------------------------------------------------------*/
#define MAX_MSG_SIZE   4
#define MAX_MSG_LENGHT 18

int8_t tab_msg[MAX_MSG_SIZE][MAX_MSG_LENGHT];
uint8_t index_msg = 0;

/* Vbus measure value */
uint32_t _vbusVoltage = 0;
int32_t _vbusCurrent = 0;

/* cc state */
CCxPin_TypeDef _cc = CCNONE;

/* Counter for Sel Joystick pressed*/
osMessageQId  DemoEvent, DemoMsgBox;

/* timer for demo display */
uint32_t demo_timeout;
static uint8_t st_logo=0;

/* Private function prototypes -----------------------------------------------*/

static void Display_clear(void);
static void string_completion(uint8_t *str);
static const int8_t *Decode_HEADER(uint16_t Header);

static void Display_sinkcapa_menu(void);
static void Display_sinkcapa_menu_nav(int8_t Or);

static void Display_sourcecapa_menu_nav(int8_t Or);
static uint8_t Display_sourcecapa_menu_exec(void);

static void Display_reset_spymsg(void);
static void Display_add_spymsg(USBPD_Msg Message);
static void Display_menu_spy(void);
static void Display_menu_version(void);

static void Display_dual_role_power_menu(void);
static void Display_unchunkedmode_menu(void);
static void Display_display_port_menu(void);
static void Display_data_role_swap_menu(void);
static void Display_extcapa_menu_nav(int8_t Or);

static void Display_command_menu(void);
static void Display_command_menu_nav(int8_t Or);
static uint8_t Display_command_menu_exec(void);

static CCxPin_TypeDef Check_cc_attachement(uint32_t CC1Voltage, uint32_t CC2Voltage);
void Initialize_USBPD_Spy(void);
void Intialize_RX_processing(CCxPin_TypeDef cc);

void DEMO_Task_SPY(void const *);
void DEMO_Task_Standalone(void const *);
void SPY_TRACE_TX_Task(void const *argument);
static void DEMO_PostGetInfoMessage(uint8_t PortNum, uint16_t GetInfoVal);

void vTimerCallback( TimerHandle_t xTimer );

uint8_t DEMO_IsSpyMode(void)
{
  if(MODE_SPY == hmode) return 1;
  return 0;
}

/**
  * @brief  Demo BSP initialisation
  * @retval DEMO_ErrorCode
  */
DEMO_ErrorCode DEMO_InitBSP(void)
{
  BSP_JOY_Init(JOY_MODE_EXTI);

  /*##-1- Initialize the LCD #################################################*/
  /* Initialize the LCD */
  BSP_LCD_Init();
  BSP_LCD_SetFont(&Font12);

  /*##-1- Initialize the power #################################################*/
  /* Initialize the LCD */
  PWRMON_Config_t DefaultConfig =
  {
    .ShuntConvertTime  = CONVERT_TIME_1100,
    .BusConvertTime    = CONVERT_TIME_1100,
    .AveragingMode     = AVERAGING_MODE_1,
  };
  
  BSP_PWRMON_Init(ALERT_VBUS, &DefaultConfig);
  BSP_PWRMON_StartMeasure(ALERT_VBUS, OPERATING_MODE_CONTINUOUS);

  BSP_PWRMON_Init(ALERT_CC1, &DefaultConfig);
  BSP_PWRMON_StartMeasure(ALERT_CC1, OPERATING_MODE_CONTINUOUS);

  BSP_PWRMON_Init(ALERT_CC2, &DefaultConfig);
  BSP_PWRMON_StartMeasure(ALERT_CC2, OPERATING_MODE_CONTINUOUS);

 return DEMO_OK;
}

DEMO_ErrorCode DEMO_InitTask(DEMO_MODE mode)
{
  osThreadDef(SPY, DEMO_Task_SPY, osPriorityNormal, 0, 280);
  osThreadDef(STD, DEMO_Task_Standalone, osPriorityAboveNormal, 0, 280);

  osMessageQDef(DemoEvent, 30, uint32_t);
  DemoEvent = osMessageCreate(osMessageQ(DemoEvent), NULL);

  osMessageQDef(DemoMsgBox,48, USBPD_Msg);
  DemoMsgBox = osMessageCreate(osMessageQ(DemoMsgBox), NULL);

  if (MODE_SPY == mode)
  {
    Initialize_USBPD_Spy();
    Intialize_RX_processing(CC1);

    if( NULL == osThreadCreate(osThread(SPY), NULL))
    {
      while(1);
    }
    osThreadDef(TRA_TX, SPY_TRACE_TX_Task, osPriorityLow, 0, configMINIMAL_STACK_SIZE * 2);
    osThreadCreate(osThread(TRA_TX), NULL);
  }
  else
  {
    osThreadCreate(osThread(STD), &DemoEvent);
  }

  hmode = mode;

  return DEMO_OK;
}


/*
 * Decode header info to string
 */
const int8_t *Decode_HEADER(uint16_t Header)
{
  uint16_t _msgtype = Header & 0x1F;

  if (Header & 0x8000) {
    return _tab_extdata_msg[_msgtype];
  }
  else
  {
    // check data message
    if (Header & 0x7000) {
      return _tab_data_msg[_msgtype];
    }

    // Control message
    return _tab_control_msg[_msgtype];
  }
}


/**
  * @brief  UserCableDetection reporting events on a specified port from CAD layer.
  * @param  PortNum   The handle of the port
  * @param  State     CAD state
  * @param  Cc        The Communication Channel for the USBPD communication
  * @retval None
  */
void DEMO_PostCADMessage(uint8_t PortNum, USBPD_CAD_EVENT State, CCxPin_TypeDef Cc)
{
  uint32_t event = (DEMO_MSG_CAD |  (Cc << DEMO_CAD_CC_NUM_Pos | (PortNum << DEMO_CAD_PORT_NUM_Pos) | State ));
  osMessagePut(DemoEvent, event, 0);
}

/**
  * @brief  Callback function called by PE to inform DPM about PE event.
  * @param  PortNum   The handle of the port
  * @param  EventVal @ref USBPD_NotifyEventValue_TypeDef
  * @retval None
  */
void DEMO_PostNotificationMessage(uint8_t PortNum, USBPD_NotifyEventValue_TypeDef EventVal)
{
  uint32_t event = DEMO_MSG_PENOTIFY | PortNum << DEMO_MSG_PORT_NUM_Pos | EventVal;
  osMessagePut(DemoEvent, event, 0);
}

/**
  * @brief  function called by MMI to order a PE request.
  * @param  PortNum   The handle of the port
  * @param  GetInfoVal
  * @retval None
  */
static void DEMO_PostGetInfoMessage(uint8_t PortNum, uint16_t GetInfoVal)
{
  uint32_t event = DEMO_MSG_GETINFO | PortNum << DEMO_MSG_PORT_NUM_Pos | GetInfoVal;
  osMessagePut(DemoEvent, event, 0);
}

/**
  * @brief  Callback function called by PE to inform DPM about PE event.
  * @param  EventVal @ref USBPD_NotifyEventValue_TypeDef
  * @retval None
  */
void DEMO_PostMMIMessage(uint32_t EventVal)
{
  uint32_t event = DEMO_MSG_MMI | EventVal;
  osMessagePut(DemoEvent, event, 0);
}

void HAL_GPIO_EXTI_Rising_Callback(uint16_t GPIO_Pin)
{
  if (GPIO_Pin == LEFT_JOY_PIN)
  {
    DEMO_PostMMIMessage(DEMO_MMI_ACTION_LEFT_PRESS);
  }
  if (GPIO_Pin == RIGHT_JOY_PIN)
  {
    DEMO_PostMMIMessage(DEMO_MMI_ACTION_RIGHT_PRESS);
  }
  if (GPIO_Pin == DOWN_JOY_PIN)
  {
    DEMO_PostMMIMessage(DEMO_MMI_ACTION_DOWN_PRESS);
  }
  if (GPIO_Pin == UP_JOY_PIN)
  {
    DEMO_PostMMIMessage(DEMO_MMI_ACTION_UP_PRESS);
  }
  if (GPIO_Pin == SEL_JOY_PIN)
  {
    DEMO_PostMMIMessage(DEMO_MMI_ACTION_SEL_PRESS);
  }
}

/* stockage de l'historique des donnee de power */
uint32_t   power_data[127];
uint8_t    power_index = 0;

/**
  * @brief  check_cc_attached
  * @param  CC1Voltage   CC1 voltage
  * @param  CC2Voltage   CC2 voltage
  * @retval CC1 CC2 or CCNONE
  */
char str[40];
static CCxPin_TypeDef Check_cc_attachement(uint32_t CC1Voltage, uint32_t CC2Voltage)
{
  CCxPin_TypeDef val = CCNONE;
  static uint8_t index = 0;
  index++;

  if ((CC2Voltage > 350) && (CC2Voltage < 2500)) {val = CC2;}
  if (MODE_SPY == hmode) /* if we are in spy mode, the offset on CC1 is different */
  { 
    if ((CC1Voltage > 650) && (CC1Voltage < 2500)) {val = CC1;}
  }
  else
  {
    if ((CC1Voltage > 350) && (CC1Voltage < 2500)) {val = CC1;}
  }

  return val;
}

/**
  * @brief  Display clear info
  * @retval None
  */
static void Display_clear(void)
{
  BSP_LCD_Clear(LCD_COLOR_BLACK);
}

/**
  * @brief  Display clear info
  * @retval None
  */
static void Display_stlogo(void)
{
  if (st_logo < 10)
  {
    st_logo ++;
  }
  else if (st_logo ==10)
  {
    st_logo ++;
    BSP_LCD_SetFont(&Font16);
    BSP_LCD_DisplayStringAt(0,1*16, (uint8_t*)"STM32G0",CENTER_MODE);
    BSP_LCD_DisplayStringAt(0,2*16, (uint8_t*)"Disco Kit",CENTER_MODE);
    if (MODE_SPY == hmode)
      BSP_LCD_DisplayStringAt(0,3*16, (uint8_t*)"SPY",CENTER_MODE);
    else
      BSP_LCD_DisplayStringAt(0,3*16, (uint8_t*)"SINK",CENTER_MODE);
  }
  
}

/**
  * @brief  Menu selection managment
  * @param  IndexMax    MAX index
  * @param  LineMax     MAX line
  * @param  Start       Pointer on Start
  * @param  End         Pointer on end
  * @param  Orientation Orientation
  * @retval None
  */
static void Menu_manage_selection(uint8_t IndexMax, uint8_t LineMax, uint8_t *Start, uint8_t *End, int8_t Orientation)
{
  *Start = 0;
  *End   = *Start + LineMax;

  if(( Orientation == 0 ) || (IndexMax == 0))
  {
    *Start = 0;
    *End =(LineMax > IndexMax)? IndexMax: LineMax;
    g_tab_menu_pos = g_tab_menu_sel = 0;
    return;
  }

  if( -1 == Orientation)
  {
    if(0 == g_tab_menu_sel)
    {
      /* got at the end of the selection */
      g_tab_menu_pos = LineMax > IndexMax? IndexMax-1: LineMax-1;
      g_tab_menu_sel = IndexMax-1;
      *End = IndexMax;
      if( IndexMax > LineMax ) {
        *Start = IndexMax - LineMax;
      } else {
        *Start = 0;
      }
    }
    else
    {
      if(0 == g_tab_menu_pos)
      {
        g_tab_menu_sel--;
        *Start =  g_tab_menu_sel;
        if(IndexMax > LineMax )
          *End = *Start + LineMax;
        else
          *End = *Start + IndexMax;
      }
      else
      {
      *Start = g_tab_menu_sel - g_tab_menu_pos;
      if(IndexMax > LineMax )
        *End   = g_tab_menu_sel + LineMax - g_tab_menu_pos;
      else
        *End   = g_tab_menu_sel + IndexMax - g_tab_menu_pos;

      g_tab_menu_pos--;
      g_tab_menu_sel--;
      }
    }
  }
  else  /* +1 == orientation */
  {
    if(((LineMax-1) == g_tab_menu_pos) || (((IndexMax-1) == g_tab_menu_pos) && (LineMax >= IndexMax)))
    {      /* at the end of selection, so two case there is others data or at the end of the data */
      if(g_tab_menu_sel == (IndexMax-1))
      {
        /* restart at the begin of the list */
        g_tab_menu_pos = 0;
        g_tab_menu_sel = 0;
        *Start = 0;
        *End   = LineMax > IndexMax? IndexMax : LineMax;
      }
      else
      {
        *Start = g_tab_menu_sel - (LineMax-1) + 1;
        *End = *Start+LineMax;
        g_tab_menu_sel++;
      }
    }
    else
    {
      *Start = g_tab_menu_sel - g_tab_menu_pos;
      if(IndexMax > LineMax)
        *End   = *Start + LineMax;
      else
        *End   = *Start + IndexMax;
      g_tab_menu_sel++;
      g_tab_menu_pos++;
    }
  }
}

/**
  * @brief  get next menu
  * @param  MenuId
  * @retval menuid
  */
static DEMO_MENU Menu_manage_next(uint8_t MenuId)
{
  if (MODE_SPY == hmode)
  {
    if(_cc == CCNONE)                                   
    {
      BSP_PWRMON_GetVoltage(ALERT_VBUS, &_vbusVoltage);
      if(_vbusVoltage < 1500)
      {
        return MENU_VERSION; /* only allowed change when nothing plugged : version */
      }
    }
    if(MENU_STLOGO == MenuId)                           return MENU_PD_SPEC;
    if(MENU_START == MenuId)                            return MENU_PD_SPEC;
    if((MENU_PD_SPEC == MenuId)&& (0 == DPM_Ports[0].DPM_NumberOfRcvSRCPDO))      return MENU_MEASURE; // No PD config
    if((MENU_PD_SPEC == MenuId)&& (DPM_Ports[0].DPM_NumberOfRcvSRCPDO >0))        return MENU_POWER;   // PD CONFIG
    if(MENU_POWER == MenuId)                            return MENU_DUAL_ROLE_POWER;
    if(MENU_DUAL_ROLE_POWER == MenuId)                  return MENU_UNCHUNKED_MODE;
    if(MENU_UNCHUNKED_MODE == MenuId)                   return MENU_DATA_ROLE_SWAP;
    if(MENU_DATA_ROLE_SWAP == MenuId)                   return MENU_SELECT_SOURCECAPA;
    if(MENU_SELECT_SOURCECAPA == MenuId)                return MENU_MEASURE;
    if(MENU_MEASURE == MenuId)                          return MENU_VERSION;
    if(MENU_VERSION == MenuId)                          return MENU_VERSION;
  }
  else /* Standalone mode */
  {
    if(MENU_START == MenuId)
    {
      BSP_PWRMON_GetVoltage(ALERT_VBUS, &_vbusVoltage);
      if(_vbusVoltage < 1500)
      {
        return MENU_VERSION; /* only allowed change when nothing plugged : version */
      }
      else
      {      
        return MENU_PD_SPEC;
      }
    }
    if(MENU_START == MenuId)                                            return MENU_PD_SPEC;
    if((MENU_PD_SPEC == MenuId)&& (DPM_Ports[0].DPM_NumberOfRcvSRCPDO >0)) return MENU_POWER;   // PD CONFIG
    if((MENU_PD_SPEC == MenuId)&& (0 == DPM_Ports[0].DPM_NumberOfRcvSRCPDO)) return MENU_MEASURE; // No PD config
    if(MENU_POWER == MenuId)                                            return MENU_DUAL_ROLE_POWER;
    if(MENU_DUAL_ROLE_POWER == MenuId)                                  return MENU_UNCHUNKED_MODE;
    if(MENU_UNCHUNKED_MODE == MenuId)                                   return MENU_DATA_ROLE_SWAP;
    if(MENU_DATA_ROLE_SWAP == MenuId)                                   return MENU_DISPLAY_PORT;
    if(MENU_DISPLAY_PORT == MenuId)                                     return MENU_MEASURE;
    if((MENU_MEASURE == MenuId)&& (DPM_Ports[0].DPM_NumberOfRcvSRCPDO >0)) return MENU_SELECT_SOURCECAPA;// PD CONFIG
    if((MENU_MEASURE == MenuId)&& (0 == DPM_Ports[0].DPM_NumberOfRcvSRCPDO)) return MENU_VERSION;          // no PD CONFIG
    if(MENU_SELECT_SOURCECAPA == MenuId)                                return MENU_SINKCAPA_RECEIVED;
    if(MENU_SINKCAPA_RECEIVED == MenuId)                                return MENU_COMMAND;
    if(MENU_COMMAND == MenuId)                                          return MENU_VERSION;
    if(MENU_VERSION == MenuId)                                          return MENU_VERSION;
  }

  return MENU_VERSION;
}

/**
  * @brief  get prev menu
  * @param  MenuId
  * @retval menuid
  */
static DEMO_MENU Menu_manage_prev(uint8_t MenuId)
{
  if (MODE_SPY == hmode )
  {
    if(_cc == CCNONE)      
    {
      BSP_PWRMON_GetVoltage(ALERT_VBUS, &_vbusVoltage);
      if(_vbusVoltage < 1500)
      {
        st_logo=10;
        return MENU_STLOGO; /* only allowed change when nothing plugged : version */
      }
    } /* only allowed change when nothing plugged : start menu */
    if(MENU_START == MenuId)                            return MENU_START;
    if(MENU_PD_SPEC == MenuId)                          return MENU_START;
    if(MENU_POWER == MenuId)                            return MENU_PD_SPEC;
    if(MENU_DUAL_ROLE_POWER == MenuId)                  return MENU_POWER;
    if(MENU_UNCHUNKED_MODE == MenuId)                   return MENU_DUAL_ROLE_POWER;
    if(MENU_DATA_ROLE_SWAP == MenuId)                   return MENU_UNCHUNKED_MODE;
    if(MENU_SELECT_SOURCECAPA == MenuId)                return MENU_DATA_ROLE_SWAP;
    if((MENU_MEASURE == MenuId) && (DPM_Ports[0].DPM_NumberOfRcvSRCPDO >0))       return MENU_SELECT_SOURCECAPA; // if PD
    if((MENU_MEASURE == MenuId) && (0 == DPM_Ports[0].DPM_NumberOfRcvSRCPDO))     return MENU_PD_SPEC;           // If not PD device
    if(MENU_VERSION == MenuId)                          return MENU_MEASURE;
  }
  else  /* Standalone mode */
  {
    if(MENU_START == MenuId)                            return MENU_START;
    if(MENU_PD_SPEC == MenuId)                          return MENU_START;
    if(MENU_POWER == MenuId)                            return MENU_PD_SPEC;
    if(MENU_DUAL_ROLE_POWER == MenuId)                  return MENU_POWER;
    if(MENU_UNCHUNKED_MODE == MenuId)                   return MENU_DUAL_ROLE_POWER;
    if(MENU_DATA_ROLE_SWAP == MenuId)                   return MENU_UNCHUNKED_MODE;
    if(MENU_DISPLAY_PORT == MenuId)                     return MENU_DATA_ROLE_SWAP;
    if((MENU_MEASURE == MenuId) && (0 == DPM_Ports[0].DPM_NumberOfRcvSRCPDO))  return MENU_PD_SPEC;      // If not PD device
    if((MENU_MEASURE == MenuId) && (DPM_Ports[0].DPM_NumberOfRcvSRCPDO>0))     return MENU_DISPLAY_PORT; // If PD device
    if(MENU_SELECT_SOURCECAPA == MenuId)                return MENU_MEASURE;
    if(MENU_SINKCAPA_RECEIVED == MenuId)                return MENU_SELECT_SOURCECAPA;
    if(MENU_COMMAND == MenuId)                          return MENU_SINKCAPA_RECEIVED;
    BSP_PWRMON_GetVoltage(ALERT_VBUS, &_vbusVoltage);
    if(_vbusVoltage < 1500) /* if nothing is connected */
    {
      if(MENU_VERSION == MenuId) {st_logo=10;  return MENU_STLOGO;}
    }
    else
    {
      if((MENU_VERSION == MenuId) && (0 == DPM_Ports[0].DPM_NumberOfRcvSRCPDO))     return MENU_MEASURE; // If not PD device
    }
    if((MENU_VERSION == MenuId) && (DPM_Ports[0].DPM_NumberOfRcvSRCPDO>0))     return MENU_COMMAND; // If PD device
  }
  st_logo=10;
  return MENU_STLOGO;
}

/**
  * @brief  snk capa menu display
  * @retval None
  */
static void Display_sinkcapa_menu(void)
{
  uint8_t _str[19];

  /* Display menu sink capa */
  sprintf((char*)_str, "Distant Sink capa:");
  BSP_LCD_DisplayStringAtLine(0, _str);
  if ( DPM_Ports[0].DPM_ListOfRcvSRCPDO[0] & USBPD_PDO_SNK_FIXED_DRP_SUPPORT_Msk) /* if DRP */
  {
    Display_sinkcapa_menu_nav(0);
  }
  else /* not DRP */
  {
    sprintf((char*)_str, "Connected to");
    BSP_LCD_DisplayStringAtLine(2 ,_str);
    sprintf((char*)_str, "source only");
    BSP_LCD_DisplayStringAtLine(3 ,_str);
    sprintf((char*)_str, "No sink capa");
    BSP_LCD_DisplayStringAtLine(4 ,_str);
  }
}

/**
  * @brief  snk capa menu navigation
  * @param  Nav
  * @retval None
  */
static void Display_sinkcapa_menu_nav(int8_t Nav)
{
  uint8_t _str[25];
  uint8_t _max = DPM_Ports[0].DPM_NumberOfRcvSNKPDO;
  uint8_t _start, _end, _pos = 0;

  if (0 == DPM_Ports[0].DPM_NumberOfRcvSNKPDO)
  {
    sprintf((char*)_str, "No sink capa");
    BSP_LCD_DisplayStringAtLine(2 ,_str);
  }
  else
  {
    if (hmode == MODE_STANDALONE)
    {
      Menu_manage_selection(_max, MAX_LINE_PDO, &_start, &_end, Nav);
    }
    else
    {
      if((Nav == 1) && (_max > MAX_LINE_PDO))
      {
        _start = _max - MAX_LINE_PDO;
        _end = _max;
      }
      else
      {
        _start = 0;
        _end = MIN(_max, MAX_LINE_PDO);
      }
    }
    
    for(int8_t index=_start; index < _end; index++)
    {
      switch(DPM_Ports[0].DPM_ListOfRcvSNKPDO[index] & USBPD_PDO_TYPE_Msk)
      {
      case USBPD_PDO_TYPE_FIXED :
        {
          uint32_t maxcurrent = ((DPM_Ports[0].DPM_ListOfRcvSNKPDO[index] & USBPD_PDO_SRC_FIXED_MAX_CURRENT_Msk) >> USBPD_PDO_SRC_FIXED_MAX_CURRENT_Pos)*10;
          uint32_t maxvoltage = ((DPM_Ports[0].DPM_ListOfRcvSNKPDO[index] & USBPD_PDO_SRC_FIXED_VOLTAGE_Msk) >> USBPD_PDO_SRC_FIXED_VOLTAGE_Pos)*50;
          sprintf((char*)_str, "FIXED:%2dV %2d.%dA ",(int)(maxvoltage/1000), (int)(maxcurrent/1000), (int)((maxcurrent % 1000)/100));
        }
        break;

      case USBPD_PDO_TYPE_BATTERY :
        {
          uint32_t maxvoltage = ((DPM_Ports[0].DPM_ListOfRcvSNKPDO[index] & USBPD_PDO_SRC_BATTERY_MAX_VOLTAGE_Msk) >> USBPD_PDO_SRC_BATTERY_MAX_VOLTAGE_Pos) * 50;
          uint32_t minvoltage = ((DPM_Ports[0].DPM_ListOfRcvSNKPDO[index] & USBPD_PDO_SRC_BATTERY_MIN_VOLTAGE_Msk) >> USBPD_PDO_SRC_BATTERY_MIN_VOLTAGE_Pos) * 50;
          uint32_t maxpower = ((DPM_Ports[0].DPM_ListOfRcvSNKPDO[index] & USBPD_PDO_SRC_BATTERY_MAX_POWER_Msk) >> USBPD_PDO_SRC_BATTERY_MAX_POWER_Pos) * 250;
          if (maxpower==100000)  /* 100W */
          {
            sprintf((char*)_str, "B:%2d.%1d-%2d.%1dV %2dW",(int) (minvoltage/1000),(int)(minvoltage/100)%10, (int)(maxvoltage/1000),(int)(maxvoltage/100)%10, (int)(maxpower/1000));
          }
          else
          {
            sprintf((char*)_str, "B:%2d.%1d-%2d.%1dV %2d.%dW",(int) (minvoltage/1000),(int)(minvoltage/100)%10,(int)(maxvoltage/1000),(int)(maxvoltage/100)%10, (int)(maxpower/1000), (int)(maxpower/100)%10);
          }
        }
        break;
      case USBPD_PDO_TYPE_VARIABLE :
        {
          uint32_t maxvoltage = ((DPM_Ports[0].DPM_ListOfRcvSNKPDO[index] & USBPD_PDO_SRC_VARIABLE_MAX_VOLTAGE_Msk) >> USBPD_PDO_SRC_VARIABLE_MAX_VOLTAGE_Pos) * 50;
          uint32_t minvoltage = ((DPM_Ports[0].DPM_ListOfRcvSNKPDO[index] & USBPD_PDO_SRC_VARIABLE_MIN_VOLTAGE_Msk) >> USBPD_PDO_SRC_VARIABLE_MIN_VOLTAGE_Pos) * 50;
          uint32_t maxcurrent = ((DPM_Ports[0].DPM_ListOfRcvSNKPDO[index] & USBPD_PDO_SRC_VARIABLE_MAX_CURRENT_Msk) >> USBPD_PDO_SRC_VARIABLE_MAX_CURRENT_Pos) * 10;
          sprintf((char*)_str, "V:%2d.%1d-%2d.%1dV %2d.%1dA ", (int)(minvoltage/1000),(int)(minvoltage/100)%10,(int) (maxvoltage/1000),(int)(maxvoltage/100)%10, (int)(maxcurrent/1000),(int)(maxcurrent/100)%10);
        }
        break;
      case USBPD_PDO_TYPE_APDO :
        {
          uint32_t minvoltage = ((DPM_Ports[0].DPM_ListOfRcvSNKPDO[index] & USBPD_PDO_SRC_APDO_MIN_VOLTAGE_Msk) >> USBPD_PDO_SRC_APDO_MIN_VOLTAGE_Pos) * 100;
          uint32_t maxvoltage = ((DPM_Ports[0].DPM_ListOfRcvSNKPDO[index] & USBPD_PDO_SRC_APDO_MAX_VOLTAGE_Msk) >> USBPD_PDO_SRC_APDO_MAX_VOLTAGE_Pos) * 100;
          uint32_t maxcurrent = ((DPM_Ports[0].DPM_ListOfRcvSNKPDO[index] & USBPD_PDO_SRC_APDO_MAX_CURRENT_Msk) >> USBPD_PDO_SRC_APDO_MAX_CURRENT_Pos) * 50;
          sprintf((char*)_str, "A:%2d.%1d-%2d.%1dV %2d.%dA ", (int)(minvoltage/1000),(int)(minvoltage/100)%10, (int)(maxvoltage/1000),(int)(maxvoltage/100)%10, (int)(maxcurrent/1000), (int)((maxcurrent / 100)%10));
        }
        break;
      default :
        sprintf((char*)_str,"Unknown Sink PDO type");
        break;
      }

      if (hmode == MODE_STANDALONE)
      {
        if((index - _start) == g_tab_menu_pos)
        {
          BSP_LCD_SetBackColor(LCD_COLOR_WHITE);
          BSP_LCD_SetTextColor(LCD_COLOR_BLACK);
        }
      }
      string_completion(_str);
      BSP_LCD_DisplayStringAtLine(1 + _pos++, (uint8_t*)_str);

      if (hmode == MODE_STANDALONE)
      {
        if((index - _start) == g_tab_menu_pos)
        {
          BSP_LCD_SetBackColor(LCD_COLOR_BLACK);
          BSP_LCD_SetTextColor(LCD_COLOR_WHITE);
        }
      }
    }

  }
}

/**
  * @brief  src capa menu display
  * @retval None
  */
static void Display_sourcecapa_menu(void)
{
  uint8_t _str[20];

  /* Display menu source capa */
  if (hmode == MODE_STANDALONE)
  {
    sprintf((char *)_str, "Select the profile:");
    string_completion(_str);
    BSP_LCD_DisplayStringAtLine(0, _str);
  }
  else
  {
    sprintf((char *)_str, "Source profile(s)");
    string_completion(_str);
    BSP_LCD_DisplayStringAtLine(0, _str);
  }

  _str[0] = '\0';

  if(DPM_Ports[0].DPM_NumberOfRcvSRCPDO != 0)
  {
    if( DPM_Ports[0].DPM_ListOfRcvSRCPDO[0] & USBPD_PDO_SRC_FIXED_UNCHUNK_SUPPORTED)     sprintf((char *)_str, "%s UNCK", (char *)_str);
    if( DPM_Ports[0].DPM_ListOfRcvSRCPDO[0] & USBPD_PDO_SRC_FIXED_DRD_SUPPORT_Msk)       sprintf((char *)_str, "%s DRD", (char *)_str);
    if( DPM_Ports[0].DPM_ListOfRcvSRCPDO[0] & USBPD_PDO_SRC_FIXED_USBCOMM_NOT_SUPPORTED) sprintf((char *)_str, "%s USB", (char *)_str);
    if( DPM_Ports[0].DPM_ListOfRcvSRCPDO[0] & USBPD_PDO_SRC_FIXED_DRP_SUPPORT_Msk)       sprintf((char *)_str, "%s DRP", (char *)_str);
  }

  if(_str[0] == ' ')
  {
    memcpy(&_str[0], &_str[1], STR_SIZE_MAX);
  }

  string_completion(_str);
  BSP_LCD_DisplayStringAtLine(1, _str);

  Display_sourcecapa_menu_nav(0);
}

/**
  * @brief  src capa menu navigation
  * @param  Nav
  * @retval None
  */
static void Display_sourcecapa_menu_nav(int8_t Nav)
{
  uint8_t _str[30];
  uint8_t _max = DPM_Ports[0].DPM_NumberOfRcvSRCPDO;
  uint8_t _start, _end, _pos = 0;

  if (hmode == MODE_STANDALONE)
  {
    Menu_manage_selection(_max, MAX_LINE_PDO, &_start, &_end, Nav);
  }
  else
  {
    if((Nav == 1) && (_max > MAX_LINE_PDO))
    {
      _start = _max - MAX_LINE_PDO;
      _end = _max;
    }
    else
    {
      _start = 0;
      _end = MIN(_max, MAX_LINE_PDO);
    }
  }


  for(int8_t index=_start; index < _end; index++)
  {
    switch(DPM_Ports[0].DPM_ListOfRcvSRCPDO[index] & USBPD_PDO_TYPE_Msk)
    {
    case USBPD_PDO_TYPE_FIXED :
      {
        uint32_t maxcurrent = ((DPM_Ports[0].DPM_ListOfRcvSRCPDO[index] & USBPD_PDO_SRC_FIXED_MAX_CURRENT_Msk) >> USBPD_PDO_SRC_FIXED_MAX_CURRENT_Pos)*10;
        uint32_t maxvoltage = ((DPM_Ports[0].DPM_ListOfRcvSRCPDO[index] & USBPD_PDO_SRC_FIXED_VOLTAGE_Msk) >> USBPD_PDO_SRC_FIXED_VOLTAGE_Pos)*50;
        sprintf((char*)_str, "FIXED:%2dV %2d.%dA", (int)(maxvoltage/1000), (int)(maxcurrent/1000), (int)((maxcurrent % 1000) /100));
        break;
      }
    case USBPD_PDO_TYPE_BATTERY :
      {
        uint32_t maxvoltage = ((DPM_Ports[0].DPM_ListOfRcvSRCPDO[index] & USBPD_PDO_SRC_BATTERY_MAX_VOLTAGE_Msk) >> USBPD_PDO_SRC_BATTERY_MAX_VOLTAGE_Pos) * 50;
        uint32_t minvoltage = ((DPM_Ports[0].DPM_ListOfRcvSRCPDO[index] & USBPD_PDO_SRC_BATTERY_MIN_VOLTAGE_Msk) >> USBPD_PDO_SRC_BATTERY_MIN_VOLTAGE_Pos) * 50;
        uint32_t maxpower = ((DPM_Ports[0].DPM_ListOfRcvSRCPDO[index] & USBPD_PDO_SRC_BATTERY_MAX_POWER_Msk) >> USBPD_PDO_SRC_BATTERY_MAX_POWER_Pos) * 250;
        if ((maxpower)==100000) /* 100W */
        {
          sprintf((char*)_str, "B:%2d.%1d-%2d.%1dV %2dW",(int)(minvoltage/1000),(int)(minvoltage/100)%10, (int)(maxvoltage/1000),(int)(maxvoltage/100)%10, (int)(maxpower/1000));
        }
        else
        {
          sprintf((char*)_str, "B:%2d.%1d-%2d.%1dV %2d.%dW", (int)(minvoltage/1000),(int)(minvoltage/100)%10, (int)(maxvoltage/1000),(int)(maxvoltage/100)%10, (int)(maxpower/1000), (int)(maxpower/100)%10);
        }
      }
      break;
    case USBPD_PDO_TYPE_VARIABLE :
      {
        uint32_t maxvoltage = ((DPM_Ports[0].DPM_ListOfRcvSRCPDO[index] & USBPD_PDO_SRC_VARIABLE_MAX_VOLTAGE_Msk) >> USBPD_PDO_SRC_VARIABLE_MAX_VOLTAGE_Pos) * 50;
        uint32_t minvoltage = ((DPM_Ports[0].DPM_ListOfRcvSRCPDO[index] & USBPD_PDO_SRC_VARIABLE_MIN_VOLTAGE_Msk) >> USBPD_PDO_SRC_VARIABLE_MIN_VOLTAGE_Pos) * 50;
        uint32_t maxcurrent = ((DPM_Ports[0].DPM_ListOfRcvSRCPDO[index] & USBPD_PDO_SRC_VARIABLE_MAX_CURRENT_Msk) >> USBPD_PDO_SRC_VARIABLE_MAX_CURRENT_Pos) * 10;
        sprintf((char*)_str, "V:%2d.%1d-%2d.%1dV %d.%dA", (int)(minvoltage/1000),(int)(minvoltage/100)%10, (int)(maxvoltage/1000),(int)(maxvoltage/100)%10, (int)(maxcurrent/1000), (int)((maxcurrent % 1000) /100));
      }
      break;
    case USBPD_PDO_TYPE_APDO :
      {
        uint32_t minvoltage = ((DPM_Ports[0].DPM_ListOfRcvSRCPDO[index] & USBPD_PDO_SRC_APDO_MIN_VOLTAGE_Msk) >> USBPD_PDO_SRC_APDO_MIN_VOLTAGE_Pos) * 100;
        uint32_t maxvoltage = ((DPM_Ports[0].DPM_ListOfRcvSRCPDO[index] & USBPD_PDO_SRC_APDO_MAX_VOLTAGE_Msk) >> USBPD_PDO_SRC_APDO_MAX_VOLTAGE_Pos) * 100;
        uint32_t maxcurrent = ((DPM_Ports[0].DPM_ListOfRcvSRCPDO[index] & USBPD_PDO_SRC_APDO_MAX_CURRENT_Msk) >> USBPD_PDO_SRC_APDO_MAX_CURRENT_Pos) * 50;
        sprintf((char*)_str, "A:%2d.%1d-%2d.%1dV %d.%dA",(int) (minvoltage/1000),(int)(minvoltage/100)%10, (int)(maxvoltage/1000),(int)(maxvoltage/100)%10, (int)(maxcurrent/1000), (int)((maxcurrent % 1000) /100));
      }
      break;
    default :
      sprintf((char*)_str,"Unknown Source PDO");
      break;
    }

    if (hmode == MODE_STANDALONE)
    {
      if((index - _start) == g_tab_menu_pos)
      {
        BSP_LCD_SetBackColor(LCD_COLOR_WHITE);
        BSP_LCD_SetTextColor(LCD_COLOR_BLACK);
      }
    }
    string_completion(_str);
    BSP_LCD_DisplayStringAtLine(1 + _pos++, (uint8_t*)_str);

    if (hmode == MODE_STANDALONE)
    {
      if((index - _start) == g_tab_menu_pos)
      {
        BSP_LCD_SetBackColor(LCD_COLOR_BLACK);
        BSP_LCD_SetTextColor(LCD_COLOR_WHITE);
      }
    }
  }
}

/**
  * @brief  ext capa menu display
  * @retval None
  */
static void Display_extcapa_menu(void)
{
  uint8_t str[30];

  /* Display menu source capa */
  sprintf((char *)str, "Extended capa :   ");
  BSP_LCD_DisplayStringAtLine(1, str);

  Display_extcapa_menu_nav(0);
}

/**
  * @brief  ext capa menu navigation
  * @param  Orientation
  * @retval None
  */
static void Display_extcapa_menu_nav(int8_t Orientation)
{
  uint8_t _str[16][19];
  uint8_t _max = 16;  /* Nb field inside extended capa */
  uint8_t _start, _end;
  uint8_t _position = 0;

  memset(_str, 0, sizeof(_str));

  sprintf((char *)_str[0], "VID:0x%x",      DPM_Ports[0].DPM_RcvSRCExtendedCapa.VID);
  sprintf((char *)_str[1], "PID:0x%x",      DPM_Ports[0].DPM_RcvSRCExtendedCapa.PID);
  sprintf((char *)_str[2], "XID:0x%lx",     DPM_Ports[0].DPM_RcvSRCExtendedCapa.XID);
  sprintf((char *)_str[3], "F rev:0x%x",    DPM_Ports[0].DPM_RcvSRCExtendedCapa.FW_revision);
  sprintf((char *)_str[4], "H rev:0x%x",    DPM_Ports[0].DPM_RcvSRCExtendedCapa.HW_revision);
  sprintf((char *)_str[5], "V reg:%d",      DPM_Ports[0].DPM_RcvSRCExtendedCapa.Voltage_regulation);
  sprintf((char *)_str[6], "Htime:%d",      DPM_Ports[0].DPM_RcvSRCExtendedCapa.Holdup_time);
  sprintf((char *)_str[7], "Compliance:%d", DPM_Ports[0].DPM_RcvSRCExtendedCapa.Compliance);
  sprintf((char *)_str[8], "Tcurre:%d",     DPM_Ports[0].DPM_RcvSRCExtendedCapa.TouchCurrent);
  sprintf((char *)_str[9], "Peak1:%d",      DPM_Ports[0].DPM_RcvSRCExtendedCapa.PeakCurrent1);
  sprintf((char *)_str[10],"Peak2:%d",      DPM_Ports[0].DPM_RcvSRCExtendedCapa.PeakCurrent2);
  sprintf((char *)_str[11],"Peak3:%d" ,     DPM_Ports[0].DPM_RcvSRCExtendedCapa.PeakCurrent3);
  sprintf((char *)_str[12],"Ttemp:%d",      DPM_Ports[0].DPM_RcvSRCExtendedCapa.Touchtemp);
  sprintf((char *)_str[13],"SRCin:%d",      DPM_Ports[0].DPM_RcvSRCExtendedCapa.Source_inputs);
  sprintf((char *)_str[14],"Nbbatt:%d",     DPM_Ports[0].DPM_RcvSRCExtendedCapa.NbBatteries);
  sprintf((char *)_str[15],"PDP:%d",        DPM_Ports[0].DPM_RcvSRCExtendedCapa.SourcePDP);

  Menu_manage_selection(_max, MAX_LINE_EXTCAPA, &_start, &_end, Orientation);

  for(int8_t index=_start; index < _end; index++)
  {
    if((index - _start) == g_tab_menu_pos)
    {
      BSP_LCD_SetBackColor(LCD_COLOR_WHITE);
      BSP_LCD_SetTextColor(LCD_COLOR_BLACK);
    }

    string_completion(_str[index]);
    BSP_LCD_DisplayStringAtLine(2 +  _position++, (uint8_t*)_str[index]);

    if((index - _start) == g_tab_menu_pos)
    {
      BSP_LCD_SetBackColor(LCD_COLOR_BLACK);
      BSP_LCD_SetTextColor(LCD_COLOR_WHITE);
    }
  }
}


void Display_reset_spymsg()
{
  memset(tab_msg, 0, sizeof(tab_msg));
  DPM_Ports[0].DPM_NumberOfRcvSNKPDO = 0;
  DPM_Ports[0].DPM_NumberOfRcvSRCPDO = 0;
  memset((uint8_t *)&DPM_Ports[0].DPM_RcvSRCExtendedCapa, 0, sizeof(DPM_Ports[0].DPM_RcvSRCExtendedCapa));
}


void Display_add_spymsg(USBPD_Msg Message)
{
  DEMO_MsgHeader_TypeDef header;
  header.d16 = *(uint16_t *)Message.data;

#define CTRL_GOODCRC   (char *)_tab_control_msg[1]
#define DATA_SRCCAPA   (char *)_tab_data_msg[1]
#define DATA_SNKCAPA   (char *)_tab_data_msg[4]
#define EXT_SRCCAPA    (char *)_tab_extdata_msg[1]

  if(strcmp((char *)Decode_HEADER(header.d16),CTRL_GOODCRC) != 0)
  {
    strncpy((char *)tab_msg[index_msg], (char *)Decode_HEADER(header.d16), MAX_MSG_LENGHT);

    /* get SNK capa */
    if(strcmp((char *)tab_msg[index_msg],DATA_SNKCAPA) == 0)
    {
      /* Save the SNK capa into DPM */
      DPM_Ports[0].DPM_NumberOfRcvSNKPDO = header.b.NumberOfDataObjects;
      memcpy(DPM_Ports[0].DPM_ListOfRcvSNKPDO, &Message.data[2], header.b.NumberOfDataObjects * 4);
    }

    /* get SRC capa send, but store only the data sent by the source */
    if((strcmp((char *)tab_msg[index_msg],DATA_SRCCAPA) == 0) && (header.b.PortPowerRole_CablePlug == 1))
    {
      /* Save the SRC capa into DPM */
      DPM_Ports[0].DPM_NumberOfRcvSRCPDO = header.b.NumberOfDataObjects;
      memcpy(DPM_Ports[0].DPM_ListOfRcvSRCPDO, &Message.data[2], header.b.NumberOfDataObjects * 4);
      DPM_Params[0].PE_SpecRevision = header.b.SpecificationRevision;
    }

    /* get EXT capa */
    if((strcmp((char *)tab_msg[index_msg], EXT_SRCCAPA) == 0) && (header.b.PortPowerRole_CablePlug == 1))
    {
      /* Save the EXT SRC capa into DPM */
      memcpy((uint8_t*)&DPM_Ports[0].DPM_RcvSRCExtendedCapa, &Message.data[4], 24);
    }

    index_msg++;
    if(index_msg == MAX_MSG_SIZE) index_msg = 0;
  }
}

/**
  * @brief  command menu display
  * @retval None
  */
static void Display_command_menu(void)
{
  /* Menu command display */
  BSP_LCD_SetFont(&Font12);

  /* Display menu command */
  BSP_LCD_DisplayStringAtLine(0, (uint8_t*)"Select a command: ");

  Display_command_menu_nav(0);
}

static void Display_start_menu(void)
{
  BSP_LCD_SetFont(&Font16);
  if(MODE_SPY == hmode)
  {
    BSP_LCD_DisplayStringAt(0,1*16, (uint8_t*)"Spy mode",CENTER_MODE);
    BSP_LCD_DisplayStringAt(0,2*16, (uint8_t*)"Two devices",CENTER_MODE);
    BSP_LCD_DisplayStringAt(0,3*16, (uint8_t*)"connected",CENTER_MODE);
  }
  else
  {
    BSP_LCD_DisplayStringAt(0,1*16, (uint8_t*)"Attached to",CENTER_MODE);
    if((DPM_Ports[0].DPM_NumberOfRcvSRCPDO != 0) && ( DPM_Ports[0].DPM_ListOfRcvSRCPDO[0] & USBPD_PDO_SNK_FIXED_DRP_SUPPORT_Msk))
    {
      BSP_LCD_DisplayStringAt(0,2*16, (uint8_t*)"a Dual Role",CENTER_MODE);
      BSP_LCD_SetFont(&Font12);
      BSP_LCD_DisplayStringAt(0,3*16, (uint8_t*)"Power Source",CENTER_MODE);
    }
    else
    {
      BSP_LCD_DisplayStringAt(0,2*16, (uint8_t*)"a Power",CENTER_MODE);
      BSP_LCD_DisplayStringAt(0,3*16, (uint8_t*)"Source",CENTER_MODE);
    }
  }
}

static void Display_pd_spec_menu(void)
{
  char str[12];

  /* Menu command display */
  BSP_LCD_SetFont(&Font16);

  /* Display menu command */
  BSP_PWRMON_GetVoltage(ALERT_VBUS, &_vbusVoltage);
  if ((hmode != MODE_SPY) && (_vbusVoltage >1500) && (DPM_Ports[0].DPM_NumberOfRcvSRCPDO == 0)) /* VBUS is there, but no source capa received yet */
  {
    if (pe_disabled == 1) /* i.e. if we received notification that the attached device is not a PD device */
    {
      BSP_LCD_DisplayStringAt(0,1*20, (uint8_t*)"USBPD not",CENTER_MODE);
      BSP_LCD_DisplayStringAt(0,2*20, (uint8_t*)"supported",CENTER_MODE);
    } 
    else /* in case we are trying to communicate to attached device : some HARD RESET may be on going */
    {
      BSP_LCD_DisplayStringAt(0,1*20, (uint8_t*)"Analysis",CENTER_MODE);
      BSP_LCD_DisplayStringAt(0,2*20, (uint8_t*)"on going",CENTER_MODE);
    }
  }
  else if(DPM_Ports[0].DPM_NumberOfRcvSRCPDO >0)
  {
    BSP_LCD_DisplayStringAt(0,1*20,(uint8_t*)"USB Type-C",CENTER_MODE);
    sprintf((char *)str, "PD%s capable", DPM_Params[0].PE_SpecRevision == USBPD_SPECIFICATION_REV3? "3" : "2");
    BSP_LCD_DisplayStringAt(0,2*20, (uint8_t *)str,CENTER_MODE);
  }
  else
    {
      BSP_LCD_DisplayStringAt(0,1*20, (uint8_t*)"USBPD not",CENTER_MODE);
      BSP_LCD_DisplayStringAt(0,2*20, (uint8_t*)"supported",CENTER_MODE);
    } 
}

static void Display_power_menu(void)
{
  char str[20];
  uint32_t maxvoltage, maxcurrent, maxpower;
  maxvoltage =0;
  maxcurrent =0;
  maxpower = 0;
  uint8_t _max = DPM_Ports[0].DPM_NumberOfRcvSRCPDO;

  /* Menu command display */
  BSP_LCD_SetFont(&Font16);

  /* Display menu command */
  sprintf((char *)str, "%d power",_max);
  BSP_LCD_DisplayStringAt(0,0*16+10, (uint8_t *)str, CENTER_MODE);
  if (_max <= 1)
  {
    BSP_LCD_DisplayStringAt(0,1*16+10, (uint8_t*)"profile", CENTER_MODE);
  }
  else
  {
    BSP_LCD_DisplayStringAt(0,1*16+10, (uint8_t*)"profiles", CENTER_MODE);
  }

for(int8_t index=0; index < _max; index++)
  {
    uint32_t _max_power_temp = 0U;
    switch(DPM_Ports[0].DPM_ListOfRcvSRCPDO[index] & USBPD_PDO_TYPE_Msk)
    {
    case USBPD_PDO_TYPE_FIXED :
      {
      maxcurrent = ((DPM_Ports[0].DPM_ListOfRcvSRCPDO[index] & USBPD_PDO_SRC_FIXED_MAX_CURRENT_Msk) >> USBPD_PDO_SRC_FIXED_MAX_CURRENT_Pos)*10;
      maxvoltage = ((DPM_Ports[0].DPM_ListOfRcvSRCPDO[index] & USBPD_PDO_SRC_FIXED_VOLTAGE_Msk) >> USBPD_PDO_SRC_FIXED_VOLTAGE_Pos)*50;
      _max_power_temp = maxcurrent * maxvoltage;
      }
      break;
    case USBPD_PDO_TYPE_BATTERY :
      {
        _max_power_temp = ((DPM_Ports[0].DPM_ListOfRcvSRCPDO[index] & USBPD_PDO_SRC_BATTERY_MAX_POWER_Msk) >> USBPD_PDO_SRC_BATTERY_MAX_POWER_Pos) * 250;
      }
      break;
    case USBPD_PDO_TYPE_VARIABLE :
      {
        maxvoltage = ((DPM_Ports[0].DPM_ListOfRcvSRCPDO[index] & USBPD_PDO_SRC_VARIABLE_MAX_VOLTAGE_Msk) >> USBPD_PDO_SRC_VARIABLE_MAX_VOLTAGE_Pos) * 50;
        maxcurrent = ((DPM_Ports[0].DPM_ListOfRcvSRCPDO[index] & USBPD_PDO_SRC_VARIABLE_MAX_CURRENT_Msk) >> USBPD_PDO_SRC_VARIABLE_MAX_CURRENT_Pos) * 10;
        _max_power_temp = maxvoltage * maxcurrent;
      }
      break;
    case USBPD_PDO_TYPE_APDO :
      {
        maxvoltage = ((DPM_Ports[0].DPM_ListOfRcvSRCPDO[index] & USBPD_PDO_SRC_APDO_MAX_VOLTAGE_Msk) >> USBPD_PDO_SRC_APDO_MAX_VOLTAGE_Pos) * 100;
        maxcurrent = ((DPM_Ports[0].DPM_ListOfRcvSRCPDO[index] & USBPD_PDO_SRC_APDO_MAX_CURRENT_Msk) >> USBPD_PDO_SRC_APDO_MAX_CURRENT_Pos) * 50;
        _max_power_temp = maxvoltage * maxcurrent;
      }
      break;
    default :
      break;
    }
    if (_max_power_temp > maxpower)
    {
      maxpower = _max_power_temp;
    }
  }

  sprintf((char *)str, "max %2d.%01d W",(int)(maxpower/1000000),(int)(maxpower%1000000)/100000);
  BSP_LCD_DisplayStringAt(0,2*16+10, (uint8_t *)str, CENTER_MODE);

}

static void Display_measure_menu(void)
{
  char str[20];

  BSP_LCD_SetFont(&Font20);

  BSP_PWRMON_GetVoltage(ALERT_VBUS, &_vbusVoltage);
  BSP_PWRMON_GetCurrent(ALERT_VBUS, &_vbusCurrent);

  if (_vbusCurrent <0)
  {
    _vbusCurrent = -_vbusCurrent;
  }

/* Display measure voltage, current and power */
  sprintf((char *)str, "%2d.%03d V", (int)_vbusVoltage/1000, (int)_vbusVoltage%1000);
  BSP_LCD_DisplayStringAt(0,0*16+10, (uint8_t *)str, CENTER_MODE);

  sprintf((char *)str, "%2d.%03d A",(int)_vbusCurrent/1000,(int) _vbusCurrent%1000);
  BSP_LCD_DisplayStringAt(0,1*16+10, (uint8_t *)str, CENTER_MODE);

  sprintf((char *)str, "%2d.%03d W",(int)(_vbusVoltage * _vbusCurrent)/ 1000000,(int)((_vbusVoltage * _vbusCurrent)%1000000)/1000);
  BSP_LCD_DisplayStringAt(0,2*16+10, (uint8_t *)str, CENTER_MODE);

}

/**
  * @brief  Display dual role power capabilities
  * @retval None
 **/
static void Display_dual_role_power_menu()
{
  uint8_t _str[20];

  BSP_LCD_SetFont(&Font16);
  sprintf((char *)_str, "Dual Role");
  BSP_LCD_DisplayStringAt(0,0*16+10, _str, CENTER_MODE);
  sprintf((char *)_str, "Power (DRP)");
  BSP_LCD_DisplayStringAt(0,1*16+10, _str, CENTER_MODE);
  BSP_LCD_SetFont(&Font16);

  /* Display the dual role power capabilities */
  if((DPM_Ports[0].DPM_NumberOfRcvSRCPDO != 0) && ( DPM_Ports[0].DPM_ListOfRcvSRCPDO[0] & USBPD_PDO_SNK_FIXED_DRP_SUPPORT_Msk))
  {
    sprintf((char *)_str, ": YES");
  }
  else
  {
    sprintf((char *)_str, ": NO");
  }

  BSP_LCD_DisplayStringAt(0,2*16+10, _str, CENTER_MODE);
}

/**
  * @brief  Display Unchunked Mode capabilities
  * @retval None
 **/
static void Display_unchunkedmode_menu()
{
  uint8_t _str[20];

  BSP_LCD_SetFont(&Font16);
  sprintf((char *)_str, "Unchunked");
  BSP_LCD_DisplayStringAt(0,0*16+10, _str, CENTER_MODE);
  sprintf((char *)_str, "Mode(UNCKD)");
  BSP_LCD_DisplayStringAt(0,1*16+10, _str, CENTER_MODE);

  /* Display the fast role swap capabilities */
  if((DPM_Ports[0].DPM_NumberOfRcvSRCPDO != 0) && ( DPM_Ports[0].DPM_ListOfRcvSRCPDO[0] & USBPD_PDO_SRC_FIXED_UNCHUNK_SUPPORTED))
  {
    sprintf((char *)_str, ": YES");
  }
  else
  {
    sprintf((char *)_str, ": NO");
  }

  BSP_LCD_DisplayStringAt(0,2*16+10, _str, CENTER_MODE);
}

/**
  * @brief  Display data role swap capabilities
  * @retval None
 **/
static void Display_data_role_swap_menu()
{
  uint8_t _str[20];
  uint8_t line_offset=0;
  
  if(MODE_SPY == hmode)
  {
    line_offset = 10;
  }
  
  /* Display the data role swap capabilities */
  if((DPM_Ports[0].DPM_NumberOfRcvSRCPDO != 0) && ( DPM_Ports[0].DPM_ListOfRcvSRCPDO[0] & USBPD_PDO_SNK_FIXED_DRD_SUPPORT_Msk))
  {
    BSP_LCD_SetFont(&Font16);
    sprintf((char *)_str, "Dual Role");
    BSP_LCD_DisplayStringAt(0,0*16+line_offset, _str, CENTER_MODE);
    sprintf((char *)_str, "Data (DRD)");
    BSP_LCD_DisplayStringAt(0,1*16+line_offset, _str, CENTER_MODE);
    sprintf((char *)_str, ": YES");
    BSP_LCD_DisplayStringAt(0,2*16+line_offset, _str, CENTER_MODE);
    
    if(MODE_SPY != hmode) 
    {
      if (DPM_Params[0].PE_DataRole == 0)
      {
        sprintf((char *)_str, "Disco :UFP");
      }
      else
      {
        sprintf((char *)_str, "Disco :DFP");
      }
      BSP_LCD_DisplayStringAt(0,3*16, _str, CENTER_MODE);    
    }
  }
  else /* Data role swap not supported */
  {
    BSP_LCD_SetFont(&Font16);
    sprintf((char *)_str, "Dual Role");
    BSP_LCD_DisplayStringAt(0,0*16+10, _str, CENTER_MODE);
    sprintf((char *)_str, "Data (DRD)");
    BSP_LCD_DisplayStringAt(0,1*16+10, _str, CENTER_MODE);
    sprintf((char *)_str, ": NO");
    BSP_LCD_DisplayStringAt(0,2*16+10, _str, CENTER_MODE);
  }
  
}

/**
  * @brief  Display display port capabilities
  * @retval None
**/
static void Display_display_port_menu()
{
  uint8_t _str[20];                             
  uint8_t DP_supported=0;
  uint8_t Thunderbolt_supported=0;

  /* Display the display port capabilities */
  BSP_LCD_SetFont(&Font16);
  sprintf((char *)_str, "                 ");
  BSP_LCD_DisplayStringAt(0,0*16+10, _str, CENTER_MODE);
  _str[0]=0;
  if ((DP_info_received == 1) && (1 == DPM_Ports[0].VDM_SVIDPortPartner.FlagAllSVIDReceived))
  {
    if (DPM_Ports[0].VDM_SVIDPortPartner.NumSVIDs >0 )
    {
      for (int i=0; i<DPM_Ports[0].VDM_SVIDPortPartner.NumSVIDs; i++)
      {
        if (0xFF01 == DPM_Ports[0].VDM_SVIDPortPartner.SVIDs[i])
        {
          DP_supported =1;
        }
        if (0x8087 == DPM_Ports[0].VDM_SVIDPortPartner.SVIDs[i])
        {
          Thunderbolt_supported =1;
        }
      }
      if (DP_supported + Thunderbolt_supported>1) /* if both thunderbolt and Display Port are supported */
      {
        BSP_LCD_SetFont(&Font12);
        sprintf((char *)_str, "Display port and");       
        BSP_LCD_DisplayStringAt(0,0*16, _str, CENTER_MODE);
        BSP_LCD_SetFont(&Font16);
        sprintf((char *)_str, "Thunderbolt");       
        BSP_LCD_DisplayStringAt(0,1*16, _str, CENTER_MODE);
        sprintf((char *)_str, "Support");
        BSP_LCD_DisplayStringAt(0,2*16, _str, CENTER_MODE);
        sprintf((char *)_str, ": YES");      
        BSP_LCD_DisplayStringAt(0,3*16, _str, CENTER_MODE);
      }
      else  /* if only one is supported : either thunderbolt, or display port */
      {
        BSP_LCD_SetFont(&Font16);
        if (1 == DP_supported){sprintf((char *)_str, "DisplayPort");}
        else if (1== Thunderbolt_supported) {sprintf((char *)_str, "Thunderbolt");} 
        else {sprintf((char *)_str, "Unknown VDM");} 
        BSP_LCD_DisplayStringAt(0,0*16+10, _str, CENTER_MODE);
        sprintf((char *)_str, "Support");
        BSP_LCD_DisplayStringAt(0,1*16+10, _str, CENTER_MODE);
        sprintf((char *)_str, ": YES");      
        BSP_LCD_DisplayStringAt(0,2*16+10, _str, CENTER_MODE);
      }
    }
    else /* in case our request was NAKed */
    {
      BSP_LCD_SetFont(&Font16);
      sprintf((char *)_str, "Display port");
      BSP_LCD_DisplayStringAt(0,0*16+10, _str, CENTER_MODE);
      sprintf((char *)_str, "Support :");
      BSP_LCD_DisplayStringAt(0,1*16+10, _str, CENTER_MODE);
      sprintf((char *)_str, "No info received");
      BSP_LCD_DisplayStringAt(0,2*16+10, _str, CENTER_MODE);
    }
  }
  else
  {
      BSP_LCD_SetFont(&Font12);
      sprintf((char *)_str, "no VDM info");
      BSP_LCD_DisplayStringAt(0,0*16+10, _str, CENTER_MODE);
      sprintf((char *)_str, "available.");
      BSP_LCD_DisplayStringAt(0,1*16+10, _str, CENTER_MODE);
      sprintf((char *)_str, "Run VDM disco");
      BSP_LCD_DisplayStringAt(0,2*16+10, _str, CENTER_MODE);
  }
}

/**
  * @brief  command menu navigation
  * @param  Nav (+1 -1 or 0)
  * @retval None
  */
void Display_command_menu_nav(int8_t Nav)
{
  uint8_t _max = 0, _pos = 0;
  uint8_t _start;
  uint8_t _end;

  _max = COMMAND_MAX;

  Menu_manage_selection(_max, MAX_LINE_COMMAND, &_start, &_end, Nav);

  BSP_LCD_SetFont(&Font12);

  for(int8_t index=_start; index < _end; index++, _pos++)
  {
    if((index - _start) == g_tab_menu_pos)
    {
      BSP_LCD_SetBackColor(LCD_COLOR_WHITE);
      BSP_LCD_SetTextColor(LCD_COLOR_BLACK);
    }

    BSP_LCD_DisplayStringAtLine(1 + _pos,  (uint8_t*)g_tab_command_global[index].commandstr);

    if((index - _start) == g_tab_menu_pos)
    {
      BSP_LCD_SetBackColor(LCD_COLOR_BLACK);
      BSP_LCD_SetTextColor(LCD_COLOR_WHITE);
    }
  }
}

/**
  * @brief  manage menu display
  * @param  MenuSel
  * @retval None
  */
static void Display_menuupdate_info(DEMO_MENU MenuSel)
{
  BSP_LCD_SetFont(&Font12);

  Display_clear();
  switch(MenuSel)
  {
  case MENU_STLOGO :
    Display_stlogo();
    break;
  case MENU_MAIN : /* No Port connected so nothing to display */
    break;
  case MENU_COMMAND : /* Display menu command */
    Display_command_menu();
    break;
  case MENU_START : /* Display menu command */
    Display_start_menu();
    break;
  case MENU_POWER : /* Display menu command */
    Display_power_menu();
    break;
  case MENU_PD_SPEC : /* Display menu command */
    Display_pd_spec_menu();
    break;
  case MENU_DUAL_ROLE_POWER : /* Display menu command */
    Display_dual_role_power_menu();
    break;
  case MENU_UNCHUNKED_MODE : /* Display menu command */
    Display_unchunkedmode_menu();
    break;
  case MENU_DISPLAY_PORT : /* Display menu command */
    Display_display_port_menu();
    break;
  case MENU_DATA_ROLE_SWAP : /* Display menu command */
    Display_data_role_swap_menu();
    break;
  case MENU_MEASURE : /* Display menu command */
    Display_measure_menu();
    break;
  case MENU_SELECT_SOURCECAPA : /* Display menu source capa */
    Display_sourcecapa_menu();
    break;
  case MENU_SINKCAPA_RECEIVED : /* Display menu sink capa */
    Display_sinkcapa_menu();
    break;
  case MENU_EXTCAPA_RECEIVED :
    Display_extcapa_menu();
    break;
  case MENU_SPY_MESSAGE :
    Display_menu_spy();
    break;
  case MENU_VERSION :
    Display_menu_version();
    break;
  }
  BSP_LCD_SetFont(&Font12);
}

/**
  * @brief  command menu execution
  * @retval None
  */
uint8_t Display_command_menu_exec(void)
{
  switch(g_tab_command_global[g_tab_menu_sel].commandid)
  {
  case COMMAND_HARDRESET :
    next_menu = MENU_START;
    return (USBPD_OK != USBPD_PE_Request_HardReset(0));
  case COMMAND_CONTROLMSG_GET_SRC_CAP :
    next_menu = MENU_SELECT_SOURCECAPA;
    return (USBPD_OK != USBPD_PE_Request_CtrlMessage(0, USBPD_CONTROLMSG_GET_SRC_CAP, USBPD_SOPTYPE_SOP));
  case COMMAND_CONTROLMSG_DR_SWAP :
    next_menu = MENU_DATA_ROLE_SWAP;
    return (USBPD_OK != USBPD_PE_Request_CtrlMessage(0, USBPD_CONTROLMSG_DR_SWAP, USBPD_SOPTYPE_SOP));
  case COMMAND_CONTROLMSG_SOFT_RESET :
    next_menu = MENU_START;
    return (USBPD_OK != USBPD_PE_Request_CtrlMessage(0, USBPD_CONTROLMSG_SOFT_RESET, USBPD_SOPTYPE_SOP));
  case COMMAND_CONTROLMSG_GET_SNK_CAP :
    next_menu = MENU_SINKCAPA_RECEIVED;
    return (USBPD_OK != USBPD_PE_Request_CtrlMessage(0, USBPD_CONTROLMSG_GET_SNK_CAP, USBPD_SOPTYPE_SOP));
  case COMMAND_REQUEST_VDM_DISCOVERY :
    next_menu = MENU_DISPLAY_PORT;
    return (USBPD_OK != USBPD_DPM_RequestVDM_DiscoverySVID(0, USBPD_SOPTYPE_SOP));
  default :
    return(1);
  }
}

/**
  * @brief  src capa menu exec
  * @retval None
  */
uint8_t Display_sourcecapa_menu_exec(void)
{
  USBPD_SNKRDO_TypeDef rdo;
  USBPD_PDO_TypeDef  pdo;
  uint32_t voltage, allowablepower, size;
  uint32_t snkpdolist[USBPD_MAX_NB_PDO];
  USBPD_PDO_TypeDef snk_fixed_pdo;

  /* Read SNK PDO list for retrieving useful data to fill in RDO */
  USBPD_PWR_IF_GetPortPDOs(0, USBPD_CORE_DATATYPE_SNK_PDO, (uint8_t*)&snkpdolist[0], &size);
  /* Store value of 1st SNK PDO (Fixed) in local variable */
  snk_fixed_pdo.d32 = snkpdolist[0];

  /* selected SRC PDO */
  pdo.d32 = DPM_Ports[0].DPM_ListOfRcvSRCPDO[g_tab_menu_sel];
  switch(pdo.GenericPDO.PowerObject)
  {
  case USBPD_CORE_PDO_TYPE_APDO :
    voltage = 4500;
    allowablepower = 4500;
    break;
  default:
    voltage = 0;
    allowablepower = 0;
    break;
  }

  /* check if the selected source PDO is matching any of the SNK PDO */
  if (USBPD_TRUE == USBPD_DPM_SNK_EvaluateMatchWithSRCPDO(0, pdo.d32, &voltage, &allowablepower))
  {
    /* consider the current PDO as correct */
  }
  else
  {
    return(1); /* error */
  }

  if (USBPD_CORE_PDO_TYPE_APDO == pdo.GenericPDO.PowerObject)
  {
    rdo.d32 = 0;
    rdo.ProgRDO.CapabilityMismatch = 0;
    rdo.ProgRDO.NoUSBSuspend = 0;
    rdo.ProgRDO.OperatingCurrentIn50mAunits = 1500 /50;
    rdo.ProgRDO.OutputVoltageIn20mV = (4500 + (indexAPDO * 100)) /20;
    indexAPDO++;
    if (indexAPDO > 14) indexAPDO = 0;
    rdo.FixedVariableRDO.ObjectPosition = g_tab_menu_sel + 1;
    USBPD_PE_Send_Request(0, rdo.d32, USBPD_CORE_PDO_TYPE_APDO);
    return(0); /* ok */
  }
  else
  {
    rdo.d32 = 0;
    rdo.FixedVariableRDO.CapabilityMismatch = 0;
    rdo.FixedVariableRDO.GiveBackFlag = 0;
    rdo.FixedVariableRDO.MaxOperatingCurrent10mAunits = ((DPM_Ports[0].DPM_ListOfRcvSRCPDO[g_tab_menu_sel] & USBPD_PDO_SRC_FIXED_MAX_CURRENT_Msk) >> USBPD_PDO_SRC_FIXED_MAX_CURRENT_Pos);
    rdo.FixedVariableRDO.NoUSBSuspend = 0;
    rdo.FixedVariableRDO.ObjectPosition = g_tab_menu_sel + 1;
    rdo.FixedVariableRDO.USBCommunicationsCapable = snk_fixed_pdo.SNKFixedPDO.USBCommunicationsCapable;
#if defined(USBPD_REV30_SUPPORT) && defined(_UNCHUNKED_SUPPORT)
    if (USBPD_SPECIFICATION_REV2 < DPM_Params[0].PE_SpecRevision)
    {
      rdo.FixedVariableRDO.UnchunkedExtendedMessage = DPM_Settings[0].PE_PD3_Support.d.PE_UnchunkSupport;
      fixed_pdo.d32 = DPM_Ports[0].DPM_ListOfRcvSRCPDO[0];
      DPM_Params[0].PE_UnchunkSupport   = USBPD_FALSE;
      /* Set unchuncked bit if supported by port partner;*/
      if (USBPD_TRUE == fixed_pdo.SRCFixedPDO.UnchunkedExtendedMessage)
      {
        DPM_Params[0].PE_UnchunkSupport   = USBPD_TRUE;
      }
    }
#endif /* USBPD_REV30_SUPPORT && _UNCHUNKED_SUPPORT */

    if ((DPM_Ports[0].DPM_ListOfRcvSRCPDO[g_tab_menu_sel] & USBPD_PDO_TYPE_Msk) == USBPD_PDO_TYPE_FIXED)
    {
      if( USBPD_OK == USBPD_DPM_RequestMessageRequest(0, rdo.GenericRDO.ObjectPosition, voltage))
      {
        return(0); /* ok */
      }
      else
      {
        return(1); /* error */
      }
    }
    else
    {
      return(1); /* error */
    }
  }
}

/**
 **/
static void Display_menu_version()
{
  uint8_t str_version[20];

  BSP_LCD_SetFont(&Font16);
  BSP_LCD_DisplayStringAtLine(0, (uint8_t*)"Connect ");
  BSP_LCD_DisplayStringAtLine(1, (uint8_t*)"STM32Cube-");
  BSP_LCD_SetFont(&Font16);
  BSP_LCD_DisplayStringAtLine(2, (uint8_t*)"MonitorUCPD");

  /* Display the version of firmware */
  sprintf((char *)str_version, "w14.5 C");
#if defined(_GUI_INTERFACE)
  if(MODE_SPY != hmode) {
  sprintf((char *)str_version,"%s GUI", str_version);
  }
#endif
#if defined(_TRACE)
  sprintf((char *)str_version,"%s TRACE", str_version);
#endif
  BSP_LCD_SetFont(&Font8);
  BSP_LCD_DisplayStringAtLine(7,  (uint8_t *)str_version);
}

static void Display_menu_spy()
{
  uint8_t start = index_msg;
  uint8_t line = 1;

  BSP_LCD_SetFont(&Font12);

  /* Display on the string the latest msg exchanged */
  for(uint8_t index = 0; index < MAX_MSG_SIZE; index++)
  {
    BSP_LCD_DisplayStringAtLine(line++,(uint8_t*)tab_msg[start]);
    start++;
    if(start == MAX_MSG_SIZE) start = 0;
  }
}

/**
  * @brief  manage menu selection
  * @param  MenuSel
  * @param  Nav (+1, -1, 0)
  * @retval None
  */
static void Display_menunav_info(uint8_t MenuSel, int8_t Nav)
{
  BSP_LCD_SetFont(&Font12);
  switch(MenuSel)
  {
  default :
  case MENU_MAIN : /* No Port connected so nothing to display */
    break;
  case MENU_COMMAND : /* Display menu command */
    Display_command_menu_nav(Nav);
    break;
  case MENU_SELECT_SOURCECAPA : /* Display menu source capa */
    Display_sourcecapa_menu_nav(Nav);
    break;
  case MENU_SINKCAPA_RECEIVED : /* Display menu source capa */
    Display_sinkcapa_menu_nav(Nav);
    break;
  case MENU_EXTCAPA_RECEIVED : /* Display menu source capa */
    Display_extcapa_menu_nav(Nav);
    break;
  }
  BSP_LCD_SetFont(&Font12);
}

/**
  * @brief  manage menu execution
  * @param  MenuSel
  * @retval None
  */
static uint8_t Display_menuexec_info(uint8_t MenuSel)
{
  uint8_t _exec_ok = 0; /* 0 = OK */
  switch(MenuSel)
  {
  default :
  case MENU_MAIN : /* No Port connected so nothing to display */
    _exec_ok = 1; /* no command to be executed : exit error */
    break;
  case MENU_COMMAND : /* Display menu command */
    _exec_ok = Display_command_menu_exec();
    break;
  case MENU_SELECT_SOURCECAPA : /* Display menu source capa */
    _exec_ok = Display_sourcecapa_menu_exec();
    next_menu = MENU_MEASURE;
    break;
  }
  return _exec_ok;
}

/**
  * @brief  main demo function to manage all the appplication event and to update MMI
  * @retval None
  */
uint32_t count_msg_received = 0;
static void DEMO_Manage_spy(void)
{
  static DEMO_STATE _state =  DETACHED;
  uint32_t _cc1Voltage = 0, _cc2Voltage = 0;
  int32_t _vbusCurrent;
  USBPD_Msg _msg;
  uint32_t _mmi;
  static DEMO_MENU _menuSel = MENU_STLOGO;
  static uint16_t _refresh_counter = 0;
#define REFRESH_TIME 200
  _refresh_counter++;
  /* Get voltage on cc line */
  BSP_PWRMON_GetVoltage(ALERT_CC1, &_cc1Voltage);
  BSP_PWRMON_GetVoltage(ALERT_CC2, &_cc2Voltage);

  /* Display VBUS IBUS */
  BSP_PWRMON_GetVoltage(ALERT_VBUS, &_vbusVoltage);
  BSP_PWRMON_GetCurrent(ALERT_VBUS, &_vbusCurrent);

  _cc = Check_cc_attachement(_cc1Voltage, _cc2Voltage);

  if (_refresh_counter == REFRESH_TIME)
  {
    if (GPIO_PIN_RESET == HAL_GPIO_ReadPin(DOOR_SENSE_PORT, DOOR_SENSE_PIN))
    {
      // Disco door has changed position : Reset the board
      HAL_NVIC_SystemReset();
    }
    _refresh_counter = 0;
    if(MENU_MEASURE == _menuSel)
    {
      Display_menuupdate_info(_menuSel);
    }
    if (_state == ATTACHED)
    {
      /* current indication */
      if(_vbusCurrent < -2 )
      {
        BSP_LED_Off(LED4);
        BSP_LED_On(LED5);
      }
      else
      {
        if( _vbusCurrent == 0)
        {
          BSP_LED_Off(LED5);
          BSP_LED_Off(LED4);
        }
        else
        {
          BSP_LED_On(LED4);
          BSP_LED_Off(LED5);
        }
      }
    }
  }
  switch(_state)
  {
  case DETACHED :
    {
      if((_vbusVoltage > 1500) && (_cc != CCNONE))
      {
        _state   = ATTACHED;
        /* Set the USCP to receive message on the right CC line */
        Intialize_RX_processing(_cc);

        char strtmp[100];
        sprintf(strtmp, "VBUS:%d, CC:%d", (int)_vbusVoltage, (int)_cc);

        USBPD_TRACE_Add(USBPD_TRACE_CADEVENT, 0, USBPD_CAD_EVENT_ATTACHED, NULL, 0);
        USBPD_TRACE_Add(USBPD_TRACE_DEBUG,    0, 0,                        (uint8_t*)strtmp, strlen(strtmp));
        _menuSel = MENU_PD_SPEC;
        demo_timeout = HAL_GetTick();
        Display_menuupdate_info(_menuSel);

      }
      else
      {
        Display_stlogo();
        _cc = CCNONE;
      }
      _vbusCurrent = 0;
      break;
    }
  case ATTACHED :
    {
      BSP_PWRMON_GetCurrent(ALERT_VBUS, &_vbusCurrent);
      /* detach event only linked with vbus level */
      if(_vbusVoltage < 1000)
      {
        char strtmp[100];
        sprintf(strtmp, "VBUS:%d: CC:%d", (int)_vbusVoltage, (int)_cc);

        USBPD_TRACE_Add(USBPD_TRACE_DEBUG,    0, 0,                        (uint8_t*)strtmp, strlen(strtmp));

        _state = DETACHED;
        /* disable message reception on USCP to */
        USBPD_TRACE_Add(USBPD_TRACE_CADEVENT, 0, USBPD_CAD_EVENT_DETACHED, NULL, 0);
        Display_reset_spymsg();
        _menuSel = MENU_STLOGO;
        _cc = CCNONE;
        DPM_Ports[0].DPM_NumberOfRcvSRCPDO = 0;
        DPM_Ports[0].DPM_NumberOfRcvSNKPDO = 0;
        memset(&DPM_Ports[0].DPM_RcvSRCExtendedCapa, 0, sizeof(DPM_Ports[0].DPM_RcvSRCExtendedCapa));
        Display_menuupdate_info(_menuSel);
        BSP_LED_Off(LED7);
        BSP_LED_Off(LED6);
        BSP_LED_Off(LED5);
        BSP_LED_Off(LED4);
        st_logo =10;
        pe_disabled = 0; /* reset PE state information in case of no PD device attached */
      }
      break;
    }
  }

  /* check the message reception of message */
  if(xQueueReceive(DemoMsgBox, &_msg, 0) == pdTRUE)
  {
    /* decode the message to extract information */
    count_msg_received++;
    Display_add_spymsg(_msg);
    Display_menuupdate_info(_menuSel);
  }

  if (ATTACHED == _state)
  {
    if (HAL_GetTick() - demo_timeout> 20000)
    {
      _menuSel = MENU_MEASURE;
      demo_timeout = HAL_GetTick();
      Display_menuupdate_info(_menuSel);
    }
  }

  /* check reception message of MMI event */
  if(xQueueReceive(DemoEvent, &_mmi, 0) == pdTRUE)
  {
    switch(_mmi)
    {
    case DEMO_MMI_ACTION_LEFT_PRESS :
      _menuSel = Menu_manage_prev(_menuSel);
      Display_menuupdate_info(_menuSel);
      demo_timeout = HAL_GetTick();;
      break;
    case DEMO_MMI_ACTION_RIGHT_PRESS :
      _menuSel = Menu_manage_next(_menuSel);
      Display_menuupdate_info(_menuSel);
      demo_timeout = HAL_GetTick();;
      break;
    case DEMO_MMI_ACTION_DOWN_PRESS :       /* up /down */
      Display_menunav_info(_menuSel, +1);
      demo_timeout = HAL_GetTick();;
      break;
    case  DEMO_MMI_ACTION_UP_PRESS:
      Display_menunav_info(_menuSel, -1);
      demo_timeout = HAL_GetTick();;
      break;
    }
  }


  BSP_LCD_Refresh();
}

DMA_Channel_TypeDef *hdmarx;
UCPD_TypeDef *husbpd;
uint8_t ptr_RxBuff[264];
#define SIZE_MAX_PD_TRANSACTION 264

void Initialize_USBPD_Spy(void)
{
  LL_UCPD_InitTypeDef settings;

  husbpd = USBPD_HW_GetUSPDInstance(0);

  /* initialise usbpd */
  LL_UCPD_StructInit(&settings);
  LL_UCPD_Init(husbpd, &settings);
  LL_UCPD_SetRxOrderSet(husbpd, LL_UCPD_ORDERSET_SOP | LL_UCPD_ORDERSET_HARDRST);
  LL_UCPD_Enable(husbpd);
  LL_UCPD_SetccEnable(husbpd, LL_UCPD_CCENABLE_CC1CC2);

  /* disable dead battery */
  RCC->APBENR2|= RCC_APBENR2_SYSCFGEN;
//  for(uint32_t index= 0; index < 1000; index++) { asm("NOP");};
  SET_BIT(SYSCFG->CFGR1,SYSCFG_CFGR1_UCPD1_STROBE);
//  for(uint32_t index= 0; index < 1000; index++) { asm("NOP");};
}

void Intialize_RX_processing(CCxPin_TypeDef cc)
{
  /* Prepare ucpd to handle PD message
            RX message start listen
            TX prepare the DMA to be transfer ready
            Detection listen only the line corresponding CC=Rd for SRC/SNK */
//  Ports[PortNum].hdmatx = BSP_USBPD_Init_DMATxInstance(PortNum);
  hdmarx = USBPD_HW_Init_DMARxInstance(0);

  /* Set the RX dma to allow reception */
  WRITE_REG(hdmarx->CPAR, (uint32_t)&husbpd->RXDR);
  WRITE_REG(hdmarx->CMAR, (uint32_t)ptr_RxBuff);
  hdmarx->CNDTR = SIZE_MAX_PD_TRANSACTION;
  hdmarx->CCR|= DMA_CCR_EN;

  /* disabled non Rd line set CC line enable */
#define INTERRUPT_MASK  UCPD_IMR_HRSTDISCIE  | UCPD_IMR_HRSTSENTIE | \
                        UCPD_IMR_RXORDDETIE  | UCPD_IMR_RXHRSTDETIE | UCPD_IMR_RXOVRIE | UCPD_IMR_RXMSGENDIE

  MODIFY_REG(husbpd->IMR, INTERRUPT_MASK, INTERRUPT_MASK);

  /* Handle CC enable */
//  Ports[PortNum].CCx = cc;
  LL_UCPD_SetccEnable(husbpd, LL_UCPD_CCENABLE_CC1CC2);

  /* Set CC pin for PD message */
  LL_UCPD_SetCCPin(husbpd, cc == CC1?LL_UCPD_CCPIN_CC1:LL_UCPD_CCPIN_CC2);

  LL_UCPD_SetRxMode(husbpd, LL_UCPD_RXMODE_NORMAL);
  LL_UCPD_RxDMAEnable(husbpd);
  LL_UCPD_RxEnable(husbpd);
}


/**
  * @brief  main demo function to manage all the appplication event and to update MMI in standalone mode
  * @param  Event
  * @retval None
  */
static void DEMO_Manage_event(uint32_t Event)
{
  static DEMO_MENU _tab_menu_val = MENU_START;
  static uint8_t   _tab_connect_status = 0;

  if (GPIO_PIN_SET == HAL_GPIO_ReadPin(DOOR_SENSE_PORT, DOOR_SENSE_PIN))
  {
    // Disco door has changed position : Reset the board
    HAL_NVIC_SystemReset();
  }

  switch(Event & DEMO_MSG_TYPE_MSK)
  {
  case DEMO_MSG_MMI:
    {
      switch(Event & DEMO_MMI_ACTION_Msk)
      {
      case DEMO_MMI_ACTION_NONE:
        if(_tab_connect_status != 0)
        {
          if ((MODE_SPY == hmode) || ((MODE_SPY != hmode) && (MENU_MEASURE == _tab_menu_val)))
          {
            Display_menuupdate_info(_tab_menu_val);
          }
          if (DPM_Params[0].PE_Power != USBPD_POWER_NO)
          {
            if (HAL_GetTick() - demo_timeout> 20000)
            {
              _tab_menu_val = MENU_MEASURE;
              demo_timeout = HAL_GetTick();
              Display_menuupdate_info(_tab_menu_val);
            }
          }
        }
        break;
      case  DEMO_MMI_ACTION_RIGHT_PRESS:
        _tab_menu_val = Menu_manage_next(_tab_menu_val);
        Display_menuupdate_info(_tab_menu_val);
        demo_timeout = HAL_GetTick();
        break;
      case  DEMO_MMI_ACTION_LEFT_PRESS:
        _tab_menu_val = Menu_manage_prev(_tab_menu_val);
        Display_menuupdate_info(_tab_menu_val);
        demo_timeout = HAL_GetTick();
        break;
      case  DEMO_MMI_ACTION_UP_PRESS:       /* up /down */
        Display_menunav_info(_tab_menu_val, -1);
        demo_timeout = HAL_GetTick();
        break;
      case  DEMO_MMI_ACTION_DOWN_PRESS:
        Display_menunav_info(_tab_menu_val, +1);
        demo_timeout = HAL_GetTick();
        break;
      case DEMO_MMI_ACTION_SEL_PRESS :
        if (Display_menuexec_info(_tab_menu_val) ==0) /* If action successfull */
        {
          _tab_menu_val=next_menu;
        }
        Display_menuupdate_info(_tab_menu_val);
        demo_timeout = HAL_GetTick();
        break;
      }
    }
    break;
  case DEMO_MSG_CAD :
    {
       switch((Event & DEMO_CAD_STATE_Msk)>> DEMO_CAD_STATE_Pos)
       {
       case USBPD_CAD_EVENT_ATTEMC :
       case USBPD_CAD_EVENT_ATTACHED :
         _tab_connect_status = 1;
         indexAPDO = 0;
         BSP_LED_On(LED4);
         _tab_menu_val = MENU_START;
         demo_timeout = HAL_GetTick();
         Display_menuupdate_info(_tab_menu_val);
         st_logo =10;
         break;
       case USBPD_CAD_EVENT_DETACHED :
         _tab_connect_status = 0;
         st_logo =10;
         pe_disabled = 0; /* reset PE status information for no PD device attached */
         _tab_menu_val = MENU_STLOGO;
         Display_menuupdate_info(_tab_menu_val);
         
         BSP_LED_Off(LED4);
         BSP_LED_Off(LED5);
         BSP_LED_Off(LED6);
         break;
       }
    }
  break;
  case DEMO_MSG_GETINFO  :
    {
      switch((Event & DEMO_MSG_DATA_Msk))
      {
      case DEMO_MSG_GETINFO_SNKCAPA :
        {
          if(0 != _tab_connect_status)
          {
            if( USBPD_OK == USBPD_PE_Request_CtrlMessage(0, USBPD_CONTROLMSG_GET_SNK_CAP, USBPD_SOPTYPE_SOP))
            {
              /* Request has accepted so switch to the next request */
              DEMO_PostGetInfoMessage(0, DEMO_MSG_GETINFO_SVID);
            }
            else
            {
              /* Request has not been accept by the stack so retry to send a message */
              DEMO_PostGetInfoMessage(0, DEMO_MSG_GETINFO_SNKCAPA);
            }
          }
          break;
        }
      case DEMO_MSG_GETINFO_SVID :
        {
          if(0 != _tab_connect_status)
          {
            if( USBPD_OK != USBPD_DPM_RequestVDM_DiscoverySVID(0, USBPD_SOPTYPE_SOP))
            {
              /* Request has not been accept by the stack so retry to send a message */
              DEMO_PostGetInfoMessage(0, DEMO_MSG_GETINFO_SVID);
            }
          }
          break;
        }

      }
      break;
    }
  case DEMO_MSG_PENOTIFY :
    {
      switch((Event & DEMO_MSG_DATA_Msk)>> DEMO_MSG_DATA_Pos)
      {
      case USBPD_NOTIFY_POWER_EXPLICIT_CONTRACT :
        if (_tab_connect_status == 1)
        {
          _tab_menu_val = MENU_START;
          Display_menuupdate_info(_tab_menu_val);
          /* start a timer to delay request to avoid ony conflict with request coming from oposite part */
          xTimerStart( xTimers, 0 );
          _tab_connect_status = 2;
        }
        break;
      case USBPD_NOTIFY_VDM_SVID_RECEIVED :
        {
          DP_info_received = 1; /* Display port info */
          break;
        }
      case USBPD_NOTIFY_HARDRESET_RX :
        {
         st_logo =10;
         pe_disabled = 0; /* reset PE state information in case of no PD device attached */
         _tab_menu_val = MENU_START;
         Display_menuupdate_info(_tab_menu_val);
         break;
        }
      case USBPD_NOTIFY_PE_DISABLED :
        {
         pe_disabled = 1; /* means that attached device is not PD : PE is disabled */
         _tab_menu_val = MENU_PD_SPEC;
         Display_menuupdate_info(_tab_menu_val);
         break;
        }        
      case USBPD_NOTIFY_HARDRESET_TX :
      case USBPD_NOTIFY_SNK_GOTOMIN :
      case USBPD_NOTIFY_SNK_GOTOMIN_READY :
      case USBPD_NOTIFY_SOFTRESET_RECEIVED :
      case USBPD_NOTIFY_PING_RECEIVED :
      case USBPD_NOTIFY_GETSRCCAP_ACCEPTED :
      case USBPD_NOTIFY_GETSRCCAP_REJECTED :
      case USBPD_NOTIFY_GETSNKCAP_TIMEOUT :
      case USBPD_NOTIFY_REQUEST_ACCEPTED :
      case USBPD_NOTIFY_REQUEST_REJECTED :
      case USBPD_NOTIFY_REQUEST_WAIT :
      case USBPD_NOTIFY_REQUEST_GOTOMIN :
      case USBPD_NOTIFY_POWER_SWAP_TO_SNK_DONE :
      case USBPD_NOTIFY_POWER_SWAP_TO_SRC_DONE :
      case USBPD_NOTIFY_POWER_SWAP_REJ :
        break;
      case USBPD_NOTIFY_DATAROLESWAP_UFP:
        {
          _tab_menu_val = MENU_DATA_ROLE_SWAP;
          Display_menuupdate_info(_tab_menu_val);
          break;
        }
      case USBPD_NOTIFY_DATAROLESWAP_DFP:
        {
          _tab_menu_val = MENU_DATA_ROLE_SWAP;
          Display_menuupdate_info(_tab_menu_val);
          break;
        }
      case USBPD_NOTIFY_REQUEST_ENTER_MODE :
      case USBPD_NOTIFY_REQUEST_ENTER_MODE_ACK :
      case USBPD_NOTIFY_REQUEST_ENTER_MODE_NAK :
      case USBPD_NOTIFY_REQUEST_ENTER_MODE_BUSY :
      default :
        break;
      }
    }
  break;
  }
  BSP_LCD_Refresh();
}


/**
  * @brief  demo task function
  * @param  arg
  * @retval None
  */
void DEMO_Task_SPY(void const *arg)
{
  NVIC_SetPriority(UCPD1_2_IRQn,0);
  NVIC_EnableIRQ(UCPD1_2_IRQn);
    
  for (;;)
  {
      DEMO_Manage_spy();
      osDelay(1); /* to allow FreeRTOS scheduling*/
  }
}


/**
  * @brief  demo task function
  * @param  arg
  * @retval None
  */

void DEMO_Task_Standalone(void const *arg)
{
  st_logo=10;
  Display_stlogo();

  /* Create a timer to ask PE stack request delayed of 200ms after the EXPLICIT contract */
  xTimers = xTimerCreate
            ( /* Just a text name, not used by the RTOS
                 kernel. */
              "Timer",
               /* The timer period in ticks, must be
                  greater than 0. */
               300,
               /* The timers will auto-reload themselves
                  when they expire. */
               0,
               /* The ID is used to store a count of the
                  number of times the timer has expired, which
                  is initialised to 0. */
               ( void * ) 0,
                /* Each timer calls the same callback when
                   it expires. */
                vTimerCallback
             );

  DEMO_Manage_event(DEMO_MSG_MMI | DEMO_MMI_ACTION_NONE);

  for (;;)
  {
    osEvent event = osMessageGet(DemoEvent, 100);
    DEMO_Manage_event(DEMO_MSG_MMI | DEMO_MMI_ACTION_NONE);
    if(osEventMessage == event.status)
    {
      DEMO_Manage_event(event.value.v);
    }
  }
}

/**
  * @brief  demo ucpd spy irq handler
  * @retval None
  */
uint32_t count_message_it;
void DEMO_SPY_Handler(void)
{
  uint32_t _interrupt = LL_UCPD_ReadReg(husbpd, SR);
  static uint8_t ovrflag = 0;

  if((husbpd->IMR & _interrupt) != 0)
  {
    /* RXORDDET: not needed so stack will not enabled this interrupt */
    if(_interrupt & UCPD_SR_RXORDDET)
    {
      LL_UCPD_ClearFlag_RxOrderSet(husbpd);
      return;
    }

    /* check RXHRSTDET */
    if(_interrupt & UCPD_SR_RXHRSTDET)
    {
       /* receive an hardreset message */
       LL_UCPD_ClearFlag_RxHRST(husbpd);
       return;
    }

    /* check RXOVR */
    if(_interrupt & UCPD_SR_RXOVR)
    {
     /* nothing to do the message will be discarded and the port partner retry the send */
      ovrflag = 1;
      LL_UCPD_ClearFlag_RxOvr(husbpd);
      return;
    }

    /* check RXMSGEND an Rx message has been recieved */
    if(_interrupt & UCPD_SR_RXMSGEND )
    {
      /* for DMA mode add a control to check if the number of data recived is corresponding with the number of
         data receive by USBPD */
      uint16_t _datasize = husbpd->RX_PAYSZ;
      LL_UCPD_ClearFlag_RxMsgEnd(husbpd);

      CLEAR_BIT(hdmarx->CCR, DMA_CCR_EN);

      if(((_interrupt & UCPD_SR_RXERR) == 0) && (ovrflag == 0))
      {
        /* Rx message has been recieved without error */
        DEMO_MsgHeader_TypeDef header = *(DEMO_MsgHeader_TypeDef *)ptr_RxBuff;

        if(header.b.PortDataRole == USBPD_PORTDATAROLE_UFP)
        {
          USBPD_TRACE_Add(USBPD_TRACE_SNK, 0, 0, ptr_RxBuff, _datasize);
        }
        else
        {
          USBPD_TRACE_Add(USBPD_TRACE_SRC, 0, 0, ptr_RxBuff, _datasize);
        }

        /* message reception complete */
        USBPD_Msg _msg;
        BaseType_t xHigherPriorityTaskWoken;

        _msg.SOPx = 0;
        _msg.size = _datasize;
        memcpy(_msg.data, ptr_RxBuff, 30);
        count_message_it++;
        xQueueSendFromISR(DemoMsgBox, &_msg, &xHigherPriorityTaskWoken);
      }
      ovrflag = 0;

      /* Ready for next transaction */
      WRITE_REG(hdmarx->CMAR, (uint32_t)ptr_RxBuff);
      WRITE_REG(hdmarx->CNDTR, SIZE_MAX_PD_TRANSACTION);
      SET_BIT(hdmarx->CCR, DMA_CCR_EN);
      return;
    }

    /* check FRSEVTIE */
    if(_interrupt !=  UCPD_SR_FRSEVT)
    {
      /* not yet handled */
    }
  }
}

void SPY_TRACE_TX_Task(void const *argument)
{
  for(;;)
  {
    USBPD_TRACE_TX_Process();
    osDelay(5);
  }
}

static void string_completion(uint8_t *str)
{
  uint8_t size = strlen((char *)str);

  if(size > STR_SIZE_MAX) str[STR_SIZE_MAX] = '\0';
  else
  {
    /* add space */
    for(uint8_t index = size; index < STR_SIZE_MAX; index++)
    {
      str[index] = ' ';
    }
    str[STR_SIZE_MAX] = '\0';
  }
}

void vTimerCallback(TimerHandle_t xTimer)
{
  /* the timer has expired so if the connection is still valid, send a stack message to request a message */
  DEMO_PostGetInfoMessage(0, DEMO_MSG_GETINFO_SNKCAPA);
}

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
