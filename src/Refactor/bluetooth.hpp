#pragma once
#include <ArduinoBLE.h>

class Bluetooth {
private:
    static Bluetooth* instance;
    BLEService lockService;
    BLECharacteristic lockCharacteristic;

    Bluetooth() : lockService("1234-5678-9012-3456", BLERead | BLEWrite),
                  lockCharacteristic("ABCD-5678-9012-EFGH", BLERead | BLEWrite, sizeof(int)) {
        // Constructor is private for singleton implementation
    }

    // Static method for handling BLE device events
    static void bleDeviceEvent(BLEDevice central, BLECharacteristic characteristic) {
        // Example: Handle lock/unlock characteristic changes
        if (characteristic.uuid() == lockCharacteristic.uuid()) {
            int lockState = characteristic.value();
            if (lockState == 1) {
                // Handle locking logic
            } else if (lockState == 0) {
                // Handle unlocking logic
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
            // Can extend to handle multiple events
        }
    }
};

Bluetooth* Bluetooth::instance = nullptr;
