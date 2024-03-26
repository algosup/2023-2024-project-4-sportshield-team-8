# SportShield Technical Specifications

<details>
<summary>

## Table of Contents

</summary>

- [SportShield](#sportshield)
  - [Table of Contents](#table-of-contents)
    - [Technical](#technical)
- [Audience](#audience)
- [Deliverable](#deliverable)
  - [Details](#details)
  - [Requirements](#requirements)
  - [Nice to have](#nice-to-have)
  - [Priorities](#priorities)
- [Technical Architecture](#technical-architecture)
  - [Technology Used](#technology-used)
    - [Hardware](#hardware)
    - [Software](#software)
  - [Conventions](#conventions)
    - [Files \& Folder](#files--folder)
      - [*Naming*](#naming)
      - [*Organisation*](#organisation)
    - [GitHub](#github)
      - [*Naming*](#naming-1)
      - [*Organisation*](#organisation-1)
    - [Coding](#coding)
      - [*Naming*](#naming-2)
      - [*Comments*](#comments)
      - [*Formatting*](#formatting)
      - [*Miscellaneous*](#miscellaneous)
  - [Key Functionality](#key-functionality)
    - [Motion Detection](#motion-detection)
      - [*Organisation*](#organisation-2)
      - [*Initialization*](#initialization)
      - [*Threshold*](#threshold)
      - [*Output*](#output)
      - [*Get movement*](#get-movement)
      - [*Reference and resources*](#reference-and-resources)
    - [NFC](#nfc)
      - [*Reference and resources*](#reference-and-resources-1)
    - [GPS](#gps)
      - [*Organization*](#organization)
      - [*re-used code*](#re-used-code)
      - [*reference and resources*](#reference-and-resources-2)
    - [GPRS](#gprs)
      - [*Class organization*](#class-organization)
      - [*Power down/up*](#power-downup)
      - [*Communication*](#communication)
      - [*Re-using legacy code*](#re-using-legacy-code)
      - [*Parallel tasking*](#parallel-tasking)
      - [*Reference and resources*](#reference-and-resources-3)
    - [Buzzer](#buzzer)
      - [*Small motion*](#small-motion)
      - [*Large motion*](#large-motion)
      - [*Class organization*](#class-organization-1)
      - [*Sound loop*](#sound-loop)
      - [*Reference and resources*](#reference-and-resources-4)
    - [Bluetooth](#bluetooth)
      - [Organization](#organization-1)
      - [*setup*](#setup)
      - [*BLE event handler*](#ble-event-handler)
      - [*Sources*](#sources)
    - [Main task and low energy](#main-task-and-low-energy)
      - [*Organization*](#organization-2)
      - [*Setup*](#setup-1)
      - [*Receiving Message*](#receiving-message)
      - [*Tickless mode*](#tickless-mode)
        - [*Waking up*](#waking-up)
      - [*Sources*](#sources-1)
  - [Function Organization](#function-organization)
  - [Security](#security)
- [Glossary](#glossary)

</details>

# Technical

# Audience

**This document is primarily intended for:**

- Software developer: to understand the user and technical requirements, and to guide decision-making and planning. Help them understand risks and challenges, customer requirements, additional technical requirements and choices made.

**Secondary audiences:**

- Program manager: to validate against the functional specification and the client's expectations.
- QA: to assist in preparing the Test Plan and to use it for validating issues.
- Project manager: to help identify risks and dependencies

# Deliverable

The team is tasked with delivering firmware designed to facilitate the locking, unlocking, and alarm activation functions of the SportShield alarm system.

## Details

The system is used for up to 6 hours a day.
When not in use, only the NFC can turn the system on.
It can be locked and unlocked using Bluetooth or NFC.
The alarm is triggered upon detecting movement.

## Requirements

- Should run for 7 days without charging.
- NFC should only accept one signal.
- Bluetooth should only recognize a paired phone.
- Has to run on a nRF52840 CPU.
- Has to be programmed in Arduino C++.
- NFC should always be active.

## Nice to have

- Battery should never go below 20% and above 80%.
- Should send battery status over GPRS.

## Priorities

1. Reliability
2. Execution optimization

# Technical Architecture

## Technology Used

### Hardware

- Xiao BLE Sense nrf52840 (CPU & Bluetooth).
- GSM/2G SIM800L Module (GPRS).
- NFC antenna.
- 3.7V, 1100mAh lithium battery.
- 12V buzzer.
- 12V, 500mA Electromagnet.
- LSM6DS3 (Accelerometer).

### Software

- C++ 11.
- nRF52 Assembly.
- Arduino IDE & Compiler v2.3.3.
- list of dependencies:
  - *NRF52_MBED_TimerInterrupt V1.4.1.*
  - *adafruit-nrfutil 0.5.3* (for Linux).
  - *ArduinoBLE V1.3.6.*
  - *Sim800L HTTP connector V1.14.0.*
  - *Seeed Arduino LSM6DS3 V2.0.3.*
  - *OneWire V2.3.7.*
  - *FreeRTOS V11.0.1-5.*

Those choices were made for hardware reasons.

## Conventions

### Files & Folder

##### *Naming*

All files and folders should be named in *camelCase*.

##### *Organisation*

- Images should be in an *./image* subfolder from where they are called.
- Technical, Functional and Management documents should be in their folder in a *./documents* folder.
- The code should be in an *./src* folder from the root.

### GitHub

##### *Naming*

GitHub branches should be named in *Pascal_Snake_Case*.

##### *Organisation*

- The working version of the project goes into the *Release* branch.
- The main should be updated at least once a week.
- Release should be made from the main.
- There can't be any direct push to the main.
- A new branch should be made for each feature.
- Only push code that compiles (exception for Error related to the SIM800L).
- Issues should only be opened for problems in the *main* or *Release* branches.

### Coding

When in doubt follow [this document](https://google.github.io/styleguide/cppguide.html) as this document only highlights the most important parts.

##### *Naming*

Use clear names that describe the purpose of the object. Avoid abbreviations. Everything should be done in English.

- Variables:
  - snake_case in a function.
  - snake_case_ with an underscore at the end when in the scope of a class.
- Types and concepts (class, struct, alias, enum and type aliases): PascalCase.
- Const and enum: prefixed with ``k`` then camelCase eg. ``kConstVariable``
- Functions: camelCase.
- Namespaces: snake_case. The top-level namespace should be the project name.
- Macro: UPPERCASE_SNAKE.

##### *Comments*

Do not make comments on things that are obvious and easily readable. Focus more on the Why than the what.

All files should start with an overview of the abstraction provided in the file. \
Class comments should describe when and how to use the class. \
If a function use is not obvious it should explain its use case. If the operation is not obvious it should be explained in the definition. \
Global variables should have an explanation.

##### *Formatting*

- tabs are 4 spaces long.
- lines should be at most 80 characters long.
- avoid non-ASCII characters.
- If function parameters don't fit on a line, jump to the next line to finish declaring the parameters.
- Open curly brackets on the same line as your end parameter declaration

example :

```cpp
int example(string word, int number,
            Type parameter3){
    //do something
}
```

##### *Miscellaneous*

- don't use ``using std;`` instead do ``std::`` when needed to avoid naming conflict.
- use the ``#pragma once`` macro "include guard" on all header files
- always use namespace.
- favor pass by reference over pass by pointer
- create header files for modules that can work on their own.
- all classes should follow encapsulation.
- avoid dynamic memory allocation if possible.
- never have two tasks writing to the same object to avoid race conditions.
- never use ``sleep``, use ``vTaskDelay`` or ``vTaskDelayUntil`` in tasks. This program makes use of concurrent programming, putting the CPU to sleep will stop all the ongoing tasks and will prevent new ones from happening.

## Key Functionality

Those key functionalities are independent of one another and can be developed assuming the other works. \
Each part should be worked on as separate classes.

### Motion Detection

The LSM6DS3 motion detection module can be set to send data only when detecting movement that exceeds a certain threshold thanks to its interrupt control registers. The accelerometer should only return a signal to the GPIO pin if the movement goes past a certain threshold. This way the CPU can be kept in sleep mode the rest of the time.
The *LSM6DS3Core* class in the *Seeed Arduino LSM6DS3* library allows writing to the motion detection module's registers.

##### *Organisation*

The class should follow this organisation:

```cpp
class MotionDetection{
    float wakeup_threshold_;
    float large_movement_threshold_;
    bool active_;

public:
    //make it so the motion detector goes back to passive get.
    //return a success or failure
    bool setPassive()

    //Setup the motion detector to constantly send acceleration information
    bool setActive()

    //change threshold on the motion detection module
    bool setWakeupThreshold(float threshold)

    // the movement above which it is considered a large movement (can't be negative)
    bool setLargeMovementThreshold(float threshold)

    //constantly get the acceleration of the device to know the amplitude
    //set large_movement to true if above threshold
    uint8_t getMovement()
}
```

``getMovement`` returns are :

- 0 for a movement smaller than the ``wakeup_threshold_``
- 1 for a movement smaller than the ``large_movement_threshold_``
- 2 for a movement larger than the ``large_movement_threshold_``

use an enum to make it easier to maintain

##### *Initialization*

The setup should be done at the initialization of the CPU and whenever the motion detector is being switched to passive.

- The FIFO should be set in bypass with the ``FIFO_CTRL5`` <sub>[p.50](https://content.arduino.cc/assets/st_imu_lsm6ds3_datasheet.pdf#page=50) & [p.31](https://content.arduino.cc/assets/st_imu_lsm6ds3_datasheet.pdf#page=31)</sub> register.
- ``INT2_CTRL``<sub>[p.53](https://content.arduino.cc/assets/st_imu_lsm6ds3_datasheet.pdf#page=53)</sub> should be set to 0
- If the gyroscope ends up being ignored it should be turned off by setting ``CTRL2_G``<sub>[p.55](https://content.arduino.cc/assets/st_imu_lsm6ds3_datasheet.pdf#page=55)</sub> to ``0b0000``
- The acceleration sensor should be set to a low power mode 26Hz with ``CTRL1_XL``<sub>[p.54](https://content.arduino.cc/assets/st_imu_lsm6ds3_datasheet.pdf#page=54)</sub>

##### *Threshold*

A significant motion and til event can be read from the ``FUNC_SRC`` register. The ``SIGN_MOTION_IA`` reads 1 when a large movement is detected.
A large movement is defined in the ``SM_THS``<sub>[p.91](https://content.arduino.cc/assets/st_imu_lsm6ds3_datasheet.pdf#page=91)</sub> register for the accelerometer. Tilt threshold can not be changed and if the sensitivity proves to be too high or too low, tilt detection should be ignored.

##### *Output*

The output should be sent via the ``INT1`` pin. The interrupt 1 registers can be set to 0 for the bits we don't want to return and 1 for those we want to listen to.
The ``INT1`` pin does an ``OR`` operation of the different output from ``MD1_CFG`` and ``INT1_CTRL`` to output a boolean.
The only value we are interested in is ``INT1_TILT`` from the ``MD1_CFG``<sub>[p.80](https://content.arduino.cc/assets/st_imu_lsm6ds3_datasheet.pdf#page=80)</sub> register. So it should be set to ``0b00000010``. And ``INT1_SIGN_MOT`` from ``INT1_CTRL``<sub>[p.52](https://content.arduino.cc/assets/st_imu_lsm6ds3_datasheet.pdf#page=52)</sub> which should accordingly be set to ``0b01000000``.

##### *Get movement*

When the CPU is active, it should constantly pull data from the motion detection module using the non-core LSM6DS3 library.
The motion detection module can be set to actively send data with ``LSM6DS3.begin()`` which changes the read mode of the motion sensor to be active.

``readFloatAccelX()`` is used to read these acceleration data from the motion detector. X can be changed to Y or Z to get a different axis. The axis are added together before being compared to the threshold.

##### *Reference and resources*

- [LSM6DS3 documentation](https://content.arduino.cc/assets/st_imu_lsm6ds3_datasheet.pdf)
- [Seeed Arduino LSM6DS3 Github](https://github.com/Seeed-Studio/Seeed_Arduino_LSM6DS3): Take inspiration from the low-level example

### NFC

Unfortunately, [there is no working library to interact with the NFC](https://github.com/Seeed-Studio/wiki-documents/discussions/214?sort=new) as of 19/03/2024 as per Seed Studio.

A solution would be to use assembly and registers<sub>[p.208](https://infocenter.nordicsemi.com/pdf/nRF52840_PS_v1.7.pdf#page=208)</sub> to make the NFC work. This is not a priority.

##### *Reference and resources*

- [nRF52840 Product Specification - NFC](https://infocenter.nordicsemi.com/pdf/nRF52840_PS_v1.7.pdf#page=208)
- [Seeed Studio XIAO nRF52840 - Github Wiki](https://github.com/Seeed-Studio/wiki-documents/discussions/214)
- [Seeed Studio XIAO nRF52840 - Seeed Studio Wiki](https://wiki.seeedstudio.com/XIAO-BLE-Sense-NFC-Usage/)

### GPS

There is a GPS module on the board. It can be turned on to give the user the position of the device in low battery situations or cases of theft. Otherwise, this module should be turned off to minimize battery consumption.
The provided GPS module is the [CD-PA1010D](https://www.mouser.com/pdfDocs/CD_PA1010D_Datasheet_v03.pdf) GNSS antenna. This antenna is supported by the [Adafruit GPS library](https://github.com/adafruit/Adafruit_GPS).

##### *Organization*

Although the time to send a message could be reduced if the start-up of the SIM and GPS query were done concurrently, this would be minimal as the CPU does concurrent programming through RTOS and there are already 3 or more tasks running on a single core CPU at that moment in the execution. Code simplicity is favored instead in this instance.

```cpp
class GPS{
    Adafruit_GPS adafruit_gps_(&Serial1);
    gps_wakeup_pin_;
    gps_serial_pin_;

    latitude_;
    Longitude_;

    activateGPS();
    convertDMMtoDD();

    public:
    GPS(){
        //setup the GPS
    }

    //should return formated GPS data
    getLatitude(){
        convertDMMtoDD()
    };
    getLongitude(){
        convertDMMtoDD()
    };

    updateCoordinate(){
        activateGPS()
        //update coordinate

        //turn of GPS after use
        digitalWrite(GPS_WKUP_PIN, LOW);
    };
}
```

##### *re-used code*

For the sake of simplicity, a large part of the existing GPS interface will be reused as is or with very little direct modification. Notably replacing the macro and global variables with private variables and putting all the GPS related functions in the GPS class. The ISR related functionality is also removed as they are either no longer relevant or handled by *freeRTOS*

the existing function from ``SS_05-03_anglais-batterycontrol.ino`` should be moved to GPS:

- ``gps_setup()`` which becomes the constructor of the class.
- ``activateGPS()`` which is called to wake the GPS module up should be private
- ``convertDMMtoDD()`` is used to translate GPS return into the correct format for the server. should be private

The objects in this code snippet should be declared in private in the ``GPS`` class instead of as global variables as they are in the original code.

```cpp
Adafruit_GPS GPS(&Serial1);
#define GPS_WKUP_PIN D8
```

The ``position_acquired`` and ``start_gps`` variables are removed as they are no longer relevant to the code organization. This means that the logic checking for them should be removed from the following snippets

this code snippet from the loop becomes the ``getGPSdata()`` private function. Note that the ``GPS`` object in this snippet and the one after is the ``Adafruit_GPS`` class.

This should become the ``updateCoordinate()`` function

```cpp
GPS.read();
if (GPS.newNMEAreceived()) {
Serial.print(GPS.lastNMEA());    // this also sets the newNMEAreceived() flag to false
if (!GPS.parse(GPS.lastNMEA()))  // this also sets the newNMEAreceived() flag to false
    Serial.println("fail to parse");// we can fail to parse a   sentence in which case we should just wait for another
}

if (GPS.fix && position_acquired == false) {  // if location detected
Serial.println("fix + false");
position_acquired = true;
GPS.fix = 0;
digitalWrite(GPS_WKUP_PIN, LOW);
GPS.sendCommand("$PMTK225,4*2F");  // send to backup mode
}
```

##### *reference and resources*

- [GNSS module Data Sheet](https://www.mouser.com/pdfDocs/CD_PA1010D_Datasheet_v03.pdf)
- [Adafruit GPS library](https://github.com/adafruit/Adafruit_GPS)

### GPRS

The GSM/2G SIM800L Module will be used to send messages over the 2G network in HTTP. When not in use this module will be in power down mode to save battery. As we are unable to test this module's functionality for hardware reasons we will reuse the existing code for 2G communication.

##### *Class organization*

```cpp
class SIM{
    //if the board is turned on or off
    bool board_status_;

    //return a string formatted as {latitude, longitude, battery level}
    string writeMessage(){
        float battery; //do an analog read of PIN_VBAT

        //get data GPS from the GPS Class
        float latitude;
        float longitude;

        return "{\"latitude\": xxxx,
                \"longitude\": xxxx,
                \"batterie\": xx}"
    }

    //turn on the board
    bool turnOn(){
        //use GPIO digitalWrite to turn on the SIM800 board
    };

    //turn off the board
    bool turnOff(){
        //send AT command to turn off the SIM800 board
    };

    TaskHandle_t send_message_task_handle = NULL;

public:
    SIM()

    //no setters

    TaskHandle_t getSendMessageTaskHandle();

    //send a message
    void sendMessageTask(TaskHandle_t mainTaskHandle){
        //send the formatted message
    }
}
```

##### *Power down/up*

The AT command ``AT+CPOWD=1``<sub>[p.24](https://datasheetspdf.com/pdf-down/S/I/M/SIM800H-SIMCom.pdf#page=24) & [p.146](https://wiki.elecrow.com/images/2/20/SIM800_Series_AT_Command_Manual_V1.09.pdf#page=146)</sub> is used to power down the board when not in use. The module can then be turned back on by sending a 1-second high, followed by 2 2-second low and then a 1-second high on a GPIO pin.

##### *Communication*

When a large motion is detected the GPS position is sent over HTTP. This message should be formatted as follows to be understood by the server ``{"latitude": LATITUDE, "longitude": LONGITUDE, "batterie": BATTERY LEVEL}``. ``LATITUDE`` and ``LONGITUDE`` are floats we get from the GPS library. ``BATTERY LEVEL`` is a float between 0 & 1. This should also be sent if the battery gets below 20%.

"batterie" is not a typo, this is a French company that named their variable in French.

##### *Re-using legacy code*

We have no way to try the SIM800L as our hardware is non-functional. As such we should mostly keep the original setup and protocol:

This code snippet needs to be added at the start of the class constructor.

```cpp
sim800l = new SIM800L((Stream*)&Serial2, SIM800_RST_PIN, 200, 512);
pinMode(SIM800_DTR_PIN, OUTPUT);
```

``sim_setup()`` needs to be added after that in the constructor. The delay can be kept in place as the setup should be done before any RTOS task is scheduled.

Sending a message looks like this in the original code and should be kept as is:

```cpp
sim800l->setupGPRS("iot.1nce.net");
sim800l->connectGPRS();
String Route = "http://141.94.244.11:2000/sendNotfication/" + BLE.address();
String RouteCoord = "http://141.94.244.11:2000/updateCoordinate/" + BLE.address();
String str = "Message"
char position[200];
char posbat[200];
str.toCharArray(position, str.length() + 1);
//Serial.println(str);
bat.toCharArray(posbat, bat.length() + 1);
Serial.println(posbat);
char direction[200];
char directionCoord[200];
Route.toCharArray(direction, Route.length() + 1);
RouteCoord.toCharArray(directionCoord, RouteCoord.length() + 1);
sim800l->doPost(direction, "application/json", position, 10000, 10000);
sim800l->doPost(directionCoord, "application/json", posbat, 10000, 10000);
sim800l->disconnectGPRS();
```

##### *Parallel tasking*

While the nrf52 CPU does not support multithreading, the message should still be sent while other operations continue. This can be done using *freeRTOS* which is supported by the *nrf52 SDK*.

The task should be handled following this pseudocode

```cpp
sendMessageTask(){
    while(true){
        //wait for a message notification indefinitely using portMAX_DELAY
        if (xTaskNotifyWait(portMAX_DELAY) == true){
            //if a notification is recieved the condition is true

            //tell the main task that a message is being sent
            xTaskNotify(mainTaskHandle, 0b100)

            turnOn();
            setup();
            writeMessage();
            //send the formatted message
            turnOff();

            //tell the main task that the message has been sent
            xTaskNotify(mainTaskHandle, 0b101)
        }
        //if the xTaskNotifyWait times out, the loop restart
    }
}
```

The notification value doesn't matter.

##### *Reference and resources*

- [GMS Library - Arduino docs](https://docs.arduino.cc/retired/archived-libraries/GSM/)
- [SIM800H&SIM800L_Hardware Design_V2.02](https://www.scribd.com/document/700531402/SIM800L-datasheet)
- [SIM800 Series_AT Command Manual_V1.09](https://wiki.elecrow.com/images/2/20/SIM800_Series_AT_Command_Manual_V1.09.pdf)

### Buzzer

##### *Small motion*

If a small motion is detected (bigger than the wakeup threshold, lower than the large motion threshold), the buzzer should sound for 0.1 seconds every 2 seconds. This should stop on its own if the movement stops for 10 seconds.

##### *Large motion*

When a large movement is detected, the alarm should be for 0.1 seconds every 0.5 seconds. This should stop after 10 seconds if no large or small movement is detected. The Bluetooth should also have the option to stop the alarm by unlocking the device.

##### *Class organization*

the input from the motion detection to soundControlTask looks like this :

- small movement: ``0b01``
- large movement: ``0b11``
- no movement: no notification sent

the input from soundControlTask to digitalWriteTask looks like :

- small movement: ``0b00``
- large movement: ``0b10``
- no movement: ``0b01``

```cpp
class Buzzer{
    const float kBuzzerPin_ = D2;
    //time in s when sound is emitted in a cycle
    float large_movement_high_time_
    float small_movement_high_time_
    
    //time in s when there is no sound in a cycle
    float large_movement_low_time_
    float small_movement_low_time_

    //how long should 
    float cycle_time_ 

    //the task handle for 
    TaskHandle_t digital_write_task_handle = NULL;
    TaskHandle_t sound_control_task_handle = NULL;

public:
    //constructor
    Buzzer()
    //setter functions for movement_time and cycle_time
    setX()//...

    //getter functions for the taskHandle
    getXTaskHandle()//...

    void digitalWriteTask();
    void soundControlTask(TaskHandle_t mainTaskHandle);
}
```

##### *Sound loop*

The [electronic diagram](./image/SportShield%20-%20Electronics%20diagram%20.png) the buzzer is wired to the D2 pin. ``digitalWrite()`` is used to set the pin to High or low.

Making the time last 10 seconds can be done using freeRTOS. This is a pseudocode of how this could be implemented:

```cpp
//the function loops the sound until it receive the notification to stop
void digitalWriteTask(){
    large_movement = false
    float low_time, high_time
    while(true){
        //store the byte passed by the notification
        //clear bits on exit
        uint32_t notification = xTaskNotifyWait()

        //check if the rest of the loop should be skipped
        if(notification & 0b01 == true){
            continue
        }else{
            //check if this was a large or small movement
            if((notification & 0b10 == true) || large_movement ){
                //change the time 
                low_time = this->large_movement_low_time_
                high_time = this->large_movement_high_time_
                large_movement = true
            }else{
                low_time = this->small_movement_low_time_
                high_time = this->small_movement_high_time_
            }
        }

        // Toggle the digital pin HIGH
        digitalWrite(D2, HIGH)
        vTaskDelay(high_time)

        // Toggle the digital pin LOW
        digitalWrite(D2, LOW)
        vTaskDelay(low_time)
    }
}

//a function controlling the the 10s loop and when it should be reset
void soundControlTask(){
    while(true){
        //if a notification was returned in the last 10s 
        //check if there was movement
        uint32_t notification = xTaskNotifyWait(cycle_time_)
        if(notification & 0b01 == true){
            //tell the main task that the buzzer has restarted 
            xTaskNotify(mainTaskHandle, 0b010)

            //only send the 2nd bit as the 1st bit stop the loop
            uint32_t sentValue = notification & 0b10;
            xTaskNotify(digitalWriteTaskHandle, sentValue)
            continue; //skip the rest of the loop
        }

        //if xTaskNotifyWait times out break, the digitalWriteTask loop
        xTaskNotify(digitalWriteTaskHandle, 0b01)*
        
        //tell the main task that the buzzer has stoped 
        xTaskNotify(mainTaskHandle, 0b011)
    }
}

//function for example
void motionDetection(){
    if(large movement){
        xTaskNotify(soundControlTaskHandle, 0b11)
    }else if(small movement){
        xTaskNotify(soundControlTaskHandle, 0b01)
    }else{
        //don't send a notification let the function timeout
    }
}
```

##### *Reference and resources*

- [nRF5 FreeRTOS support](https://infocenter.nordicsemi.com/index.jsp?topic=%2Fsdk_nrf5_v17.0.2%2Ffreertos.html)
- [FreeRTOS documentation](https://www.freertos.org/Documentation/Mastering-the-FreeRTOS-Real-Time-Kernel.v1.0.pdf)
- [SportShield's electronic diagram](./image/SportShield%20-%20Electronics%20diagram%20.png)

### Bluetooth

The program will mostly reuse the existing Bluetooth code. It will be modified so that the pairing is password-protected.

##### Organization

Bluetooth should be its task.

```cpp
class Bluetooth{

    TaskHandle_t bluetooth_handler_task_handle_ = NULL;

public:
    Bluetooth(){
        //setup
    }

    TaskHandle_t getHandler();

    bluetooth_handler_task(){

        while(true){
            if(xTaskNotifyWait()){
                xTaskNotify(mainTaskHandle, task_not_ended)
                //handle message

                continue;
            }

            //send when the task end
            xTaskNotify(mainTaskHandle, Value = ((message << 32) | task_ended))
        }
    }
}
```

##### *setup*

- Use the ``ble_gap_sec_params_t`` class to set up the security of the connection. Set encryption key parameter and enable "Man in the Middle" protection.
- For the sake of simplicity, everything else from the original Bluetooth implementation should be kept. The setup should be in the constructor of the class.

##### *BLE event handler*

Create an event handler that listens to `p_ble_evt```. When the event is ``BLE_GAP_EVT_SEC_PARAMS_REQUEST`` send a reply with the security parameter. This should prompt the user to enter the passkey.

##### *Sources*

- [Arduino Blueprints](https://ecs-pw-facweb.ecs.csus.edu/~dahlquid/eee174/S2016/handouts/Labs/ArduinoLab/ArduinoInfo/Arduino%20Android%20Blueprints.pdf)
- [Bluetooth Core Specs](https://books.google.fr/books?hl=fr&lr=&id=3nCuDgAAQBAJ&oi=fnd&pg=PR7&dq=Bluetooth+Core+specification+Version+4.0&ots=rNT4oZsbn9&sig=SK5aTwJ0tB2Mz4RHhEvGAyDLYtM&redir_esc=y#v=onepage&q&f=true)
- [BLE introduction](https://elainnovation.com/en/what-is-bluetooth-low-energy/)
- [Arduino BLE reference](https://www.arduino.cc/reference/en/libraries/arduinoble/)
- [Bluetooth Documentation](https://github.com/fanqh/document/blob/master/Core_v5.0.pdf)

### Main task and low energy

The CPU should be in low energy and wake up when the BLE module finds an attempt to pair or when a GPIO interrupt happens. However, since other parts of the code are operating concurrently with freeRTOS, entering and exiting low energy mode will be a challenge.

##### *Organization*

This is a simplified organization of the class as it does not take into account any Bluetooth functions that need to be transferred from the legacy code.

```cpp
class Main{

    TaskHandle_t main_task_handle_ = NULL;

public:
    Main()

    TaskHandle_t getMainTaskHandle();

    void main_task(TaskHandle_t sound_control_task_handle,      
        TaskHandle_t send_message_task_handle,
        TaskHandle_t bluetooth_handler_task_handle);
}
```

This is what the ``main_task`` should look like

```cpp
void main_task(){
    //store the status of the other task
    //both need to be true
    bool sound_control_ended = false;
    bool send_message_ended = false;
    bool read_bluetooth_ended = false;

    //wether the device is locked or unlocked
    bool locked = false

    while(true){
        if(sound_control_ended && send_message_ended && read_bluetooth_ended){
            //sleep all task and set module to passive
            __WFI()//sleep CPU

            //on CPU wakeup set module to active
            //restart ticks
            //task_ended = false;
        }

        //check if anything was sent over bluetooth.
        //result of the message is stored in last bit
        if(xTaskNotifyWait(0) & >> 31 == true){
            //activate the electromagnet to lock/unlock the device
            digitalWrite(HIGH, magnetPin)
            vTaskDelay(1s) //locking the task here doesn't mater as the lock is the only this that is used here
            digitalWrite(LOW, magnetPin)
            locked = !locked
        }

        int movement()
        //don't start the buzzer if the device isn't locked
        if(large movement && locked){
            //tell the buzzer to start or restart the loop
            xTaskNotify(soundControlTaskHandle, large)

            //if no message is being sent, send one
            if(send_message_ended == true){
                xTaskNotify(sendMessageTaskTaskHandle)
                send_message_ended = false
            }
        }else if(small movement && locked){
            //tell the buzzer to start or restart the loop
            xTaskNotify(soundControlTaskHandle, small)
        }

        //constantly check task notify with 0 wait
        if(xTaskNotifyWait(0) == true){
            //if notification received go here

            //either depending on the value returned
            send_message_ended = notification_value
            sound_control_ended = notification_value
            read_bluetooth_ended  = notification_value
        }
    }
}

```

##### *Setup*

- set ``#define configUSE_TICKLESS_IDLE 1`` in ``FreeRTOSConfig.h`` as this is necessary to use tickless mode.
- use `nrf_gpio_cfg_sense_input` to set up the CPU to wake up on a ``HIGH`` from the motion detection module.

##### *Receiving Message*

Upon receiving a message, the lock should be opened by setting the PIN ``D3`` to high for 1s as it is the one controlling the locking mechanism.

##### *Tickless mode*

free RTOS implements a low energy consumption mode called tickless. When in tickless mode all tasks stop running and the CPU is no longer being periodically woken up. To avoid issues by stopping tasks mid-execution, tickless mode shouldn't be activated until the ``sound_control_task`` and ``send_message_task`` both confirm that they are done running.

This can be done simply by using ``xTaskNotifyWait()`` and reading the value sent :

- ``0b10x`` for the message task
- ``0b01x`` for the sound control task
- ``0b11x`` for the Bluetooth task
- ``0bx1`` if the task has ended
- ``0bx0`` if the task is running

Once both tasks have returned the confirmation that they are done and the motion detector has been put in passive mode using ``setPassive()``, tickless mode can be activated with ``xTaskGlobalSetTickless()``

Then send the ``__WFI()`` instruction in assembly to put the CPU in sleep mode.

###### *Waking up*

Once the CPU is in sleep mode it needs to be woken up. Because it was put in sleep mode through ``__WFI()`` it should wake up on any GPIO interrupt sent by the motion detector. It should also wake up on any interrupt from the BLE module. After the wakeup, the code restarts from the ``__WFI()``. Run ``xTaskResumeTick()`` to exit tickless mode.

##### *Sources*

- [Arduino Blueprints](https://ecs-pw-facweb.ecs.csus.edu/~dahlquid/eee174/S2016/handouts/Labs/ArduinoLab/ArduinoInfo/Arduino%20Android%20Blueprints.pdf)
- [Bluetooth Core Specs](https://books.google.fr/books?hl=fr&lr=&id=3nCuDgAAQBAJ&oi=fnd&pg=PR7&dq=Bluetooth+Core+specification+Version+4.0&ots=rNT4oZsbn9&sig=SK5aTwJ0tB2Mz4RHhEvGAyDLYtM&redir_esc=y#v=onepage&q&f=true)
- [BLE introduction](https://elainnovation.com/en/what-is-bluetooth-low-energy/)
- [Arduino BLE reference](https://www.arduino.cc/reference/en/libraries/arduinoble/)
- [Bluetooth Documentation](https://github.com/fanqh/document/blob/master/Core_v5.0.pdf)

## Function Organization

The standard when using free RTOS is to start the tasks in ``setup()``.

from ``setup()`` create an object for each class. Their constructor should handle the module or function setup.

Create the tasks with ``xTaskCreate()``.

start the code with ``vTaskStartScheduler()`` which will start all the classes. The logic should be entirely operated in the ``mainTask()`` from the ``Main`` class

## Security

As per the client's requirement, the firmware doesn't need to be updated. The firmware also needs to be protected from external attack through its USB port which should only enable charging.
A solution to lock the firmware, **//!\\\\ Should only be done once a final version of the firmware is approved //!\\\\**, is to remove the USB stack from the nrf52840 SDK and any library used in the firmware.
As an added layer of security, the P0.13 and P0.15 Pins should be set as GPIO pins in the program to make sure they can not be used as USB.
This will completely lock anyone from using the USB port, as the USB communication protocol wouldn't exist anymore on the device. This can not be reverted.

A reversible approach would be to create password protection on the USB port.

# Glossary

- **NFC (Near Field Communication)**: A communication protocol that enables two devices, such as a smartphone and a sensor, to communicate when they are placed close together, typically within a few centimeters.

- **Bluetooth**: A wireless technology standard for exchanging data over short distances using short-wavelength radio waves.
Firmware: Software that is embedded into hardware devices to control their functionality.

- **CPU (Central Processing Unit)**: The main component of a computer or embedded system that performs most of the processing tasks.
RTOS (Real-Time Operating System): An operating system that guarantees a certain capability within a specified time constraint, commonly used in embedded systems and applications that require real-time processing.

- **GPRS (General Packet Radio Service)**: A packet-oriented mobile data service on the 2G and 3G cellular communication systems.

- **GNSS (Global Navigation Satellite System)**: A satellite-based navigation system that provides location and time information anywhere on or near the Earth.

- **GPS (Global Positioning System)**: A satellite-based navigation system that provides location and time information anywhere on or near the Earth.

- **Interrupt**: A signal to the CPU that notifies it of an event that needs immediate attention, causing the CPU to temporarily suspend its current operation and execute a specified interrupt handler function.

- **Tickless Mode**: A power-saving mode in real-time operating systems where the system timer (tick) is disabled during idle periods to reduce power consumption.

- **GPIO (General Purpose Input/Output)**: A generic pin on a microcontroller that can be configured to either input or output digital signals.

- **BLE**: Bluetooth Low Energy, a wireless communication technology designed for short-range communication with low power consumption.

- **FreeRTOS**: A real-time operating system kernel for embedded devices, facilitating multitasking and task scheduling.

- **RTOS**: Real-Time Operating System, an operating system designed for real-time tasks and applications.

- **SDK**: Software Development Kit, a set of tools, libraries, and documentation for developing software applications.

- **UART**: Universal Asynchronous Receiver-Transmitter, a hardware communication protocol used for serial communication.

- **Encapsulation**: The bundling of data and methods that operate on that data into a single unit, typically a class in object-oriented programming.

- **Race Condition**: A situation in concurrent programming where the outcome of execution depends on the timing or interleaving of multiple threads or processes.

- **Man in the Middle (MITM) Attack**: A cyber attack where an attacker intercepts and possibly alters communication between two parties without their knowledge.

- **FIFO**: First-In, First-Out, a data structure where the first elements added are the first to be removed.

- **HTTP**: Hypertext Transfer Protocol, a protocol used for transferring hypertext requests and information on the World Wide Web.
