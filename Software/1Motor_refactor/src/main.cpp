//
// Created by firat on 20.01.20.
//
#include "Motor.h"
#include <array>
#include "SVPWMLookUpTable.h"

/*

constexpr auto generate()
{
    std::array<uint8_t , 5000> res{};
    //bool res[64];
    for(int i = 0;i<5000;i++){
        res[i]= 123;
    }
    return res;
}
*/
std::array <uint16_t , 1000> kam;

#define INT_FIRAT 1
void setup() {
    constexpr auto kam1=SVPWMLookUpTable::generate();

    //constexpr auto table = generate();

   for(int i =0;i<1000;i++){
        kam[i]=kam1[i];
    }
    //constexpr InhibitPins inhibitPins_{23, 24, 25};
    //constexpr InitPins initPins{1,2,3};
    //constexpr SPIPins spiPins{1,2,3};
    //constexpr ISPins isPins {1,2,3};
    //Motor x(inhibitPins_,initPins,spiPins,isPins,Direction::STOP);
    //x.direction = Direction::CCW;
    //constexpr uint8_t vint = 12;
    //const uint8_t * ptr1 = &vint;
   //const std::array <const uint8_t* ,inhibitPins_.InhibitPinU> y = {{ptr1}};
    //auto kk = table[12];


#if INT_FIRAT
    cli(); //Disable global interrupts
    NVIC_SET_PRIORITY(IRQ_FTM0, 64);
    NVIC_ENABLE_IRQ(IRQ_FTM0);
    sei(); //Enable global interrupts

#endif

}


void loop() {
    //int kam[10000]={0};
    //kam[12] = 12;
    // tuw::FOC::getInstance().setSpeedFromADC();


}
