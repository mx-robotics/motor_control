//
// Created by firat on 21.01.20.
//

#ifndef INC_1MOTOR_REFACTOR_FOC_H
#define INC_1MOTOR_REFACTOR_FOC_H
#define NEW_BOARD

#include "Motor.h"
#include "SpeedCalculation.h"
#include "RotaryEncoderCommnunication.h"
#include "SVPWM.h"
#include "PID.h"
#include "utils.h"
#include "Teensy32Drivers.h"


struct CommandParameters{
    int32_t angle;
    float rps;
    Direction direction;
};


class FOC {

public:

    inline static  uint8_t numberOfMotors = 0;
    Motor * motors[2];

    void registerMotors(Motor *m_ptr);

    void run();

    void initHardware(uint8_t SPI_CLK);


    void initMotorParams(const uint16_t LUTindex);
    static FOC &getInstance();  // Singleton handler

    void speedSweep2();
    void doTheMagic2();
    CommandParameters setVelandSteeringAngleFromSerial();


    int16_t calculateSensorOffset(Motor &motor,
                                  const uint16_t LUTindex);


    void testMotors(Motor &x);
    void primitiveSpin(uint16_t LUTindex, Motor &motor);
    void speedSweep();


};

#endif //INC_1MOTOR_REFACTOR_FOC_H
