//
// Created by firat on 21.01.20.
//

#ifndef INC_1MOTOR_REFACTOR_STEERINGSERVOCONTROLLER_H
#define INC_1MOTOR_REFACTOR_STEERINGSERVOCONTROLLER_H

#include "Motor.h"

class SteeringServoController {

    SteeringServoController( ServoPins servoPins ):servoPins(servoPins){}
    const ServoPins servoPins;
public:
    void controlSteeringServo(float_t angle); // not implemented yet



};


#endif //INC_1MOTOR_REFACTOR_STEERINGSERVOCONTROLLER_H
