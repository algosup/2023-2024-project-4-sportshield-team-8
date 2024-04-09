#pragma once
#include <cstddef>
#include <Arduino.h>
#include <Arduino_FreeRTOS.h>





class Buzzer{
private:
    const uint8_t buzzerPin_ = D2;

    static const uint32_t LARGE_MOVEMENT_MASK = 0x01; 
    static const uint32_t NOTHING_MASK = 0x10;


  // Durations for buzzer signals for different movement intensities
    const uint32_t smallMovementHighTime_ = 100; // Time in milliseconds buzzer is on for small movement
    const uint32_t smallMovementLowTime_ = 2000; // Time in milliseconds buzzer is off for small movement
    const uint32_t largeMovementHighTime_ = 100; // Time in milliseconds buzzer is on for large movement
    const uint32_t largeMovementLowTime_ = 500; // Time in milliseconds buzzer is off for large movement

    //how long should 
    float cycle_time_ ;

    //the task handle for 
    TaskHandle_t digital_write_task_handle = NULL;
    TaskHandle_t sound_control_task_handle = NULL;

    // Movement type indicator
    const uint8_t largeMovement_ = 0;
    const uint8_t stop_ = 0;
    const uint8_t smallMovement_ = 0;

public:
    //constructor
    Buzzer(){
        pinMode(buzzerPin_, OUTPUT);
    }


    static void digitalWriteTask(void *parameters) {
        Buzzer* buzzer = static_cast<Buzzer*>(parameters);
        uint32_t notificationValue;

        for (;;) {
            // Wait indefinitely for a notification to be received
            if (xTaskNotifyWait(0x00, // Don’t clear bits on entry.
                                ULONG_MAX, // Clear all bits on exit.
                                &notificationValue, // Value from the notification stored here.
                                portMAX_DELAY)) { // Wait indefinitely

                // Determine behavior based on the notification received
                if (notificationValue & buzzer->NOTHING_MASK) {
                    // If a stop notification is received, simply continue waiting for the next notification
                    continue;
                }
                // Determine the duration of the high and low periods based on the movement intensity
                    uint32_t highTime = (notificationValue & buzzer->LARGE_MOVEMENT_MASK) ? buzzer->largeMovementHighTime_ : buzzer->smallMovementHighTime_;
                    uint32_t lowTime = (notificationValue & buzzer->LARGE_MOVEMENT_MASK) ? buzzer->largeMovementLowTime_ : buzzer->smallMovementLowTime_;


                    // Activate buzzer
                    digitalWrite(buzzer->buzzerPin_, HIGH);
                    vTaskDelay(pdMS_TO_TICKS(highTime)); // Wait for the high period

                    // Deactivate buzzer
                    digitalWrite(buzzer->buzzerPin_, LOW);
                    vTaskDelay(pdMS_TO_TICKS(lowTime)); // Wait for the low period
          }
        }
    }

    void startBuzzerTask() {
        xTaskCreate(digitalWriteTask, "BuzzerTask", 1024, this, 1, &digitalWriteTaskHandle_);
    }

    void stopBuzzerTask() {
        if (digitalWriteTaskHandle_ != NULL) {
            vTaskDelete(digitalWriteTaskHandle_);
            digitalWriteTaskHandle_ = NULL;
        }
    }
}