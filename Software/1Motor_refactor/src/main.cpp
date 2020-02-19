//
// Created by firat on 20.01.20.
//
#include <array>
#include "FOC.h"
#include "utils.h"

/*
 * There is only one ftm0_isr function that is being triggered by either
 * - FTM0 TOF(Timer Overflow Event) currently in use
 * - Channel (n) Interrupt: FTM0_CnSC [Channel Flag] This interrupt comes when the CnV value is matched
 *   [CHnIE] = 1 must be set, The interrupt sets the CHF FTMO_C6SC &= ~FTM_CSC_CHF
 *
 *
 *  #define FTM_CSC_CHF			0x80				// Channel Flag
 *   #define FTM_CSC_CHIE			0x40				// Channel Interrupt Enable
 *
 *   - Fault event - not relevant
 * */

void ftm0_isr(void)
{

    FTM0_SC &= ~FTM_SC_TOF;
    FOC::getInstance().doTheMagic2();
    //tuw::FOC::getInstance().doTheMagic();




}


int LUTindex = 100;


#define INT_FIRAT 1
void setup() {


    constexpr InhibitPins inhibitPins_{33, 24, 31};
    constexpr InitPins initPins{21, 23, 22};
    constexpr SPIPins spiPins{1,2,10};
    constexpr ISPins isPins {A15,A16,A17};
    Motor x(inhibitPins_,initPins,spiPins,isPins);

    FOC::getInstance().motors = &x;
    FOC::getInstance().initHardware();
    Serial.begin(9600);
    while (!Serial);

   /* while (!Serial);
    for (int j = 0; j < 1489 ; j+=20 ) {
        FOC::getInstance().calculateSensorOffset(x,j);
    while (!Serial);

    }
*/

#if INT_FIRAT
    cli(); //Disable global interrupts
    NVIC_SET_PRIORITY(IRQ_FTM0, 64);
    NVIC_ENABLE_IRQ(IRQ_FTM0);
    sei(); //Enable global interrupts

#endif

        while(1){
            //Serial.println(f.setSpeedFromADC());
            //delay(100);
            //FOC::getInstance().doTheMagic2();
            //delayMicroseconds(40);
        }



}


void loop() {
    //int kam[10000]={0};


    //Serial.println("POS");
    //@ 1:plot all 3 duty cycles to see if there are in unision
    //@ 2: test the modulo index offset calculation, plot it after scaling as well for debugging
    //@ 3: hardware init contuniuation for multi motor setup
    //@ 4: proper order of U V W and its documentation, drawing
    //@ 6: speed calculation
    //@TODO proper FOC class constructor for multi motor setup
    //@TODO header - cpp proper placing of functions
    //@ 5: initial spin
    //@7: SetSpeedFromSomewhere implementation 1-ADC
    //@TODO PID

    //@TODO SetSpeedFromSomewhere implementation 2-Serial Interface For Speed
    //@TODO 2MBoard migration and tests
    //@TODO 1M board current measurement tryouts, torque measurement
    //@TODO Torque control tryouts


}
