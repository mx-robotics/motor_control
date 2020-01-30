//
// Created by firat on 21.01.20.
//

#include "RotaryEncoderCommnunication.h"

const SPISettings RotaryEncoderCommunication::spiSettings(10000000, MSBFIRST, SPI_MODE1);

void RotaryEncoderCommunication::initSPI(const std::initializer_list<uint16_t> &list) {
    for(auto x :list){
        pinMode(x, OUTPUT);
        digitalWrite(x, HIGH);
    }
    SPI.begin();
    SPI.setSCK(RotaryEncoderCommunication::CLK);

}
