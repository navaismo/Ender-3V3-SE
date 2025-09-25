#include "../../inc/MarlinConfig.h"
#include "../../MarlinCore.h"
#include "../gcode.h"
#include "../parser.h"
#include "../../lcd/dwin/e3v2/dwin.h"
#include "../../lcd/marlinui.h"

/**
 * O9001: Octoprint Plugin to Update LCD Prnt Job Details
 *
 * Parameters in one line:
 *   - T<int> : remaining seconds (ETA)
 *   - C<int> : current layer
 *   - P<int> : progress (0..100)
 **/
 
static uint32_t o9_remain_s   = 0;  // T: remaining seconds (ETA)
static int      o9_curr_ly    = 0;  // C: current layer
static int      o9_progress   = 0;  // P: 0..100

// ---- Small string buffers 
static char o9_eta_hms[10]    = {0};  // "HH:MM:SS"
static char o9_prog_str[10]   = {0};  // "62"
static char o9_currly_str[10] = {0};  // "      7" with padding

// Format seconds into "HH:MM:SS"
static inline void fmt_hms(uint32_t s, char *out, size_t n) {
  if (!out || n < 9) return;
  const uint32_t h = s / 3600U;
  const uint32_t m = (s % 3600U) / 60U;
  const uint32_t sec = s % 60U;
  // Clamp hours to 99 to keep width; adjust if your LCD can show 3+ digits hours
  const uint32_t hh = h > 99U ? 99U : h;
  snprintf(out, n, "%02lu:%02lu:%02lu", (unsigned long)hh, (unsigned long)m, (unsigned long)sec);
}

// Left-pad an integer to a minimum width (spaces)
static inline void fmt_int_lpad(int v, char *out, size_t n, int width) {
  if (!out || n == 0) return;
  char tmp[16];
  snprintf(tmp, sizeof(tmp), "%d", v);
  const int len = (int)strlen(tmp);
  int pad = width - len;
  if (pad < 0) pad = 0;
  int i = 0;
  // write padding
  for (; i < pad && (size_t)i < n - 1; ++i) out[i] = ' ';
  // write number
  for (int j = 0; tmp[j] && (size_t)(i + j) < n - 1; ++j) out[i + j] = tmp[j];
  out[(size_t)i + strlen(tmp) < n ? (i + (int)strlen(tmp)) : (n - 1)] = '\0';
}

// Update the LCD with cached values
static inline void o9_refresh_lcd() {
  // Format strings the LCD routine expects
  fmt_hms(o9_remain_s, o9_eta_hms, sizeof(o9_eta_hms));   // ETA / remaining
  snprintf(o9_prog_str, sizeof(o9_prog_str), "%d", o9_progress);
  // If you want padding for current layer similar to your legacy right-just(7):
  fmt_int_lpad(o9_curr_ly, o9_currly_str, sizeof(o9_currly_str), 7);

  // Minimal UI update (replace with your exact routine)
  // Old code used: DWIN_SetPrintingDetails(eta_str, progress_str, current_layer_str);
  DWIN_SetPrintingDetails(o9_eta_hms, o9_prog_str, o9_currly_str);
}

void GcodeSuite::O9001() {
  // Cache numeric fields (all optional)
  if (parser.seen('T'))  o9_remain_s  = (uint32_t)parser.value_ulong();
  if (parser.seen('C'))  o9_curr_ly   = parser.value_int();
  if (parser.seen('P')) {
    const int p = parser.value_int();
    o9_progress = constrain(p, 0, 100);
  }

 o9_refresh_lcd();

}
  

  
