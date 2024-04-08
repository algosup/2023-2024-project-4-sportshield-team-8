#include "NRF52_MBED_TimerInterrupt.h"
#include "NRF52_MBED_ISR_Timer.h"
#include <Arduino.h>
#include <ArduinoBLE.h>
#include "buzzer.hpp"
#include "bluetooth.hpp"

void BluetoothTask(void *pvParameters);
void SensorTask(void *pvParameters);
void MainTask(void *pvParameters);

void setup() {
class Main{

    TaskHandle_t main_task_handle_ = NULL;

    public:
        Main();
        Taskhandle_t getMainTaskHandle();
        void main_task(
        TaskHandle_t sound_control_task_handle,      
        TaskHandle_t send_message_task_handle,
        TaskHandle_t bluetooth_handler_task_handle);
    };

    

    // Create tasks
    xTaskCreate(
        BluetoothTask,  // Task function
        "Bluetooth",    // Name of the task (for debugging)
        1024,           // Stack size (bytes)
        NULL,           // Task input parameter
        1,              // Priority
        NULL            // Task handle
    );

    xTaskCreate(SensorTask, "Sensor", 1024, NULL, 1, NULL);
    xTaskCreate(MainTask, "Main", 2048, NULL, 2, NULL); // Higher priority for MainTask

    // Start the scheduler - this function will not return
    vTaskStartScheduler();

    void BluetoothTask(void *pvParameters) {
    for (;;) {
        myBluetooth.handleEvents();
        vTaskDelay(pdMS_TO_TICKS(100)); // Delay to prevent task from monopolizing CPU
        }
    }

    void SensorTask(void *pvParameters) {
        for (;;) {
            mySensor.readSensors();
            vTaskDelay(pdMS_TO_TICKS(100));
        }
    }

    void MainTask(void *pvParameters) {
        for (;;) {
            myMain.main_task(sound_control_task_handle, send_message_task_handle, bluetooth_handler_task_handle);
            vTaskDelay(pdMS_TO_TICKS(100));
        }
    }

};

void loop() {
}
