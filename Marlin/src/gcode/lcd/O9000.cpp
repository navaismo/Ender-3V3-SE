#include "../../inc/MarlinConfig.h"
#include "../gcode.h"
#include "../parser.h"
#include "../../lcd/dwin/e3v2/dwin.h"
#include "../../lcd/marlinui.h"

/**
 * O9000: Octoprint Plugin to Set the LCD Print Job Details
 *
 * Parameters:
 *   - A{0|1}          : mark serial connection active/inactive (toggles main menu)
 *   - R1              : render the job card immediately (was "SC|")
 *   - F1              : mark job as finished (was "PF|")
 *   - K1 S"..."       : set string #1 (filename)
 *   - K2 S"..."       : set string #2 (print_time) and update UI time label
 *   - K3 S"..."       : set string #3 (ptime_left)
 *   - K4 S"..."       : set string #4 (total_layers)
 *   - K5 S"..."       : set string #5 (curr_layer)
 *   - K6 S"..."       : set string #6 (progress)
 *
**/

char filename[35]     = {0};
char print_time[10]   = {0};   
char ptime_left[10]   = {0};   
char total_layers[10] = {0};
char curr_layer[10]   = {0};
char progress[10]     = {0};


// Safe copy from parser string (S"...") to fixed-size buffer, handling escapes
static inline void safe_copy(char *dst, size_t dstlen, const char *src) {
  if (!dst || !dstlen) return;
  dst[0] = '\0';
  if (!src) return;

  const char *p = src;

  // Some Marlin builds return S"...", others return "...", others the bare content.
  if (p[0] == 'S' && p[1] == '"') p += 2;     // skip S"
  else if (p[0] == '"')            p += 1;     // skip leading "

  size_t i = 0;
  for (; *p && *p != '"' && i < dstlen - 1; ++p) {
    if (*p == '\\' && p[1] == '"') {           // unescape \" -> "
      dst[i++] = '"';
      ++p;                                     // skip the escaped quote
    } else {
      dst[i++] = *p;
    }
  }
  dst[i] = '\0';
}

void GcodeSuite::O9000() {
  // A{0|1}: mark connection active/inactive and go to main menu
  if (parser.seen('A')) {
    serial_connection_active = parser.value_bool();
    Goto_MainMenu();
  }

   // G{0|1}: Enable/Disable the Receiving Thumbnail
  if (parser.seen('G')) {
    Show_Default_IMG = parser.value_bool();
  }

  // R1: render job card immediately ("SC|")
  if (parser.boolval('R')) {
    DWIN_OctoPrintJob(filename, print_time, ptime_left, total_layers, curr_layer, progress);
    SERIAL_ECHOLNPGM("O9000 sc-rendered");
  }

  // F1: mark job finished ("PF|")
  if (parser.boolval('F')) {
    DWIN_OctoJobFinish();
  }

  // K{n} S"...": set string values (K1..K6 mapped to your legacy fields)
  if (parser.seen('K')) {
    const int key = parser.value_int();
    if (parser.seen('S')) {
      const char* s = parser.string_arg;  // S"...", preserved by Marlin parser
      switch (key) {
        case 1: // filename
          safe_copy(filename, sizeof(filename), s);
          break;
        case 2: // print_time ("UPT|")
          safe_copy(print_time, sizeof(print_time), s);
          DWIN_OctoSetPrintTime(print_time);
          break;
        case 3: // ptime_left
          safe_copy(ptime_left, sizeof(ptime_left), s);
          break;
        case 4: // total_layers
          safe_copy(total_layers, sizeof(total_layers), s);
          break;
        case 5: // curr_layer
          safe_copy(curr_layer, sizeof(curr_layer), s);
          break;
        case 6: // progress
          safe_copy(progress, sizeof(progress), s);
          break;
        default:
          break;
      }
    }
  }

}