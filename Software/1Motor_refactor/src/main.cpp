//
// Created by firat on 20.01.20.
//
#include <array>
#include "FOC.h"


void ftm0_isr(void)
{
    FTM0_SC &= ~FTM_SC_TOF;
    //tuw::FOC::getInstance().doTheMagic();




}


int i = 0;


#define INT_FIRAT 1
void setup() {


    constexpr InhibitPins inhibitPins_{23, 22, 21};
    constexpr InitPins initPins{33,23,31};
    constexpr SPIPins spiPins{1,2,10};
    constexpr ISPins isPins {A15,A16,A17};
    Motor x(inhibitPins_,initPins,spiPins,isPins);
    FOC f;
    f.motors = &x;


    Serial.begin(9600);


#if INT_FIRAT
    cli(); //Disable global interrupts
    NVIC_SET_PRIORITY(IRQ_FTM0, 64);
    NVIC_ENABLE_IRQ(IRQ_FTM0);
    sei(); //Enable global interrupts

#endif
    while (!Serial);


}


void loop() {
    //int kam[10000]={0};
    delay(10);
    //uint16_t rotorPos= RotaryEncoderCommunication::SPITransfer(10);
    //Serial.println(rotorPos);
    //Serial.println("POS");
    //@TODO 1:plot all 3 duty cycles to see if there are in unision
    //@TODO 2: test the modulo index offset calculation, plot it after scaling as well for debugging
    //@TODO hardware init contuniuation for multi motor setup
    //@TODO proper order of U V W and its documentation, drawing
    //@TODO proper FOC class constructor for multi motor setup
    //@TODO header - cpp proper placing of functions
    //@TODO initial spin
    //@TODO SetSpeedFromSomewhere implementation 1-ADC
    //@TODO PID
    //@TODO SetSpeedFromSomewhere implementation 2-Serial Interface For Speed
    //@TODO 2MBoard migration and tests
    //@TODO 1M board current measurement tryouts, torque measurement
    //@TODO Torque control tryouts


}
