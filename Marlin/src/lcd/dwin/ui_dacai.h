#ifndef UI_DACAI_H
#define UI_DACAI_H

#include "string.h"
#include <Arduino.h>

#include "../../inc/MarlinConfig.h"
#include "../../module/temperature.h"


#define RegAddr_W   0x80
#define RegAddr_R   0x81
#define VarAddr_W   0x82
#define VarAddr_R   0x83
#define ExchangePageBase    ((unsigned long)0x5A010000)
#define StartSoundSet       ((unsigned long)0x060480A0)
#define ExchangepageAddr    0x0084

// #define DACAI_JPG_BYTES_PER_FRAME (2048)    //下载包大小
// #define DACAI_JPG_BYTES_PER_BUF (DACAI_JPG_BYTES_PER_FRAME + 4)    //下载包大小

// 这部分为“命令类型 ”
#define NOTIFY_TOUCH_PRESS         0X01  // 屏幕按下通知
#define NOTIFY_TOUCH_RELEASE       0X03  // 屏幕松开通知
#define NOTIFY_WRITE_FLASH_OK      0X0C  // 写FLASH成功
#define NOTIFY_WRITE_FLASH_FAILD   0X0D  // 写FLASH失败
#define NOTIFY_READ_FLASH_OK       0X0B  // 读flash成功
#define NOTIFY_READ_FLASH_FAILD    0X0F  // 读flash失败
#define NOTIFY_MENU                0X14  // 菜单事件通知
#define NOTIFY_TIMER               0X43  // 定时器超时通知
#define NOTIFY_CONTROL             0XB1  // 控件更新通知
#define NOTIFY_READ_RTC            0XF7  // 读取RTC变化通知
#define MSG_GET_CURRENT_SCREEN     0X01  // 画面ID变化通知
#define MSG_GET_DATA               0X11  // 控件数据通知
#define NOTIFY_HandShake           0X55  // 握手通知

// 往屏幕发送的指令
#define CMD_SET_CHANGE_LNGUAGE     0xC1  // 修改语言

// 这部分为“命令类型” + “消息类型”
// 定义了这些为发送给大彩屏时，用到的指令
#define CMD_GOTO_SCREEN     0xB100  // 切换页面

#define CMD_SEND_TO_SCREEN  0xB110  // 发送到屏幕端的命令

#define CMD_ICON_SHOW_HIDE  0xB103  // 隐藏、显示控件
#define CMD_ICON_START      0xB120  // 启动动画播放
#define CMD_ICON_STOP       0xB121  // 停止动画播放
#define CMD_ICON_PAUSE      0xB122  // 暂停动画播放
#define CMD_ICON_SET_FRAME  0xB123  // 设置与“图标控件”、“动画控件” 的显示（指定帧播放）

#define CMD_TEXT_TWINKLE    0xB115  // 文本-闪烁周期(10毫秒单位)，0表示不闪烁
#define CMD_TEXT_ROLL       0xB116  // 文本-文本滚动速度(每秒移动像素)，0表示不滚动
#define CMD_TEXT_COLOR_FORE 0xB119  // 文本-前景色

#define CMD_HEAD    0XEE            // 帧头
#define CMD_TAIL    0XFFFCFFFF      // 帧尾

#define FBONE   (0xFF)
#define FBTWO   (0xFC)
#define FBTHREE (0xFF)
#define FBFOUR  (0xFF)

#define VAL_DEFAILT 0xFFFF

// #define SizeofDatabuf    300

/* 文字颜色定义 */
#define COLOE_WHITE 0xFFFF
#define COLOR_GREY  0x8410

/* JPG 图片显示相关定义 */
// #define DACAI_JPG_BYTES_PER_PACKET 240   // 每一帧发送的字节数（图片数据）
// #define DACAI_JPG_WORD_PER_PACKET  DACAI_JPG_BYTES_PER_PACKET/2   //每一帧发送的字数（图片数据）

#pragma pack(1)
typedef struct DacaiDataBuf
{
  uint8_t head;          // 帧头：0xEE
  uint8_t cmd_type;      // 命令类型 
  uint8_t ctrl_msg;      // 消息类型
  uint16_t screen_id;    // 屏幕ID
  uint16_t control_id;   // 地址（控件ID）
  uint8_t  control_type; // 控件类型
  // unsigned long bytelen;
  unsigned char data[300];
  uint16_t bytelen; 
} DDB;
#pragma pack(0)

// 这部分为按钮控件类型（control_type）
enum CtrlType
{
  KEY_UNKNOWN     = 0x0,      // 未知状态
  KEY_BUTTON      = 0x10,     // 按钮
  KEY_TEXT        = 0x11,     // 文本
  KEY_PROGRESS    = 0x12,     // 进度条
  KEY_SLIDER      = 0x13,     // 滑动条
  KEY_METER       = 0x14,     // 仪表
  KEY_DROPLIST    = 0x15,     // 下拉列表
  KEY_ANIMATION   = 0x16,     // 动画
  KEY_RTC         = 0x17,     // 时间显示
  KEY_GRAPH       = 0x18,     // 图表
  KEY_TABLE       = 0x19,     // 表格控件
  KEY_MENU        = 0x1A,     // 菜单控件
  KEY_SELECTOR    = 0x1B,     // 选择控件
  KEY_QR_CODE     = 0x1C,     // 二维码
};

// 单位定义
enum UnitType
{
  U_NULL       = 0x00,
  U_PERCENT    = 0x01,      // %
  U_MM         = 0x02,      // mm
  U_CENTIGRADE = 0x03,      // ℃
  U_MAX,
};

// /* 基础图形变量--图片的复制结构体 */
// typedef struct
// {
//   unsigned long addr;   // 变量地址
//   unsigned long cutPage;// 剪切图片所在页面
//   unsigned int  cutLeftUp_X;  // 剪切图片的左上角X
//   unsigned int  cutLeftUp_Y;  // 剪切图片的左上角Y
//   unsigned int  cutRightDown_X;  // 剪切图片的右下角X
//   unsigned int  cutRightDown_Y;  // 剪切图片的右下角Y
//   unsigned int  paste_X;// 粘贴页面坐标--X (左上角)
//   unsigned int  paste_Y;// 粘贴页面坐标--Y (左上角)
// }DwinCopyPic_t;

// /* 区域亮度调节变量--区域亮度调节构体 */
// typedef struct
// {
//     unsigned long addr;         // 变量地址
//     unsigned long spAddr;       // 描述指针
//     unsigned int  brightness;   // 亮度（0x0000 - 0x0100, 单位为 1/256）
//     unsigned int  LeftUp_X;     // 显示区域的左上角X
//     unsigned int  LeftUp_Y;     // 显示区域的左上角Y
//     unsigned int  RightDown_X;  // 显示区域的右下角X
//     unsigned int  RightDown_Y;  // 显示区域的右下角Y
// }DwinBrightness_t;

// class UI_SHOW_t {
//   public:
//     uint8_t IsDacaiScreenConnect;
//     UI_SHOW_t();
//     void RTS_Init(void);

//     void RTS_SndData(void);
//     void RTS_SndData(const String &, unsigned long, unsigned long, unsigned long = CMD_SEND_TO_SCREEN);
//     void RTS_SndData(const char[],   unsigned long, unsigned long, unsigned long = CMD_SEND_TO_SCREEN);
//     void RTS_SndData(char,           unsigned long, unsigned long, unsigned long = CMD_SEND_TO_SCREEN);
//     void RTS_SndData(unsigned char*, unsigned long, unsigned long, unsigned long = CMD_SEND_TO_SCREEN);
//     void RTS_SndData(int,            unsigned long, unsigned long, unsigned long = CMD_SEND_TO_SCREEN);
//     void RTS_SndData(unsigned int,   unsigned long, unsigned long, unsigned long = CMD_SEND_TO_SCREEN);
//     void RTS_SndData(float,          unsigned long, unsigned long, unsigned long = CMD_SEND_TO_SCREEN);
//     void RTS_SndData(long,           unsigned long, unsigned long, unsigned long = CMD_SEND_TO_SCREEN);
//     void RTS_SndData(unsigned long,  unsigned long, unsigned long, unsigned long = CMD_SEND_TO_SCREEN);
//     void RTS_SndData(int,            unsigned long = CMD_GOTO_SCREEN);

//     bool UI_Handshake(void);
//     void UI_GotoScreen(int);  // 切换界面
//     void UI_ShowIcon(int,               unsigned long, unsigned long, unsigned long = CMD_ICON_SET_FRAME);                   // 图标控件显示、动画控件显示某一帧;图标动画控件 开始、停止、暂停等
//     void UI_ShowOrHideConctrols(bool,        unsigned long, unsigned long, unsigned long = CMD_ICON_SHOW_HIDE);              // 显示、隐藏控件
//     void UI_ShowText(char*,             unsigned long, unsigned long, unsigned long = CMD_SEND_TO_SCREEN);                   // 发送文本数据
//     void UI_ShowText(int,               unsigned long, unsigned long, unsigned long = CMD_SEND_TO_SCREEN, uint8_t = U_NULL); // 发送文本数据
//     void UI_ShowText(float, uint8_t,    unsigned long, unsigned long, unsigned long = CMD_SEND_TO_SCREEN, uint8_t = U_NULL); // 发送文本数据
//     void UI_ShowTextClear(              unsigned long, unsigned long, unsigned long = CMD_SEND_TO_SCREEN);                   // 发送清空文本数据

//     void UI_ShowTextUnicode(const char* , const int, unsigned long , unsigned long , unsigned long = CMD_SEND_TO_SCREEN);

//     bool UI_SendJpegDateHandshake(unsigned long size, const char *path);
//     bool UI_SendJpegDate(const char *jpeg, unsigned long size);
//     void UI_DisplayJpegDate(unsigned long _screen_id);
//     void UI_ChangeLanguage(unsigned int lang);

//     DDB recdat;
//     DDB snddat;
//     private:
//     // unsigned char databuf[SizeofDatabuf];
//     unsigned char frameEnd[4];
//   };

// extern UI_SHOW_t uiShow;

// #define DWIN_DEBUG
#endif
