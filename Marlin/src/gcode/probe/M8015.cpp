/**
 * @file M8015.cpp
 * @author creality
 * 
 * @version 0.1
 * @date 2021-12-15
 * 
 * @copyright Copyright (c) 2021
 * 
 */
#include "../gcode.h"
#include "../../module/planner.h"
#include "../../module/stepper.h"
#include "../../module/probe.h"
#include "../../../Configuration_adv.h"
#include "../../lcd/dwin/e3v2/dwin.h"

#if ANY(USE_AUTOZ_TOOL,USE_AUTOZ_TOOL_2)

/**
 * One key to obtain z-axis offset value
 */
void GcodeSuite::M8015(void)
{
  const bool withLevel = parser.seen('S');
  float zOffset = 0;
   for(int x = 0; x < GRID_MAX_POINTS_X; x ++)
  {
    for(int y = 0; y < GRID_MAX_POINTS_Y; y ++)
    {
      z_values[x][y] = 0;
    }
  }
  refresh_bed_level();//刷新逻辑分区值
  
  if(axis_is_trusted(Z_AXIS))
  {
    DISABLE_AXIS_Z();  //释放Z轴电机,如果不释放，第二次调整数据异常，无法清除
    probe.offset.z = 0;//清除数据，防止数据二次叠加
  }
  // gcode.process_subcommands_now_P(PSTR("G28"));
  // probe.auto_get_offset(); //一键对高逻辑
  HMI_flag.leveling_offset_flag=true;
  checkkey = ONE_HIGH;
  if(getZOffset(1, 1, 1,&zOffset))
  {
    if(HMI_flag.Need_boot_flag)//开机引导中
    {
      HMI_flag.boot_step=Set_levelling; //当前步骤设置到开机引导完成标志位并保存
      //Save_Boot_Step_Value();//保存开机引导步骤
    }
    // else
    // {
      probe.offset.z=zOffset;
      probe.offset.z -= NOZ_AUTO_OFT_MM; //将最后的Z轴补偿值再往下压0.05mm。Rock_20230516
      // TERN_(EEPROM_SETTINGS, settings.save());
      // TERN_(USE_AUTOZ_TOOL_2, DWIN_CompletedHeight());
      RUN_AND_WAIT_GCODE_CMD("G28", true);                   //测量前先获取一次HOME点 
      
      if (withLevel){

      HMI_flag.leveling_offset_flag=false;
      HMI_flag.Pressure_Height_end=true;
      
      }else{
        settings.save(); 
        Goto_MainMenu();
      }
    // }
  }
  else //对高失败
  {
    HMI_flag.leveling_offset_flag=false; //测试失败
    checkkey=POPUP_CONFIRM; 
    Popup_window_boot(High_faild_clear);//弹出对高失败的界面
  }
     
}
#endif  // USE_AUTOZ_TOOL
