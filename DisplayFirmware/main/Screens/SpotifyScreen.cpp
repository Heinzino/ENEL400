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

    if (trackInfo.coverUrl != currentTrackImageUrl)
    {
        currentTrackImageUrl = trackInfo.coverUrl;
        if (!fetchTrackImageTaskHandle)
        {
            char *urlCopy = strdup(currentTrackImageUrl.c_str());
            xTaskCreatePinnedToCore(fetchTrackImage, "FetchTrackImage", 5 * 1024, (void *)urlCopy, 13, &fetchTrackImageTaskHandle, 0);
        }
    }

    lv_label_set_text(ui_LabelTrack, trackInfo.name.c_str());
    lv_label_set_text(ui_LabelArtist, trackInfo.artistName.c_str());

    currentTrackDuration = trackInfo.duration;
    currentTrackProgress = trackInfo.progress;
    if (currentTrackDuration != 0)
    {
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

void fetchTrackImage(void *parameter)
{
    const char *imageUrl = (const char *)parameter;
    HTTPClient http;
    http.begin(imageUrl);
    int httpResponseCode = http.GET();

    if (httpResponseCode == HTTP_CODE_OK)
    {
        int jpegSize = http.getSize();
        WiFiClient *stream = http.getStreamPtr();
        uint8_t *jpegBuffer = (uint8_t *)malloc(jpegSize);

        if (jpegBuffer && stream->readBytes(jpegBuffer, jpegSize) == jpegSize)
        {
            // Allocate buffer for RGB565 output
            size_t rgbSize = JPEG_MAX_WIDTH * JPEG_MAX_HEIGHT * 2;
            uint8_t *rgbBuffer = (uint8_t *)heap_caps_malloc(rgbSize, MALLOC_CAP_SPIRAM | MALLOC_CAP_8BIT);

            if (rgbBuffer)
            {
                esp_jpeg_image_cfg_t cfg = {
                    .indata = jpegBuffer,
                    .indata_size = (uint32_t)jpegSize,
                    .outbuf = rgbBuffer,
                    .outbuf_size = rgbSize,
                    .out_format = JPEG_IMAGE_FORMAT_RGB565
                };

                esp_jpeg_image_output_t out;
                esp_err_t res = esp_jpeg_decode(&cfg, &out);

                if (res == ESP_OK)
                {
                    LOG(LOG_LEVEL_INFO, "Decoded JPEG: " + String(out.width) + "x" + String(out.height));

                    lv_img_dsc_t *img_dsc = (lv_img_dsc_t *)malloc(sizeof(lv_img_dsc_t));
                    if (img_dsc)
                    {
                        img_dsc->header.always_zero = 0;
                        img_dsc->header.w = out.width;
                        img_dsc->header.h = out.height;
                        img_dsc->header.cf = LV_IMG_CF_TRUE_COLOR;
                        img_dsc->data_size = out.width * out.height * 2;
                        img_dsc->data = rgbBuffer;

                        lv_img_set_src(ui_ImageAlbumCover, img_dsc);
                        // LVGL now owns img_dsc and rgbBuffer
                    }
                    else
                    {
                        LOG(LOG_LEVEL_ERROR, "Failed to allocate image descriptor");
                        free(rgbBuffer);
                    }
                }
                else
                {
                    LOG(LOG_LEVEL_ERROR, "JPEG decode failed with code: " + String(res));
                    free(rgbBuffer);
                }
            }
            else
            {
                LOG(LOG_LEVEL_ERROR, "Failed to allocate RGB buffer");
            }

            free(jpegBuffer);
        }
        else
        {
            LOG(LOG_LEVEL_ERROR, "Failed to download full JPEG stream");
            free(jpegBuffer);
        }
    }
    else
    {
        LOG(LOG_LEVEL_ERROR, "HTTP GET failed: " + String(httpResponseCode));
    }

    free((void *)imageUrl);
    http.end();
    fetchTrackImageTaskHandle = NULL;
    vTaskDelete(NULL);
}
