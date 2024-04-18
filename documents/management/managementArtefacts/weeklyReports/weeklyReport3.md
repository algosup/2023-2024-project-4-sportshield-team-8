<h1 align="center"> Weekly Report 3 </h1>

<p align="center">
Created by: Quentin CLÉMENT <br> Creation Date: 05/04/2024 <br> Last update: 08/04/2024
</p>

<details>
<summary>

## Table of Contents

</summary>

- [Table of Contents](#table-of-contents)
- [1. Overall project status summary](#1-overall-project-status-summary)
- [2. Milestones met this week](#2-milestones-met-this-week)
- [3. Timeline status](#3-timeline-status)
- [4. Resources status](#4-resources-status)
- [5. Scope status](#5-scope-status)
- [6. Work accomplished](#6-work-accomplished)
- [7. Next steps](#7-next-steps)
- [8. Anticipated risks](#8-anticipated-risks)
- [9. Conclusion](#9-conclusion)

</details>

## 1. Overall project status summary

25/03/2024 - 29/03/2024

During this week, we started to dig deeper into the development of the project. We are finally finding a constant rythm even though we are still struggling with the hardware. The technical specifications v1.0 were delivered on Tuesday the 26th and we are now working on the software part. We are a bit late on the schedule but we are confident that we will catch up.

## 2. Milestones met this week

- ✅ Technical Specifications

## 3. Timeline status

Following the hardware problems met during the three first weeks, we are a late on schedule and conscious we will probably not be able to deliver the project on time without reducing the scope. We are going to discuss this with the client next week.

## 4. Resources status

After further investigations, here are the status of the hardware modules:
- ❌ SIM Module: we don't succeed to make it work but we are not sure if it's a hardware or software problem.
- ❌ PCB: we tested the PCB with a multimeter and we found voltage disparity between ports that is probably the symptom of a short circuit.
- ❌ NFC: the NFC module is working at the hardware level but unfortunately there is no library compatible with our CPU architecture meaning that we can only use it as an emitter and it's making it useless.
- ✅ Battery
- ✅ Bluetooth
- ✅ GPS
- ✅ Buzzer
- ✅ Electro-magnet: the hardware in itself is working but we can't supply it with enough power to make it work properly.

## 5. Scope status

Improve battery optimisation in the software, as well as connection via Bluetooth. The NFC will be removed from the scope as it is not compatible with our architecture. We prefer to have few features working well than many features working poorly or not at all.

## 6. Work accomplished

➭ <ins>Documents<ins>:
- ✅ Technical Specifications v1.0: **Max BERNARD, Maxime THIZEAU, Thibaud MARLIER**
- ✅ Test Plan v0.1: **Thibaud MARLIER**
- ✅ User Manual v0.1: **Victor LEROY**
- ✅ Risc Matrix: **Quentin CLÉMENT**

<br>

➭ <ins>Hardware<ins>:
- ✅ Equipment final check-up: **Max BERNARD**

➭ <ins>Software<ins>:
- ✅ Consumption optimisation: **Max BERNARD, Thomas PLANCHARD**

## 7. Next steps

- 📝 Continue writing the user manual: **Victor LEROY**
- 📝 Continue writing the test plan: **Thibaud MARLIER**
- 📱 Bluetooth unlocking: **Max BERNARD, Thomas PLANCHARD**

## 8. Anticipated risks

- 😴 Laziness among team members
- ⏰ Incomplete document delivery
- 💻 Inaccurate scope due to lack of time and faulty equipment

## 9. Conclusion

We are finally fixed on the project's scope and can focus on documents and the softwares' features. The documents are progressing well, but we're making slow progress on the software part. We will not deliver the original scope on time.