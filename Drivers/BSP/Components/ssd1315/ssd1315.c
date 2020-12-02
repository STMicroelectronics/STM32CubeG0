/**
  ******************************************************************************
  * @file    ssd1315.c
  * @author  MCD Application Team
  * @brief   This file includes the LCD driver for ssd1315 LCD.
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
#include "ssd1315.h"

/** @addtogroup BSP
* @{
*/

/** @addtogroup Components
* @{
*/

/** @addtogroup ssd1315
* @brief     This file provides a set of functions needed to drive the
*            ssd1315 LCD.
* @{
*/

/** @defgroup ssd1315_Private_TypesDefinitions
* @{
*/

/**
* @}
*/

/** @defgroup ssd1315_Private_Macros
* @{
*/

/**
* @}
*/

/** @defgroup ssd1315_Private_Variables
* @{
*/
LCD_DrvTypeDef   ssd1315_drv =
{
  ssd1315_Init,
  0,
  ssd1315_DisplayOn,
  ssd1315_DisplayOff,
  0,
  ssd1315_WritePixel,
  ssd1315_ReadPixel,
  0,
  ssd1315_DrawHLine,
  ssd1315_DrawVLine,
  ssd1315_GetLcdPixelWidth,
  ssd1315_GetLcdPixelHeight,
  ssd1315_DrawBitmap,
  0,
};

static uint8_t Is_ssd1315_Initialized = 0;

/* Physical frame buffer for background and foreground layers */
/* 128*64 pixels with 1bpp */
#if defined ( __ICCARM__ )  /* IAR Compiler */
  #pragma data_alignment = 16
uint8_t              PhysFrameBuffer[SSD1315_LCD_COLUMN_NUMBER*SSD1315_LCD_PAGE_NUMBER];
#elif defined (__GNUC__)    /* GNU Compiler */
uint8_t              PhysFrameBuffer[SSD1315_LCD_COLUMN_NUMBER*SSD1315_LCD_PAGE_NUMBER] __attribute__ ((aligned (16)));
#else                       /* ARM Compiler */
__align(16) uint8_t  PhysFrameBuffer[SSD1315_LCD_COLUMN_NUMBER*SSD1315_LCD_PAGE_NUMBER];
#endif
/**
* @}
*/

/** @defgroup ssd1315_Private_FunctionPrototypes
* @{
*/

/**
* @}
*/

/** @defgroup ssd1315_Private_Functions
* @{
*/

/**
  * @brief  Initialise the ssd1315 LCD Component.
  * @param  None
  * @retval None
  */
void ssd1315_Init(void)
{
  if(Is_ssd1315_Initialized == 0)
  {
    Is_ssd1315_Initialized = 1;
    /* Initialise ssd1315 low level bus layer --------------------------------*/
    LCD_IO_Init();

    /* Driving ability setting */
    /* Initialisation sequence */
    LCD_IO_WriteCommand(0x80);
    LCD_IO_WriteCommand(0x8D);  /* Disable charge pump regulator */
    LCD_IO_WriteCommand(0x14);
    LCD_IO_WriteCommand(0x20);  /* Set Memory Addressing Mode */
    LCD_IO_WriteCommand(0x00);  /* 00b: Horizontal Addressing Mode */
    LCD_IO_WriteCommand(0x40);  /* 00b: Horizontal Addressing Mode */
    LCD_IO_WriteCommand(0xC8);  /* c8:flip the 64 rows */
    LCD_IO_WriteCommand(0xA1);  /* a1:flip the 128 columns */
    LCD_IO_WriteCommand(0xAF);  /* Display On */
  }

  ssd1315_Clear(SSD1315_COLOR_BLACK);

  LCD_IO_WriteMultipleData((uint8_t*)PhysFrameBuffer, SSD1315_LCD_COLUMN_NUMBER*SSD1315_LCD_PAGE_NUMBER);
}

/**
  * @brief  Clear Display screen.
  * @param  ColorCode: the color use to clear the screen (SSD1315_COLOR_WHITE or SSD1315_COLOR_BLACK)
  * @retval None
  */
void ssd1315_Clear(uint16_t ColorCode)
{
  /* Check color */
  if (ColorCode == SSD1315_COLOR_WHITE) 
  {
    memset(PhysFrameBuffer, 0xFF, SSD1315_LCD_COLUMN_NUMBER*SSD1315_LCD_PAGE_NUMBER);
  }
  else
  {
    memset(PhysFrameBuffer, 0x00, SSD1315_LCD_COLUMN_NUMBER*SSD1315_LCD_PAGE_NUMBER);
  }
}

/**
  * @brief  Enables the Display.
  * @param  None
  * @retval None
  */
void ssd1315_DisplayOn(void)
{
  /* Power On sequence ---------------------------------------------------------*/
  LCD_IO_WriteCommand(0x8D);
  LCD_IO_WriteCommand(0x14);
  LCD_IO_WriteCommand(0xAF);
}

/**
  * @brief  Disables the Display.
  * @param  None
  * @retval None
  */
void ssd1315_DisplayOff(void)
{
  /* Power Off sequence ---------------------------------------------------------*/
  LCD_IO_WriteCommand(0x8D);
  LCD_IO_WriteCommand(0x10);
  LCD_IO_WriteCommand(0xAE);
}

/**
  * @brief  Get the LCD pixel Width.
  * @param  None
  * @retval The Lcd Pixel Width
  */
uint16_t ssd1315_GetLcdPixelWidth(void)
{
  return (uint16_t)SSD1315_LCD_PIXEL_WIDTH;
}

/**
  * @brief  Get the LCD pixel Height.
  * @param  None
  * @retval The Lcd Pixel Height
  */
uint16_t ssd1315_GetLcdPixelHeight(void)
{
  return (uint16_t)SSD1315_LCD_PIXEL_HEIGHT;
}

/**
  * @brief  Write pixel.
  * @param  Xpos: specifies the X position.
  * @param  Ypos: specifies the Y position.
  * @param  ColorCode: the pixel color (SSD1315_COLOR_WHITE or SSD1315_COLOR_BLACK)
  * @retval None
  */
void ssd1315_WritePixel(uint16_t Xpos, uint16_t Ypos, uint16_t ColorCode)
{
  /* Set color */
  if (ColorCode == SSD1315_COLOR_WHITE) {
    PhysFrameBuffer[Xpos + (Ypos / 8) * SSD1315_LCD_PIXEL_WIDTH] |= 1 << (Ypos % 8);
  } else {
    PhysFrameBuffer[Xpos + (Ypos / 8) * SSD1315_LCD_PIXEL_WIDTH] &= ~(1 << (Ypos % 8));
  }
}

/**
  * @brief  Read pixel.
  * @param  None
  * @retval the pixel color(0 for Black, 1 for White)
  */
uint16_t ssd1315_ReadPixel(uint16_t Xpos, uint16_t Ypos)
{
  if ((Xpos >= SSD1315_LCD_PIXEL_WIDTH) || (Ypos >= SSD1315_LCD_PIXEL_HEIGHT)) return 0;
  return PhysFrameBuffer[Xpos+ (Ypos/8)*SSD1315_LCD_PIXEL_WIDTH] & (1 << Ypos%8) ? 1 : 0;
}

/**
  * @brief  Set Page position.
  * @param  Page:   specifies the Page position (0-7).
  * @retval None
  */
void ssd1315_SetPage(uint16_t Page)
{
  /* Set Page position  */
  LCD_IO_WriteCommand(0xB0 | Page);
}

/**
  * @brief  Set Column position.
  * @param  Column: specifies the Column position (0-127).
  * @retval None
  */
void ssd1315_SetColumn( uint16_t Column)
{
  /* Set Column position  */
  LCD_IO_WriteCommand(0x00);
  LCD_IO_WriteCommand(0x00 | Column);
  LCD_IO_WriteCommand(0x1F);
}

/**
  * @brief  Draw horizontal line.
  * @param  RGBCode:  the pixel color (SSD1315_COLOR_WHITE or SSD1315_COLOR_BLACK)
  * @param  Xpos:     specifies the X position.
  * @param  Ypos:     specifies the Y position.
  * @param  Length:   specifies the Line length.
  * @retval None
  */
void ssd1315_DrawHLine(uint16_t RGBCode, uint16_t Xpos, uint16_t Ypos, uint16_t Length)
{
  uint32_t i = 0;

  /* Sent a complete horizontal line */
  for (i = Xpos; i < (Xpos+Length); i++)
  {
    ssd1315_WritePixel(i, Ypos, RGBCode);
  }
}

/**
  * @brief  Draw vertical line.
  * @param  RGBCode:  the pixel color (SSD1315_COLOR_WHITE or SSD1315_COLOR_BLACK)
  * @param  Xpos:     specifies the X position.
  * @param  Ypos:     specifies the Y position.
  * @param  Length:   specifies the Line length.
  * @retval None
  */
void ssd1315_DrawVLine(uint16_t RGBCode, uint16_t Xpos, uint16_t Ypos, uint16_t Length)
{
  uint32_t i = 0;

  /* Sent a complete vertical line */
  for (i = Ypos; i < (Ypos+Length); i++)
  {
    ssd1315_WritePixel(Xpos, i, RGBCode);
  }
}

/**
  * @brief  Displays a bitmap picture loaded in the internal Flash.
  * @param  Xpos:     specifies the X position.
  * @param  Ypos:     specifies the Y position.
  * @param  pbmp:     Bmp picture address in the internal Flash.
  * @retval None
  */
void ssd1315_DrawBitmap(uint16_t Xpos, uint16_t Ypos, uint8_t *pbmp)
{
  uint32_t index = 0, size = 0;
  uint32_t height = 0, width  = 0;
  uint32_t x = 0, y  = 0, y0 = 0;
  uint32_t XposBMP = 0, YposBMP  = 0;

  /* Read bitmap size */
  size = pbmp[2] + (pbmp[3] << 8) + (pbmp[4] << 16)  + (pbmp[5] << 24);

  /* Get bitmap data address offset */
  index = pbmp[10] + (pbmp[11] << 8) + (pbmp[12] << 16)  + (pbmp[13] << 24);

  /* Read bitmap width */
  width = pbmp[18] + (pbmp[19] << 8) + (pbmp[20] << 16)  + (pbmp[21] << 24);

  /* Read bitmap height */
  height = pbmp[22] + (pbmp[23] << 8) + (pbmp[24] << 16)  + (pbmp[25] << 24);

  /* Size conversion */
  size = (size - index)/2;

  /* Apply offset to bypass header */
  pbmp += index;

  /* if bitmap cover whole screen */
  if((Xpos == 0) && (Ypos == 0) & (size == (SSD1315_LCD_PIXEL_WIDTH * SSD1315_LCD_PIXEL_HEIGHT/8)))
  {
    memcpy(PhysFrameBuffer, pbmp, size);
  }
  else
  {
    x=Xpos+width;
    y=Ypos+height;
    y0 = Ypos;

    for(; Xpos < x; Xpos++, XposBMP++)
    {
      for(Ypos = y0, YposBMP = 0; Ypos < y; Ypos++, YposBMP++)
      {
        /* if bitmap and screen are aligned on a Page */
        if(((Ypos%8) == 0) && (y-Ypos >= 8) && ((YposBMP%8) == 0))
        {
          PhysFrameBuffer[Xpos+ (Ypos/8)*SSD1315_LCD_PIXEL_WIDTH] = pbmp[XposBMP+((YposBMP/8)*width)];
          Ypos+=7;
          YposBMP+=7;
        }
        else
        {
          /* Draw bitmap pixel per pixel */
          if( (pbmp[XposBMP+((YposBMP/8)*width)]&(1<<(YposBMP%8))) != 0)
             ssd1315_WritePixel(Xpos, Ypos, 0xFF );
          else
            ssd1315_WritePixel(Xpos, Ypos, 0x00 );
        }
      }
    }
  }
}

/**
  * @brief  Shift and Displays a bitmap picture loaded in the internal Flash.
  * @param  Xpos:     specifies the X position.
  * @param  Ypos:     specifies the Y position.
  * @param  Xshift:   specifies number of pixel to shift on X position.
  * @param  Yshift:   specifies number of pixel to shift on Y position.
  * @param  pbmp:     Bmp picture address in the internal Flash.
  * @retval None
  */
void ssd1315_ShiftBitmap(uint16_t Xpos, uint16_t Ypos, int16_t Xshift, int16_t Yshift,uint8_t *pbmp)
{
  uint32_t index = 0, size = 0;
  uint32_t height = 0, width  = 0, original_width  = 0;
  uint32_t x = 0, y  = 0, y0 = 0;
  uint32_t XposBMP = 0, YposBMP  = 0, original_YposBMP = 0;

  /* Read bitmap size */
  size = *(volatile uint16_t *) (pbmp + 2);
  size |= (*(volatile uint16_t *) (pbmp + 4)) << 16;

  /* Get bitmap data address offset */
  index = *(volatile uint16_t *) (pbmp + 10);
  index |= (*(volatile uint16_t *) (pbmp + 12)) << 16;

  /* Read bitmap width */
  width = *(uint16_t *) (pbmp + 18);
  width |= (*(uint16_t *) (pbmp + 20)) << 16;
  original_width = width;
  if( Xshift>=0)
  {
    Xpos = Xpos + Xshift;
    width = width - Xshift;
  }
  else
  {
    width = width + Xshift;
    XposBMP = -Xshift;
  }

  /* Read bitmap height */
  height = *(uint16_t *) (pbmp + 22);
  height |= (*(uint16_t *) (pbmp + 24)) << 16;
  if( Yshift>=0)
  {
    height = height - Yshift;
    Ypos = Ypos + Yshift;
  }
  else
  {
    height = height + Yshift;
    YposBMP = -Yshift;
  }
  original_YposBMP = YposBMP;

  /* Size conversion */
  size = (size - index)/2;
  size = size - ((Xshift*height/8)+(Yshift*width/8 ));

  /* Apply offset to bypass header */
  pbmp += index;

  /* if bitmap cover whole screen */
  if((Xpos == 0) && (Ypos == 0) & (size == (SSD1315_LCD_PIXEL_WIDTH * SSD1315_LCD_PIXEL_HEIGHT/8)))
  {
    memcpy(PhysFrameBuffer, pbmp, size);
  }
  else
  {
    x=Xpos+width;
    y=Ypos+height;
    y0 = Ypos;

    for(; Xpos < x; Xpos++, XposBMP++)
    {
      for(Ypos = y0, YposBMP = original_YposBMP; Ypos < y; Ypos++, YposBMP++)
      {
        /* if bitmap and screen are aligned on a Page */
        if(((Ypos%8) == 0) && (y-Ypos >= 8) && ((YposBMP%8) == 0))
        {
          PhysFrameBuffer[Xpos+ (Ypos/8)*SSD1315_LCD_PIXEL_WIDTH] = pbmp[XposBMP+((YposBMP/8)*original_width)];
          Ypos+=7;
          YposBMP+=7;
        }
        else
        {
          /* Draw bitmap pixel per pixel */
          if( (pbmp[XposBMP+((YposBMP/8)*original_width)]&(1<<(YposBMP%8))) != 0)
             ssd1315_WritePixel(Xpos, Ypos, 0xFF );
          else
            ssd1315_WritePixel(Xpos, Ypos, 0x00 );
        }
      }
    }
  }
}

/**
  * @brief  Scrolling Display Page.
  * @param  ScrollMode: SSD1315_SCROLL_RIGHT or SSD1315_SCROLL_LEFT
  * @param  StartPage: Start page  for scrolling:
            @arg  0..7
  * @param  EndPage: End page for scrolling:
            This must be larger or equal to StartLine
            @arg  0..7
  * @param  Frequency: SSD1315_SCROLL_FREQ_2FRAMES to SSD1315_SCROLL_FREQ_256FRAMES
  * @retval None
  */
void ssd1315_ScrollingSetup(uint16_t ScrollMode, uint16_t StartPage, uint16_t EndPage, uint16_t Frequency)
{
  /* Scrolling setup sequence */
  LCD_IO_WriteCommand(ScrollMode);  /* Right/Left Horizontal Scroll */
  LCD_IO_WriteCommand(0x00);        /* Dummy byte (Set as 00h) */
  LCD_IO_WriteCommand(StartPage);   /* start page address*/
  LCD_IO_WriteCommand(Frequency);   /* start page address*/
  LCD_IO_WriteCommand(EndPage);     /* End page address*/
  LCD_IO_WriteCommand(0x00);        /* Dummy byte (Set as 00h) */
  LCD_IO_WriteCommand(0xFF);        /* Dummy byte (Set as ffh) */
}

/**
  * @brief  Start Display Scrolling.
  * @param  None
  * @retval None
  */
void ssd1315_ScrollingStart(void)
{
  /* Start scrolling sequence */
  LCD_IO_WriteCommand(0x2F);
}

/**
  * @brief  Stop Display Scrolling.
  * @param  None
  * @retval None
  */
void ssd1315_ScrollingStop(void)
{
  /* Stop scrolling  sequence */
  LCD_IO_WriteCommand(0x2E);
}

/**
  * @brief  Refresh Displays.
  * @param  None
  * @retval None
  */
void ssd1315_Refresh(void)
{
  /* Set Display Start Line to 0*/
  LCD_IO_WriteCommand(0x40);
  /* Set Column Address Setup column start(0) and end address (127)*/
  LCD_IO_WriteCommand(0x21);
  LCD_IO_WriteCommand(0x00);
  LCD_IO_WriteCommand(0x7F);
  /* Set Page Address Setup page start (0)  and end address (7)*/
  LCD_IO_WriteCommand(0x22);
  LCD_IO_WriteCommand(0x00);
  LCD_IO_WriteCommand(0x07);
  /* Fill Buffer in GDDRAM of LCD */
  LCD_IO_WriteMultipleData(PhysFrameBuffer, SSD1315_LCD_COLUMN_NUMBER*SSD1315_LCD_PAGE_NUMBER);
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
