//
// Created by firat on 20.01.20.
//
#include <array>
#include "FOC.h"
#include "utils.h"



constexpr INHPins inhibitPins_{33, 24, 31};
constexpr PWMPins initPins{21, 23, 22};
constexpr ISPins isPins {A15,A16,A17};
Motor x(inhibitPins_,initPins,10,isPins);
void ftm0_isr(void)
{

    FTM0_SC &= ~FTM_SC_TOF;


   FOC::getInstance().doTheMagic2();


}


#define INT_FIRAT 1

void setup() {



    FOC::getInstance().registerMotors(&x);
    FOC::getInstance().initHardware();
    Serial.begin(9600);
    while (!Serial);




#if INT_FIRAT

    cli(); //Disable global interrupts
    NVIC_SET_PRIORITY(IRQ_FTM0, 64);
    NVIC_ENABLE_IRQ(IRQ_FTM0);
    sei(); //Enable global interrupts

#endif




}


void loop() {

    //Serial.println("POS");
    //@ 1:plot all 3 duty cycles to see if there are in unision
    //@ 2: test the modulo index offset calculation, plot it after scaling as well for debugging
    //@ 3: hardware init contuniuation for multi motor setup
    //@ 4: proper order of U V W and its documentation, drawing
    //@ 6: speed calculation
    //@ 8 proper FOC class constructor for multi motor setup
    //@ TODO header - cpp proper placing of functions
    //@ 5: initial spin
    //@7: SetSpeedFromSomewhere implementation 1-ADC
    //@TODO PID

    //@TODO SetSpeedFromSomewhere implementation 2-Serial Interface For Speed
    //@TODO 2MBoard migration and tests
    //@TODO 1M board current measurement tryouts, torque measurement
    //@TODO Torque control tryouts


}
