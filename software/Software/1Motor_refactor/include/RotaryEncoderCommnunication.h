//
// Created by firat on 21.01.20.
//

#ifndef INC_1MOTOR_REFACTOR_ROTARYENCODERCOMMNUNICATION_H
#define INC_1MOTOR_REFACTOR_ROTARYENCODERCOMMNUNICATION_H

#include <Arduino.h>
#include <SPI.h>
#include "Motor.h"
#include <initializer_list>


class RotaryEncoderCommunication{
private:

    static const SPISettings spiSettings; //i
    static constexpr uint16_t CLK = 14; //i


public:
    static uint16_t SPITransfer(const Motor &x);
    static void initSPI();
    static void initMotorCSPins(const Motor &x);

};
#endif //INC_1MOTOR_REFACTOR_ROTARYENCODERCOMMNUNICATION_H
