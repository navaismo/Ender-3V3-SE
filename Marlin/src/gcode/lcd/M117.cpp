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

#if HAS_STATUS_MESSAGE

#include "../gcode.h"
#include "../../lcd/marlinui.h"
#include "../parser.h"

#if ENABLED(DWIN_CREALITY_LCD)
#include "../../lcd/dwin/e3v2/dwin.h"
#endif

/**
 * M117: Set LCD Status Message
 */
void GcodeSuite::M117()
{

#if ENABLED(HOST2_ACTION_COMMANDS)

  if (parser.string_arg && parser.string_arg[0] != '\0')
  {
    char *my_string = parser.string_arg;

    //SERIAL_ECHOLN("Got M117 Command from Serial Terminal");
    //SERIAL_ECHOLNPAIR("String received: ", my_string);
    //delay(200);

    if (strlen(my_string) > 30)
    {
      SERIAL_ECHOLN("Warning: String too long");
      SERIAL_ECHOLN("Not sending to LCD");
    }
    else
    {
      // Supress INDICATOR messages from Octoprint
      if (strstr(parser.string_arg, "INDICATOR-") != NULL)
      {
        SERIAL_ECHOLN("Indicator Layer, not showing in LCD");
      }
      else
      {
        TERN_(DWIN_CREALITY_LCD, DWIN_Show_M117(my_string));
      }
    }
  }
  else
  {
    SERIAL_ECHOLN("Warning: No string provided with M117");
  }
#endif

  if (parser.string_arg && parser.string_arg[0])
  {
    ui.set_status(parser.string_arg);
  }
  else
  {
    ui.reset_status();
  }
}

#endif // HAS_STATUS_MESSAGE
