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
- Bluetooth should only recognize a paired phone.
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
    - *FreeRTOS V11.0.1-5*
    
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

When in doubt follow [this document](https://google.github.io/styleguide/cppguide.html) as this part only highlight the most relevant parts.

##### *Naming*

Use clear name that describe the purpose of the object. Avoid abbreviations. Everything should be done in english.

- Variables : 
  - snake_case in a function. 
  - snake_case_ with an underscore at the end when in the scope of a class.
- Types and concepts (class, struct, alias, enum and type aliases) : PascalCase
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

##### *misc*

- don't use use ``using std;`` instead do ``std::`` when needed to avoid naming conflict
- always use namespace
- create header files for module that can work on their own
- all classes should follow encapsulation. 
- avoid dynamic memory allocation if possible

## Key Functionality

Those key Functionality are independents from one an other and can be developed assuming the others works. Each part should be worked on as their separate classes.

### Motion Detection

The LSM6DS3 motion detection module can be set to send data only when detecting movement that exceed a certain threshold thanks to it's interrupt control registers. The accelerometer should only return a signal to the GPIO pin if the movement go past a certain threshold. This way the CPU can be kept in sleep mode the rest of the time.
The *LSM6DS3Core* class in the *Seeed Arduino LSM6DS3* library allow writing to the motion detection module's registers.

##### *Organization*

The class should follow this organization
```cpp
class MotionDetection{
    float wakeup_threshold_;
    float large_movement_threshold_;
    bool active_;
    struct Acceleration{
        float x, float y, float z,
        bool large_movement
    }


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
    Acceleration getMovement()
}
```

##### *initialization*

The setup should be done at the initialization of the CPU and whenever the motion detector is being switched to passive.
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

##### *get movement*

When the CPU is active, it should constantly pull data from the motion detection module using the non core LSM6DS3 library.
The motion detection module can be set to actively send data with ``LSM6DS3.begin()`` which change the read mode of motion sensor to be active.

``readFloatAccelX()`` is used to read these acceleration data from the motion detector. X can be changed to Y or Z to get different axis. The axis are added together before being compared to the threshold. 

##### *reference and resources*

- [LSM6DS3 documentation](https://content.arduino.cc/assets/st_imu_lsm6ds3_datasheet.pdf)
- [Seeed Arduino LSM6DS3 Github](https://github.com/Seeed-Studio/Seeed_Arduino_LSM6DS3): Take inspiration from the low level example

### NFC

The NFC antenna needs to be in active mode as we need it to power a passive device (in this case an NFC card). This has the inconvenience of increasing power draw, and means that the CPU can not be turned off while listening for an input.
Unfortunately [There is no working library to interact with the NFC](https://github.com/Seeed-Studio/wiki-documents/discussions/214?sort=new) as of 19/03/2024 as per seed studio.

<<<<<<< Updated upstream
A solution would be to use assembly assembly registers<sub>[p.208](https://infocenter.nordicsemi.com/pdf/nRF52840_PS_v1.7.pdf#page=208)</sub> to make the NFC work.
=======
A solution would be to use assembly assembly registers<sub>[p.208](https://infocenter.nordicsemi.com/pdf/nRF52840_PS_v1.7.pdf#page=208)</sub> to make the NFC work. This is not a priority.
>>>>>>> Stashed changes

##### *reference and resources*

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

##### *Linked to MAC adresses*

After pairing two devices, the connectivity is made via linking the MAC adress of both devices, meaning that they are bound together. Such MAC adress can be queried by the and kept in memory by the hardware.

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

### GPRS

The GSM/2G SIM800L Module will be used to send messages over the 2G network in html. When not in use this module will be in power down mode to save battery. As we are unable to test this module's functionality for hardware reason we will reuse the existing code for 2G communication.

##### *class organization*

```cpp
class SIM{
    //if the board is turned on or off
    bool board_status_;

    //return a string formatted as the message should be
    string formatMessage(float battery, float latitude, float longitude);

    //setup the SIM800L module return success
    bool setup();

    //turn on the board
    bool turnOn(){
        //send use GPIO to turn on the SIM800 board
    };

    //turn off the board
    bool turnOff(){
        //send AT command to turn off the SIM800 board
    };

public:

    //send a message
    bool sendMessage(float battery, float latitude, float longitude){
        turnOn();
        setup();
        formatMessage();

        //send the formatted message

        turnOff();
    }
}
```

##### *power down/up*

The AT command ``AT+CPOWD=1``<sub>[p.24](https://datasheetspdf.com/pdf-down/S/I/M/SIM800H-SIMCom.pdf#page=24)</sub>&<sub>[p.146](https://wiki.elecrow.com/images/2/20/SIM800_Series_AT_Command_Manual_V1.09.pdf#page=146)</sub> is used to power down the board when not in use. The module can then be turned back on by sending a 1 sec high, followed by 2 seconds low and then 1 second high on a GPIO pin.

##### *communication*

When a large motion is detected the GPS position is sent over HTML. This message should be formatted as follow to be understood by the server ``{"latitude": LATITUDE, "longitude": LONGITUDE, "batterie": BATTERY LEVEL}``. ``LATITUDE`` and ``LONGITUDE`` are float we get from the GPS library. ``BATTERY LEVEL`` is a float between 0 & 1. This this should also be sent if the battery gets bellow 20%.

"batterie" is not a typo, this is a french company that named their variable in french.

We have no way to try the SIM800L as our hardware is non functional. as such we should keep the original setup and protocol:

Setup
```cpp
sim800l = new SIM800L((Stream*)&Serial2, SIM800_RST_PIN, 200, 512);
pinMode(SIM800_DTR_PIN, OUTPUT);
delay(1000);
sim_setup();
Serial.println("SIM SETUP");
```

Send a message
```cpp
sim800l->setupGPRS("iot.1nce.net");
sim800l->connectGPRS();
String Route = "http://141.94.244.11:2000/sendNotfication/" + BLE.address();
String RouteCoord = "http://141.94.244.11:2000/updateCoordinate/" + BLE.address();
String str = "{\"latitude\": \" " + convertDMMtoDD(String(float(GPS.latitude), 4)) + "\", \"longitude\":\"" + convertDMMtoDD(String(float(GPS.longitude), 4)) + "\"}";
String bat = "{\"latitude\": \" " + convertDMMtoDD(String(float(GPS.latitude), 4)) + "\", \"longitude\":\"" + convertDMMtoDD(String(float(GPS.longitude), 4)) + "\", \"batterie\":\"" + String(getBatteryVoltage()) + "\"}";
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

##### *parallel tasking*

While the nrf52 CPU does not support multithreading, the message should still be sent while other operation continue. This can be done using freeRTOS which is supported by the nrf52 SDK. Managing RTOS should be done from outside the class. 

Go to [FreeRTOS](#FreeRTOS) for more detail.

##### *reference and resources*

- [GMS Library - Arduino docs](https://docs.arduino.cc/retired/archived-libraries/GSM/)
- [SIM800H&SIM800L_Hardware Design_V2.02](https://www.scribd.com/document/700531402/SIM800L-datasheet)
- [SIM800 Series_AT Command Manual_V1.09](https://wiki.elecrow.com/images/2/20/SIM800_Series_AT_Command_Manual_V1.09.pdf)

### Buzzer

##### *class organization*

```cpp
class Buzzer{
    const float kBuzzerPin_ = D2;
    //time in s when sound is emitted in a cycle
    float time_up_;
    //time in s when there is no sound in a cycle
    float time_down_;

public:
    bool setTimeUp(int time_up){
        //check if time_up is valid

        this->time_up_ = time_up;
    }

    bool setTimeDown(int time_down){
        //check if time_down is valid

        this->time_down_ = time_down;
    }

    bool startSoundCycle();
}
```
##### *sound loop*
The [electronic diagram](./image/SportShield%20-%20Electronics%20diagram%20.png) the buzzer is wired to the D2 pin. ``digitalWrite()`` is used to set the pin to High or low. The function looks something like :

```cpp
bool startSoundCycle(){
    //loop should last 10s
    while(time < 10s){
        digitalWrite(D2, HIGH);
        sleep(time_up_)
        digitalWrite(D2, LOW);
        sleep(time_up_)
    }
}
```

Making the time last 10s can be done using RTOS :
- Create a FreeRTOS Timer Callback which does the digital write when called
- create a ``TimerHandle_t`` that last ``time_up_`` or ``time_down_`` and call the Timer Callback

##### *small motion*
If a small motion is detected (bigger than the wakeup threshold, lower than the large motion threshold), the buzzer should sound for 0.1s every 2s. This should stop on it's own if the movement stops for 10s. Those time have to be set using the seter function for ``time_up_`` and ``time_down_``. 

##### *large motion*
When a large movement is detected, the alarm should bee for 0.1s ever 0.5s. This should not stop after 10s if no large or medium movement is detected. The bluetooth should also have the option to stop the alarm by unlocking the device.

##### *do
- [nRF5 FreeRTOS support](https://infocenter.nordicsemi.com/index.jsp?topic=%2Fsdk_nrf5_v17.0.2%2Ffreertos.html)
- [](https://www.freertos.org/Documentation/Mastering-the-FreeRTOS-Real-Time-Kernel.v1.0.pdf)

<a id="FreeRTOS"></a>
### FreeRTOS



### Battery


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