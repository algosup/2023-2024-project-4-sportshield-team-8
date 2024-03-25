# SportShield

### Technical

# Audience

**This document is primarily intended for :**

- Software developer: to understand the user and technical requirements, and to guide decision-making and planning. Help them understand risks and challenges, customer requirements, additional technical requirements and choices made.

**Secondary audiences :**

- Program manager: to validate against the functional specification and the client expectations.
- QA: to assist in preparing the Test Plan and to use it for validating issues.
- Project manager: to help identify risks and dependencies

# Deliverable

The team has to deliver firmware to lock, unlock and ring the alarm of the SportShield alarm system.

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
- Technical, Functional and Management documents should be in their own folder in a *./documents* folder.
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
- never use ``sleep``, use ``vTaskDelay`` or ``vTaskDelayUntil`` in tasks. This program makes use of concurrent programming and sleep would block the CPU for all tasks.

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

Unfortunately, [There is no working library to interact with the NFC](https://github.com/Seeed-Studio/wiki-documents/discussions/214?sort=new) as of 19/03/2024 as per Seed Studio.

A solution would be to use assembly and registers<sub>[p.208](https://infocenter.nordicsemi.com/pdf/nRF52840_PS_v1.7.pdf#page=208)</sub> to make the NFC work. This is not a priority.

##### *Reference and resources*

- [nRF52840 Product Specification - NFC](https://infocenter.nordicsemi.com/pdf/nRF52840_PS_v1.7.pdf#page=208)
- [Seeed Studio XIAO nRF52840 - Github Wiki](https://github.com/Seeed-Studio/wiki-documents/discussions/214)
- [Seeed Studio XIAO nRF52840 - Seeed Studio Wiki](https://wiki.seeedstudio.com/XIAO-BLE-Sense-NFC-Usage/)

### Bluetooth

##### *Bluetooth Low Energy*

To communicate, the hardware uses the Bluetooth Low Energy technology, which consumes 1/10th of the original Bluetooth's energy.
The Bluetooth connection is always in sleep mode, except once a connection is established.

To initialize the BLE, we use:

```
MODE=Ieee802154_250kbit 
```

The radio module will comply with the [IEEE 802.15.4-2006 standard](https://en.wikipedia.org/wiki/IEEE_802.15.4).

##### *Linked to MAC addresses*

After pairing two devices, the connectivity is made via linking the MAC address of both devices, meaning that they are bound together. Such MAC address can be queried by the and kept in memory by the hardware.

```
bleDevice.address() // Query the Bluetooth® address of the Bluetooth® Low Energy device.
```

And then can be stored into a variable.

##### *Security*

Security shall be initiated by the Security Manager in the device in the master
role. The device in the slave role shall be the responding device. The slave
device may request the master device to initiate pairing or other security
procedures.

<!-- Moreover, it is possible in some way (no clue how yet), to generate a password to pair two devices, we need to dig more into that to specify how it works.  -->

##### *Sources*

- [Arduino Blueprints](https://ecs-pw-facweb.ecs.csus.edu/~dahlquid/eee174/S2016/handouts/Labs/ArduinoLab/ArduinoInfo/Arduino%20Android%20Blueprints.pdf)
- [Bluetooth Core Specs](https://books.google.fr/books?hl=fr&lr=&id=3nCuDgAAQBAJ&oi=fnd&pg=PR7&dq=Bluetooth+Core+specification+Version+4.0&ots=rNT4oZsbn9&sig=SK5aTwJ0tB2Mz4RHhEvGAyDLYtM&redir_esc=y#v=onepage&q&f=true)
- [BLE introduction](https://elainnovation.com/en/what-is-bluetooth-low-energy/)

[Arduino BLE reference](https://www.arduino.cc/reference/en/libraries/arduinoble/)

[Bluetooth Documentation](https://github.com/fanqh/document/blob/master/Core_v5.0.pdf)

### GPS

There is a GPS module on the board. It can be turned on to give the user the position of the device in low battery situation or in cases of theft. Otherwise this module should be turned off as to minimize battery consumption.
The provided GPS module is the [CD-PA1010D](https://www.mouser.com/pdfDocs/CD_PA1010D_Datasheet_v03.pdf) GNSS antenna. This antena is supported by the [Adafruit GPS library](https://github.com/adafruit/Adafruit_GPS).

##### *Organization*

Although the time to send a message could be reduced if the start up of the SIM and GPS query were done concurrently, this would be minimal as the CPU does concurrent programming through RTOS and there are already 3 or more task running on a single core CPU at that moment in the execution. Code simplicity is favored instead in this instance.

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
For the sake of simplicity, a large part of the existing GPS interface will be reused as is or with very little direct modification. Notably replacing the macro and global variable with private variable and putting all the GPS related function in the GPS class. The ISR related functionality are also removed as they are either no longer relevant or handled by *freeRTOS*

the existing function from ``SS_05-03_anglais-batterycontrol.ino`` should be moved to GPS :
- ``gps_setup()`` which becomes the constructor of the class.
- ``activateGPS()`` which is called to wakeup the GPS module should be private
- ``convertDMMtoDD()`` is used to translate GPS return into the corect format for the server. should be private

The objects in this code snipet should be declared in private in the ``GPS`` class instead of as global variable as they are in the original code.
```cpp
Adafruit_GPS GPS(&Serial1);
#define GPS_WKUP_PIN D8
```

The ``position_acquired`` and ``start_gps`` variables are removed as they are no longer relevant to the code organization. This means that the logic checking for them should be removed from the following snipets

this code snipet from the loop becomes the ``getGPSdata()`` private function. Note that the ``GPS`` object in this snipet and the one after is the ``Adafruit_GPS`` class.

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

The GSM/2G SIM800L Module will be used to send messages over the 2G network in HTML. When not in use this module will be in power down mode to save battery. As we are unable to test this module's functionality for hardware reasons we will reuse the existing code for 2G communication.

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

    TaskHandle_t digital_write_task_handle = NULL;

public:
    SIM()

    //no setters

    TaskHandle_t getDigitalWriteTaskHandle();

    //send a message
    void sendMessageTask(){
        turnOn();
        writeMessage();

        //send the formatted message

        turnOff();
    }
}
```

##### *Power down/up*

The AT command ``AT+CPOWD=1``<sub>[p.24](https://datasheetspdf.com/pdf-down/S/I/M/SIM800H-SIMCom.pdf#page=24) & [p.146](https://wiki.elecrow.com/images/2/20/SIM800_Series_AT_Command_Manual_V1.09.pdf#page=146)</sub> is used to power down the board when not in use. The module can then be turned back on by sending a 1-second high, followed by 2 2-second low and then a 1-second high on a GPIO pin.

##### *Communication*

When a large motion is detected the GPS position is sent over HTML. This message should be formatted as follows to be understood by the server ``{"latitude": LATITUDE, "longitude": LONGITUDE, "batterie": BATTERY LEVEL}``. ``LATITUDE`` and ``LONGITUDE`` are floats we get from the GPS library. ``BATTERY LEVEL`` is a float between 0 & 1. This should also be sent if the battery gets below 20%.

"batterie" is not a typo, this is a French company that named their variable in French.

##### *re-using legacy code*

We have no way to try the SIM800L as our hardware is non-functional. as such we should mostly keep the original setup and protocol:

This code snipet needs to be added at the start of the class constructor.
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

While the nrf52 CPU does not support multithreading, the message should still be sent while other operation continue. This can be done using *freeRTOS* which is supported by the *nrf52 SDK*.

The task should be handled following this pseudocode

```cpp
sendMessageTask(){
    while(true){
        //wait for a message notification indefinitely using portMAX_DELAY
        if (xTaskNotifyWait(portMAX_DELAY) == true){
            //if a notification is recieved the condition is true

            turnOn();
            setup();
            writeMessage();
            //send the formatted message
            turnOff();
        }
        //if the xTaskNotifyWait times out, the loop restart
    }
}
```

The notification value doesn't mater.

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
    void soundControlTask();
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
            //only send the 2nd bit as the 1st bit stop the loop
            uint32_t sentValue = notification & 0b10;
            xTaskNotify(digitalWriteTaskHandle, sentValue)
            continue; //skip the rest of the loop
        }

        //if xTaskNotifyWait times out break, the digitalWriteTask loop
        xTaskNotify(digitalWriteTaskHandle, 0b01)
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


<!-- LSM6DS3Core imu(I2C_MODE, 0x6A); 

https://github.com/nfc-tools/libnfc
https://content.arduino.cc/assets/st_imu_lsm6ds3_datasheet.pdf
file:///home/max/Downloads/1462360001-AS.pdf

The firmware should be efficient enough to let the battery run for at least 7 days.
The system lock and unlock feature should be accessible by Bluetooth and NFC.
When the device is locked the alarm should be triggered at low volume for slight movement. 
When the device is locked the alarm should trigger at full volume when moving a lot.

The firmware has to run on an nRF52840 chip. 
Check how many amps the battery can take 

Branch Name: Pascal_Snake_Case
File/Folder Name: camelCase

Install Arduino IDE -->