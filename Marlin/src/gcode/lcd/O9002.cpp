#include "../../inc/MarlinConfig.h"
#include "../../MarlinCore.h"
#include "../gcode.h"
#include "../parser.h"
#include "../../lcd/dwin/e3v2/dwin.h"
#include "../../lcd/marlinui.h"
#include "../../lcd/dwin/e3v2/lcd_rts.h"

/*void sendImageMap() {
    SERIAL_ECHOLN("Sending ImageMap array:");
    for (int i = 0; i < OctoIMAGE_MAP_SIZE; i++) {
        SERIAL_ECHOLNPAIR("Pixel ", i);
        SERIAL_ECHOLNPAIR(": ", OctoImageMap[i]);
    }
}*/

/**
 * O9002: Fill Marlin Variable ImageMap with incoming data
 *
 * Params:
 *   O9002|data1,data2,data3...data20
 *   O9002|END
 */

static int mypicBufIndex = 0; 
void GcodeSuite::O9002() {
    // Ensure parser.string_arg is not null.
    if (parser.string_arg && parser.string_arg[0] != '\0') {
        // Handle START command to initialize/reset.
        if (strncmp(parser.string_arg, "START", 5) == 0) {
            // (Optional) Parse dimensions if needed.
            memset(mypicBuf, 0, sizeof(mypicBuf));
            mypicBufIndex = 0;
            SERIAL_ECHOLN("O9002 START: Image map cleared.");
            return;
        }

        // Handle END command to finalize.
        if (strcmp(parser.string_arg, "END") == 0) {
            SERIAL_ECHOLN("O9002 END: Image map complete.");
            //sendImageMap();          // For example, echo the image data.
            DWIN_RenderOctoImageMap();// Trigger display update.
            return;
        }

        // Handle CHUNK command.
        if (strncmp(parser.string_arg, "CHUNK", 5) == 0) {
            // Skip the "CHUNK" part.
            char *arg = parser.string_arg + 5;
            while (*arg && isspace(*arg)) arg++;
            // Tokenize using '|' to separate index and pixel data.
            char *token = strtok(arg, "|");
            if (token != nullptr) {
                int chunk_start = atoi(token);
                token = strtok(nullptr, "|");
                if (token != nullptr) {
                    // Tokenize the pixel data by commas.
                    char *pixel_token = strtok(token, ",");
                    int local_index = chunk_start;
                    while (pixel_token != nullptr && local_index < 2096) {
                        uint8_t pixel_value = static_cast<uint8_t>(atoi(pixel_token));
                        SERIAL_ECHOLNPAIR("RecVal: ", pixel_value);
                        mypicBuf[local_index] = pixel_value;
                        SERIAL_ECHOLNPAIR("IMGVal: ", mypicBuf[local_index]);
                        local_index++;
                        pixel_token = strtok(nullptr, ",");
                    }
                }
            }
            return;
        }
    }
}