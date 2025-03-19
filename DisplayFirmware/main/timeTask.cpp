#include "timeTask.hpp"

static lv_obj_t *time_header;
static lv_obj_t *elapsed_label;
static lv_obj_t *datetime_label;
static int workout_seconds = 0; // Tracks elapsed workout time
static time_t current_time; // Stores system time

// Function to create the persistent time header
static void create_time_header() {
    time_header = lv_obj_create(lv_layer_top());
    lv_obj_set_size(time_header, LV_HOR_RES, 40);
    
    // Remove outline, border, and scrolling
    lv_obj_clear_flag(time_header, LV_OBJ_FLAG_SCROLLABLE); 
    lv_obj_set_style_border_width(time_header, 0, LV_PART_MAIN);
    lv_obj_set_style_pad_all(time_header, 0, LV_PART_MAIN);
    lv_obj_set_style_radius(time_header, 0, LV_PART_MAIN);
    
    lv_obj_set_style_bg_color(time_header, lv_color_hex(0x101418), LV_PART_MAIN);
    lv_obj_set_style_text_color(time_header, lv_color_hex(0xFFFFFF), LV_PART_MAIN);

    // Create a larger font style
    static lv_style_t time_style;
    lv_style_init(&time_style);
    lv_style_set_text_font(&time_style, &lv_font_montserrat_18); 

    // Workout elapsed time label
    elapsed_label = lv_label_create(time_header);
    lv_label_set_text(elapsed_label, "00:00");
    lv_obj_add_style(elapsed_label, &time_style, LV_PART_MAIN); // Apply larger font
    lv_obj_align(elapsed_label, LV_ALIGN_LEFT_MID, 10, 0);

    // Actual time and date label
    datetime_label = lv_label_create(time_header);
    lv_label_set_text(datetime_label, "00:00 01/01/1970");
    lv_obj_add_style(datetime_label, &time_style, LV_PART_MAIN); // Apply larger font
    lv_obj_align(datetime_label, LV_ALIGN_RIGHT_MID, -10, 0);
}

 

// Function to sync time with an NTP server (Runs every hour)
void sync_ntp_time() {
    sntp_setoperatingmode(SNTP_OPMODE_POLL);
    sntp_setservername(0, "pool.ntp.org");
    sntp_init();

    time_t now = 0;
    struct tm timeinfo = {0};
    int retry = 0;
    const int max_retries = 10;

    while (timeinfo.tm_year < (2020 - 1900) && ++retry < max_retries) {
        vTaskDelay(pdMS_TO_TICKS(2000));
        time(&now);
        localtime_r(&now, &timeinfo);
    }

    if (retry == max_retries) {
    } else {
        current_time = now; // Update stored time
    }
}

// Task to update time locally every second and sync NTP every hour
// TODO: Implement NTP sync every hour
// TODO: Make sure screen turns off after 15 seconds of inactivity
void time_task(void *pvParameter) {
    sync_ntp_time();
    uint32_t last_update = xTaskGetTickCount();

    while (1) {
        uint32_t now_ticks = xTaskGetTickCount();
        
        if (pdTICKS_TO_MS(now_ticks - last_update) >= 1000) {
            last_update = now_ticks;
            workout_seconds++;
            current_time++; 

            // Update workout elapsed time
            char elapsed_time_str[16];
            snprintf(elapsed_time_str, sizeof(elapsed_time_str), "%02d:%02d", workout_seconds / 60, workout_seconds % 60);
            lv_label_set_text(elapsed_label, elapsed_time_str);

            // Update actual time display
            struct tm timeinfo;
            localtime_r(&current_time, &timeinfo);

            const char *months[] = {"January", "February", "March", "April", "May", "June", 
                                    "July", "August", "September", "October", "November", "December"};
            const char *monthStr = months[timeinfo.tm_mon];  // tm_mon is 0-based (0 = January)

            char datetime_str[32];
            snprintf(datetime_str, sizeof(datetime_str), "%s %d, %d", monthStr, timeinfo.tm_mday, timeinfo.tm_year + 1900);
            lv_label_set_text(datetime_label, datetime_str);

            xTaskNotify(displayTaskHandle, 1 << 2, eSetBits);
        }

        // if (workout_seconds % 3600 == 0) {
        //     sync_ntp_time();
        // }

        vTaskDelay(pdMS_TO_TICKS(500));
    }
}

// Function to start the time task
void start_time_task() {
    create_time_header();
    xTaskCreatePinnedToCore(time_task, "TimeTask", 4096, NULL, 13, NULL, 0);
}
