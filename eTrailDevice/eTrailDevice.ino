
#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEServer.h>
#include "scale.h"
#include "motor.h"

#define ETRAIL_SERVICE_UUID               "2277a1fd-eb40-474b-8e05-87b5c721aec0"
#define CHARACTERISTIC_SCALE_UUID         "22770001-eb40-474b-8e05-22c7bb9543d3"
#define DESCRIPTOR_SCALE_UUID             "22770101-eb40-474b-8e05-87b5c721aec0"
#define CHARACTERISTIC_MOTOR_UUID         "22770002-eb40-474b-8e05-ea07361b26a8"
#define DESCRIPTOR_MOTOR_UUID             "22770102-eb40-474b-8e05-ea07361b26a8"
#define CHARACTERISTIC_CMD_UUID           "22770003-eb40-474b-8e05-22febb354386"


#define BATTERY_SERVICE_UUID              "180F"
#define CHARACTERISTIC_BATTERY_LEVEL_UUID "2A19"
class ServerCallbacks : public BLEServerCallbacks{
  void onConnect(BLEServer *s)  {
    Serial.println("client connected ...");
  }
  void onDisconnect(BLEServer *s){
    Serial.println("client disconnected ...");  
  }
};
class MyCmdCallbacks: public BLECharacteristicCallbacks {
    void onWrite(BLECharacteristic *pCharacteristic) {
      std::string value = pCharacteristic->getValue();

      if (value.length() > 0) {
        switch (value[0]){
          case 0: scale_tare();break;
        }
      }
    }
};

BLECharacteristic *pScaleCharacteristic;
BLECharacteristic *pMotorCharacteristic;
BLECharacteristic *pBattCharacteristic;
BLECharacteristic *pCmdCharacteristic;

void setup() {
  Serial.begin(115200);
  Serial.println("startup");
 // Serial.println("Starting BLE work!");
  
  scale_setup();
  motor_setup();
  
  BLEDevice::init("E-trail");
  BLEServer *pServer = BLEDevice::createServer();
  BLEService *pService = pServer->createService(ETRAIL_SERVICE_UUID);
  BLEService *pBattService = pServer->createService(BATTERY_SERVICE_UUID);
  
  pScaleCharacteristic = pService->createCharacteristic(
                         CHARACTERISTIC_SCALE_UUID,
                         BLECharacteristic::PROPERTY_READ|
                         BLECharacteristic::PROPERTY_NOTIFY);
  pMotorCharacteristic = pService->createCharacteristic(
                         CHARACTERISTIC_MOTOR_UUID,
                         BLECharacteristic::PROPERTY_READ|
                         BLECharacteristic::PROPERTY_NOTIFY);
  pCmdCharacteristic = pService->createCharacteristic(
                         CHARACTERISTIC_CMD_UUID,
                         BLECharacteristic::PROPERTY_WRITE);
  pBattCharacteristic =  pBattService->createCharacteristic(
                         CHARACTERISTIC_BATTERY_LEVEL_UUID,
                         BLECharacteristic::PROPERTY_READ);
  double s=scale_getValue();
               
  pScaleCharacteristic->setValue(s);
  int v=55;
  pBattCharacteristic->setValue(v);
  pService->start();
  pBattService->start();
  BLEAdvertising *pAdvertising = BLEDevice::getAdvertising();
  pAdvertising->addServiceUUID(ETRAIL_SERVICE_UUID);
  pAdvertising->setScanResponse(true);
  pAdvertising->setMinPreferred(0x06);  // functions that help with iPhone connections issue
  pAdvertising->setMinPreferred(0x12);
  pAdvertising->addServiceUUID(BATTERY_SERVICE_UUID);
  pAdvertising->setScanResponse(true);
  pAdvertising->setMinPreferred(0x06);  // functions that help with iPhone connections issue
  pAdvertising->setMinPreferred(0x12);
  pServer->setCallbacks(new ServerCallbacks());
  pCmdCharacteristic->setCallbacks(new MyCmdCallbacks());
  BLEDevice::startAdvertising();
  Serial.println("Characteristic defined! Now you can read it in your phone!");

}

void loop() {
  double s=scale_getValue();
  pScaleCharacteristic->setValue(s);
  pScaleCharacteristic->notify();
  uint8_t m=motor_getValue();
  pMotorCharacteristic->setValue(&m,1);
  pMotorCharacteristic->notify();
  scale_loop();
  motor_loop();
}
