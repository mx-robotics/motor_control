//
// Created by firat on 20.01.20.
//
#include <array>
#include "FOC.h"
#include "utils.h"

#include <arm_math.h>

constexpr INHPins inhibitPins_{33, 26, 31};
constexpr PWMPins initPins{10, 22, 23};
constexpr ISPins isPins {A15,A16,A17};
Motor x(inhibitPins_,initPins,14,isPins);

constexpr INHPins inhibitPins2{28, 8, 25};
constexpr PWMPins initPins2{5, 6, 9};
constexpr ISPins isPins2 {A15,A16,A17};

Motor x2(inhibitPins2,initPins2,2,isPins);




//Motor x(inhibitPins_,initPins,15,isPins);
volatile bool flag = false;
void ftm0_isr(void)
{

    FTM0_SC &= ~FTM_SC_TOF;
    flag = true;

}



#define INT_FIRAT 0
#define PRIMITIVE_SPIN 1
#define LOCK_MOTOR 0

void setup() {


    FOC::getInstance().registerMotors(&x);
    FOC::getInstance().registerMotors(&x2);
    Serial.begin(9600);
    while (!Serial);
    FOC::getInstance().initHardware();


#if INT_FIRAT

    cli(); //Disable global interrupts
    NVIC_SET_PRIORITY(IRQ_FTM0, 64);
    NVIC_ENABLE_IRQ(IRQ_FTM0);
    sei(); //Enable global interrupts

#endif
#if PRIMITIVE_SPIN
    while (1) {
        for (int i = 0; i < 1489; ++i) {
            delayMicroseconds(10);
            FOC::getInstance().primitiveSpin(i,x);
    }
}

#endif

#if LOCK_MOTOR
    FOC::getInstance().primitiveSpin(110,x);
#endif

}


void loop() {
    if(flag){
        //elapsedMicros k;
        Serial.println(RotaryEncoderCommunication::SPITransfer(x)%1489);
        //FOC::getInstance().doTheMagic2();
        //Serial.println(k);
        flag = false;

    }
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
