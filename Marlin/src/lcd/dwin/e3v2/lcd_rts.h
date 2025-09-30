#ifndef LCD_RTS_H
#define LCD_RTS_H

#include "../../../inc/MarlinConfigPre.h"

/*****************  gcode内嵌预览图读取相关定义(开始)  ******************/
// 图片文件标识位()
#define PIC_HEADER      "begin"
// 图片类型()
// enum{
//   PIC_FORMAT_JPG    = 0x00, // jpg格式图片
//   PIC_FORMAT_PNG    = 0x01, // png格式图片
//   PIC_FORMAT_MAX    = 0x02,
// };

// #define FORMAT_JPG "jpg"
// #define FORMAT_PNG "png"

// // 分辨率()
// #define PIC_RESOLITION_LEN      1  // 图片分辨率长度（字节）
// enum{
//   PIC_RESOLITION_36_36   = 0x00,   // 分辨率 = 36*36
//   PIC_RESOLITION_48_48   = 0x01,   // 分辨率 = 48*48
//   PIC_RESOLITION_64_64   = 0x02,   // 分辨率 = 64*64
//   PIC_RESOLITION_96_96   = 0x03,   // 分辨率 = 96*96
//   PIC_RESOLITION_144_144 = 0x04,   // 分辨率 = 144*144
//   PIC_RESOLITION_200_200 = 0x05,   // 分辨率 = 200*200
//   PIC_RESOLITION_300_300 = 0x06,   // 分辨率 = 300*300
//   PIC_RESOLITION_600_600,
//   PIC_RESOLITION_MAX,              // gcode无图片
// };

// #define RESOLITION_36_36    "36*36"
// #define RESOLITION_48_48    "48*48"
// #define RESOLITION_64_64    "64*64"
// #define RESOLITION_96_96    "96*96"
// #define RESOLITION_144_144  "144*144"
// #define RESOLITION_200_200  "200*200"
// #define RESOLITION_300_300  "300*300"
// #define RESOLITION_600_600  "600*600"

// 函数返回信息
enum{
  PIC_OK,              // 图片显示ok
  PIC_FORMAT_ERR,      // 图片格式错误
  PIC_RESOLITION_ERR,  // 图片分辨率错误
  PIC_MISS_ERR,        // gcode无图片
};

enum{
  METADATA_PARSE_OK,
  METADATA_PARSE_ERROR,
};

#define _GCODE_METADATA_STRING_LENGTH_MAX 80
#define _MAX_LINES_TO_PARSE 50

typedef struct _model_information_t
{
  char filament[15];
  char height[15];
  char MINX[15];
  char MINY[15];
  char MINZ[15];
  char MAXX[15];
  char MAXY[15];
  char MAXZ[15];
}model_information_t;

extern model_information_t model_information;

#if ENABLED(DWIN_CREALITY_480_LCD)
  #define PRIWIEW_PIC_FORMAT_NEED         PIC_FORMAT_JPG
  #define PRIWIEW_PIC_RESOLITION_NEED     PIC_RESOLITION_200_200
  #define PRINT_PIC_FORMAT_NEED           PIC_FORMAT_JPG
  #define PRINT_PIC_RESOLITION_NEED       PIC_RESOLITION_200_200
#elif ENABLED(DWIN_CREALITY_320_LCD)
  #define PRIWIEW_PIC_FORMAT_NEED         PIC_FORMAT_JPG
  #define PRIWIEW_PIC_RESOLITION_NEED     PIC_RESOLITION_96_96
  #define PRINT_PIC_FORMAT_NEED           PIC_FORMAT_JPG
  #define PRINT_PIC_RESOLITION_NEED       PIC_RESOLITION_96_96
#endif

// #define DACAI_PRIWIEW_PIC_ADDR1  "3:/0.jpg"        // 大彩文件选择预览图1
// #define DACAI_PRIWIEW_PIC_ADDR2  "3:/0.jpg"        // 大彩文件选择预览图2
// #define DACAI_PRIWIEW_PIC_ADDR3  "3:/0.jpg"        // 大彩文件选择预览图3
// #define DACAI_PRIWIEW_PIC_ADDR4  "3:/0.jpg"        // 大彩文件选择预览图4
// #define DACAI_PRINT_PIC_ADDR     "3:/0.jpg"        // 大彩打印预览图1

/*****************  gcode内嵌预览图读取相关定义(结束)  ******************/

/**
 * JPG-variable address
 * in range: 0xB000 ~ 0xFFFF
 */
// #define VP_OVERLAY_PIC_PREVIEW_1          0x0003              /* 显示第一张预览图的地址 */
// #define VP_OVERLAY_PIC_PREVIEW_2          0xB800              /* 显示第二张预览图的地址 */
// #define VP_OVERLAY_PIC_PREVIEW_3          0xC000              /* 显示第三张预览图的地址 */
// #define VP_OVERLAY_PIC_PREVIEW_4          0xC800              /* 显示第四张预览图的地址 */
// #define VP_OVERLAY_PIC_INTERVAL           0x800               /* 显示预览图的地址间隔 */
// #define VP_OVERLAY_PIC_PTINT              0xB000              /* 打印界面的预览图 */
// #define VP_OVERLAY_PIC_PTINT_DACAI        0xB001              /* 打印界面的预览图 -- 特殊处理大彩屏，预览图地址不能重复 */

// extern uint8_t gcodePicDataSendToDwin(char *, unsigned int , unsigned char , unsigned char );
// extern uint8_t OctoDWINPreview();
extern uint8_t read_gcode_model_information(const char* fileName);
// extern char Parse_Only_Picture_Data(char* fileName,char * time, char * FilamentUsed, char * layerHeight);
#endif