#include "../../../inc/MarlinConfigPre.h"
#include "dwin.h"
#include "ui_position.h"
#if ANY(AUTO_BED_LEVELING_BILINEAR, AUTO_BED_LEVELING_LINEAR, AUTO_BED_LEVELING_3POINT) && DISABLED(PROBE_MANUALLY)
#define HAS_ONESTEP_LEVELING 1
#endif
#if ANY(BABYSTEPPING, HAS_BED_PROBE, HAS_WORKSPACE_OFFSET)
#define HAS_ZOFFSET_ITEM 1
#endif
#if !HAS_BED_PROBE && ENABLED(BABYSTEPPING)
#define JUST_BABYSTEP 1
#endif
#include <WString.h>
#include <stdio.h>
#include <string.h>
#include "../../fontutils.h"
#include "../../marlinui.h"
#include "../../../sd/cardreader.h"
#include "../../../MarlinCore.h"
#include "../../../core/serial.h"
#include "../../../core/macros.h"
#include "../../../gcode/queue.h"
#include "../../../module/temperature.h"
#include "../../../module/printcounter.h"
#include "../../../module/motion.h"
#include "../../../module/planner.h"
#if ENABLED(EEPROM_SETTINGS)
#include "../../../module/settings.h"
#endif
#if ENABLED(HOST_ACTION_COMMANDS)
#include "../../../feature/host_actions.h"
#endif
#if HAS_ONESTEP_LEVELING
#include "../../../feature/bedlevel/bedlevel.h"
#endif
#if HAS_BED_PROBE
#include "../../../module/probe.h"
#endif
#if EITHER(BABYSTEP_ZPROBE_OFFSET, JUST_BABYSTEP)
#include "../../../feature/babystep.h"
#endif
#if ENABLED(POWER_LOSS_RECOVERY)
#include "../../../feature/powerloss.h"
#endif
#include "lcd_rts.h"
#include "../../dwin/ui_dacai.h"
#include "../../../module/AutoOffset.h"

#define LCD_WIDTH 240
#define LCD_HEIGHT 320
#define GRID_SIZE_X GRID_MAX_POINTS_X
#define GRID_SIZE_Y GRID_MAX_POINTS_Y
#define PADDING 10
#define Z_SCALE 15.0f
#define CELL_SIZE (min((LCD_WIDTH - PADDING) / (GRID_SIZE_X - 1), (LCD_HEIGHT - PADDING) / (GRID_SIZE_Y - 1)))
#define OFFSET_X ((LCD_WIDTH - ((GRID_SIZE_X - 1) * CELL_SIZE)) / 2)
#define OFFSET_Y ((LCD_HEIGHT - ((GRID_SIZE_Y - 1) * CELL_SIZE)) / 2) - 45
#define AXIS_COLOR 0x7BEF
#define LINE_COLOR 0xFFFF

uint16_t RGB24_TO_RGB565(uint8_t r, uint8_t g, uint8_t b)
{
    return ((r & 0xF8) << 8) | ((g & 0xFC) << 3) | (b >> 3);
}

void render_axes(float z_min, float z_max)
{
    uint16_t sx0 = OFFSET_X + LCD_WIDTH / 2;
    uint16_t sy0 = OFFSET_Y + LCD_HEIGHT / 2;

    // X Axis
    uint16_t sxX = sx0 + (GRID_SIZE_X - 1) * (CELL_SIZE / 2);
    uint16_t syX = sy0 + (GRID_SIZE_X - 1) * (CELL_SIZE / 4);
    DWIN_Draw_Line(AXIS_COLOR, sx0, sy0, sxX, syX);

    // Y Axis
    uint16_t sxY = sx0 - (GRID_SIZE_Y - 1) * (CELL_SIZE / 2);
    uint16_t syY = sy0 + (GRID_SIZE_Y - 1) * (CELL_SIZE / 4);
    DWIN_Draw_Line(AXIS_COLOR, sx0, sy0, sxY, syY);

    // Z Axis - draw upwards properly scaled and visible
    uint16_t z_height = Z_SCALE * (z_max - z_min); // Scaled height
    if (z_height < 55)
        z_height = 55; // Minimum height to ensure visibility
    uint16_t syZ = sy0 - z_height;
    DWIN_Draw_Line(AXIS_COLOR, sx0, sy0, sx0, syZ);
}

uint16_t getColorGradient(float value, float min, float max)
{
    float ratio = (value - min) / (max - min); // Normalize between 0.0 and 1.0
    ratio = constrain(ratio, 0.0f, 1.0f);      // Safety bounds

    uint8_t r, g, b;

    if (ratio < 0.5)
    {
        // From blue to green
        float t = ratio / 0.5; // Normalize to 0-1 in this range
        r = 0;
        g = (uint8_t)(255 * t);
        b = (uint8_t)(255 * (1 - t));
    }
    else
    {
        // From green to red
        float t = (ratio - 0.5) / 0.5; // Normalize to 0-1 in this range
        r = (uint8_t)(255 * t);
        g = (uint8_t)(255 * (1 - t));
        b = 0;
    }

    return RGB24_TO_RGB565(r, g, b);
}

// Function to map value to color based on your provided ranges
uint16_t getColorForHeight(float value)
{
    if (value >= 0.2f && value <= 1.0f)
        return RGB24_TO_RGB565(255, 0, 0); // Red
    else if (value >= 0.05f && value <= 0.1f)
        return RGB24_TO_RGB565(255, 255, 0); // Yellow
    else if (value >= -0.01f && value <= 0.05f)
        return RGB24_TO_RGB565(0, 255, 0); // Green
    else if (value >= -0.08f && value <= -0.01f)
        return RGB24_TO_RGB565(0, 0, 255); // Blue
    else if (value >= -1.0f && value <= -0.09f)
        return RGB24_TO_RGB565(148, 0, 211); // Violet (Purple)
    else
        return RGB24_TO_RGB565(255, 255, 255); // White (default if out of bounds)
}

void computeIsoCoord(uint8_t x, uint8_t y, float z, uint16_t &sx, uint16_t &sy)
{
    sx = OFFSET_X + (x - y) * (CELL_SIZE / 2) + LCD_WIDTH / 2;
    sy = OFFSET_Y + (x + y) * (CELL_SIZE / 4) - z + LCD_HEIGHT / 2;
}

void render_bed_mesh_3D()
{
    SERIAL_ECHOLN("START BED MESH 3D LINES");

    float z_min = z_values[0][0], z_max = z_values[0][0];
    for (uint8_t x = 0; x < GRID_SIZE_X; x++)
    {
        for (uint8_t y = 0; y < GRID_SIZE_Y; y++)
        {
            if (z_values[x][y] < z_min)
                z_min = z_values[x][y];
            if (z_values[x][y] > z_max)
                z_max = z_values[x][y];
        }
    }

    SERIAL_ECHOLNPAIR("z_min: ", z_min);
    SERIAL_ECHOLNPAIR("z_max: ", z_max);
    render_axes(z_min, z_max);

    float delta_z = (z_max - z_min);
    delta_z = (delta_z == 0) ? 1 : delta_z; // Protects against zero division

    float z_scale = Z_SCALE;

    for (uint8_t x = 0; x < GRID_SIZE_X - 1; x++)
    {
        for (uint8_t y = 0; y < GRID_SIZE_Y - 1; y++)
        {
            float z00 = pow((z_values[x][y] - z_min) / delta_z, 1.5) * z_scale;
            float z10 = pow((z_values[x + 1][y] - z_min) / delta_z, 1.5) * z_scale;
            float z01 = pow((z_values[x][y + 1] - z_min) / delta_z, 1.5) * z_scale;
            float z11 = pow((z_values[x + 1][y + 1] - z_min) / delta_z, 1.5) * z_scale;

            uint16_t sx00, sy00, sx10, sy10, sx01, sy01, sx11, sy11;
            computeIsoCoord(x, y, z00, sx00, sy00);
            computeIsoCoord(x + 1, y, z10, sx10, sy10);
            computeIsoCoord(x, y + 1, z01, sx01, sy01);
            computeIsoCoord(x + 1, y + 1, z11, sx11, sy11);

            float avg_z = (z_values[x][y] + z_values[x + 1][y] + z_values[x][y + 1] + z_values[x + 1][y + 1]) / 4.0;
            // uint16_t color = getColorGradient(avg_z, z_min, z_max);
            uint16_t color = getColorForHeight(avg_z);

            DWIN_Draw_Line(color, sx00, sy00, sx10, sy10);
            delay(3);
            DWIN_Draw_Line(color, sx10, sy10, sx11, sy11);
            delay(3);
            DWIN_Draw_Line(color, sx11, sy11, sx01, sy01);
            delay(3);
            DWIN_Draw_Line(color, sx01, sy01, sx00, sy00);
            delay(3);
            DWIN_Draw_Line(color, sx00, sy00, sx11, sy11);
            delay(3);
        }
    }

    char str[15];  // Sufficient buffer for string and number
    char str2[15]; // Sufficient buffer for string and number
    char str3[15]; // Sufficient buffer for string and number

    snprintf(str, sizeof(str), "Min Z: %.2f ", z_min);
    snprintf(str2, sizeof(str2), "Max Z: %.2f ", z_max);
    snprintf(str3, sizeof(str3), "Delta Z: %.2f ", delta_z);
    delay(3);
    DWIN_Draw_String(false, false, font10x20, Color_White, Color_Bg_Blue, 45, 4, F("Bed Level Visualizer"));
    DWIN_Draw_String(false, false, font6x12, Color_Yellow, Color_Bg_Black, 10, 40, str);
    DWIN_Draw_String(false, false, font6x12, Color_Yellow, Color_Bg_Black, 10, 60, str2);
    DWIN_Draw_String(false, false, font6x12, Color_Yellow, Color_Bg_Black, 10, 80, str3);
    delay(3);
    SERIAL_ECHOLN("FINISHED BED MESH 3D LINES");
}
