#pragma once

#include "global.h"

/*
  This file contains the functions and setup functions for the Battery.
*/


float getBatteryVoltage() {
  //unsigned int adcCount = analogRead(PIN_VBAT);
  float adcCount = analogRead(PIN_VBAT);
  float adcVoltage = (((adcCount - 3000) / 4096) * 0.55) + 3.6;
  //float vBat = adcVoltage * 1510.0/510.0;
  return adcVoltage;
}

void battery_check() {
  float vBat = getBatteryVoltage();
  Serial.print("Battery voltage: ");
  Serial.print(vBat);
  Serial.println(" V");
}

// ----------SETUP the Battery module----------

void battery_setup(void) {
  pinMode(PIN_VBAT, INPUT);
  Serial.println("Battery launched");
}