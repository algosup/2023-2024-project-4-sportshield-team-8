
#pragma once

#include "global.h" // Ensure this file provides access to the IMU object and related configurations.

/*
 * Class: IMUManager
 * Description: Manages interactions with the Inertial Measurement Unit (IMU), including initialization,
 * reading acceleration and gyroscope data, and determining the magnitude of movements.
 */
class IMUManager {
public:
    // Constructor: Initializes the IMU device and verifies communication.
    IMUManager() {
    }

    // Initializes the IMU module, checks for communication, and sets up necessary configurations.
    void setup() {
        if (imu.begin() != 0) {
            Serial.println("IMU initialization failed!");
        } else {
            Serial.println("IMU successfully initialized.");
        }
    }

    // Reads the acceleration data and calculates the magnitude of motion since the last reading.
    float getMotionData() {
        static float previousAcceleration = 0;
        float accelX = imu.readFloatAccelX();
        float accelY = imu.readFloatAccelY();
        float accelZ = imu.readFloatAccelZ();

        float currentAcceleration = sqrt(accelX * accelX + accelY * accelY + accelZ * accelZ) * 100;
        float motionDifference = currentAcceleration - previousAcceleration;
        previousAcceleration = currentAcceleration;

        return fabs(motionDifference); // Return the absolute value of the difference.
    }

    // Reads the gyroscope data and calculates the magnitude of rotation since the last reading.
    float getRotationData() {
        static float previousRotation = 0;
        float gyroX = imu.readFloatGyroX();
        float gyroY = imu.readFloatGyroY();
        float gyroZ = imu.readFloatGyroZ();

        float currentRotation = sqrt(gyroX * gyroX + gyroY * gyroY + gyroZ * gyroZ);
        float rotationDifference = currentRotation - previousRotation;
        previousRotation = currentRotation;

        return fabs(rotationDifference); // Return the absolute value of the difference.
    }

    // Analyzes motion and rotation data to determine if a movement is large or small.
    void analyzeMovement(float motionData, float rotationData) {
        if (motionData > BigMT || rotationData > BigRT) { // Detect large movements.
            Serial.print("Large movement detected: ");
            Serial.print(motionData > BigMT ? "Motion: " : "Rotation: ");
            Serial.println(motionData > BigMT ? motionData : rotationData);
            MotionBig = true;
            MotionSmall = false;
            send_move = true;
        } else if (!MotionBig && (motionData > SmallMT || rotationData > SmallRT)) { // Detect small movements.
            Serial.print("Small movement detected: ");
            Serial.print(motionData > SmallMT ? "Motion: " : "Rotation: ");
            Serial.println(motionData > SmallMT ? motionData : rotationData);
            MotionSmall = true;
        }
    }


};
