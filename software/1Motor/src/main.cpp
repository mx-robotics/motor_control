#include <Arduino.h>
#include <Foc.h>

#define INT_FIRAT 0




void ftm0_isr(void)
{
    FTM0_SC &= ~FTM_SC_TOF;
    tuw::FOC::getInstance().doTheMagic();




}



void setup() {

    tuw::FOC::getInstance().initPins();
    Serial.begin(9600);
    while(!Serial);
    tuw::FOC::getInstance().calculate_offset_parameters();

#if INT_FIRAT
    cli(); //Disable global interrupts
    NVIC_SET_PRIORITY(IRQ_FTM0, 64);
    NVIC_ENABLE_IRQ(IRQ_FTM0);
    sei(); //Enable global interrupts

#endif

}


void loop() {

    tuw::FOC::getInstance().setSpeedFromADC();


}

