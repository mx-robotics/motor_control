//
// Created by firat on 21.01.20.
//

#include "RotaryEncoderCommnunication.h"

const SPISettings RotaryEncoderCommunication::spiSettings(10000000, MSBFIRST, SPI_MODE1);

void RotaryEncoderCommunication::initSPI() {
    SPI.begin();
    SPI.setSCK(RotaryEncoderCommunication::CLK);

}


uint16_t RotaryEncoderCommunication::SPITransfer(const uint8_t CSPin) {
    SPI.beginTransaction(spiSettings);
    //SPI settings according to sensor datasheet: clock: max. 10MHz | MSB first | SPI Mode 1  | CPOL=0, CPHA= 1
    digitalWriteFast(CSPin, LOW); //SPI Mode 1 -> information gest sampled with falling endge
    uint16_t rotaryEncoderValue = SPI.transfer16(0xFFFF) & 0x3FFFu;
    //command bit15 = 1 (parity) | bit14 = 1 (read) | adresss 0x000 -> B11000000 00000000 = 0xC0000;
    digitalWriteFast(CSPin, HIGH); //SPI Mode 1 -> receive information with rising edge
    SPI.endTransaction();
    return rotaryEncoderValue;
}

void RotaryEncoderCommunication::initMotorCSPins(const uint8_t CSPin) {
    pinMode(CSPin, OUTPUT);
    digitalWrite(CSPin, HIGH);

}
