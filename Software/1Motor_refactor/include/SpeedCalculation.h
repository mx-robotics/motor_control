//
// Created by firat on 21.01.20.
//

#ifndef INC_1MOTOR_REFACTOR_SPEEDCALCULATION_H
#define INC_1MOTOR_REFACTOR_SPEEDCALCULATION_H

#include "Motor.h"



class VelocityCalculation {
    static constexpr float wheelDiameter{4.12f};
    static constexpr float wheelCircumference = wheelDiameter * M_PI * 2;
    static constexpr float measurementPeriodinMs = 100.0f;
    static constexpr float rotaryEncoderMaxValue = 16384.0f;
    static constexpr uint8_t overflowTreshold = 100;
    static float calculate_displacement(uint16_t prevRotEncValue, uint16_t newRotEncValue){
        int16_t difference = newRotEncValue - prevRotEncValue;
        if(abs(difference) > overflowTreshold){
            if(difference > 0){
                difference-=rotaryEncoderMaxValue; // CCW
            } else{

                difference+=rotaryEncoderMaxValue; // CW
            }
        }
        return (difference/ rotaryEncoderMaxValue);

    }



public:


    static float getRotationsPerMinute(Motor &motor) {
        return getRotationsPerSecond(motor) * 60;
    }

    static float getRotationsPerSecond(Motor &motor) {

        float rotationsPerSecond = calculate_displacement(motor.previousRotaryEncoderValue,motor.rotaryEncoderPosition) * (1000 / measurementPeriodinMs);
        return rotationsPerSecond;
    }

    static float getMetersPerSecond(Motor &motor) {
        return getRotationsPerSecond(motor) * wheelCircumference;
    }

};

#endif //INC_1MOTOR_REFACTOR_SPEEDCALCULATION_H
