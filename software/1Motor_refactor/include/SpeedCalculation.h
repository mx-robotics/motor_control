//
// Created by firat on 21.01.20.
//

#ifndef INC_1MOTOR_REFACTOR_SPEEDCALCULATION_H
#define INC_1MOTOR_REFACTOR_SPEEDCALCULATION_H

#include "Motor.h"

/**
 * VelocityCalculation class - The rotary encoder readings are taken a fixed time interval - every duty cycle -
 * This class encapsulates attributes and functions to calculate RPS, RPM and meters per sec
 */

class VelocityCalculation {
    static constexpr float wheelDiameter{4.12f};
    static constexpr float wheelCircumference = wheelDiameter * M_PI * 2;
    static constexpr float measurementPeriodinMs = 50.0f;
    static constexpr float rotaryEncoderMaxValue = 16384.0f;
    static constexpr uint8_t overflowTreshold = 100;
    static uint32_t encoderCumulativeValue;
    static inline uint32_t previousRotaryEncoderValue;


    /***
     * Calculates the difference between to rotary encoder position readings. It gets complex due to overflows,
     * direction changes. Direction can also be determined from here
     * @TODO : Not being used RN, finish dev and use or DISCARD
     * @param prevRotEncValue
     * @param newRotEncValue
     * @return
     */
    static float calculate_displacement(uint16_t prevRotEncValue, uint16_t newRotEncValue){
        int16_t difference = newRotEncValue - prevRotEncValue;

      /*  if(abs(difference) > overflowTreshold){
            if(difference > 0){
                difference-=rotaryEncoderMaxValue; // CCW
            } else{

                difference+=rotaryEncoderMaxValue; // CW
            }
        }
        */
        return (difference/ rotaryEncoderMaxValue);

    }



public:
    /**
     * by dividing encoderCumulativeValue by rotaryEncoderMaxValue you get the percentage of rotation. This is done
     * 20 times a sec hence (1000/measurementPeriodinMs) we multiply it with 20 to get the rotations per SECOND
     * otherwise it will give rotation per 50 miliseconds
     * @TODO : change hardcoded values. This class can also be initialized with a measurement frequency (like 20 hz)
     * @param m - motor to get encoder values
     * @return - rotation per sec
     */
    static float getRotationsPerSecond2(Motor & m){
        float_t retVal =(m.encoderCumulativeValue/rotaryEncoderMaxValue) * 20;
        return retVal;
    }
    static float getRotationsPerMinute(Motor &motor) {
        return getRotationsPerSecond(motor) * 60;
    }

    static float getRotationsPerSecond3(Motor & m){
        int32_t diff = m.rotaryEncoderPosition - previousRotaryEncoderValue;
        Serial.print("rotPos : ");
        Serial.println(m.rotaryEncoderPosition);
        Serial.print("prev  : ");
        Serial.println(previousRotaryEncoderValue);
       if(previousRotaryEncoderValue < 5000 &&  m.rotaryEncoderPosition > 11000){ // OVERFLOW ROT POS INCREASING

            diff -= 16384;

        }
       Serial.print("diff  ");
        Serial.println(diff);

        if(m.rotaryEncoderPosition < 5000 &&  previousRotaryEncoderValue > 11000){ // OVERFLOW ROT POS DECREASING
            diff += 16384;

        }
        float_t retVal = diff/16384.0f * 20.0f;
        previousRotaryEncoderValue = m.rotaryEncoderPosition;
        return retVal;
    }

    /**
     * @TODO NOT BEING USED RN, FINSIH DEV OR REMOVE
     * @param motor
     * @return
     */
    static float getRotationsPerSecond(Motor &motor) {

        float rotationsPerSecond = calculate_displacement(motor.previousRotaryEncoderValue,motor.rotaryEncoderPosition) * (1000 / measurementPeriodinMs);
        Serial.println(rotationsPerSecond);

        return rotationsPerSecond;
    }

    static float getMetersPerSecond(Motor &motor) {
        return getRotationsPerSecond(motor) * wheelCircumference;
    }

};

#endif //INC_1MOTOR_REFACTOR_SPEEDCALCULATION_H
