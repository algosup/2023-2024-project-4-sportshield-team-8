<h1 align="center"> SportShield - Functional specifications </h1>

<p align="center">
Created by: Maxime THIZEAU <br> Creation Date: 11/03/2024 <br> Last update: 15/03/2024
</p>

___

This project was requested by [ALGOSUP](https://algosup.com), a French computer science school, in partnership with [CORIS Innovation](https://www.corisinnovation.com/), a French consulting firm.

This project aims to complete and optimize the existing software given by **CORIS Innovation**[^1].

<details>
<summary>Table of Contents</summary>

- [Project overview](#project-overview)
  - [Stakeholders](#stakeholders)
    - [Project members](#project-members)
    - [Other stakeholders](#other-stakeholders)
  - [Project scope](#project-scope)
  - [Out of scope](#out-of-scope)
      <!-- - [➭ Additional key](#➭-additional-key) -->
      - [➭ Refurbish hardware](#➭-refurbish-hardware)
  - [Timeline](#timeline)
  - [Deliverables and milestones](#deliverables-and-milestones)
  - [Budget](#budget)
- [Evaluation framework](#evaluation-framework)
  - [Evaluation criteria](#evaluation-criteria)
  - [Risks and assumptions](#risks-and-assumptions)
    - [Risks](#risks)
    - [Assumptions](#assumptions)
- [Personas and use cases](#personas-and-use-cases)
  - [Persona 1 - Georges Michael](#persona-1---georges-michael)
  - [Persona 2 - Jane Hamilton](#persona-2---jane-hamilton)
  - [Persona 3 - Patric Jackson](#persona-3---patric-jackson)
- [Functional requirements](#functional-requirements)
  - [Battery consumption](#battery-consumption)
    - [➭ Modes](#➭-modes)
    - [➭ GPS removal](#➭-gps-removal)
    - [➭ LED implementation](#➭-led-implementation)
    - [➭ Low battery management](#➭-low-battery-management)
    - [➭ Notifications](#➭-notifications)
  - [GPRS management](#gprs-management)
  - [Motion detection](#motion-detection)
  - [Buzzer](#buzzer)
  - [NFC](#nfc)
    - [➭ Alarm turning off](#➭-alarm-turning-off)
    - [➭ Device unlocking](#➭-device-unlocking)
  - [Undervolting](#undervolting)
<!-- - [Non-functional requirements](#non-functional-requirements) -->
- [Future improvements](#future-improvements)
  - [➭ Battery consumption](#➭-battery-consumption)
  - [➭ Forfait & paring](#➭-forfait--paring)
  - [➭ Changing phone](#➭-changing-phone)
- [Glossary](#glossary)

</summary></details>

## Project overview

### Stakeholders

#### Project members

| Full name        | Occupation               | Links                                                               |
| ---------------- | ------------------------ | ------------------------------------------------------------------- |
| Quentin CLEMENT  | Project manager          | [LinkedIn](https://linkedin.com/in/quentin-cl%C3%A9ment-939110221/) |
| Maxime THIZEAU   | Program manager          | [LinkedIn](https://linkedin.com/in/maxime-thizeau-0b311a293)        |
| Max BERNARD      | Tech lead                | [LinkedIn](https://linkedin.com/in/max-bernard-b77680210)           |
| Thomas PLANCHARD | Senior software engineer | [LinkedIn](https://linkedin.com/in/thomas-planchard-461782221/)     |
| Thibaud MARLIER  | Quality assurance        | [LinkedIn](https://linkedin.com/in/thibaud-marlier/)                |
| Victor LEROY     | Technical writer         | [LinkedIn](https://linkedin.com/in/victor-leroy-64baa3229/)         |

#### Other stakeholders

| Name           | Occupation                                 | Links                                            |
| -------------- | ------------------------------------------ | ------------------------------------------------ |
| Florent ANON   | Client (Head of the Innovation Department) | [LinkedIn](https://linkedin.com/in/florentanon/) |

### Project scope

This project is more of a maintenance project because we aren't starting from scratch.

As a matter of fact, we were asked to optimize an existing code of the product given by CORIS Innovation but also to add more features such as the **NFC**[^2].

All these improvements should be done within the deadlines set by the school and respecting the criteria of battery consumption which is really important but also the expected requirements given by the company.

### Out of scope

This project also has some features that could be implemented but are not in our area of expertise and are not feasible in the amount of time given by the school for the SportShield project.

<!-- #### ➭ **Additional key**
Considering the potential issue of empty battery while the device is locked, the decision of adding a little key in the device could be a great idea as an additional security.  -->

#### ➭ **Refurbish hardware**

The hardware should be refurbish as it is not exactly corresponding to the project. It posses some problems in a lot of aspects and or not optimal. \
For example, the electro-magnet consumes way too much energy for the battery and can not be deacticate with the actual layout. \
However, the team was precisely asked to focus only on the software, the research and purchase of better hardware are consequently not our task.

### Timeline

- Planning and Research
  - Learning about the project and preparing it.
  - Week 1
  - 5 half days

- Adding features
  - Adding the NFC, the different level of noises.
  - Week 2 to 4
  - half days

- Tests & Optimizing
  - Reducing the battery consumption and doing the test cases.
  - Week 5
  - half days

- Pitch and support preparation
  - Time reserved for the rehearsal of the final presentation and the creation of the support.
  - Week 6
  - half days

The complete timeline and tasks can be found in the [managing folder](/documents/management/).

### Deliverables and milestones

| Date and time       | Deliverable              |
| --------------------| ------------------------ |
| 18 March 2024, 5 PM | Functional specification |
| 26 March 2024, 5 PM | Technical specification  |
| 30 March 2024, 5 PM | MVP                      |
| 10 April 2024, 5 PM | Test plan                |
| 10 April 2024, 5 PM | User manual              |
| 10 April 2024, 5 PM | Final product            |

### Budget

In term of budget, there's no budget that has been set by the client nor by the team. However, we should keep in mind that the **hardware**[^3] is fragile. We could probably break hardware unintentionally, but it would be our responsibility. It can burn via many ways, get broken etc... \
Moreover, the project can be impeded by the hardware limitation of the product. We reserve the right to give suggestion to the client in order to improve the hardware in the future. \
There are no budget set, but some expenses could happen in the future. It should be kept in mind.

## Evaluation framework

### Evaluation criteria

### Risks and assumptions

#### Risks

#### Assumptions

During this project we would assume:

- The 2G work as expected as CORIS Innovation told us because we will not be able to test it in our environment.
- The **Bluetooth**[^4] can be connected to the application as expected, we can not check it since we have not access to the application.

## Personas and use cases

### Persona 1 - Georges Michael

![Georges Michael](/documents/images/persona/persona1.png)

### Persona 2 - Jane Hamilton

![Jane Hamilton](/documents/images/persona/persona2.png)

### Persona 3 - Patrick Broussard

![Patrick Broussard](/documents/images/persona/persona3.png)

## Functional requirements

The main requirements for this project are:

- Optimize the actual code;
- Add a feature for the NFC;
- Check the buzzer's noise level according to the impact;
- Managing the battery consumption so it can last 7 days in cold conditions (6 hours of active mode and 18 in sleep mode).

### Battery consumption

#### ➭ **Modes**

Three different modes are being set for the device:

- The first mode is the lock mode, it will turn on the **motion detector**[^5], the NFC and the bluetooth, other **modules**[^6] could be activate depending on the situation such as the alarm if impact has been detected;
- The second one is the unlock mode, it consist of switching on only the bluetooth and the NFC one, allowing us to economise energy in the battery;
- Finally we would have the sleep mode which will be used when you don't need to use the device, like this it would not activate the alarm accidentally, only the NFC would be activated.

#### ➭ **GPS removal**

The decision has been made to remove the **GPS**[^7] features from the device, as they were deemed unnecessary for the project and consumed excessive energy.

#### ➭ **LED implementation**

However, consideration has been given to incorporating a small light at the bottom of the device to provide visual information regarding the battery level. It would be set like this:

- The LED is green if the battery level is above 60%;
- The LED is orange when the battery consumption lies between 59 and 20%;
- The LED is red when the battery is under 20%.

#### ➭ **Low battery management**

Additionally, a further security measure is being implemented in the device by:

- implementing a sonorous signal when the battery state is at 10%;
- sending a notification informing of the state.
The sound would also become more frequent when the battery level would be under 5%.

#### ➭ **Notifications**

The fifteen minutes' notifications during the actual sleeping mode has been estimated too high and irrevelant. They would be removed. \
However two kinds of notification will be set. One for the battery level and another for the impact detection.

### GPRS management

The decicison was made to change the **GPRS**[^8] notifications. Henceforth, they will contain:

- The current mode of the device;
- The battery level;
- The state of the device (lock or not).

### Motion detection

The motion detection has been considered as too sensitive and would then be imroved in a way that parasyte movements such as the wind should not be detected as motion and then not activate the alarm system. \
To be more precise, all the motion detected by the hardware under five mm/s for the translations and under ten deg/s for the roation would be considered as parasyte and not activate the alarm. \
Regarding the important impact, they would be detected above ten mm/s for the translation and twenty deg/s for the rotation.

### Buzzer

When considering the accidental impact against the device, someone who fell on it for example. \
It has been decided to implement a crescendo sound and not an important one from the beginning. \
It would start with a 70 dB sound for three seconds before increasing to the intense signal of 95 dB.

### NFC

#### ➭ **Alarm turning off**

It has been found relevant to implement a system to cut the alarm with a NFC such as a little card or a badge and not necessarly the application. \
It would be quickier to stop the alarm than taking the phone, opening the application and deactivate the alarm.

#### ➭ **Device unlocking**

It has been decided to add an other features with the NFC such as an unlock one. After pressing the NFC against the device during two seconds it would unlock it. \
Moreover this feature will override the previous one. In the case of an alarm ringing it could stop the alarm and unlock the device if you press the NFC against the sensor during two seconds.

### Undervolting

**Undervolting**[^9] would be used to lower the energy consumption in specific mode such as the lock one and the unlock. \
However, after a second look on it, it has been decided not to implement it due to the risk of damaging the product in itself.

<!-- ## Non-functional requirements -->

## Future improvements

Considering the short amount of time given and the important amount of tasks and requirements the project have. It has been decided to postpone some of then, deemed non-critical to the completion of the SportShield project in the short term. \
However, these tasks have been planned to be done as future improvements in the long term.

### ➭ Battery consumption
One of the requirements given by the company was to use the battery only between 20 and 80%. \
Estinating the Apple's battery management with the intelligent battery system, consisted on charging just before the average time usage and stoping it at 80%. \
The team thought it is what the device needs. \
However, considering the difficulty of the task and the amount of time, it has been decided to do it later on.

### ➭ Forfait & paring
Searching for an improvement of the system and an optimization, it has been agreeed to implement a system forfait with ski station or the feature of pairing more than one device at a time such as badges for example. \
This system would be implemented in a brief delay after the release of the product as an update.*

### ➭ Changing phone
One issue stood out, the pairing of the phone if it has to be change.
A feature would be added, comporting an option to unpair the actual device an could only be activated on the application if the bluetooth is activated (lock and un-lock modes only).

## Glossary

[^1]: **Coris Innovation**: an European company based in France and Switzerland. The compnay focuses on systems engineering and process engineering .Their ambition is to support European industrial players in their innovation,  R&D, digitalization and production challenges. [Coris Innovation's website](https://www.corisinnovation.com)

[^2]: **NFC**: stands for Near Field Communication, is a set of short-range wireless technologies. It lets you share small payloads of data between an NFC tag and an Android-powered device. It has the ability to rapidly establish a connection, taking only one tenth of the time Bluetooth requires to complete pairing. [More information](https://www.corisinnovation.com)

[^3]: **Hardware**: the machines, wiring, and other physical components of a computer or other electronic system. The physical and electronic parts of a computer or other piece of equipment, rather than its software. [Definition](https://dictionary.cambridge.org/dictionary/english/hardware)

[^4]: **Bluetooth**: is a short-range wireless technology standard that is used for exchanging data between fixed and mobile devices over short distances. It is mainly used as an alternative to wired connections to exchange files between nearby portable devices and connect cell phones and music players with wireless headphones. [Bluetooth](https://en.wikipedia.org/wiki/Bluetooth)

[^5]: **Motion Detector**: is an electronic device that is designed to detect and measure movement. Motion sensors can be customized to perform highly specific functions. For example, motion sensors can be used to activate floodlights, trigger audible alarms, activate switches, and even alert the police. [Motion Detector](https://www.fierceelectronics.com/sensors/what-a-motion-sensor)

[^6]: **Modules**:  module is an assembly of parts designed to be added and removed from a larger system easily. In this project, modules are for instance: the battery, the buzzer, the NFC scanner etc.. [Modules](https://www.techtarget.com/whatis/definition/module#:~:text=A%20module%20is%20a%20distinct,is%20designed%20for%20easy%20replacement.)

[^7]: **GPS**: Global Positioning System (GPS), is a satellite-based radio navigation system owned by the United States government. It is a satellite constellation supporting highly accurate positioning, navigation and timing (PNT) measurements worldwide. [GPS](https://novatel.com/support/knowledge-and-learning/what-is-gps-gnss)

[^8]: **GPRS**: abbreviation for general packet radio service: a system for sending and receiving images and other information using mobile phones. It is make for good battery life but limits online activity.  It is a packet oriented mobile data standard on the 2G cellular communication network's global system for mobile communications.

[^9]: **Undervoltage**: The undervoltage-lockout (UVLO) is an electronic circuit used to turn off the power of an electronic device in the event of the voltage dropping below the operational value that could cause unpredictable system behavior. [Undervoltage](https://en.wikipedia.org/wiki/Undervoltage-lockout)
