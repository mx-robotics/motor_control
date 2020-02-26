//
// Created by firat on 21.01.20.
//

#ifndef INC_1MOTOR_REFACTOR_PID_H
#define INC_1MOTOR_REFACTOR_PID_H
#include "Motor.h"

class SpeedPIDController {
    static float kP; // @TODO : can be made const after the tuning
    static float kI;
    static float kD;
    static float targetSpeedMemorizer;
    static float measurement_last;
    static float measurement_now;
    static float error_sum;
    static float acc_error;
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
