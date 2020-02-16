#include "Foc.h"


using namespace tuw;

FOC::FOC() {

}


FOC &FOC::getInstance() {
    static FOC instance;
    return instance;


}


void FOC::initSPI() {
    pinMode(CS, OUTPUT);
    SPI.begin();
    digitalWrite(CS, HIGH);   //sensor is of the low - active type
    SPI.setSCK(CLK);

}

void FOC::SPITransfer() {
    SPI.beginTransaction(
            spiSettings);   //SPI settings according to sensor datasheet: clock: max. 10MHz | MSB first | SPI Mode 1  | CPOL=0, CPHA= 1

    digitalWriteFast(CS, LOW); //SPI Mode 1 -> information gest sampled with falling endge
    uint16_t angle = SPI.transfer16(0xFFFF) &
                     0x3FFFu; //command bit15 = 1 (parity) | bit14 = 1 (read) | adresss 0x000 -> B11000000 00000000 = 0xC0000;
    digitalWriteFast(CS, HIGH); //SPI Mode 1 -> receive information with rising edge
    SPI.endTransaction();
    if (abs(angle - angle_now) > 30) {
        sensor_overflow = 1;


    }
    angle_now = angle;


    //16 bit word received, but resolution is 14 bit -> mask out first 2 MSB -> angle & B00111111 11111111 = angle & 0x3FFF;
    rotor_position = fast_mod(angle, 1489) * 0.24169f;
    Serial.println(rotor_position);
    /******* get the model according to 1489 instead f 360 *************************/



}


void FOC::initInhibitPIN() {
    pinMode(INH_W, OUTPUT);
    pinMode(INH_U, OUTPUT);
    pinMode(INH_V, OUTPUT);

}

void FOC::activateInhibitPins() {
    digitalWriteFast(INH_W, HIGH);
    digitalWriteFast(INH_U, HIGH);
    digitalWriteFast(INH_V, HIGH);


}


void FOC::initServoPins() {
    pinMode(S_PWM1, OUTPUT);
    pinMode(S_PWM2, OUTPUT);
}


void FOC::setInputPinsDutyCycle() {

    FTM0_C6V = IN_W_duty_cycle; //Teency pin 21 -> FTM0_CH6
    FTM0_C0V = IN_U_duty_cycle; //Teency pin 22 (A8) -> FTM0_CH0
    FTM0_C1V = IN_V_duty_cycle; //Teency pin 23 (A9) -> FTM0_CH1

}


void FOC::calculateInputPinsDutyCycle() {
    float r1 = 0;
    float r2 = 0;
    float t7 = 0;
    uint32_t t0 = 0;
    uint32_t t1 = 0;
    uint32_t t2 = 0;
    float angle = rotor_position;

    if (angle >= 0 && angle < 60) {

        r1 = (sqrt(3) / 2) * modulation_index * sin((60 - angle) * DEG_TO_RAD);
        r2 = (sqrt(3) / 2) * modulation_index * sin((angle) * DEG_TO_RAD);

        t1 = r1 * (MAX_DUTY_CYCLE);
        t2 = r2 * (MAX_DUTY_CYCLE);
        t0 = (MAX_DUTY_CYCLE / 2) *
             (1 - modulation_index * cos(angle * DEG_TO_RAD) + (modulation_index / 6) * cos(3 * angle * DEG_TO_RAD));
        t7 = MAX_DUTY_CYCLE - (t1 + t2 + t0);

        IN_W_duty_cycle = (t2 + t7 + t1);
        IN_U_duty_cycle = t2 + t7;
        IN_V_duty_cycle = t7;

    } else if (angle >= 60 && angle < 120) {
        r1 = (sqrt(3) / 2) * modulation_index * sin((60 - (angle - 60)) * DEG_TO_RAD);
        r2 = (sqrt(3) / 2) * modulation_index * sin((angle - 60) * DEG_TO_RAD);

        t1 = r1 * (MAX_DUTY_CYCLE);
        t2 = r2 * (MAX_DUTY_CYCLE);
        t0 = (MAX_DUTY_CYCLE / 2) * (1 - modulation_index * cos((angle - 60) * DEG_TO_RAD) +
                                     (modulation_index / 6) * cos(3 * (angle - 60) * DEG_TO_RAD));
        t7 = MAX_DUTY_CYCLE - (t1 + t2 + t0);

        IN_W_duty_cycle = t1 + t0;
        IN_U_duty_cycle = t1 + t2 + t0;
        IN_V_duty_cycle = t0;

    } else if (angle >= 120 && angle < 180) {

        r1 = (sqrt(3) / 2) * modulation_index * sin((60 - (angle - 120)) * DEG_TO_RAD);
        r2 = (sqrt(3) / 2) * modulation_index * sin((angle - 120) * DEG_TO_RAD);

        t1 = r1 * (MAX_DUTY_CYCLE);
        t2 = r2 * (MAX_DUTY_CYCLE);
        t0 = (MAX_DUTY_CYCLE / 2) * (1 - modulation_index * cos((angle - 120) * DEG_TO_RAD) +
                                     (modulation_index / 6) * cos(3 * (angle - 120) * DEG_TO_RAD));
        t7 = MAX_DUTY_CYCLE - (t1 + t2 + t0);

        IN_W_duty_cycle = t7;
        IN_U_duty_cycle = t1 + t2 + t7;
        IN_V_duty_cycle = t2 + t7;


    } else if (angle >= 180 && angle < 240) {
        r1 = (sqrt(3) / 2) * modulation_index * sin((60 - (angle - 180)) * DEG_TO_RAD);
        r2 = (sqrt(3) / 2) * modulation_index * sin((angle - 180) * DEG_TO_RAD);

        t1 = r1 * (MAX_DUTY_CYCLE);
        t2 = r2 * (MAX_DUTY_CYCLE);
        t0 = (MAX_DUTY_CYCLE / 2) * (1 - modulation_index * cos((angle - 180) * DEG_TO_RAD) +
                                     (modulation_index / 6) * cos(3 * (angle - 180) * DEG_TO_RAD));
        t7 = MAX_DUTY_CYCLE - (t1 + t2 + t0);

        IN_W_duty_cycle = t0;
        IN_U_duty_cycle = t1 + t0;
        IN_V_duty_cycle = t1 + t2 + t0;

    } else if (angle >= 240 && angle < 300) {
        r1 = (sqrt(3) / 2) * modulation_index * sin((60 - (angle - 240)) * DEG_TO_RAD);
        r2 = (sqrt(3) / 2) * modulation_index * sin((angle - 240) * DEG_TO_RAD);

        t1 = r1 * (MAX_DUTY_CYCLE);
        t2 = r2 * (MAX_DUTY_CYCLE);
        t0 = (MAX_DUTY_CYCLE / 2) * (1 - modulation_index * cos((angle - 240) * DEG_TO_RAD) +
                                     (modulation_index / 6) * cos(3 * (angle - 240) * DEG_TO_RAD));
        t7 = MAX_DUTY_CYCLE - (t1 + t2 + t0);

        IN_W_duty_cycle = t2 + t7;
        IN_U_duty_cycle = t7;
        IN_V_duty_cycle = t1 + t2 + t7;


    } else if (angle >= 300 && angle < 360) {

        r1 = (sqrt(3) / 2) * modulation_index * sin((60 - (angle - 300)) * DEG_TO_RAD);
        r2 = (sqrt(3) / 2) * modulation_index * sin((angle - 300) * DEG_TO_RAD);

        t1 = r1 * (MAX_DUTY_CYCLE);
        t2 = r2 * (MAX_DUTY_CYCLE);
        t0 = (MAX_DUTY_CYCLE / 2) * (1 - modulation_index * cos((angle - 300) * DEG_TO_RAD) +
                                     (modulation_index / 6) * cos(3 * (angle - 300) * DEG_TO_RAD));
        t7 = MAX_DUTY_CYCLE - (t1 + t2 + t0);

        IN_W_duty_cycle = t1 + t2 + t0;
        IN_U_duty_cycle = t0;
        IN_V_duty_cycle = t1 + t0;


    }


}


void FOC::doTheMagic() {
    static uint16_t control_counter=0;
    static uint8_t plot_counter =0;
    SPITransfer();
    control_counter++;
    if(control_counter>2000){

        control_counter = 0;
        calculateRPS();

        P_controller(target_rps,my_speed);

        memcpy( f_to_c_array + plot_counter, &my_speed, 4);
        memcpy( f_to_c_array + plot_counter + 4, &target_rps, 4 );
        memcpy( f_to_c_array + plot_counter + 8, &acc_error, 4 );
        memcpy( f_to_c_array + plot_counter + 12, &target_speed, 4 );
        plot_counter+=16;
        //Serial.println(target_rps);
        //Serial.println(my_speed);
    }
    if(plot_counter == 160){
        Serial.write(f_to_c_array,160);
        plot_counter = 0;
    }
    calculateInputPinsDutyCycleLUT();
    setInputPinsDutyCycle();


}



void FOC::initPins() {

    FOC::calculate_offset_parameters_short();
    initSPI();
    initInhibitPIN();
    //initInputPins();
    setPWMpins();

    initServoPins();
    activateInhibitPins();
    initADCconversions();


}

uint16_t FOC::getRotorPosition() {
    return rotor_position;
}

std::array<uint16_t, 360> FOC::getArray() {
    return array;

}

std::array<uint16_t, 360> FOC::createLUT() {
    std::array<uint16_t, 360> a;
    rotor_position = 0;
    for (uint16_t i = 0; i < a.size(); i++) {

        calculateInputPinsDutyCycle();
        a[i] = IN_W_duty_cycle;
        rotor_position += 1;


    }
    rotor_position = 0;
    IN_W_duty_cycle = 0;
    return a;


}

void FOC::calculateInputPinsDutyCycleLUT() {
    uint16_t modulo_index_offset = calculate_LUT_offset();

    IN_W_duty_cycle =
            array[fast_mod(rotor_position + sensor_offset + angle_offset * direction, 360)] * (target_speed * 0.01f) +
            modulo_index_offset;
    IN_V_duty_cycle = array[fast_mod(rotor_position + sensor_offset + angle_offset * direction + 120, 360)] *
                      (target_speed * 0.01f) + modulo_index_offset;
    IN_U_duty_cycle = array[fast_mod(rotor_position + sensor_offset + angle_offset * direction + 240, 360)] *
                      (target_speed * 0.01f) + modulo_index_offset;


}

void FOC::debugPlot() {

    for (size_t i = 0; i < tuw::FOC::getInstance().getArray().size(); i++) {
        delay(10);
        Serial.print(tuw::FOC::getInstance().getArray()[i]);
        Serial.print(",");
        //Serial.print(tuw::FOC::getInstance().getArray()[(i+120)%360]);
        //Serial.print(",");
        //Serial.println(tuw::FOC::getInstance().getArray()[(i+240)%360]);
        Serial.println(i);


    }


}

void FOC::calculate_offset_parameters() {
    for (int i = 0; i <360 ; ++i) {
    //this seems to be okay actually
        int j = 1;


            // max modulation index
            modulation_index = (2 / sqrt(3));
            rotor_position = i;
            float k1 = 0.2f*j;
            calculateInputPinsDutyCycle();

            //Serial.println(IN_U_duty_cycle);
            //Serial.println(IN_V_duty_cycle);
            uint16_t a1 = IN_W_duty_cycle * k1;
            Serial.println(a1);



            modulation_index *= (k1);
            calculateInputPinsDutyCycle();
            uint16_t x1 = IN_W_duty_cycle;
            Serial.println(IN_W_duty_cycle);

            float_t k2 = 0.4f;
            modulation_index = (2 / sqrt(3)); // max modul0 index again
            calculateInputPinsDutyCycle();


            uint16_t a2 = IN_W_duty_cycle * k2;
            modulation_index *= k2;
            calculateInputPinsDutyCycle();
            uint16_t x2 = IN_W_duty_cycle;


            uint16_t yy1 = x1 - a1;
            uint16_t yy2 = x2 - a2;

            uint16_t xx1 = k1*j * 100;
            uint16_t xx2 = k2 * 100;

            offset_param_m = (yy2 - yy1) / (xx2 - xx1);

            offset_param_c = yy2 - (offset_param_m * xx2);

            //uint16_t error = offset_param_m*k1*j*100 + offset_param_c;
            uint16_t error = abs(a1 - x1);
            Serial.println(error);
            /* Serial.print("rotor_position");
             Serial.print(i);

             Serial.print(", c = ");
             Serial.print(offset_param_c);
             Serial.print(", m = ");
             Serial.println(offset_param_m);

             */




        }

}

void FOC::calculate_offset_parameters_short() {

    offset_param_m = (-MAX_DUTY_CYCLE / 200);
    offset_param_c = (MAX_DUTY_CYCLE / 2);


}

inline uint16_t FOC::calculate_LUT_offset() {
    return offset_param_m * target_speed + offset_param_c;


}


void FOC::setPWMpins() {

    FTM0_SC = 0;

    FTM0_CONF = 0xC0; //set up BDM in 11
    FTM0_FMS = 0x00; //clear the WPEN so that WPDIS is set in FTM0_MODE

//FTM0_MODE|=0x05; // 0000 0101
    FTM0_MODE = 0b00000110; // 00000111
    FTM0_MOD = (F_BUS / PWM_FREQ) / 2;


    FTM0_C6SC = 0b00101000;
    FTM0_C6V = 0; //50%
    PORTD_PCR6 |= PORT_PCR_MUX(4) | PORT_PCR_DSE | PORT_PCR_SRE; //Teency pin 21 -> FTM0_CH6


    FTM0_C0SC = 0b00101000;
    FTM0_C0V = 0; //50%
    PORTC_PCR1 |= PORT_PCR_MUX(4) | PORT_PCR_DSE | PORT_PCR_SRE; //Teency pin 22 (A8) -> FTM0_CH0


    FTM0_C1SC = 0b00101000;
    FTM0_C1V = 0; //50%
    PORTC_PCR2 |= PORT_PCR_MUX(4) | PORT_PCR_DSE | PORT_PCR_SRE; //Teency pin 23 (A9) -> FTM0_CH1


    FTM0_CNTIN = 0x00;


    FTM0_SC = 0b01101000; //CPWM MODE 0x48 EPWM -> 0x68 0110 1000
}

void FOC::printLUToffsetValues() {
    calculate_offset_parameters();
    Serial.print(offset_param_m);
    Serial.print("x + ");
    Serial.println(offset_param_c);

    Serial.println("Calculate with short");
    calculate_offset_parameters_short();
    Serial.print(offset_param_m);
    Serial.print("x + ");
    Serial.println(offset_param_c);
}

void FOC::printMaxDutyCylcle() {
    Serial.print("MAX DUTY CYCLE");
    Serial.println(MAX_DUTY_CYCLE);

}


void FOC::setSpeedSerial() {

    String kx;
    while (Serial.available() > 0) {
        char x = Serial.read();
        if (x != '\n') {
            kx += x;
        } else {
            break;
        }


    }
    target_speed = kx.toInt();
    Serial.print("trgt spd: ");
    Serial.println(target_speed);

}

void FOC::setSpeedFromADC() {

    if (adc->isComplete()) {
        uint_fast16_t value1 = adc->analogReadContinuous(ADC_PIN);
        target_rps =  -12.5f + (value1 * 25.0f / adc->getMaxValue(ADC_0));
        //target_speed =  (value1 * 100.0f / adc->getMaxValue(ADC_0));

        //Serial.println(target_speed);
    }

}

void FOC::initADCconversions() {

    pinMode(ADC_PIN, INPUT);

    adc->setAveraging(4); // set number of averages
    adc->setResolution(12); // set bits of resolution
    adc->setConversionSpeed(ADC_CONVERSION_SPEED::VERY_HIGH_SPEED); // change the conversion speed
    adc->setSamplingSpeed(ADC_SAMPLING_SPEED::VERY_HIGH_SPEED); // change the sampling speed
    adc->startContinuous(ADC_PIN);

}


float FOC::getMySpeed() const { // current speed - RPS
    return my_speed;
}

volatile float FOC::getTargetSpeed() const {
    return target_speed;
}


void FOC::PID_controller(float rps_target, float rps_measured){
    float error = rps_target - rps_measured; // error in rps

    static float kp = 0.5f;
    static float ki= 0.2f;
    static float kd = 0.1f;

    static float measurement_last = 0;
    static float measurement_now = 0;
    static float error_sum = 0;
    static bool wind_up = false;
    measurement_last = measurement_now;
    measurement_now = rps_measured;
    if(wind_up == false) {
        error_sum += error;
    }
    float proportional = (error * kp);
    float integral = error_sum * ki; // here might be a problem
    float derivative = kd * ( measurement_now - measurement_last);

    float command =  (proportional + integral - derivative) * (11.22f - 0.22f*fabsf(rps_target));

    if (command > 99) {
        wind_up = true;
        command = 99;}
    else{
        wind_up = false;
    }


    target_speed = command;



}


void FOC::P_controller(float rps_target, float rps_measured) {

    float error = rps_target - rps_measured; // error in rps

    float kp = 0.5f;
    float ki= 0.2f;
    float kd = 0.1f;

    static float measurement_last = 0;
    static float measurement_now = 0;
    static float error_sum = 0;


    measurement_last = measurement_now;
    measurement_now = rps_measured;


    error_sum += (error * ki);

    float proportional = (error * kp);
    float integral = error_sum; // here might be a problem
    float derivative = kd * ( measurement_now - measurement_last);

    if(integral > 9.9f){
        integral = 9.9f;
    }

    if(integral < -9.9f){
        integral = -9.9f;
    }



    //else if(integral < 0.5f){

    //    integral = 0.5f;
    // }
    acc_error = error_sum;

    float command =  (proportional + integral - derivative) * (11.22f - 0.22f*fabs(rps_target));
    if(command > 0){
        if (command > 99) { command = 99; }
        direction = 1;
        //if (command < 5) { command = 5; }
    }
    else{
        if(command < -99){ command = -99; }
        direction = -1;

    }
    target_speed = abs(command);

    //command < 0 ? direction = -1 : direction = 1;



}


void FOC::led_foo() {
    FTM0_SC &= (~FTM_SC_TOF);
    static uint32_t cnt;
    static bool LED_ON = true;


    if (cnt < 10000) {
        cnt++;

    } else {
        if (LED_ON) {
            digitalWriteFast(13, HIGH);
        } else {
            digitalWriteFast(13, LOW);
        }
        LED_ON = !LED_ON;
        cnt = 0;
    }

}


void FOC::calculateSensorOffsetAngle() {
    target_speed = 100;

    uint16_t my_psudo_rotor_posion = 100;


    IN_W_duty_cycle = (array[my_psudo_rotor_posion]);
    IN_V_duty_cycle = array[fast_mod(my_psudo_rotor_posion + 120, 360)];
    IN_U_duty_cycle = array[fast_mod(my_psudo_rotor_posion + 240, 360)];

    setInputPinsDutyCycle();


}

void FOC::getModulationIndexSpeedValues() {

    uint32_t before = millis();
    target_speed = 5;
    while (1) {


        if (millis() - before > 200) {
            Serial.println(target_speed);
            Serial.println(my_speed);
            target_speed++;
            before = millis();

            //}
        }
        if (target_speed > 99) {
            break;
        }


    }


}


void FOC::calculateRPS() {

    {

        static int16_t angle_before = 0;
        int16_t angle_now_local=angle_now;

        if (sensor_overflow == 1) {
            angle_now_local += 16384 * direction;
            sensor_overflow = 0;
        }

        my_speed = ((angle_now_local - angle_before) / 16384.0f) * 10; // 10 cuz are doing this 10 time per sec
        angle_before = angle_now;

        //tuw::FOC::getInstance().P_controller(6);

    }


}

volatile float FOC::getTargetRps() const {
    return target_rps;
}
