
#include <Arduino.h>
#include <FOC.h>

#define INT_FIRAT 1




void ftm0_isr(void)
{
    FTM0_SC &= ~FTM_SC_TOF;
    tuw::FOC::getInstance().doTheMagic();





}



void setup() {


    while(!Serial);
    tuw::FOC::getInstance().initPins();
    //pinMode(13,OUTPUT);

    Serial.begin(9600);


#if INT_FIRAT
    cli(); //Disable global interrupts
    NVIC_SET_PRIORITY(IRQ_FTM0, 64);
    NVIC_ENABLE_IRQ(IRQ_FTM0);
    sei(); //Enable global interrupts
#endif

}


void loop() {
    //elapsedMicros waiting;
    //tuw::FOC::getInstance().printLUToffsetValues();

    //tuw::FOC::getInstance().calculateSensorOffsetAngle();
    //tuw::FOC::getInstance().SPITransfer();
    //tuw::FOC::getInstance().doTheMagic();

    //tuw::FOC::getInstance().setSpeedFromADC();

    //tuw::FOC::getInstance().P_controller(500);
    
    //tuw::FOC::getInstance().getModulationIndexSpeedValues();
    //uint32_t now = waiting;
    //Serial.println(now);
    //Serial.println(tuw::FOC::getInstance().getMySpeed());
    //Serial.println(tuw::FOC::getInstance().getTargetSpeed());
    //Serial.println(tuw::FOC::getInstance().getTargetRps());

    //Serial.println(tuw::FOC::getInstance().getAngleDiff2()*3662.1f/60);

    //Serial.println(tuw::FOC::getInstance().getTimeDiff());
    //Serial.println(200);
    //tuw::FOC::getInstance().debugPlot();


    //Serial.println(tuw::FOC::getInstance().getRotorPosition());

    //tuw::FOC::getInstance().printMaxDutyCylcle();
    //tuw::FOC::getInstance().setSpeedSerial();

    //Serial.print("time elapsed ");








    //delay(1);





}