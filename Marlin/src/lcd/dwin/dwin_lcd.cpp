/**
 * Marlin 3D Printer Firmware
 * Copyright (c) 2020 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
 *
 * Based on Sprinter and grbl.
 * Copyright (c) 2011 Camiel Gubbels / Erik van der Zalm
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 *
 */

/********************************************************************************
 * @file     dwin_lcd.cpp
 * @author   LEO / Creality3D
 * @date     2019/07/18
 * @version  2.0.1
 * @brief    DWIN screen control functions
 ********************************************************************************/

#include "../../inc/MarlinConfigPre.h"

#if ENABLED(DWIN_CREALITY_LCD)

#include "../../inc/MarlinConfig.h"
#include "e3v2/dwin.h"
#include "e3v2/ui_position.h"

#include "dwin_lcd.h"
#include <string.h> // for memset

//#define DEBUG_OUT 1
#include "../../core/debug_out.h"

#include <QRCodeGenerator.h>

// Make sure DWIN_SendBuf is large enough to hold the largest string plus draw command and tail.
// Assume the narrowest (6 pixel) font and 2-byte gb2312-encoded characters.
uint8_t DWIN_SendBuf[11 + DWIN_WIDTH / 6 * 2] = { 0xAA };
uint8_t DWIN_BufTail[4] = { 0xCC, 0x33, 0xC3, 0x3C };
uint8_t databuf[26] = { 0 };
uint8_t receivedType;

int recnum = 0;

inline void DWIN_Byte(size_t &i, const uint16_t bval) {
  DWIN_SendBuf[++i] = bval;
}

inline void DWIN_Word(size_t &i, const uint16_t wval) {
  DWIN_SendBuf[++i] = wval >> 8;
  DWIN_SendBuf[++i] = wval & 0xFF;
}

inline void DWIN_Long(size_t &i, const uint32_t lval) {
  DWIN_SendBuf[++i] = (lval >> 24) & 0xFF;
  DWIN_SendBuf[++i] = (lval >> 16) & 0xFF;
  DWIN_SendBuf[++i] = (lval >>  8) & 0xFF;
  DWIN_SendBuf[++i] = lval & 0xFF;
}

inline void DWIN_String(size_t &i, char * const string) {
  const size_t len = _MIN(sizeof(DWIN_SendBuf) - i, strlen(string));
  memcpy(&DWIN_SendBuf[i+1], string, len);
  i += len;
}

inline void DWIN_String(size_t &i, const __FlashStringHelper * string) {
  if (!string) return;
  const size_t len = strlen_P((PGM_P)string); // cast it to PGM_P, which is basically const char *, and measure it using the _P version of strlen.
  if (len == 0) return;
  memcpy(&DWIN_SendBuf[i+1], string, len);
  i += len;
}

// Send the data in the buffer and the packet end
inline void DWIN_Send(size_t &i) {
  ++i;
  LOOP_L_N(n, i) { LCD_SERIAL.write(DWIN_SendBuf[n]); delayMicroseconds(1); }
  LOOP_L_N(n, 4) { LCD_SERIAL.write(DWIN_BufTail[n]); delayMicroseconds(1); }
}

/*-------------------------------------- System variable function --------------------------------------*/

// Handshake (1: Success, 0: Fail)
bool DWIN_Handshake(void)
{
  #ifndef LCD_BAUDRATE
    #define LCD_BAUDRATE 115200
  #endif
  LCD_SERIAL.begin(LCD_BAUDRATE);
  const millis_t serial_connect_timeout = millis() + 1000UL;
  while (!LCD_SERIAL.connected() && PENDING(millis(), serial_connect_timeout)) { /*nada*/ }

  size_t i = 0;
  DWIN_Byte(i, 0x00);
  DWIN_Send(i);

  while (LCD_SERIAL.available() > 0 && recnum < (signed)sizeof(databuf))
  {
    databuf[recnum] = LCD_SERIAL.read();
    // ignore the invalid data
    if (databuf[0] != FHONE)
    {
      // prevent the program from running.
      if (recnum > 0)
      {
        recnum = 0;
        ZERO(databuf);
      }
      continue;
    }
    delay(10);
    recnum++;
  }

  return ( recnum >= 3
        && databuf[0] == FHONE
        && databuf[1] == '\0'
        && databuf[2] == 'O'
        && databuf[3] == 'K' );
}

// Set the backlight luminance
//  luminance: (0x00-0xFF)
void DWIN_Backlight_SetLuminance(const uint8_t luminance) 
{
  size_t i = 0;
  DWIN_Byte(i, 0x5f);
  DWIN_Byte(i, luminance); //_MAX(luminance, 0x1F));
  DWIN_Send(i);
}

// Set screen display direction
//  dir: 0=0°, 1=90°, 2=180°, 3=270°
void DWIN_Frame_SetDir(uint8_t dir)
{
  /*
    size_t i = 0;
    DWIN_Byte(i, 0x34);
    DWIN_Byte(i, 0x5A);
    DWIN_Byte(i, 0xA5);
    DWIN_Byte(i, dir);
    DWIN_Send(i);
  */
}

// Update display
void DWIN_UpdateLCD(void)
{
  size_t i = 0;
  DWIN_Byte(i, 0x3D);
  DWIN_Send(i);
}

/*---------------------------------------- Drawing functions ----------------------------------------*/

// Clear screen
//  color: Clear screen color
void DWIN_Frame_Clear(const uint16_t color) {
  size_t i = 0;
  DWIN_Byte(i, 0x01);
  DWIN_Word(i, color);
  DWIN_Send(i);
}

// Draw a point
//  width: point width   0x01-0x0F
//  height: point height 0x01-0x0F
//  x,y: upper left point
void DWIN_Draw_Point(uint8_t width, uint8_t height, uint16_t x, uint16_t y) {
  size_t i = 0;
  DWIN_Byte(i, 0x02);
  DWIN_Byte(i, width);
  DWIN_Byte(i, height);
  DWIN_Word(i, x);
  DWIN_Word(i, y);
  DWIN_Send(i);
}
void DWIN_Set_Color(uint16_t FC,uint16_t BC)
{
  size_t i = 0;
  DWIN_Byte(i, 0x40);
  DWIN_Word(i, FC);  //前景色
  DWIN_Word(i, BC);  //背景色
  DWIN_Send(i);
}

void DWIN_Set_24_Color(uint32_t BC)
{
  size_t i = 0;
  DWIN_Byte(i, 0x40); 
  DWIN_Byte(i, BC>>16); //背景色
  DWIN_Byte(i, BC>>8);
  DWIN_Byte(i, BC);
  DWIN_Byte(i, 255);  //前景色
  DWIN_Byte(i, 255);
  DWIN_Byte(i, 255);
  DWIN_Send(i);
}

// Draw a line
//  color: Line segment color
//  xStart/yStart: Start point
//  xEnd/yEnd: End point
void DWIN_Draw_Line(uint16_t color, uint16_t xStart, uint16_t yStart, uint16_t xEnd, uint16_t yEnd) 
{
  size_t i = 0;
  DWIN_Set_Color(color, 0xffff);
  DWIN_Byte(i, 0x56);
  DWIN_Word(i, xStart);
  DWIN_Word(i, yStart);
  DWIN_Word(i, xEnd);
  DWIN_Word(i, yEnd);
  DWIN_Send(i);
}

// Draw a rectangle
//  mode: 0=frame, 1=fill, 2=XOR fill
//  color: Rectangle color
//  xStart/yStart: upper left point
//  xEnd/yEnd: lower right point
void DWIN_Draw_Rectangle(uint8_t mode, uint16_t color,
                         uint16_t xStart, uint16_t yStart, uint16_t xEnd, uint16_t yEnd)
{
  size_t i = 0;
  uint8_t temp_mode = 0;
  switch (mode)
  {
    case 0:
      temp_mode = 0x59;
      break;
     case 1:
      temp_mode = 0x5B;
      break;
     case 2:
      temp_mode = 0x69; //背景色显示矩形区域
      break;
     case 3:
      temp_mode = 0x5A; //背景色填充矩形区域
      break;
    default:
      break;
  }
  if(xEnd >= DWIN_WIDTH)
    xEnd = DWIN_WIDTH - 1;
  DWIN_Set_Color(color,0xffff);
  i = 0;
  DWIN_Byte(i, temp_mode);
  DWIN_Word(i, xStart);
  DWIN_Word(i, yStart);
  DWIN_Word(i, xEnd);
  DWIN_Word(i, yEnd);
  DWIN_Send(i);
}

// Move a screen area
//  mode: 0, circle shift; 1, translation
//  dir: 0=left, 1=right, 2=up, 3=down
//  dis: Distance
//  color: Fill color
//  xStart/yStart: upper left point
//  xEnd/yEnd: bottom right point
void DWIN_Frame_AreaMove(uint8_t mode, uint8_t dir, uint16_t dis,
                         uint16_t color, uint16_t xStart, uint16_t yStart, uint16_t xEnd, uint16_t yEnd) {
  size_t i = 0;
  if(xEnd==DWIN_WIDTH) xEnd-=1;
  DWIN_Byte(i, 0x09);
  DWIN_Byte(i, (mode << 7) | dir);
  DWIN_Word(i, dis);
  DWIN_Word(i, color);
  DWIN_Word(i, xStart);
  DWIN_Word(i, yStart);
  DWIN_Word(i, xEnd);
  DWIN_Word(i, yEnd);
  DWIN_Send(i);
}

// uint16_t color,uint8_t width,uint8_t x_step,uint16_t y_ratio     通道  固定  XsYs      XeYe      颜色    粗细  X步长  Y=0位置 Y数据比例
// AA 84 00    FF    0010 0020 0074 00C8 00F800  01    05     00C8    0100      CC33C33C 
void Draw_Curve_Set(uint8_t line_wide,uint8_t step_x,uint16_t step_y,uint32_t colour)
{
  size_t i = 0;
  DWIN_Byte(i, 0x84);
  DWIN_Byte(i, 0x00);
  DWIN_Byte(i, 0xFF);
  DWIN_Word(i, Curve_Psition_Start_X); //left_up_x
  DWIN_Word(i, Curve_Psition_Start_Y); //left_up_y
  DWIN_Word(i, Curve_Psition_End_X);  //right_down_x
  DWIN_Word(i, Curve_Psition_End_Y); //right_down_y

  DWIN_Byte(i, colour>>16);
  DWIN_Word(i, colour);    //颜色

  DWIN_Byte(i,line_wide);  //粗细
  DWIN_Byte(i, step_x);   //x步长
  DWIN_Word(i, Curve_Zero_Y);
  DWIN_Word(i, step_y);
  DWIN_Send(i);
}

void Draw_Curve_Data(uint8_t index,int16_t* temp_data)
{
  size_t i = 0;
  DWIN_Byte(i, 0x84);
  DWIN_Byte(i, 0x00);//通道
  DWIN_Byte(i, 0x00);//固定
  //数据
  for(uint8_t num=0;num<index;num++)
  {
    DWIN_Word(i, *(temp_data+num));
  }
  DWIN_Send(i);
}
/*---------------------------------------- Text related functions ----------------------------------------*/

// Draw a string
//  widthAdjust: true=self-adjust character width; false=no adjustment
//  bShow: true=display background color; false=don't display background color
//  size: Font size
//  color: Character color
//  bColor: Background color
//  x/y: Upper-left coordinate of the string
//  *string: The string
void DWIN_Draw_String(bool widthAdjust, bool bShow, uint8_t size,
                      uint16_t color, uint16_t bColor, uint16_t x, uint16_t y, char *string)
{
  size_t i = 0;
  char mode;
  if(size == font10x20)
  {
    size = font8x16;
  }
  DWIN_Byte(i, 0x98);
  DWIN_Word(i, x);
  DWIN_Word(i, y);
  DWIN_Byte(i, 0);  //字库

  if(bShow)
  {
    // 显示背景;
    mode = 0x40;
  }
  else
  {
    mode = 0;
  }

  // GBK编码
  mode |= 0x02;

  DWIN_Byte(i, mode);
  DWIN_Byte(i, size);
  DWIN_Word(i, color);
  DWIN_Word(i, bColor);
  DWIN_String(i, string);
  DWIN_Send(i);
}

void DWIN_Draw_MultilineString(bool widthAdjust, bool bShow, uint8_t size, uint16_t color, uint16_t bColor, uint16_t x, uint16_t y, uint8_t char_limit, uint8_t line_height, const char *text)
{
  const char *ptr = text;
  char currentLine[char_limit + 1];
  size_t currentLength = 0;
  uint8_t line = 0;

  while (*ptr)
  {
    // Find the next word
    const char *start = ptr;
    while (*ptr && !isspace(*ptr))
    {
      ptr++;
    }
    size_t wordLength = ptr - start;

    // Check if the current word can fit in the current line
    if (currentLength + wordLength + (currentLength > 0 ? 1 : 0) <= char_limit)
    {
      // Add a space before the word if the current line is not empty
      if (currentLength > 0)
      {
        currentLine[currentLength++] = ' ';
      }
      // Copy the word into the current line
      memcpy(currentLine + currentLength, start, wordLength);
      currentLength += wordLength;
    }
    else
    {
      // Print the current line
      currentLine[currentLength] = '\0';

      DWIN_Draw_String(widthAdjust, bShow, size, color, bColor, x, y + (line_height * line), currentLine);
      line++;

      // Start a new line with the current word
      if (wordLength > char_limit)
      {
        // If the word itself is too long, split it
        for (size_t i = 0; i < wordLength; i += char_limit)
        {
          size_t len = (i + char_limit > wordLength) ? (wordLength - i) : char_limit;
          memcpy(currentLine, start + i, len);
          currentLine[len] = '\0';
          DWIN_Draw_String(widthAdjust, bShow, size, color, bColor, x, y + (line_height * line), currentLine);
          line++;
        }
        currentLength = 0;
      }
      else
      {
        // Otherwise, start a new line with the current word
        memcpy(currentLine, start, wordLength);
        currentLength = wordLength;
      }
    }

    // Skip spaces
    while (*ptr && isspace(*ptr))
    {
      ptr++;
    }
  }

  // Print the remaining part of the current line
  if (currentLength > 0)
  {
    currentLine[currentLength] = '\0';
    DWIN_Draw_String(widthAdjust, bShow, size, color, bColor, x, y + (line_height * line), currentLine);
  }
}

void DWIN_SHOW_MAIN_PIC()
{
  size_t i = 0;
  DWIN_Byte(i, 0x70);
  DWIN_Word(i, 0);
  DWIN_Send(i);
}
// Draw a positive integer
//  bShow: true=display background color; false=don't display background color
//  zeroFill: true=zero fill; false=no zero fill
//  zeroMode: 1=leading 0 displayed as 0; 0=leading 0 displayed as a space
//  size: Font size
//  color: Character color
//  bColor: Background color
//  iNum: Number of digits
//  x/y: Upper-left coordinate
//  value: Integer value
void DWIN_Draw_IntValue(uint8_t bShow, bool zeroFill, uint8_t zeroMode, uint8_t size, uint16_t color,
                          uint16_t bColor, uint8_t iNum, uint16_t x, uint16_t y, uint16_t value)
{
  size_t i = 0;
  if(size == font10x20)
  {
    size = font8x16;
  }
  DWIN_Byte(i, 0x14);
  // Bit 7: bshow
  // Bit 6: 1 = signed; 0 = unsigned number;
  // Bit 5: zeroFill
  // Bit 4: zeroMode
  // Bit 3-0: size
  DWIN_Byte(i, (bShow * 0x80) | (zeroFill * 0x20) | (zeroMode * 0x10) | size);
  DWIN_Word(i, color);
  DWIN_Word(i, bColor);
  DWIN_Byte(i, iNum);
  DWIN_Byte(i, 0); // fNum
  DWIN_Word(i, x);
  DWIN_Word(i, y);
  #if 0
    for (char count = 0; count < 8; count++)
    {
      DWIN_Byte(i, value);
      value >>= 8;
      if (!(value & 0xFF)) break;
    }
  #else
    // Write a big-endian 64 bit integer
    const size_t p = i + 1;
    for (char count = 8; count--;) { // 7..0
      ++i;
      DWIN_SendBuf[p + count] = value;
      value >>= 8;
    }
  #endif
  // DWIN_Draw_String(true, true,size,color, bColor, x, y, cmd);
  DWIN_Send(i);
}

void DWIN_Draw_IntValue_N0SPACE(uint8_t bShow, bool zeroFill, uint8_t zeroMode, uint8_t size, uint16_t color,
                          uint16_t bColor, uint8_t iNum, uint16_t x, uint16_t y, uint16_t value)
{
  size_t i = 0;
  if(size == font10x20)
  {
    size = font8x16;
  }
  DWIN_Byte(i, 0x15);
  // Bit 7: bshow
  // Bit 6: 1 = signed; 0 = unsigned number;
  // Bit 5: zeroFill
  // Bit 4: zeroMode
  // Bit 3-0: size
  DWIN_Byte(i, (bShow * 0x80) | (zeroFill * 0x20) | (zeroMode * 0x10) | size);
  DWIN_Word(i, color);
  DWIN_Word(i, bColor);
  DWIN_Byte(i, iNum);
  DWIN_Byte(i, 0); // fNum
  DWIN_Word(i, x);
  DWIN_Word(i, y);
  #if 0
    for (char count = 0; count < 8; count++)
    {
      DWIN_Byte(i, value);
      value >>= 8;
      if (!(value & 0xFF)) break;
    }
  #else
    // Write a big-endian 64 bit integer
    const size_t p = i + 1;
    for (char count = 8; count--;) { // 7..0
      ++i;
      DWIN_SendBuf[p + count] = value;
      value >>= 8;
    }
  #endif
  // DWIN_Draw_String(true, true,size,color, bColor, x, y, cmd);
  DWIN_Send(i);
}
// Draw a floating point number
//  bShow: true=display background color; false=don't display background color
//  zeroFill: true=zero fill; false=no zero fill
//  zeroMode: 1=leading 0 displayed as 0; 0=leading 0 displayed as a space
//  size: Font size
//  color: Character color
//  bColor: Background color
//  iNum: Number of whole digits
//  fNum: Number of decimal digits
//  x/y: Upper-left point
//  value: Float value
void DWIN_Draw_FloatValue(uint8_t bShow, bool zeroFill, uint8_t zeroMode, uint8_t size, uint16_t color,
                            uint16_t bColor, uint8_t iNum, uint8_t fNum, uint16_t x, uint16_t y, long value)
{
  //uint8_t *fvalue = (uint8_t*)&value;
  size_t i = 0;
  if(size == font10x20)
  {
    size = font8x16;
  }
  DWIN_Byte(i, 0x14);
  DWIN_Byte(i, (bShow * 0x80) | (zeroFill * 0x20) | (zeroMode * 0x10) | size);
  DWIN_Word(i, color);
  DWIN_Word(i, bColor);
  DWIN_Byte(i, iNum);
  DWIN_Byte(i, fNum);
  DWIN_Word(i, x);
  DWIN_Word(i, y);
  DWIN_Long(i, value);
  /*
  DWIN_Byte(i, fvalue[3]);
  DWIN_Byte(i, fvalue[2]);
  DWIN_Byte(i, fvalue[1]);
  DWIN_Byte(i, fvalue[0]);
  */
  DWIN_Send(i);
}

void DWIN_Draw_qrcode(uint16_t topLeftX, uint16_t topLeftY, const uint8_t moduleSize, const char *qrcode_data) {
  // The structure to manage the QR code
  QRCode qrcode;

  // QR version 2 allows strings up to 47 uppercase chars, e.g. "https://bit.ly/qwertyuiop_asdfghjkl_zxcvbnm_123"
  char uppercase_data[48];

  size_t i = 0;
  while (qrcode_data[i] && i < sizeof(uppercase_data) - 1) {
      uppercase_data[i] = toupper(qrcode_data[i]);
      i++;
  }
  uppercase_data[i] = '\0'; // Null-terminate the new string

  // Allocate a chunk of memory to store the QR code
  uint8_t qrcodeBytes[qrcode_getBufferSize(QR_VERSION)];

  qrcode_initText(&qrcode, qrcodeBytes, QR_VERSION, ECC_LOW, uppercase_data);

  DWIN_Draw_Rectangle(1, Color_White, topLeftX, topLeftY, topLeftX + (qrcode.size + QUIET_ZONE_SIZE * 2) * moduleSize, topLeftY + (qrcode.size + QUIET_ZONE_SIZE * 2) * moduleSize);
  topLeftX = topLeftX + moduleSize * QUIET_ZONE_SIZE;
  topLeftY = topLeftY + moduleSize * QUIET_ZONE_SIZE;

  // top left position marker
  DWIN_Draw_Rectangle(1, Color_Bg_Black, topLeftX, topLeftY, topLeftX + moduleSize * 7, topLeftY + moduleSize * 7);
  DWIN_Draw_Rectangle(1, Color_White, topLeftX + moduleSize * 1, topLeftY + moduleSize * 1, topLeftX + moduleSize * 6, topLeftY + moduleSize * 6);
  DWIN_Draw_Rectangle(1, Color_Bg_Black, topLeftX + moduleSize * 2, topLeftY + moduleSize * 2, topLeftX + moduleSize * 5, topLeftY + moduleSize * 5);
  // top right position marker
  DWIN_Draw_Rectangle(1, Color_Bg_Black, topLeftX + moduleSize * (qrcode.size - 7), topLeftY, topLeftX + moduleSize * qrcode.size, topLeftY + moduleSize * 7);
  DWIN_Draw_Rectangle(1, Color_White, topLeftX + moduleSize * (qrcode.size - 6), topLeftY + moduleSize * 1, topLeftX + moduleSize * (qrcode.size - 1), topLeftY + moduleSize * 6);
  DWIN_Draw_Rectangle(1, Color_Bg_Black, topLeftX + moduleSize * (qrcode.size - 5), topLeftY + moduleSize * 2, topLeftX + moduleSize * (qrcode.size - 2), topLeftY + moduleSize * 5);
  // // bottom left position marker
  DWIN_Draw_Rectangle(1, Color_Bg_Black, topLeftX, topLeftY + moduleSize * (qrcode.size - 7), topLeftX + moduleSize * 7, topLeftY + moduleSize * qrcode.size);
  DWIN_Draw_Rectangle(1, Color_White, topLeftX + moduleSize * 1, topLeftY + moduleSize * (qrcode.size - 6), topLeftX + moduleSize * 6, topLeftY + moduleSize * (qrcode.size - 1));
  DWIN_Draw_Rectangle(1, Color_Bg_Black, topLeftX + moduleSize * 2, topLeftY + moduleSize * (qrcode.size - 5), topLeftX + moduleSize * 5, topLeftY + moduleSize * (qrcode.size - 2));

  #define QR_POSITIONING_SIZE 8
  for (uint8_t y = 0; y < qrcode.size; y++) {
      for (uint8_t x = 0; x < qrcode.size; x++) {
        // skip top left and bottom left position markers
        if (x < QR_POSITIONING_SIZE && (y < QR_POSITIONING_SIZE || y > (qrcode.size - QR_POSITIONING_SIZE - 1))) {
          continue;
        }
        // skip top right position marker
        if (x > (qrcode.size - QR_POSITIONING_SIZE - 1) && y < QR_POSITIONING_SIZE) {
          continue;
        }
        if (qrcode_getModule(&qrcode, x, y)) {
          DWIN_Draw_Rectangle(
            1,
            Color_Bg_Black,
            topLeftX + moduleSize * x,
            topLeftY + moduleSize * y,
            topLeftX + moduleSize * (x + 1),
            topLeftY + moduleSize * (y + 1)
          );
        }
      }
      delay(10);
  }
}

void DWIN_Draw_qrcode(const uint16_t topLeftX, const uint16_t topLeftY, const uint8_t moduleSize, const __FlashStringHelper *qrcode_data) {
  DWIN_Draw_qrcode(topLeftX, topLeftY, moduleSize, (const char *)qrcode_data);
}

/*---------------------------------------- Picture related functions ----------------------------------------*/

// Draw JPG and cached in #0 virtual display area
// id: Picture ID
// void DWIN_JPG_ShowAndCache(const uint8_t id)
// {
//   size_t i = 0;
//   DWIN_Word(i, 0x2200);
//   DWIN_Byte(i, id);
//   DWIN_Send(i);     // AA 23 00 00 00 00 08 00 01 02 03 CC 33 C3 3C
// }

// Draw an Icon
//  libID: Icon library ID
//  picID: Icon ID
//  x/y: Upper-left point
void DWIN_ICON_Not_Filter_Show(uint8_t libID, uint8_t picID, uint16_t x, uint16_t y) 
{
  NOMORE(x, DWIN_WIDTH - 1);
  NOMORE(y, DWIN_HEIGHT - 1); // -- ozy -- srl
  size_t i = 0;

  DWIN_Byte(i, 0x97);
  DWIN_Word(i, x);
  DWIN_Word(i, y);
  DWIN_Byte(i, libID);
  DWIN_Byte(i, 0x01);
  DWIN_Word(i, picID);
  DWIN_Send(i);
}

// Draw an Icon
// libID: Icon library ID
// picID: Icon ID
// x/y: Upper-left point
void DWIN_ICON_Show(uint8_t libID, uint8_t picID, uint16_t x, uint16_t y)
{
  NOMORE(x, DWIN_WIDTH - 1);
  // -- ozy -- srl
  NOMORE(y, DWIN_HEIGHT - 1);
  size_t i = 0;
  DWIN_Byte(i, 0x97);
  DWIN_Word(i, x);
  DWIN_Word(i, y);
  DWIN_Byte(i, libID);
  DWIN_Byte(i, 0x00);
  DWIN_Word(i, picID);
  DWIN_Send(i);
}

// Unzip the JPG picture to a virtual display area
//  n: Cache index
//  id: Picture ID
// void DWIN_JPG_CacheToN(uint8_t n, uint8_t id)
// {
//   size_t i = 0;
//   DWIN_Byte(i, 0x25);
//   DWIN_Byte(i, n);
//   DWIN_Byte(i, id);
//   DWIN_Send(i);
// }

// Copy area from virtual display area to current screen
//  cacheID: virtual area number
//  xStart/yStart: Upper-left of virtual area
//  xEnd/yEnd: Lower-right of virtual area
//  x/y: Screen paste point
void DWIN_Frame_AreaCopy(uint8_t cacheID, uint16_t xStart, uint16_t yStart,
                         uint16_t xEnd, uint16_t yEnd, uint16_t x, uint16_t y) {
  size_t i = 0;
  /*
  DWIN_Byte(i, 0x27);
  DWIN_Byte(i, 0x80 | cacheID);
  DWIN_Word(i, xStart);
  DWIN_Word(i, yStart);
  DWIN_Word(i, xEnd);
  DWIN_Word(i, yEnd);
  DWIN_Word(i, x);
  DWIN_Word(i, y);
  DWIN_Send(i);
  */
  DWIN_Byte(i, 0x71);
  DWIN_Byte(i, cacheID);
  DWIN_Word(i, xStart);
  DWIN_Word(i, yStart);
  DWIN_Word(i, xEnd);
  DWIN_Word(i, yEnd);
  DWIN_Word(i, x);
  DWIN_Word(i, y);
  DWIN_Send(i);
}

// Animate a series of icons
//  animID: Animation ID; 0x00-0x0F
//  animate: true on; false off;
//  libID: Icon library ID
//  picIDs: Icon starting ID
//  picIDe: Icon ending ID
//  x/y: Upper-left point
//  interval: Display time interval, unit 10mS
void DWIN_ICON_Animation(uint8_t animID, bool animate, uint8_t libID, uint8_t picIDs, uint8_t picIDe, uint16_t x, uint16_t y, uint16_t interval) {
  NOMORE(x, DWIN_WIDTH - 1);
  NOMORE(y, DWIN_HEIGHT - 1); // -- ozy -- srl
  size_t i = 0;
  DWIN_Byte(i, 0x28);
  DWIN_Word(i, x);
  DWIN_Word(i, y);
  // Bit 7: animation on or off
  // Bit 6: start from begin or end
  // Bit 5-4: unused (0)
  // Bit 3-0: animID
  DWIN_Byte(i, (animate * 0x80) | 0x40 | animID);
  DWIN_Byte(i, libID);
  DWIN_Byte(i, picIDs);
  DWIN_Byte(i, picIDe);
  DWIN_Byte(i, interval);
  DWIN_Send(i);
}

// Animation Control
//  state: 16 bits, each bit is the state of an animation id
void DWIN_ICON_AnimationControl(uint16_t state) {
  size_t i = 0;
  DWIN_Byte(i, 0x28);
  DWIN_Word(i, state);
  DWIN_Send(i);
}

// void DWIN_ICON_WR_SRAM(uint16_t data)
// {
//   size_t i = 0;
//   DWIN_Byte(i, 0x31);
//   DWIN_Byte(i, 0x5A);
//   DWIN_Word(i, data);
//   DWIN_Send(i);
// }

// void DWIN_ICON_SHOW_SRAM(uint16_t x,uint16_t y,uint16_t addr)
// {
//   size_t i = 0;
//   DWIN_Byte(i, 0xC1);
//   DWIN_Byte(i, 0x12);
//   DWIN_Word(i, x);
//   DWIN_Word(i, y);
//   DWIN_Byte(i, 0);
//   DWIN_Word(i, addr);
//   DWIN_Send(i);
// }

/*---------------------------------------- Memory functions ----------------------------------------*/
// The LCD has an additional 32KB SRAM and 16KB Flash

// Data can be written to the sram and save to one of the jpeg page files

// Write Data Memory
//  command 0x31
//  Type: Write memory selection; 0x5A=SRAM; 0xA5=Flash
//  Address: Write data memory address; 0x000-0x7FFF for SRAM; 0x000-0x3FFF for Flash
//  Data: data
//
//  Flash writing returns 0xA5 0x4F 0x4B

// Read Data Memory
//  command 0x32
//  Type: Read memory selection; 0x5A=SRAM; 0xA5=Flash
//  Address: Read data memory address; 0x000-0x7FFF for SRAM; 0x000-0x3FFF for Flash
//  Length: leangth of data to read; 0x01-0xF0
//
//  Response:
//    Type, Address, Length, Data

// Write Picture Memory
//  Write the contents of the 32KB SRAM data memory into the designated image memory space
//  Issued: 0x5A, 0xA5, PIC_ID
//  Response: 0xA5 0x4F 0x4B
//
//  command 0x33
//  0x5A, 0xA5
//  PicId: Picture Memory location, 0x00-0x0F
//
//  Flash writing returns 0xA5 0x4F 0x4B

#endif // DWIN_CREALITY_LCD
