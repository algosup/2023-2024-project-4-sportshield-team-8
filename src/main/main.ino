#pragma once
#include <Arduino.h>
#include "bleModule.h"
#include "gpsModule.h"
#include "imuModule.h"
#include "simModule.h"
#include "batteryManager.h"
#include "buzzerModule.h"
#include "struct.h"



BLEModule bleModule;
GPSModule gpsModule;
IMUModule imuModule;
SIMModule simModule;
BatteryManager batteryManager( A0, 3.3, 2.0, 3.4);  // Example values: analog pin A0, 3.3V reference, 2:1 voltage divider, 3.4V low battery threshold


void setup() {
  Serial.begin(115200);
  bleModule.setup();
  gpsModule.setup();
  imuModule.setup();
  simModule.setup();
}

void loop() {
  // Implement logic to interact with each module using their methods
}
