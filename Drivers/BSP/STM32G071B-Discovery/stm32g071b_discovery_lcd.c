/**
  ******************************************************************************
  * @file    stm32g071b_discovery_lcd.c
  * @author  MCD Application Team
  * @brief   This file includes the driver for Liquid Crystal Display device
  *          supported by the STM32G071B-DISCO board (MB1378).
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
#include "stm32g071b_discovery_lcd.h"
#include "../../../Utilities/Fonts/fonts.h"
#include "../../../Utilities/Fonts/font24.c"
#include "../../../Utilities/Fonts/font20.c"
#include "../../../Utilities/Fonts/font16.c"
#include "../../../Utilities/Fonts/font12.c"
#include "../../../Utilities/Fonts/font8.c"

/** @addtogroup BSP
  * @{
  */

/** @addtogroup STM32G071B-DISCO
  * @{
  */

/** @addtogroup STM32G071B_DISCOVERY_LCD
  * @{
  */

/** @defgroup STM32G071B_DISCOVERY_LCD_Private_Defines Private Constants
  * @{
  */
#define POLY_X(Z)               ((int32_t)((pPoints + (Z))->X))
#define POLY_Y(Z)               ((int32_t)((pPoints + (Z))->Y))

#define MAX_HEIGHT_FONT         11U
#define MAX_WIDTH_FONT          16U
#define OFFSET_BITMAP           54U
/**
  * @}
  */

/** @defgroup STM32G071B_DISCOVERY_LCD_Private_Macros Private Macros
  * @{
  */
#define ABS(X)  ((X) > 0 ? (X) : -(X))

/**
  * @}
  */

/** @defgroup STM32G071B_DISCOVERY_LCD_Private_Variables Private Variables
  * @{
  */
LCD_DrawPropTypeDef DrawProp;

static LCD_DrvTypeDef  *lcd_drv;

/**
  * @}
  */

/** @defgroup STM32G071B_DISCOVERY_LCD_Private_Functions Private Functions
  * @{
  */
static void LCD_DrawChar(uint16_t Column, uint16_t Page, const uint8_t *pChar);
/**
  * @}
  */

/** @addtogroup STM32G071B_DISCOVERY_LCD_Exported_Functions
  * @{
  */

/**
  * @brief  Initializes the LCD.
  * @retval LCD state
  */
uint8_t BSP_LCD_Init(void)
{
  uint8_t ret = LCD_ERROR;

  lcd_drv = &ssd1315_drv;
  ret = LCD_OK;

  if(ret != LCD_ERROR)
  {
    /* LCD Init */
    lcd_drv->Init();

    /* Initialize the font */
    BSP_LCD_SetFont(&LCD_DEFAULT_FONT);
    BSP_LCD_SetTextColor(LCD_COLOR_WHITE);
    BSP_LCD_SetBackColor(LCD_COLOR_BLACK);
  }

  return ret;
}

/**
  * @brief  Gets the LCD X size.
  * @retval Used LCD X size
  */
uint32_t BSP_LCD_GetXSize(void)
{
  return(lcd_drv->GetLcdPixelWidth());
}

/**
  * @brief  Gets the LCD Y size.
  * @retval Used LCD Y size
  */
uint32_t BSP_LCD_GetYSize(void)
{
  return(lcd_drv->GetLcdPixelHeight());
}

/**
  * @brief  Gets the LCD text color.
  * @retval Used text color.
  */
uint16_t BSP_LCD_GetTextColor(void)
{
  return DrawProp.TextColor;
}

/**
  * @brief  Gets the LCD background color.
  * @retval Used background color
  */
uint16_t BSP_LCD_GetBackColor(void)
{
  return DrawProp.BackColor;
}

/**
  * @brief  Sets the LCD text color.
  * @param  Color Text color code RGB(5-6-5)
  * @retval None
  */
void BSP_LCD_SetTextColor(uint16_t Color)
{
  DrawProp.TextColor = Color;
}

/**
  * @brief  Sets the LCD background color.
  * @param  Color Background color code RGB(5-6-5)
  * @retval None
  */
void BSP_LCD_SetBackColor(uint16_t Color)
{
  DrawProp.BackColor = Color;
}

/**
  * @brief  Sets the LCD text font.
  * @param  pFonts Font to be used
  * @retval None
  */
void BSP_LCD_SetFont(sFONT *pFonts)
{
  DrawProp.pFont = pFonts;
}

/**
  * @brief  Gets the LCD text font.
  * @retval Used font
  */
sFONT *BSP_LCD_GetFont(void)
{
  return DrawProp.pFont;
}

/**
  * @brief  Clears the whole LCD.
  * @param  Color Color of the background
  * @retval None
  */
void BSP_LCD_Clear(uint16_t Color)
{
  ssd1315_Clear(Color);
}

/**
  * @brief  Clears the selected line.
  * @param  Line Line to be cleared
  *          This parameter can be one of the following values:
  *            @arg  0..9: if the Current fonts is Font16x24
  *            @arg  0..19: if the Current fonts is Font12x12 or Font8x12
  *            @arg  0..29: if the Current fonts is Font8x8
  * @retval None
  */
void BSP_LCD_ClearStringLine(uint16_t Line)
{
  uint32_t colorbackup = DrawProp.TextColor;
  DrawProp.TextColor = DrawProp.BackColor;

  /* Draw a rectangle with background color */
  BSP_LCD_FillRect(0, (Line * DrawProp.pFont->Height), BSP_LCD_GetXSize(), DrawProp.pFont->Height);

  DrawProp.TextColor = colorbackup;
}

/**
  * @brief  Displays one character.
  * @param  Xpos Start column address
  * @param  Ypos Line where to display the character shape.
  * @param  Ascii Character ascii code
  *           This parameter must be a number between Min_Data = 0x20 and Max_Data = 0x7E
  * @retval None
  */
void BSP_LCD_DisplayChar(uint16_t Xpos, uint16_t Ypos, uint8_t Ascii)
{
  LCD_DrawChar(Xpos, Ypos, &DrawProp.pFont->table[(Ascii-' ') *\
    DrawProp.pFont->Height * ((DrawProp.pFont->Width + 7) / 8)]);
}

/**
  * @brief  Displays characters on the LCD.
  * @param  Xpos X position (in pixel)
  * @param  Ypos Y position (in pixel)
  * @param  pText Pointer to string to display on LCD
  * @param  Mode Display mode
  *          This parameter can be one of the following values:
  *            @arg  CENTER_MODE
  *            @arg  RIGHT_MODE
  *            @arg  LEFT_MODE
  * @retval None
  */
void BSP_LCD_DisplayStringAt(uint16_t Xpos, uint16_t Ypos, uint8_t *pText, Line_ModeTypdef Mode)
{
  uint16_t refcolumn = 1, counter = 0;
  uint32_t size = 0, ysize = 0;
  uint8_t  *ptr = pText;

  /* Get the text size */
  while (*ptr++) size ++ ;

  /* Characters number per line */
  ysize = (BSP_LCD_GetXSize()/DrawProp.pFont->Width);

  switch (Mode)
  {
  case CENTER_MODE:
    {
      refcolumn = Xpos + ((ysize - size)* DrawProp.pFont->Width) / 2;
      break;
    }
  case LEFT_MODE:
    {
      refcolumn = Xpos;
      break;
    }
  case RIGHT_MODE:
    {
      refcolumn = Xpos + ((ysize - size)*DrawProp.pFont->Width);
      break;
    }
  default:
    {
      refcolumn = Xpos;
      break;
    }
  }
  
  /* Check that the Start column is located in the screen */
  if ((refcolumn < 1) || (refcolumn > 128))
  {
    refcolumn = 1;
  }

  /* Send the string character by character on lCD */
  while ((*pText != 0) & (((BSP_LCD_GetXSize() - (counter*DrawProp.pFont->Width)) & 0xFFFF) >= DrawProp.pFont->Width))
  {
    /* Display one character on LCD */
    BSP_LCD_DisplayChar(refcolumn, Ypos, *pText);
    /* Decrement the column position by 16 */
    refcolumn += DrawProp.pFont->Width;
    /* Point on the next character */
    pText++;
    counter++;
  }
}

/**
  * @brief  Displays a character on the LCD.
  * @param  Line Line where to display the character shape
  *          This parameter can be one of the following values:
  *            @arg  0..9 if the Current fonts is Font16x24
  *            @arg  0..19 if the Current fonts is Font12x12 or Font8x12
  *            @arg  0..29 if the Current fonts is Font8x8
  * @param  pText Pointer to string to display on LCD
  * @retval None
  */
void BSP_LCD_DisplayStringAtLine(uint16_t Line, uint8_t *pText)
{
  BSP_LCD_DisplayStringAt(0, LINE(Line),pText, LEFT_MODE);
}

/**
  * @brief  Scrolling LCD Line.
  * @param  StartLine Start line for scrolling: 
            @arg  0..3
  * @param  EndLine End line for scrolling: 
            This must be larger or equal to StartLine
            @arg  0..3
  * @param  Mode LCD_SCROLL_LEFT or LCD_SCROLL_RIGHT 
  * @retval None
  */
void BSP_LCD_ScrollingSetup(uint16_t StartLine, uint16_t EndLine, uint16_t Mode)
{
  uint16_t StartPage = StartLine*2;
  uint16_t EndPage   = EndLine*2+1;
    
  ssd1315_ScrollingSetup(Mode, StartPage,  EndPage, SSD1315_SCROLL_FREQ_3FRAMES);
  ssd1315_ScrollingStart();
}

/**
  * @brief  Start LCD Scrolling.
  * @retval None
  */
void BSP_LCD_ScrollingStart(void)
{
  ssd1315_ScrollingStart();
}

/**
  * @brief  Stop LCD Scrolling.
  * @retval None
  */
void BSP_LCD_ScrollingStop(void)
{
  ssd1315_ScrollingStop();
}

/**
  * @brief  Reads an LCD pixel.
  * @param  Xpos X position
  * @param  Ypos Y position
  * @retval RGB pixel color
  */
uint16_t BSP_LCD_ReadPixel(uint16_t Xpos, uint16_t Ypos)
{
  uint16_t ret = 0;

  if(lcd_drv->ReadPixel != NULL)
  {
    ret = lcd_drv->ReadPixel(Xpos, Ypos);
  }

  return ret;
}

/**
  * @brief  Draws an horizontal line.
  * @param  Xpos X position
  * @param  Ypos Y position
  * @param  Length Line length
  * @retval None
  */
void BSP_LCD_DrawHLine(uint16_t Xpos, uint16_t Ypos, uint16_t Length)
{
  uint32_t index = 0;

  if(lcd_drv->DrawHLine != NULL)
  {
    lcd_drv->DrawHLine(DrawProp.TextColor,  Xpos, Ypos,Length);
  }
  else
  {
    for(index = 0; index < Length; index++)
    {
      BSP_LCD_DrawPixel( Xpos, (Ypos + index), DrawProp.TextColor);
    }
  }
}

/**
  * @brief  Draws a vertical line.
  * @param  Xpos X position
  * @param  Ypos Y position
  * @param  Length Line length
  * @retval None
  */
void BSP_LCD_DrawVLine(uint16_t Xpos, uint16_t Ypos, uint16_t Length)
{
  uint32_t index = 0;

  if(lcd_drv->DrawVLine != NULL)
  {
    lcd_drv->DrawVLine(DrawProp.TextColor, Ypos, Xpos, Length);
  }
  else
  {
    for(index = 0; index < Length; index++)
    {
      BSP_LCD_DrawPixel(Ypos, Xpos + index, DrawProp.TextColor);
    }
  }
}

/**
  * @brief  Draws an uni-line (between two points).
  * @param  X1 Point 1 X position
  * @param  Y1 Point 1 Y position
  * @param  X2 Point 2 X position
  * @param  Y2 Point 2 Y position
  * @retval None
  */
void BSP_LCD_DrawLine(uint16_t X1, uint16_t Y1, uint16_t X2, uint16_t Y2)
{
  int16_t deltax = 0, deltay = 0, x = 0, y = 0, xinc1 = 0, xinc2 = 0,
  yinc1 = 0, yinc2 = 0, den = 0, num = 0, numadd = 0, numpixels = 0,
  curpixel = 0;

  deltax = ABS(Y2 - Y1);        /* The difference between the x's */
  deltay = ABS(X2 - X1);        /* The difference between the y's */
  x = Y1;                       /* Start x off at the first pixel */
  y = X1;                       /* Start y off at the first pixel */

  if (Y2 >= Y1)                 /* The x-values are increasing */
  {
    xinc1 = 1;
    xinc2 = 1;
  }
  else                          /* The x-values are decreasing */
  {
    xinc1 = -1;
    xinc2 = -1;
  }

  if (X2 >= X1)                 /* The y-values are increasing */
  {
    yinc1 = 1;
    yinc2 = 1;
  }
  else                          /* The y-values are decreasing */
  {
    yinc1 = -1;
    yinc2 = -1;
  }

  if (deltax >= deltay)         /* There is at least one x-value for every y-value */
  {
    xinc1 = 0;                  /* Don't change the x when numerator >= denominator */
    yinc2 = 0;                  /* Don't change the y for every iteration */
    den = deltax;
    num = deltax / 2;
    numadd = deltay;
    numpixels = deltax;         /* There are more x-values than y-values */
  }
  else                          /* There is at least one y-value for every x-value */
  {
    xinc2 = 0;                  /* Don't change the x for every iteration */
    yinc1 = 0;                  /* Don't change the y when numerator >= denominator */
    den = deltay;
    num = deltay / 2;
    numadd = deltax;
    numpixels = deltay;         /* There are more y-values than x-values */
  }

  for (curpixel = 0; curpixel <= numpixels; curpixel++)
  {
    BSP_LCD_DrawPixel(x, y, DrawProp.TextColor);  /* Draw the current pixel */
    num += numadd;                            /* Increase the numerator by the top of the fraction */
    if (num >= den)                           /* Check if numerator >= denominator */
    {
      num -= den;                             /* Calculate the new numerator value */
      x += xinc1;                             /* Change the x as appropriate */
      y += yinc1;                             /* Change the y as appropriate */
    }
    x += xinc2;                               /* Change the x as appropriate */
    y += yinc2;                               /* Change the y as appropriate */
  }
}

/**
  * @brief  Draws a rectangle.
  * @param  Xpos X position
  * @param  Ypos Y position
  * @param  Width Rectangle width
  * @param  Height Rectangle height
  * @retval None
  */
void BSP_LCD_DrawRect(uint16_t Xpos, uint16_t Ypos, uint16_t Width, uint16_t Height)
{
  /* Draw horizontal lines */
  BSP_LCD_DrawHLine(Xpos, Ypos, Width);
  BSP_LCD_DrawHLine(Xpos, (Ypos+ Height), Width);

  /* Draw vertical lines */
  BSP_LCD_DrawVLine(Xpos, Ypos, Height);
  BSP_LCD_DrawVLine((Xpos + Width), Ypos, Height);
}

/**
  * @brief  Draws a circle.
  * @param  Xpos X position
  * @param  Ypos Y position
  * @param  Radius Circle radius
  * @retval None
  */
void BSP_LCD_DrawCircle(uint16_t Xpos, uint16_t Ypos, uint16_t Radius)
{
  int32_t  decision;       /* Decision Variable */
  uint32_t  curx;   /* Current X Value */
  uint32_t  cury;   /* Current Y Value */

  decision = 3 - (Radius << 1);
  curx = 0;
  cury = Radius;

  while (curx <= cury)
  {
    BSP_LCD_DrawPixel((Ypos + curx), (Xpos - cury), DrawProp.TextColor);

    BSP_LCD_DrawPixel((Ypos - curx), (Xpos - cury), DrawProp.TextColor);

    BSP_LCD_DrawPixel((Ypos + cury), (Xpos - curx), DrawProp.TextColor);

    BSP_LCD_DrawPixel((Ypos - cury), (Xpos - curx), DrawProp.TextColor);

    BSP_LCD_DrawPixel((Ypos + curx), (Xpos + cury), DrawProp.TextColor);

    BSP_LCD_DrawPixel((Ypos - curx), (Xpos + cury), DrawProp.TextColor);

    BSP_LCD_DrawPixel((Ypos + cury), (Xpos + curx), DrawProp.TextColor);

    BSP_LCD_DrawPixel((Ypos - cury), (Xpos + curx), DrawProp.TextColor);

    /* Initialize the font */
    BSP_LCD_SetFont(&LCD_DEFAULT_FONT);

    if (decision < 0)
    {
      decision += (curx << 2) + 6;
    }
    else
    {
      decision += ((curx - cury) << 2) + 10;
      cury--;
    }
    curx++;
  }
}

/**
  * @brief  Draws an poly-line (between many points).
  * @param  pPoints Pointer to the points array
  * @param  PointCount Number of points
  * @retval None
  */
void BSP_LCD_DrawPolygon(pPoint pPoints, uint16_t PointCount)
{
  int16_t x = 0, y = 0;

  if(PointCount < 2)
  {
    return;
  }

  BSP_LCD_DrawLine(pPoints->X, pPoints->Y, (pPoints+PointCount-1)->X, (pPoints+PointCount-1)->Y);

  while(--PointCount)
  {
    x = pPoints->X;
    y = pPoints->Y;
    pPoints++;
    BSP_LCD_DrawLine(x, y, pPoints->X, pPoints->Y);
  }

}

/**
  * @brief  Draws an ellipse on LCD.
  * @param  Xpos X position
  * @param  Ypos Y position
  * @param  XRadius Ellipse X radius
  * @param  YRadius Ellipse Y radius
  * @retval None
  */
void BSP_LCD_DrawEllipse(int Xpos, int Ypos, int XRadius, int YRadius)
{
  int x = 0, y = -XRadius, err = 2-2*YRadius, e2;
  float k = 0, rad1 = 0, rad2 = 0;

  rad1 = YRadius;
  rad2 = XRadius;

  k = (float)(rad2/rad1);

  do {
    BSP_LCD_DrawPixel((Ypos-(uint16_t)(x/k)), (Xpos+y), DrawProp.TextColor);
    BSP_LCD_DrawPixel((Ypos+(uint16_t)(x/k)), (Xpos+y), DrawProp.TextColor);
    BSP_LCD_DrawPixel((Ypos+(uint16_t)(x/k)), (Xpos-y), DrawProp.TextColor);
    BSP_LCD_DrawPixel((Ypos-(uint16_t)(x/k)), (Xpos-y), DrawProp.TextColor);

    e2 = err;
    if (e2 <= x) {
      err += ++x*2+1;
      if (-y == x && e2 <= y) e2 = 0;
    }
    if (e2 > y) err += ++y*2+1;
  }
  while (y <= 0);
}

/**
  * @brief  Draws a bitmap picture loaded in the internal Flash (32 bpp).
  * @param  Xpos Bmp X position in the LCD
  * @param  Ypos Bmp Y position in the LCD
  * @param  pBmp Pointer to Bmp picture address in the internal Flash
  * @retval None
  */
void BSP_LCD_DrawBitmap(uint16_t Xpos, uint16_t Ypos, uint8_t *pBmp)
{
  if(lcd_drv->DrawBitmap != NULL)
  {
    lcd_drv->DrawBitmap(Xpos, Ypos, pBmp);
  }
}

/**
  * @brief  Draws a full rectangle.
  * @param  Xpos X position
  * @param  Ypos Y position
  * @param  Width Rectangle width
  * @param  Height Rectangle height
  * @retval None
  */
void BSP_LCD_FillRect(uint16_t Xpos, uint16_t Ypos, uint16_t Width, uint16_t Height)
{
  BSP_LCD_SetTextColor(DrawProp.TextColor);
  do
  {
    BSP_LCD_DrawHLine(Xpos, Ypos++, Width);
  }
  while(Height--);
}

/**
  * @brief  Draws a full circle.
  * @param  Xpos X position
  * @param  Ypos Y position
  * @param  Radius Circle radius
  * @retval None
  */
void BSP_LCD_FillCircle(uint16_t Xpos, uint16_t Ypos, uint16_t Radius)
{
  int32_t  decision; /* Decision Variable */
  uint32_t  curx;    /* Current X Value */
  uint32_t  cury;    /* Current Y Value */

  decision = 3 - (Radius << 1);

  curx = 0;
  cury = Radius;

  BSP_LCD_SetTextColor(DrawProp.TextColor);

  while (curx <= cury)
  {
    if(cury > 0)
    {
      BSP_LCD_DrawVLine(Xpos + curx, Ypos - cury, 2*cury);
      BSP_LCD_DrawVLine(Xpos - curx, Ypos - cury, 2*cury);
    }

    if(curx > 0)
    {
      BSP_LCD_DrawVLine(Xpos - cury, Ypos - curx, 2*curx);
      BSP_LCD_DrawVLine(Xpos + cury, Ypos - curx, 2*curx);
    }
    if (decision < 0)
    {
      decision += (curx << 2) + 6;
    }
    else
    {
      decision += ((curx - cury) << 2) + 10;
      cury--;
    }
    curx++;
  }

  BSP_LCD_SetTextColor(DrawProp.TextColor);
  BSP_LCD_DrawCircle(Xpos, Ypos, Radius);
}

/**
  * @brief  Draws a full ellipse.
  * @param  Xpos X position
  * @param  Ypos Y position
  * @param  XRadius Ellipse X radius
  * @param  YRadius Ellipse Y radius
  * @retval None
  */
void BSP_LCD_FillEllipse(int Xpos, int Ypos, int XRadius, int YRadius)
{
  int x = 0, y = -XRadius, err = 2-2*YRadius, e2;
  float k = 0, rad1 = 0, rad2 = 0;

  rad1 = YRadius;
  rad2 = XRadius;

  k = (float)(rad2/rad1);

  do
  {
    BSP_LCD_DrawVLine((Xpos+y), (Ypos-(uint16_t)(x/k)), (2*(uint16_t)(x/k) + 1));
    BSP_LCD_DrawVLine((Xpos-y), (Ypos-(uint16_t)(x/k)), (2*(uint16_t)(x/k) + 1));

    e2 = err;
    if (e2 <= x)
    {
      err += ++x*2+1;
      if (-y == x && e2 <= y) e2 = 0;
    }
    if (e2 > y) err += ++y*2+1;
  }
  while (y <= 0);
}

/**
  * @brief  Enables the display.
  * @retval None
  */
void BSP_LCD_DisplayOn(void)
{
  lcd_drv->DisplayOn();
}

/**
  * @brief  Disables the display.
  * @retval None
  */
void BSP_LCD_DisplayOff(void)
{
  lcd_drv->DisplayOff();
}

/**
  * @brief  Draws a pixel on LCD.
  * @param  Xpos X position
  * @param  Ypos Y position
  * @param  RGBCode Pixel color in RGB mode (5-6-5)
  * @retval None
  */
void BSP_LCD_DrawPixel(uint16_t Xpos, uint16_t Ypos, uint16_t RGBCode)
{
  if(lcd_drv->WritePixel != NULL)
  {
    lcd_drv->WritePixel(Xpos, Ypos, RGBCode);
  }
}

/**
  * @brief  Refresh LCD.
  * @retval None
  */
void BSP_LCD_Refresh(void)
{
  ssd1315_Refresh();
}

/**
  * @}
  */

/******************************************************************************
                            Static Function
*******************************************************************************/
/** @addtogroup STM32G071B_DISCOVERY_LCD_Private_Functions
  * @{
  */

/**
  * @brief  Draws a character on LCD.
  * @param  Xpos Line where to display the character shape
  * @param  Ypos Start column address
  * @param  c Pointer to the character data
  * @retval None
  */
static void LCD_DrawChar(uint16_t Xpos, uint16_t Ypos, const uint8_t *c)
{
  uint32_t i = 0, j = 0;
  uint16_t height = 0, width = 0;
  uint8_t offset = 0;
  uint8_t *pchar = NULL;
  uint32_t line = 0;

  height = DrawProp.pFont->Height;
  width  = DrawProp.pFont->Width;

  offset =  8 *((width + 7)/8) -  width ;

  for(i = 0; i < height; i++)
  {
    pchar = ((uint8_t *)c + (width + 7)/8 * i);

    switch(((width + 7)/8))
    {
    case 1:
      line =  pchar[0];
      break;

    case 2:
      line =  (pchar[0]<< 8) | pchar[1];
      break;
      
    case 3:
    default:
      line =  (pchar[0]<< 16) | (pchar[1]<< 8) | pchar[2];
      break;
    }

    for (j = 0; j < width; j++)
    {
      if(line & (1 << (width- j + offset- 1)))
      {
        BSP_LCD_DrawPixel((Xpos + j), Ypos, DrawProp.TextColor);
      }
      else
      {
        BSP_LCD_DrawPixel((Xpos + j), Ypos, DrawProp.BackColor);
      }
    }
    Ypos++;
  }
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
