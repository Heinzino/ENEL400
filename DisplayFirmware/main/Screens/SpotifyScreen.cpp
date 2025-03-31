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
