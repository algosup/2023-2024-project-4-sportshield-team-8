#pragma once

#include "global.h"

/*
  This file contains the functions and setup functions for the GPS.
*/



void GPS_ISR() {
  if (Config.isActivate != 0) {
    if (!position_acquired) {
      start_gps = true;
      digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));
    } else {
      // digitalWrite(GPS_WKUP_PIN, LOW);
      // GPS.sendCommand("$PMTK225,4*2F");  // send to backup mode
      send_position = true;
      position_acquired = false;
    }
  }
}

void activateGPS() {
  if (start_gps == true) {
    digitalWrite(GPS_WKUP_PIN, HIGH);
    start_gps = false;
  }
}



String convertDMMtoDD(String dmmCoordinates) {
  int degrees;
  float minutes;
  // Separate coordinates in degrees and decimal minutes
  if (dmmCoordinates.length() == 9) {
    degrees = dmmCoordinates.substring(0, 2).toInt();
    minutes = dmmCoordinates.substring(2).toFloat();
  } else {
    degrees = dmmCoordinates.substring(0, 1).toInt();
    minutes = dmmCoordinates.substring(1).toFloat();
  }
  // Convert decimal minutes to decimal degrees
  float decimalDegrees = degrees + (minutes / 60.0);

  // Convert to string and format coordinates to decimal degrees
  String ddCoordinates = String(decimalDegrees, 10);  // You can adjust the number of decimals here

  return ddCoordinates;
}

// ----------SETUP the GPS module----------
void gps_setup(void) {
  pinMode(GPS_WKUP_PIN, OUTPUT);
  digitalWrite(GPS_WKUP_PIN, LOW);
  GPS.begin(9600);
  GPS.sendCommand(PMTK_SET_NMEA_OUTPUT_RMCGGA);
  GPS.sendCommand(PMTK_SET_NMEA_UPDATE_1HZ);
  GPS.sendCommand("$PMTK225,4*2F");  // send to backup mode
  // GPS.sendCommand("$PMTK225,8*23");   // send to Always Locate backup mode
  // GPS.sendCommand("$PMTK225,9*22");   // send to Always Locate standby mode
  // GPS.sendCommand("$PMTK225,2,4000,15000,24000,90000*16");  // send to periodic standby mode
  // GPS.sendCommand("$PMTK161,0*28");   // send to standby mode
}
