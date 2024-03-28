<h1 align="center"> SportShield - Test Plan </h1>

<p align="center">
Created by: Thibaud MARLIER <br> Creation Date: 25/03/2024 <br> Last update: 25/03/2024
</p>

___

<details>
<summary>

## Table of Contents

</summary>

- [Table of Contents](#table-of-contents)
- [I. Introduction](#i-introduction)
  - [a. Overview](#a-overview)
  - [b. Targeted Audience](#b-targeted-audience)
    - [➭ Primary Audience](#-primary-audience)
    - [➭ Secondary Audience](#-secondary-audience)
  - [c. Hardware/Software Used](#c-hardwaresoftware-used)
    - [➭  Hardware ](#--hardware-)
    - [➭  Software ](#--software-)
- [II. Test Strategy](#ii-test-strategy)
  - [a. Quality Assurance Team](#a-quality-assurance-team)
  - [b. Testing Scope](#b-testing-scope)
  - [c. Testing Environment](#c-testing-environment)
  - [d. Test Tool](#d-test-tool)
    - [➭ 1. GitHub Actions](#-1-github-actions)
    - [➭ 2. GitHub Projects](#-2-github-projects)
    - [➭ 3. GitHub Issues](#-3-github-issues)
  - [e. Test Cases](#e-test-cases)
  - [f. Unit Tests](#f-unit-tests)
  - [g. Bug Reports](#g-bug-reports)
    - [➭  By Users ](#--by-users-)
    - [➭  By QAs ](#--by-qas-)
- [III. Test Criteria](#iii-test-criteria)
- [IV. Plan Test Environment](#iv-plan-test-environment)
- [V. Schedule](#v-schedule)
- [VI. Test Deliverables](#vi-test-deliverables)
- [VII. Conclusion \& Acknowledgments](#vii-conclusion--acknowledgments)
- [VIII. Glossary](#viii-glossary)
- [IX. Sources](#ix-sources)

</details>

## I. Introduction

### a. Overview

Our team was tasked with upgrading and maintaining an already existing prototype device. This device, made by CORIS Innovation, is an anti-theft connected device. \
We were asked to bring our expertise to the device to improve the overall quality, security, and usability of the device. \
We have to improve the battery's lifespan and optimise its performance, manage the device using the NFC technology, allow multi-threading, and improve the security of the device.

This document will serve to set all the tools and define the strategy for testing the product to make sure that our refurbished device complies with the client's requirements.

### b. Targeted Audience

#### ➭ Primary Audience

This document is primarily intended for:

- **Quality Assurance**: to comprehend what should be tested, how, the expected result and why. To make sure that the device complies with the client's requirements. To define the stakes of testing and guide the team through tests. To then improve the quality of the final product.

#### ➭ Secondary Audience

- **Future testers**: to understand our team's testing strategy, what we have tested, how, and the final result. To apply our testing strategy if it complies with what they are testing. To have an insight into our testing strategies to keep on upgrading the product.

### c. Hardware/Software Used

#### ➭ <ins> Hardware </ins>

| Hardware                | Details                                                                                 | Picture |
|-------------------------|-----------------------------------------------------------------------------------------|---------|
| **Xiao BLE Sense nrf52840** | Small Bluetooth programmable card with a built-in antenna. <br>  Bluetooth Low Energy.  | <img src="https://github.com/algosup/2023-2024-project-4-sportshield-team-8/blob/main/documents/images/hardware/xiaoBLESense.png" alt="drawing" width="75"/>|
| **GNSS PA1010D**           | GPS Module with a built-in antenna. <br> Gives real-time location. <br> **Power Supply:** VCC：3.0V to 4.3V；VBACKUP：2.0V to 4.3V,  <br> **Consumption:** Acquisition: 36mA, Tracking: 28mA                      |<img src="https://github.com/algosup/2023-2024-project-4-sportshield-team-8/blob/main/documents/images/hardware/GNSSPA1010D.png" alt="drawing" width="90"/>|
| **GSM/2G SIM800L Module**   | Mini GSM Module. <br> Provides 2G data.                                                 |<img src="https://github.com/algosup/2023-2024-project-4-sportshield-team-8/blob/main/documents/images/hardware/SIM800L.png" alt="drawing" width="100"/>|
| **1NCE SIM Card**           | Multi-network and multi-mode SIM card. <br>                                             |<img src="https://github.com/algosup/2023-2024-project-4-sportshield-team-8/blob/main/documents/images/hardware/SIMCard.png" alt="drawing" width="100"/>|
| **Electromagnet**           | Is used to unlock the device.                                                           |<img src="https://github.com/algosup/2023-2024-project-4-sportshield-team-8/blob/main/documents/images/hardware/electromagnet.png" alt="drawing" width="75"/>|
| **Piezoelectric buzzer**    | Buzzer with an oscillator control circuit.                                                 |<img src="https://github.com/algosup/2023-2024-project-4-sportshield-team-8/blob/main/documents/images/hardware/piezoelectricBuzzerCircuit.png" alt="drawing" width="100"/>|
| **Lithium-Polymer battery** | A 3.7V, 1100mAh, 4.1Wh Lithium-Polymer powered battery. It has an estimated lifespan of around 300 to 500 charge cycles.|<img src="https://github.com/algosup/2023-2024-project-4-sportshield-team-8/blob/main/documents/images/hardware/lithiumPolymerBattery.png" alt="drawing" width="100"/>
| **NFC Antenna**             | NFC module. Provides NFC compatibility.                                                 |<img src="https://github.com/algosup/2023-2024-project-4-sportshield-team-8/blob/main/documents/images/hardware/NFCAntennaCompatibility.png" alt="drawing" width="75"/>|

#### ➭ <ins> Software </ins>

- C++ 11.
- nRF52 Assembly.
- Arduino IDE & Compiler v2.3.3.
- list of dependencies:
  - NRF52_MBED_TimerInterrupt V1.4.1.
  - adafruit-nrfutil 0.5.3 (for Linux).
  - ArduinoBLE V1.3.6.
  - Sim800L HTTP connector V1.14.0.
  - Seeed Arduino LSM6DS3 V2.0.3.
  - OneWire V2.3.7.
  - FreeRTOS V11.0.1-5.

## II. Test Strategy

### a. Quality Assurance Team

The testing team will be divided into three. Our team's, Team seven's and Team five's Quality Assurance. A collaboration has been established among the three teams for testing purposes and the project's well-being.

Our team's QA (Team 8):

- MARLIER Thibaud (Windows)

Team Seven's QA:

- KAKAL Mathis (MacOS)

Team Five's QA:

- PREVOST Antoine (Windows)

However, we have the possibility of having a Linux environment as BERNARD Max has such an Operating System. It will be useful to widen the range of the testing environment.

### b. Testing Scope

The main things that will be tested and quality checked during this project will be:

- The documents
- The software and how it behaves with the hardware

It is necessary to check the quality of the document to ensure that the right message has been conveyed through the document, ensuring the communication is effective is paramount in such a project. \
We will be checking the documents for grammatical and typographical mistakes, as well as their content.
Such checking prevents us from missing important points and ensures the communication and understanding of the document will be effective. To gain time and prevent mistakes, errors and potential threats for the product.

Concerning the software and the hardware, we will be testing the features that have been defined and are featured in both functional and technical specifications. Such features will be tested to make sure that they comply with the client's requirements, and that they are working as expected. We will try to replicate as closely as possible the environment that the final product will be in.

### c. Testing Environment

To prove that our solution is effective and respects the client's requirements, we have to put the device in the condition it will be in on the terrain. However, the anti-theft device is currently aimed at a mountain environment. The client wants to widen the range of use of the device, to a rural and more diverse audience. \
We assume that the final environment of the device will be a rural environment in a temperate climate.
We are lucky to be in such an environment here in France. Meaning that the testing environment will be easy to replicate.
However, we cannot test the device in a mountain environment, so will assume that if it works in a temperate climate, it could be working in a mountain environment, but we cannot ensure it.

### d. Test Tool

#### ➭ 1. GitHub Actions

#### ➭ 2. GitHub Projects

#### ➭ 3. GitHub Issues

### e. Test Cases

### f. Unit Tests

### g. Bug Reports

#### ➭ <ins> By Users </ins>

#### ➭ <ins> By QAs </ins>

## III. Test Criteria

## IV. Plan Test Environment

## V. Schedule

## VI. Test Deliverables

## VII. Conclusion & Acknowledgments

## VIII. Glossary

## IX. Sources
