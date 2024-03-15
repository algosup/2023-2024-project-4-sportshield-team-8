# SportShield

### Technical

# Audience

**This document is primarily intended for :**
- Software developer : to understand the user and technical requirements, and to guide decision-making and planning. Help them understand risks and challenges, customer requirements, and additional technical requirements and choices made.

**Secondary audiences :**
- Program manager : to validate against the functional specification, and the client expectations.
- QA : to aid in preparing the test-plan and to use for validating issues.
- Project manager : to help identify risks and dependencies

# Deliverable

The team has to deliver a firmware to lock, unlock and ring the alarm of the SportShield alarm system.

## Details

The system is used for up to 6 hours a day.
When not in use only the NFC can turn on the system
It can be locked and unlocked using bluetooth or NFC.
The alarm is trigger upon detecting movement.

## Requirements

- Should run for 7 day without charging
- NFC should only accept one signal
- Bluetooth should only recognize the a paired phone.
- Has to run on a nRF52840 CPU.
- Has to be programed in Arduino C++.
- NFC should always be active

## Nice to have

- Battery should never go bellow 20 and above 80%
- Should send battery status over GPRS

## Priorities

1. Reliability
2. Execution optimization

# Technical Architecture

## Technology Used

### Hardware

- Xiao BLE Sense nrf52840 (CPU & Bluetooth)
- GSM/2G SIM800L Module (GPRS)
- NFC antenna
- 3.7V, 1100mAh lithium battery
- 12V buzzer
- 12V, 500mA Electromagnet
- LSM6DS3 (Accelerometer)

### Software
- C++ 11
- Arduino Compiler
- list of dependency :
    - *NRF52_MBED_TimerInterrupt V1.4.1*
    - *adafruit-nrfutil 0.5.3* (only for linux)
    - *ArduinoBLE V1.3.6*
    - *Sim800L http connector V1.14.0*
    - *Seeed Arduino LSM6DS3 V2.0.3*
    - *OneWire V2.3.7*
    
Those choices where made for hardware reason.

# 


https://github.com/nfc-tools/libnfc
https://content.arduino.cc/assets/st_imu_lsm6ds3_datasheet.pdf
file:///home/max/Downloads/1462360001-AS.pdf

The firmware should be efficient enough to let the battery run for at least 7 days.
The system lock and and unlock feature should be accessible by bluetooth and NFC.
When the device is locked the alarm should be trigger at low volume for a slight movement. 
When the device is locked the alarm should trigger at full volume when moving a lot.



The firmware has to run on an nRF52840 chip. 

Check how much amps the battery can take 

Branch Name: Pascal_Snake_Case
File/Folder Name: camelCase

Install Arduino IDE