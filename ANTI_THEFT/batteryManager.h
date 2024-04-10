#pragma once

#include "global.h" // Include global definitions and configurations.

/*
 * Class: BatteryManager
 * ---------------------
 * The BatteryManager class encapsulates battery voltage measurement
 * functionality for the project. It utilizes the analog reading from a specified
 * pin (PIN_VBAT) to calculate and report the battery's current voltage.
 */
class BatteryManager {
public:
    /*
     * Constructor: BatteryManager
     * Usage: BatteryManager batteryManager;
     * -----------------------------------
     * Constructs a BatteryManager object. Currently, the constructor does not
     * perform any actions but is defined to allow for future expansion where
     * initialization parameters might be needed.
     */
    BatteryManager() {}

    /*
     * Method: setup
     * Usage: batteryManager.setup();
     * ------------------------------
     * Initializes the hardware pin (PIN_VBAT) for reading battery voltage. This
     * method must be called before attempting to read the battery voltage.
     * It sets the pin mode to INPUT and prints an initialization message
     * to the Serial monitor for debugging purposes.
     */
    void setup() {
        pinMode(PIN_VBAT, INPUT); // Set the battery voltage pin as input.
        Serial.println("Battery module initialized"); // Debugging message.
    }

    /*
     * Method: getVoltage
     * Usage: float voltage = batteryManager.getVoltage();
     * ---------------------------------------------------
     * Reads the current battery voltage by performing an analog-to-digital
     * conversion on the PIN_VBAT pin. It then calculates the actual battery
     * voltage using a predefined formula and returns this value.
     *
     * The formula used to calculate the battery voltage from the ADC count
     * takes into account the specific characteristics of the hardware setup,
     * including the ADC resolution and any voltage division applied to the
     * battery voltage before it is fed to the ADC input.
     *
     * Returns:
     *   The current battery voltage as a float.
     */
    float getVoltage() {
        float adcCount = analogRead(PIN_VBAT); // Read the ADC count from the battery pin.
        // Calculate the actual battery voltage from the ADC count.
        float adcVoltage = (((adcCount - 3000) / 4096.0) * 0.55) + 3.6;
        return adcVoltage;
    }

    /*
     * Method: checkVoltage
     * Usage: batteryManager.checkVoltage();
     * -------------------------------------
     * Prints the current battery voltage to the Serial monitor. This method
     * is useful for debugging and monitoring the battery status during
     * development and testing. It calls getVoltage() to retrieve the current
     * battery voltage and then prints this value to the Serial monitor.
     */
    void checkVoltage() {
        float vBat = getVoltage(); // Retrieve the current battery voltage.
        Serial.print("Battery voltage: "); // Debug message prefix.
        Serial.print(vBat); // Print the current battery voltage.
        Serial.println(" V"); // Voltage unit.
    }
};
