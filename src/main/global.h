/*
    This file contains all the global variables used in the project.
*/
#pragma once

#ifndef GLOBALS_H
#define GLOBALS_H

#include "NRF52_MBED_TimerInterrupt.h"
#include "NRF52_MBED_ISR_Timer.h"
#include <ArduinoBLE.h>
#include "struct.h"
#include <LSM6DS3.h>
#include <Wire.h>
#include <Adafruit_GPS.h>
#include "SIM800L.h"

// Timer settings
#define TIMER_INTERRUPT_DEBUG 0
#define _TIMERINTERRUPT_LOGLEVEL_ 0
#define HW_TIMER_INTERVAL_MS 1000
#define TIMER_INTERVAL_120S 120000L

// GPS Wakeup Pin
#define GPS_WKUP_PIN D8

// SIM800L Pins and Timeouts
#define SIM800_RST_PIN A5
#define SIM800_DTR_PIN A5
#define TIME_OUT_MS_BLE_ACT 5000  // Timeout for BLE activity

// Buzzer Pin
#define BUZZER_PIN D2

// Electro-magnet Pin
#define AIMANT_PIN D3

// Battery Voltage Enable Pin
#define VBAT_ENABLE 14

// Battery Voltage Thresholds
#define FULL_CHARGE 3.65
#define LOW_BATTERY 3.475
#define LOCKED_BATTERY 3.4

// Motion Thresholds
#define SMALL_MOTION_THRESHOLD 40.0
#define BIG_MOTION_THRESHOLD 170.0
#define SMALL_ROTATION_THRESHOLD 40.0
#define BIG_ROTATION_THRESHOLD 145.0

// Global Configuration
myConfig Config;
bool isAuthenticate = false;

// BLE Global Variables
BLEService PasswordService("19B10000-E8F2-537E-4F6C-D104768A1213");
BLEService ConfigService("19B10000-E8F2-537E-4F6C-D104768A1214");
BLEStringCharacteristic PasswordCharacteristic("19B10000-E8F2-537E-4F6C-D104768A1213", BLEWrite, 32);
BLEStringCharacteristic NameCharacteristic("19B10001-E8F2-537E-4F6C-D104768A1214", BLERead | BLEWrite, 20);
BLEStringCharacteristic MACCharacteristic("19B10001-E8F2-537E-4F6C-D104768A1217", BLERead, 20);
BLEBooleanCharacteristic ActivationCharacteristic("19B10001-E8F2-537E-4F6C-D104768A1215", BLERead | BLEWrite);
BLEBooleanCharacteristic UnlockCharacteristic("19B10001-E8F2-537E-4F6C-D104768A1216", BLEWrite);

BLEDescriptor PasswordDescriptor("2901", "Password");  // Bluetooth® Low Energy Descriptor
BLEDescriptor NameDescriptor("2901", "Name");
BLEDescriptor ActivationDescriptor("2901", "Activation");
BLEDescriptor UnlockDescriptor("2901", "Unlock");
BLEDescriptor MACDescriptor("2901", "MAC Address");

BLEDevice userDevice;  // Device connected to BLE

// IMU (LSM6DS3) Global Variables
LSM6DS3 imu(I2C_MODE, 0x6A);

// GPS Global Variables
Adafruit_GPS GPS(&Serial1);

// SIM800L Global Variables
UART Serial2(D0, D1, NC, NC);
SIM800L* sim800l;

// Battery and Movement Detection Variables
bool isLowBattery = false;
bool isLockedBattery = false;
float MotionData;
float RotationData;
float lastLatitude = 0.0;
float lastLongitude = 0.0;

// Timer Objects
NRF52_MBED_Timer ITimer(NRF_TIMER_3);
NRF52_MBED_ISRTimer ISR_Timer;

#endif // GLOBALS_H
