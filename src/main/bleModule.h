/*
  Bluetooth.h
  This file encapsulates the Bluetooth Low Energy module functionalities for the project.
*/

#pragma once

#include <ArduinoBLE.h>
#include "globalVar.h"  // Include global variables and configurations

class BluetoothBLE {
public:
    BluetoothBLE() {}

    void setup() {
        if (!BLE.begin()) {
            Serial.println("Starting BLE module failed!");
            while (1);
        }

        // Set device name and advertised services
        BLE.setLocalName("Sportshield 8");
        BLE.setDeviceName("portshield 8");
        BLE.setAdvertisedService(PasswordService);

        // Add characteristics to services
        PasswordService.addCharacteristic(PasswordCharacteristic);
        ConfigService.addCharacteristic(NameCharacteristic);
        ConfigService.addCharacteristic(ActivationCharacteristic);
        ConfigService.addCharacteristic(UnlockCharacteristic);
        ConfigService.addCharacteristic(MACCharacteristic);

        // Add descriptors to characteristics
        addDescriptors();

        // Add services to BLE
        BLE.addService(PasswordService);
        BLE.addService(ConfigService);

        // Initialize characteristics
        initCharacteristics();

        // Set event handlers
        setEventHandlers();

        // Start advertising
        BLE.advertise();
        Serial.println("BLE setup complete");
    }

    void poll() {
        // Call BLE.poll() to process BLE events
        BLE.poll();
    }

private:
    void addDescriptors() {
        PasswordCharacteristic.addDescriptor(PasswordDescriptor);
        NameCharacteristic.addDescriptor(NameDescriptor);
        ActivationCharacteristic.addDescriptor(ActivationDescriptor);
        UnlockCharacteristic.addDescriptor(UnlockDescriptor);
        MACCharacteristic.addDescriptor(MACDescriptor);
    }

    void initCharacteristics() {
        PasswordCharacteristic.writeValue("0");
        NameCharacteristic.writeValue("\n");
        ActivationCharacteristic.writeValue(false);
        UnlockCharacteristic.writeValue(false);
        MACCharacteristic.writeValue(BLE.address());
    }

    void setEventHandlers() {
        BLE.setEventHandler(BLEConnected, onConnect);
        BLE.setEventHandler(BLEDisconnected, onDisconnect);
        PasswordCharacteristic.setEventHandler(BLEWritten, onWritePassword);
        NameCharacteristic.setEventHandler(BLEWritten, onWriteName);
        NameCharacteristic.setEventHandler(BLERead, onReadName);
        ActivationCharacteristic.setEventHandler(BLEWritten, onWriteActivation);
        ActivationCharacteristic.setEventHandler(BLERead, onReadActivation);
        UnlockCharacteristic.setEventHandler(BLEWritten, onWriteUnlock);
    }

    // Event handler implementations remain the same as provided in your code.
};
