//
// Created by firat on 21.01.20.
//

#ifndef INC_1MOTOR_REFACTOR_PID_H
#define INC_1MOTOR_REFACTOR_PID_H
#include "Motor.h"

class SpeedPIDController {
    inline static float kP = 0;
    inline static float kI = 0;
    inline static float kD = 0;
    static float targetSpeedMemorizer;
    inline static float measurement_last = 0;
    inline static float measurement_now = 0;
    inline static float error_sum = 0;
    inline static float acc_error = 0;
public:
    static float getSpeedCommand(Motor &motor, float targetSpeed){ // float might be a better valur for speed command

        float error = targetSpeed - motor.speedRPS; // error in rps


        kP = 0.5f ;
        kI= 0.2f ;
        kD = 0.1f ;





        measurement_last = measurement_now;
        measurement_now = motor.speedRPS;


        error_sum += (error * kI);

        float proportional = (error * kP);
        float integral = error_sum; // here might be a problem
        float derivative = kD * ( measurement_now - measurement_last);

        if(integral > 6.9f){
            integral = 6.9f;
        }

        if(integral < -6.9f){
            integral = -6.9f;
        }

        acc_error = error_sum; // this was required for  plotting

        float command = ( (proportional + integral - derivative)+1.30f)/0.145f;
       // float command =  (proportional + integral - derivative) * (11.22f - 0.22f*fabs(targetSpeed));
        if(command > 0){
            if (command > 99) { command = 99; }
            //motor.direction = 1;
            //if (command < 5) { command = 5; }
        }
        else{
            if(command < -99){ command = -99; }
            //direction = -1;

        }
        return abs(command);

        //command < 0 ? direction = -1 : direction = 1;



    }
};

class TorquePIDController {
    float proportionalGain;
    float integralGain;
    float derivative;
    float targetTorqueMemorizer;

};
#endif //INC_1MOTOR_REFACTOR_PID_H
