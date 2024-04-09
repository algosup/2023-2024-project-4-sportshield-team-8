#include "global.h"
#include "bleModule.h"
#include "imuModule.h"
#include "buzzerModule.h"
#include "gpsModule.h"
#include "buzzerModule.h"
#include "simModule.h"
#include "batteryManager.h"

//--------------------------- ADDITIONAL FUNCTIONS -------------------------------
void TimerHandler() {
  ISR_Timer.run();
}


// --------------------------- OBJECTS -------------------------------
BatteryManager batteryManager;

//-------------------------------- SETUP ----------------------------------------
void setup() {
  pinMode(buzzerPin, OUTPUT);  // setup for buzzer
  digitalWrite(buzzerPin, HIGH);
  delay(1000);
  digitalWrite(buzzerPin, LOW);
  Serial.println(" buzzer");

  pinMode(aimantPin, OUTPUT);  //setup electro-magnet
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
  ISR_Timer.setInterval(TIMER_INTERVAL_120S, GPS_ISR);

  ble_setup();
  Serial.println(" ble_setup");
  imu_setup();
  Serial.println(" imu_setup");
  gps_setup();
  Serial.println(" gps_setup");
  Serial2.begin(9600);
  delay(100);
  sim800l = new SIM800L((Stream*)&Serial2, SIM800_RST_PIN, 200, 512);
  pinMode(SIM800_DTR_PIN, OUTPUT);
  delay(1000);
  //sim_setup();
  Serial.println("SIM SETUP");

  analogReadResolution(ADC_RESOLUTION);  //setup battery reading
  batteryManager.setup();
  pinMode(VBAT_ENABLE, OUTPUT);
  digitalWrite(VBAT_ENABLE, LOW);

  Serial.println("fin setup ");
  digitalWrite(LEDR, HIGH);
  digitalWrite(LEDG, LOW);
  Temps();

}
//-------------------------------- LOOP ----------------------------------------
void loop() {

  MotionData = getMotionData();
  RotationData = getRotationData();

  batteryManager.checkVoltage();

  if (Config.isActivate) {  //alarm enalbled
    activateGPS();
    checkIfaMovementIsEitherLargeOrSmall(MotionData, RotationData);
  }

  if (MotionBig) {
    pulseBuzzer(5, 500, 1000);  // repetitions, DurationOn , DurationOff
  }

  if (MotionSmall) {
    pulseBuzzer(3, 100, 100);  // repetitions, DurationOn , DurationOff
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
        Serial.println("BLE_START");
        ble_setup();
      }
    }

    // if the lock is activated and the bluetooth is activated, the communication is authorized
    if ((BLE_activated == true) || (Config.isActivate)) {
      BLE.poll();  // authorized communication
    }

    // if the bluetooth is activated and the time is exceeded, the bluetooth is deactivated
    if ((millis() - tim_connec > TIME_OUT_MS_BLE_ACT) && (BLE_activated == true) && (Config.isActivate != 1)) {
      BLE_activated = false;
      Serial.println("TIMEOUT -> BLE STOP");
      BLE.end();
    }


  //capture clocked GPS data
  GPS.read();
  if (GPS.newNMEAreceived()) {
    Serial.print(GPS.lastNMEA());    // this also sets the newNMEAreceived() flag to false
    if (!GPS.parse(GPS.lastNMEA()))  // this also sets the newNMEAreceived() flag to false
      Serial.println("fail to parse");
    ;  // we can fail to parse a sentence in which case we should just wait for another
  }

 //after capturing and verifying the GPS data
  if (GPS.fix && position_acquired == false) {
     float currentLatitude = convertDMMtoDD(String(float(GPS.latitude), 4)).toFloat();
    float currentLongitude = convertDMMtoDD(String(float(GPS.longitude), 4)).toFloat();
    if(abs(currentLatitude - previousLatitude) > 0.0001 || abs(currentLongitude - previousLongitude) > 0.0001){
      previousLatitude = currentLatitude;
      previousLongitude = currentLongitude;
      GPS.fix = 0;
      digitalWrite(GPS_WKUP_PIN, LOW);
      GPS.sendCommand("$PMTK225,4*2F");  // send to backup mode
    }
  }
   



  if (send_move) {  //sending of positions via SIM module
    Serial.println("Envoi detection mouvement");
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
    send_move = false;
  }

  if (send_position) {  //regular sending of positions via SIM module
    Serial.println("Envoi regulier position");
    sim800l->setupGPRS("iot.1nce.net");
    sim800l->connectGPRS();
    String RouteCoord = "http://141.94.244.11:2000/updateCoordinate/" + BLE.address();
    String bat = "{\"latitude\": \" " + convertDMMtoDD(String(float(GPS.latitude), 4)) + "\", \"longitude\":\"" + convertDMMtoDD(String(float(GPS.longitude), 4)) + "\", \"batterie\":\"" + String(getBatteryVoltage()) + "\"}";
    char posbat[200];
    bat.toCharArray(posbat, bat.length() + 1);
    Serial.println(posbat);
    Serial.println(RouteCoord);
    char directionCoord[200];
    RouteCoord.toCharArray(directionCoord, RouteCoord.length() + 1);
    sim800l->doPost(directionCoord, "application/json", posbat, 10000, 10000);
    sim800l->disconnectGPRS();
    send_position = false;
  }
}




