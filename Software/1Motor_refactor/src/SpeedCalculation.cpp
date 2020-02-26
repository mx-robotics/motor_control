//
// Created by firat on 21.01.20.
//
#include "SpeedCalculation.h"

float VelocityCalculation::getRotationsPerSecond2(uint16_t encoderCumulativeValue){

    float_t retVal =(encoderCumulativeValue/rotaryEncoderMaxValue) * (1000 / measurementPeriodinMs);
    return retVal;
}


 float VelocityCalculation::calculate_displacement(uint16_t prevRotEncValue, uint16_t newRotEncValue){ // remmanst of an old implementation, should be discarded at the final version
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


float VelocityCalculation::getRotationsPerMinute(Motor &motor) {
    return getRotationsPerSecond(motor) * 60;
}

float VelocityCalculation::getRotationsPerSecond(Motor &motor) {

    float rotationsPerSecond = calculate_displacement(motor.previousRotaryEncoderValue,motor.rotaryEncoderPosition) * (1000 / measurementPeriodinMs);
    Serial.println(rotationsPerSecond);

    return rotationsPerSecond;
}

float VelocityCalculation::getMetersPerSecond(Motor &motor) {
    return getRotationsPerSecond(motor) * wheelCircumference;
}

