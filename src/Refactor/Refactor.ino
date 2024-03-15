//----------------- INCLUDES -----------------
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
//GPS
#include <Adafruit_GPS.h>
//SIM
#include "SIM800L.h"

//---------------- GLOBAL VARIABLES -----------------------------
myConfig Config;
bool isAuthenticate = true;
// Timer
#define HW_TIMER_INTERVAL_MS 1
NRF52_MBED_Timer ITimer(NRF_TIMER_3);
NRF52_MBED_ISRTimer ISR_Timer;
#define TIMER_INTERVAL_120S 120000L
// BLE
BLEService PasswordService("19B10000-E8F2-537E-4F6C-D104768A1213");  // Bluetooth® Low Energy Service
BLEService ConfigService("19B10000-E8F2-537E-4F6C-D104768A1214");

BLEShortCharacteristic PasswordCharacteristic("19B10000-E8F2-537E-4F6C-D104768A1213", BLEWrite);  // Bluetooth® Low Energy Characteristic
BLEStringCharacteristic NameCharacteristic("19B10001-E8F2-537E-4F6C-D104768A1214", BLERead | BLEWrite, 20);
BLEStringCharacteristic MACCharacteristic("19B10001-E8F2-537E-4F6C-D104768A1217", BLERead, 20);
BLEBooleanCharacteristic ActivationCharacteristic("19B10001-E8F2-537E-4F6C-D104768A1215", BLERead | BLEWrite);
BLEBooleanCharacteristic UnlockCharacteristic("19B10001-E8F2-537E-4F6C-D104768A1216", BLEWrite);

BLEDescriptor PasswordDescriptor("2901", "Password");  // Bluetooth® Low Energy Descriptor
BLEDescriptor NameDescriptor("2901", "Name");
BLEDescriptor ActivationDescriptor("2901", "Activation");
BLEDescriptor UnlockDescriptor("2901", "Unlock");
BLEDescriptor MACDescriptor("2901", "MAC Address");

bool BLE_activated = true;  //true if the bluetooth is activated
uint32_t tim_connec = 0;    // time in ms or we start to activate the bluetooth following a detection of movement

//IMU : LSM6DS3
LSM6DS3 imu(I2C_MODE, 0x6A);  //I2C device address 0x6A
uint32_t timer_imu = millis();
bool MotionBig = false;
bool MotionSmall = false;
bool MotionDetect = false;

// Buzzer
const int buzzerPin = D2;
void PulseBuzzer(int repetitions, unsigned long durationOn, unsigned long durationOff);
unsigned long previousMillis = 0;
int currentRep = 0;

//Electroaimant
const int aimantPin = D3;
bool isAimantActivate = false;

// Set a threshold to determine a "small" or "big" movement

float SmallMT = 20.0;  //     SmallMotionThreshold
float BigMT = 150.0;   //    BigMotionThreshold

float SmallRT = 20.0;  //     SmallRotationThreshold
float BigRT = 125.0;   //     BigRotationThreshold

//batterie
#define VBAT_ENABLE 14
float getBatteryVoltage();

float MotionData;
float RotationData;

unsigned long StartCoolDown = 0;  //check point for millis aided cooldown

 //----------------- SETUP -----------------
void setup() {
  pinMode(buzzerPin, OUTPUT);  // setup for buzzer
  digitalWrite(buzzerPin, HIGH);
  delay(1000);
  digitalWrite(buzzerPin, LOW);
  Serial.println(" buzzer");

  pinMode(aimantPin, OUTPUT);  //setup electro-aimant
  digitalWrite(aimantPin, HIGH);
  delay(1000);
  digitalWrite(aimantPin, LOW);
  Serial.println("electro");

  //debug led initialization
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LEDR, LOW);

  // power bridge control
  pinMode(D4, OUTPUT);
  digitalWrite(D4, HIGH);

  // power battery control with the transistor
  pinMode(D9, OUTPUT);
  digitalWrite(D9, HIGH);

  // battery charging enable with high current 100mA > 50mA
  pinMode(P0_13, OUTPUT);
  digitalWrite(P0_13, LOW);

  Serial.begin(115200);
  if (!Serial) delay(1000);
  Serial.println("BLE Antivol Peripheral");
}

  // Timer
  if (ITimer.attachInterruptInterval(HW_TIMER_INTERVAL_MS * 1000, TimerHandler))  // Interval in microsecs
  {
    Serial.print(F("Starting ITimer OK, millis() = "));
    Serial.println(millis());
  }
  ISR_Timer.setInterval(TIMER_INTERVAL_120S, GPS_ISR);


  ble_setup();
  Serial.println(" ble_setup");
  imu_setup();





 //------------- SETUP FUNCTIONS ------------------------------

 void ble_setup(void){
    if (!BLE.begin()) {
    Serial.println("starting BLE failed!");
    while (1){
        digitalWrite(LEDR, HIGH);
        delay(500);
        digitalWrite(LEDR, LOW);
        delay(500);
        }
    } 
    Serial.println("BLE started");
    // set advertised local name and service UUID:
    BLE.setLocalName("L'Imcomensurable Team 8");
    BLE.setDeviceName("L'Imcomensurable Team 8");
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

 void imu_setup(void){
    if (!imu.init()) {
    Serial.println("Failed to detect and initialize IMU!");
    while (1){
        digitalWrite(LEDR, HIGH);
        delay(500);
        digitalWrite(LEDR, LOW);
        delay(500);
        }
    }
    Serial.println("IMU started");
    imu.enableDefault();
    imu.writeAccelODR(LSM6DS3::ODR_52Hz);
    imu.writeGyroODR(LSM6DS3::ODR_52Hz);
    Serial.println("IMU initialized");
 }





 //----------------- COMMANDS -----------------

// shutdown : Allowing the user to shutdown the device
void shutdown(){
  digitalWrite(D4, LOW);
  digitalWrite(D9, LOW);
  digitalWrite(P0_13, HIGH);
  digitalWrite(aimantPin, HIGH);
  digitalWrite(buzzerPin, HIGH);
  digitalWrite(LEDR, HIGH);
  digitalWrite(LEDG, HIGH);
  digitalWrite(LEDB, HIGH);
  while(1){
    delay(1000);
  }
}

// unlock : Allowing the user to unlock the device
void unlock(){
  digitalWrite(aimantPin, HIGH);
  digitalWrite(buzzerPin, HIGH);
  digitalWrite(LEDR, HIGH);
  digitalWrite(LEDG, HIGH);
  digitalWrite(LEDB, HIGH);
  while(1){
    delay(1000);
  }
}

// lock : Allowing the user to lock the device
void lock(){
  digitalWrite(aimantPin, LOW);
  digitalWrite(buzzerPin, LOW);
  digitalWrite(LEDR, LOW);
  digitalWrite(LEDG, LOW);
  digitalWrite(LEDB, LOW);
  while(1){
    delay(1000);
  }
}

// getBatteryVoltage : Get the battery voltage
float getBatteryVoltage(){
  digitalWrite(VBAT_ENABLE, HIGH);
  delay(100);
  float voltage = analogRead(A7) * 0.003363075;
  digitalWrite(VBAT_ENABLE, LOW);
  return voltage;
}

// pulse : Make the buzzer pulse
void PulseBuzzer(int repetitions, unsigned long durationOn, unsigned long durationOff){
  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= durationOn){
    previousMillis = currentMillis;
    if (currentRep < repetitions){
      digitalWrite(buzzerPin, HIGH);
      delay(durationOn);
      digitalWrite(buzzerPin, LOW);
      delay(durationOff);
      currentRep++;
    }
  }
}

