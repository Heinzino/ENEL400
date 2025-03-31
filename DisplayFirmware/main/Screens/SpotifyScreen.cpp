#include "SpotifyScreen.hpp"

const char *spotifyBaseApiUrl = "https://api.spotify.com/v1";

constexpr uint32_t C_SPOTIFY_GREEN = 0x1DB954;
constexpr uint32_t C_SPOTIFY_BLACK = 0x000000;
constexpr uint32_t C_WHITE = 0xFFFFFF;


void SpotifyScreen::updateScreen()
{
    if (pendingSwitchToHomeScreen)
    {
        pendingSwitchToHomeScreen = false;
        ScreenManager::getInstance().safeSwitchToScreen(ScreenTitles::POWER_DISPLAY, ui_Screen1);
        return;
    }

    TrackInfo trackInfo = apiClient.getCurrentTrackInfo();

    lv_label_set_text(ui_LabelTrack, trackInfo.name.c_str());
    lv_label_set_text(ui_LabelArtist, trackInfo.artistName.c_str());

    currentTrackDuration = trackInfo.duration;
    currentTrackProgress = trackInfo.progress;
    if (currentTrackDuration != 0) {
      lv_bar_set_value(ui_ProgressBar, (currentTrackProgress * 100) / currentTrackDuration, LV_ANIM_OFF);
    }

    int currentTimeSec = currentTrackProgress / 1000;
    int totalTimeSec = currentTrackDuration / 1000;
    char currentTimeStr[16];
    char totalTimeStr[16];
    snprintf(currentTimeStr, sizeof(currentTimeStr), "%02d:%02d", currentTimeSec / 60, currentTimeSec % 60);
    snprintf(totalTimeStr, sizeof(totalTimeStr), "%02d:%02d", totalTimeSec / 60, totalTimeSec % 60);
    lv_label_set_text(ui_LabelTimeCurrent, currentTimeStr);
    lv_label_set_text(ui_LabelTimeTotal, totalTimeStr);


    lv_task_handler();
    lv_timer_handler();
    safeLvglRefresh();
}

void SpotifyScreen::handleButton(ButtonID btn)
{

    switch (btn)
    {
    case ButtonID::SHIFT_HRZN_BTN:
        pendingSwitchToHomeScreen = true;
        break;
    default:
        break;
    }
}
