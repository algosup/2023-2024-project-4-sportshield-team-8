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
  - [c. Testing Type](#c-testing-type)
  - [d. Assumptions](#d-assumptions)
    - [1.Requirements Analysis](#1requirements-analysis)
    - [2.System Design](#2system-design)
    - [3.Implementation (Coding)](#3implementation-coding)
    - [4.Integration Testing](#4integration-testing)
    - [5.System Testing](#5system-testing)
    - [6.User Acceptance Testing (UAT)](#6user-acceptance-testing-uat)
    - [7.User Feedback](#7user-feedback)
  - [d. Testing Environment](#d-testing-environment)
  - [e. Test Tools](#e-test-tools)
    - [➭ 1. GitHub Actions](#-1-github-actions)
    - [➭ 2. GitHub Projects](#-2-github-projects)
    - [➭ 3. GitHub Issues](#-3-github-issues)
  - [f. Test Cases](#f-test-cases)
  - [g. Unit Tests](#g-unit-tests)
  - [h. Bug Reports](#h-bug-reports)
    - [➭  By Users ](#--by-users-)
    - [➭  By QAs ](#--by-qas-)
  - [i. Bug Data Report](#i-bug-data-report)
- [III. Test Criteria](#iii-test-criteria)
  - [a. Suspension Criteria](#a-suspension-criteria)
    - [➭  Critical Path Incomplete Tasks ](#--critical-path-incomplete-tasks-)
    - [➭  Critical Bug Incidents ](#--critical-bug-incidents-)
    - [➭  High Volume of Bugs ](#--high-volume-of-bugs-)
    - [➭  Environmental Challenges ](#--environmental-challenges-)
    - [➭  Stakeholder Directives ](#--stakeholder-directives-)
  - [b. Entry Criteria](#b-entry-criteria)
  - [c. Exit Criteria](#c-exit-criteria)
    - [➭  Client's Reliability Indicators ](#--clients-reliability-indicators-)
    - [➭  Document's Completion and Quality ](#--documents-completion-and-quality-)
- [IV. Plan Test Environment](#iv-plan-test-environment)
- [V. Schedule](#v-schedule)
- [VI. Test Deliverables](#vi-test-deliverables)
    - [➭  Test Plan](#--test-plan)
    - [➭  Test Cases ](#--test-cases-)
    - [➭  Bug Reports  ](#--bug-reports--)
    - [➭  Bug Data Report ](#--bug-data-report-)
- [VII. Glossary](#vii-glossary)
- [VII. Sources](#vii-sources)

</details>

## I. Introduction

### a. Overview

Our team was tasked with upgrading and maintaining an already existing prototype device. This device, made by CORIS Innovation, is an anti-theft connected device. \
We were asked to bring our expertise to the device to improve the overall quality, security, and usability of the device. \
We have to improve the battery's lifespan and optimize its performance, manage the device using the NFC technology, allow multi-threading, and improve the security of the device.

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

Each team will run their own Testing Strategies and will use their own Test Plan document while testing.
Whenever a product is tested by a team their own strategy will be applied for testing.
I'd recommend you to check [Team 5's](https://github.com/algosup/2023-2024-project-4-sportshield-team-5/blob/main/documents/quality-assurance/test-plan.md) and [Team 7's]() Test Plan

### b. Testing Scope

The main things that will be tested and quality checked during this project will be:

- The documents
- The software and how it behaves with the hardware

It is imperative to thoroughly assess the quality of the documentation to guarantee that it effectively conveys the intended message. Ensuring clear and effective communication is crucial in any project of this nature. \
Our review process will encompass scrutinizing the documents for grammatical and typographical errors, as well as evaluating the content for accuracy and completeness. This meticulous examination helps mitigate the risk of overlooking essential points and ensures that the document facilitates effective communication and understanding. By proactively addressing errors and potential threats, we aim to optimize efficiency and safeguard the integrity of the product.

Regarding both the software and hardware components, our testing focus will be on the features outlined in the functional and technical specifications. \
These features will undergo rigorous testing to ensure alignment with the client's requirements and to verify their functionality as intended. Our objective is to replicate the final product's environment as closely as feasible during testing.

### c. Testing Type

Our team has opted for the waterfall testing strategy, which follows a linear and sequential approach to software testing. This method is structured into distinct phases, with each phase being completed before progressing to the next. The waterfall model is best suited for projects with well-defined requirements and a fixed scope.
Here's an overview of the phases:

### d. Assumptions

For this project, we depend on the hardware provided by the client, which has undergone testing prior to the commencement of our project and has been confirmed to be in a functional state. \
Consequently, we operate under the assumption that the hardware does not introduce any issues, as we have confidence in its reliability based on the client's assurances.

In the event that problems arise, any bugs or issues can only be attributed to our team's actions, as we acknowledge our responsibility for the software's performance. \
Additionally, we recognize that any potential hardware malfunctions could result from mishandling on our part. \
Therefore, we are committed to ensuring the integrity of both the software and the hardware throughout the project's lifecycle.

#### 1.Requirements Analysis

Testing begins by thoroughly understanding the requirements outlined during the software development's requirements analysis phase. \
Testers carefully review these requirements to develop comprehensive test cases and test plans.

#### 2.System Design

During this phase, testers create test cases based on the system design documents. \
Additionally, they may formulate test data and plans for setting up the testing environment.

#### 3.Implementation (Coding)

Once the development team finishes coding, testing activities focus on unit testing. This involves testing individual components or modules in isolation. This part will principally be handled by the development team.

#### 4.Integration Testing

Testers verify that the individual modules integrate seamlessly and function correctly together. This ensures that interactions between different modules do not lead to unexpected behavior.

#### 5.System Testing

After integration testing, the entire system undergoes comprehensive testing. Testers assess whether the software meets the specified requirements and operates correctly across various scenarios.

#### 6.User Acceptance Testing (UAT)

In the final phase, users or stakeholders test the software in a real-world environment to validate that it fulfills their needs and expectations. This testing typically occurs before the software is released into production.

#### 7.User Feedback

Once the software is released, users provide feedback on its performance and usability. This feedback is crucial for identifying areas that require improvement and enhancing the software's overall quality.

<br>

This model then loops if a new bug is found or if a new feature is added to the software. The testing team will then have to go back to the requirements analysis phase and start the process again. It might be a long process but it is the most efficient way to ensure the quality of the product.

### d. Testing Environment

To validate the effectiveness of our solution and ensure alignment with the client's requirements, our team must subject the device to conditions resembling its intended usage environment. \
Presently, the anti-theft device is optimized for mountainous terrain. However, the client seeks to broaden its applicability to encompass a wider demographic, including rural areas. As such, we will transition the testing environment to simulate a rural setting within a temperate climate. \
Fortunately, our team is situated in France, providing convenient access to such conditions for replication. While we may not be able to directly test the device in a mountainous environment, we will operate under the assumption that its performance in a temperate climate will correlate with functionality in mountainous regions, although this cannot be guaranteed.

### e. Test Tools

#### ➭ 1. GitHub Actions

GitHub Actions are employed for continuous integration and deployment, ensuring that tests are automatically executed with each push to the main branch. Additionally, during the development phase, the workflow is triggered on each push to the dev branch. \
This setup enables us to identify any functional issues in the code, pinpoint when they occurred, and determine which changes led to them.

#### ➭ 2. GitHub Projects

GitHub Projects will be utilized to compile a comprehensive list of test cases along with their current status. This platform will act as a repository for unit tests, providing visibility into what requires testing, the methodologies involved, and the expected outcomes. \
Leveraging GitHub Projects will significantly aid in streamlining the testing process and enhancing the overall quality of the product

#### ➭ 3. GitHub Issues

GitHub Issues serves as an integrated issue-tracking tool, facilitating the creation and management of issues while enabling assignment to team members. Primarily, Quality Assurance is responsible for generating GitHub Issues for every problem identified within the product.

### f. Test Cases

To ensure every feature and all possible bugs have been tested and mitigated, test cases are created and stored in GitHub Projects.

They are defined and classed on different feature names corresponding to the device and the project.
The different names are the following:

- NFC
- Battery
- Bluetooth
- Trigger Mode
- Lock Mode
- Unlock Mode
- Software
- Alarm
- Low Battery Management
- Notifications
- GPRS
- Motion Detection
- GPS
- Security

The test cases would follow the template described hereunder:

```
{Name Of The Feature}:
  {Name Of The Test Case 1}{Failed/Passed}{Error Related}{Testing Status}{Priority}{Test Size}{Start Date}{End Date}
  {Name of the test case 2}{Failed/Passed}{Error Related}{Testing Status}{Priority}{Test Size}{Start Date}{End Date}
```

![Test case example](https://github.com/algosup/2023-2024-project-4-sportshield-team-8/blob/Documents/documents/qualityAssurance/img/testCases.png)

Moreover, when the name of the test case is clicked, a new window opens with more information such as:

- Description of the test;
- Pre-conditions;
- steps to realise it;
- Expected result.

It would follow the next template:

```
{Name Of The Test Case}:
  ## Description
    *text*
  ## Pre-conditions
    *list*
  ## Steps
    *list*
  ## Expected Result
    *text*
```

The test cases are divided into many scroll-down menus and can be organised according to the one you want to see. The scroll-down menus are composed of labels, they are all defined in the table below.

|Failed/Passed|Error Related|Testing Status|Priority|Test Size|
|-------------|-------------|--------------|--------|---------|
|- Testing    |- Syntax     |- Done        |- Low   |- S      |
|- Passed     |- Hardware   |- Doing       |- Medium|- M      |
|- Failed     |- Software   |- Will Do     |- High  |- L      |
|             |- Hardware/Software|- Won't Do|- Critical|- XL |
|             |- None       |- Pending     |        |- XXL    |

### g. Unit Tests

Unit tests play a crucial role in validating the functionality of individual components or modules within the software. These tests are conducted in isolation to confirm that each unit behaves as expected.

In our project, unit tests are developed using C++ and executed using the Arduino IDE, which aligns with the language and platform driving the project forward.

### h. Bug Reports

#### ➭ <ins> By Users </ins>

Once the product hits the market, one of the most crucial testing phases kicks off. The product will face real-world conditions, potentially revealing bugs or issues that our team might have missed during development. \
To address these challenges, we'll provide a link to our GitHub repository in the User Manual. This link will grant users access to the "Issues" section, where they can review known issues and submit new ones if necessary. \
We've prepared a bug report template, which we'll outline in detail later in this document. \
Gathering extensive feedback from users once the product is launched is of utmost importance. This feedback will play a crucial role in identifying and rectifying errors within the product, ultimately enhancing its efficiency for future iterations.

Communication between bug reporters and our QA team will be facilitated through the comments section within each issue. This allows our team to request additional information if necessary, expanding our troubleshooting capabilities. \
Once the bug and its solution have been clearly defined by both the user and the QA team, it becomes the responsibility of the development team to rectify and eliminate bugs from the program.

#### ➭ <ins> By QAs </ins>

When a bug is identified by the Quality Assurance team, they will promptly generate a GitHub issue within the project's repository. \
This practice streamlines the developer's workflow by furnishing them with a comprehensive and articulate description of the bug. \
From the title to the step-by-step instructions, clarity and precision are paramount to ensure the developer team fully grasps the nature of the issue and the necessary actions for resolution. \
This meticulous approach aims to enhance the overall quality of the product. \
To facilitate this process, a standardised template has been devised to optimise the quality and simplicity of bug reports. \
It can be found [here](https://github.com/algosup/2023-2024-project-4-sportshield-team-8/blob/main/.github/ISSUE_TEMPLATE/bugReport.yml).

Here's what the template looks like when opening a new Bug report related issue:

![Bug Report Template](https://github.com/algosup/2023-2024-project-4-sportshield-team-8/blob/Documents/documents/qualityAssurance/img/bugReport.png)

After the resolution of the bug, the development team is responsible for updating the initial bug report to indicate that it has been fixed. \
Following verification and confirmation of the bug fix's effectiveness, the issue will be closed. \
This systematic approach ensures that all stakeholders are informed of the bug's status and promotes transparency throughout the resolution process. \
But is also a way to keep track of the progress and to ensure the quality of the product.

### i. Bug Data Report


## III. Test Criteria

### a. Suspension Criteria

#### ➭ <ins> Critical Path Incomplete Tasks </ins>

#### ➭ <ins> Critical Bug Incidents </ins>

#### ➭ <ins> High Volume of Bugs </ins>

#### ➭ <ins> Environmental Challenges </ins>

#### ➭ <ins> Stakeholder Directives </ins>

### b. Entry Criteria

Entry criteria are prerequisites that must be fulfilled before testing commencement. They guarantee the testing process's effectiveness and efficiency. \
These criteria necessitate clear definition, team consensus, and understanding among testers to ensure seamless execution.

The specified criteria include:

- Conclusion of the coding phase (for a feature);
- Development, implementation, and execution of unit tests;
- Establishment of a functional and specified test environment and strategy;
- Defined and clear test cases.

### c. Exit Criteria

Exit criteria are the predefined conditions that signify when testing activities can be concluded. \
They serve as benchmarks to assess whether the testing objectives have been met and whether the software system is ready for the next phase or release. \
By adhering to these criteria, we ensure that testing is terminated without compromising the quality or efficacy of the software.

Before we can finalize the testing phase and prepare the product for distribution, certain critical objectives must be met.

#### ➭ <ins> Client's Reliability Indicators </ins>

- Shock detection accuracy: 99%
- Detection level accuracy (small or major): 99%
- Unlock: 99%
- Alarm: 98%
- Shock notification: 95%
- GNSS position acquisition and transmission: 80%

**Battery operating time between two charges**

- 7 days operating duration
- Considering 6 hours in activation mode

#### ➭ <ins> Document's Completion and Quality </ins>

- Documents: 100% completion
  - Functional Specifications
  - Technical Specifications
  - User Manual
  - Test Plan
  - Management Documents

- The documentation should be clear and concise, comprehensible to external parties involved in and out of the project, whether technical or non-technical. Furthermore, the document should maintain consistency in terms of format throughout.



## IV. Plan Test Environment

1. OS WINDOWS MAC LINUX
2. SERVEUR voir doc d'Antoine
3. IDE ARDUINO VERSION
4. Language + librairies
5. HARDWARE
6. App SportShield + my phone spec

## V. Schedule

| Task                              | Start date     | End date       | Estimated duration |
| --------------------------------- | -------------- | -------------- | ------------------ |
| Issue and pull request templates  | March 11, 2024 | March 11, 2024 | 1 day              |
| Functional Specifications review  | March 15, 202  | March 18, 2024 | 2 days             |
| Technical Specifications review   | March 21, 2024 | March 26, 2024 | 3 days             |
| Test Plan Creation                | March 22,2024  | April 10, 2024 |  19 days           |
| Github Actions                    | March 26, 2024 | April 4, 2024  | 9 days             |
| Test Cases Definition             | March 26, 2024 | March 29, 2024 | 3 days             |
| First meeting with team 5 & 7 QA  | April 2, 2024  | April 2, 2024  | 1 day              |
| Second meeting with team 5 & 7 QA | April 8, 2024  | April 8, 2024  | 1 day              |
| User Manual review                | March 8, 2024  | March 10, 2024 | 2 days             |

## VI. Test Deliverables

#### ➭ <ins> Test Plan</ins>

define the testing strategy and how the tests will be done.

#### ➭ <ins> Test Cases </ins>

define all the test that will be done to be sure that the game works as intended.

#### ➭ <ins> Bug Reports  </ins>

Bug reports: report all the bugs that have been found during the tests.

#### ➭ <ins> Bug Data Report </ins>

Bug data report: to have a global view of the bugs that have been found during the tests.

## VII. Glossary

## VII. Sources
