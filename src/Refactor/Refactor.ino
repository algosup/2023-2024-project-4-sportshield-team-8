#define TIMER_INTERRUPT_DEBUG 0
#define _TIMERINTERRUPT_LOGLEVEL_ 0
#include "NRF52_MBED_TimerInterrupt.h"
#include "NRF52_MBED_ISR_Timer.h"
#include <Arduino.h>
// BLE
#include <ArduinoBLE.h>
#include "struct.h"
// IMU
#include <LSM6DS3.h>
#include <Wire.h>

//---------------- GLOBAL VARIABLES -----------------------------
myConfig Config;
bool isAuthenticate = true;
// Timer
#define HW_TIMER_INTERVAL_MS 1
NRF52_MBED_Timer ITimer(NRF_TIMER_3);
NRF52_MBED_ISRTimer ISR_Timer;
#define TIMER_INTERVAL_120S 120000L
#define TIME_OUT_MS_BLE_ACT 5000  // 300s 300000 times in ms or bluetooth enabled to allow connection following motion detection. (value 5000 for 5s tests)
// BLE
BLEService PasswordService("19B10000-E8F2-537E-4F6C-D104768A1213"); // Bluetooth® Low Energy Service
BLEService ConfigService("19B10000-E8F2-537E-4F6C-D104768A1214");

BLEShortCharacteristic PasswordCharacteristic("19B10000-E8F2-537E-4F6C-D104768A1213", BLEWrite); // Bluetooth® Low Energy Characteristic
BLEStringCharacteristic NameCharacteristic("19B10001-E8F2-537E-4F6C-D104768A1214", BLERead | BLEWrite, 20);
BLEStringCharacteristic MACCharacteristic("19B10001-E8F2-537E-4F6C-D104768A1217", BLERead, 20);
BLEBooleanCharacteristic ActivationCharacteristic("19B10001-E8F2-537E-4F6C-D104768A1215", BLERead | BLEWrite);
BLEBooleanCharacteristic UnlockCharacteristic("19B10001-E8F2-537E-4F6C-D104768A1216", BLEWrite);

BLEDescriptor PasswordDescriptor("2901", "Password"); // Bluetooth® Low Energy Descriptor
BLEDescriptor NameDescriptor("2901", "Name");
BLEDescriptor ActivationDescriptor("2901", "Activation");
BLEDescriptor UnlockDescriptor("2901", "Unlock");
BLEDescriptor MACDescriptor("2901", "MAC Address");

bool BLE_activated = true; //true if the bluetooth is activated
uint32_t tim_connec = 0;   // time in ms or we start to activate the bluetooth following a detection of movement

//IMU : LSM6DS3
LSM6DS3 imu(I2C_MODE, 0x6A); //I2C device address 0x6A
uint32_t timer_imu = millis();
bool MotionBig = false;
bool MotionSmall = false;
bool MotionDetect = false;

// Buzzer
const int buzzerPin = D2;
unsigned long previousMillis = 0;
int currentRep = 0;
bool buzzerActive = false;

//Electroaimant
const int aimantPin = D3;

// Set a threshold to determine a "small" or "big" movement
float SmallMT = 20.0; //     SmallMotionThreshold
float BigMT = 150.0;  //    BigMotionThreshold

float SmallRT = 20.0; //     SmallRotationThreshold
float BigRT = 125.0;  //     BigRotationThreshold

//batterie
#define VBAT_ENABLE 14
float getBatteryVoltage();

float MotionData;
float RotationData;

unsigned long StartCoolDown = 0; //check point for millis aided cooldown

//-------------------------------- SETUP ----------------------------------------
void setup() {
  setupSerial();
  setupBuzzer();
  setupElectromagnet();
  setupDebugLED();
  setupTime();
  ble_setup();
  imu_setup();
  setupBatteryReading();

  Serial.println("setup end");
  digitalWrite(LEDR, HIGH);
  digitalWrite(LEDG, LOW);
  Time();

  Serial.print("V Bat: ");
  Serial.println(getBatteryVoltage());
}

//-------------------------------- LOOP ----------------------------------------
void loop() {
  // Read motion data from the IMU
  MotionData = getMotionData();
  RotationData = getRotationData();


   if (Config.isActivate) {  //alarm enalbled

    if (MotionData > BigMT || RotationData > BigRT) {  //Big motion detection
      if (MotionData > BigMT) {
        Serial.print("Motion detected : ");
        Serial.println(MotionData);
      } else {
        Serial.print("Rotation detected : ");
        Serial.println(RotationData);
      }
      MotionBig = true;
      MotionSmall = false;

    } else if ((MotionBig == false) && (MotionData > SmallMT || RotationData > SmallRT)) {  //Small motion detection
      if (MotionData > SmallMT) {
        Serial.print(" Small motion: ");
        Serial.println(MotionData);
      } else {
        Serial.print("Small rota : ");
        Serial.println(RotationData);
      }
      MotionSmall = true;
    }
  }


  if (MotionBig) {
    PulseBuzzer(5, 500, 1000);  // repetitions, DurationOn , DurationOff
  }

  if (MotionSmall) {
    PulseBuzzer(3, 500, 1000);  // repetitions, DurationOn , DurationOff
  }

    MotionDetect = true;
  if ((MotionData > SmallMT) || (RotationData > SmallRT)) {
    if (MotionData > SmallMT) {
      Serial.print("WAKE UP : ");
      Serial.println(MotionData);
    } else {
      Serial.print("WAKE UP Rota: ");
      Serial.println(RotationData);
    }
  }

  //if a mvt is detected and bluetooth is disabled bluetooth activation
  if (MotionDetect == true) {
    tim_connec = millis();
    MotionDetect = false;
    if (BLE_activated == false) {
      BLE_activated = true;
      Serial.println("MVT_detect->setup");
      ble_setup();
    }
  }

  // Poll BLE and handle timeout for disconnect
  if (BLE_activated || Config.isActivate) {
    BLE.poll();  // Allow BLE communication if activated or alarm is on
  }

  // // If no motion is detected for a specified time, turn off BLE to save power
  // if ((millis() - tim_connec > TIME_OUT_MS_BLE_ACT) && BLE_activated && !Config.isActivate) {
  //   BLE_activated = false;
  //   Serial.println("Timeout reached - turning off BLE");
  //   BLE.end();
  // }

  // // Check battery voltage
  // Serial.print("Battery Voltage: ");
  // Serial.println(getBatteryVoltage());
}


//------------- SETUP FUNCTIONS ------------------------------

void setupSerial() {
  Serial.begin(115200);
  if (!Serial) delay(1000);
  Serial.println("BLE Anti-theft Peripheral");
}

void setupBuzzer() {
  pinMode(buzzerPin, OUTPUT);
  digitalWrite(buzzerPin, HIGH);
  delay(1000);
  digitalWrite(buzzerPin, LOW);
  Serial.println("buzzer");
}

void setupElectromagnet() {
  pinMode(aimantPin, OUTPUT);
  digitalWrite(aimantPin, HIGH);
  delay(1000);
  digitalWrite(aimantPin, LOW);
  Serial.println("electro");
}

void setupDebugLED() {
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LEDR, LOW);
}

void setupTime() {
  if (ITimer.attachInterruptInterval(HW_TIMER_INTERVAL_MS * 1000, TimerHandler))  // Interval in microsecs
  {
    Serial.print(F("Starting ITimer OK, millis() = "));
    Serial.println(millis());
  }
}

void ble_setup(void) {
    if (!BLE.begin()) {
      Serial.println("starting Bluetooth® Low Energy module failed!");
      while (1)  // set a reset in this case? risk of card crash?
        ;
    }
    // set advertised local name and service UUID:
    BLE.setLocalName("SportShield 5");
    BLE.setDeviceName("SportShield 5");
    BLE.setAdvertisedService(PasswordService);
    // add descriptors
    PasswordCharacteristic.addDescriptor(PasswordDescriptor);
    NameCharacteristic.addDescriptor(NameDescriptor);
    ActivationCharacteristic.addDescriptor(ActivationDescriptor);
    UnlockCharacteristic.addDescriptor(UnlockDescriptor);
    MACCharacteristic.addDescriptor(MACDescriptor);
    // add the characteristic to the service
    PasswordService.addCharacteristic(PasswordCharacteristic);
    ConfigService.addCharacteristic(NameCharacteristic);
    ConfigService.addCharacteristic(ActivationCharacteristic);
    ConfigService.addCharacteristic(UnlockCharacteristic);
    ConfigService.addCharacteristic(MACCharacteristic);
    // add service
    BLE.addService(PasswordService);
    BLE.addService(ConfigService);
    // set the initial value for the characeristic:
    PasswordCharacteristic.writeValue(0);
    NameCharacteristic.writeValue("\n");
    ActivationCharacteristic.writeValue(false);
    UnlockCharacteristic.writeValue(false);
    MACCharacteristic.writeValue(BLE.address());
    //set event handler
    BLE.setEventHandler(BLEConnected, onConnect);
    BLE.setEventHandler(BLEDisconnected, onDisconnect);
    PasswordCharacteristic.setEventHandler(BLEWritten, onWritePassword);
    NameCharacteristic.setEventHandler(BLEWritten, onWriteName);
    NameCharacteristic.setEventHandler(BLERead, onReadName);
    ActivationCharacteristic.setEventHandler(BLEWritten, onWriteActivation);
    ActivationCharacteristic.setEventHandler(BLERead, onReadActivation);
    UnlockCharacteristic.setEventHandler(BLEWritten, onWriteUnlock);
    // start advertising
    BLE.advertise();
}

void imu_setup(void) {
  if (imu.begin() != 0) {
    Serial.println("Device error");
  } else {
    Serial.println("Accelerometer launched");
  }
}

void setupBatteryReading() {
  analogReadResolution(ADC_RESOLUTION);
  pinMode(PIN_VBAT, INPUT);
  pinMode(VBAT_ENABLE, OUTPUT);
  digitalWrite(VBAT_ENABLE, LOW);
}

//------------- ADDITIONAL FUNCTIONS ------------------------------
float getBatteryVoltage(){
  //unsigned int adcCount = analogRead(PIN_VBAT);
  float adcCount = analogRead(PIN_VBAT);
  float adcVoltage = (((adcCount - 3000) / 4096) * 0.55) + 3.6;
  //float vBat = adcVoltage * 1510.0/510.0;
  return adcVoltage;
}

float getMotionData() {
  static float previousAcceleration = 0;
  //r
  float accelX = imu.readFloatAccelX();
  float accelY = imu.readFloatAccelY();
  float accelZ = imu.readFloatAccelZ();

  float currentAcceleration = sqrt(accelX * accelX + accelY * accelY + accelZ * accelZ) * 100;
  float MotionDataerence = currentAcceleration - previousAcceleration;  // Calculate the acceleration difference
  previousAcceleration = currentAcceleration;

  return fabs(MotionDataerence);  //returns a value always positive
}

float getRotationData() {
  static float previousRotation = 0;

  // Read gyroscope values
  float gyroX = imu.readFloatGyroX();
  float gyroY = imu.readFloatGyroY();
  float gyroZ = imu.readFloatGyroZ();

  float currentRotation = sqrt(gyroX * gyroX + gyroY * gyroY + gyroZ * gyroZ);  // Calculate the current rotation based on gyroscope readings
  float RotationDataerence = currentRotation - previousRotation;                // Calculate the difference in rotation
  previousRotation = currentRotation;                                           // Update the previous rotation value

  return fabs(RotationDataerence);
}

void Time(void) {
  unsigned long millisPassed = millis();
  unsigned int seconds = (millisPassed / 1000) % 60;
  unsigned int minutes = (millisPassed / (1000 * 60)) % 60;
  unsigned int hours = (millisPassed / (1000 * 60 * 60)) % 24;
  Serial.print("Détecté a : ");
  Serial.print(hours);
  Serial.print("h");
  Serial.print(minutes);
  Serial.print("mn");
  Serial.print(seconds);
  Serial.println("s");
}

void PulseBuzzer(int repetitions, unsigned long durationOn, unsigned long durationOff) {
  static int buzzerState = LOW;
  unsigned long currentMillis = millis();

  if (currentRep < repetitions) {
    if (currentMillis - previousMillis >= (buzzerState == LOW ? durationOn : durationOff)) {
      digitalWrite(buzzerPin, buzzerState = !buzzerState);
      previousMillis = currentMillis;
      if (!buzzerState) currentRep++;
    }
  } else {
    // Reset variables after performing all repetitions
    currentRep = 0;
    previousMillis = 0;
    MotionSmall = false;
    MotionBig = false;
  }
}

// BLE Event Handlers
void onConnect(BLEDevice central) {
  Serial.print("Connected to ");
  Serial.println(central.address());
  Serial.println(BLE.address());
  digitalWrite(LEDB, LOW);
}

void onDisconnect(BLEDevice central) {
  Serial.print(F("Disconnected from central: "));
  Serial.println(central.address());
  isAuthenticate = false;
  digitalWrite(LEDB, HIGH);
}

void onWritePassword(BLEDevice central, BLECharacteristic characteristic) {
  const int expectedPassword = 1;
  int receivedPassword = PasswordCharacteristic.value();
  // Authenticate directly with the received password value
  isAuthenticate = (receivedPassword == expectedPassword);
  Serial.println(isAuthenticate ? "successful authentication" : "wrong password");
}


void onWriteName(BLEDevice central, BLECharacteristic characteristic) {
  if (isAuthenticate) {
    Config.Name = NameCharacteristic.value();
    String value = NameCharacteristic.value();
    Serial.print("Written name : ");
    Serial.println(value);
  } else {
    NameCharacteristic.writeValue("\n");
  }
}

void onReadName(BLEDevice central, BLECharacteristic characteristic) {
  Serial.println("CALLBACK READ");
  Serial.println(isAuthenticate);
  if (isAuthenticate) {
    NameCharacteristic.writeValue(Config.Name);
  } else {
    NameCharacteristic.writeValue("\n");
  }
}

void onWriteActivation(BLEDevice central, BLECharacteristic characteristic) {
  if (isAuthenticate) {
    Config.isActivate = ActivationCharacteristic.value();
    if (Config.isActivate) {
      Serial.println("Alarm enabled");
      // Add any code here that should run when the alarm is enabled.
    } else {
      Serial.println("Alarm disabled");
      // Add any code here that should run when the alarm is disabled.
    }
  } else {
    // If not authenticated, reset the characteristic value to reflect the actual configuration.
    ActivationCharacteristic.writeValue(Config.isActivate);
  }
}


void onReadActivation(BLEDevice central, BLECharacteristic characteristic) {
  Serial.println("CALLBACK READ");
  Serial.println(isAuthenticate);
  ActivationCharacteristic.writeValue(Config.isActivate);
}

void onWriteUnlock(BLEDevice central, BLECharacteristic characteristic) {
  if (isAuthenticate) {
    // activate electromagnet
    Serial.println("Unlock");
    digitalWrite(aimantPin, HIGH);
    delay(2000);
    digitalWrite(aimantPin, LOW);
  }
}

// No longer needed convertDMMtoDD() function and other GPS/GPRS related code removed.

// The TimerHandler function which was originally tied to the GPS functionality can be repurposed or removed depending on the new requirements.

void TimerHandler() {
  ISR_Timer.run();
}

// Any other ISR functions or utility functions related to GPS/GPRS should be removed.
