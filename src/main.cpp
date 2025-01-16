// Tutorial https://www.youtube.com/watch?v=ssokvToiYU0 
#include <Arduino.h>
#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEServer.h>

#define SERVICE_UUID        "12345678-1234-1234-1234-1234567890ab" // A random UUID for your service
#define CHARACTERISTIC_UUID "87654321-4321-4321-4321-ba0987654321" // A random UUID for your characteristic

BLECharacteristic *pCharacteristic;

// Optional callback if you want to observe central connections
class MyServerCallbacks: public BLEServerCallbacks {
  void onConnect(BLEServer* pServer) {
    Serial.println("Client connected");
  }
  void onDisconnect(BLEServer* pServer) {
    Serial.println("Client disconnected");
    // By default, the ESP32 stops advertising after a client disconnects
    pServer->getAdvertising()->start();
  }
};

void setup() {
  Serial.begin(115200);

  // 1) Initialize BLE
  BLEDevice::init("ESP32_Power_Sensor");  // This is the device name that will appear in iOS scanning

  // 2) Create a BLE Server
  BLEServer *pServer = BLEDevice::createServer();
  pServer->setCallbacks(new MyServerCallbacks()); //Optional callback
 
  // 3) Create the BLE Service
  BLEService *pService = pServer->createService(SERVICE_UUID);

  // 4) Create a BLE Characteristic
  pCharacteristic = pService->createCharacteristic(
                      CHARACTERISTIC_UUID,
                      BLECharacteristic::PROPERTY_READ   |
                      BLECharacteristic::PROPERTY_NOTIFY 
                    );

  // Optionally, set descriptors like the Client Characteristic Configuration (CCC) if needed

  // 5) Start the service
  pService->start();

  // 6) Start advertising
  BLEAdvertising *pAdvertising = BLEDevice::getAdvertising();
  pAdvertising->addServiceUUID(SERVICE_UUID);
  pAdvertising->setScanResponse(true);
  pAdvertising->setMinPreferred(0x06);  // functions that help with iPhone connections
  pAdvertising->setMinPreferred(0x12);
  pServer->getAdvertising()->start();
  Serial.println("BLE advertising started...");
}

void loop() {
  // Generate a random power reading between 0 and 300 W (for example)
  int powerValue = random(0, 300);

  // Convert it to a string or raw bytes
  char powerStr[8];
  sprintf(powerStr, "%d", powerValue);

  // Set the characteristic value
  pCharacteristic->setValue(powerStr);
  
  // Notify if there are any connected clients
  pCharacteristic->notify();

  // Wait a bit before sending the next reading
  delay(2000);
}
