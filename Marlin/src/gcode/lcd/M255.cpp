// /**
//  * Marlin 3D Printer Firmware
//  * Copyright (c) 2022 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
//  *
//  * Based on Sprinter and grbl.
//  * Copyright (c) 2011 Camiel Gubbels / Erik van der Zalm
//  *
//  * This program is free software: you can redistribute it and/or modify
//  * it under the terms of the GNU General Public License as published by
//  * the Free Software Foundation, either version 3 of the License, or
//  * (at your option) any later version.
//  *
//  * This program is distributed in the hope that it will be useful,
//  * but WITHOUT ANY WARRANTY; without even the implied warranty of
//  * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  * GNU General Public License for more details.
//  *
//  * You should have received a copy of the GNU General Public License
//  * along with this program.  If not, see <https://www.gnu.org/licenses/>.
//  *
//  */
// #include "../../inc/MarlinConfig.h"

// #if ENABLED(DWIN_CREALITY_LCD)
//   #include "../../lcd/dwin/e3v2/dwin.h"

// #include "../gcode.h"
// #include "../../lcd/marlinui.h"



// void GcodeSuite::M255_report() {
//     SERIAL_ECHOLNPAIR("Display Sleep timeout in minutes: ", TURN_OFF_TIME);
//   }

// /**
//  * M255: Set the LCD sleep timeout (in minutes)
//  *  S<minutes> - Period of inactivity required for display / backlight sleep
//  */
// void GcodeSuite::M255() {
//   if (parser.seenval('S')) {
//     const int m = parser.value_int();
//     if(m > 0 && m <= 60){
//         TURN_OFF_TIME = constrain(m, 1, 60);
//         settings.save();
//     }
//     else{
//         SERIAL_ECHOLNPGM("Invalid value for M255 S<minutes>! (1-60)");    
//     }    
//   }
//   else
//     M255_report();
// }


// #endif // EDITABLE_DISPLAY_TIMEOUT