#include "SpotifyScreen.hpp"
#include "spotify_api.h"

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
