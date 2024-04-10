<h1 align="center"> First Brainstorming Report </h1>

<p align="center">
Created by: MARLIER Thibaud <br> Creation Date: 14/03/2024 <br> Last update: 15/03/2024
</p>

<details>
<summary>

## Table of Contents

</summary>

- [Table of Contents](#table-of-contents)
- [1. Introduction](#1-introduction)
- [2. Ideas](#2-ideas)
  - [2.1 Battery](#21-battery)
  - [2.2 NFC](#22-nfc)
  - [2.3 GPRS](#23-gprs)
  - [2.4 Shock Detection](#24-shock-detection)
  - [2.5 Alarm](#25-alarm)
  - [2.6 Bluetooth](#26-bluetooth)
  - [2.7 Other](#27-other)
- [3. Conclusion](#3-conclusion)

</details>

## 1. Introduction

On Thursday the 14th of March 2024, the team took an hour to discuss and brainstorm on the project. Many ideas, information and decisions were taken during this brainstorming session. \
This document will be used to keep track of this session. You'll find all the information that seemed relevant to be kept into account.

## 2. Ideas

This part will summarize all the ideas that we've kept after the brainstorming session.

### 2.1 Battery

- We noticed that the device constantly loops through the whole code, it doesn't seem relevant to us. Moreover, it consumes a consequent amount of energy and time to execute. To improve this, instead of looping the whole program constantly, we have decided to only execute the motion detection and the NFC scanning.

- We decided to implement multithreading. As of right now, the hardware is not able to execute multiple operations at the same time and it is a waste of time.

- Whenever the device is in locked mode, we thought about putting the system in an undervoltage stage, to save battery. It will make the battery last longer without being recharged and improve the lifespan of the battery.

- To save battery power, we decided to remove the GPS, as its usefulness/energy consumption ratio seems to us to be completely unfavourable.

- Considering the magnet, the fact that it heats up easily, shows that it consumes a lot of energy. And has a high rate of waste of energy (apparently the battery doesn't provide enough power to make the magnet work).

- About sending requests every 15 minutes to the server, giving the location and battery state (which wouldn't work without GPS) seems irrelevant and way too frequent. Therefore, instead of sending a request this frequently, we decided that when the user scans the NFC, the device will send a request to the server to notify that the device has been unlocked and the battery state. This will be done to save battery and to make the device more efficient.

- To add to this, we thought about putting an LED on the device that will show the battery state to the user (Green = battery level fine, Red = critical battery level)

- Additionally, we thought about adding a notification on the phone of the user when the battery level is low (around 10-5%). Moreover, if the battery is critically low (around 3%), frequent low beeps will be emitted by the device to notify the user that it is now critical to recharge the device.

- 3 Different modes:
  - Sleep mode: The board and the NFC are still running, but the rest of the device is off. Scanning the NFC will wake the device up.
  - Locked mode: The device is locked, the alarm is activated, and the magnet is on. The board, the motion detection, the NFC and the Bluetooth are always running.
  If the motion system is triggered, it will activate the alarm and the GPRS to send the information about the device being moved.

- We found out that the board runs 24/24 7/7, even in sleep mode. Which is not always necessary.

### 2.2 NFC

- NFC is going to be our secondary focus, after the battery.

- NFC will be used to stop the alarm and to unlock the device.

### 2.3 GPRS

- GPRS usage seems hard to use and test as 2G coverage isn't frequent anymore in our location. As we can't send requests to the server without the GPRS, if we can't test it, we can't be sure that it works and will have to assume that it does.

### 2.4 Shock Detection

- Shock detection is quite effective as shown by the KPIs provided by the client. Unfortunately, it seems to be a bit too sensible. It is a great starting point.

- To prevent a mistake when the user is moving the device, the beep intensity will increase until the reaches the maximum intensity. It still shouldn't be too long to reach the maximum intensity because otherwise, it would get easier to steal.

### 2.5 Alarm

- We have decided to implement NFC compatible with the device, meaning that you could cut the alarm off using the NFC. As well as unlocking the magnet.

### 2.6 Bluetooth

- Needs to be emulated because we can't test it via the application.

### 2.7 Other

- We have found out that the hardware isn't flawless and has many drawbacks, so we have thought about giving a solution to the client.

- Communication with the client is always important, but it seems twice as important as other projects because it's a maintenance/improvement project.

- We shouldn't be scared of being ambitious. We will take some risks during this project.

## 3. Conclusion

We had multiple ideas on how to improve the current system, the final decisions about what will be done will be taken by the program manager and the client via the functional specifications.
