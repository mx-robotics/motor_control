//
// Created by firat on 20.01.20.
//
#include <array>
#include "FOC.h"
#include "utils.h"

#include <arm_math.h>

constexpr INHPins inhibitPins_{33, 26, 31};

//constexpr PWMPins initPins{22, 23, 10}; // working spin
//constexpr PWMPins initPins{23, 22, 10};
//constexpr PWMPins initPins{23, 10, 22};
//constexpr PWMPins initPins{22, 10, 23};
//constexpr PWMPins initPins{10, 23, 22};
constexpr PWMPins initPins{10, 22, 23};

//constexpr INHPins inhibitPins_{33, 24, 3};
//constexpr INHPins inhibitPins_2{0, 1, 2};
//constexpr PWMPins initPins{21, 23, 22};
//constexpr PWMPins initPins2{5, 6, 10};
constexpr ISPins isPins {A15,A16,A17};
Motor x(inhibitPins_,initPins,2,isPins);

//Motor x(inhibitPins_,initPins,15,isPins);
volatile bool flag = false;
void ftm0_isr(void)
{

    FTM0_SC &= ~FTM_SC_TOF;
    flag = true;

}



#define INT_FIRAT 1

void setup() {


    FOC::getInstance().registerMotors(&x);
    Serial.begin(9600);
    while (!Serial);
    FOC::getInstance().initHardware();


#if INT_FIRAT

    cli(); //Disable global interrupts
    NVIC_SET_PRIORITY(IRQ_FTM0, 64);
    NVIC_ENABLE_IRQ(IRQ_FTM0);
    sei(); //Enable global interrupts

#endif
/*
    while (1) {
        for (int i = 0; i < 1489; ++i) {
            delayMicroseconds(100);
            FOC::getInstance().primitiveSpin(i);
    }
}
*/


    //FOC::getInstance().testMotors(x);
}


void loop() {
    if(flag){
       //Serial.println(RotaryEncoderCommunication::SPITransfer(x));
       FOC::getInstance().doTheMagic2();
        flag = false;

    }

    /*for (int i = 0; i <1489 ; ++i) {

        delayMicroseconds(50);

        FOC::getInstance().primitiveSpin(i);

        ;}
    */

    //Serial.println(RotaryEncoderCommunication::SPITransfer(x));

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
