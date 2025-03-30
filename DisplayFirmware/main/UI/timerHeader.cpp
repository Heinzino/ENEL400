#include "UI/timerHeader.hpp"

lv_obj_t *elapsed_label = nullptr;
lv_obj_t *datetime_label = nullptr;

void create_time_header() {
    lv_obj_t *time_header = lv_obj_create(lv_layer_top());
    lv_obj_set_size(time_header, LV_HOR_RES, 40);
    
    //  Remove unwanted styles (scroll, borders)
    lv_obj_clear_flag(time_header, LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_set_style_border_width(time_header, 0, LV_PART_MAIN);
    lv_obj_set_style_pad_all(time_header, 0, LV_PART_MAIN);
    lv_obj_set_style_radius(time_header, 0, LV_PART_MAIN);
    
    //  Set background color to #101418
    lv_obj_set_style_bg_color(time_header, lv_color_hex(0x101418), LV_PART_MAIN);
    lv_obj_set_style_text_color(time_header, lv_color_hex(0xFFFFFF), LV_PART_MAIN);

    //  Create a larger font style
    static lv_style_t time_style;
    lv_style_init(&time_style);
    lv_style_set_text_font(&time_style, &lv_font_montserrat_18);  // Bigger font

    //  Create elapsed time label
    elapsed_label = lv_label_create(time_header);
    lv_label_set_text(elapsed_label, "00:00");
    lv_obj_add_style(elapsed_label, &time_style, LV_PART_MAIN);
    lv_obj_align(elapsed_label, LV_ALIGN_LEFT_MID, 10, 0);

    //  Create real-time label
    datetime_label = lv_label_create(time_header);
    lv_label_set_text(datetime_label, "00:00 | January 1, 1970");
    lv_obj_add_style(datetime_label, &time_style, LV_PART_MAIN);
    lv_obj_align(datetime_label, LV_ALIGN_RIGHT_MID, -10, 0);

    // ============================
    // Battery group inside header
    // ============================
    init_battery_icon_image();

    batteryGroup = lv_obj_create(time_header);
    lv_obj_set_size(batteryGroup, 100, 40);
    lv_obj_set_style_bg_color(batteryGroup, lv_color_hex(0xFF0000), LV_PART_MAIN);  // Red
    lv_obj_clear_flag(batteryGroup, LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_set_style_border_width(batteryGroup, 0, LV_PART_MAIN);
    lv_obj_set_style_pad_all(batteryGroup, 0, LV_PART_MAIN);
    lv_obj_set_style_bg_opa(batteryGroup, 0, LV_PART_MAIN);
    lv_obj_align(batteryGroup, LV_ALIGN_CENTER, 0, 0);  // Adjust position

    // Battery icon
    lv_obj_t *ui_BatteryPic = lv_img_create(batteryGroup);
    lv_img_set_src(ui_BatteryPic, &ui_img_batteryicon_png);
    lv_obj_set_width(ui_BatteryPic, LV_SIZE_CONTENT);
    lv_obj_set_height(ui_BatteryPic, LV_SIZE_CONTENT);
    lv_obj_align(ui_BatteryPic, LV_ALIGN_CENTER, 9, -3);
    lv_obj_add_flag(ui_BatteryPic, LV_OBJ_FLAG_ADV_HITTEST);
    lv_obj_clear_flag(ui_BatteryPic, LV_OBJ_FLAG_SCROLLABLE);
    lv_img_set_zoom(ui_BatteryPic, 25);

    // Panels
    auto make_panel = [&](int w, int h, int x, int y) {
        lv_obj_t *p = lv_obj_create(batteryGroup);
        lv_obj_set_size(p, w, h);
        lv_obj_align(p, LV_ALIGN_CENTER, x, y);
        lv_obj_clear_flag(p, LV_OBJ_FLAG_SCROLLABLE);
        lv_obj_set_style_bg_color(p, lv_color_hex(0x101418), LV_PART_MAIN);
        lv_obj_set_style_border_color(p, lv_color_hex(0x101418), LV_PART_MAIN);
        lv_obj_set_style_border_opa(p, 255, LV_PART_MAIN);
        lv_obj_set_style_bg_opa(p, 255, LV_PART_MAIN);
        return p;
    };

    make_panel(25, 23, -16, -17);   // Panel2
    make_panel(8, 21, 26, -15);  // Panel1
    make_panel(26, 21, -13, 6);     // Panel3
    make_panel(9, 18, 23, 10);   // Panel4
    make_panel(28, 9, 10, -19);   // Panel5
    make_panel(24, 4, 10, 14);    // Panel6

    // ============================
    // Bike image on top (last)
    // ============================
    init_bike_icon_image();
    ui_BikePic = lv_img_create(time_header);
    lv_img_set_src(ui_BikePic, &ui_img_bikeicon_png);
    lv_obj_set_width(ui_BikePic, LV_SIZE_CONTENT);
    lv_obj_set_height(ui_BikePic, LV_SIZE_CONTENT);
    lv_obj_align(ui_BikePic, LV_ALIGN_CENTER, 9, 0);  // Adjust as needed
    lv_obj_add_flag(ui_BikePic, LV_OBJ_FLAG_ADV_HITTEST);
    lv_obj_clear_flag(ui_BikePic, LV_OBJ_FLAG_SCROLLABLE);
    lv_img_set_zoom(ui_BikePic, 40);  // Optional: adjust size

    lv_obj_clear_flag(batteryGroup, LV_OBJ_FLAG_HIDDEN); // Show group

}
