#pragma once

#include <Arduino.h>
#include <LSM6DS3.h>
#include <stdint.h>

enum MovementType {
    NO_MOVEMENT = 0,
    WAKEUP_MOVEMENT = 1,
    LARGE_MOVEMENT = 2
};

class MotionDetection {
private:
    LSM6DS3 imu;
    float wakeupThreshold_;
    float largeMovementThreshold_;
    bool active_;

public:
    // Constructor initializes the LSM6DS3 on I2C
    MotionDetection() : imu(LSM6DS3_CORE_I2C, 0x6A), wakeupThreshold_(0.0), largeMovementThreshold_(0.0), active_(false) {
        if (!imu.begin()) {
            Serial.println("Failed to initialize IMU!");
            while (1);
        }
    }

      void configureIMU() {
        // Set FIFO to bypass mode
        imu.writeRegister(LSM6DS3_ACC_GYRO_FIFO_CTRL5, 0x00); 
        if (imu.readRegister(LSM6DS3_ACC_GYRO_FIFO_CTRL5) != 0x00) {
            Serial.println("Failed to set FIFO to bypass mode!");
            while (1);
        }

        // Disable interrupt 2
        imu.writeRegister(LSM6DS3_ACC_GYRO_INT2_CTRL, 0x00);
        if (imu.readRegister(LSM6DS3_ACC_GYRO_INT2_CTRL) != 0x00) {
            Serial.println("Failed to disable interrupt 2!");
            while (1);
        }

        // If not using the gyroscope, turn it off to save power
        imu.writeRegister(LSM6DS3_ACC_GYRO_CTRL2_G, 0x00);
        if (imu.readRegister(LSM6DS3_ACC_GYRO_CTRL2_G) != 0x00) {
            Serial.println("Failed to power down gyroscope!");
            while (1);
        }

        // Set the accelerometer to 26 Hz low power mode
        imu.writeRegister(LSM6DS3_ACC_GYRO_CTRL1_XL, 0x10); 
        if (imu.readRegister(LSM6DS3_ACC_GYRO_CTRL1_XL) != 0x10) {
            Serial.println("Failed to set accelerometer to 26 Hz low power mode!");
            while (1);
        }
        // Configure INT1 pin for tilt detection
        imu.writeRegister(LSM6DS3_ACC_GYRO_MD1_CFG, 0b00000010);
        if (imu.readRegister(LSM6DS3_ACC_GYRO_MD1_CFG) != 0b00000010) {
            Serial.println("Failed to configure INT1 pin for tilt detection!");
            while (1);
        }
        // Enable significant motion detection interrupt on INT1
        imu.writeRegister(LSM6DS3_ACC_GYRO_INT1_CTRL, 0b01000000);
        if (imu.readRegister(LSM6DS3_ACC_GYRO_INT1_CTRL) != 0b01000000) {
            Serial.println("Failed to enable significant motion detection interrupt on INT1!");
            while (1);
        }
    }


    bool setPassive() {
        imu.writeRegister(LSM6DS3_ACC_GYRO_CTRL1_XL, 0x00); // Power down accelerometer
        if (imu.readRegister(LSM6DS3_ACC_GYRO_CTRL1_XL) != 0x00) {
            return false; // Return false if unsuccessful
        }
        imu.writeRegister(LSM6DS3_ACC_GYRO_CTRL2_G, 0x00);  // Power down gyroscope
        if (imu.readRegister(LSM6DS3_ACC_GYRO_CTRL2_G) != 0x00) {
            return false; // Return false if unsuccessful
        }
        active_ = false;
        return true; // Return true if successful
    }


     bool setActive() {
        // Reactivate the IMU with the initial configuration for active detection
        configureIMU();
        active_ = true;
        return true; // Return true if successful
    }

    bool setWakeupThreshold(float threshold) {
        if (threshold >= 0) {
            wakeupThreshold_ = threshold;
            return true;
        }
        Serial.println("Wakeup threshold must be greater than or equal to 0!");
        return false;
    }

    bool setLargeMovementThreshold(float threshold) {
        if (threshold >= wakeupThreshold_) {
            largeMovementThreshold_ = threshold;
            return true;
        }
        Serial.println("Large movement threshold must be greater than wakeup threshold!");
        return false;
    }

    uint8_t getMovement() {
        if (!active_) return 0; // Return 0 if IMU is not active

        // Read accelerometer data
        float x = imu.readFloatAccelX();
        float y = imu.readFloatAccelY();
        float z = imu.readFloatAccelZ();

        // Compute the magnitude of acceleration vector
        float magnitude = sqrt(x * x + y * y + z * z) - 1.0; // Subtract 1g for gravity compensation

        // Check against thresholds
        if (magnitude < wakeupThreshold_) {
            return NO_MOVEMENT; // No significant movement
        } else if (magnitude >= wakeupThreshold_ && magnitude < largeMovementThreshold_) {
            return WAKEUP_MOVEMENT; // Wakeup/small movement
        } else if (magnitude >= largeMovementThreshold_) {
            return LARGE_MOVEMENT; // Large movement
        }

        return 0; // Default case, should not be reached
    }
};
