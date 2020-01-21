//
// Created by firat on 21.01.20.
//

#ifndef INC_1MOTOR_REFACTOR_SPEEDCALCULATION_H
#define INC_1MOTOR_REFACTOR_SPEEDCALCULATION_H

#include "Motor.h"



class VelocityCalculation {
    static constexpr float wheelDiameter{4.12f};
    static constexpr float wheelCircumference = wheelDiameter * M_PI * 2;
    static constexpr uint16_t measurementPeriodinMs = 100;
    static constexpr float rotaryEncoderMaxValue = 16384.0f;



public:
 /*   Direction determineDirection(Motor &motor) {  // @TODO implementation missing this is supposed be determine overflow -> might be redundant
        if (abs(rotaryEncoderValueMemoriser - motor.rotaryEncoderPosition) > 30) {
            rotaryEncoderOverflow = true;
        }
        if (rotaryEncoderOverflow == true) {

        }
    }
*/
    static float getRotationsPerMinute(Motor &motor) {
        return getRotationsPerSecond(motor) * 60;
    }

    static float getRotationsPerSecond(Motor &motor) {
        uint16_t tempEncoderValue = motor.rotaryEncoderPosition;
        if (abs(motor.previousRotaryEncoderValue - tempEncoderValue) > 30) {
            tempEncoderValue += rotaryEncoderMaxValue * motor.direction;
        }

        int16_t valueDifference = tempEncoderValue - motor.previousRotaryEncoderValue;
        float rotationsPerSecond = valueDifference / rotaryEncoderMaxValue * (1000 / measurementPeriodinMs);
        return rotationsPerSecond;
    }

    static float getMetersPerSecond(Motor &motor) {
        return getRotationsPerSecond(motor) * wheelCircumference;
    }

};

#endif //INC_1MOTOR_REFACTOR_SPEEDCALCULATION_H
