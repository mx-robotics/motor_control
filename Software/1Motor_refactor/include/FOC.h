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

    inline static  uint8_t numberOfMotors = 0;
    Motor * motors[2];
    ADC *adc = new ADC();
    static constexpr uint16_t PWM_FREQ = 20000;
    static constexpr uint8_t ADC_PIN = A1;

    void registerMotors(Motor *m_ptr);

    void speedSweep();

    void initInhibitPins(Motor &x) ;

    void activateInhibitPins(Motor &x);


    void updatePWMPinsDutyCycle(const SPWMDutyCycles &x, Motor &motor);


    void initHardware();

    void initMotorParams(const uint16_t LUTindex);
    static FOC &getInstance();  // Singleton handler

    void initPWMPins();

    uint16_t getSpeedFromSomewhere();
    void doTheMagic() ;

    void doTheMagic2();

    void primitiveSpin(uint16_t LUTindex);
    void initADCconversions() ;

    uint16_t setSpeedFromADC();

    int16_t calculateSensorOffset(Motor &motor,
                                  const uint16_t LUTindex);
};

#endif //INC_1MOTOR_REFACTOR_FOC_H
