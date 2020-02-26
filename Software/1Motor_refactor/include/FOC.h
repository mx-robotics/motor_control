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
#include <ADC.h>
class FOC {

public:

    static const uint8_t numberOfMotors = 1;
    Motor *motors; //@TODO add a function to register motors
    ADC *adc = new ADC();
    static constexpr uint16_t PWM_FREQ = 20000;
    static constexpr uint8_t ADC_PIN = A1;


    void initInhibitPins(Motor &x);
    void initPWMPins(); //TODO:: clarify the difference between INH, IN and PWM PINS
    void initADCconversions();
    void initHardware();

    void activateInhibitPins(Motor &x);
    void updatePWMPinsDutyCycle(const SPWMDutyCycles &x, Motor &motor);
    void primitiveSpin(uint16_t LUTindex);

    int16_t calculateSensorOffset(Motor &motor, const uint16_t LUTindex);
    void initMotorParams(const uint16_t LUTindex);

    static FOC &getInstance();  // Singleton handler


    uint16_t getSpeedFromSomewhere();
    uint16_t setSpeedFromADC();


    void run();
    void run2();

};

#endif //INC_1MOTOR_REFACTOR_FOC_H
