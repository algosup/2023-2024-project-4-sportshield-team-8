
#pragma once

#include "global.h" // Ensure this includes necessary definitions for sim800l, pins, and other configurations.

/*
 * Class: SIMModuleManager
 * Description: Manages the initialization, configuration, and operation of the SIM module.
 * This class encapsulates the functionality related to the SIM800L module, including its
 * setup, network registration, and power management. It provides a streamlined interface
 * for activating the SIM module and handling network communications.
 */


class SIMModuleManager {
public:
    // Constructor: Optionally accepts configuration parameters.
    SIMModuleManager() {}

    // Initializes and configures the SIM module for operation.
    void setup() {
        // Ensure the SIM module is ready, retrying if necessary.
        while (!sim800l->isReady()) {
            Serial.println(F("Problem to initialize AT command, retry in 1 sec"));
            toggleLED(LEDR); // Toggle an LED to indicate waiting status.
            delay(1000);
        }

        // Enable echo mode for debugging and setup GPRS with a predefined APN.
        sim800l->enableEchoMode();
        sim800l->setupGPRS("iot.1nce.net");

        // Wait for a valid signal strength indication.
        waitForSignal();

        // Register on the network (home or roaming).
        waitForNetworkRegistration();

        // After successful setup, set the module to minimum functionality mode to save power.
        enterSleepMode();
    }

    // Interrupt Service Routine (ISR) for SIM-related operations.
    void ISR() {
        send_position = true; // Set a flag or trigger an action to send the current position.
    }

private:
    // Waits for a valid signal strength before proceeding.
    void waitForSignal() {
        uint8_t signal = sim800l->getSignal();
        while (signal <= 0) {
            delay(1000); // Retry every second until a signal is found.
            signal = sim800l->getSignal();
        }
        Serial.println(String(signal)); // Log signal strength for debugging.
    }

    // Ensures network registration is successful before proceeding.
    void waitForNetworkRegistration() {
        NetworkRegistration network = sim800l->getRegistrationStatus();
        while (network != REGISTERED_HOME && network != REGISTERED_ROAMING) {
            delay(1000); // Retry every second until registered.
            network = sim800l->getRegistrationStatus();
            Serial.print(network + " ");
            Serial.println(F("Problem to register, retry in 1 sec"));
            toggleLED(LEDG); // Optionally toggle an LED to indicate registration status.
        }
    }

    // Enters sleep mode to reduce power consumption when not actively using the SIM module.
    void enterSleepMode() {
        sim800l->setPowerMode(MINIMUM); // Set to minimum functionality mode.
        digitalWrite(SIM800_DTR_PIN, HIGH); // Signal the module to enter sleep mode.
    }

    // Utility function to toggle an LED's state.
    void toggleLED(int pin) {
        digitalWrite(pin, !digitalRead(pin));
    }
};
