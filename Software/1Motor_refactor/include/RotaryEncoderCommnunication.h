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
    static const uint16_t CLK = 14; //i


public:
    static uint16_t SPITransfer(uint8_t CSPin) {
        SPI.beginTransaction(spiSettings);
        //SPI settings according to sensor datasheet: clock: max. 10MHz | MSB first | SPI Mode 1  | CPOL=0, CPHA= 1
        digitalWriteFast(CSPin, LOW); //SPI Mode 1 -> information gest sampled with falling endge
        uint16_t rotaryEncoderValue = SPI.transfer16(0xFFFF) & 0x3FFFu;
        //command bit15 = 1 (parity) | bit14 = 1 (read) | adresss 0x000 -> B11000000 00000000 = 0xC0000;
        digitalWriteFast(CSPin, HIGH); //SPI Mode 1 -> receive information with rising edge
        SPI.endTransaction();
        return rotaryEncoderValue;
    }

    static void initSPI(const std::initializer_list<uint16_t> &list);

};
#endif //INC_1MOTOR_REFACTOR_ROTARYENCODERCOMMNUNICATION_H
