//
// Created by firat on 21.01.20.
//

#ifndef INC_1MOTOR_REFACTOR_SPEEDCALCULATION_H
#define INC_1MOTOR_REFACTOR_SPEEDCALCULATION_H

#include "Motor.h"



class VelocityCalculation {
    static constexpr float wheelDiameter{4.12f};
    static constexpr float wheelCircumference = wheelDiameter * M_PI * 2;
    static constexpr float measurementPeriodinMs = 50.0f;
    static constexpr float rotaryEncoderMaxValue = 16384.0f;

    static float calculate_displacement(uint16_t prevRotEncValue, uint16_t newRotEncValue);


public:

    static float getRotationsPerSecond2(uint16_t encoderCumulativeValue);

    static float getRotationsPerMinute(Motor &motor);

    static float getRotationsPerSecond(Motor &motor);

    static float getMetersPerSecond(Motor &motor);

};

#endif //INC_1MOTOR_REFACTOR_SPEEDCALCULATION_H
