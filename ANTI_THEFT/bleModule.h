#pragma once

#include "global.h" // Include global definitions, configurations, and the BLE library.

/*
 * Class BluetoothManager
 * ----------------------
 * Overview:
 * The BluetoothManager class is responsible for managing all aspects of the Bluetooth Low Energy (BLE) functionality within the project.
 * It encapsulates the initialization, setup, and management of BLE services, characteristics, and event handling. This class aims to provide
 *
 * Features:
 * - Initializes the BLE module with predefined services and characteristics.
 * - Manages advertising settings and starts the BLE advertisement.
 * - Handles BLE events such as device connection, disconnection, and characteristic read/write operations.
 * - Implements security checks for authenticated operations.
 */

class BluetoothManager {
public:
    // Default constructor.
    BluetoothManager() {
        instance = this;
    }

    /*
     * Initializes the BLE module with predefined services and characteristics.
     * This method sets up the BLE device information, initializes services and characteristics,
     * and starts advertising. Must be called in the setup routine of the Arduino sketch.
     */
    void setup() {
        if (!BLE.begin()) {
            Serial.println("Starting Bluetooth® Low Energy module failed!");
            while (1); // Halt further execution if BLE cannot be initialized.
        }

        BLE.setLocalName("SPORTSHIELD_8"); // Set the local name displayed in BLE scans.
        BLE.setDeviceName("SPORTSHIELD_8"); // Set the device name.
        BLE.setAdvertisedService(PasswordService); // Advertise the password // add descriptors
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
        BLE.setEventHandler(BLEConnected, onConnectStatic);
        BLE.setEventHandler(BLEDisconnected, onDisconnectStatic);
        PasswordCharacteristic.setEventHandler(BLEWritten, onWritePasswordStatic);
        NameCharacteristic.setEventHandler(BLEWritten, onWriteNameStatic);
        NameCharacteristic.setEventHandler(BLERead, onReadNameStatic);
        ActivationCharacteristic.setEventHandler(BLEWritten, onWriteActivationStatic);
        ActivationCharacteristic.setEventHandler(BLERead, onReadActivationStatic);
        UnlockCharacteristic.setEventHandler(BLEWritten, onWriteUnlockStatic);
        // start advertising
        BLE.advertise();
    }

    

    // Handles connection events with central devices.
    void onConnect(BLEDevice central) {
        Serial.print("Connected to ");
        Serial.println(central.address());
        Serial.println(BLE.address());
        digitalWrite(LEDB, LOW); // Optionally, update an LED status to indicate a connection.
    }

     // Static event handler that forwards to the instance method
    static void onConnectStatic(BLEDevice central) {
        if (instance) instance->onConnect(central);
    }

    // Handles disconnection events with central devices.
    void onDisconnect(BLEDevice central) {
        Serial.print("Disconnected from central: ");
        Serial.println(central.address());
        isAuthenticate = false; // Reset the authentication flag.
        userDevice = BLEDevice();
        digitalWrite(LEDB, HIGH); // Optionally, update an LED status to indicate a disconnection.
    }

    static void onDisconnectStatic(BLEDevice central) {
        if (instance) instance->onDisconnect(central);
    }

    // Event handler for writing to the password characteristic.
    void onWritePassword(BLEDevice central, BLECharacteristic characteristic) {
        String password = "13330"; // Predefined password "13330
        String value = String(PasswordCharacteristic.value());
        isAuthenticate = (password.compareTo(value) == 0); // Compare the received password with the predefined password.
        if (isAuthenticate) {
            userDevice = central; // Store the authenticated device.
            Serial.println("Authenticated");
        } else {
            Serial.println("Authentication failed");
        }
    }

    static void onWritePasswordStatic(BLEDevice central, BLECharacteristic characteristic) {
        if (instance) instance->onWritePassword(central, characteristic);
    }

    // Event handler for writing to the name characteristic.
    void onWriteName(BLEDevice central, BLECharacteristic characteristic) {
      if (isAuthenticated(central)) {
        Config.Name = NameCharacteristic.value();
        String value = NameCharacteristic.value();
        Serial.print("Written name : ");
        Serial.println(value);
      } else {
        NameCharacteristic.writeValue("Not authenticated");
      }
    }

    static void onWriteNameStatic(BLEDevice central, BLECharacteristic characteristic) {
        if (instance) instance->onWriteName(central, characteristic);
    }

    // Event handler for reading the name characteristic.
    void onReadName(BLEDevice central, BLECharacteristic characteristic) {
      Serial.println("CALLBACK READ");
      Serial.println(isAuthenticate);
      if (isAuthenticated(central)) {
        NameCharacteristic.writeValue(Config.Name);
      } else {
        NameCharacteristic.writeValue("\n");
      }
    }


    static void onReadNameStatic(BLEDevice central, BLECharacteristic characteristic) {
        if (instance) instance->onReadName(central, characteristic);
    }


    // Event handler for writing to the activation characteristic.
    void onWriteActivation(BLEDevice central, BLECharacteristic characteristic) {
      if (isAuthenticated(central)) {
        Config.isActivate = ActivationCharacteristic.value();
        if (Config.isActivate != 0) {
          Serial.println("Alarme enabled");
          digitalWrite(SIM800_DTR_PIN, LOW);  // put in normal mode
          delay(100);
          sim800l->setPowerMode(NORMAL);  // set normal functionnality mode
        } else {
          Serial.print("Deactivation");
          sim800l->setPowerMode(MINIMUM);      // set minimum functionnality mode
          digitalWrite(SIM800_DTR_PIN, HIGH);  // put in sleep mode
        }
      } else {
        ActivationCharacteristic.writeValue(Config.isActivate);
      }
   }

    static void onWriteActivationStatic(BLEDevice central, BLECharacteristic characteristic) {
        if (instance) instance->onWriteActivation(central, characteristic);
    }

    // Event handler for reading the activation characteristic.
    void onReadActivation(BLEDevice central, BLECharacteristic characteristic) {
      ActivationCharacteristic.writeValue(Config.isActivate);
    }


    static void onReadActivationStatic(BLEDevice central, BLECharacteristic characteristic) {
        if (instance) instance->onReadActivation(central, characteristic);
    }

   // Event handler for writing to the unlock characteristic.
    void onWriteUnlock(BLEDevice central, BLECharacteristic characteristic) {
      if (isAuthenticated(central)) {
        // activate electromagnet
        Serial.println("Unlock");
        digitalWrite(aimantPin, HIGH);
        delay(2000);
        digitalWrite(aimantPin, LOW);
      }
    }


    static void onWriteUnlockStatic(BLEDevice central, BLECharacteristic characteristic) {
        if (instance) instance->onWriteUnlock(central, characteristic);
    }



private:
    // Verifies if the connected central device is the authenticated device.
    bool isAuthenticated(BLEDevice& central) {
        return isAuthenticate && (userDevice.address() == central.address());
    }
    static BluetoothManager* instance; 
};

// Initialize the static instance pointer
BluetoothManager* BluetoothManager::instance = nullptr;