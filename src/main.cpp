/**
 * Example: BLE Fitness Machine Service (Indoor Bike Data) for ESP32
 *
 * This sample creates:
 * - A BLE device named "MyWoosh-ESP32"
 * - A Fitness Machine Service (0x1826)
 * - An Indoor Bike Data characteristic (0x2AD2)
 * 
 * It then sends notifications containing mock speed/cadence/power 
 * values every few seconds.
 * 
 * Tested with the Arduino-ESP32 core. Adjust as needed.
 * Specifications : https://www.bluetooth.com/specifications/specs/fitness-machine-service-1-0/ 
 */

#include <Arduino.h>
#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>
#include <BLE2902.h>

// Use official GATT UUIDs for FTMS:
static const BLEUUID SERVICE_FTMS_UUID((uint16_t)0x1826);     // Fitness Machine Service
static const BLEUUID CHAR_BIKE_DATA_UUID((uint16_t)0x2AD2);   // Indoor Bike Data

// Pointers to our BLE objects
BLEServer* pServer = nullptr;
BLEService* pFTMSService = nullptr;
BLECharacteristic* pIndoorBikeDataChar = nullptr;

// Flag to keep track of connected status
bool deviceConnected = false;

/**
 * BLE Server Callbacks: adjust or extend as needed.
 */
class MyServerCallbacks : public BLEServerCallbacks {
  void onConnect(BLEServer* pServer) {
    deviceConnected = true;
    Serial.println("BLE Client Connected!");
  }

  void onDisconnect(BLEServer* pServer) {
    deviceConnected = false;
    Serial.println("BLE Client Disconnected!");
    // Restart advertising so other devices (like MyWoosh) can find it again
    pServer->startAdvertising();
  }
};

/**
 * Pack and send indoor bike data to the characteristic.
 * This is a minimal example: we construct a data packet with 
 * flags, instantaneous speed, instantaneous cadence, and power.
 *
 * The Indoor Bike Data characteristic (0x2AD2) requires a specific format:
 *    - 2 bytes: Flags
 *    - (Optional) 2 bytes for Instantaneous Speed (in 0.01 kph)
 *    - (Optional) 2 bytes for Instantaneous Cadence (in 0.5 rpm)
 *    - (Optional) 2 bytes for Instantaneous Power (in Watts)
 *    - ... [more fields if flags indicate they are present]
 */
void sendIndoorBikeData(uint16_t speed, uint16_t cadence, int16_t power) {
  // Example flags to indicate presence of speed, cadence, and power:
  // Bit definitions (from BT spec; partial listing):
  //   0 - More Data (not used here)
  //   1 - Average Speed Present
  //   2 - Instantaneous Cadence Present
  //   3 - Average Cadence Present
  //   4 - Total Distance Present
  //   5 - Resistance Level Present
  //   6 - Instantaneous Power Present
  //   ...
  // For this example: speed (bit 1), cadence (bit 2), power (bit 6)
  uint16_t flags = 0x0000;
  flags |= (1 << 1); // Speed present
  flags |= (1 << 2); // Cadence present
  flags |= (1 << 6); // Power present

  // We will build the raw byte array in this order:
  // [Flags(2 bytes)] [Speed(2 bytes, 0.01 kph)] [Cadence(2 bytes, 0.5 rpm)] [Power(2 bytes)]
  // Speed is in 0.01 kph, Cadence is in 0.5 rpm, Power in Watts (int16)
  // e.g. speed=1234 => 12.34 kph; cadence=60 => 30 rpm; power=150 => 150W

  // Prepare a buffer. Make it large enough for the fields we set.
  uint8_t buffer[8];
  memset(buffer, 0, sizeof(buffer));

  // Flags
  buffer[0] = (uint8_t)(flags & 0xFF);
  buffer[1] = (uint8_t)((flags >> 8) & 0xFF);

  // Instantaneous Speed (2 bytes, little endian)
  buffer[2] = (uint8_t)(speed & 0xFF);
  buffer[3] = (uint8_t)((speed >> 8) & 0xFF);

  // Instantaneous Cadence (2 bytes, little endian)
  buffer[4] = (uint8_t)(cadence & 0xFF);
  buffer[5] = (uint8_t)((cadence >> 8) & 0xFF);

  // Instantaneous Power (2 bytes, little endian)
  buffer[6] = (uint8_t)(power & 0xFF);
  buffer[7] = (uint8_t)((power >> 8) & 0xFF);

  // Push this data to the characteristic and notify the connected client.
  pIndoorBikeDataChar->setValue(buffer, sizeof(buffer));
  pIndoorBikeDataChar->notify(); 
  // (If deviceConnected is false, calling notify() will simply have no effect 
  // on the client side, but it’s safe to call.)
}

void setup() {
  Serial.begin(115200);
  Serial.println("Starting BLE Workout Device...");

  // Initialize BLE and set the device name.
  BLEDevice::init("ReCycle");

  // Create the BLE Server
  pServer = BLEDevice::createServer();
  pServer->setCallbacks(new MyServerCallbacks());

  // Create the Fitness Machine Service
  pFTMSService = pServer->createService(SERVICE_FTMS_UUID);

  // Create the Indoor Bike Data Characteristic (Notify)
  pIndoorBikeDataChar = pFTMSService->createCharacteristic(
    CHAR_BIKE_DATA_UUID,
    BLECharacteristic::PROPERTY_NOTIFY
  );

  // Required descriptor for notifications in many clients
  pIndoorBikeDataChar->addDescriptor(new BLE2902());

  // Start the service
  pFTMSService->start();

  // Start advertising
  BLEAdvertising* pAdvertising = BLEDevice::getAdvertising();
  pAdvertising->addServiceUUID(SERVICE_FTMS_UUID);
  pAdvertising->setScanResponse(true);
  pServer->getAdvertising()->start();

  Serial.println("BLE Advertising with FTMS Service started.");
}

void loop() {
  // Example of sending mock data every 2 seconds
  static unsigned long lastUpdate = 0;
  unsigned long now = millis();

  if(deviceConnected && (now - lastUpdate > 2000)) {
    lastUpdate = now;

    // Generate some arbitrary data
    // Speed in 0.01 kph, for example: 1234 -> 12.34 kph
    uint16_t speed = 1234;  
    // Cadence in 0.5 rpm steps, e.g. 60 -> 30 RPM
    uint16_t cadence = 60;   
    // Power in watts
    int16_t power = 150;    

    sendIndoorBikeData(speed, cadence, power);

    Serial.println("Indoor Bike Data notified: speed=12.34kph, cadence=30rpm, power=150W");
  }

  // Do other tasks as needed
}