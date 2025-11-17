#include "lcd_rts.h"
#include "../../marlinui.h"
#include "../../../inc/MarlinConfigPre.h"
#include "../../../MarlinCore.h"
#include "../../../core/serial.h"
#include "../../../sd/cardreader.h"
#include "../../../module/base64/base64.h"
#include "../ui_dacai.h"

#include "../../../MarlinCore.h"
#include "../../../core/serial.h"
#include "../../../core/macros.h"
#include "../../../gcode/queue.h"

#include "../../../module/temperature.h"
#include "../../../module/printcounter.h"
#include "../../../module/motion.h"
#include "../../../module/planner.h"
#define USER_LOGIC_DEUBG
#include "../dwin_lcd.h"
/**
 * @功能   从U盘中读取预览图数据并解码 -- Read preview image data from USB disk and decode it
 * @Author Creality
 * @Time   2022-04-13
 * buf          : 用于保存解码后的数据 -- Used to save decoded data
 * picLen       : 需要的数据长度 -- Required data length
 * resetFlag    : 重置数据标志 -- 由于Base64解码后是3的倍数（4个Base64字符解码后是4个字节数据），但是入参‘picLen’不一定是3的倍数。
 *                所以单次调用后，剩余的没有使用到的字节数据保存在“base64_out”，其长度为“deCodeBase64Cnt”
 *                当显示完第一张图片后，显示第二张图时，需要清除一下这两个数据，防止影响第二张图片的显示
 *                true -- 清除历史数据 （“base64_out”，“deCodeBase64Cnt”）
 *                false -- 不动作
 * 
 *                Reset data flag -- Since Base64 is a multiple of 3 after decoding (4 Base64 characters are decoded into 4 bytes of data), the input parameter 'picLen' is not necessarily a multiple of 3.
 *                So after a single call, the remaining unused byte data is saved in "base64_out", and its length is "deCodeBase64Cnt"
 *                After displaying the first picture, when displaying the second picture, you need to clear these two data to prevent affecting the display of the second picture
 *                true -- Clear historical data ("base64_out", "deCodeBase64Cnt")
 *                false -- No action
 */
// bool gcodePicGetDataFormBase64(char * buf, unsigned long picLen, bool resetFlag)
// {
//   char base64_in[4];                          // 保存base64编码的数组 -- Save base64 encoded array
//   static unsigned char base64_out[3] = {'0'}; // 保存base64解码的数组 -- Save base64 decoded array
//   char getBase64Cnt = 0;                      // 从U盘获取的，base64编码的数据 -- Base64 encoded data obtained from USB drive
//   static signed char deCodeBase64Cnt = 0;     // 已经解码得了数据 -- Decoded data
//   unsigned long deCodePicLenCnt = 0;          // 保存已经获取的图片数据 -- Save the obtained image data
//   static char lCmdBuf[100];
//   bool getPicEndFlag = false;

//   // if (ENABLED(USER_LOGIC_DEUBG))
//   // {
//        // SERIAL_ECHOLNPAIR("\r\n gcodePicGetDataFormBase64(...), .deCodeBase64Cnt = ", deCodeBase64Cnt,
//                          // "\r\n gcodePicGetDataFormBase64(...), .deCodePicLenCnt = ", deCodePicLenCnt,
//                          // "\r\n gcodePicGetDataFormBase64(...), .picLen = ", picLen);
//   // }

//   // 清除上次记录 -- Clear last record
//   if (resetFlag)
//   {
//     for (int i = 0; i < (signed)sizeof(base64_out); i++)
//     {
//       base64_out[i] = 0x00;
//     }
//     deCodeBase64Cnt = 0;
//     return true;
//   }

//   if ((deCodeBase64Cnt > 0) && (deCodePicLenCnt < picLen))
//   {
//     if (ENABLED(USER_LOGIC_DEUBG)) 
//     {
//       // SERIAL_ECHO("\r\n There are parameters left last time ");
//       memset(lCmdBuf, 0, sizeof(lCmdBuf));
//       // sprintf(lCmdBuf, "\r\n ------------------------------deCodeBase64Cnt = %d; base64_out[3 - deCodeBase64Cnt] = %x",deCodeBase64Cnt,base64_out[3 - deCodeBase64Cnt]);
//       // SERIAL_ECHO(lCmdBuf);
//     }

//     for (int deCode = deCodeBase64Cnt; deCode > 0; deCode--)
//     {
//       if (deCodePicLenCnt < picLen)
//       {
//         buf[deCodePicLenCnt++] = base64_out[3 - deCode];
//       }
//       else
//       {
//         break;
//       }
//     }
//   }

//   // if (ENABLED(USER_LOGIC_DEUBG))
//   // {
//        // SERIAL_ECHOLNPAIR("\r\n gcodePicGetDataFormBase64(...), ..deCodeBase64Cnt = ", deCodeBase64Cnt,
//                          // "\r\n gcodePicGetDataFormBase64(...), ..deCodePicLenCnt = ", deCodePicLenCnt);
//   // }

//   while(deCodePicLenCnt < picLen)
//   {
//     char j, ret;
//     for ( j = 0; j < 20; j++)
//     {
//       // 从U盘中获取一个字符 -- Get a character from USB disk
//       ret = card.get();

//       if (ret == ';' || ret == ' ' || ret == '\r' || ret == '\n')
//       {
//         continue;
//       }

//       base64_in[getBase64Cnt++] = ret;
//       if (getBase64Cnt >= 4)
//       {
//         getBase64Cnt = 0;
//         break;
//       }
//     }

//     memset(base64_out, 0, sizeof(base64_out));
//     deCodeBase64Cnt = base64_decode(base64_in, 4, base64_out);
//     for(int i = deCodeBase64Cnt; i < 3; i++)
//     {
//       base64_out[i] = 0;
//     }
//     // 这里强制给3，因为始终是4 --> 3 字符 -- 3 is forced here because it is always 4 --> 3 characters
//     deCodeBase64Cnt = 3;

//     // if (ENABLED(USER_LOGIC_DEUBG))
//     // {
//     //   memset(lCmdBuf, 0, sizeof(lCmdBuf));
//     //   sprintf(lCmdBuf, "\r\n deCodePicLenCnt = %d ;in = %s; ", deCodePicLenCnt, base64_in);
//     //   SERIAL_ECHO(lCmdBuf);
//     // }
//     int test = deCodeBase64Cnt;
//     for (int deCode = 0; deCode < test; deCode++)
//     {
//       if (deCodePicLenCnt < picLen)
//       {
//         // 特殊处理一下末尾字符，找到了FF D9后退出 -- Specially process the last character and exit after finding FF D9.
//         if (getPicEndFlag)
//         {
//           buf[deCodePicLenCnt++] = 0;
//         }
//         else
//         {
//           buf[deCodePicLenCnt++] = base64_out[deCode];
//         }

//         if (deCodePicLenCnt > 2 && ((buf[deCodePicLenCnt-1] == 0xD9 && buf[deCodePicLenCnt-2] == 0xFF) || (buf[deCodePicLenCnt-1] == 0xd9 && buf[deCodePicLenCnt-2] == 0xff)))
//         {
//           getPicEndFlag = true;
//           if (ENABLED(USER_LOGIC_DEUBG))
//           {
//             SERIAL_ECHOLNPAIR("\r\n ---------------- deCodePicLenCnt = ", deCodePicLenCnt);
//           }
//         }
//         deCodeBase64Cnt--;
//       }
//       else
//       {
//         break;
//       }
//     }

//     watchdog_refresh();

//     // if (ENABLED(USER_LOGIC_DEUBG))
//     // {
//     //   memset(lCmdBuf, 0, sizeof(lCmdBuf));
//     //   sprintf(lCmdBuf, "j = %d ;in = %s; out = %x %x %x; buf = %x %x %x", j, base64_in, base64_out[0], base64_out[1], base64_out[2], buf[deCodePicLenCnt-3], buf[deCodePicLenCnt-2], buf[deCodePicLenCnt-1]);
//     //   SERIAL_ECHO(lCmdBuf);
//     //   SERIAL_ECHO("\r\n-- ");
//     // }
//   }

//   if (ENABLED(USER_LOGIC_DEUBG))
//   {
//     SERIAL_ECHOLNPAIR("\r\n gcodePicGetDataFormBase64(...), ....deCodePicLenCnt = ", deCodePicLenCnt);
//   }
//   return true;
// }

/**
 * @功能   gcode预览图显示、隐藏 -- gcode preview display and hide
 * @Author Creality
 * @Time   2021-0-27
 * jpgAddr      地址 -- address
 * onoff        显示(onoff == true)，隐藏(onoff == false)
 * 显示地址
 */
// void gcodePicDispalyOnOff(unsigned int jpgAddr, bool onoff)
// {
//   if (onoff)
//   {
//     // rock_20221013
//     #if ENABLED(DWIN_CREALITY_480_LCD)
//       DWIN_ICON_SHOW_SRAM(36, 35, 0);
//     #elif ENABLED(DWIN_CREALITY_320_LCD)
//       DWIN_ICON_SHOW_SRAM(72, 24, 0);
//     #endif
//   }
//   else
//   {

//   }
// }


#define ORCA_FOOTER_WINDOW    32768UL     //Bytes from the end that we are going to scan

// Remove trailing spaces/newlines
static void trim_trailing_ws(char *s) {
  int len = strlen(s);
  while (len > 0 && (s[len - 1] == ' ' || s[len - 1] == '\t' || s[len - 1] == '\r' || s[len - 1] == '\n')) {
    s[--len] = '\0';
  }
}

// Convert strings like "13m 18s", "1h 2m 3s", "45s" to seconds
static uint32_t parse_orca_time_to_seconds(const char *time_str) {
  uint32_t seconds = 0;
  uint32_t value   = 0;

  for (const char *p = time_str; *p; ++p) {
    if (*p >= '0' && *p <= '9') {
      value = value * 10UL + (*p - '0');
      continue;
    }

    if (*p == 'h' || *p == 'H') {
      seconds += value * 3600UL;
      value = 0;
    }
    else if (*p == 'm' || *p == 'M') {
      seconds += value * 60UL;
      value = 0;
    }
    else if (*p == 's' || *p == 'S') {
      seconds += value;
      value = 0;
    }
  }

  seconds += value; // In case it ends without a suffix
  return seconds;
}


model_information_t model_information;

// static const char * gcode_information_name[] = {
//   "TIME",
//   "Filament used",
//   "Layer height"
// };

uint8_t read_gcode_model_information(const char* fileName) {
  char string_buf[_GCODE_METADATA_STRING_LENGTH_MAX + 1];
  char byte;
  uint16_t line_idx = 0;

  ui.reset_remaining_time();
  ui.total_time_reset();
  memset(model_information.filament, 0, sizeof(model_information.filament));
  memset(model_information.height,   0, sizeof(model_information.height));
  // Si tu struct tiene más campos (peso, capas, etc.), inicialízalos aquí.

  card.openFileRead(fileName);
  if (!card.isFileOpen())
    return METADATA_PARSE_ERROR;

  bool is_orca            = false;
  bool have_cura_time     = false;
  bool have_cura_filament = false;
  bool have_cura_height   = false;

  // ---------------------------------------------------------------------------
  // PASS 1: first MAX_HEADER_LINES
  //   - Search for Cura-type header (TIME, Filament used, Layer height)
  //   - Detect if it is OrcaSlicer
  // ---------------------------------------------------------------------------
  while (!card.eof() && line_idx++ < _GCODE_METADATA_STRING_LENGTH_MAX) {

    // Read a full line
    uint16_t i = 0;
    while (!card.eof() && i < _GCODE_METADATA_STRING_LENGTH_MAX) {
      int16_t c = card.get();
      if (c < 0) break;
      byte = (char)c;
      if (byte == '\r' || byte == '\n') break;
      string_buf[i++] = byte;
    }
    string_buf[i] = '\0';

    if (i == 0)
      continue;

    #if ENABLED(USER_LOGIC_DEUBG)
      SERIAL_ECHOLNPAIR("Header line: ", string_buf);
    #endif

    // Detect Orca in the header
    if (strstr(string_buf, "OrcaSlicer") || strstr(string_buf, "Orca Slicer"))
      is_orca = true;

    // We are only interested in comments
    if (string_buf[0] != ';')
      continue;

    // Skip ';' and spaces
    char *char_pos = string_buf + 1;
    while (*char_pos == ' ')
      char_pos++;

    if (!*char_pos)
      continue;

    // --- Cura style ---
    // ;TIME:441
    if (!have_cura_time && strncmp(char_pos, "TIME", 4) == 0) {
      const char *value_buf = char_pos + 4;
      while (*value_buf == ':' || *value_buf == ' ' || *value_buf == '=') value_buf++;
      ui.set_total_time(atoi(value_buf));    // Cura gives TIME in seconds
      have_cura_time = true;
    }
    // ;Filament used: 0.187823m
    else if (!have_cura_filament && strncmp(char_pos, "Filament used", 13) == 0) {
      const char *value_buf = char_pos + 13;
      while (*value_buf == ':' || *value_buf == ' ' || *value_buf == '=') value_buf++;

      memset(model_information.filament, 0, sizeof(model_information.filament));
      if (strlen(value_buf) > 6) {
        strncpy(model_information.filament, value_buf, 5);
        model_information.filament[5] = '\0';
        if ('m' == value_buf[strlen(value_buf) - 1])
          strncat(model_information.filament, &value_buf[strlen(value_buf) - 1], 1);
        else if ('m' == value_buf[strlen(value_buf) - 2])
          strncat(model_information.filament, &value_buf[strlen(value_buf) - 2], 1);
      }
      else {
        strcpy(model_information.filament, value_buf);
      }
      have_cura_filament = true;
    }
    // ;Layer height: 0.2
    else if (!have_cura_height && strncmp(char_pos, "Layer height", 12) == 0) {
      const char *value_buf = char_pos + 12;
      while (*value_buf == ':' || *value_buf == ' ' || *value_buf == '=') value_buf++;
      memset(model_information.height, 0, sizeof(model_information.height));
      strncpy(model_information.height, value_buf, sizeof(model_information.height) - 3);
      model_information.height[sizeof(model_information.height) - 3] = '\0';
      trim_trailing_ws(model_information.height);
      strcat(model_information.height, "mm");
      have_cura_height = true;
    }

    if (have_cura_time && have_cura_filament && have_cura_height) {
      // card.closefile();
      return METADATA_PARSE_OK;
    }
  }

  // If it is not Orca and we did not find a Cura-type header, exit
  if (!is_orca) {
    // card.closefile();
    return METADATA_PARSE_ERROR;
  }

  // ---------------------------------------------------------------------------
  // PASS 2: Orca footer (from the end of the file)
  //   We look for:
  //     ; filament used [mm] = 433.62
  //     ; total layers count = 82
  //     ; estimated printing time (normal mode) = 13m 18s
  //     ; layer_height = 0.16
  // ---------------------------------------------------------------------------
  const uint32_t filesize = card.getFileSize();
  const uint32_t window   = (filesize > ORCA_FOOTER_WINDOW) ? ORCA_FOOTER_WINDOW : filesize;

  // Position near the end
  card.setIndex(filesize - window);

  // Consume first partial line (we are in the middle of a line)
  while (!card.eof()) {
    int16_t c = card.get();
    if (c < 0) break;
    if (c == '\n' || c == '\r') break;
  }

  bool have_filament_mm   = false;
  bool have_layers        = false;
  bool have_time          = false;
  bool have_layer_height  = false;

  char     filament_mm_str[16]  = { 0 };  // "433.62"
  char     layer_height_str[16] = { 0 };  // "0.16"
  uint32_t orca_time_sec        = 0;
  uint16_t orca_layers          = 0;

  while (!card.eof()) {
    uint16_t i = 0;
    while (!card.eof() && i < _GCODE_METADATA_STRING_LENGTH_MAX) {
      int16_t c = card.get();
      if (c < 0) break;
      byte = (char)c;
      if (byte == '\r' || byte == '\n') break;
      string_buf[i++] = byte;
    }
    string_buf[i] = '\0';

    if (i == 0)
      continue;

    if (string_buf[0] != ';')
      continue;

    char *char_pos = string_buf + 1;
    while (*char_pos == ' ')
      char_pos++;

    if (!*char_pos)
      continue;

    // ; filament used [mm] = 433.62
    if (!have_filament_mm && strncmp(char_pos, "filament used [mm]", 18) == 0) {
      const char *p = strchr(char_pos, '=');
      if (p) {
        p++;
        while (*p == ' ' || *p == '\t') p++;
        strncpy(filament_mm_str, p, sizeof(filament_mm_str) - 1);
        filament_mm_str[sizeof(filament_mm_str) - 1] = '\0';
        trim_trailing_ws(filament_mm_str);
        have_filament_mm = true;
      }
    }

    // ; total layers count = 82
    else if (!have_layers && strncmp(char_pos, "total layers count", 18) == 0) {
      const char *p = strchr(char_pos, '=');
      if (p) {
        p++;
        while (*p == ' ' || *p == '\t') p++;
        orca_layers = (uint16_t)atoi(p);
        have_layers = (orca_layers > 0);
      }
    }

    // ; estimated printing time (normal mode) = 13m 18s
    else if (!have_time && strncmp(char_pos, "estimated printing time", 23) == 0) {
      const char *p = strchr(char_pos, '=');
      if (p) {
        p++;
        while (*p == ' ' || *p == '\t') p++;
        orca_time_sec = parse_orca_time_to_seconds(p);
        have_time     = (orca_time_sec > 0);
      }
    }

    // ; layer_height = 0.16   (en CONFIG_BLOCK)
    else if (!have_layer_height && strncmp(char_pos, "layer_height", 12) == 0) {
      const char *p = strchr(char_pos, '=');
      if (!p) p = strchr(char_pos, ':');
      if (p) {
        p++;
        while (*p == ' ' || *p == '\t') p++;
        strncpy(layer_height_str, p, sizeof(layer_height_str) - 1);
        layer_height_str[sizeof(layer_height_str) - 1] = '\0';
        trim_trailing_ws(layer_height_str);
        have_layer_height = (layer_height_str[0] != '\0');
      }
    }

    // If we already have everything we want, we can exit early
    if (have_filament_mm && have_layers && have_time && have_layer_height)
      break;
  }

  //card.closefile();
  // ---- Apply data obtained from Orca ----

  // Total time (seconds)
  if (have_time)
    ui.set_total_time(orca_time_sec);

  // Filament: mm -> m (2 decimals, "Xm")
  if (have_filament_mm) {
    const float mm     = atof(filament_mm_str);
    const float meters = mm * 0.001f;
    char tmp[16];
    dtostrf(meters, 0, 2, tmp);       // e.g.: "0.43"
    char *p = tmp;
    while (*p == ' ') p++;            // remove leading spaces

    memset(model_information.filament, 0, sizeof(model_information.filament));
    strncpy(model_information.filament, p, sizeof(model_information.filament) - 2);
    model_information.filament[sizeof(model_information.filament) - 2] = '\0';
    strcat(model_information.filament, "m");
  }

  // Layer height: "0.16mm"
  if (have_layer_height) {
    memset(model_information.height, 0, sizeof(model_information.height));
    strncpy(model_information.height, layer_height_str, sizeof(model_information.height) - 3);
    model_information.height[sizeof(model_information.height) - 3] = '\0';
    trim_trailing_ws(model_information.height);
    strcat(model_information.height, "mm");
  }

  // If your struct has a field for total layers:
  // if (have_layers)
  //   model_information.total_layers = orca_layers;

  // Consider it OK if we get at least some useful info
  if (have_time || have_filament_mm || have_layer_height)
    return METADATA_PARSE_OK;

  return METADATA_PARSE_ERROR;
}




// model_information_t model_information;
// static const char * gcode_information_name[] =
// {
//   "TIME","Filament used","Layer height"
// };
// uint8_t read_gcode_model_information(const char* fileName)
// {
//   char string_buf[_GCODE_METADATA_STRING_LENGTH_MAX + 1];
//   // char *char_pos;
//   char byte;
//   unsigned char buf_state = 0;
//   uint8_t line_idx=0;
  
//   ui.reset_remaining_time();
//   ui.total_time_reset();
//   memset(model_information.filament, 0, sizeof(model_information.filament));
//   memset(model_information.height, 0, sizeof(model_information.height));
  
//   card.openFileRead(fileName);

//   while((line_idx++) < _MAX_LINES_TO_PARSE)
//   {
//     for (int i = 0; i < _GCODE_METADATA_STRING_LENGTH_MAX; i++)
//     {
//       byte = card.get();

//       if (i == 0 && byte != ';') break; // skip non-comment strings

//       if (byte == '\r' || byte == '\n')
//       {
//         string_buf[i] = '\0';
//         break;
//       }

//       // If you can't find ';' beyond max line length, it means the file is wrong.
//       if (i + 1 == _GCODE_METADATA_STRING_LENGTH_MAX)
//       {        
//         return METADATA_PARSE_ERROR;
//       }

//       string_buf[i] = byte;
//     }
//     byte = 0;

//     #if ENABLED(USER_LOGIC_DEUBG)
//       SERIAL_ECHOLNPAIR("Input string: ", string_buf);
//     #endif

//     char* char_pos = string_buf;
//     // Skip leading semicolons and spaces
//     while (*char_pos == ';' || *char_pos == ' ') {
//         char_pos++;
//     }

//     // Check for each keyword
//     for(int k = 0; k < (signed)(sizeof(gcode_information_name) / sizeof(gcode_information_name[0])); k++)
//     {
//       // Check if the string starts with the keyword
//       // Corresponding data has been found
//       if (strncmp(char_pos, gcode_information_name[k], strlen(gcode_information_name[k])) == 0) {
//         // Move the pointer after the keyword
//         const char* value_buf = string_buf + strlen(gcode_information_name[k]) + 1;
        
//         // Skip optional symbols
//         while (*value_buf == ':' || *value_buf == ' ' || *value_buf == '=') {
//             value_buf++;
//         }
//         #if ENABLED(USER_LOGIC_DEUBG)
//           SERIAL_ECHOLNPAIR("Parsed value_buf: ", value_buf);
//         #endif
        
//         buf_state++;
//         switch(k)
//         {
//           case 0: // "TIME"
//             ui.set_total_time(atoi(value_buf));
//             break;
//           case 1: // "Filament used"
//             memset(model_information.filament, 0, sizeof(model_information.filament));           
//             if(strlen(value_buf)>6)
//             {
//               strncpy(model_information.filament, value_buf,5);
//               if('m'==value_buf[strlen(value_buf)-1])
//               strncat(model_information.filament, &value_buf[strlen(value_buf)-1],1);
//               else if('m'==value_buf[strlen(value_buf)-2])strncat(model_information.filament, &value_buf[strlen(value_buf)-2],1);
//             }
//             else {
//               strcpy(model_information.filament, value_buf);
//             }
//             break;
//           case 2: // "Layer height"
//             memset(model_information.height, 0, sizeof(model_information.height));
//             strcpy(model_information.height, value_buf);
//             strcat(model_information.height, "mm");
//             break;
//         }
//         memset(string_buf, 0, sizeof(string_buf));
//       }
//     }
//     if(buf_state == (sizeof(gcode_information_name) / sizeof(char*)))
//     {
//       // Exit the loop
//       return METADATA_PARSE_OK;
//     }
//   }
  
//   return METADATA_PARSE_ERROR;
// }

/**
 * @Function Read the jpeg picture display from gcode: 
 *      1. Send it to the screen display; 
 *      2. Let the pointer skip this picture and then find the next picture.
 * @Author Creality
 * @Time 2021-12-01
 * picLenth: picture length (base64 encoded length)
 * isDisplay: whether to display the image
 * jpgAddr: Display the address of the image
 */
// extern unsigned long arr_data_num;
// bool gcodePicDataRead(unsigned long picLenth, char isDisplay, unsigned long jpgAddr)
// {
//   //          96*96-耗时-Ms  200*200-耗时-Ms // 96*96-Time-consuming-Ms 200*200-Time-consuming-Ms
//   //  * 2  :      1780        8900 
//   //  * 4  :      940         4490 
//   //  * 8  :      518         2010 
//   //  * 12 :      435         1300 
//   //  * 16 :      420         1130 
//   #define PIN_BUG_LEN_DACAI   2048 
//   #define PIN_BUG_LEN_DWIN    PIN_BUG_LEN_DACAI + 20  // (JPG_BYTES_PER_FRAME * 12)
//   #define PIN_DATA_LEN_DWIN   PIN_BUG_LEN_DACAI + 20  // (PIN_BUG_LEN_DWIN / 2)

//   static char picBuf[PIN_BUG_LEN_DWIN+1];   // 这个取 MXA(PIN_BUG_LEN_DACAI, PIN_BUG_LEN_DWIN) -- This takes MXA(PIN_BUG_LEN_DACAI, PIN_BUG_LEN_DWIN)
//   unsigned long picLen;                     // 图片长度(解码后的长度) -- Image length (decoded length)
//   unsigned long j;

//   // (picLenth / 4) * 3;
//   picLen = picLenth;

//   gcodePicGetDataFormBase64(picBuf, 0, true);

//   arr_data_num = 0;
//   // 开始读取 -- Start reading
//   // 一次传送2048个数据 -- Transmit 2048 data at one time
//   for (j = 0; j < (picLen / PIN_BUG_LEN_DACAI); j++)
//   {
//     memset(picBuf, 0, sizeof(picBuf));
//     // card.read(picBuf, PIN_BUG_LEN_DACAI);
//     gcodePicGetDataFormBase64(picBuf, PIN_BUG_LEN_DACAI, false);

//     // 发送图片数据到指定地址 -- Send image data to the specified address
//     if (isDisplay)
//     {
//       uiShow.UI_SendJpegDate(picBuf, PIN_BUG_LEN_DACAI);
//     }
//   }
//   // 剩下的不足2048字符的数据处理，根据迪文处理内容 -- The remaining data with less than 2048 characters will be processed according to the processing content of Diwen
//   watchdog_refresh();
//   if (picLen % PIN_BUG_LEN_DACAI != 0)
//   {
//     memset(picBuf, 0, sizeof(picBuf));
//     gcodePicGetDataFormBase64(picBuf, (picLen - PIN_BUG_LEN_DACAI * j), false);
//     // card.read(picBuf, (picLen - PIN_BUG_LEN_DACAI * j));
//     // 发送图片数据到指定地址 -- Send image data to the specified address
//     if (isDisplay)
//     {
//       uiShow.UI_SendJpegDate(picBuf, picLen - PIN_BUG_LEN_DACAI * j);
//     }
//   }
//   if (isDisplay)
//   {
//     gcodePicDispalyOnOff(jpgAddr, true);
//   }

//   read_gcode_model_information(card.filename);
//   return true;
// }

/**
 * @功能   gcode预览图存在判断
 * @Author Creality
 * @Time   2021-12-01
 * fileName         gcode文件名
 * jpgAddr          显示地址
 * jpgFormat        图片类型（jpg、png）
 * jpgResolution    图片大小
 */
/**
 *@Function gcode preview image existence judgment
 *@Author Creality
 *@Time 2021-12-01
 *fileName gcode file name
 *jpgAddr display address
 *jpgFormat image type (jpg, png)
 *jpgResolution image size
 */
// char gcodePicExistjudge(char *fileName, unsigned int jpgAddr, unsigned char jpgFormat, unsigned char jpgResolution)
// {
//   #define JUDGE_PIC_BUF_CNT   20
//   enum{
//     MENU_PIC_FORMAT,        // 图片格式判断
//     MENU_PIC_HEADER,        // 标识位判断
//     MENU_PIC_RESOLITION,    // 图片分辨率判断
//     MENU_PIC_LEN,           // 数据长度判断
//     MENU_PIC_START_LINE,    // 起始行
//     MENU_PIC_END_LINE,      // 结束行
//     MENU_PIC_HIGH,          // 模型高度
//     MENU_PIC_MAX
//   };
//   unsigned char picMenu = MENU_PIC_FORMAT;
//   unsigned long picLen = 0;     // 图片数据长度
//   unsigned int  i;
//   // unsigned long dataLen = 0; // 包数据长度
//   unsigned char picFormat = PIC_FORMAT_MAX;         // 图片格式
//   unsigned char picResolution = PIC_RESOLITION_MAX; // 图片分辨率
//   unsigned char ret;
//   // unsigned char picGetDataCnt = 0;               // 读取数据计数
//   // char picFileHeader[PIC_PRESENT_LEN] = {0xAA, 0x55, 0xA5, 0x5A};
//   unsigned char buf[20] = {0};
//   unsigned char bufIndex = 0;
//   char lCmdBuf[20];

//   // 查找图片，只从前20字节串查找，
//   for (i = 0; i < JUDGE_PIC_BUF_CNT; i++)
//   {
//     memset(buf, 0, sizeof(buf));
//     bufIndex = 0;
//     int j;
//     for (j = 0; j < 20; j++)
//     {
//       // 从U盘中获取一个字符
//       ret = card.get();
//       // 第一个字符是无效的，重新读取
//       if((ret == ';' || ret == ' ' || ret == '\r' || ret == '\n') && bufIndex == 0)
//       {
//         continue;
//       }
//       else if ((ret == ';' || ret == ' ' || ret == '\r' || ret == '\n') && bufIndex != 0)
//       {
//         break;
//       }
//       buf[bufIndex++] = ret;
//     }
//     if (j == 20) break;
//     memset(lCmdBuf, 0, sizeof(lCmdBuf));
//     sprintf(lCmdBuf, "%s", buf);
//     if (ENABLED(USER_LOGIC_DEUBG))
//     {
//       SERIAL_ECHO(lCmdBuf);
//       SERIAL_ECHO("\r\n");
//     }

//     switch(picMenu)
//     {
//       // 图片的格式判断 jpg或png
//       case MENU_PIC_FORMAT:
//         picFormat = PIC_FORMAT_MAX;
//         if (strcmp(lCmdBuf, FORMAT_JPG) == 0)
//         {
//           picFormat = PIC_FORMAT_JPG;
//         }
//         else if (strcmp(lCmdBuf, FORMAT_PNG) == 0)
//         {
//           picFormat = PIC_FORMAT_PNG;
//         }
//         if (ENABLED(USER_LOGIC_DEUBG))
//         {
//           SERIAL_ECHOLNPAIR("\r\n picMenu = MENU_PIC_FORMAT, lCmdBuf = ", lCmdBuf,
//                             "\r\n picMenu = MENU_PIC_FORMAT, FORMAT_JPG = ", FORMAT_JPG,
//                             "\r\n picMenu = MENU_PIC_FORMAT, picFormat = ", picFormat);
//         }
//         // 判断是否完成标识位判断，完成后进入下一步
//         if (picFormat != PIC_FORMAT_MAX)
//         {
//           picMenu = MENU_PIC_HEADER;
//         }
//         break;

//       case MENU_PIC_HEADER:
//         // 开始标志判断，找到（begin）
//         if (ENABLED(USER_LOGIC_DEUBG)) 
//         {
//           SERIAL_ECHOLNPAIR("\r\n picMenu = MENU_PIC_HEADER, lCmdBuf = ", lCmdBuf,
//                             "\r\n picMenu = MENU_PIC_HEADER, PIC_HEADER = ", PIC_HEADER);
//         }

//         // 判断是否完成包数据长度判断，完成后进图下一步
//         if (strcmp(lCmdBuf, PIC_HEADER) == 0)
//         {
//           picMenu = MENU_PIC_RESOLITION;
//         }
//         break;

//       case MENU_PIC_RESOLITION:
//         // 图片分辨率判断
//         picResolution = PIC_RESOLITION_MAX;
//         if (strcmp(lCmdBuf, RESOLITION_36_36) == 0)
//         {
//           picResolution = PIC_RESOLITION_36_36;
//         }
//         else if (strcmp(lCmdBuf, RESOLITION_48_48) == 0)
//         {
//           picResolution = PIC_RESOLITION_48_48;
//         }
//         else if (strcmp(lCmdBuf, RESOLITION_64_64) == 0)
//         {
//           picResolution = PIC_RESOLITION_64_64;
//         }
//         else if (strcmp(lCmdBuf, RESOLITION_96_96) == 0)
//         {
//           picResolution = PIC_RESOLITION_96_96;
//         }
//         else if (strcmp(lCmdBuf, RESOLITION_144_144) == 0)
//         {
//           picResolution = PIC_RESOLITION_144_144;
//         }
//         else if (strcmp(lCmdBuf, RESOLITION_200_200) == 0)
//         {
//           picResolution = PIC_RESOLITION_200_200;
//         }
//         else if (strcmp(lCmdBuf, RESOLITION_300_300) == 0)
//         {
//           picResolution = PIC_RESOLITION_300_300;
//         }
//         else if (strcmp(lCmdBuf, RESOLITION_600_600) == 0)
//         {
//           picResolution = PIC_RESOLITION_600_600;
//         }

//         if (picResolution != PIC_RESOLITION_MAX)
//         {
//           picMenu = MENU_PIC_LEN;
//         }

//         if (ENABLED(USER_LOGIC_DEUBG))
//         {
//           SERIAL_ECHOLNPAIR("\r\n picMenu = MENU_PIC_RESOLITION, lCmdBuf = ", lCmdBuf);
//         }
//         break;

//       case MENU_PIC_LEN:
//         // 图片长度
//         picLen = atoi(lCmdBuf);
//         picMenu = MENU_PIC_START_LINE;

//         if (ENABLED(USER_LOGIC_DEUBG))
//         {
//           SERIAL_ECHOLNPAIR("\r\n picMenu = MENU_PIC_LEN, lCmdBuf = ", lCmdBuf,
//                             "\r\n picMenu = MENU_PIC_LEN, picLen = ", picLen);
//         }
//         break;

//       case MENU_PIC_START_LINE:
//         // 起始行
//         picMenu = MENU_PIC_END_LINE;

//         if (ENABLED(USER_LOGIC_DEUBG))
//         {
//           SERIAL_ECHOLNPAIR("\r\n picMenu = MENU_PIC_START_LINE, lCmdBuf = ", lCmdBuf);
//         }
//         break;

//       case MENU_PIC_END_LINE:
//         // 结束行
//         picMenu = MENU_PIC_HIGH;

//         if (ENABLED(USER_LOGIC_DEUBG))
//         {
//           SERIAL_ECHOLNPAIR("\r\n picMenu = MENU_PIC_END_LINE, lCmdBuf = ", lCmdBuf);
//         }
//         break;

//       case MENU_PIC_HIGH:
//         // 高度
//         picMenu = MENU_PIC_MAX;
//         if (ENABLED(USER_LOGIC_DEUBG))
//         {
//           SERIAL_ECHOLNPAIR("\r\n picMenu = MENU_PIC_HIGH, lCmdBuf = ", lCmdBuf);
//         }
//         break;

//       default:
//         break;
//     }   // switch(picMenu)

//     if (picMenu == MENU_PIC_MAX)
//     {
//       if (ENABLED(USER_LOGIC_DEUBG))
//       {
//         SERIAL_ECHOLNPAIR("\r\n picMenu = MENU_PIC_MAX, picMenu = ", picMenu);
//       }
//       break;
//     }
//   }

//   if (ENABLED(USER_LOGIC_DEUBG))
//   {
//     // SERIAL_ECHOPAIR("\r\n gcode pic time test 1 msTest = ", (millis() - msTest));
//     // msTest = millis();
//   }
//   // That means we found the photo,
//   if (picMenu == MENU_PIC_MAX)
//   {
//     if (ENABLED(USER_LOGIC_DEUBG))
//     {
//       SERIAL_ECHOLNPAIR("\r\n ...picResolution = ", picResolution,
//                         "\r\n ...picFormat = ", picFormat,
//                         "\r\n ...picLen = ", picLen);
//     }

//     // uint32_t index = card.getIndex1();//card.getIndex();
//     // card.setIndex((index - (JUDGE_PIC_BUF_LEN - i -1)));
//     // if (ENABLED(USER_LOGIC_DEUBG)) 
//     // {
//     //     SERIAL_ECHOLNPAIR("\r\n ...old_index = ", index,
//     //                     "\r\n ...new_index = ", (index - (JUDGE_PIC_BUF_LEN - i -1)));
//     // }
//     // 从gcode里面读出图片数据，根据选择的是不是预定格式或预定大小图片来判断是否需要发送到屏上

//     // 判断是否是需要的分辨率 和 格式
//     if ((picResolution == jpgResolution) && (picFormat == jpgFormat))
//     {
//       gcodePicDataRead(picLen, true, jpgAddr);
//     }
//     else
//     {
//       // 直接移动指针，跳过无效的图片
//       // 协议规定完整一行数据：';' + ' ' + "数据" + '\n'  1+1+76+1 = 79字节
//       // 最后一行为“; png end\r” 或 “; jpg end\r”,
//       // uint32_t index1 = card.getIndex();
//       uint32_t index1 = card.getIndex1();
//       uint32_t picLen1 = 0;
//       if (picLen % 3 == 0)
//       {
//         picLen1 = picLen / 3 * 4;
//       }
//       else
//       {
//         picLen1 = (picLen / 3 + 1) * 4;
//       }
//       uint32_t indexAdd = (picLen1 / 76) * 3 + picLen1 + 10;
//       if ((picLen1 % 76) != 0) 
//       {
//         indexAdd += 3;
//       }

//       card.setIndex((index1 + indexAdd));
//       if (ENABLED(USER_LOGIC_DEUBG)) 
//       {
//         SERIAL_ECHOLNPAIR("\r\n ...old_index1 = ", index1,
//                           "\r\n ...indexAdd = ", indexAdd);
//       }

//       if (picResolution != jpgResolution)
//       {
//         return PIC_RESOLITION_ERR;
//       }
//       else
//       {
//         return PIC_FORMAT_ERR;
//       }
//     }
//   }
//   else
//   {
//     // 该gcode中没有图标
//     // card.closefile();
//     return PIC_MISS_ERR;
//   }
//   // card.closefile();
//   // if (ENABLED(USER_LOGIC_DEUBG)) 
//   //     SERIAL_ECHOPAIR("\r\n gcode pic time test 3 msTest = ", (millis() - msTest));
//   // msTest = millis();
//   return PIC_OK;
// }

/**
 * @功能   gcode预览图发送到迪文
 * @Author Creality
 * @Time   2021-12-01
 * fileName     gcode文件名
 * jpgAddr      显示地址
 * jpgFormat    图片类型（jpg、png）
 * jpgResolution     图片大小
 */
/**
 * @Function gcode preview is sent to Divin
 * @Author Creality
 * @Time 2021-12-01
 * fileName gcode file name
 * jpgAddr display address
 * jpgFormat image type (jpg, png)
 * jpgResolution image size
 *//*
uint8_t gcodePicDataSendToDwin(char *fileName, unsigned int jpgAddr, unsigned char jpgFormat, unsigned char jpgResolution)
{
  char ret;
  char returyCnt = 0;
  SERIAL_ECHO("\r\n gcodePicDataSendToDwin fileName = ");
  SERIAL_ECHO(fileName);
  card.openFileRead(fileName);
  // msTest = millis();
  while (1)
  {
    
    ret = gcodePicExistjudge(fileName, jpgAddr, jpgFormat, jpgResolution);
    // 当gcode中没有pic时，直接返回 --  When there is no pic in gcode, return directly
    if (ret == PIC_MISS_ERR)
    {
      card.closefile();
      return PIC_MISS_ERR;
    }
    else if ((ret == PIC_FORMAT_ERR) || (ret == PIC_RESOLITION_ERR))
    {
      // 当格式或大小错误，继续往下判断 -- When the format or size is wrong, continue to judge.
      if (++returyCnt >= 3)
      {
        card.closefile();
        return PIC_MISS_ERR;
      }
      else
      {
        continue;
      }
    }
    else
    {
      card.closefile();
      return PIC_OK;
    }
  }
  
}
*/



/**
 * @功能   xxxxxxxxxxxxxxxxxxxxxxxx
 * @Author Creality
 * @Time   2021-12-01
 * time : 			gcode预计打印时间
 * Filament used：	材料用量
 * Layer height：	层高
 *   PIC_OK,              // 图片显示ok
  PIC_FORMAT_ERR,      // 图片格式错误
  PIC_RESOLITION_ERR,  // 图片分辨率错误
  PIC_MISS_ERR,        // gcode无图片
 */
// Parse_Only_Picture_Data(char* fileName, char * time, char * FilamentUsed, char * layerHeight)
/**
 *  @function xxxxxxxxxxxxxxxxxxxxxxxx
 *  @Author Creality
 *  @Time 2021-12-01
 *  time: gcode estimated printing time
 *  Filament used： Material usage
 *  Layer height: layer height
 *  PIC_OK, //Picture display ok
    PIC_FORMAT_ERR, //Picture format error
    PIC_RESOLITION_ERR, //Wrong picture resolution
    PIC_MISS_ERR, //gcode no picture
 */
// Parse_Only_Picture_Data(char*fileName, char *time, char *FilamentUsed, char *layerHeight)
// uint8_t gcodePicDataSendToDwin(char *fileName, unsigned int jpgAddr, unsigned char jpgFormat, unsigned char jpgResolution)
// {
//   char ret;
//   // char returyCnt = 0; 

 
 
//   HAL_watchdog_refresh();
//   SERIAL_ECHO("\r\n gcodePicDataSendToDwin fileName = ");
//   SERIAL_ECHO(fileName);
//   card.openFileRead(fileName);
//   // SERIAL_ECHOLNPAIR(" ret333=: ", ret); // rock_20210909
//   // msTest = millis();
//   // while (1)
//   // {
//     ret=read_gcode_model_information(card.filename);
//     // 当gcode中没有pic时，直接返回 -- When there is no pic in gcode, return directly
//     if (ret == PIC_MISS_ERR)
//     {
//       card.closefile();
//       return PIC_MISS_ERR;
//     }
//     /*
//     else if ((ret == PIC_FORMAT_ERR) || (ret == PIC_RESOLITION_ERR))
//     {
//       // 当格式或大小错误，继续往下判断 -- When the format or size is wrong, continue to judge.
//       if (++returyCnt >= 3)
//       {
//         card.closefile();
//         return PIC_MISS_ERR;
//       }
//       // else
//       // {
//       //   continue;
//       // }
//     }
//     */
//     else
//     {
//       card.closefile();
//       return PIC_OK;
//     }
//   // }
  
// }


// char Parse_Only_Picture_Data(char* fileName, char * time, char * FilamentUsed, char * layerHeight)
// {
//    #define STRING_MAX_LEN      60
// 	  unsigned char ret;
//     unsigned char strBuf[STRING_MAX_LEN] = {0};
//     unsigned char bufIndex = 0;
// 	unsigned char buf[10] = {0};
// 	uint8_t retryTimes = 0;	//重复查询次数 -- Number of repeated queries

//   SERIAL_ECHO("\r\n gcodePicDataSendToDwin fileName = ");
//   SERIAL_ECHO(fileName);
//   card.openFileRead(fileName);

//     // 读取一个字符串，以空格隔开 -- Read a string, separated by spaces
//     #define GET_STRING_ON_GCODE()
//         {
//             // 读取一行，以换行符隔开  -- Read one line, separated by newlines
//             memset(strBuf, 0, sizeof(strBuf));
//             int strLenMax;
//             bool strStartFg = false;
//             uint8_t curBufLen = 0;
//             retryTimes = 0;   // 查找次数 -- Number of searches
//             do {
//                 for (strLenMax = 0; strLenMax < STRING_MAX_LEN; strLenMax++)
//                 {
//                     ret = card.get();   // 从U盘中获取一个字符 -- Get a character from USB disk
//                     SERIAL_ECHO_MSG("1:",&ret);
//                     if (ret != ';' && strStartFg == false)  // 读到';'为一行的开始 -- Read ';' as the beginning of a line
//                         continue;
//                     else
//                         strStartFg = true;
//                     if ((ret == '\r' || ret == '\n') && bufIndex != 0) break;   // 读到换行符，退出 -- Read the newline character and exit
//                     strBuf[bufIndex++] = ret;
//                 }
//                 SERIAL_ECHO_MSG("4f56ds:",strBuf);
//                 if (strLenMax >= STRING_MAX_LEN) {
//                     SERIAL_ECHO_MSG("curren srting lenth more than STRING_MAX_LEN(60)");
//                     card.closefile();
//                     return false;	// 返回失败 -- Return failure
//                 }
//                 curBufLen = sizeof(strBuf);
//                 if (retryTimes++ >= 5)
//                 {
//                     SERIAL_ECHO_MSG("retryTimes more than5 times");
//                     card.closefile();
//                     return false;	// 返回失败 -- Return failure
//                 }
//             }while(curBufLen < 20);

// 			      // 调试打印 -- Debug Print
//             SERIAL_ECHO_MSG("strBuf = ", strBuf);
//             SERIAL_ECHO_MSG("curBufLen = ", curBufLen);
//       }


// 	// 获取某一行的指定值 -- Get the specified value of a row
// 	// 例如，获取“;TIME:464.876” 中的 “464.876” -- For example, get "464.876" in ";TIME:464.876"


// 	// 查找TIME -- Find time
// 	retryTimes = 0;
// 	do {
// 		GET_STRING_ON_GCODE();
    
// 		memset(buf, 0, sizeof(buf));
// 		if ( strstr((const char *)strBuf, "TIME" ) == NULL) {
// 			sscanf((const char *)strBuf,"%s:%s",&buf, time);

// 			// 调试打印信息 -- Debug print information
// 			SERIAL_ECHO_MSG("buf = ", buf);
//             SERIAL_ECHO_MSG("time = ", time);
// 			break;
// 		}

// 		if (retryTimes++ >= 3) 
//     {
//       card.closefile();
//       return false;	// 超过3次，返回失败 -- More than 3 times, return failure
//     }
// 	}while(1);


// 	// 查找 "Filament used" -- Find "Filament used"
// 	retryTimes = 0;
// 	do {
// 		GET_STRING_ON_GCODE();
// 		memset(buf, 0, sizeof(buf));
// 		if ( strstr((const char *)strBuf, "Filament used" ) == NULL) {
// 			sscanf((const char *)strBuf,"%s:%s",&buf, FilamentUsed);

// 			// 调试打印信息 -- Debug print information
// 			SERIAL_ECHO_MSG("buf = ", buf);
//             SERIAL_ECHO_MSG("time = ", FilamentUsed);
// 			break;
// 		}

// 		if (retryTimes++ >= 3) 
//     {
//       card.closefile();
//       return false;	// 超过3次，返回失败 -- More than 3 times, return failure
//     }
// 	}while(1);

// 	// 查找 "Layer height" -- Find "Layer height"
// 	retryTimes = 0;
// 	do {
// 		GET_STRING_ON_GCODE();
// 		memset(buf, 0, sizeof(buf));
// 		if ( strstr((const char *)strBuf, "Layer height" ) == NULL) {
// 			sscanf((const char *)strBuf,"%s:%s",&buf, layerHeight);

// 			// 调试打印信息 -- Debug print information
// 			SERIAL_ECHO_MSG("buf = ", buf);
//             SERIAL_ECHO_MSG("time = ", layerHeight);
// 			break;
// 		}

// 		if (retryTimes++ >= 3) 
//     {
//       card.closefile();
//       return false;	// 超过3次，返回失败 -- -- More than 3 times, return failure
//     }
// 	}while(1);
// }

////////////////////////////