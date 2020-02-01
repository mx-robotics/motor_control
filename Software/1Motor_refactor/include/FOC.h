//
// Created by firat on 21.01.20.
//

#ifndef INC_1MOTOR_REFACTOR_FOC_H
#define INC_1MOTOR_REFACTOR_FOC_H

#include "Motor.h"
#include "SpeedCalculation.h"
#include "RotaryEncoderCommnunication.h"
#include "SVPWM.h"
#include "PID.h"
class FOC {

public:

    static constexpr uint8_t numberOfMotors=1;
    Motor *motors;
    //VelocityCalculation velocityCalculation;
    //static constexpr auto LUT = SVPWM::generate();
    static constexpr uint16_t PWM_FREQ = 20000;


    void initInhibitPins(Motor &x){
        pinMode(x.inhibitPins.InhibitPinW,OUTPUT);
        pinMode(x.inhibitPins.InhibitPinU,OUTPUT);
        pinMode(x.inhibitPins.InhibitPinV,OUTPUT);

    }

    void activateInhibitPins(Motor &x){
        digitalWriteFast(x.inhibitPins.InhibitPinW,HIGH);
        digitalWriteFast(x.inhibitPins.InhibitPinU,HIGH);
        digitalWriteFast(x.inhibitPins.InhibitPinV,HIGH);

    }




    void updatePWMPinsDutyCycle(const SPWMDutyCycles &x, Motor &motor){
        if(motor.initPins.InitPinW == 21){
            FTM0_C6V = x.inDutyCycleW;
            FTM0_C0V = x.inDutyCycleU;
            FTM0_C1V = x.inDutyCycleV;
        }

        // @TODO else for the other motor, other sets of pins
        // using motors rotary Poisiton, offset, direction, and another offset due to modulation_index scaling
        // + 120, - 120,LUT

        // another idea would be updating the modulation index with PID everything then gets set according to it

    }
    void initHardware(){

        initPWMPins();
        RotaryEncoderCommunication::initSPI();


        initInhibitPins(motors[0]);
        activateInhibitPins(motors[0]);
        RotaryEncoderCommunication::initMotorCSPins(motors[0].SpiPins.CS);


    }

    void initMotorParams(){
        uint16_t sensorOffset = calculateSensorOffset(motors[0],50); // this calculation can be done only once and then hardcoded until there is a motor change.
        motors[0].setSensorOffset(sensorOffset);

    }

    void initPWMPins(){
        FTM0_SC = 0;

        FTM0_CONF = 0xC0; //set up BDM in 11
        FTM0_FMS = 0x00; //clear the WPEN so that WPDIS is set in FTM0_MODE

//FTM0_MODE|=0x05; // 0000 0101
        FTM0_MODE = 0b00000110; // 00000111
        FTM0_MOD = (F_BUS / PWM_FREQ) / 2;


        FTM0_C6SC = 0b00101000;
        FTM0_C6V = 0; //50%
        PORTD_PCR6 |= PORT_PCR_MUX(4) | PORT_PCR_DSE | PORT_PCR_SRE; //Teency pin 21 -> FTM0_CH6


        FTM0_C0SC = 0b00101000;
        FTM0_C0V = 0; //50%
        PORTC_PCR1 |= PORT_PCR_MUX(4) | PORT_PCR_DSE | PORT_PCR_SRE; //Teency pin 22 (A8) -> FTM0_CH0


        FTM0_C1SC = 0b00101000;
        FTM0_C1V = 0; //50%
        PORTC_PCR2 |= PORT_PCR_MUX(4) | PORT_PCR_DSE | PORT_PCR_SRE; //Teency pin 23 (A9) -> FTM0_CH1

        //@TODO all the other pins for the second motor
        FTM0_CNTIN = 0x00;


        FTM0_SC = 0b01101000; //CPWM MODE 0x48 EPWM -> 0x68 0110 1000


    }
    float_t getSpeedFromSomewhere(); //@TODO get speed from ADC, serial port etc, interrupt driven maybe

    void doTheMagic() {
        uint16_t rotaryEncoderValue = RotaryEncoderCommunication::SPITransfer(motors[0].SpiPins.CS);
        motors[0].updateRotaryEncoderPosition(rotaryEncoderValue);
        uint16_t rps = VelocityCalculation::getRotationsPerSecond(motors[0]);
        motors[0].updateSpeedRPM(rps);
        float_t targetSpeed = getSpeedFromSomewhere();
        float_t newModulationIndex = SpeedPIDController::getSpeedCommand(motors[0],targetSpeed);
        motors[0].updateModulationIndex(newModulationIndex);
        SPWMDutyCycles dutyCycles = SVPWM::calculateDutyCycles(motors[0]);
        updatePWMPinsDutyCycle(dutyCycles,motors[0]);
        

    }

    uint16_t calculateSensorOffset(Motor &motor, const uint16_t LUTindex){ //the index is used as a parameter to maybe plot the sensor offset all over the motor range
        uint16_t LUTSize = SVPWM::getLutSize();
        uint16_t dutyCycleW=SVPWM::getLUT()[LUTindex];
        uint16_t dutyCycleU=SVPWM::getLUT()[LUTindex + (LUTSize / 3)];
        uint16_t dutyCycleV=SVPWM::getLUT()[LUTindex + (LUTSize / 3) * 2];
        SPWMDutyCycles x{dutyCycleW, dutyCycleU, dutyCycleV};
        updatePWMPinsDutyCycle(x,motor);
        uint16_t encoderVal = RotaryEncoderCommunication::SPITransfer(motor.SpiPins.CS);
        uint16_t encoderValScaled = encoderVal % LUTSize;
        uint16_t expectedRotorFlux = (LUTindex + LUTSize/2) % LUTSize; // + 180
        int16_t sensorOffset = expectedRotorFlux - encoderValScaled;
        return sensorOffset;
        /**
         *If there was not a sensor offset, i.e. the rotary encoder directly gave the orientation of the rotor flux vector,
         * energising 3 phases with the correct duty cycles (120° shifted relative to each other) should give
         * a rotary encoder position that corresponds to the rotor flux vector. This is the "happy position of the motor
         * ", rotator flux is alligned with stator flux, Permanent magnets south poles are alligened with the north poles of our created stator flux.
         * So when lets say put 90 degrees as index to create a stator fluw, the rotor flux should 180* decrees ahead of it, so at 270.
         * So when we apply 90 the motors locks itself at 270 but we dont want it to lock itself, we want it to keep moving so we either apply 90+90 or 90-90 so it will move in either direction
         *
         * If we know this position we would just create the
         * corresponding stator flux vector that exerts a force perpendicular to it. This would be
         * the corresponding duty cycle +90° or -°90 ( 1/4 of the LUT size, depending on the direction).
         */

    }
};
#endif //INC_1MOTOR_REFACTOR_FOC_H
