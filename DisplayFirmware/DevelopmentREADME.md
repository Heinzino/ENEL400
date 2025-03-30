# Setup

## 1. Set Device Target to ESP32 / ESP32S3
Perfboard / PCB
- There's a chip icon on the bottom task bar to change device target
- Build is the wrench on the bottom task bar
- Flash is the lightning bolt on the bottom task bar
- Make sure to select the proper COM PORT on the bottom task bar
- Fire icon in the bottom task bar builds, flashes and monitors all in one

## 2. Config Changes

### SdkConfig
- Change CONFIG_FREERTOS_HZ=1000 (not 100)

### Menu Config
Ctrl + Shift + P -> ESP-IDF: SDK Configuration Editor (MenuConfig)

List of Changes:
- Set TFT Driver to ILI9488
- Set TFT SPI Lines, CS, DC according to `pins.hpp`
- Enable Montserrat font sizes 48, 24, 28, 18, 40, 22, 32, 16, 44
- Partition table select single factory app (large), no OTA. From the dropdown menu