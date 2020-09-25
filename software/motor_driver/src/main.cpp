//
// Created by firat on 20.01.20.
//
#include <array>
#include "Controller.h"
#include "utils.h"
#include <arm_math.h>


#if defined(NEW_BOARD)
constexpr INHPins inhibitPins_{33, 26, 31};
constexpr PWMPins initPins{10, 22, 23};
constexpr ISPins isPins{A15, A16, A17};
Motor motor0(inhibitPins_, initPins, 2, isPins);

constexpr INHPins inhibitPins2{28, 8, 25};
constexpr PWMPins initPins2{5, 6, 9};
constexpr ISPins isPins2{A15, A16, A17};

Motor motor1(inhibitPins2, initPins2, 14, isPins2);


#else

constexpr INHPins inhibitPins_{33, 24, 31};
constexpr PWMPins initPins{21, 23, 22};
constexpr ISPins isPins {A15,A16,A17};
Motor motor0(inhibitPins_,initPins,10,isPins);
#endif

volatile bool flag = false;

void ftm0_isr(void) {

    FTM0_SC &= ~FTM_SC_TOF;
    flag = true;

}


#define INT_FIRAT 1
#define PRIMITIVE_SPIN 0
#define LOCK_MOTOR 0
#define SPEED_SWEEP 0

void setup() {
#if defined(NEW_BOARD)
    Controller::getInstance().registerMotors(&motor0); // 80
    motor0.setAngleOffset(-10); // - 10 seems aight for CCW
    motor0.setAsRightWheel();
    Controller::getInstance().registerMotors(&motor1);
    motor1.setAngleOffset(-10); // - 110 is da best for direction, - 10 for the other one


#else
    Controller::getInstance().registerMotors(&x);
#endif
    Serial.begin(9600);
    while (!Serial);
    delay(1000);
    Controller::getInstance().initHardware(13);

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
            //Serial.println("asdasd");
            //delayMicroseconds(8);
            Serial.print(i);
            Serial.print(" ");
            //Controller::getInstance().primitiveSpin(i,motor0);
            //Serial.println(RotaryEncoderCommunication::SPITransfer(motor0) % 1489);
            Controller::getInstance().primitiveSpin(i,motor1);
            Serial.println(RotaryEncoderCommunication::SPITransfer(motor1) % 1489);
    }
}

#endif

#if LOCK_MOTOR
    while(1) {
        Controller::getInstance().primitiveSpin(410, motor0);
        Serial.println(RotaryEncoderCommunication::SPITransfer(motor0) % 1489);
        delay(5000);
    }
#endif

}


void loop() {
    if (flag) {


        flag = false;
#if SPEED_SWEEP
        Controller::getInstance().speedSweep2();
#else
        Controller::getInstance().run();
        //uint16_t rotaryEncoderValue = RotaryEncoderCommunication::SPITransfer(motor0);
        //Serial.println(rotaryEncoderValue);
#endif
    }
    //Serial.println("POS");
    //@ 1:plot all 3 duty cycles to see if there are in unision
    //@ 2: test the modulo index offset calculation, plot it after scaling as well for debugging
    //@ 3: hardware init contuniuation for multi motor setup
    //@ 4: proper order of U V W and its documentation, drawing
    //@ 6: speed calculation
    //@ 8 proper Controller class constructor for multi motor setup
    //@ TODO header - cpp proper placing of functions
    //@ 5: initial spin
    //@7: SetSpeedFromSomewhere implementation 1-ADC
    //@TODO PID

    //@TODO SetSpeedFromSomewhere implementation 2-Serial Interface For Speed
    //@TODO 2MBoard migration and tests
    //@TODO 1M board current measurement tryouts, torque measurement
    //@TODO Torque control tryouts


}
