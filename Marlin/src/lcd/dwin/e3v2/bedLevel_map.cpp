#include "../../../inc/MarlinConfigPre.h"

#if ENABLED(ADVANCED_HELP_MESSAGES)
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

#define PADDING 10
#define MIN_MESH_HEIGHT 55
#define Z_SCALE 15.0f
#define CELL_SIZE (min((DWIN_WIDTH - PADDING) / (GRID_MAX_POINTS_X - 1), (DWIN_HEIGHT - PADDING) / (GRID_MAX_POINTS_Y - 1)))
#define OFFSET_X ((DWIN_WIDTH - ((GRID_MAX_POINTS_X - 1) * CELL_SIZE)) / 2)
#define OFFSET_Y ((DWIN_HEIGHT - ((GRID_MAX_POINTS_Y - 1) * CELL_SIZE)) / 2) - 45
#define AXIS_COLOR 0x7BEF
#define LINE_COLOR 0xFFFF

uint16_t RGB24_TO_RGB565(uint8_t r, uint8_t g, uint8_t b)
{
    return ((r & 0xF8) << 8) | ((g & 0xFC) << 3) | (b >> 3);
}

void render_axes(float z_delta)
{
    uint16_t sx0 = OFFSET_X + DWIN_WIDTH / 2;
    uint16_t sy0 = OFFSET_Y + DWIN_HEIGHT / 2;

    // X Axis
    uint16_t sxX = sx0 + (GRID_MAX_POINTS_X - 1) * (CELL_SIZE / 2);
    uint16_t syX = sy0 + (GRID_MAX_POINTS_X - 1) * (CELL_SIZE / 4);
    DWIN_Draw_Line(AXIS_COLOR, sx0, sy0, sxX, syX);

    // Y Axis
    uint16_t sxY = sx0 - (GRID_MAX_POINTS_Y - 1) * (CELL_SIZE / 2);
    uint16_t syY = sy0 + (GRID_MAX_POINTS_Y - 1) * (CELL_SIZE / 4);
    DWIN_Draw_Line(AXIS_COLOR, sx0, sy0, sxY, syY);

    // Z Axis - draw upwards properly scaled and visible
    uint16_t z_height = Z_SCALE * z_delta; // Scaled height
    if (z_height < MIN_MESH_HEIGHT)
        z_height = MIN_MESH_HEIGHT; // Minimum height to ensure visibility
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
    if (value >= BED_LEVEL_SLOPE_THRESHOLD_2) return HeightColor_VeryHigh;
    if (value >= BED_LEVEL_SLOPE_THRESHOLD_1) return HeightColor_High;
    if (value >= BED_LEVEL_SLOPE_THRESHOLD_0) return HeightColor_FlatHigh;

    if (value <= -(BED_LEVEL_SLOPE_THRESHOLD_2)) return HeightColor_VeryLow;
    if (value <= -(BED_LEVEL_SLOPE_THRESHOLD_1)) return HeightColor_Low;
    if (value <= -(BED_LEVEL_SLOPE_THRESHOLD_0)) return HeightColor_FlatLow;

    return HeightColor_FlatLow;
}

void computeIsoCoord(uint8_t x, uint8_t y, float z, uint16_t &sx, uint16_t &sy)
{
    sx = OFFSET_X + (x - y) * (CELL_SIZE / 2) + DWIN_WIDTH / 2;
    sy = OFFSET_Y + (x + y) * (CELL_SIZE / 4) - z + DWIN_HEIGHT / 2;
}

void render_bed_mesh_3D()
{
    float z_min = z_values[0][0], z_max = z_values[0][0];
    for (uint8_t x = 0; x < GRID_MAX_POINTS_X; x++)
    {
        for (uint8_t y = 0; y < GRID_MAX_POINTS_Y; y++)
        {
            if (z_values[x][y] < z_min)
                z_min = z_values[x][y];
            if (z_values[x][y] > z_max)
                z_max = z_values[x][y];
        }
    }
    BedLevelResult result = assess_bed_level();
    SERIAL_ECHOLNPAIR("LevelMap: z_min: ", z_min);
    SERIAL_ECHOLNPAIR("LevelMap: result.z_min: ", result.z_min);
    SERIAL_ECHOLNPAIR("LevelMap: result.z_max: ", result.z_max);
    SERIAL_ECHOLNPAIR("LevelMap: result.delta: ", result.delta);

    render_axes(result.delta);

    float delta_z = (result.delta == 0) ? 1 : result.delta; // Protects against zero division

    for (uint8_t x = 0; x < GRID_MAX_POINTS_X - 1; x++)
    {
        for (uint8_t y = 0; y < GRID_MAX_POINTS_Y - 1; y++)
        {
            float z00 = pow((z_values[x][y] - result.z_min) / delta_z, 1.5) * Z_SCALE;
            float z10 = pow((z_values[x + 1][y] - result.z_min) / delta_z, 1.5) * Z_SCALE;
            float z01 = pow((z_values[x][y + 1] - result.z_min) / delta_z, 1.5) * Z_SCALE;
            float z11 = pow((z_values[x + 1][y + 1] - result.z_min) / delta_z, 1.5) * Z_SCALE;

            uint16_t sx00, sy00, sx10, sy10, sx01, sy01, sx11, sy11;
            computeIsoCoord(x, y, z00, sx00, sy00);
            computeIsoCoord(x + 1, y, z10, sx10, sy10);
            computeIsoCoord(x, y + 1, z01, sx01, sy01);
            computeIsoCoord(x + 1, y + 1, z11, sx11, sy11);

            float avg_z = (z_values[x][y] + z_values[x + 1][y] + z_values[x][y + 1] + z_values[x + 1][y + 1]) / 4.0;
            // uint16_t color = getColorGradient(avg_z, z_min, z_max);
            uint16_t color = getColorForHeight(avg_z);

            DWIN_Draw_Line(color, sx00, sy00, sx10, sy10);
            DWIN_Draw_Line(color, sx10, sy10, sx11, sy11);
            DWIN_Draw_Line(color, sx11, sy11, sx01, sy01);
            DWIN_Draw_Line(color, sx01, sy01, sx00, sy00);
            DWIN_Draw_Line(color, sx00, sy00, sx11, sy11);
            delay(10);
        }
    }

    char delta_str[15];
    char stddev_str[15];
    // char score_str[22];

    sprintf(delta_str, "Delta: %.2f", result.delta);
    sprintf(stddev_str, "StdDev: %.3f", result.stddev);

    DWIN_Draw_String(false, false, font10x20, Color_White, Color_Bg_Blue, 45, 4, F("Bed Level Visualizer"));
    DWIN_Draw_String(false, false, font6x12, Color_Yellow, Color_Bg_Black, 10, 40, delta_str);
    DWIN_Draw_String(false, false, font6x12, Color_Yellow, Color_Bg_Black, 10, 60, stddev_str);
    DWIN_Draw_String(false, false, font6x12, Color_Yellow, Color_Bg_Black, 10, 80, F("Bed level score:"));

    switch(result.score) {
        case BedLevel_Perfect:
            DWIN_Draw_String(false, false, font14x28, Flat_Color, Color_Bg_Black, 10, 100, F("PERFECT"));
            break;
        case BedLevel_Good:
            DWIN_Draw_String(false, false, font14x28, Relatively_Flat, Color_Bg_Black, 10, 100, F("GOOD"));
            break;
        case BedLevel_Ok:
            DWIN_Draw_String(false, false, font14x28, Slope_Small, Color_Bg_Black, 10, 100, F("OK"));
            break;
        case BedLevel_Bad:
            DWIN_Draw_String(false, false, font14x28, Slope_Big, Color_Bg_Black, 10, 100, F("BAD"));
            break;
        case BedLevel_Horrible:
            DWIN_Draw_String(false, false, font14x28, Slope_Big, Color_Bg_Black, 10, 100, F("HORRIBLE"));
            break;
        default:
            DWIN_Draw_String(false, false, font14x28, Slope_Big, Color_Bg_Black, 10, 100, F("Unknown"));
            break;
    }
    delay(3);
    DWIN_Draw_qrcode(155, 40, 3, "https://bit.ly/navaismo-bed-leveling");
    SERIAL_ECHOLN("FINISHED BED MESH 3D LINES");
}

#endif // ENABLED(ADVANCED_HELP_MESSAGES)