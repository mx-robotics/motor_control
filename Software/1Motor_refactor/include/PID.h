//
// Created by firat on 21.01.20.
//

#ifndef INC_1MOTOR_REFACTOR_PID_H
#define INC_1MOTOR_REFACTOR_PID_H
#include "Motor.h"

class SpeedPIDController {
    float proportionalGain;
    float integralGain;
    float derivative;
    float targetSpeedMemorizer;
public:
    static float getSpeedCommand(Motor &motor, float targetSpeed);
};

class TorquePIDController {
    float proportionalGain;
    float integralGain;
    float derivative;
    float targetTorqueMemorizer;

};
#endif //INC_1MOTOR_REFACTOR_PID_H
