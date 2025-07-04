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
#pragma once

#include "../../inc/MarlinConfigPre.h"

#if EITHER(RESTORE_LEVELING_AFTER_G28, ENABLE_LEVELING_AFTER_G28)
  #define G28_L0_ENSURES_LEVELING_OFF 1
#endif

#if ENABLED(PROBE_MANUALLY)
  extern bool g29_in_progress;
#else
  constexpr bool g29_in_progress = false;
#endif

bool leveling_is_valid();
void set_bed_leveling_enabled(const bool enable=true);
void reset_bed_level();

#if ENABLED(ENABLE_LEVELING_FADE_HEIGHT)
  void set_z_fade_height(const_float_t zfh, const bool do_report=true);
#endif

#if EITHER(MESH_BED_LEVELING, PROBE_MANUALLY)
  void _manual_goto_xy(const xy_pos_t &pos);
#endif

/**
 * A class to save and change the bed leveling state,
 * then restore it when it goes out of scope.
 */
class TemporaryBedLevelingState {
  bool saved;
  public:
    TemporaryBedLevelingState(const bool enable);
    ~TemporaryBedLevelingState() { set_bed_leveling_enabled(saved); }
};
#define TEMPORARY_BED_LEVELING_STATE(enable) const TemporaryBedLevelingState tbls(enable)

#if HAS_MESH

  typedef float bed_mesh_t[GRID_MAX_POINTS_X][GRID_MAX_POINTS_Y];

  #if ENABLED(AUTO_BED_LEVELING_BILINEAR)
    #include "abl/abl.h"
  #elif ENABLED(AUTO_BED_LEVELING_UBL)
    #include "ubl/ubl.h"
  #elif ENABLED(MESH_BED_LEVELING)
    #include "mbl/mesh_bed_leveling.h"
  #endif

  #define Z_VALUES(X,Y) Z_VALUES_ARR[X][Y]
  #define _GET_MESH_POS(M) { _GET_MESH_X(M.a), _GET_MESH_Y(M.b) }

  #if EITHER(AUTO_BED_LEVELING_BILINEAR, MESH_BED_LEVELING)

    #include <stdint.h>

    typedef float (*element_2d_fn)(const uint8_t, const uint8_t);

    /**
     * Print calibration results for plotting or manual frame adjustment.
     */
    void print_2d_array(const uint8_t sx, const uint8_t sy, const uint8_t precision, element_2d_fn fn);

  #endif

  struct mesh_index_pair {
    xy_int8_t pos;
    float distance;   // When populated, the distance from the search location
    void invalidate() { pos = -1; }
    bool valid() const { return pos.x >= 0 && pos.y >= 0; }
    #if ENABLED(AUTO_BED_LEVELING_UBL)
      xy_pos_t meshpos() {
        return { ubl.mesh_index_to_xpos(pos.x), ubl.mesh_index_to_ypos(pos.y) };
      }
    #endif
    operator xy_int8_t&() { return pos; }
    operator const xy_int8_t&() const { return pos; }
  };

  #if ENABLED(ADVANCED_HELP_MESSAGES)
    enum BedLevel_Score : uint8_t{
      BedLevel_Perfect = 0,
      BedLevel_Good,
      BedLevel_Ok,
      BedLevel_Bad,
      BedLevel_Horrible,
    };

    struct BedLevelResult {
        BedLevel_Score score;
        float stddev;
        float delta;
        float z_min;
        float z_max;
    };

    // Bed level assessment stddev thresholds
    #define BED_LEVEL_STDDEV_PERFECT 0.1f
    #define BED_LEVEL_STDDEV_GOOD 0.2f
    #define BED_LEVEL_STDDEV_OK 0.3f
    #define BED_LEVEL_STDDEV_BAD 1.0f

    // Bed level assessment min-max delta thresholds
    #define BED_LEVEL_DELTA_PERFECT 0.3f
    #define BED_LEVEL_DELTA_GOOD 0.5f
    #define BED_LEVEL_DELTA_OK 1.0f
    #define BED_LEVEL_DELTA_BAD 2.0f

    // Bed level assessment min-max delta thresholds
    #define BED_LEVEL_SLOPE_THRESHOLD_0 0.05f
    #define BED_LEVEL_SLOPE_THRESHOLD_1 0.1f
    #define BED_LEVEL_SLOPE_THRESHOLD_2 1.0f

    BedLevelResult assess_bed_level();
  #endif

#endif
