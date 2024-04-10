#pragma once

#include "global.h" // Ensure this includes the definition for `buzzerPin` and any necessary setup.

/*
 * Class: Buzzer
 * Description: Manages the operation of a buzzer component, allowing for controlled pulsing
 * (turning on and off for specified durations) for a given number of repetitions. This class
 * encapsulates both the initialization of the buzzer hardware and the functionality to
 * pulse the buzzer with specific patterns.
 */
class Buzzer {
public:
    // Constructor for the Buzzer class.
    Buzzer() {
    }

    /*
     * Initializes the buzzer hardware, setting the buzzer pin as an output
     * and ensuring it's in a known state (turned off) at startup.
     */
    void setup() {
        pinMode(buzzerPin, OUTPUT); // Set the buzzer pin as OUTPUT.
        digitalWrite(buzzerPin, LOW); // Ensure the buzzer is off on startup.
        delay(1000);
        digitalWrite(buzzerPin, LOW);
        Serial.println("Buzzer initialized.");
    }

    /*
     * Pulses the buzzer on and off for specified durations and repetitions.
     * 
     * Parameters:
     * repetitions - The number of times to repeat the on/off cycle.
     * durationOn - Duration (in milliseconds) the buzzer stays on in each cycle.
     * durationOff - Duration (in milliseconds) the buzzer stays off in each cycle.
     */
    void pulseBuzzer(int repetitions, unsigned long durationOn, unsigned long durationOff) {
        unsigned long currentMillis = millis(); // Capture the current time.
        static unsigned long previousMillis = 0; // Stores the last time the buzzer state changed.
        static int currentRep = 0; // Tracks the current repetition.
        static bool buzzerState = LOW; // Tracks the current state of the buzzer, initially OFF.

        if (currentRep < repetitions) {
            // Check if it's time to toggle the buzzer state.
            if (currentMillis - previousMillis >= (buzzerState ? durationOff : durationOn)) {
                buzzerState = !buzzerState; // Toggle the state.
                digitalWrite(buzzerPin, buzzerState); // Apply the new state to the buzzer.
                previousMillis = currentMillis; // Update the time of the last toggle.

                // If we just turned the buzzer off, increment the repetition counter.
                if (!buzzerState) {
                    currentRep++;
                }
            }
        } else {
            // After completing the specified repetitions, reset the control variables.
            currentRep = 0;
            buzzerState = LOW; // Ensure the buzzer is off.
            digitalWrite(buzzerPin, LOW); // Apply the off state.
        }
    }
};
