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

 #include "../../inc/MarlinConfig.h"
 #include "../../lcd/dwin/e3v2/dwin.h"
 #if ENABLED(SDSUPPORT)
 
 #include "../gcode.h"
 #include "../../sd/cardreader.h"
 #include "../../module/printcounter.h"
 #include "../../lcd/marlinui.h"
 
 #if ENABLED(PARK_HEAD_ON_PAUSE)
 #include "../../feature/pause.h"
 #endif
 
 #if ENABLED(HOST_ACTION_COMMANDS)
 #include "../../feature/host_actions.h"
 #endif
 
 #if ENABLED(POWER_LOSS_RECOVERY)
 #include "../../feature/powerloss.h"
 #endif
 
 #if ENABLED(DGUS_LCD_UI_MKS)
 #include "../../lcd/extui/dgus/DGUSDisplayDef.h"
 #endif
 
 #include "../../MarlinCore.h" // for startOrResumeJob
 
 /**
  * M24: Start or Resume SD Print
  */
 void GcodeSuite::M24()
 {
 
  //  SERIAL_ECHOLNPAIR("=====++++>> M24 Resume & Flag = ", serial_connection_active);
   if (serial_connection_active)
   {
    //  SERIAL_ECHOLN("=====++++>> M24 with Octo, Sending Resume Command");
 #if ENABLED(HOST_ACTION_COMMANDS)
 #ifdef ACTION_ON_RESUME
     host_action_resume();
 #endif
     TERN_(HOST_PROMPT_SUPPORT, host_prompt_open(PROMPT_INFO, PSTR("Resuming"), DISMISS_STR));
 #endif
   }
   else
   {
    //  SERIAL_ECHOLN("=====++++>> M24 with SD, Sending Resume Command");
 #if ENABLED(DGUS_LCD_UI_MKS)
     if ((print_job_timer.isPaused() || print_job_timer.isRunning()) && !parser.seen("ST"))
       MKS_resume_print_move();
 #endif
 
 #if ENABLED(POWER_LOSS_RECOVERY)
     if (parser.seenval('S'))
       card.setIndex(parser.value_long());
     if (parser.seenval('T'))
       print_job_timer.resume(parser.value_long());
 #endif
 
 #if ENABLED(PARK_HEAD_ON_PAUSE)
     if (did_pause_print)
     {
       resume_print(); // will call print_job_timer.start()
       return;
     }
 #endif
 
     if (card.isFileOpen())
     {
       card.startOrResumeFilePrinting(); // SD card will now be read for commands
       startOrResumeJob();               // Start (or resume) the print job timer
       TERN_(POWER_LOSS_RECOVERY, recovery.prepare());
     }
 
     ui.reset_status();
 // rock_20211021 In the case of non-SD card printing, enter the M24 and enter the printing page
 #if ENABLED(DWIN_CREALITY_LCD)
     if (recovery.info.sd_printing_flag == false)
     {
       // Update_Time_Value = 0;
       print_job_timer.start();
       // The print page is displayed.
       Goto_PrintProcess();
     }
 #endif
   }
 }
 
 /**
  * M25: Pause SD Print
  *
  * With PARK_HEAD_ON_PAUSE:
  *   Invoke M125 to store the current position and move to the park
  *   position. M24 will move the head back before resuming the print.
  */
 void GcodeSuite::M25()
 {
   // Determine whether to print online
   if (!HMI_flag.remove_card_flag && !HMI_flag.pause_action && !HMI_flag.cutting_line_flag)
   {
     if (!HMI_flag.filement_resume_flag)
     {
       HMI_flag.online_pause_flag = true;
       ICON_Continue();
     }
   }
   // #if ENABLED(PARK_HEAD_ON_PAUSE)
 
   //   M125();
 
   // #else
 
   // Set initial pause flag to prevent more commands from landing in the queue while we try to pause
  //  SERIAL_ECHOLNPAIR("=====++++>> M25 PAUSE & Flag = ", serial_connection_active);
   if (serial_connection_active)
   {
    //  SERIAL_ECHOLN("=====++++>> M25 with Octo, Sending Pause Command");
 #if ENABLED(HOST_ACTION_COMMANDS)
     TERN_(HOST_PROMPT_SUPPORT, host_prompt_open(PROMPT_PAUSE_RESUME, PSTR("Pause"), PSTR("Resume")));
 #ifdef ACTION_ON_PAUSE
     host_action_pause();
 #endif
 #endif
   }
   else
   {
    //  SERIAL_ECHOLN("=====++++>> M25 with SD, Sending Pause Command");
 #if ENABLED(SDSUPPORT)
     if (IS_SD_PRINTING())
       card.pauseSDPrint();
 #endif
 
 #if ENABLED(POWER_LOSS_RECOVERY) && DISABLED(DGUS_LCD_UI_MKS)
     if (recovery.enabled)
       recovery.save(true);
 #endif
 
     print_job_timer.pause();
 
     TERN_(DGUS_LCD_UI_MKS, MKS_pause_print_move());
 
     // IF_DISABLED(DWIN_CREALITY_LCD, ui.reset_status());
     TERN_(DWIN_CREALITY_LCD, ui.reset_status());
   }
 
   // #endif
 }
 
 #endif // SDSUPPORT
 