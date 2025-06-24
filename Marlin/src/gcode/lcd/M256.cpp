// // /**
// //  * Marlin 3D Printer Firmware
// //  * Copyright (c) 2021 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
// //  *
// //  * Based on Sprinter and grbl.
// //  * Copyright (c) 2011 Camiel Gubbels / Erik van der Zalm
// //  *
// //  * This program is free software: you can redistribute it and/or modify
// //  * it under the terms of the GNU General Public License as published by
// //  * the Free Software Foundation, either version 3 of the License, or
// //  * (at your option) any later version.
// //  *
// //  * This program is distributed in the hope that it will be useful,
// //  * but WITHOUT ANY WARRANTY; without even the implied warranty of
// //  * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// //  * GNU General Public License for more details.
// //  *
// //  * You should have received a copy of the GNU General Public License
// //  * along with this program.  If not, see <https://www.gnu.org/licenses/>.
// //  *
// //  */
// #include "../../inc/MarlinConfig.h"

// #if ENABLED(DWIN_CREALITY_LCD)

// #include "../gcode.h"
// #include "../../lcd/marlinui.h"
// #include "../../lcd/dwin/e3v2/dwin.h"

// void GcodeSuite::M256_report() {
//     SERIAL_ECHOLNPAIR("Display Max Brightness: ", (MAX_SCREEN_BRIGHTNESS*100)/64);
//     SERIAL_ECHOLNPAIR("Display Dimm Brightness: ", (DIMM_SCREEN_BRIGHTNESS*100)/64);
//   }

// /**
//  * M256: Set the LCD brightness
//  */
// void GcodeSuite::M256() {
//   if (parser.seenval('B')){
//     const int b = parser.value_int();
//     if(b < 0 || b > 100){
//       SERIAL_ECHOLNPGM("Invalid value for M256 B<percentage>! (0-100)");    
//       return;
//     }
//     MAX_SCREEN_BRIGHTNESS = (uint8_t)(((uint16_t)b * 64) / 100);
//     DWIN_Backlight_SetLuminance(MAX_SCREEN_BRIGHTNESS);
//     settings.save();
    
//   } else if (parser.seenval('D')) {
//     const int d = parser.value_int();
//     if(d < 0 || d > 100){
//       SERIAL_ECHOLNPGM("Invalid value for M256 D<percentage>! (0-100)");    
//       return;
//     }
//     DIMM_SCREEN_BRIGHTNESS = (uint8_t)(((uint16_t)d * 64) / 100);
//     settings.save();
    
//   } 
  
//   else{
//     M256_report();
//   }
// }


// #endif // HAS_LCD_BRIGHTNESS