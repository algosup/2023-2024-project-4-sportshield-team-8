<h1 align="center"> First Brainstorming Report </h1>

<p align="center">
Created by: MARLIER Thibaud <br> Creation Date: 14/03/2024 <br> Last update: 14/03/2024
</p>

<details>
<summary>

## Table of Contents

</summary>
// insert Table Of Content

</details>

# Introduction

On Thursday the 14th of March 2024, the team took an hour to discuss and brainstorm on the project. Many ideas, information and decisions were taken during this brainstorming session. /
This document is used to keep track of this session. You'll find all the information that seemed relevant to be kept into account.

# Ideas

This part will summarize all the ideas that we've kept after the brainstorming session.

## Battery

- We have noticed that the code constantly runs a big loop, that didn't seem this relevant to us. Moreover, it consumes loads of energy and time to do so. To improve this, instead of looping the whole program constantly, we have decided that only the motion detection should be looped by the program constantly, and only activate the modules when needed.

- We have decided to implement multithreading. As of right now, the hardware is not capable of that. We thought that it would be necessary to fulfil some client requirements.

- Whenever the device is in sleep mode, we have decided to put the system in an undervoltage stage, to save battery. It will make the battery last longer without being recharged and improve the lifespan of the battery.

- We have thought of removing the GPS, as the device is in Bluetooth or NFC, it seems irrelevant to us to keep the GPS, as tracking isn't a requirement. Moreover, it will save the battery of the device.

- Considering the magnet, the fact that it heats up easily, shows that it consumes a lot of energy. And has a high rate of waste of energy.

- About sending requests every 15 minutes to the server, giving the location and battery state (which wouldn't work without GPS) seemed irrelevant and way too frequent. Therefore, instead of sending a request this frequently, we have decided to use Bluetooth paired with the NFC for the user to ask about the battery state.

- To add to this, we have thought about putting an LED on the device that will show the battery state to the user (Green = battery level fine, Red = critical battery level)

- Additionally, we have thought about adding a notification on the phone of the user when the battery level is low (around 10-5%). Moreover, if the battery is critically low (around 3%), frequent low beeps will be made by the device to notify the user that it is now critical to recharge the device.

- Different modes (I forgot again, add to that Quentin plz)

- We have found out that the card runs 24/24 7/7, even in sleep mode. Which is not always necessary.

## NFC

- NFC is going to be our secondary focus, after the battery.

- NFC will be used to unlock the device.

## GPRS

- GPRS usage seems hard to use and test as 2G coverage isn't frequent anymore in our location. To test whether the 2G is working, we would need to assume it does. The behaviour of the GPRS connection will be supposed.

## Shock Detection

- Shock detection is quite effective as shown by the KPIs provided by the client. It is a great starting point.

- We have decided to make the beeps go crescendo if the shock are becoming more frequent and harder.

## Alarm

- We have decided to implement NFC compatible with the device, meaning that you could cut the alarm off using the NFC. As well as unlocking the magnet.

## Bluetooth

- Needs to be emulated

## Other

- We have found out that the hardware isn't flawless and has many drawbacks, so we have thought about giving a solution to the client.
  
- Communication with the client is always important, but it seems twice as important as other projects.

- We shouldn't be scared of being ambitious. We will take some risks during this project.

# Decisions

# Glossary
