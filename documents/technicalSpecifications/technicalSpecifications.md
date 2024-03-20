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
- nRF52 Assembly
- Arduino IDE & Compiler v2.3.3
- list of dependency :
    - *NRF52_MBED_TimerInterrupt V1.4.1*
    - *adafruit-nrfutil 0.5.3* (for linux)
    - *ArduinoBLE V1.3.6*
    - *Sim800L http connector V1.14.0*
    - *Seeed Arduino LSM6DS3 V2.0.3*
    - *OneWire V2.3.7*
    
Those choices where made for hardware reason.

## Conventions

### Files & Folder

##### *Naming*

All files and folders should be named in *camelCase*.

##### *organization*

- Images should be in an *./image* subfolder from where they are called.
- Technical, Functional and Management documents should be in their own folder in a *./documents* folder.
- The code should be in an *./src* folder from the root.

### GitHub

##### *Naming*

GitHub branches should be named in *Pascal_Snake_Case*.

##### *organization*

- Working version of the project go to the *Release* branch.
- The main should be updated at least once a week.
- Release should be made from the main.
- There can't be any direct push to the main.
- A new branch should be made for each feature.
- Only push code that compile (exception for Error related to the SIM800L)
- Issues should only be opened for problems in the *main* or *Release* branches.

### Coding

When in doubt follow [this document](https://google.github.io/styleguide/cppguide.htm) as this part only highlight the most relevant parts.

##### *Naming*

Use clear name that describe the purpose of the object. Avoid abbreviations. Everything should be done in english.

- Variables : 
  - snake_case in a function. 
  - snake_case_ with an underscore at the end when in the scope of a class.
- Types and concepts (class, struct, alias, enum and type aliases) : PascalCamelCase
- Const and enum : prefixed with ``k`` then camelCase eg. ``kConstVariable``
- Functions : camelCase
- Namespaces : snake_case. Top level namespace should be the project name.
- Macro : UPPERCASE_SNAKE

##### *Comments*

Do not comment things that are obvious and easily readable. Focus more on the Why than the what.

All file should start with an overview of the abstraction provided in the file. 
Class comments should describe when and how to use the class
If a function use is not obvious it should explain it's use case. If the operation is not obvious it should be explained in the definition.
Global variable should have an explanation.

##### *formatting*

- tab are 4 spaces long
- lines should be at most 80 characters long.
- avoid non ASCII character
- If function parameter don't fit on a line they should wrap around
- open curly brackets on the same line as you end parameter declaration

example :
```cpp
int example(string word, int number,
            Type parameter3){
    //do something
}
```

## Key Functions

Those key function are independents from one an other and can be developed assuming the others works. 

### Motion Detection

The LSM6DS3 motion detection module can be set to send data only when detecting movement that exceed a certain threshold thanks to it's interrupt control registers. The accelerometer should only return a signal to the GPIO pin if the movement go past a certain threshold. This way the CPU can be kept in sleep mode the rest of the time.
The *LSM6DS3Core* class in the *Seeed Arduino LSM6DS3* library allow writing to the motion detection module's registers.

##### *initialization*

The setup should be done at the initialization of the CPU and whenever the motion detector is being started up.
- The FIFO should be set in bypass with the ``FIFO_CTRL5`` <sub>[p.50](https://content.arduino.cc/assets/st_imu_lsm6ds3_datasheet.pdf#page=50) & [p.31](https://content.arduino.cc/assets/st_imu_lsm6ds3_datasheet.pdf#page=31)</sub> register.
- ``INT2_CTRL``<sub>[p.53](https://content.arduino.cc/assets/st_imu_lsm6ds3_datasheet.pdf#page=53)</sub> should be set to 0
- If the gyroscope ends up being ignored it should be turned off with by setting ``CTRL2_G``<sub>[p.55](https://content.arduino.cc/assets/st_imu_lsm6ds3_datasheet.pdf#page=55)</sub> to ``0b0000``
- The acceleration sensor should be set to a low power mode 26Hz with ``CTRL1_XL``<sub>[p.54](https://content.arduino.cc/assets/st_imu_lsm6ds3_datasheet.pdf#page=54)</sub>

##### *Threshold*

A significant motion and til event can be read from the ``FUNC_SRC`` register. The ``SIGN_MOTION_IA`` read 1 when a large movement is detected.
A large movement is defined in ``SM_THS``<sub>[p.91](https://content.arduino.cc/assets/st_imu_lsm6ds3_datasheet.pdf#page=91)</sub> register for the accelerometer. Tilt threshold can not be changed and if the sensitivity prove to high or too low, tilt detection should be ignored.

##### *Output*

The output should be sent via the ``INT1`` pin. The interrupt 1 registers can be set to 0 for the bits we don't want to return and 1 for those we want to listen to.
The ``INT1`` pin does a ``OR`` of the different output from ``MD1_CFG`` and ``INT1_CTRL`` to output a boolean.
The only value we are interested in is ``INT1_TILT`` from the ``MD1_CFG``<sub>[p.80](https://content.arduino.cc/assets/st_imu_lsm6ds3_datasheet.pdf#page=80)</sub> register. So it should be set to ``0b00000010``. And ``INT1_SIGN_MOT`` from ``INT1_CTRL``<sub>[p.52](https://content.arduino.cc/assets/st_imu_lsm6ds3_datasheet.pdf#page=52)</sub> which should accordingly be set to ``0b01000000``.

##### *reference and resources*

- [LSM6DS3 documentation](https://content.arduino.cc/assets/st_imu_lsm6ds3_datasheet.pdf)
- [Seeed Arduino LSM6DS3 Github](https://github.com/Seeed-Studio/Seeed_Arduino_LSM6DS3): Take inspiration from the low level example

### NFC

The NFC antenna needs to be in active mode as we need it to power a passive device (in this case an NFC card). This has the inconvenience of increasing power draw, and means that the CPU can not be turned off while listening for an input.
Unfortunately [There is no working library to interact with the NFC](https://github.com/Seeed-Studio/wiki-documents/discussions/214?sort=new) as of 19/03/2024 as per seed studio.

A solution would be to use assembly assembly registers<sub>[p.208](https://infocenter.nordicsemi.com/pdf/nRF52840_PS_v1.7.pdf#page=208)</sub> to make the NFC work. 

##### *reference and resources*

- [nRF52840 Product Specification - NFC](https://infocenter.nordicsemi.com/pdf/nRF52840_PS_v1.7.pdf#page=208)
- [Seeed Studio XIAO nRF52840 - Github Wiki](https://github.com/Seeed-Studio/wiki-documents/discussions/214)
- [Seeed Studio XIAO nRF52840 - Seeed Studio Wiki](https://wiki.seeedstudio.com/XIAO-BLE-Sense-NFC-Usage/)

<!--https://forum.seeedstudio.com/t/xiao-ble-nfc-doesnt-work/264543/14 -->>

### Bluetooth

### GPRS

### Battery Optimization


<!-- LSM6DS3Core imu(I2C_MODE, 0x6A); 


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

Install Arduino IDE -->