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
#pragma once

/**
 * DWIN by Creality3D
 */

#include "../dwin_lcd.h"
#include "rotary_encoder.h"
#include "../../../libs/BL24CXX.h"

#include "../../../inc/MarlinConfigPre.h"

#include "../../../src/libs/duration_t.h"

#if ANY(HAS_HOTEND, HAS_HEATED_BED, HAS_FAN) && PREHEAT_COUNT
  #define HAS_PREHEAT 1
  #if PREHEAT_COUNT < 2
    #error "Creality DWIN requires two material preheat presets."
  #endif
#endif

//暂停恢复前的进料动作。
//暂停恢复前的进料动作。
#define FEEDING_DEF_DISTANCE                10                // in material: default distance of feeding material
#define FEEDING_DEF_SPEED                   4                 // in material: default speed of feeding

#define LEVEL_DISTANCE   50  //一键对高回抽距离，
#define FEEDING_DEF_DISTANCE_1              15                /* in material: default distance of feeding material默认进料距离 */ 
#define IN_DEF_DISTANCE                 90               //默认进出料距离
// #define FEEDING_DEF_SPEED       5                   // in material: default speed of feeding   _MMS

#define FEED_TEMPERATURE 240  //Feed temperature进料温度
#define EXIT_TEMPERATURE 240 //出料温度
#define STOP_TEMPERATURE 140 //停止温度

#if ENABLED(SHOW_GRID_VALUES)  //如果显示调平值
  #define Flat_Color        0x7FE0  //草坪绿 -->平坦颜色
  #define Relatively_Flat   0x1C9F  //道奇蓝色 --> 较平坦颜色
  // #define Slope_Small       0xF81F  //洋红色 -->小倾斜
  #define Slope_Small       0xFFE0  //黄色 -->小倾斜 
  #define Slope_Big         0xF800  //深红色 -->大倾斜
  
  #define  Select_Block_Color 0xFFE0 //黄色色 选中块颜色

  #define Slope_Big_Max        2.0  //大倾斜上边界值
  #define Slope_Big_Min       -2.0  //大倾斜下边界值
  #define Slope_Small_Max      1.0  //小倾斜上边界值
  #define Slope_Small_Min     -1.0  //小倾斜下边界值
  #define Relatively_Flat_Max  0.5  //较平坦上边界值
  #define Relatively_Flat_Min -0.5  //较平坦下边界值

                                    // 0.5~ -0.5的数据代表平坦比较平坦


#endif
enum Auto_Hight_Stage:uint8_t{Nozz_Start,Nozz_Hot,Nozz_Clear,Nozz_Hight,Nozz_Finish};  //一键对高的四个阶段
enum processID : uint8_t {
  // Process ID
  MainMenu,
  SelectFile,
  Prepare,
  Control,
  Leveling,
  Level,
  PrintProcess,
  AxisMove,
  TemperatureID,
  Motion,
  Info,
  Pstats,
  Tune,
  #if HAS_PREHEAT
    PLAPreheat,
    TPUPreheat,
    PETGPreheat,
    ABSPreheat,
  #endif
  MaxSpeed,
  MaxSpeed_value,
  MaxAcceleration,
  MaxAcceleration_value,
  MaxJerk,
  MaxJerk_value,
  Step,
  Step_value,
  InputShaping,
  SkewCorrection,
  skewxy_dac,
  skewxy_dbd,
  skewxy_sad,
  InputShaping_XFreq,
  InputShaping_XZeta,
  InputShaping_YFreq,
  InputShaping_YZeta,
  CExtrude_Menu,
  custom_extrude_temp,
  custom_extrude_length,
  Display_Menu,
  Max_LCD_Bright,
  Dimm_Bright,
  DimmTime,
  HomeOff,
  HomeOffX,
  HomeOffY,
  HomeOffZ,

  // Last Process ID
  Last_Prepare,

  // Advance Settings  //20210724 _rock
  AdvSet,
  ProbeOff,
  ProbeOffX,
  ProbeOffY,

  // Back Process ID
  Back_Main,
  Back_Print,

  // Date variable ID
  Move_X,
  Move_Y,
  Move_Z,
  #if HAS_HOTEND
    Extruder,
    ETemp,
  #endif
  Homeoffset,
  #if HAS_HEATED_BED
    BedTemp,
  #endif
  #if HAS_FAN
    FanSpeed,
  #endif
  PrintSpeed,

  // Window ID
  Print_window,
  Filament_window,
  Popup_Window,
  // remove card screen
  Remove_card_window,
  Show_gcode_pic,
  Selectlanguage,
  Poweron_select_language,
  HM_SET_PID,            // 手动设置PID
  AUTO_SET_PID,          // 自动设置PID
  AUTO_SET_NOZZLE_PID,   // 自动设置喷嘴PID
  AUTO_SET_BED_PID,      // 自动设置热床PID
  HM_PID_Value,          // 手动PID值
  AUTO_PID_Value,        // 自动PID值
  PID_Temp_Err_Popup,    // PID温度异常弹窗
  AUTO_IN_FEEDSTOCK, //自动进料
  AUTO_OUT_FEEDSTOCK, //自动退料
  Level_Value_Edit,  //编辑调平数据
  Change_Level_Value, //改变调平值
  ONE_HIGH, //一键对高页面
  POPUP_CONFIRM,//弹窗确定界面
  Max_GUI, 
  M117Info,
};

enum DC_language{
  Chinese   = 2, /*中文*/
  English   = 4, /*英语*/
  German    = 6, /*德语*/
  Russian   = 9, /*俄语*/
  French    = 12, /*法语*/
  Turkish   = 15, /*土耳其语*/
  Spanish   = 17, /*西班牙语*/
  Italian   = 19, /*意大利语*/
  Portuguese= 21, /*葡萄牙语*/
  Japanese = 23,  /*日语*/
  Korean   = 25,  /*韩语*/
  Language_Max    /*语言边界值*/
};
//开机引导步骤
#define LANGUAGE_TOTAL 11  //总的语言个数
enum DC_Boot_Step{
  Set_language,    //设置语言
  Set_high,        //设置对高
  Set_levelling,   //设置调平界面
  Boot_Step_Max    /*开机引导边界值*/
};
//开机引导弹框界面
enum DWIN_Poupe{
  Clear_nozz_bed,//提示清洁喷嘴和热床
  High_faild_clear,//对高失败，请清洁喷嘴和热床
  Level_faild_QR, //调平失败请扫描二维码，获取解决方案
  Boot_undone,  //开机引导未完成
  CRTouch_err,  //CRTouch异常，
  UnknownError, // Used for future MarlinUI integration
};
extern enum DC_language current_language;
// Picture ID
#define Background_ICON     27//12  // 背景图片ID 27-31 =256*5KB
#define Start_Process       0   // 启动画面图片
#define Auto_Set_Nozzle_PID 1   // 自动设置喷嘴PID
#define Auto_Set_Bed_PID    2   // 自动设置热床PID

#define Background_min  4 //03
#define Background_max  42//53//53 + 50
#define Background_reset 43 //恢复出厂设置
#define BG_PRINTING_CIRCLE_MIN 145 //打印中进度条0%
#define BG_PRINTING_CIRCLE_MAX 245 //打印中进度条100%

#define BG_NOZZLE_MIN  104
#define BG_NOZZLE_MAX  118//123
#define BG_BED_MIN     125 
#define BG_BED_MAX     139//144
// ICON ID
#define ICON                       0//2//13
#define ICON_LOGO                  0
#define ICON_Print_0               1
#define ICON_Print_1               2
#define ICON_Prepare_0             3
#define ICON_Prepare_1             4
#define ICON_Control_0             5
#define ICON_Control_1             6
#define ICON_Leveling_0            7
#define ICON_Leveling_1            8
#define ICON_HotendTemp            9
#define ICON_BedTemp              10
#define ICON_Speed                11
#define ICON_Zoffset              12
#define ICON_Back                 13
#define ICON_File                 14
#define ICON_PrintTime            15
#define ICON_RemainTime           16
#define ICON_Setup_0              17
#define ICON_Setup_1              18
#define ICON_Pause_0              19
#define ICON_Pause_1              20
#define ICON_Continue_0           21
#define ICON_Continue_1           22
#define ICON_Stop_0               23
#define ICON_Stop_1               24
//#define ICON_Bar                  25
#define ICON_More                 26

#define ICON_Axis                 27
#define ICON_CloseMotor           28
#define ICON_Homing               29
#define ICON_SetHome              30
#define ICON_PLAPreheat           31
#define ICON_ABSPreheat           32
#define ICON_Cool                 33
#define ICON_Language             34

#define ICON_MoveX                35
#define ICON_MoveY                36
#define ICON_MoveZ                37
#define ICON_Extruder             38
#define ICON_Alignheight          39

#define ICON_Temperature          40
#define ICON_Motion               41
#define ICON_WriteEEPROM          42
#define ICON_ReadEEPROM           43
#define ICON_ResumeEEPROM         44
#define ICON_Info                 45

#define ICON_SetEndTemp           46
#define ICON_SetBedTemp           47
#define ICON_FanSpeed             48
#define ICON_SetPLAPreheat        49
#define ICON_SetABSPreheat        50

#define ICON_MaxSpeed             51
#define ICON_MaxAccelerated       52
#define ICON_MaxJerk              53
#define ICON_Step                 54
#define ICON_PrintSize            55
#define ICON_Version              56
#define ICON_Contact              57
#define ICON_StockConfiguraton    58
#define ICON_MaxSpeedX            59
#define ICON_MaxSpeedY            60
#define ICON_MaxSpeedZ            61
#define ICON_MaxSpeedE            62
#define ICON_MaxAccX              63
#define ICON_MaxAccY              64
#define ICON_MaxAccZ              65
#define ICON_MaxAccE              66
#define ICON_MaxSpeedJerkX        67
#define ICON_MaxSpeedJerkY        68
#define ICON_MaxSpeedJerkZ        69
#define ICON_MaxSpeedJerkE        70
#define ICON_StepX                71
#define ICON_StepY                72
#define ICON_StepZ                73
#define ICON_StepE                74
#define ICON_Setspeed             75
#define ICON_SetZOffset           76
//#define ICON_Rectangle            77 //手动绘制的矩形替代
#define ICON_BLTouch              78
#define ICON_TempTooLow           79  //喷嘴或者热床温度过低
//#define ICON_AutoLeveling         80
#define ICON_TempTooHigh          81  //喷嘴或者热床温度过高
#define ICON_Hardware_version     82   // 硬件版本小图标
#define ICON_LEVELING_ERR         83   // 调平失败
#define ICON_HIGH_ERR             84    //对高失败


//#define ICON_Continue_E           85   // 确定英文按钮

//#define ICON_Cancel_C             86   // 取消中文按钮
//#define ICON_Cancel_E             87   // 取消英文按钮
//#define ICON_Confirm_C            88   // 确定中文按钮
//#define ICON_Confirm_E            89   // 确定英文按钮
//#define ICON_Info_0               90
//#define ICON_Info_1               91
//#define ICON_Power_On_Home_C      92   // ICON_Degree   92

//#define  ICON_Card_Remove_C       93   // 拔卡中文提示
//#define  ICON_Card_Remove_E       94   // 拔卡英文提示

#define ICON_HM_PID               95
#define ICON_Auto_PID             96
#define ICON_Feedback             97
#define ICON_Level_ERR_QR_CH     98   //
#define ICON_Level_ERR_QR_EN     99
#define ICON_HM_PID_NOZZ_P       100
#define ICON_HM_PID_NOZZ_I       101
#define ICON_HM_PID_NOZZ_D       102
#define ICON_HM_PID_Bed_P        103
#define ICON_HM_PID_Bed_I        104
#define ICON_HM_PID_Bed_D        105
// 这里的编号和图片是对应的
#define ICON_Auto_PID_Nozzle     106 // 自动设置喷嘴PID曲线
#define ICON_Auto_PID_Bed        107 // 自动设置热床PID曲线
#define ICON_Edit_Level_Data      108 //编辑调平数据

#define ICON_Defaut_Image        143

#define ICON_Word_CN 150
#define ICON_Word_EN 151
#define ICON_Word_FR 152
#define ICON_Word_PT 153
#define ICON_Word_TR 154
#define ICON_Word_DE 155
#define ICON_Word_ES 156
#define ICON_Word_IT 157
#define ICON_Word_PYC 158
#define ICON_Word_JP 159

#define ICON_Word_KOR 160  //韩语

#define ICON_OUT_STORK 166  //进料小图标
#define ICON_IN_STORK 167 //退料小图标
#define ICON_STORKING_TIP 168 //进料中提示
#define ICON_STORKED_TIP  169 //进料完成提示
#define ICON_OUT_STORKING_TIP 170 //退料中提示
#define ICON_OUT_STORKED_TIP 171  //退料完成提示
//166-172一键对高逻辑的小图标
#define ICON_NOZZLE_HIGH 172   //对高完成
#define ICON_NOZZLE_HIGH_HOT 173 //未对高
#define ICON_NOZZLE_HEAT 174   //加热完成
#define ICON_NOZZLE_HEAT_HOT 175 //未加热
#define ICON_NOZZLE_CLEAR 176    //喷嘴清洁未开始
#define ICON_NOZZLE_CLEAR_HOT 177 //喷嘴清洁完成
#define ICON_NOZZLE_LINE 178  //一键对高连接线
#define ICON_LEVEL_CALIBRATION_OFF 179//调平校准关
#define ICON_LEVEL_CALIBRATION_ON  180 //调平校准开


#define ICON_FLAG_MAX 181

/* LANGUAGE ID Chinese*/
  #define LANGUAGE_ID              4//14
  // Main
  #define LANGUAGE_Main             1
  #define LANGUAGE_Print_0          2
  #define LANGUAGE_Prepare_0        3
  #define LANGUAGE_Control_0        4
  #define LANGUAGE_Info_0           5
  #define LANGUAGE_Stop_0           6
  #define LANGUAGE_Pause_0          7
  #define LANGUAGE_Print_1          8
  #define LANGUAGE_Prepare_1        9
  #define LANGUAGE_Control_1        10
  #define LANGUAGE_Info_1           11
  #define LANGUAGE_Stop_1           12
  #define LANGUAGE_Pause_1          13
  #define LANGUAGE_Level_0          14
  #define LANGUAGE_Level_1          15
 // Printing
  #define LANGUAGE_Zoffset          16
  #define LANGUAGE_Setup            17
  #define LANGUAGE_PrintTime        18
  #define LANGUAGE_PrintSpeed       19
  #define LANGUAGE_Printing         20
  #define LANGUAGE_Back             21
  #define LANGUAGE_Fan              22
  #define LANGUAGE_Hotend           23
  #define LANGUAGE_Bedend           24
  #define LANGUAGE_RemainTime       25
  #define LANGUAGE_SelectFile       26
  #define LANGUAGE_Pausing          27
  // Control
  #define LANGUAGE_Store            28
  #define LANGUAGE_Read             29
  #define LANGUAGE_Reset            30
  #define LANGUAGE_Temp             31
  #define LANGUAGE_Motion           32
  #define LANGUAGE_Motion_Title     33
  // Prepare
  #define LANGUAGE_ABS              34
  #define LANGUAGE_PLA              35
  #define LANGUAGE_Home             36
  // #define LANGUAGE_SetHome       37
  #define LANGUAGE_info_new         37
  #define LANGUAGE_CloseMotion      38
  #define LANGUAGE_Move_Title       39
  #define LANGUAGE_Prepare          40
  #define LANGUAGE_Cool             41
  // Move
  #define LANGUAGE_MoveX            42
  #define LANGUAGE_MoveY            43
  #define LANGUAGE_MoveZ            44
  #define LANGUAGE_MoveE            45
  #define LANGUAGE_Move             46
  // Temperpare
  #define LANGUAGE_PLASetup         47
  #define LANGUAGE_ABSSetup         48
  #define LANGUAGE_Temp_Title       49
  // Motion
  #define LANGUAGE_X                50
  #define LANGUAGE_Y                51
  #define LANGUAGE_Z                52
  #define LANGUAGE_E                53
  #define LANGUAGE_Step             54
  #define LANGUAGE_Acc              55
  #define LANGUAGE_Corner           56
  #define LANGUAGE_MaxSpeed         57
  // Info
  #define LANGUAGE_Version          58
  #define LANGUAGE_Size             59
  #define LANGUAGE_Contact          60
  #define LANGUAGE_Info             61
  // Preheat Configuration
  #define LANGUAGE_PLASetup_Title   62
  #define LANGUAGE_PLASetupSave     63
  #define LANGUAGE_ABSSetup_Title   64
  #define LANGUAGE_ABSSetupSave     65
  // Language
  #define LANGUAGE_language         66
  // Popup Window
  #define LANGUAGE_FilamentLoad     67
  #define LANGUAGE_FilamentUseup    68
  #define LANGUAGE_TempLow          69   // 喷嘴温度过低
  #define LANGUAGE_PowerLoss        70
  #define LANGUAGE_TempHigh         71
  #define LANGUAGE_Cancel           72   // 取消按钮
  #define LANGUAGE_Confirm          73   // 确定按钮
  #define LANGUAGE_Homing           74
  #define LANGUAGE_waiting          75
  #define LANGUAGE_PausePrint       76
  #define LANGUAGE_StopPrint        77
  // Add
  #define LANGUAGE_Setup_0          78
  #define LANGUAGE_Setup_1          79
  #define LANGUAGE_Control          80
  #define LANGUAGE_Finish           81
  #define LANGUAGE_PrintFinish      82   // 确定按钮
  #define LANGUAGE_Card_Remove_JPN  83
  #define LANGUAGE_Homeing          84
  #define LANGUAGE_leveing          85

#define LANGUAGE_mjerk_title        86
#define LANGUAGE_ratio_title        87
#define LANGUAGE_mspeed_title       88
#define LANGUAGE_maccel_title       89
// new_add rock_202220302
#define LANGUAGE_recard_OK          90   // 拔卡确定按钮
#define LANGUAGE_align_height       91   // 一键对高
#define LANGUAGE_filament_cancel    92   // 断料恢复停止按钮
#define LANGUAGE_keep_print_0       93
#define LANGUAGE_keep_print_1       94
#define LANGUAGE_print_stop         95   // 停止按钮
#define LANGUAGE_Powerloss_go       96   // 断电续打继续打印按钮
// 20220819 rock_add
//#define LANGUAGE_Laser_switch       97   // 切换激光雕刻
#define LANGUAGE_PID_Manually       98   // 手动设置PID
#define LANGUAGE_Auto_PID           99   // 自动设置PID
// PLA设置
#define LANGUAGE_PLA_FAN              100
#define LANGUAGE_PLA_NOZZLE           101
#define LANGUAGE_PLA_BED              102
// ABS设置
#define LANGUAGE_ABS_NOZZLE           103
#define LANGUAGE_ABS_BED              104
#define LANGUAGE_ABS_FAN              105

// 最大速度
#define LANGUAGE_MAX_SPEEDX          107
#define LANGUAGE_MAX_SPEEDY          108
#define LANGUAGE_MAX_SPEEDZ          109
#define LANGUAGE_MAX_SPEEDE          110
// 最大加速度
#define LANGUAGE_MAX_ACCX          112
#define LANGUAGE_MAX_ACCY          113
#define LANGUAGE_MAX_ACCZ          114
#define LANGUAGE_MAX_ACCE          115
// 最大拐角速度
#define LANGUAGE_MAX_CORNERX          117
#define LANGUAGE_MAX_CORNERY          118
#define LANGUAGE_MAX_CORNERZ          119
#define LANGUAGE_MAX_CORNERE          120

#define LANGUAGE_Auto_Set_Bed_PID     116  // 自动设置热床PID
#define LANGUAGE_Auto_Set_Nozzle_PID  121  // 自动设置喷嘴PID

#define LANGUAGE_Step_Per_X       122  // 传动比X
#define LANGUAGE_Step_Per_Y       123  // 传动比Y
#define LANGUAGE_Step_Per_Z       124  // 传动比Z
#define LANGUAGE_Step_Per_E       125  // 传动比E

#define LANGUAGE_Title_Feedback   111 // 意见反馈
#define LANGUAGE_Feedback         126 // 意见反馈
// 图片预览文本-需要添加
#define LANGUAGE_Estimated_Time      127  // 预计时间
#define LANGUAGE_Filament_Used       128  // 用料长度
#define LANGUAGE_Layer_Height        129  // 层高
#define LANGUAGE_Volume              130  // 体积

// PID设置
#define LANGUAGE_Set_PID_Manually   131  // 手动设置PID
#define LANGUAGE_Nozz_P             132  // 喷嘴P值
#define LANGUAGE_Nozz_I             133  // 喷嘴I值
#define LANGUAGE_Nozz_D             134  // 喷嘴D值
#define LANGUAGE_Bed_P              135  // 热床P值
#define LANGUAGE_Bed_I              136  // 热床I值
#define LANGUAGE_Bed_D              137  // 热床D值
#define LANGUAGE_Set_Auto_PID       138  // 自动PID

#define LANGUAGE_Title_Language     106  // 语言标题
//#define LANGUAGE_Fun_LANGUAGE       139  // 语言标题
#define LANGUAGE_Bed_LOW            140  // 热床温度过低
#define LANGUAGE_Bed_HIGH           141  // 热床温度过高

#define LANGUAGE_Auto_PID_ING       142  // 自动PID检测中
#define LANGUAGE_Auto_PID_END       143  // 自动PID检测完成
#define LANGUAGE_Auto_NOZZ_EX       144  // 自动PID检测完成
#define LANGUAGE_Auto_BED_EX        145  // 自动PID检测完成

#define LANGUAGE_Auto_Set_Nozzle_PID_Title  146  // 自动设置喷嘴PID
#define LANGUAGE_Auto_Set_Bed_PID_Title     147  // 自动设置热床PID
#define LANGUAGE_IN_STORK  148  //进料词条
#define LANGUAGE_OUT_STORK 149  //退料词条
#define LANGUAGE_STORKING_TIP1  150 //进料中提示1
#define LANGUAGE_OUT_STORKING_TIP2 151 //退料中提示1
#define LANGUAGE_STORKING_TIP2 152 //进料中提示送料
#define LANGUAGE_OUT_STORKED_TIP2 153 //退料完成提示
#define LANGUAGE_IN_TITLE   154 //进料标题
#define LANGUAGE_OUT_TITLE 155 //退料标题
#define LANGUAGE_Hardware_Version 156 //硬件版本词条
#define LANGUAGE_Level_Calibration 157 //打印校准词条
#define LANGUAGE_HIGH_ERR_CLEAR    158  //对高失败请清洁
#define LANGUAGE_CLEAR_HINT       159 //提示清洁喷嘴和平台
#define LANGUAGE_SCAN_QR          160 //扫描二维码获取解决方案
#define LANGUAGE_BOOT_undone      161 // 开机引导未完成
#define LANGUAGE_CRTouch_error      162 // CRTouch异常，请联系客服

#define LANGUAGE_LEVELING_EDIT     189  //调平数据编辑按钮
#define LANGUAGE_LEVELING_CONFIRM  190  //调平确定按钮
#define LANGUAGE_EDIT_LEVEL_DATA   191 //编辑调平数据
#define LANGUAGE_EDIT_DATA_TITLE   192 //编辑调平数据标题
#define LANGUAGE_LEVEL_FINISH      193 //调平完成词条
#define LANGUAGE_LEVEL_ERROR       194 //调平失败词条
#define LANGUAGE_LEVEL_EDIT_DATA   195 //编辑调平数据标题
#define LANGUAGE_AUTO_HIGHT_TITLE  196 //对高中
#define LANGUAGE_NOZZLE_HOT        197  //喷嘴加热
#define LANGUAGE_NOZZLE_CLRAR  198  //喷嘴清洁
#define LANGUAGE_NOZZLE_HIGHT  199  //喷嘴对高

#define ICON_AdvSet               ICON_Language
#define ICON_HomeOff              ICON_AdvSet
#define ICON_HomeOffX             ICON_StepX
#define ICON_HomeOffY             ICON_StepY
#define ICON_HomeOffZ             ICON_StepZ
#define ICON_ProbeOff             ICON_AdvSet
#define ICON_ProbeOffX            ICON_StepX
#define ICON_ProbeOffY            ICON_StepY
#define ICON_PIDNozzle            ICON_SetEndTemp
#define ICON_PIDbed               ICON_SetBedTemp

/**
 * 3-.0：The font size, 0x00-0x09, corresponds to the font size below:
 * 0x00=6*12   0x01=8*16   0x02=10*20  0x03=12*24  0x04=14*28
 * 0x05=16*32  0x06=20*40  0x07=24*48  0x08=28*56  0x09=32*64
 */
#define font6x12  0x00
#define font8x16  0x01
#define font10x20 0x02
#define font12x24 0x03
#define font14x28 0x04
#define font16x32 0x05
#define font20x40 0x06
#define font24x48 0x07
#define font28x56 0x08
#define font32x64 0x09

// Color  FE29
#define Color_White       0xFFFF
#define Color_Yellow      0xFE29
#define Color_Blue        0x19FF
#define Color_Red         0xF44F
#define Color_Bg_Window   0x31E8  // Popup background color
#define Color_Bg_Blue     0x1145  // Dark blue background color
#define Color_Bg_Black    0x0841  // Black background color
#define Color_Bg_Red      0xF00F  // Red background color
#define Popup_Text_Color  0xD6BA  // Popup font background color
#define Line_Color        0x3A6A  // Split line color
#define Rectangle_Color   0xFE29//0xEE2F  // Blue square cursor color
#define Percent_Color     0xFE29  // Percentage color
#define BarFill_Color     0x10E4  // Fill color of progress bar
#define Select_Color      0x33BB  // Selected color
#define Button_Select_Color 0xFFFF  // Selected color
#define All_Black         0x0000  // Selected color

extern volatile uint8_t checkkey;
extern float zprobe_zoffset;
extern char print_filename[16];
extern int16_t temphot;
extern uint8_t afterprobe_fan0_speed;
extern bool home_flag;
extern bool G29_flag;

extern millis_t dwin_heat_time;
extern float dwin_zoffset;
extern float last_zoffset;

typedef struct 
{
  #if ENABLED(HAS_HOTEND)
    celsius_t E_Temp = 0;
    int16_t Extrusion_Length = 0;
    int16_t LCD_MaxBright  = MAX_SCREEN_BRIGHTNESS;
    int16_t LCD_DimmBright = DIMM_SCREEN_BRIGHTNESS;
    uint8_t Dimm_Time = TURN_OFF_TIME;
  #endif
  #if ENABLED(HAS_HEATED_BED)
    celsius_t Bed_Temp = 0;
  #endif
  #if ENABLED(HAS_FAN)
    int16_t Fan_speed = 0;
  #endif
  int16_t print_speed     = 100;
  float Max_Feedspeed     = 0;
  float Max_Acceleration  = 0;
  float Max_Jerk_scaled   = 0;
  float Max_Step_scaled   = 0;
  float Move_X_scaled     = 0;
  float Move_Y_scaled     = 0;
  float Move_Z_scaled     = 0;
  float InputShaping_scaled = 0;
  uint8_t Curve_index = 0;
  uint16_t Auto_PID_Temp  = 0;
  uint16_t Auto_PID_Value[3] = {0, 100, 260}; // 1:热床温度; 2 喷嘴温度
  float HM_PID_Temp_Value  = 0;
  float Temp_Leveling_Value=0;
  float HM_PID_Value[7]  = {0,DEFAULT_Kp,DEFAULT_Ki,DEFAULT_Kd,DEFAULT_bedKp,DEFAULT_bedKi,DEFAULT_bedKd};
  #if HAS_HOTEND
    float Move_E_scaled   = 0;
  #endif
  float offset_value      = 0;
  int8_t show_mode        = 0; // -1: Temperature control    0: Printing temperature
  float Home_OffX_scaled  = 0;
  float Home_OffY_scaled  = 0;
  float Home_OffZ_scaled  = 0;
  float Probe_OffX_scaled = 0;
  float Probe_OffY_scaled = 0;
} HMI_value_t;

#define DWIN_CHINESE 123
#define DACAI_JAPANESE 124
#define DWIN_ENGLISH 0

typedef struct {
  uint8_t boot_step;  //开机引导步骤
  uint8_t language;   //语言
  uint8_t g_code_pic_sel;
  bool pause_flag:1;
  bool pause_action:1;
  bool print_finish:1;
  bool done_confirm_flag:1;
  bool select_flag:1;
  bool home_flag:1;
  bool heat_flag:1;               // 0: heating done  1: during heating
  bool cutting_line_flag:1;       // 断料检测标志位
  bool filement_resume_flag:1;    // 本地断料状态是否恢复，0：已经恢复，可以接收云指令；1：还在缺料状态，不能接收云控制指令
  bool remove_card_flag:1;        // 拔卡检测标志位
  bool flash_remain_time_flag:1;  // 刷新剩余时间标志位
  bool online_pause_flag:1;       // 联机打印暂停指令标志位，解决双料打印中间暂停，需要人为恢复打印。
  bool disallow_recovery_flag:1;  // 不允许数据恢复标志位
  bool cloud_printing_flag:1;     // 云打印标志
  bool power_back_to_zero_flag:1; // 上电回零标志位
  bool disable_queued_cmd :1;     // 云打印中断料后禁止响应gcode指令
  bool filament_recover_flag :1;  // 断料检测恢复标志位
  bool PID_autotune_start :1;     // 自动PID开始
  bool PID_autotune_end :1;       // 自动PID结束
  bool Refresh_bottom_flag:1;    //刷新底部参数标志位 1 不刷新，0需要刷新
  bool Auto_inout_end_flag:1;  //自动进退料完成标志位
  bool Level_check_flag:1; //调平校准标志位
  bool Level_check_start_flag:1; //调平校准标志位
  bool Need_g29_flag:1; //需要G29标志位
  bool Need_boot_flag:1; //需要对高标志位
  bool abort_end_flag:1; //自定义停止打印标志位
  #if ENABLED(PREVENT_COLD_EXTRUSION)
    bool ETempTooLow_flag:1;
  #endif
  #if HAS_LEVELING
    bool leveling_offset_flag:1;   //调平对高标志位
    bool Pressure_Height_end :1;   //一键对高标志位 1 开始 
    bool G29_finish_flag     :1;
    bool G29_level_not_normal    :1; //此次调平正常
    bool Edit_Only_flag      :1; 
    bool local_leveling_flag :1;   //本地调平标志
    bool leveling_edit_home_flag :1; //调平编辑页面回零是否完成
    bool cr_touch_error_flag :1; //CR_Touch错误标志位
  #endif
  AxisEnum feedspeed_axis, acc_axis, jerk_axis, step_axis;
  uint8_t HM_PID_ROW,Auto_PID_ROW;
  uint8_t PID_ERROR_FLAG;
  uint8_t High_Status;
} HMI_Flag_t;

extern HMI_value_t HMI_ValueStruct;
extern HMI_Flag_t HMI_flag;
extern bool end_flag; //防止反复刷新曲线完成指令
// Show ICO
void ICON_Print(bool show);
void ICON_Prepare(bool show);
void ICON_Control(bool show);
void ICON_Leveling(bool show);
void ICON_StartInfo(bool show);

void ICON_Setting(bool show);
// void ICON_Pause(bool show);
// void ICON_Continue(bool show);
void ICON_Pause();
void ICON_Continue();
void ICON_Stop(bool show);

#if HAS_HOTEND || HAS_HEATED_BED
  // Popup message window
  void DWIN_Popup_Temperature(const bool toohigh);
  void DWIN_Popup_Temperature(const bool toohigh,int8_t Error_id);
#endif

#if HAS_HOTEND
  void Popup_Window_ETempTooLow();
#endif

void Popup_Window_Resume();
void Popup_Window_Home(const bool parking=false);
void Popup_Window_Leveling();

void Goto_PrintProcess();
void Goto_MainMenu();

// Variable control
void HMI_Move_X();
void HMI_Move_Y();
void HMI_Move_Z();
void HMI_Move_E();

void HMI_Zoffset();

#if ENABLED(HAS_HOTEND)
  void HMI_ETemp();
#endif
#if ENABLED(HAS_HEATED_BED)
  void HMI_BedTemp();
#endif
#if ENABLED(HAS_FAN)
  void HMI_FanSpeed();
#endif

void HMI_PrintSpeed();

void HMI_MaxFeedspeedXYZE();
void HMI_MaxAccelerationXYZE();
void HMI_MaxJerkXYZE();
void HMI_StepXYZE();


void DWIN_Draw_Signed_Float(uint8_t size, uint16_t bColor, uint8_t iNum, uint8_t fNum, uint16_t x, uint16_t y, long value);

// SD Card
void HMI_SDCardInit();
void HMI_SDCardUpdate();

// Main Process
void Icon_print(bool value);
void Icon_control(bool value);
void Icon_temperature(bool value);
void Icon_leveling(bool value);

// Other
void Draw_Status_Area(const bool with_update); // Status Area
void HMI_StartFrame(const bool with_update);   // Prepare the menu view
void HMI_MainMenu();    // Main process screen
void HMI_SelectFile();  // File page
void HMI_Printing();    // Print page
void HMI_Prepare();     // Prepare page
void HMI_Control();     // Control page
void HMI_Leveling();    // Level the page
void HMI_AxisMove();    // Axis movement menu
void HMI_Temperature(); // Temperature menu
void HMI_Motion();      // Sports menu
void HMI_Info();        // Information menu
void HMI_Tune();        // Adjust the menu
void HMI_Confirm();  //需要点击确定的界面
#if HAS_PREHEAT
  void HMI_PLAPreheatSetting(); // PLA warm-up setting
  void HMI_TPUPreheatSetting(); // TPU warm-up setting
  void HMI_PETGPreheatSetting(); // PETG warm-up setting
  void HMI_ABSPreheatSetting(); // ABS warm-up setting
#endif

void HMI_MaxSpeed();        // Maximum speed submenu
void HMI_MaxAcceleration(); // Maximum acceleration submenu
void HMI_MaxJerk();         // Maximum jerk speed submenu
void HMI_Step();            // Transmission ratio
void HMI_Boot_Set(); //开机引导设置

void HMI_Init();
void DWIN_Update();
void EachMomentUpdate();
void Check_Filament_Update(void);
void DWIN_HandleScreen();
// void DWIN_StatusChanged(const char *text);
void DWIN_Draw_Checkbox(uint16_t color, uint16_t bcolor, uint16_t x, uint16_t y, bool mode /* = false*/);

inline void DWIN_StartHoming() { HMI_flag.home_flag = true; }

void DWIN_Show_M117(char* str);

void DWIN_CompletedHoming();
void DWIN_CompletedHeight();
void DWIN_CompletedLeveling();
void HMI_SetLanguage();

void Popup_window_Filament(void);
void Popup_window_Remove_card(void);
void Remove_card_window_check(void);
void ICON_Continue();
void Save_Boot_Step_Value();//保存开机引导步骤
// 新增PID相关的接口
 void HMI_Hm_Set_PID(void);       // 手动设置PID
 void HMI_Auto_set_PID(void);     // 自动设置PID
 void HMI_Auto_Nozzle_PID(void);  // 自动设置喷嘴PID
 void HMI_Auto_Bed_PID(void);     // 自动设置热床PID
void make_name_without_ext(char *dst, char *src, size_t maxlen);
// 云打印宏
#define TEXTBYTELEN     20   // 文件名长度
#define MaxFileNumber   20   // 最大文件数

#define AUTO_BED_LEVEL_PREHEAT  120

#define FileNum             MaxFileNumber
#define FileNameLen         TEXTBYTELEN
#define RTS_UPDATE_INTERVAL 2000
#define RTS_UPDATE_VALUE    RTS_UPDATE_INTERVAL

#define SizeofDatabuf       26

typedef struct CardRecord
{
  int recordcount;
  int Filesum;
  unsigned long addr[FileNum];
  char Cardshowfilename[FileNum][FileNameLen];
  char Cardfilename[FileNum][FileNameLen];
}CRec;

extern bool DWIN_lcd_sd_status;
extern bool pause_action_flag ;
extern CRec CardRecbuf;

extern uint8_t Cloud_Progress_Bar; // 云打印传输的进度条数据

extern void Draw_Print_ProgressRemain();
void Draw_Select_language();
void Draw_Poweron_Select_language();
void HMI_Poweron_Select_language();
void Draw_Print_File_Menu();
void Clear_Title_Bar();
void Draw_HM_PID_Set();
void Draw_Auto_PID_Set();
void Draw_auto_nozzle_PID();
void Draw_auto_bed_PID();
void HMI_HM_PID_Value_Set();   // 手动设置PID值
void HMI_AUTO_PID_Value_Set(); // 自动设置PID值
void HMI_PID_Temp_Error();     // PID温度错误界面
void Draw_PID_Error();         // 画PID异常界面
void Popup_Window_Feedstork_Finish(bool parking/*=false*/);
void Popup_Window_Feedstork_Tip(bool parking/*=false*/);
void HMI_Plan_Move(const feedRate_t fr_mm_s);
void HMI_Auto_In_Feedstock();
void HMI_Auto_IN_Out_Feedstock();
void Draw_More_Icon(uint8_t line);
void Draw_Menu_Icon(uint8_t line,  uint8_t icon);
void Draw_Leveling_Highlight(const bool sel);
void Popup_Window_Height(uint8_t state);
void Leveling_Error();  //调平失败弹窗
void Draw_Status_Area(bool with_update);
void Draw_Mid_Status_Area(bool with_update);
void update_variable();
void update_middle_variable();
duration_t estimate_remaining_time(const duration_t elapsed);
void Draw_laguage_Cursor(uint8_t line);
void In_out_feedtock(uint16_t _distance,uint16_t _feedRate,bool dir);
void In_out_feedtock_level(uint16_t _distance,uint16_t _feedRate,bool dir);
void Popup_window_boot(uint8_t type_popup); //弹窗类型接口
void CR_Touch_error_func();//CR_TOUCH错误
// void Draw_Dots_On_Screen(xy_int8_t *mesh_Count,uint8_t Set_En,uint16_t Set_BG_Color);
#if ENABLED(SHOW_GRID_VALUES)  //如果显示调平值
  void Draw_Dots_On_Screen(xy_int8_t *mesh_Count,uint8_t Set_En,uint16_t Set_BG_Color);
  void DWIN_Draw_Z_Offset_Float(uint8_t size, uint16_t color,uint16_t bcolor, uint8_t iNum, uint8_t fNum, uint16_t x, uint16_t y, long value);
  void Refresh_Leveling_Value();//刷新调平值
#endif
#if ENABLED(USER_LEVEL_CHECK)  //使用调平校准功能
    #define CHECK_POINT_NUM        4
    #define CHECK_ERROR_NUM        2
    #define CHECK_ERROR_MIN_VALUE  0.05
    #define CHECK_ERROR_MAX_VALUE  0.1
#endif
extern uint8_t G29_level_num;//记录G29调平了多少个点，用来判断g29是否正常调平
///////////////////////////
