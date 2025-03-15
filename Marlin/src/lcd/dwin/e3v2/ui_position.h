
#ifndef UI_POSITION_H
#define UI_POSITION_H

#if ENABLED(DWIN_CREALITY_480_LCD) //

#elif ENABLED(DWIN_CREALITY_320_LCD)//3.2 inch screen
//Main interface
    #define LOGO_LITTLE_X  72  //Small logo coordinates
    #define LOGO_LITTLE_Y  36
//Automatic leveling interface --Automatic leveling interface
//Edit Leveling Page --Edit Leveling Page
    #define WORD_TITLE_X 29 
    #define WORD_TITLE_Y 1
    #if GRID_MAX_POINTS_X <= 5
        #define OUTLINE_HORIZONTAL_MARGIN 12
    #else
        #define OUTLINE_HORIZONTAL_MARGIN 2
    #endif
    #define OUTLINE_INNER_WIDTH (DWIN_WIDTH - (OUTLINE_HORIZONTAL_MARGIN * 2))
    #define OUTLINE_INNER_HEIGHT 220
    #define OUTLINE_LEFT_X OUTLINE_HORIZONTAL_MARGIN
    #define OUTLINE_LEFT_Y 30
    #define OUTLINE_RIGHT_X (OUTLINE_HORIZONTAL_MARGIN + OUTLINE_INNER_WIDTH)
    #define OUTLINE_RIGHT_Y (OUTLINE_LEFT_Y + OUTLINE_INNER_HEIGHT)
   
    //Button——position
    #define BUTTON_W 82
    #define BUTTON_H 32
    #define BUTTON_EDIT_X OUTLINE_LEFT_X
    #define BUTTON_EDIT_Y OUTLINE_RIGHT_Y+20//Outline right y+27
    #define BUTTON_OK_X  OUTLINE_RIGHT_X-BUTTON_W
    #define BUTTON_OK_Y  BUTTON_EDIT_Y//
    //Data coordinates --Data coordinates

    #if ENABLED(SHOW_GRID_VALUES) && GRID_MAX_POINTS_X >= 7 && DISABLED(COMPACT_GRID_VALUES)
        #error "Cannot fit GRID_MAX_POINTS_X >= 7 into LCD screen. Either set GRID_MAX_POINTS_X to lower value or enable COMPACT_GRID_VALUES"
    #endif
    #if ENABLED(SHOW_GRID_VALUES) && GRID_MAX_POINTS_X > 10
        #error "Cannot fit GRID_MAX_POINTS_X > 10 into LCD screen. Either set GRID_MAX_POINTS_X to lower value or disable SHOW_GRID_VALUES"
    #endif
    #if ENABLED(SHOW_GRID_VALUES) && GRID_MAX_POINTS_Y > 10
        #error "Cannot fit GRID_MAX_POINTS_Y > 10 into LCD screen. Either set GRID_MAX_POINTS_Y to lower value or disable SHOW_GRID_VALUES"
    #endif

    #if ENABLED(COMPACT_GRID_VALUES)
        #define CELL_TEXT_WIDTH (3 * 8 + 1) // ".00" or "1.0" gives maximum 3 chars of 8x12 font per cell
        #define CELL_TEXT_HEIGHT 22 // 12px for character height of 8x12 font + 10px for padding per cell
    #else // COMPACT_GRID_VALUES
        #define CELL_TEXT_WIDTH (5 * 8) // "-0.00" gives maximum 5 chars of 8x12 font per cell
        #define CELL_TEXT_HEIGHT 14 // 12px for character height of 8x12 font + 14px for padding per cell
    #endif // COMPACT_GRID_VALUES

    #define _TOTAL_MIN_CELL_WIDTH (CELL_TEXT_WIDTH * GRID_MAX_POINTS_X)
    #define _TOTAL_MIN_CELL_HEIGHT (CELL_TEXT_HEIGHT * GRID_MAX_POINTS_Y)

    #define _SPACE_TO_SPREAD_HORIZONTAL (OUTLINE_INNER_WIDTH - _TOTAL_MIN_CELL_WIDTH)
    #define _SPACE_TO_SPREAD_VERTICAL (OUTLINE_INNER_HEIGHT - _TOTAL_MIN_CELL_HEIGHT)

    #if (_SPACE_TO_SPREAD_HORIZONTAL / (GRID_MAX_POINTS_X + GRID_MAX_POINTS_X + 1)) != 0 // allocate space evenly to celss and cell spacing and outer spacing
        #define _CELL_GROW_HORIZONTAL (_SPACE_TO_SPREAD_HORIZONTAL / (GRID_MAX_POINTS_X + GRID_MAX_POINTS_X + 1))
        #define _CELL_SPACING_HORIZONTAL (_SPACE_TO_SPREAD_HORIZONTAL / (GRID_MAX_POINTS_X + GRID_MAX_POINTS_X + 1))
    #elif (_SPACE_TO_SPREAD_HORIZONTAL / (GRID_MAX_POINTS_X + GRID_MAX_POINTS_X - 1)) != 0 // allocate space evenly to celss and cell spacing
        #define _CELL_GROW_HORIZONTAL (_SPACE_TO_SPREAD_HORIZONTAL / (GRID_MAX_POINTS_X + GRID_MAX_POINTS_X - 1))
        #define _CELL_SPACING_HORIZONTAL (_SPACE_TO_SPREAD_HORIZONTAL / (GRID_MAX_POINTS_X + GRID_MAX_POINTS_X - 1))
    #elif (_SPACE_TO_SPREAD_HORIZONTAL / GRID_MAX_POINTS_X) != 0 // allocate space evenly to cells
        #define _CELL_GROW_HORIZONTAL (_SPACE_TO_SPREAD_HORIZONTAL / GRID_MAX_POINTS_X)
        #define _CELL_SPACING_HORIZONTAL 0
    #elif (_SPACE_TO_SPREAD_HORIZONTAL / GRID_MAX_POINTS_X - 1) != 0 // allocate space evenly to cell spacing
        #define _CELL_GROW_HORIZONTAL 0
        #define _CELL_SPACING_HORIZONTAL (_SPACE_TO_SPREAD_HORIZONTAL / GRID_MAX_POINTS_X - 1)
    #else
        #define _CELL_GROW_HORIZONTAL 0
        #define _CELL_SPACING_HORIZONTAL 0
    #endif

    #if (_SPACE_TO_SPREAD_VERTICAL / (GRID_MAX_POINTS_Y + GRID_MAX_POINTS_Y + 1)) != 0 // allocate space evenly to celss and cell spacing
        #define _CELL_GROW_VERTICAL (_SPACE_TO_SPREAD_VERTICAL / (GRID_MAX_POINTS_Y + GRID_MAX_POINTS_Y + 1))
        #define _CELL_SPACING_VERTICAL (_SPACE_TO_SPREAD_VERTICAL / (GRID_MAX_POINTS_Y + GRID_MAX_POINTS_Y + 1))
    #elif (_SPACE_TO_SPREAD_VERTICAL / (GRID_MAX_POINTS_Y + GRID_MAX_POINTS_Y - 1)) != 0 // allocate space evenly to celss and cell spacing
        #define _CELL_GROW_VERTICAL (_SPACE_TO_SPREAD_VERTICAL / (GRID_MAX_POINTS_Y + GRID_MAX_POINTS_Y - 1))
        #define _CELL_SPACING_VERTICAL (_SPACE_TO_SPREAD_VERTICAL / (GRID_MAX_POINTS_Y + GRID_MAX_POINTS_Y - 1))
    #elif (_SPACE_TO_SPREAD_VERTICAL / GRID_MAX_POINTS_Y) != 0 // allocate space evenly to cells
        #define _CELL_GROW_VERTICAL (_SPACE_TO_SPREAD_VERTICAL / GRID_MAX_POINTS_Y)
        #define _CELL_SPACING_VERTICAL 0
    #elif (_SPACE_TO_SPREAD_VERTICAL / GRID_MAX_POINTS_Y - 1) != 0 // allocate space evenly to cell spacing
        #define _CELL_GROW_VERTICAL 0
        #define _CELL_SPACING_VERTICAL (_SPACE_TO_SPREAD_VERTICAL / GRID_MAX_POINTS_Y - 1)
    #else
        #define _CELL_GROW_VERTICAL 0
        #define _CELL_SPACING_VERTICAL 0
    #endif

    #define CELL_WIDTH (CELL_TEXT_WIDTH + _CELL_GROW_HORIZONTAL)
    #define CELL_HEIGHT (CELL_TEXT_HEIGHT + _CELL_GROW_VERTICAL)

    #define X_Axis_Interval (CELL_WIDTH + _CELL_SPACING_HORIZONTAL)
    #define Y_Axis_Interval (CELL_HEIGHT + _CELL_SPACING_VERTICAL)
    
    #define _CELLS_WITH_SPACING_WIDTH    ((GRID_MAX_POINTS_X * CELL_WIDTH) + ((GRID_MAX_POINTS_X - 1) * _CELL_SPACING_HORIZONTAL))
    #define _CELLS_WITH_SPACING_HEIGHT   ((GRID_MAX_POINTS_Y * CELL_HEIGHT) + ((GRID_MAX_POINTS_Y - 1) * _CELL_SPACING_VERTICAL))
    #define _OUTLINE_PADDING_HORIZONTAL  ((OUTLINE_INNER_WIDTH - _CELLS_WITH_SPACING_WIDTH) / 2)
    #define _OUTLINE_PADDING_VERTICAL    ((OUTLINE_INNER_HEIGHT - _CELLS_WITH_SPACING_HEIGHT) / 2)

    #define Rect_LU_X_POS    (OUTLINE_LEFT_X + _OUTLINE_PADDING_HORIZONTAL) // top-left X of the first (bottom-left) cell
    #define Rect_LU_Y_POS    (OUTLINE_LEFT_Y + OUTLINE_INNER_HEIGHT - _OUTLINE_PADDING_VERTICAL - CELL_HEIGHT) // top-left Y of the first (bottom-left) cell
    #define Rect_RD_X_POS    (Rect_LU_X_POS + CELL_WIDTH) // bottom-right X of the first (bottom-left) cell
    #define Rect_RD_Y_POS    (Rect_LU_Y_POS + CELL_HEIGHT) // bottom-right Y of the first (bottom-left) cell

#define TITLE_X 29
#define TITLE_Y  1
//  TITLE_X, TITLE_Y);
//Start page
#define CREALITY_LOGO_X   20
#define CREALITY_LOGO_Y   96
//Main interface
    #define LOGO_LITTLE_X  72  //Small logo coordinates
    #define LOGO_LITTLE_Y  36

    #define ICON_PRINT_X 12
    #define ICON_PRINT_Y 51
    #define ICON_W 102-1
    #define ICON_H 115-1

    #define ICON_PREPARE_X 126
    #define ICON_PREPARE_Y 51
    #define ICON_CONTROL_X 12
    #define ICON_CONTROL_Y 178
    #define ICON_LEVEL_X 126
    #define ICON_LEVEL_Y 178

    #define WORD_PRINT_X 12+1
    #define WORD_PRINT_Y 120
    #define WORD_PREPARE_X 126+1
    #define WORD_PREPARE_Y 120
    #define WORD_CONTROL_X 12+1
    #define WORD_CONTROL_Y 247
    #define WORD_LEVEL_X 126+1
    #define WORD_LEVEL_Y 247

    //Print confirmation page
    #define ICON_Defaut_Image_X 72
    #define ICON_Defaut_Image_Y 25

    //Control 》Information page
    #define ICON_SIZE_X  20  // 
    #define ICON_SIZE_Y  75  //39
    // #define ICON_SIZE_X 20 // 
    // #define ICON_SIZE_Y 79 //39
    //Language selection page
    #define WORD_LAGUAGE_X 20


    //Printing interface
    #define FIEL_NAME_X   12 
     #define FIEL_NAME_Y  24+11 
    #define ICON_PRINT_TIME_X 117
    #define ICON_PRINT_TIME_Y 77
    #define WORD_PRINT_TIME_X 141 
    #define WORD_PRINT_TIME_Y 61 
    #define NUM_PRINT_TIME_X  WORD_PRINT_TIME_X
    #define NUM_PRINT_TIME_Y  92

    #define ICON_RAMAIN_TIME_X ICON_PRINT_TIME_X
    #define ICON_RAMAIN_TIME_Y ICON_PRINT_TIME_Y+37+24
    #define WORD_RAMAIN_TIME_X WORD_PRINT_TIME_X
    #define WORD_RAMAIN_TIME_Y WORD_PRINT_TIME_Y+31+30
    #define NUM_RAMAIN_TIME_X  NUM_PRINT_TIME_X
    #define NUM_RAMAIN_TIME_Y  NUM_PRINT_TIME_Y+40+21

    //Underline 1,2,3
    #define LINE_X_START  12
    #define LINE_Y_START  59            
    #define LINE_X_END    12+216
    #define LINE_Y_END    LINE_Y_START+1
    #define LINE2_SPACE    122
    #define LINE3_SPACE    LINE2_SPACE+108
    // //划线3.4
    // #define LINE3_X_START  94
    // #define LINE3_Y_START  94            
    // #define LINE3_X_END    LINE3_X_START+134
    // #define LINE3_Y_END    LINE3_Y_START+1
    // #define LINE3_SPACE    27

    

    #define ICON_PERCENT_X   12
    #define ICON_PERCENT_Y   75
     //Print parameter icon coordinates
    #define ICON_NOZZ_X  6//ICON_PERCENT_X //Nozzle icon
    #define ICON_NOZZ_Y  268.5
    #define ICON_BED_X   ICON_NOZZ_X  //Hot bed icon
    #define ICON_BED_Y   ICON_NOZZ_Y+6+20
    #define ICON_SPEED_X 99//ICON_NOZZ_X+64+20 //Print speed
    #define ICON_SPEED_Y  ICON_NOZZ_Y
    #define ICON_STEP_E_X ICON_SPEED_X//ICON_BED_X+64+20 //E-axis extrusion ratio
    #define ICON_STEP_E_Y ICON_BED_Y
    #define ICON_ZOFFSET_X 171//Icon step e x+64+20//z-axis compensation value
    #define ICON_ZOFFSET_Y ICON_STEP_E_Y
    #define ICON_FAN_X     171//Icon speed x+64+20//Fan duty cycle
    #define ICON_FAN_Y     ICON_SPEED_Y
//Print parameter data coordinates
    #define ICON_TO_NUM_OFFSET 4
    #define NUM_NOZZ_X     ICON_NOZZ_X+18  //(32,388)
    #define NUM_NOZZ_Y     ICON_NOZZ_Y+ICON_TO_NUM_OFFSET
    #define NUM_BED_X      ICON_BED_X+18   //(32,388)
    #define NUM_BED_Y      ICON_BED_Y+ICON_TO_NUM_OFFSET
    #define NUM_SPEED_X    ICON_SPEED_X+20
    #define NUM_SPEED_Y    ICON_SPEED_Y+ICON_TO_NUM_OFFSET
    #define NUM_STEP_E_X   ICON_STEP_E_X+20
    #define NUM_STEP_E_Y   ICON_STEP_E_Y+ICON_TO_NUM_OFFSET
    #define NUM_ZOFFSET_X  ICON_ZOFFSET_X+18
    #define NUM_ZOFFSET_Y  ICON_ZOFFSET_Y+ICON_TO_NUM_OFFSET
    #define NUM_FAN_X      ICON_FAN_X+18
    #define NUM_FAN_Y      ICON_FAN_Y+ICON_TO_NUM_OFFSET
    
    // #define NUM_PRECENT_X  ICON_PERCENT_X+21
    // #define NUM_PRECENT_Y  ICON_PERCENT_Y+26
    // #define PRECENT_X  NUM_PRECENT_X+20
    // #define PRECENT_Y  NUM_PRECENT_Y

    //Icon coordinates
    #define RECT_SET_X1  12
    #define RECT_SET_Y1  191
    #define RECT_SET_X2  RECT_SET_X1+68
    #define RECT_SET_Y2  RECT_SET_Y1+64
    #define RECT_OFFSET_X   6+68

    #define ICON_SET_X  RECT_SET_X1//Rect set x1+19  
    #define ICON_SET_Y  RECT_SET_Y1//Rect set y1+5
    #define WORD_SET_X  RECT_SET_X1  
    #define WORD_SET_Y  RECT_SET_Y1+34

    #define ICON_PAUSE_X  ICON_SET_X+RECT_OFFSET_X  
    #define ICON_PAUSE_Y  ICON_SET_Y
    #define WORD_PAUSE_X  ICON_PAUSE_X //Word set x+rect offset x   
    #define WORD_PAUSE_Y  WORD_SET_Y

    #define ICON_STOP_X  ICON_PAUSE_X+RECT_OFFSET_X  
    #define ICON_STOP_Y  ICON_PAUSE_Y
    #define WORD_STOP_X  ICON_STOP_X //Word pause x+rect offset x  
    #define WORD_STOP_Y  WORD_PAUSE_Y

  

//Align the interface coordinates with one click
    #define LINE_X  111
    #define LINE_Y  117
    #define LINE_Y_SPACE 81+18

    #define WORD_NOZZ_HOT_X 24
    #define WORD_NOZZ_HOT_Y 72
    #define WORD_Y_SPACE    54+45

    #define ICON_NOZZ_HOT_X 102
    #define ICON_NOZZ_HOT_Y 36
    #define ICON_Y_SPACE    63+36
//Leveling failed ui coordinates
#define ICON_LEVELING_ERR_X  90
#define ICON_LEVELING_ERR_Y  79
#define WORD_LEVELING_ERR_X  16
#define WORD_LEVELING_ERR_Y  148

//Printing complete clear bottom range
#define CLEAR_50_X    0
#define CLEAR_50_Y    ICON_SET_Y-1
#define OK_BUTTON_X   72  //Print completion button location
#define OK_BUTTON_Y   264

//Boot boot, language selection
#define FIRST_X  20
#define FIRST_Y  29
#define LINE_START_X 12
#define LINE_START_Y 39+24
#define LINE_END_X   LINE_START_X+215
#define LINE_END_Y   LINE_START_Y+1
#define WORD_INTERVAL  40
#define LINE_INTERVAL  40
#define REC_X1   0
#define REC_Y1   25
#define REC_X2   10
#define REC_Y2   REC_Y1+37
#define REC_INTERVAL 40

//Abnormal interface coordinates
#define ICON_X 90
#define ICON_Y 53
#define WORD_X 16 
#define WORD_Y ICON_X+9+60
#define REC_60_X1 16
#define REC_60_Y1 41
#define REC_60_X2 REC_X1+208
#define REC_60_Y2 REC_Y1+210

//Pid setting interface
#define WORD_AUTO_X 12
#define WORD_AUTO_Y  45+24
#define WORD_TEMP_X 84
#define WORD_TEMP_Y 101+24
#define ICON_AUTO_X 0
#define ICON_AUTO_Y 101+24
#define SHOW_CURE_X1 34
#define SHOW_CURE_Y1 167 //Icon car and 28  
#define SHOW_CURE_X2 SHOW_CURE_X1+194//Show cure x1+191
#define SHOW_CURE_Y2 236//Show cure y1+71

// Curve data related
#define CURVE_POINT_NUM       40
#define Curve_Color_Bed       0xFF6B17 // light red
#define Curve_Color_Nozzle    0xFF0E42 // red
#define Curve_Psition_Start_X   SHOW_CURE_X1//34 //0x0000 //Upper left
#define Curve_Psition_Start_Y   SHOW_CURE_Y1//144    //0x0000
#define Curve_Psition_End_X     SHOW_CURE_X2//225 //0x0000 //Lower right
#define Curve_Psition_End_Y     SHOW_CURE_Y2//221-6 //0x0000 //Lower right
#define Curve_DISTANCE_BED_Y    70
#define Curve_DISTANCE_NOZZLE_Y 70
#define Curve_Line_Width        0x01
#define Curve_Step_Size_X       5      // 0x05
#define Curve_Bed_Size_Y       Curve_DISTANCE_BED_Y*256/150  // 90 //Y-axis data pixel interval y data ratio
#define Curve_Nozzle_Size_Y    Curve_DISTANCE_NOZZLE_Y*256/300
#define Curve_Zero_Y           Curve_Psition_End_Y+2                                 // 0x0556 Y axis 0 point coordinate

//Picture preview interface icon position

#if ENABLED(USER_LEVEL_CHECK)  //Using the leveling calibration function
    #define WORD_TIME_X  12  
    #define WORD_TIME_Y  122
    #define WORD_LENTH_X WORD_TIME_X
    #define WORD_LENTH_Y WORD_TIME_Y+21
    #define WORD_HIGH_X  WORD_TIME_X
    #define WORD_HIGH_Y  WORD_LENTH_Y+21
    #define WORD_PRINT_CHECK_X WORD_TIME_X
    #define WORD_PRINT_CHECK_Y WORD_HIGH_Y+21+14
    
    #define ICON_ON_OFF_X  WORD_PRINT_CHECK_X+160+20
    #define ICON_ON_OFF_Y  WORD_PRINT_CHECK_Y

    #define BUTTON_X  12
    #define BUTTON_Y  230
    #define BUTTON_OFFSET_X BUTTON_X+82+52-10 
#else
    #define WORD_TIME_X  12  
    #define WORD_TIME_Y  134
    #define WORD_LENTH_X WORD_TIME_X
    #define WORD_LENTH_Y WORD_TIME_Y+21+4
    #define WORD_HIGH_X  WORD_TIME_X
    #define WORD_HIGH_Y  WORD_LENTH_Y+21+4

    #define BUTTON_X  12
    #define BUTTON_Y  225
    #define BUTTON_OFFSET_X BUTTON_X+82+52-10 
#endif
#define DATA_OFFSET_X  103+60
#define DATA_OFFSET_Y  4


//Boot boot pop-up window
#define POPUP_BG_X_LU  16
#define POPUP_BG_Y_LU  67
#define POPUP_TITLE_HEIGHT 26
#define POPUP_BG_X_RD  224
#define POPUP_BG_Y_RD  277
//Cleaning tips
#define WORD_HINT_CLEAR_X POPUP_BG_X_LU
#define WORD_HINT_CLEAR_Y 17+POPUP_BG_Y_LU
//Tips for cleaning after failure
#define WORD_HIGH_CLEAR_X POPUP_BG_X_LU
#define WORD_HIGH_CLEAR_Y 81+POPUP_BG_Y_LU
#define ICON_HIGH_ERR_X   70+POPUP_BG_X_LU
#define ICON_HIGH_ERR_Y   12+POPUP_BG_Y_LU
//Leveling failed
#define ICON_LEVEL_ERR_X  42+POPUP_BG_X_LU
#define ICON_LEVEL_ERR_Y  11+POPUP_BG_Y_LU
#define WORD_SCAN_QR_X    POPUP_BG_X_LU
#define WORD_SCAN_QR_Y    146+POPUP_BG_Y_LU

#define BUTTON_BOOT_X  POPUP_BG_X_LU+63
#define BUTTON_BOOT_Y  POPUP_BG_Y_LU+164
#define BUTTON_BOOT_LEVEL_X  79
#define BUTTON_BOOT_LEVEL_Y  260


//The coordinates of each point in G29
#define G29_X_MIN 3
#define G29_Y_MIN 3
#define G29_X_MAX 200.75
#define G29_Y_MAX 212
#define G29_X_INTERVAL ((G29_X_MAX-G29_X_MIN)/(GRID_MAX_POINTS_X-1))
#define G29_Y_INTERVAL ((G29_Y_MAX-G29_Y_MIN)/(GRID_MAX_POINTS_Y-1))


#endif 

#endif   //E3 s1 laser h