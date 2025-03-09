#include "main.hpp"

TFT_eSPI tftDisplay = TFT_eSPI(); // TFT Instance
float voltage, current;
QueueHandle_t uartQueue;
TaskHandle_t buttonTaskHandle;
TaskHandle_t displayTaskHandle;

void my_disp_flush(lv_disp_drv_t *disp, const lv_area_t *area, lv_color_t *color_p)
{
  uint16_t c;

  tftDisplay.startWrite();
  tftDisplay.setAddrWindow(area->x1, area->y1, (area->x2 - area->x1 + 1), (area->y2 - area->y1 + 1));

  for (int y = area->y1; y <= area->y2; y++)
  {
    for (int x = area->x1; x <= area->x2; x++)
    {
      c = color_p->full; // Convert color
      tftDisplay.pushColor(c);
      color_p++;
    }
  }
  tftDisplay.endWrite();

  lv_disp_flush_ready(disp); // Tell LVGL that flushing is done
}

void setup()
{
  initArduino();
  lv_init();

  // Serial2.begin(115200,SERIAL_8N1,ESP32_RX_PIN,ESP32_TX_PIN);
  Serial.begin(115200);
  pinMode(TFT_SCREEN_LED, OUTPUT);
  digitalWrite(TFT_SCREEN_LED, HIGH);

  uartQueue = xQueueCreate(QUEUE_LENGTH, sizeof(uart_event_t));
  if (uartQueue == NULL)
  {
    Serial.println("Failed to create queue");
  }

  // WiFiSetup();
  // initSDLog(); TODO: Make sure you configure HSPI, VSPI in menuconfig

  buttonSetup();

  tftDisplay.begin();
  tftDisplay.setRotation(3);

  setupUART2();

  // Initialize display buffer and driver
  static lv_disp_draw_buf_t draw_buf;
  static lv_color_t buf[LV_HOR_RES_MAX * 10]; // Buffer for 10 lines
  lv_disp_draw_buf_init(&draw_buf, buf, NULL, LV_HOR_RES_MAX * 10);

  static lv_disp_drv_t disp_drv;     // Display driver
  lv_disp_drv_init(&disp_drv);       // Basic initialization
  disp_drv.hor_res = 480;            // Set horizontal resolution
  disp_drv.ver_res = 320;            // Set vertical resolution
  disp_drv.flush_cb = my_disp_flush; // Set flush callback
  disp_drv.draw_buf = &draw_buf;     // Assign the buffer to the driver
  lv_disp_drv_register(&disp_drv);   // Register the driver

  ui_init();
  updateScreen1();
  Serial.println("Initialzed Screen");

  xTaskCreate(uart_event_task, "uart_event_task", 4096, NULL, 10, NULL);
  xTaskCreate(displayTask, "display_task", 4096, NULL, 12, &displayTaskHandle);
  xTaskCreate(buttonTask, "ButtonTask", 4096, NULL, 11, &buttonTaskHandle);
}

extern "C" void app_main()
{
  setup();
}