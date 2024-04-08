#pragma once

#include <stdint.h>
#include <LSM6DS3.h>

class MotionDetection{
    LSM6DS3 imu();

    float wakeup_threshold_;
    float large_movement_threshold_;
    bool active_;

public:
    bool setPassive(){

    }

    bool setActive(){

    }

    bool setWakeupThreshold(float threshold){
        if(threshold >= 0){
            this->wakeup_threshold_ = threshold;
            return 0;
        }else{
            this->wakeup_threshold_ = 0;
            return 1;
        }
    }

    bool setLargeMovementThreshold(float threshold){
        if(threshold >= wakeup_threshold_){
            this->large_movement_threshold_ = threshold;
        }else if ()
        {
            /* code */
        }
        
    }

    uint8_t getMovement(){

    }
};