//
// Created by firat on 21.01.20.
//

#ifndef INC_1MOTOR_REFACTOR_FOC_H
#define INC_1MOTOR_REFACTOR_FOC_H

#include "Motor.h"
#include "SpeedCalculation.h"
#include "RotaryEncoderCommnunication.h"
#include "SVPWMLookUpTable.h"
#include "PID.h"
class FOC {

    Motor *motors;
    uint8_t numberOfMotors;
    //VelocityCalculation velocityCalculation;
    static constexpr auto LUT = SVPWMLookUpTable::generate();

    void updatePWMPinsDutyCycle(Motor &motor){

        return;
        // @TODO There will be a switch case here wrapping the embedded stuff
        // using motors rotaryPoisiton, offset, direction, and another offset due to modulation_index scaling
        // + 120, - 120,LUT

        // another idea would be updating the modulation index with PID everything then gets set according to it

    }
    float_t getSpeedFromSomewhere(); //@TODO get speed from ADC, serial port etc, interrupt driven maybe

    void doTheMagic() {
        uint16_t rotaryEncoderValue = RotaryEncoderCommunication::SPITransfer(motors[0].SpiPins.CS);
        motors[0].setRotaryEncoderPosition(rotaryEncoderValue);
        uint16_t rps = VelocityCalculation::getRotationsPerSecond(motors[0]);
        motors[0].updateSpeedRPM(rps);
        float_t targetSpeed = getSpeedFromSomewhere();
        float_t newModulationIndex = SpeedPIDController::getSpeedCommand(motors[0],targetSpeed);
        motors[0].updateModulationIndex(newModulationIndex);
        updatePWMPinsDutyCycle(motors[0]);

    }
};
#endif //INC_1MOTOR_REFACTOR_FOC_H
