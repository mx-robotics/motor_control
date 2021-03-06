//
// Created by firat on 20.01.20.
//
#include <array>
#include "FOC.h"
#include "utils.h"

void ftm0_isr(void)
{

    FTM0_SC &= ~FTM_SC_TOF;
    FOC::getInstance().run2();



}


int LUTindex = 100;


#define INT_FIRAT 1
void setup() {


    constexpr InhibitPins inhibitPins_{33, 24, 31};
    constexpr InitPins initPins{21, 23, 22};
    constexpr SPIPins spiPins{1,2,10};
    constexpr ISPins isPins {A15,A16,A17};
    Motor m1(inhibitPins_,initPins,spiPins,isPins);

    FOC::getInstance().motors = &m1;
    FOC::getInstance().initHardware();

    Serial.begin(9600);
    while (!Serial); // wait till there is serial connection


#if INT_FIRAT
    cli(); //Disable global interrupts
    NVIC_SET_PRIORITY(IRQ_FTM0, 64);
    NVIC_ENABLE_IRQ(IRQ_FTM0);
    sei(); //Enable global interrupts

#endif


}


void loop() {
    //@ 1:plot all 3 duty cycles to see if there are in unision
    //@ 2: test the modulo index offset calculation, plot it after scaling as well for debugging
    //@ 3: hardware init contuniuation for multi motor setup
    //@ TODO : have a ascending index for motors for easy distinguishing (PWM PINS etc)
    //@ 4: proper order of U V W and its documentation, drawing
    //@ 6: speed calculation
    //@TODO proper FOC class constructor for multi motor setup
    //@8 header - cpp proper placing of functions
    //@ 5: initial spin
    //@7: SetSpeedFromSomewhere implementation 1-ADC
    //@TODO Add a HAL, hide pure binary values using bit masks, most of them are already defined in kinetis.h
    //@TODO PID
    //@TODO Implement PID visualisation functions
    //@TODO SetSpeedFromSomewhere implementation 2-Serial Interface For Speed
    //@TODO 2MBoard migration and tests
    //@TODO 1M board current measurement tryouts, torque measurement
    //@TODO Torque control tryouts

}
