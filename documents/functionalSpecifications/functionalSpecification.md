# SportShield - Functional specifications

This project was requested by [ALGOSUP](https://algosup.com), a French computer science school, in partnership with [CORIS Innovation](https://www.corisinnovation.com/), a French consulting firm.

This project aims to complete and optimize the existing software given by CORIS Innovation.

<details>
<summary>Table of Contents</summary>

- [Project overview](#project-overview)
    - [Stakeholders](#stakeholders)
        - [Project members](#project-members)
        - [Other stakeholders](#other-stakeholders)
    - [Project scope](#project-scope)
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
    - [Goals](#goals)
    - [Challenges](#challenges)
  - [Persona 2 -](#persona-2---jane-hamilton)
    - [Goals](#goals-1)
    - [Challenges](#challenges-1)
  - [Persona 3 - Patric Jackson](#persona-3---patric-jackson)
    - [Goals](#goals-2)
    - [Challenges](#challenges-2)
- [Functional requirements](#functional-requirements)
    - [Battery consumption](#battery-consumption)
        - [➭ Modes](#➭-modes)
        - [➭ GPS removal](#➭-gps-removal)
        - [➭ LED implementation](#➭-led-implementation)
        - [➭ Low battery management](#➭-low-battery-management)
- [Non-functional requirements](#non-functional-requirements)
- [Future improvements](#future-improvements)
- [Out of scope](#out-of-scope)
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

As a matter of fact, we were asked to optimize an existing code of the product given by CORIS Innovation but also to add more features such as the NFC or ... 

All these improvements should be done within the deadlines set by the school and respecting the criteria of battery consumption which is really important.

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

The complete timeline and tasks can be found in the [managing folder](). <!-- TODO -->

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

In term of budget, there's no budget that has been set by the client nor by the team. However, we should keep in mind that the hardware is fragile. We could probably break hardware unintentionally, but it would be our responsibility. It can burn via many ways, get broken etc... \
Moreover, the project can be impeded by the hardware limitation of the product. We reserve the right to give suggestion to the client in order to improve the hardware in the future. \
There are no budget set, but some expenses could happen in the future. It should be kept in mind.

## Evaluation framework

### Evaluation criteria

### Risks and assumptions

#### Risks

#### Assumptions

During this project we would assume:
- The 2G work as expected as CORIS Innovation told us because we will not be able to test it in our environment.
- The bluetooth can be connected to the application as expected, we can not check it since we have not access to the application.

## Personas and use cases

### Persona 1 - Georges Michael

Georges Michael is a 50-years-old man working as a ski instructor during the season.

#### Goals

- Georges wants to secure all the skis of his students when they are doing theoretical courses in the school.
- He also wants to be informed every fifteen minutes of their location to reassure himself.

#### Challenges

- Georges is looking for a simple device allowing him to secure the skis.
- He is searching for a device capable to send him the actual position of the skis during the class.
- He needs a device lasting more than six hours in snowy conditions.

### Persona 2 - Jane Hamilton

Jane Hamilton is a young 24-years-old woman eager to see new landscapes such as mountains.

#### Goals

- Jane wants to go downhill but also to visit the town. She then needs a device to secure her snowboard when she goes in an establishment.
- She is not an enthusiast of smartphone while doing tourism, she wants an audio signal if her snowboard is stolen.

#### Challenges

- Jane needs a device capable of tracking the movement of her snowboard while she isn't using it.
- She wants to activate and deactivate the security easily.
- She is looking for a sonorous security included in the device.

### Persona 3 - Patric Jackson

#### Goals

#### Challenges

## Functional requirements

The main requirements for this project are:

- Optimize the actual code;
- Add a feature for the NFC;
- Check the buzzer's noise level according to the impact;
- Managing the battery consumption so it can last 7 days in cold conditions (6 hours of active mode and 18 in sleep mode).

### Battery consumption

#### ➭ **Modes**
Three different modes are being set for the device:
  - The first mode is the actif mode, it turns on all the modules inside the device;
  - The second one is the anti-thief mode, it consist of switching on only the movement sensor and the NFC one, allowing us to economise energy in the battery. However if a movement is detected it will automatically switch on the needed module;
  - Finally we would have the off mode which will be used when you don't need to use the device, like this it would not consume energy.

#### ➭ **GPS removal**
The decision has been made to remove the GPS features from the device, as they were deemed unnecessary for the project and consumed excessive energy.

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

#### ➭ **Undervolting**
Undervolting would be used to lower the energy consumption in specific mode such as the actif one and the anti-thieft.

#### ➭ **Notification reduction**
The fifteen minutes' notifications during the actual sleeping mode has been estimated too high and irrevelant. They would be changed as one notification every forty-five minutes. However, the user will be able to request the notification via the application.

### GPRS management

The decicison was made to change the GPRS notifications. Henceforth, they will contain:
  - The current mode of the device;
  - The battery level;
  - The state of the device (lock or not).

### Buzzer

When considering the accidental impact against the device, someone who fell on it for example. It has been decide to implement a crescendo sound and not an important one from the beginning. It would start with a ... dB for two seconds before increasing it at ... dB to finally had the intense signal of ... dB at the fourth second.

### NFC

#### ➭ **Alarm turning off**
It has been found relevant to implement a system to cut the alarm with a NFC such as a little card or a badge and not necessarly the application. It would be quickier to stop the alarm than taking the phone, opening the application and deactivate the alarm.

#### ➭ **Device unlocking**
It has been decided to add an other features with the NFC such as an unlock one. After pressing the NFC against the device during two seconds it would unlock it. Moreover this feature will override the previous one. In the case of an alarm ringing it could stop the alarm and unlock the device if you press the NFC against the sensor during two seconds.

## Non-functional requirements

## Future improvements

## Out of scope

## Glossary