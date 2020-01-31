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
    static constexpr uint16_t PWM_FREQ = 20000;
    static constexpr uint16_t LUTSize = 1489;
    uint16_t IN_W_duty_cycle=0;
    uint16_t IN_U_duty_cycle=0;
    uint16_t IN_V_duty_cycle=0;
    uint16_t sensorOffset = 0; //@TODO to be calculated by this class
    uint16_t angleOffset = 0; // @TODO to be calculated by this class
    float modulationIndexOffset = 0; // @TODO modulation index + offfset // this should be delegated to LUT Class, the const offset calculation, since a motor should know its sensor offset and maybe angle offset ? -> the whole index calculation can be pushed to LUTClass s
    //@TODO Even the look up table itself can be pushed to the LUTClass which makes sense. Calculating the proper index and applying the proper offset should be hanndle by the LUTClass. This class should just fetch it. Anything that involves LUT should be in LUT

/*
    void calculate_offset_parameters() {

        // max modulation index
        uint16_t rotor_position = 1000;
        float k1 = 0.2f;
        calculateInputPinsDutyCycle();
        uint16_t a1 = IN_W_duty_cycle * k1;
        modulation_index *= (k1);
        calculateInputPinsDutyCycle();
        uint16_t x1 = IN_W_duty_cycle;

        float_t k2 = 0.4f;
        modulation_index = (2 / sqrt(3)); // max modul0 index again
        calculateInputPinsDutyCycle();
        uint16_t a2 = IN_W_duty_cycle * k2;
        modulation_index *= k2;
        calculateInputPinsDutyCycle();
        uint16_t x2 = IN_W_duty_cycle;



        uint16_t yy1 = x1 - a1;
        uint16_t yy2 = x2 - a2;

        uint16_t xx1 = k1 * 100;
        uint16_t xx2 = k2 * 100;

        offset_param_m = (yy2 - yy1) / (xx2 - xx1);

        offset_param_c = yy2 - (offset_param_m * xx2);


    }
*/
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

    void initHardware(){
        initInhibitPins(motors[0]);
        activateInhibitPins(motors[0]);
    }

    void updatePWMPinsDutyCycle(Motor &motor){
        calculateDutyCycles(motor);
        if(motor.initPins.InitPinW == 21){
            FTM0_C6V = IN_W_duty_cycle;
            FTM0_C0V = IN_U_duty_cycle;
            FTM0_C1V = IN_V_duty_cycle;
        }

        // @TODO else for the other motor, other sets of pins
        // using motors rotary Poisiton, offset, direction, and another offset due to modulation_index scaling
        // + 120, - 120,LUT

        // another idea would be updating the modulation index with PID everything then gets set according to it

    }
    void calculateDutyCycles(Motor &x){
        uint16_t base = (x.rotaryEncoderPosition + sensorOffset ) % LUTSize;
        uint16_t WDutyCycleIndex = (base + angleOffset * x.direction);
        uint16_t UDutyCycleIndex = ((base + angleOffset * x.direction) + LUTSize/3) % LUTSize;
        uint16_t VDutyCycleIndex = ((base + angleOffset * x.direction) + (2*(LUTSize/3)) )% LUTSize;
        IN_W_duty_cycle = LUT[WDutyCycleIndex] + modulationIndexOffset;
        IN_U_duty_cycle = LUT[UDutyCycleIndex] + modulationIndexOffset;
        IN_V_duty_cycle = LUT[VDutyCycleIndex] + modulationIndexOffset;

    };

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


        FTM0_CNTIN = 0x00;


        FTM0_SC = 0b01101000; //CPWM MODE 0x48 EPWM -> 0x68 0110 1000


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
