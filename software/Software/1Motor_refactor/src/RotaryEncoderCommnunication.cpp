//
// Created by firat on 21.01.20.
//

#include "RotaryEncoderCommnunication.h"

const SPISettings RotaryEncoderCommunication::spiSettings(10000000, MSBFIRST, SPI_MODE1);
/**
 * Inits SPI peripheral. This function has to be called before any SPI communication
 * and is called at FOC::initHardware
 * @param SPI_CLK - The SPI Clock Pin. It is either 13 or 14
 */
void RotaryEncoderCommunication::initSPI(uint8_t SPI_CLK) {
    SPI.begin();
    SPI.setSCK(SPI_CLK);

}

/**
 * Raw SPI transfer. Sends the predefined bit combination (refer to AMS sensor data sheet for more details)
 * and get the response. The response is 14 bits long so some bitwise operations are performed to get the actual value.
 * @param x - Motor object
 * @return - 14 bit rotary encoder position
 */
uint16_t RotaryEncoderCommunication::SPITransfer(const Motor &x) {

    SPI.beginTransaction(spiSettings);
    //SPI settings according to sensor datasheet: clock: max. 10MHz | MSB first | SPI Mode 1  | CPOL=0, CPHA= 1
    digitalWriteFast(x.CSPin, LOW); //SPI Mode 1 -> information gest sampled with falling endge
    uint16_t rotaryEncoderValue = SPI.transfer16(0xFFFF) & 0x3FFFu;
    //command bit15 = 1 (parity) | bit14 = 1 (read) | adresss 0x000 -> B11000000 00000000 = 0xC0000;
    digitalWriteFast(x.CSPin, HIGH); //SPI Mode 1 -> receive information with rising edge

    SPI.endTransaction();


    return rotaryEncoderValue;

}

/**
 * Configures CS pin as Output and set it to HIGH (Inactive). Must be called once before any SPI Communication
 * is called at FOC::initHardware for each motor.
 * @param x - Motor object to get the corresponding Chip Select Pin
 */

void RotaryEncoderCommunication::initMotorCSPins(const Motor &x) {
    pinMode(x.CSPin, OUTPUT);
    digitalWrite(x.CSPin, HIGH);

}
