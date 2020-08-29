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


class Controller {

public:

    inline static  uint8_t numberOfMotors = 0;
    Motor * motors[2];
    void doTheMagic2();

    void registerMotors(Motor *m_ptr);
    void initHardware(uint8_t SPI_CLK);
    static Controller &getInstance();  // Singleton handler



};

#endif //INC_1MOTOR_REFACTOR_FOC_H
