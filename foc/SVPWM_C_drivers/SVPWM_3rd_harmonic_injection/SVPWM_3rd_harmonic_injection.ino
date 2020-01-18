
#include <stdarg.h>
#define RPM 180 //dummy rpm to adjust the delay
#define PWM_FREQ 20000
#define MAX_DUTY_CYCLE ((F_BUS/PWM_FREQ)/2)
#define DEBUG_MODE 0
#define DEBUG_PLOT 1

double modulation_index =  2/sqrt(3); // this is the max value for 3rd Harmonic injection, min -> 0, max -> (2/sqrt(3))

uint8_t LED_ON = true;
const int pinLED = 13;
uint8_t pin_on= true;
float dummy_angle = 180;

uint16_t my_delay = ((RPM/60)*1000)/360; // dummy delay derived from the given rpm


/*
* This functions takes the angle in degrees and 
* calculates the appropripirate duty cycles for 
* the given pins (the pin numbers must correspond 
* to FTM0 PINS 5,6,9,10,20,21,22,23)
*
*/

void SPWM_calculate_and_update_duty_cyle(uint8_t pin1,uint8_t pin2,uint8_t pin3, float angle){
  

  float r1=0;
  float r2=0;
  float t7=0;
  float t0=0;
  float t1=0;
  float t2=0;
  uint16_t p1_duty_cycle=0;
  uint16_t p2_duty_cycle=0;
  uint16_t p3_duty_cycle=0;
  
  if(angle == 360){
    angle = 0;
    }


  
  if(angle >= 0 && angle < 60 ){
    
   r1= (sqrt(3)/2)*modulation_index*sin((60-angle)*DEG_TO_RAD);
   r2= (sqrt(3)/2)*modulation_index*sin((angle)*DEG_TO_RAD);
   
   t1=r1*(MAX_DUTY_CYCLE);
   t2=r2*(MAX_DUTY_CYCLE);
   t0= (MAX_DUTY_CYCLE/2)*(1- modulation_index*cos(angle*DEG_TO_RAD) + (modulation_index/6)*cos(3*angle*DEG_TO_RAD));
   t7= MAX_DUTY_CYCLE - (t1 +t2 + t0);
   
   p1_duty_cycle=t2+t7+t1;
   p2_duty_cycle=t2+t7;
   p3_duty_cycle=t7;
   
   } 
  else if (angle >= 60 && angle < 120 ){
   r1= (sqrt(3)/2)*modulation_index*sin((60-(angle-60))*DEG_TO_RAD);
   r2= (sqrt(3)/2)*modulation_index*sin((angle-60)*DEG_TO_RAD);
   
   t1=r1*(MAX_DUTY_CYCLE);
   t2=r2*(MAX_DUTY_CYCLE);
   t0= (MAX_DUTY_CYCLE/2)*(1- modulation_index*cos((angle-60)*DEG_TO_RAD) + (modulation_index/6)*cos(3*(angle-60)*DEG_TO_RAD));
   t7= MAX_DUTY_CYCLE - (t1 + t2 + t0);
   
   p1_duty_cycle=t1+t0;
   p2_duty_cycle=t1+t2+t0;
   p3_duty_cycle=t0;
   
   } 
   else  if(angle >= 120 && angle < 180 ){
    
   r1= (sqrt(3)/2)*modulation_index*sin((60-(angle-120))*DEG_TO_RAD);
   r2= (sqrt(3)/2)*modulation_index*sin((angle-120)*DEG_TO_RAD);
   
   t1=r1*(MAX_DUTY_CYCLE);
   t2=r2*(MAX_DUTY_CYCLE);
   t0= (MAX_DUTY_CYCLE/2)*(1- modulation_index*cos((angle-120)*DEG_TO_RAD) + (modulation_index/6)*cos(3*(angle-120)*DEG_TO_RAD));
   t7= MAX_DUTY_CYCLE - (t1 +t2 + t0);
   
   p1_duty_cycle=t7;
   p2_duty_cycle=t1+t2+t7;
   p3_duty_cycle=t2+t7;
  
   
   } 
   else if(angle >= 180 && angle < 240 ){
   r1= (sqrt(3)/2)*modulation_index*sin((60-(angle-180))*DEG_TO_RAD);
   r2= (sqrt(3)/2)*modulation_index*sin((angle-180)*DEG_TO_RAD);
   
   t1=r1*(MAX_DUTY_CYCLE);
   t2=r2*(MAX_DUTY_CYCLE);
   t0= (MAX_DUTY_CYCLE/2)*(1- modulation_index*cos((angle-180)*DEG_TO_RAD) + (modulation_index/6)*cos(3*(angle-180)*DEG_TO_RAD));
   t7= MAX_DUTY_CYCLE - (t1 + t2 + t0);
   
   p1_duty_cycle=t0;
   p2_duty_cycle=t1+t0;
   p3_duty_cycle=t1+t2+t0;
   
   } 
   else if(angle >= 240 && angle < 300 ){
   r1= (sqrt(3)/2)*modulation_index*sin((60-(angle-240))*DEG_TO_RAD);
   r2= (sqrt(3)/2)*modulation_index*sin((angle-240)*DEG_TO_RAD);
   
   t1=r1*(MAX_DUTY_CYCLE);
   t2=r2*(MAX_DUTY_CYCLE);
   t0= (MAX_DUTY_CYCLE/2)*(1- modulation_index*cos((angle-240)*DEG_TO_RAD) + (modulation_index/6)*cos(3*(angle-240)*DEG_TO_RAD));
   t7= MAX_DUTY_CYCLE - (t1 + t2 + t0);
   
   p1_duty_cycle=t2+t7;
   p2_duty_cycle=t7;
   p3_duty_cycle=t1+t2+t7;
    
   
   } 
   else  if(angle >= 300 && angle < 360 ){

   r1= (sqrt(3)/2)*modulation_index*sin((60-(angle-300))*DEG_TO_RAD);
   r2= (sqrt(3)/2)*modulation_index*sin((angle-300)*DEG_TO_RAD);
   
   t1=r1*(MAX_DUTY_CYCLE);
   t2=r2*(MAX_DUTY_CYCLE);
   t0= (MAX_DUTY_CYCLE/2)*(1- modulation_index*cos((angle-300)*DEG_TO_RAD) + (modulation_index/6)*cos(3*(angle-300)*DEG_TO_RAD));
   t7= MAX_DUTY_CYCLE - (t1 + t2 + t0);
   
   p1_duty_cycle=t1+t2+t0;
   p2_duty_cycle=t0;
   p3_duty_cycle=t1+t0;
    
   
   } 

#if DEBUG_MODE 
                     Serial.print("\n\nANGLE: ");
           Serial.println(angle);
           Serial.print("PIN 1 DUTY CYCLE : ");
           Serial.println(p1_duty_cycle);
           Serial.print("PIN 2 DUTY CYCLE : ");
           Serial.println(p2_duty_cycle);
           Serial.print("PIN 3 DUTY CYCLE : ");
           Serial.println(p3_duty_cycle);
           Serial.println(MAX_DUTY_CYCLE);
           Serial.print("t1 DUTY CYCLE : ");
           Serial.println(t1);
           Serial.print("t2 DUTY CYCLE : ");
           Serial.println(t2);
           Serial.print("t7 DUTY CYCLE : ");
           Serial.println(t7);
           Serial.print("t1 + t2 + t7 : ");
           Serial.println(t1+t2+t7);
           
#endif

#if DEBUG_PLOT

      Serial.print(p1_duty_cycle);
      Serial.print(" ");
      Serial.print(p2_duty_cycle);
      Serial.print(" ");
      Serial.print(p3_duty_cycle);
      Serial.print(" ");
      Serial.println(angle);
      //Serial.println(p2_duty_cycle-p1_duty_cycle);
#endif

  change_pwm_duty_cycle(pin1,p1_duty_cycle);
  change_pwm_duty_cycle(pin2,p2_duty_cycle);
  change_pwm_duty_cycle(pin3,p3_duty_cycle);
  
  
 
  
  }
/*
This function changes the duty cycle of the given pin, 
it doesnt check if its within the correct range of values,
this should be done before passing the parameter
*/
void change_pwm_duty_cycle(int pin_name, int duty_cycle){
  
 switch(pin_name){
      case 5:
         FTM0_C7V = duty_cycle; //Teency pin 5 -> FTM0_CH7
        break;
       case 6:
         FTM0_C4V = duty_cycle; //Teency pin 6 -> FTM0_CH4
         break;
       case 9:
         FTM0_C2V = duty_cycle;; //Teency pin 9 -> FTM0_CH2
         break;
       case 10:
         FTM0_C3V = duty_cycle; //Teency pin 10 -> FTM0_CH3
         break;
       case 20:
         FTM0_C5V = duty_cycle; //Teency pin 20 -> FTM0_CH5
         break;
       case 21:
         FTM0_C6V = duty_cycle; //Teency pin 21 -> FTM0_CH6
         break;
       case 22:
         FTM0_C0V = duty_cycle; //Teency pin 22 (A8) -> FTM0_CH0
         break;
       case 23:
         FTM0_C1V = duty_cycle; //Teency pin 23 (A9) -> FTM0_CH1
         break;
       default:
         Serial.println("errrorrr");
         break;
      
      }
  
  
  
  }
/*
Variadic function to initialize the givens pins,
after the pwm frequncy and the number of pins to
be initialized is given, simply enter the desired pins.
The pins must be FTM0 controlled pins

*/
void set_PWM_pins(int freq, int number_of_pins...)
{
   
   FTM0_SC=0;
   
   FTM0_CONF=0xC0; //set up BDM in 11 
   FTM0_FMS=0x00; //clear the WPEN so that WPDIS is set in FTM0_MODE 

//FTM0_MODE|=0x05; // 0000 0101
   FTM0_MODE = 0b00000110; // 00000111
    FTM0_MOD = (F_BUS/freq)/2;
    Serial.println(FTM0_MOD);

   
   va_list list;
 

   va_start(list, number_of_pins); 
   
   int arg;
   int i=0;
   for(i=0;i<number_of_pins;i++){

    arg = va_arg(list, int);
#if DEBUG_MODE     

    Serial.print("the argument is ");
    Serial.println(arg);
#endif

    switch(arg){
      case 5:
        FTM0_C7SC = 0b00101000;
        FTM0_C7V = MAX_DUTY_CYCLE/2; //50%
        PORTD_PCR7 |= PORT_PCR_MUX(4) | PORT_PCR_DSE | PORT_PCR_SRE; //Teency pin 5 -> FTM0_CH7
        break;
       case 6:
         FTM0_C4SC = 0b00101000;
         FTM0_C4V = MAX_DUTY_CYCLE/2; //50%
         PORTD_PCR4 |= PORT_PCR_MUX(4) | PORT_PCR_DSE | PORT_PCR_SRE; //Teency pin 6 -> FTM0_CH4
         break;
       case 9:
         FTM0_C2SC = 0b00101000;
         FTM0_C2V = MAX_DUTY_CYCLE/2; //50%
         PORTC_PCR3 |= PORT_PCR_MUX(4) | PORT_PCR_DSE | PORT_PCR_SRE; //Teency pin 9 -> FTM0_CH2
         break;
       case 10:
         FTM0_C3SC = 0b00101000;
         FTM0_C3V = MAX_DUTY_CYCLE/2; //50%
         PORTC_PCR4 |= PORT_PCR_MUX(4) | PORT_PCR_DSE | PORT_PCR_SRE; //Teency pin 10 -> FTM0_CH3
         break;
       case 20:
         FTM0_C5SC = 0b00101000;
         FTM0_C5V = MAX_DUTY_CYCLE/2; //50%
         PORTD_PCR5 |= PORT_PCR_MUX(4) | PORT_PCR_DSE | PORT_PCR_SRE; //Teency pin 20 -> FTM0_CH5
         break;
       case 21:
         FTM0_C6SC = 0b00101000;
         FTM0_C6V = MAX_DUTY_CYCLE/2; //50%
         PORTD_PCR6 |= PORT_PCR_MUX(4) | PORT_PCR_DSE | PORT_PCR_SRE; //Teency pin 21 -> FTM0_CH6
         break;
       case 22:
         FTM0_C0SC = 0b00101000;
         FTM0_C0V = MAX_DUTY_CYCLE/2; //50%
         PORTC_PCR1 |= PORT_PCR_MUX(4) | PORT_PCR_DSE | PORT_PCR_SRE; //Teency pin 22 (A8) -> FTM0_CH0
         break;
       case 23:
         FTM0_C1SC = 0b00101000;
         FTM0_C1V = MAX_DUTY_CYCLE/2; //50%
         PORTC_PCR2 |= PORT_PCR_MUX(4) | PORT_PCR_DSE | PORT_PCR_SRE; //Teency pin 23 (A9) -> FTM0_CH1
         break;
       default:
#if DEBUG_MODE 
         Serial.println("errrorrr");
#endif
         break;
      
      }
      
   
    }


   va_end(list);

   FTM0_CNTIN = 0x00;


FTM0_SC = 0b01101000; //CPWM MODE 0x48 EPWM -> 0x68 0110 1000
}


/*interrupt routine to be called, it was needed to trigger the oscilloscope not doing anything right now*/
void ftm0_isr(void)
{   
    FTM0_SC &= ~FTM_SC_TOF;
    static uint32_t cnt;

    
    
    if(cnt < 10000 ) { // just to slow it down
      cnt++;

    }
    else {
    if (LED_ON == true) { 
    digitalWriteFast(pinLED, HIGH); 
    }
    else { digitalWriteFast(pinLED, LOW);
    }
    LED_ON = !LED_ON;
      cnt=0;}

} 

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  delay(1000);
  pinMode(4, OUTPUT);
  pinMode(pinLED, OUTPUT);
  set_PWM_pins(PWM_FREQ,5,23,22,5,6,9);
  cli(); //Disable global interrupts
  NVIC_SET_PRIORITY(IRQ_FTM0, 64); 
  NVIC_ENABLE_IRQ(IRQ_FTM0);
  sei(); //Enable global interrupts

}

void loop() {
  delay(my_delay*2);

       if(pin_on){
      digitalWriteFast(4, HIGH);
      }
      else {
      digitalWriteFast(4, LOW); 
        }
    pin_on = !pin_on;
   dummy_angle+=1;
   if(dummy_angle==361){dummy_angle=1;}


   
   SPWM_calculate_and_update_duty_cyle(23,6,9, dummy_angle);
  // put your main code here, to run repeatedly:

}
