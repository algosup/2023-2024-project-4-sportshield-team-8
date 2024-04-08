#pragma once
#include <ArduinoBLE.h>

class Bluetooth {
private:
    static Bluetooth* instance;
    BLEService lockService;
    BLECharacteristic lockCharacteristic;

    Bluetooth() : lockService("19B10000-E8F2-537E-4F6C-D104768A1213", BLERead | BLEWrite),
                  lockCharacteristic("19B10000-E8F2-537E-4F6C-D104768A1213", BLERead | BLEWrite, sizeof(int)) {
    }

    // Static method for handling BLE device events
    static void bleDeviceEvent(BLEDevice central, BLECharacteristic characteristic) {
        if (characteristic.uuid() == lockCharacteristic.uuid()) {
            int lockState = characteristic.value();
            if (lockState == 1) {
                //  locking logic
            } else if (lockState == 0) {
                //  unlocking logic
            }
        }
    }

public:
    static Bluetooth* getInstance() {
        if (!instance) {
            instance = new Bluetooth();
        }
        return instance;
    }

    void begin() {
        BLE.begin();
        BLE.setLocalName("SportShield");
        BLE.setAdvertisedService(lockService);
        lockService.addCharacteristic(lockCharacteristic);
        BLE.addService(lockService);
        lockCharacteristic.setEventHandler(BLEWritten, bleDeviceEvent);
        BLE.advertise();
    }

    void processEvents() {
        BLEDevice central = BLE.central();
        if (central) {
            // A central has connected
            Serial.print("Connected to central: ");
            Serial.println(central.address());
        }
    }
};

Bluetooth* Bluetooth::instance = nullptr;
