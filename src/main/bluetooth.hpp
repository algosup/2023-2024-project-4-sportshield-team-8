#pragma once
#include <ArduinoBLE.h>

class Bluetooth {
private:
    static Bluetooth* instance;
    BLEService lockService;
    BLECharacteristic lockCharacteristic;

    // Private constructor for the singleton pattern
        Bluetooth() : lockService("19B10000-E8F2-537E-4F6C-D104768A1214", BLERead | BLEWrite),
                  lockCharacteristic("19B10001-E8F2-537E-4F6C-D104768A1214", BLERead | BLEWrite, sizeof(int)) {
        if (!BLE.begin()) {
            Serial.println("Starting BLE failed!");
            while (1);
        }
        // set advertised local name and service UUID:
        BLE.setLocalName("SportShield");
        BLE.setDeviceName("SportShield");
        BLE.setAdvertisedService(PasswordService);
        // add descriptors
        PasswordCharacteristic.addDescriptor(PasswordDescriptor);
        NameCharacteristic.addDescriptor(NameDescriptor);
        ActivationCharacteristic.addDescriptor(ActivationDescriptor);
        UnlockCharacteristic.addDescriptor(UnlockDescriptor);
        MACCharacteristic.addDescriptor(MACDescriptor);
        // add the characteristic to the service
        PasswordService.addCharacteristic(PasswordCharacteristic);
        ConfigService.addCharacteristic(NameCharacteristic);
        ConfigService.addCharacteristic(ActivationCharacteristic);
        ConfigService.addCharacteristic(UnlockCharacteristic);
        ConfigService.addCharacteristic(MACCharacteristic);
        // add service
        BLE.addService(PasswordService);
        BLE.addService(ConfigService);
        // set the initial value for the characeristic:
        PasswordCharacteristic.writeValue(0);
        NameCharacteristic.writeValue("\n");
        ActivationCharacteristic.writeValue(false);
        UnlockCharacteristic.writeValue(false);
        MACCharacteristic.writeValue(BLE.address());
        //set event handler
        BLE.setEventHandler(BLEConnected, onConnect);
        BLE.setEventHandler(BLEDisconnected, onDisconnect);
        PasswordCharacteristic.setEventHandler(BLEWritten, onWritePassword);
        NameCharacteristic.setEventHandler(BLEWritten, onWriteName);
        NameCharacteristic.setEventHandler(BLERead, onReadName);
        ActivationCharacteristic.setEventHandler(BLEWritten, );
        ActivationCharacteristic.setEventHandler(BLERead, onReadActivation);
        UnlockCharacteristic.setEventHandler(BLEWritten, onWriteUnlock);
        // start advertising
        BLE.advertise();
    }
    // Actual method for handling BLE device events, non-static
    void handleBLEDeviceEvent(BLEDevice central, BLECharacteristic characteristic) {
        if (characteristic.uuid() == lockCharacteristic.uuid()) {
            int lockState = characteristic.value();
            if (lockState == 1) {
                Serial.println("Lock");
            } else if (lockState == 0) {
                Serial.println("Unlock");
            }
        }
    }

    void configureSecurity() {
        BLE.setEncryptionLevel(LEVEL_2);
        BLE.enableMITMProtection(true);
    }

public:
    Bluetooth(const Bluetooth&) = delete;
    Bluetooth& operator=(const Bluetooth&) = delete;

    // Static method to get the singleton instance
    static Bluetooth* getInstance() {
        if (!instance) {
            instance = new Bluetooth();
        }
        return instance;
    }


    void handleEvents() {
        BLE.poll();
    }
};

// Initialize the static member variable
Bluetooth* Bluetooth::instance = nullptr;
