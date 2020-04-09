//
// Created by firat on 20.01.20.
//

#ifndef INC_1MOTOR_REFACTOR_MOTOR_H
#define INC_1MOTOR_REFACTOR_MOTOR_H
#include <Arduino.h>
#include <array>


struct INHPins {
    const uint8_t InhibitPinU;
    const uint8_t InhibitPinV;
    const uint8_t InhibitPinW;

    constexpr INHPins(uint8_t PinU_, uint8_t PinV_, uint8_t PinW_) : InhibitPinU(PinU_), InhibitPinV(PinV_),
                                                                     InhibitPinW(PinW_) {}

};

struct PWMPins {
    const uint8_t InitPinU;
    const uint8_t InitPinV;
    const uint8_t InitPinW;

    constexpr PWMPins(uint8_t PinU_, uint8_t PinV_, uint8_t PinW_) : InitPinU(PinU_), InitPinV(PinV_),
                                                                     InitPinW(PinW_) {}

};

struct ServoPins {
    const uint8_t servoPin1;
    const uint8_t servoPin2;

    constexpr ServoPins(uint8_t servoPin1_, uint8_t servoPin2_) : servoPin1(servoPin1_), servoPin2(servoPin2_) {}
};



struct ISPins {
    const uint8_t ISPinU;
    const uint8_t ISPinV;
    const uint8_t ISPinW;

    constexpr ISPins(uint8_t ISPinU_, uint8_t ISPinV_, uint8_t ISPinW_) : ISPinU(ISPinU_), ISPinV(ISPinV_),
                                                                          ISPinW(ISPinW_) {}

};


enum Direction {
    CW = -1, STOP, CCW
};





class Motor {
public:
    Motor(INHPins inhibitPins_, PWMPins initPins_, uint8_t CSPin_, ISPins IsPins_)
    : inhibitPins(inhibitPins_), initPins(initPins_), CSPin(CSPin_), IsPins(IsPins_){}

    const  INHPins inhibitPins;
    const PWMPins initPins;
    const uint8_t CSPin;
    const ISPins IsPins;
    Direction direction = Direction::CCW;
    float speedRPS = 0;
    float torque = 0;
    float speedScalar= 80; // actual speed command 0.. 100
    //int16_t fieldWeakening = -100; // best : 80 for -1;
    int16_t fieldWeakening = -100; // best : 80 for -1;
    uint16_t rotaryEncoderPosition = 0;
    uint16_t previousRotaryEncoderValue = 0; // hold the previous rotaryEncoderValue
    uint16_t scaledRotaryEncoderPosition = 0; // accounts for the fieldWeakening
    uint16_t encoderCumulativeValue = 0;

    void setSensorOffset(uint16_t sensorOffset_) {
        fieldWeakening = sensorOffset_;
    }
    void setEncoderCumulativeValueToZero(){
        encoderCumulativeValue = 0;
    }
     void cumulativeAdd(uint16_t rotPos){
        uint16_t diff = abs(rotPos - previousRotaryEncoderValue);
        if(diff < 2){
            diff = 0;
        }
        if(diff > 100){
            diff = 16384 - diff;
        }
         previousRotaryEncoderValue = rotPos;
         encoderCumulativeValue += diff;




    }

    void updateRotaryEncoderPosition(uint16_t rotPos){
        //Serial.println("Rotary Encoder Position: ");
        //Serial.println(rotPos);
        scaledRotaryEncoderPosition = 1489 - (rotPos % 1489);
        //Serial.println("Rotary Encoder Position: ");
        //Serial.println(scaledRotaryEncoderPosition);
        //scaledRotaryEncoderPosition = rotPos;
        rotaryEncoderPosition = rotPos;

    }
    void updatePrevRotaryEncoderPosition(uint16_t rotPos){
        previousRotaryEncoderValue = rotPos;

    }

        void updateSpeedRPS(float_t rps){
            speedRPS = rps;
    }

    void updateSpeedScalar(float_t speed){
        speedScalar=speed;
    };



};




#endif //INC_1MOTOR_REFACTOR_MOTOR_H
