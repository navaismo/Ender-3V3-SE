#include "../../inc/MarlinConfig.h"
#include "../../MarlinCore.h"
#include "../gcode.h"
#include "../parser.h"
#include "../../lcd/dwin/e3v2/dwin.h"
#include "../../lcd/marlinui.h"

#include <stdio.h>
#include <string.h>

/**
 *O9000 /O9001: Unified Control for Printing Details in LCD
 *
 *O9000 (Setup/General):
 *-A {0 | 1}: Active/inactive serial connection (returns to the menu)
 *-R1: immediate rendering of the work card
 *-F1: Mark work finished
 *-U <at>: Elapset Seconds (elapsed time)
 *-T <nt>: Remaining Seconds (ETA)
 *-L <nt>: Total Layers
 *-C <at>: Current Layer
 *-P <at>: progress 0..100
 *-K1 S "...": (only String allowed) Fillename to show
 *
 *O9001 (fast /live update):
 *-T <nt>: Remaining Seconds (ETA)
 *-C <at>: Current Layer
 *-P <at>: progress 0..100
 */

static uint32_t o9_elapsed_s = 0;  // U: elapsed
static uint32_t o9_remain_s  = 0;  // T: remaining (ETA)
static int      o9_total_ly  = 0;  // L: total of layers
static int      o9_curr_ly   = 0;  // C: capa actual
static int      o9_progress  = 0;  // P: 0..100

static char o9_filename[35] = {0};  // If it is not sent, it is empty

static char o9_buf_eta[10]     = {0};  // "hh:mm:ss"
static char o9_buf_elapsed[10] = {0};  // "hh:mm:ss"
static char o9_buf_prog[10]    = {0};  // "0..100"
static char o9_buf_currly[10]  = {0};  // padded "      7"
static char o9_buf_totly[10]   = {0};  // "123"

// Format seconds into "HH:MM:SS"
static inline void fmt_hms(uint32_t s, char *out, size_t n) {
  if (!out || n < 9) return;
  const uint32_t h = s / 3600U;
  const uint32_t m = (s % 3600U) / 60U;
  const uint32_t sec = s % 60U;
  const uint32_t hh = h > 99U ? 99U : h; // 2 digit clamp
  snprintf(out, n, "%02lu:%02lu:%02lu",
           (unsigned long)hh, (unsigned long)m, (unsigned long)sec);
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
  for (; i < pad && (size_t)i < n - 1; ++i) out[i] = ' ';
  for (int j = 0; tmp[j] && (size_t)(i + j) < n - 1; ++j) out[i + j] = tmp[j];
  const size_t end = (size_t)i + strlen(tmp);
  out[end < n ? end : (n - 1)] = '\0';
}

// Safely copy a filename from a G-code string argument, handling quotes
static inline void safe_copy_filename(char *dst, size_t dstlen, const char *src) {
  if (!dst || !dstlen) return;
  dst[0] = '\0';
  if (!src) return;

  const char *p = src;
  if (p[0] == 'S' && p[1] == '"') p += 2;
  else if (p[0] == '"')           p += 1;

  size_t i = 0;
  for (; *p && *p != '"' && i < dstlen - 1; ++p) {
    if (*p == '\\' && p[1] == '"') {
      dst[i++] = '"';
      ++p;
    } else {
      dst[i++] = *p;
    }
  }
  dst[i] = '\0';
}

// Update only the details on the LCD (ETA, progress, current layer)
static inline void o9_refresh_details() {
  // DWIN_SetPrintingDetails(ETA, progress, curr_layer)
  fmt_hms(o9_remain_s, o9_buf_eta, sizeof(o9_buf_eta));
  snprintf(o9_buf_prog, sizeof(o9_buf_prog), "%d", constrain(o9_progress, 0, 100));
  fmt_int_lpad(o9_curr_ly, o9_buf_currly, sizeof(o9_buf_currly), 7);

  DWIN_SetPrintingDetails(o9_buf_eta, o9_buf_prog, o9_buf_currly);
}

// Render the full job card on the LCD with all details
static inline void o9_render_job_card() {
  // DWIN_OctoPrintJob(filename, elapsed_str, remain_str, total_layers_str, curr_layer_str, curr_layer_dup, progress_str)
  fmt_hms(o9_elapsed_s, o9_buf_elapsed, sizeof(o9_buf_elapsed));
  fmt_hms(o9_remain_s,  o9_buf_eta,     sizeof(o9_buf_eta));
  snprintf(o9_buf_totly, sizeof(o9_buf_totly), "%d", o9_total_ly);
  fmt_int_lpad(o9_curr_ly, o9_buf_currly, sizeof(o9_buf_currly), 7);
  snprintf(o9_buf_prog, sizeof(o9_buf_prog), "%d", constrain(o9_progress, 0, 100));

  // UPDATE LABEL TIME CLOSE
  DWIN_OctoSetPrintTime(o9_buf_elapsed);

  // Render card
  DWIN_OctoPrintJob(o9_filename, o9_buf_elapsed, o9_buf_eta, o9_buf_totly, o9_buf_currly, o9_buf_prog);
}

// ---------------------------------------------------------------------------
// G-Code O9000: Setup /Control General (numerical + filename by k1 s "...")
// ---------------------------------------------------------------------------
void GcodeSuite::O9000() {
  // A {0 | 1}: Active/inactive connection -> Main Menu
  if (parser.seen('A')) {
    serial_connection_active = parser.value_bool();
    Goto_MainMenu();
  }

  // F1: Finish work
  if (parser.boolval('F')) {
    DWIN_OctoJobFinish();
  }

  // Preferred numerical parameters
  if (parser.seen('U')) o9_elapsed_s = (uint32_t)parser.value_ulong();  // Elapsed
  if (parser.seen('T')) o9_remain_s  = (uint32_t)parser.value_ulong();  // Remaining (Ita)
  if (parser.seen('L')) o9_total_ly  = parser.value_int();
  if (parser.seen('C')) o9_curr_ly   = parser.value_int();
  if (parser.seen('P')) {
    const int p = parser.value_int();
    o9_progress = constrain(p, 0, 100);
  }

  // compat string: K1 S"..." (filename)
  if (parser.seen('K')) {
    const int key = parser.value_int();
    if (key == 1 && parser.seen('S')) {
      const char* s = parser.string_arg;
      safe_copy_filename(o9_filename, sizeof(o9_filename), s);
    }
  }

  // R1: immediate render on the card
  if (parser.boolval('R')) {
    o9_render_job_card();
    SERIAL_ECHOLNPGM("O9000 sc-rendered");
  }
}

// ---------------------------------------------------------------------------
// G-Code O9001: Rapid Update
// ---------------------------------------------------------------------------
void GcodeSuite::O9001() {
  if (parser.seen('T')) o9_remain_s = (uint32_t)parser.value_ulong();
  if (parser.seen('C')) o9_curr_ly  = parser.value_int();
  if (parser.seen('P')) {
    const int p = parser.value_int();
    o9_progress = constrain(p, 0, 100);
  }
  o9_refresh_details();
}
