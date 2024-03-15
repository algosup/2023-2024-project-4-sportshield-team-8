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

// SIM800L GSM 2G Module
UART Serial2(D0, D1, NC, NC);
#define SIM800_RST_PIN A5
#define SIM800_DTR_PIN A5
#define TIME_OUT_MS_BLE_ACT 5000  // 300s 300000 times in ms or bluetooth enabled to allow connection following motion detection. (value 5000 for 5s tests)
SIM800L* sim800l;
bool send_position = false;
bool send_move = false;

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


  // Timer
  if (ITimer.attachInterruptInterval(HW_TIMER_INTERVAL_MS * 1000, TimerHandler))  // Interval in microsecs
  {
    Serial.print(F("Starting ITimer OK, millis() = "));
    Serial.println(millis());
  }
  ISR_Timer.setInterval(TIMER_INTERVAL_120S, TimerHandler);


  ble_setup();
  Serial.println(" ble_setup");
  imu_setup();
  Serial.println(" imu_setup");
  Serial2.begin(9600);
  delay(100);
  sim800l = new SIM800L((Stream*)&Serial2, SIM800_RST_PIN, 200, 512);
  pinMode(SIM800_DTR_PIN, OUTPUT);
  delay(1000);
  sim_setup();
  Serial.println(" sim_setup");

  analogReadResolution(ADC_RESOLUTION);  //setup battery reading
  pinMode(PIN_VBAT, INPUT);
  pinMode(VBAT_ENABLE, OUTPUT);
  digitalWrite(VBAT_ENABLE, LOW);

  Serial.println("fin setup ");
  digitalWrite(LEDR, HIGH);
  digitalWrite(LEDG, LOW);
  Temps();


  Serial.print("V Bat: ");
  Serial.println(getBatteryVoltage());
  Serial.print("Battery Level: ");
  Serial.println(getBatteryLevel());
}

//----------------- LOOP -----------------

void loop(){

    MotionData = getMotionData();
    RotationData = getRotationData();
    static String message = "";
    
    if (Serial.available() > 0){
        char c = Serial.read();
        if (c == '\n' || c == '\r'){
            message.trim();
            if (message.equalsIgnoreCase("shutdown")){
                Serial.println("Shutting down");
                shutdown();
            } else if (message.equalsIgnoreCase("unlock")){
                Serial.println("Unlocking");
                unlock();
            } else if (message.equalsIgnoreCase("lock")){
                lock();
            } else if (message.equalsIgnoreCase("battery")){
                Serial.print("V Bat: ");
                Serial.println(getBatteryVoltage());
                Serial.print("Battery Level: ");
                Serial.println(getBatteryLevel());
            } else {
                Serial.println("Unknown command");
            }
            message = "";
        } else {
            message += c;
        }
    }

    if (Config.isActivate){
        if (MotionData > SmallMT){
            MotionDetect = true;
            MotionSmall = true;
            MotionBig = true;
            tim_connec = millis();
        }
        if (RotationData > SmallRT){
            MotionDetect = true;
            MotionSmall = true;
            MotionBig = true;
            tim_connec = millis();
        }
    }

    if (MotionDetect){
        if (MotionData < SmallMT){
            MotionSmall = false;
        }
        if (RotationData < SmallRT){
            MotionSmall = false;
        }
        if (MotionData < BigMT){
            MotionBig = false;
        }
        if (RotationData < BigRT){
            MotionBig = false;
        }
        if (MotionSmall == false && MotionBig == false){
            MotionDetect = false;
        }
    }

    if (MotionDetect && BLE_activated){
        if (millis() - tim_connec > 30000){
            BLE_activated = false;
            Serial.println("BLE deactivated");
        }
    }   

    if (millis() - StartCoolDown > 30000){
        StartCoolDown = millis();
        if (MotionDetect){
            PulseBuzzer(1, 100, 100);
        }
    }

   





}



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

void imu_setup(void) {
  if (imu.begin() != 0) {
    Serial.println("Device error");
  } else {
    Serial.println("Accelerometer launched");
  }
}

void sim_setup(void){
    while (!sim800l->isReady()) {
        Serial.println(F("Problem to initialize AT command, retry in 1 sec"));
        digitalWrite(LEDR, !digitalRead(LEDR));
        delay(1000);
    }
    sim800l->enableEchoMode();
    sim800l->setupGPRS("iot.1nce.net");

    uint8_t signal = sim800l->getSignal();
    while (signal <= 0) {
        delay(1000);
        signal = sim800l->getSignal();
        Serial.println(F("Waiting signal..."));
    }
    Serial.println(String(signal));
    NetworkRegistration network = sim800l->getRegistrationStatus();
    // while (network != REGISTERED_HOME && network != REGISTERED_ROAMING) {
    //   delay(1000);
    //   network = sim800l->getRegistrationStatus();
    //   Serial.print(network + " ");
    //   Serial.println(F("Problem to register, retry in 1 sec"));
    //   digitalWrite(LEDG, !digitalRead(LEDG));
    // }
    delay(50);
    sim800l->setPowerMode(MINIMUM);      // set minimum functionnality mode
    digitalWrite(SIM800_DTR_PIN, HIGH);  // put in sleep mode
}



void Temps(void) {
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
  //r
  float gyroX = imu.readFloatGyroX();
  float gyroY = imu.readFloatGyroY();
  float gyroZ = imu.readFloatGyroZ();

  float currentRotation = sqrt(gyroX * gyroX + gyroY * gyroY + gyroZ * gyroZ) * 100;
  float RotationDataerence = currentRotation - previousRotation;  // Calculate the rotation difference
  previousRotation = currentRotation;

  return fabs(RotationDataerence);  //returns a value always positive
}

void TimerHandler() {
  ISR_Timer.run();
}

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
  if (isAuthenticate) {
    NameCharacteristic.writeValue(Config.Name);
  } else {
    NameCharacteristic.writeValue("\n");
  }

}

void onWriteActivation(BLEDevice central, BLECharacteristic characteristic) {
  if (isAuthenticate) {
    Config.isActivate = ActivationCharacteristic.value();
    bool value = ActivationCharacteristic.value();
    Serial.print("Written activation : ");
    Serial.println(value);
  } else {
    ActivationCharacteristic.writeValue(false);
  }
}

void onReadActivation(BLEDevice central, BLECharacteristic characteristic) {
  if (isAuthenticate) {
    ActivationCharacteristic.writeValue(Config.isActivate);
  } else {
    ActivationCharacteristic.writeValue(false);
  }
}

void onWriteUnlock(BLEDevice central, BLECharacteristic characteristic) {
  if (isAuthenticate) {
    bool value = UnlockCharacteristic.value();
    Serial.print("Written unlock : ");
    Serial.println(value);
    if (value) {
      unlock();
    }
  } else {
    UnlockCharacteristic.writeValue(false);
  }
}

void onWritePassword(BLEDevice central, BLECharacteristic characteristic) {
  const int motDePasseAttendu = 1234;
  short int value = PasswordCharacteristic.value();
  Conversion(value);
  isAuthenticate = (value == motDePasseAttendu);
  Serial.println(isAuthenticate ? "successful authentication" : "wrong password");
}


void Conversion(short int value) {
  int i = 0;
  int j = 0;
  int tab[4];
  while (value > 0) {
    tab[i] = value % 10;
    value = value / 10;
    i++;
  }
  for (j = 0; j < i; j++) {
    Config.pin = Config.pin + tab[j] * pow(10, j);
  }
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
  float voltage = analogRead(PIN_VBAT) * 0.003363075;
  digitalWrite(VBAT_ENABLE, LOW);
  return voltage;
}


// getBatteryLevel : Get the battery level
float getBatteryLevel() {
  const float minTension = 3.7;
  const float maxTension = 4.2;
  float battery = analogRead(PIN_VBAT);

  int minValue = (1023 * minTension) / 5;
  int maxValue = (1023 * maxTension) / 5;

  battery = ((battery - minValue) / (maxValue - minValue)) * 100;

  if (battery > 100)
    battery = 100;

  else if (battery < 0)
    battery = 0;
  
  return battery;
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

