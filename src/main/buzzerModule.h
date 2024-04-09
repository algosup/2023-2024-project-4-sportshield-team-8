#pragma once 


/*
  This file contains the functions and setup functions for the Buzzer.
*/

#include "global.h"



void pulseBuzzer(int repetitions, unsigned long durationOn, unsigned long durationOff) {
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


// ----------SETUP the Buzzer module----------

void setupBuzzer(){
    pinMode(buzzerPin, OUTPUT);  // setup for buzzer
    digitalWrite(buzzerPin, HIGH);
    delay(1000);
    digitalWrite(buzzerPin, LOW);
    Serial.println(" buzzer");
}