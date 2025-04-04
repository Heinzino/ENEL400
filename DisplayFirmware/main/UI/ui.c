// This file was generated by SquareLine Studio
// SquareLine Studio version: SquareLine Studio 1.5.0
// LVGL version: 8.3.11
// Project name: SquareLine_Project

#include "UI/ui.h"
#include "UI/ui_helpers.h"

///////////////////// VARIABLES ////////////////////


// SCREEN: ui_Screen1
void ui_Screen1_screen_init(void);
lv_obj_t * ui_Screen1;
lv_obj_t * ui_Label2;
lv_obj_t * ui_Label3;
lv_obj_t * ui_Label4;
lv_obj_t * ui_CurrentArc;
lv_obj_t * ui_Arc2;
lv_obj_t * ui_Arc3;
lv_obj_t * ui_Label5;
lv_obj_t * ui_Label1;
lv_obj_t * ui_Label6;
// CUSTOM VARIABLES


// SCREEN: ui_Screen2
void ui_Screen2_screen_init(void);
lv_obj_t * ui_Screen2;
lv_obj_t * ui_IncrementLevelBTN;
lv_obj_t * ui_LEVELTITLE;
lv_obj_t * ui_LEVELVAL;
lv_obj_t * ui_IncrementSign;
lv_obj_t * ui_IncrementLevelBTN1;
lv_obj_t * ui_DecrementSign;
// CUSTOM VARIABLES

// SCREEN: ui_Screen3
void ui_Screen3_screen_init(void);
lv_obj_t * ui_Screen3;
lv_obj_t * ui_CaloriesLabel;
lv_obj_t * ui_CaloriesUnit;
lv_obj_t * ui_CaloriesValue;
lv_obj_t * ui_CadenceLabel;
lv_obj_t * ui_CadenceValue;
lv_obj_t * ui_CadenceUnit;
lv_obj_t * ui_SpeedLabel;
lv_obj_t * ui_SpeedUnit;
lv_obj_t * ui_SpeedValue;
lv_obj_t * ui_DistanceLabel;
lv_obj_t * ui_DistanceUnit;
lv_obj_t * ui_SpeedValue1;
// CUSTOM VARIABLES

// SCREEN: ui_Screen4
void ui_Screen4_screen_init(void);
lv_obj_t * ui_Screen4;
lv_obj_t * ui_Chart1;
lv_obj_t * ui_PlotTitle;
lv_obj_t * ui_XLabel;
lv_obj_t * ui_YLabel;
lv_obj_t * ui_YLabel1;

// SCREEN: ui_Screen5
void ui_Screen5_screen_init(void);
lv_obj_t * ui_Screen5;
// lv_obj_t * ui_Image1;
// lv_obj_t * ui_Image2;
lv_obj_t * ui_CooldownMessage;
lv_obj_t * ui_SafeProceedMessage;
lv_obj_t * ui_Label7;

// CUSTOM VARIABLES
lv_obj_t * ui_BikePic;
lv_obj_t * batteryGroup;

// EVENTS
lv_obj_t * ui____initial_actions0;

// IMAGES AND IMAGE SETS

///////////////////// TEST LVGL SETTINGS ////////////////////
#if LV_COLOR_DEPTH != 16
    #error "LV_COLOR_DEPTH should be 16bit to match SquareLine Studio's settings"
#endif
#if LV_COLOR_16_SWAP !=0
    #error "LV_COLOR_16_SWAP should be 0 to match SquareLine Studio's settings"
#endif

///////////////////// ANIMATIONS ////////////////////

///////////////////// FUNCTIONS ////////////////////

///////////////////// SCREENS ////////////////////

void ui_init(void)
{
    lv_disp_t * dispp = lv_disp_get_default();
    lv_theme_t * theme = lv_theme_default_init(dispp, lv_palette_main(LV_PALETTE_BLUE), lv_palette_main(LV_PALETTE_RED),
                                               true, LV_FONT_DEFAULT);
    lv_disp_set_theme(dispp, theme);
    ui_Screen1_screen_init();
    ui_Screen2_screen_init();
    ui_Screen3_screen_init();
    ui_Screen4_screen_init();
    ui_Screen5_screen_init();
    ui_SpotifyScreen_screen_init();
    ui____initial_actions0 = lv_obj_create(NULL);
    lv_disp_load_scr(ui_Screen1);
}