// //#include <wstring.h>
// #include <stdio.h>
// #include <stdlib.h>
// #include <string.h>
// #include <Arduino.h>
// #include "ui_dacai.h"
// // #include <libmaple/usart.h>
// #include "../../MarlinCore.h"
// #include "../../inc/MarlinConfig.h"
// #include "../../core/serial.h"
// #include "../../core/macros.h"
// #include "../../../Configuration.h"
// #include "dwin_lcd.h"

// #ifdef LCD_SERIAL_PORT
//   #define LCDSERIAL LCD_SERIAL
// #elif SERIAL_PORT_2
//   #define LCDSERIAL LCDSERIAL
// #endif

// uint8_t IsDacaiScreenConnect = true;
// char unitBuf[4][10] = {"", "%", "mm", "℃"};

// //#if ENABLED(UI_SHOW_t)
// inline uint16_t work16(const uint8_t high, const uint8_t low) { return (high) << 8U | (low); }

// UI_SHOW_t uiShow;

// // dwin模块初始化(串口号为LCD_SERIAL_PORT)
// void UI_SHOW_t::RTS_Init(void)
// {
//   #ifndef LCD_BAUDRATE
//     #define LCD_BAUDRATE 115200
//   #endif
//   SERIAL_ECHO("lcd serial init");
//   LCDSERIAL.begin(LCD_BAUDRATE);
//   const millis_t serial_connect_timeout = millis() + 1000UL;
//   while (!LCDSERIAL.connected() && PENDING(millis(), serial_connect_timeout)) { /*nada*/ }
//   delay(500); // 延时等待屏幕准备好

//   for (uint8_t i = 0; i < 5; i++)
//   {
//     if (UI_Handshake())
//     {
//       SERIAL_ECHO("\r\n RTS_Init OK!!!");
//       IsDacaiScreenConnect = true;
//       break;
//     }
//     else
//     {
//       SERIAL_ECHO("\r\n RTS_Init failse!!!");
//       IsDacaiScreenConnect = false;
//       continue;
//     }
//   }
// }

// UI_SHOW_t::UI_SHOW_t()
// {
//   recdat.head = CMD_HEAD;

//   frameEnd[0] = FBONE;
//   frameEnd[1] = FBTWO;
//   frameEnd[2] = FBTHREE;
//   frameEnd[3] = FBFOUR;

//   memset(databuf,0, sizeof(databuf));
// }

// static inline void dwin_uart_write(unsigned char *buf, int len)
// {
//   LOOP_L_N(n, len) { LCDSERIAL.write(buf[n]); }
// }

// void UI_SHOW_t::RTS_SndData(void)
// {
//   if(snddat.head == CMD_HEAD)
//   {
//     databuf[0] = snddat.head;
//     databuf[1] = snddat.cmd_type;   // 命令类型
//     databuf[2] = snddat.ctrl_msg;   // 消息类型
//     // 表示上传的组态控件指令
//     if (/*work16(snddat.cmd_type, snddat.ctrl_msg) == CMD_SEND_TO_SCREEN && */snddat.bytelen >= 7)
//     {
//       databuf[3] = snddat.screen_id >> 8;
//       databuf[4] = snddat.screen_id & 0x00FF;
//       databuf[5] = snddat.control_id >> 8;
//       databuf[6] = snddat.control_id & 0xFF;
//       if ((snddat.bytelen - 7) != 0)
//       {
//         memcpy(&databuf[7], snddat.data, (snddat.bytelen - 7));
//       }
//       // 增加帧尾 
//       memcpy(&databuf[7 + (snddat.bytelen - 7)], frameEnd, 4);
//     }

//     dwin_uart_write(databuf, snddat.bytelen + 4);

//     memset(&snddat, 0, sizeof(snddat));
//     memset(databuf, 0, sizeof(databuf));
//     snddat.head = CMD_HEAD;
//   }
// }

// void UI_SHOW_t::RTS_SndData(const String &s, unsigned long _screen_id, unsigned long _control_id, unsigned long cmd /*= VarAddr_W*/)
// {
//   if(s.length() < 1)
//   {
//     return;
//   }
//   RTS_SndData(s.c_str(), _screen_id, _control_id, cmd);
// }

// void UI_SHOW_t::RTS_SndData(const char *str, unsigned long _screen_id, unsigned long _control_id, unsigned long cmd/*= VarAddr_W*/)
// {
//   int len = strlen(str);
//   int i = 0;
//   if( len >= 0)
//   {
//     databuf[0] = CMD_HEAD;
//     databuf[1] = cmd >> 8;     // 命令类型
//     databuf[2] = cmd & 0x00FF; // 消息类型
//     databuf[3] = _screen_id >> 8;
//     databuf[4] = _screen_id & 0x00FF;
//     databuf[5] = _control_id >> 8;
//     databuf[6] = _control_id & 0xFF;

//     for(i = 0; i < len ; i++)
//     {
//       databuf[7 + i] = str[i];
//     }
//     // 增加帧尾
//     memcpy(&databuf[7 + i], frameEnd, 4);

//     dwin_uart_write(databuf, len + 11);

//     memset(databuf, 0, sizeof(databuf));
//   }
// }

// void UI_SHOW_t::RTS_SndData(char c, unsigned long _screen_id, unsigned long _control_id, unsigned long cmd/*= VarAddr_W*/)
// {
//   snddat.head = CMD_HEAD;
//   snddat.cmd_type = cmd >> 8;     // 命令类型
//   snddat.ctrl_msg = cmd & 0x00FF; // 消息类型
//   snddat.screen_id = _screen_id;
//   snddat.control_id = _control_id;
//   snddat.data[0] = c;
//   snddat.bytelen = 8;
//   RTS_SndData();
// }

// void UI_SHOW_t::RTS_SndData(unsigned char* str, unsigned long addr, unsigned long addr1, unsigned long cmd)
// {
//   RTS_SndData((char *)str, addr, addr1, cmd);
// }

// void UI_SHOW_t::RTS_SndData(int n, unsigned long _screen_id, unsigned long _control_id, unsigned long cmd/*= VarAddr_W*/)
// {
//   snddat.head = CMD_HEAD;
//   snddat.cmd_type = cmd >> 8;     // 命令类型 
//   snddat.ctrl_msg = cmd & 0x00FF; // 消息类型
//   snddat.screen_id = _screen_id;
//   snddat.control_id = _control_id;

//   if (cmd == CMD_ICON_SET_FRAME || cmd == CMD_ICON_SHOW_HIDE)
//   {
//     snddat.data[0] = n & 0xFF;
//     snddat.bytelen = 8;
//   }
//   else
//   {
//     // if(cmd == CMD_SEND_TO_SCREEN)
//     if(n > 0xFFFF)
//     {
//       snddat.data[0] = n >> 24;
//       snddat.data[1] = (n >> 16) & 0xFF;
//       snddat.data[2] = (n >> 8) & 0xFF;
//       snddat.data[3] = n & 0xFF;
//       snddat.bytelen = 11;
//     }
//     else
//     {
//       snddat.data[0] = (n >> 8) & 0xFF;
//       snddat.data[1] = n & 0xFF;
//       snddat.bytelen = 9;
//     }
//   }
//   RTS_SndData();
// }

// void UI_SHOW_t::RTS_SndData(unsigned int n, unsigned long addr, unsigned long addr1, unsigned long cmd)
// {
//   RTS_SndData((int)n, addr, addr1, cmd);
// }

// void UI_SHOW_t::RTS_SndData(float n, unsigned long addr, unsigned long addr1, unsigned long cmd)
// {
//   RTS_SndData((int)n, addr, addr1, cmd);
// }

// void UI_SHOW_t::RTS_SndData(long n, unsigned long addr, unsigned long addr1, unsigned long cmd)
// {
//   RTS_SndData((unsigned long)n, addr, addr1, cmd);
// }

// void UI_SHOW_t::RTS_SndData(unsigned long n, unsigned long _screen_id, unsigned long _control_id, unsigned long cmd/*= VarAddr_W*/)
// {
//   snddat.head == CMD_HEAD;
//   snddat.cmd_type = cmd >> 8;     // 命令类型
//   snddat.ctrl_msg = cmd & 0x00FF; // 消息类型
//   snddat.screen_id = _screen_id;
//   snddat.control_id = _control_id;

//   if(work16(snddat.cmd_type, snddat.ctrl_msg) == CMD_SEND_TO_SCREEN)
//   {
//     if(n > 0xFFFF)
//     {
//       snddat.data[0] = n >> 24;
//       snddat.data[1] = (n >> 16) & 0xFF;
//       snddat.data[2] = (n >> 8) & 0xFF;
//       snddat.data[3] = n & 0xFF;
//       snddat.bytelen = 11;
//     }
//     else
//     {
//       snddat.data[0] = (n >> 8) & 0xFF;
//       snddat.data[1] = n & 0xFF;
//       snddat.bytelen = 9;
//     }
//   }
//   RTS_SndData();
// }

// /* 发送的数据不用指定 页面ID、控件ID */
// void UI_SHOW_t::RTS_SndData(int n, unsigned long cmd/* = CMD_GOTO_SCREEN*/)
// {
//   if(cmd == CMD_GOTO_SCREEN)
//   {
//     databuf[0] = CMD_HEAD;
//     databuf[1] = cmd >> 8;     // 命令类型
//     databuf[2] = cmd & 0x00FF; // 消息类型
//     databuf[3] = n >> 8;
//     databuf[4] = n & 0xFF;

//     // 增加帧尾
//     memcpy(&databuf[5], frameEnd, 4);

//     dwin_uart_write(databuf, 9);

//     memset(databuf, 0, sizeof(databuf));
//   }
// }

// bool UI_SHOW_t::UI_Handshake(void)
// {
//   uint8_t receivedbyte;
//   uint8_t cmd_pos = 0;
//   uint8_t buffer[10];
//   // 队列帧尾检测状态
//   static uint32_t cmd_state = 0;
//   uint8_t handBuf[6] = {0xEE, 0x55, 0xFF, 0xFC, 0xFF, 0xFF};
//   // 发送握手信息
//   databuf[0] = CMD_HEAD;
//   databuf[1] = 0x04;
//   memcpy(&databuf[2], frameEnd, 4);
//   dwin_uart_write(databuf, 6);
//   memset(databuf, 0, sizeof(databuf));

//   delay(10);
//   while(LCD_SERIAL.available())
//   {
//     // 取一个数据
//     receivedbyte = LCD_SERIAL.read();
//     //指令的第一个字节必须是帧头
//     if(cmd_pos == 0 && receivedbyte != CMD_HEAD)
//     {
//       continue;
//     }

//     if(cmd_pos < 10)
//     {
//       // 防止溢出
//       buffer[cmd_pos++] = receivedbyte;
//     }
//     else
//     {
//       break;
//     }

//     // 拼接最后4字节，组成最后一个32位整数
//     cmd_state = ((cmd_state << 8) | receivedbyte);
//     delay(2);
//     // 帧尾判断
//     if(cmd_state == CMD_TAIL)
//     {
//       for (int i = 0; i < cmd_pos; i++)
//       {
//         SERIAL_ECHOLNPAIR("  ", buffer[i]);
//       }

//       if (memcmp(buffer, handBuf, sizeof(handBuf)) == 0)
//       {
//         return true;
//       }
//       cmd_state = 0;      // 重新检测帧尾
//       cmd_pos = 0;        // 复位指令指针
//     }
//   }
//   return false;
// }

// //切换界面
// void UI_SHOW_t::UI_GotoScreen(int screen)
// {
//   // 检测不是大彩屏连接，直接返回
//   // if (!IsDacaiScreenConnect)
//   //   return;

//   RTS_SndData(screen);
// }

// /*
//  * frame：       ICON控件中的第frame个图标
//  * _screen_id：  图片页面
//  * _control_id:  控件ID
//  * cmd == CMD_ICON_SET_FRAME:  设置与“图标控件”、“动画控件” 的显示（指定帧播放）
//  *        Screen_id(2个字节) + Control_id(2个字节) + FlashImgae_ID(1个字节)某一动画帧ID
//  * 
//  * cmd == CMD_ICON_START:    启动动画播放, 此时frame无效
//  *        Screen_id(2个字节) + Control_id(2个字节)
//  * 
//  * cmd == CMD_ICON_STOP:     停止动画播放, 此时frame无效
//  *        Screen_id(2个字节) + Control_id(2个字节)
//  * 
//  * cmd == CMD_ICON_PAUSE:    暂停动画播放, 此时frame无效
//  *        Screen_id(2个字节) + Control_id(2个字节)
//  */
// void UI_SHOW_t::UI_ShowIcon(int frame , unsigned long _screen_id, unsigned long _control_id, unsigned long cmd/* = CMD_ICON_SET_FRAME*/)
// {
//   // 检测不是大彩屏连接，直接返回
//   // if (!IsDacaiScreenConnect)
//   //   return;

//   if (cmd == CMD_ICON_SET_FRAME)
//   {
//     RTS_SndData(frame, _screen_id, _control_id, cmd);
//   }
//   else if (cmd == CMD_ICON_START || cmd == CMD_ICON_STOP || cmd == CMD_ICON_PAUSE)
//   {
//     snddat.head = CMD_HEAD;
//     snddat.cmd_type = cmd >> 8;     // 命令类型
//     snddat.ctrl_msg = cmd & 0x00FF; // 消息类型
//     snddat.screen_id = _screen_id;
//     snddat.control_id = _control_id;
//     snddat.bytelen = 7;
//     RTS_SndData();
//   }
// }

// /*
//  * 功能：        ICON控件的控制
//  * _screen_id：  图片页面
//  * _control_id:  控件ID
//  * cmd == CMD_ICON_SHOW_HIDE:  显示或隐藏控件
//  *        Screen_id(2个字节) + Control_id(2个字节) + Enable(1个字节) 0x00：屏蔽或隐藏控件;  0x01: 屏蔽/隐藏解除
//  * 
//  * 注意：1、控件使用‘自动播放’且 ‘初始化显示’先择为 '否' 时，把控件显示出来，才开始第一次播放
//  */
// void UI_SHOW_t::UI_ShowOrHideConctrols(bool isDisplay , unsigned long _screen_id, unsigned long _control_id, unsigned long cmd/* = CMD_ICON_SHOW_HIDE*/)
// {
//   // 检测不是大彩屏连接，直接返回
//   // if (!IsDacaiScreenConnect)
//   //     return;

//   if (cmd == CMD_ICON_SHOW_HIDE)
//   {
//     RTS_SndData(isDisplay, _screen_id, _control_id, cmd);
//   }
// }

// void UI_SHOW_t::UI_ShowText(char* n , unsigned long _screen_id, unsigned long _control_id, unsigned long cmd/* = CMD_SEND_TO_SCREEN*/)
// {
//   // 检测不是大彩屏连接，直接返回
//   // if (!IsDacaiScreenConnect)
//   //   return;

//   RTS_SndData(n, _screen_id, _control_id, cmd);
// }

// /*
//  * n：           需要发送的内容
//  * _screen_id：  图片页面
//  * _control_id:  控件ID
//  * cmd == CMD_SEND_TO_SCREEN:  发送字符串到屏幕显示
//  *        Screen_id(2个字节) + Control_id(2个字节) + Strings(不定长)
//  * cmd == CMD_TEXT_TWINKLE:    设置文本闪烁，闪烁周期(10毫秒单位)，0表示不闪烁
//  *        Screen_id(2个字节) + Control_id(2个字节) + Cycle(2个字节)
//  * cmd == CMD_TEXT_ROLL:       设置文本滚动，文本滚动速度(每秒移动像素)，0表示不滚动
//  *        Screen_id(2个字节) + Control_id(2个字节) + Speed(2个字节)
//  * cmd == CMD_TEXT_COLOR_FORE: 文本-前景色
//  *        Screen_id(2个字节) + Control_id(2个字节) + BK_Color(2个字节)
//  */
// void UI_SHOW_t::UI_ShowText(int n ,                                     // 需要发送的内容(数据 或 控件属性设置)
//                             unsigned long _screen_id,                   // 所在屏幕ID
//                             unsigned long _control_id,                  // 控件ID
//                             unsigned long cmd/* = CMD_SEND_TO_SCREEN*/, // 命令，默认为 CMD_SEND_TO_SCREEN(发送文本到屏幕)
//                             uint8_t unit)                               // 单位
// {
//   // 检测不是大彩屏连接，直接返回
//   // if (!IsDacaiScreenConnect)
//   //   return;

//   static char txt[15] = {0};
//   memset(txt, 0, sizeof(txt));
//   // 注意：根据大彩协议，对于文本的显示需要转换成字符串，对于文本的属性（文本闪烁、文本滚动、设置颜色等，不需要转变成字符串）
//   if (cmd == CMD_SEND_TO_SCREEN)
//   {
//     itoa(n, txt, 10);
//     // 单位处理
//     if (unit != U_NULL)
//     {
//       strcat(txt, unitBuf[unit]);
//     }
//     UI_ShowText(txt, _screen_id, _control_id, cmd);
//   }
//   else
//   {
//     RTS_SndData(n, _screen_id, _control_id, cmd);
//   }
// }

// void UI_SHOW_t::UI_ShowText(float n ,                                   // 需要显示的数字
//                             uint8_t ndigit,                             // 小数位数 
//                             unsigned long _screen_id,                   // 所在屏幕ID
//                             unsigned long _control_id,                  // 控件ID
//                             unsigned long cmd/* = CMD_SEND_TO_SCREEN*/, 
//                             uint8_t unit/* = U_NULL*/)                  // 单位
// {
//   // 检测不是大彩屏连接，直接返回
//   // if (!IsDacaiScreenConnect)
//   //   return;

//   char txt[15];
//   char isNegative = false; // 是否为负数标志
//   int len = 0;
//   int integer;
//   int decimal ;  // = (int)((unsigned long)(n*1000))%1000;

//   if (n < 0)
//   {
//     isNegative = true;
//     n = -n;
//   }
//   integer = n;        // 整数部分
//   // 小数部分
//   if (ndigit == 3)
//   {
//     decimal = (int)((unsigned long)(n*1000))%1000;
//   }
//   else if (ndigit == 2)
//   {
//     decimal = (int)((unsigned long)(n*100))%100;
//   }
//   else
//   {
//     decimal = (int)((unsigned long)(n*10))%10;
//   }

//   itoa(integer, txt, 10);
//   len = strlen(txt);
//   txt[len++] = '.';
//   // itoa(decimal, &txt[len+1], 10);
//   // 小数部分
//   if (ndigit == 3)
//   {
//     txt[len++] = decimal/100 + 0x30;
//     txt[len++] = decimal%100/10 + 0x30;
//     txt[len++] = decimal%10 + 0x30;
//   }
//   else if (ndigit == 2)
//   {
//     txt[len++] = decimal/10 + 0x30;
//     txt[len++] = decimal%10 + 0x30;
//   }
//   else
//   {
//     txt[len++] = decimal%10 + 0x30;
//   }
//   // 单位处理
//   if (unit != U_NULL)
//   {
//     int unitLen = strlen(unitBuf[unit]);
//     memcpy(&txt[len++], &unitBuf[unit], unitLen);
//     len += unitLen;
//   }
//   txt[len++] = '\0';

//   // 特殊处理浮点型
//   if (isNegative)
//   {
//     for (int i = sizeof(txt) - 1; i > 0; i--)
//     {
//       txt[i] = txt[i - 1];
//     }
//     txt[0] = '-';
//   }

//   UI_ShowText(txt, _screen_id, _control_id, cmd);
// }

// void UI_SHOW_t::UI_ShowTextClear(unsigned long _screen_id, unsigned long _control_id, unsigned long cmd/* = CMD_SEND_TO_SCREEN*/)
// {
//   // 检测不是大彩屏连接，直接返回
//   // if (!IsDacaiScreenConnect)
//   //   return;

//   RTS_SndData("", _screen_id, _control_id, cmd);
// }

// void UI_SHOW_t::UI_ShowTextUnicode(const char *str, const int strlen, unsigned long _screen_id, unsigned long _control_id, unsigned long cmd/*= VarAddr_W*/)
// {
//   // if (!IsDacaiScreenConnect)
//   //   return;
//   int len = strlen;
//   // SERIAL_ECHOLNPAIR("\r\n\r\n\r\n len = ", len);
//   int i = 0;
//   if( len >= 0)
//   {
//     databuf[0] = CMD_HEAD;
//     databuf[1] = cmd >> 8;     // 命令类型
//     databuf[2] = cmd & 0x00FF; // 消息类型
//     databuf[3] = _screen_id >> 8;
//     databuf[4] = _screen_id & 0x00FF;
//     databuf[5] = _control_id >> 8;
//     databuf[6] = _control_id & 0xFF;

//     for(i = 0; i < len ; )
//     {
//       databuf[7 + i] = str[i + 1];
//       databuf[7 + i + 1] = str[i];
//       if (databuf[7 + i] == 0 && databuf[7 + i + 1] == 0)
//       {
//         break;
//       }
//       i = i + 2;
//     }
//     databuf[7 + i] = 0;
//     databuf[7 + i + 1] = 0;
//     // 增加帧尾 
//     memcpy(&databuf[7 + i + 2], frameEnd, 4);

//     dwin_uart_write(databuf, i + 2 + 13);

//     memset(databuf, 0, sizeof(databuf));
//   }
// }

// // unsigned long arr_data_num;
// // /**
// //  * 功能:    发送jpg图片前的握手动作 -- Handshake before sending jpg images
// //  * jpeg:    指向图片数据的指针 -- pointer to image data
// //  * path:    发送的路径 -- path to send
// //  */
// // bool UI_SHOW_t::UI_SendJpegDateHandshake(unsigned long size, const char *path)
// // {
// //   // 当使用大彩屏时 -- When using large color screen
// //   if (!IsDacaiScreenConnect)
// //     return true;

// //   uint8_t receivedbyte;
// //   uint8_t cmd_pos = 0;
// //   uint8_t buffer[10];
// //   uint32_t cmd_state = 0;  // 队列帧尾检测状态 -- Queue frame end detection status

// //   uint8_t cmd_dl_response[7] = {0xEE,0xFB,0x01,0xFF,0xFC,0xFF,0xFF};   // 屏幕返回的下载确认命令 -- Download confirmation command returned by the screen
// //   unsigned long jpgSize = size;

// //   // 下载命令 -- Download command
// //   databuf[0] = CMD_HEAD;
// //   databuf[1] = 0xFB;
// //   // 下载包大小 -- Download package size
// //   databuf[2] = (DACAI_JPG_BYTES_PER_BUF>>8)&0xff;
// //   databuf[3] = (DACAI_JPG_BYTES_PER_BUF)&0xff;
// //   // 零食波特率，不修改时，设置0 -- Snack baud rate, set to 0 when not modified
// //   databuf[4] = (0>>8)&0xff;
// //   databuf[5] = (0)&0xff;
// //   // 文件大小 -- File size
// //   databuf[6] = (jpgSize>>24)&0xff;
// //   databuf[7] = (jpgSize>>16)&0xff;
// //   databuf[8] = (jpgSize>>8)&0xff;
// //   databuf[9] = (jpgSize)&0xff;
// //   // 文件路径 -- File path
// //   memcpy(&databuf[10], path, strlen(path));
// //   // 增加帧尾 -- Add frame tail
// //   memcpy(&databuf[strlen(path) + 12], frameEnd, 4);
// //   // 发送命令帧 -- Send command frame
// //   // SERIAL_ECHO("\r\n ");
// //   // for (int aa = 0; aa <= strlen(path) + 16; aa++) {
// //   //     SERIAL_ECHO(databuf[aa]);
// //   //     SERIAL_ECHO("  ");
// //   // }
// //   dwin_uart_write(databuf, strlen(path) + 16);

// //   memset(databuf, 0, sizeof(databuf));
// //   delay(20);
// //   uint32_t ms = millis();

// //   uint32_t MsTest = millis();
// //   while(1)
// //   {
// //     watchdog_refresh();
// //     if (millis() - ms >= 3000)
// //     {
// //       return false;
// //     }
// //     // 接收处理 -- Receive processing
// //     if(LCD_SERIAL.available())
// //     {
// //       // 取一个数据 -- Take a data
// //       receivedbyte = LCD_SERIAL.read();
// //       if(cmd_pos == 0 && receivedbyte != CMD_HEAD)
// //       {
// //         // 指令的第一个字节必须是帧头 -- The first byte of the instruction must be the frame header
// //         continue;
// //       }

// //       if(cmd_pos < 10)
// //       {
// //         // 防止溢出 -- Prevent overflow
// //         buffer[cmd_pos++] = receivedbyte;
// //       }
// //       else
// //       {
// //         return false;
// //       }

// //       // 拼接最后4字节，组成最后一个32位整数 -- Splice the last 4 bytes to form the last 32-bit integer
// //       cmd_state = ((cmd_state << 8) | receivedbyte);
// //       delay(2);
// //       // 帧尾判断 -- Frame end
// //       if(cmd_state == CMD_TAIL)
// //       {
// //         // for (int i = 0; i < cmd_pos; i++)
// //         //   SERIAL_ECHOLNPAIR("  ", buffer[i]);

// //         if (memcmp(buffer, cmd_dl_response, sizeof(cmd_dl_response)) == 0)
// //         {
// //           break;
// //         }
// //         // 重新检测帧尾 -- Re-detect frame end
// //         cmd_state = 0;
// //       }
// //     }
// //   }
// //   if (ENABLED(USER_LOGIC_DEUBG))
// //   {
// //     SERIAL_ECHOLNPAIR("\r\n UI_SendJpegDateHandshake time1 = ", millis() - MsTest);
// //   }
// //   // DWIN_Handshake();
// //   return true;
// // }

// // /**
// //  * 功能:    发送jpg图片整包数据 -- Send jpg picture data in a whole package
// //  * jpeg:    指向图片数据的指针 -- Pointer to the image data
// //  * size:    图片数据长度（从gcode里面读取出来）-- Image data length (read from gcode)
// //  */
// //  bool UI_SHOW_t::UI_SendJpegDate(const char *jpeg, unsigned long size)
// // {
// //   // static char buf[DACAI_JPG_BYTES_PER_BUF] = {0};
// //   unsigned long jpgSize = size;
// //   // static uint8_t sn = 0;
// //   uint16_t i, j;
// //   // uint16_t checkSum = 0;
// //   int sendPacket;
// //   uint32_t MS = millis();

// //   databuf[0] = 0xAA;
// //   databuf[1] = 0xC0;
// //   databuf[4 + DACAI_JPG_BYTES_PER_PACKET] = 0xCC;
// //   databuf[4 + DACAI_JPG_BYTES_PER_PACKET + 1] = 0x33;
// //   databuf[4 + DACAI_JPG_BYTES_PER_PACKET + 2] = 0xC3;
// //   databuf[4 + DACAI_JPG_BYTES_PER_PACKET + 3] = 0X3C;

// //   for (i = 0; i < jpgSize / DACAI_JPG_BYTES_PER_FRAME; i++)
// //   {
// //     // 发送命令帧 -- dwin_uart_write 这个接口传入的字节长度不要超255 -- Send command frame -- dwin_uart_write The byte length passed in by this interface should not exceed 255
// //     for (sendPacket = 0; sendPacket < jpgSize / DACAI_JPG_BYTES_PER_PACKET; sendPacket++)
// //     {
// //       // SERIAL_ECHOLN(sendPacket);
// //       // SERIAL_ECHOLN(arr_data_num);
// //       databuf[2] = (arr_data_num) >> 8;
// //       databuf[3] = (arr_data_num) & 0xFF;
// //       arr_data_num += 120;
// //       memcpy(&databuf[4], &jpeg[sendPacket * DACAI_JPG_BYTES_PER_PACKET], DACAI_JPG_BYTES_PER_PACKET);
// //       watchdog_refresh();
// //       dwin_uart_write(databuf, DACAI_JPG_BYTES_PER_PACKET + 8);
// //       watchdog_refresh();
// //       delay(5);
// //     }

// //     // 2048字节发送完剩下的 -- 2048 bytes are sent and the rest
// //     if(jpgSize % DACAI_JPG_BYTES_PER_PACKET)
// //     {
// //       j = (jpgSize - DACAI_JPG_BYTES_PER_PACKET * sendPacket);
// //       databuf[2] = (arr_data_num) >> 8;
// //       databuf[3] = (arr_data_num) & 0xFF;
// //       arr_data_num+=j/2;
// //       databuf[4 + j] = 0xCC;
// //       databuf[4 + j + 1] = 0x33;
// //       databuf[4 + j + 2] = 0xC3;
// //       databuf[4 + j + 3] = 0X3C;
// //       memcpy(&databuf[4], &jpeg[(sendPacket * DACAI_JPG_BYTES_PER_PACKET)], j);
// //       watchdog_refresh();
// //       delay(5);
// //       dwin_uart_write(databuf, j + 8);
// //     }
// //   }

// //   if (jpgSize % DACAI_JPG_BYTES_PER_FRAME)
// //   {
// //     // 小于2048字节处理 -- Processing less than 2048 bytes
// //     for (sendPacket = 0; sendPacket < jpgSize / DACAI_JPG_BYTES_PER_PACKET; sendPacket++)
// //     {
// //       databuf[2] = (arr_data_num) >> 8;
// //       databuf[3] = (arr_data_num) & 0xFF;
// //       arr_data_num+=120;
// //       memcpy(&databuf[4], &jpeg[sendPacket * DACAI_JPG_BYTES_PER_PACKET], DACAI_JPG_BYTES_PER_PACKET);
// //       dwin_uart_write(databuf, DACAI_JPG_BYTES_PER_PACKET + 8);
// //       watchdog_refresh();
// //       delay(5);
// //     }

// //     // 2048字节发送完剩下的 -- 2048 bytes are sent and the rest
// //     if(jpgSize % DACAI_JPG_BYTES_PER_PACKET)
// //     {
// //       j = (jpgSize - DACAI_JPG_BYTES_PER_PACKET * sendPacket);
// //       databuf[2] = (arr_data_num) >> 8;
// //       databuf[3] = (arr_data_num) & 0xFF;
// //       arr_data_num+=j;
// //       databuf[4 + j] = 0xCC;
// //       databuf[4 + j + 1] = 0x33;
// //       databuf[4 + j + 2] = 0xC3;
// //       databuf[4 + j + 3] = 0X3C;
// //       memcpy(&databuf[4], &jpeg[(sendPacket * DACAI_JPG_BYTES_PER_PACKET)], j);
// //       watchdog_refresh();
// //       delay(5);
// //       dwin_uart_write(databuf, j + 8);
// //     }
// //   }
// //   return true;
// // }

// // void UI_SHOW_t::UI_DisplayJpegDate(unsigned long _screen_id)
// // {
// //   // 当使用大彩屏时
// //   // if (!IsDacaiScreenConnect)
// //   //   return ;
// //   databuf[0] = CMD_HEAD;
// //   databuf[1] = 0xB5;   // 命令类型
// //   databuf[2] = 0x01;   // 消息类型
// //   databuf[3] = _screen_id >> 8;
// //   databuf[4] = _screen_id & 0xFF;

// //   // 增加帧尾 -- Add frame tail
// //   memcpy(&databuf[5], frameEnd, 4);

// //   dwin_uart_write(databuf, 9);
// // }

// // 修改语言
// void UI_SHOW_t::UI_ChangeLanguage(unsigned int lang)
// {
//   databuf[0] = CMD_HEAD;
//   databuf[1] = CMD_SET_CHANGE_LNGUAGE;   // 命令类型
//   databuf[2] = lang & 0xFF;
//   databuf[3] = (CMD_SET_CHANGE_LNGUAGE + lang) & 0xFF;

//   // 增加帧尾
//   memcpy(&databuf[4], frameEnd, 4);

//   dwin_uart_write(databuf, 8);
// }
