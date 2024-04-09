#pragma once 


/*
  This file contains the functions and setup functions for the IMU.
*/

#include "global.h"



float getMotionData() {
  static float previousAcceleration = 0;
  //r
  float accelX = imu.readFloatAccelX();
  float accelY = imu.readFloatAccelY();
  float accelZ = imu.readFloatAccelZ();

  float currentAcceleration = sqrt(accelX * accelX + accelY * accelY + accelZ * accelZ) * 100;
  float MotionDataerence = currentAcceleration - previousAcceleration;  // Calculate the acceleration difference
  previousAcceleration = currentAcceleration;

  return fabs(MotionDataerence);  //returns a value always positive
}


float getRotationData() {
  static float previousRotation = 0;

  // Read gyroscope values
  float gyroX = imu.readFloatGyroX();
  float gyroY = imu.readFloatGyroY();
  float gyroZ = imu.readFloatGyroZ();

  float currentRotation = sqrt(gyroX * gyroX + gyroY * gyroY + gyroZ * gyroZ);  // Calculate the current rotation based on gyroscope readings
  float RotationDataerence = currentRotation - previousRotation;                // Calculate the difference in rotation
  previousRotation = currentRotation;                                           // Update the previous rotation value

  return fabs(RotationDataerence);
}

void checkIfaMovementisEitherLargeOrSmall(float MotionData, float RotationData){
  if (MotionData > BigMT || RotationData > BigRT) {  //Big motion detection
      if (MotionData > BigMT) {
        Serial.print("Motion detected : ");
        Serial.println(MotionData);
      } else {
        Serial.print("Rotation detected : ");
        Serial.println(RotationData);
      }
      MotionBig = true;
      MotionSmall = false;
      send_move = true;

    } else if ((MotionBig == false) && (MotionData > SmallMT || RotationData > SmallRT)) {  //Small motion detection
      if (MotionData > SmallMT) {
        Serial.print(" Small motion: ");
        Serial.println(MotionData);
      } else {
        Serial.print("Small rota : ");
        Serial.println(RotationData);
      }
      MotionSmall = true;
    }
}




// ----------SETUP the IMU module----------

void imu_setup(void) {
  if (imu.begin() != 0) {
    Serial.println("Device error");
  } else {
    Serial.println("Accelerometer launched");
  }
}
