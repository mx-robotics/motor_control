
#include <Arduino.h>
#include <FOC.h>

#define INT_FIRAT 1




void ftm0_isr(void)
{
    FTM0_SC &= ~FTM_SC_TOF;
    tuw::FOC::getInstance().doTheMagic();




}



void setup() {

    tuw::FOC::getInstance().initPins();
    Serial.begin(9600);
    while(!Serial);

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