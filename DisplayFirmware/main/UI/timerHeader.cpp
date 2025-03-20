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
}
