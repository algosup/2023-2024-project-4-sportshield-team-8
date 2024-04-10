
#pragma once

#include "global.h" // Ensure this file defines GPS, GPS_WKUP_PIN, and other related configurations.

/*
 * Class: GPSManager
 * Description: Manages the GPS module, handling initialization, activation, and coordinate conversion.
 * This class encapsulates operations related to the GPS module, including setting it up, activating it,
 * and converting GPS coordinates from DMM format to decimal degrees format for easier use in applications.
 */
class GPSManager {
public:
    // Constructor
    GPSManager() {
      instance = this;
    }

    // Initializes the GPS module with predefined settings.
    void setup() {
        pinMode(GPS_WKUP_PIN, OUTPUT); // Setup wakeup pin as output.
        digitalWrite(GPS_WKUP_PIN, LOW); // Ensure GPS module is not in wakeup mode initially.
        GPS.begin(9600); // Start communication with GPS module at 9600 baud.
        
        // Set GPS module to desired operational modes
        GPS.sendCommand(PMTK_SET_NMEA_OUTPUT_RMCGGA); // Output only minimum recommended data.
        GPS.sendCommand(PMTK_SET_NMEA_UPDATE_1HZ); // Set update rate to 1 Hz.
        GPS.sendCommand("$PMTK225,4*2F"); // Put GPS into backup mode when not in use.
        // Additional configurations can be uncommented as necessary.
        // GPS.sendCommand("$PMTK225,8*23");   // send to Always Locate backup mode
        // GPS.sendCommand("$PMTK225,9*22");   // send to Always Locate standby mode
        // GPS.sendCommand("$PMTK225,2,4000,15000,24000,90000*16");  // send to periodic standby mode
        // GPS.sendCommand("$PMTK161,0*28");   // send to standby mode
    }

    // Handles GPS activation based on certain conditions.
    void activateGPS() {
        if (start_gps) { // Check if the start condition is met.
            digitalWrite(GPS_WKUP_PIN, HIGH); // Activate GPS module.
            start_gps = false; // Reset start condition.
        }
    }

    // Converts DMM GPS coordinates to Decimal Degrees.
    String convertDMMtoDD(String dmmCoordinates) {
        int degrees;
        float minutes;
        // Parse degrees and minutes from DMM format.
        if (dmmCoordinates.length() == 9) {
            degrees = dmmCoordinates.substring(0, 2).toInt();
            minutes = dmmCoordinates.substring(2).toFloat();
        } else {
            degrees = dmmCoordinates.substring(0, 1).toInt();
            minutes = dmmCoordinates.substring(1).toFloat();
        }
        // Convert to decimal degrees.
        float decimalDegrees = degrees + (minutes / 60.0);

        // Return as formatted string.
        return String(decimalDegrees, 10); // Adjust precision as needed.
    }

    // ISR for GPS related interrupts or conditions.
    void GPS_ISR() {
        if (Config.isActivate != 0) {
            if (!position_acquired) {
                start_gps = true;
                digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN)); // Toggle built-in LED as an indicator.
            } else {
                send_position = true; // Trigger position sending.
                position_acquired = false; // Reset position acquired flag.
            }
        }
    }

      // Static method that acts as a wrapper
    static void GPS_ISR_Static() {
        instance->GPS_ISR(); // Call the non-static member function
    }


private:
    bool start_gps = false; // Control flag to determine GPS module activation.
    // Other private members can be added as necessary.
    static GPSManager* instance; // Static instance pointer
};



// Define the static instance pointer somewhere in your .cpp file
GPSManager* GPSManager::instance = nullptr;