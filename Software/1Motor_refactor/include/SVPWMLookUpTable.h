//
// Created by firat on 21.01.20.
//

#ifndef INC_1MOTOR_REFACTOR_SVPWMLOOKUPTABLE_H
#define INC_1MOTOR_REFACTOR_SVPWMLOOKUPTABLE_H

#include <Arduino.h>
#include <array>

class SVPWMLookUpTable {
private:
    static constexpr uint16_t maxSensorResolution = 16384;
    static constexpr uint8_t motorPoleCount = 11;
    static constexpr uint16_t LUTSize = maxSensorResolution / motorPoleCount;
    static constexpr uint16_t Sixty_degree_equivelant = LUTSize / 6;
    //static constexpr float calculation_const = sqrt(3/2.0f);
    static constexpr uint16_t PWM_FREQ = 20000;
    static constexpr uint16_t MAX_DUTY_CYCLE =  (F_BUS/PWM_FREQ)/2;
public:
    constexpr static  std::array<uint16_t, LUTSize> generate() {

        std::array <uint16_t, LUTSize> temp_table{};

        float r1 = 0;
        float r2 = 0;
        float t7 = 0;
        float t0 = 0;
        float t1 = 0;
        float t2 = 0;
        uint16_t IN_W_duty_cycle = 0;
        //uint32_t IN_U_duty_cycle;
        //uint32_t IN_V_duty_cycle;
        float modulation_index = 1.1547005f;
        float angle_scaler = 360.0f/LUTSize;
        uint16_t oneSixthLUTSize = LUTSize/6;

        uint angle = 0;
        for (int i = 0; i < LUTSize; i++) {

            if (angle >= 0 && angle < oneSixthLUTSize) {

                r1 =  sin((60 - angle*angle_scaler) * DEG_TO_RAD);
                r2 =  sin(angle*angle_scaler * DEG_TO_RAD);


                t1 = r1 * MAX_DUTY_CYCLE;
                t2 = r2 * (MAX_DUTY_CYCLE);
                t0 = (MAX_DUTY_CYCLE / 2) *
                     (1 - modulation_index * cos(angle*angle_scaler * DEG_TO_RAD) + (modulation_index / 6) * cos(3 * angle*angle_scaler * DEG_TO_RAD));
                t7 = MAX_DUTY_CYCLE - (t1 + t2 + t0);

                IN_W_duty_cycle = (t2 + t7 + t1);
                //IN_U_duty_cycle = t2 + t7;
                //IN_V_duty_cycle = t7;

                } else if (angle >= oneSixthLUTSize && angle < oneSixthLUTSize*2 ) {
                     r1 = sin((60 - (angle*angle_scaler - 60)) * DEG_TO_RAD);
                     //r2 = sin((angle - 60) * DEG_TO_RAD);

                     t1 = r1 * (MAX_DUTY_CYCLE);
                     //t2 = r2 * (MAX_DUTY_CYCLE);
                     t0 = (MAX_DUTY_CYCLE / 2) * (1 - modulation_index * cos((angle*angle_scaler - 60) * DEG_TO_RAD) +
                                                  (modulation_index / 6) * cos(3 * (angle*angle_scaler - 60) * DEG_TO_RAD));
                     //t7 = MAX_DUTY_CYCLE - (t1 + t2 + t0);

                     IN_W_duty_cycle = t1 + t0;
                     //IN_U_duty_cycle = t1 + t2 + t0;
                     //IN_V_duty_cycle = t0;

                 } else if (angle >= oneSixthLUTSize*2 && angle < oneSixthLUTSize*3 ) {

                     r1 =  sin((60 - (angle*angle_scaler - 120)) * DEG_TO_RAD);
                     r2 =  sin((angle*angle_scaler - 120) * DEG_TO_RAD);

                     t1 = r1 * (MAX_DUTY_CYCLE);
                     t2 = r2 * (MAX_DUTY_CYCLE);
                     t0 = (MAX_DUTY_CYCLE / 2) * (1 - modulation_index * cos((angle*angle_scaler - 120) * DEG_TO_RAD) +
                                                  (modulation_index / 6) * cos(3 * (angle*angle_scaler - 120) * DEG_TO_RAD));
                     t7 = MAX_DUTY_CYCLE - (t1 + t2 + t0);

                     IN_W_duty_cycle = t7;
                     //IN_U_duty_cycle = t1 + t2 + t7;
                     //IN_V_duty_cycle = t2 + t7;


                 } else if (angle >= oneSixthLUTSize*3 && angle < oneSixthLUTSize*4) {
                     //r1 = sin((60 - (angle - 180)) * DEG_TO_RAD);
                     //r2 = sin((angle - 180) * DEG_TO_RAD);

                     //t1 = r1 * (MAX_DUTY_CYCLE);
                     //t2 = r2 * (MAX_DUTY_CYCLE);
                     t0 = (MAX_DUTY_CYCLE / 2) * (1 - modulation_index * cos((angle*angle_scaler - 180) * DEG_TO_RAD) +
                                                  (modulation_index / 6) * cos(3 * (angle*angle_scaler - 180) * DEG_TO_RAD));
                     //t7 = MAX_DUTY_CYCLE - (t1 + t2 + t0);

                     IN_W_duty_cycle = t0;
                     //IN_U_duty_cycle = t1 + t0;
                     //IN_V_duty_cycle = t1 + t2 + t0;

                 } else if (angle >= oneSixthLUTSize*4 && angle < oneSixthLUTSize*5) {
                     r1 =  sin((60 - (angle*angle_scaler - 240)) * DEG_TO_RAD);
                     r2 =  sin((angle*angle_scaler - 240) * DEG_TO_RAD);

                     t1 = r1 * (MAX_DUTY_CYCLE);
                     t2 = r2 * (MAX_DUTY_CYCLE);
                     t0 = (MAX_DUTY_CYCLE / 2) * (1 - modulation_index * cos((angle*angle_scaler - 240) * DEG_TO_RAD) +
                                                  (modulation_index / 6) * cos(3 * (angle*angle_scaler - 240) * DEG_TO_RAD));
                     t7 = MAX_DUTY_CYCLE - (t1 + t2 + t0);

                     IN_W_duty_cycle = t2 + t7;
                     //IN_U_duty_cycle = t7;
                     //IN_V_duty_cycle = t1 + t2 + t7;


                 } else if (angle >= oneSixthLUTSize*5 && angle < LUTSize) {

                     r1 =   sin((60 - (angle*angle_scaler - 300)) * DEG_TO_RAD);
                     r2 =  sin((angle*angle_scaler - 300) * DEG_TO_RAD);

                     t1 = r1 * (MAX_DUTY_CYCLE);
                     t2 = r2 * (MAX_DUTY_CYCLE);
                     t0 = (MAX_DUTY_CYCLE / 2) * (1 - modulation_index * cos((angle*angle_scaler - 300) * DEG_TO_RAD) +
                                                  (modulation_index / 6) * cos(3 * (angle*angle_scaler - 300) * DEG_TO_RAD));
                     //t7 = MAX_DUTY_CYCLE - (t1 + t2 + t0);

                     IN_W_duty_cycle = t1 + t2 + t0;
                     //IN_U_duty_cycle = t0;
                     //IN_V_duty_cycle = t1 + t0;


                 }
                //Serial.println(IN_W_duty_cycle);
                temp_table[i] = IN_W_duty_cycle;

                angle++;
            }

            return temp_table;
        }

    //static constexpr std::array<uint16_t, LUTSize > LUT =createPureSineLUT();
    //static constexpr std::array<uint16_t , LUTSize> SVPWMLUT = generate();

};
#endif //INC_1MOTOR_REFACTOR_SVPWMLOOKUPTABLE_H
