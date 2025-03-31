#include "UI/ui.h"

lv_obj_t *ui_SpotifyScreen;
lv_obj_t *ui_LabelTrack;
lv_obj_t *ui_LabelArtist;
lv_obj_t *ui_ImageAlbumCover;
lv_obj_t *ui_ProgressBar;
lv_obj_t *ui_LabelTimeCurrent;
lv_obj_t *ui_LabelTimeTotal;
lv_obj_t *ui_ButtonPlayPause;

void ui_SpotifyScreen_screen_init(void)
{
    ui_SpotifyScreen = lv_obj_create(NULL);
    lv_obj_clear_flag(ui_SpotifyScreen, LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_set_size(ui_SpotifyScreen, 480, 320); // Ensure full screen if needed
    lv_obj_center(ui_SpotifyScreen);

    // === Album Art Centered Top ===
    ui_ImageAlbumCover = lv_img_create(ui_SpotifyScreen);
    lv_obj_set_size(ui_ImageAlbumCover, 64, 64);
    lv_img_set_zoom(ui_ImageAlbumCover, 510); // Set zoom level to 256 (4x)
    lv_obj_align(ui_ImageAlbumCover, LV_ALIGN_TOP_MID, 0, 85); // Push it down a bit

    // === Text container (title + artist) ===
    lv_obj_t *textContainer = lv_obj_create(ui_SpotifyScreen);
    lv_obj_remove_style_all(textContainer);
    lv_obj_set_size(textContainer, 400, 80);
    lv_obj_align_to(textContainer, ui_ImageAlbumCover, LV_ALIGN_OUT_BOTTOM_MID, 0, 30);
    lv_obj_set_flex_flow(textContainer, LV_FLEX_FLOW_COLUMN);
    lv_obj_set_flex_align(textContainer, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);

    ui_LabelTrack = lv_label_create(textContainer);
    lv_label_set_text(ui_LabelTrack, "Track");
    lv_obj_set_style_text_font(ui_LabelTrack, &lv_font_montserrat_28, 0);
    lv_label_set_long_mode(ui_LabelTrack, LV_LABEL_LONG_SCROLL_CIRCULAR);

    ui_LabelArtist = lv_label_create(textContainer);
    lv_label_set_text(ui_LabelArtist, "Artist");
    lv_obj_set_style_text_font(ui_LabelArtist, &lv_font_montserrat_18, 0);

    // === Progress Bar ===
    ui_ProgressBar = lv_bar_create(ui_SpotifyScreen);
    lv_obj_set_size(ui_ProgressBar, 440, 15);
    lv_obj_align(ui_ProgressBar, LV_ALIGN_BOTTOM_MID, 0, -50);

    // === Time Container ===
    lv_obj_t *timeContainer = lv_obj_create(ui_SpotifyScreen);
    lv_obj_remove_style_all(timeContainer);
    lv_obj_set_size(timeContainer, 440, 30);
    lv_obj_align_to(timeContainer, ui_ProgressBar, LV_ALIGN_OUT_TOP_MID, 0, -5);
    lv_obj_set_flex_flow(timeContainer, LV_FLEX_FLOW_ROW);
    lv_obj_set_flex_align(timeContainer, LV_FLEX_ALIGN_SPACE_BETWEEN, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);

    ui_LabelTimeCurrent = lv_label_create(timeContainer);
    lv_label_set_text(ui_LabelTimeCurrent, "00:00");

    ui_LabelTimeTotal = lv_label_create(timeContainer);
    lv_label_set_text(ui_LabelTimeTotal, "00:00");

    // === Play Button ===
    ui_ButtonPlayPause = lv_btn_create(ui_SpotifyScreen);
    lv_obj_set_size(ui_ButtonPlayPause, 50, 40);
    lv_obj_align_to(ui_ButtonPlayPause, ui_ProgressBar, LV_ALIGN_OUT_BOTTOM_MID, 0, 5);
    lv_obj_t *icon = lv_label_create(ui_ButtonPlayPause);
    lv_label_set_text(icon, LV_SYMBOL_PLAY);
    lv_obj_center(icon);
}
