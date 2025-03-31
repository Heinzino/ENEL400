#pragma once

#include "Screen.hpp"
#include "screenEnums.hpp"
#include "screenManager.hpp"
#include "secrets.hpp"
#include "spotify_api.h"
#include <lvgl.h>

extern SpotifyApi apiClient; // Spotify API client instance

class SpotifyScreen : public Screen
{
public:
    void updateScreen() override;
    void handleButton(ButtonID btn) override;
    lv_obj_t *getScreen() override { return ui_Screen1; }

private:
    // Variables to store current track information
    int currentTrackDuration = 0;
    int currentTrackProgress = 0;
    bool isPlaying = false;
    int httpErrorCount = 0;
    const int maxErrorCount = 3;
    String currentTrackImageUrl = "";
    unsigned long lastActivityTime = millis();

    
    bool pendingSwitchToHomeScreen = false;
};
