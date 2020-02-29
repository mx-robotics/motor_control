//
// Created by firat on 21.01.20.
//

#ifndef INC_1MOTOR_REFACTOR_SVPWM_H
#define INC_1MOTOR_REFACTOR_SVPWM_H

#include <Arduino.h>
#include <array>
#include "Motor.h"


struct SPWMDutyCycles{
    uint16_t inDutyCycleW;
    uint16_t inDutyCycleU;
    uint16_t inDutyCycleV;

};

struct ModulationIndexScalingParams{
    constexpr ModulationIndexScalingParams(float m, float c): offsetParam_m(m), offsetParam_c(c){};
    float offsetParam_m;
    float offsetParam_c;

};

class LUTGenerator{

public:
    static constexpr uint16_t maxSensorResolution = 16384;
    static constexpr uint8_t motorPoleCount = 11;
    static constexpr uint16_t LUTSize = maxSensorResolution / motorPoleCount;
    static constexpr uint16_t PWM_FREQ = 20000;
    static constexpr uint16_t MAX_DUTY_CYCLE = (F_BUS / PWM_FREQ) / 2;


    constexpr static  std::array<uint16_t, LUTSize> generate() {

        std::array <uint16_t, LUTSize> temp_table{};

        float modulation_index = 1.1547005f;

        uint angle = 0;
        for (int i = 0; i < LUTSize; i++) {

            temp_table[i] = static_cast<uint16_t>(calculateDutyCycleW(angle,modulation_index,MAX_DUTY_CYCLE));

            angle++;
        }

        return temp_table;
    }


    constexpr static uint16_t calculateDutyCycleW(uint16_t rotor_position,float modulation_index,uint16_t MAX_DUTY_CYCLE){
        uint16_t IN_W_duty_cycle=0;
        float r1 = 0;
        float r2 = 0;
        float t7 = 0;
        uint32_t t0 = 0;
        uint32_t t1 = 0;
        uint32_t t2 = 0;
        float angle_scaler = 360.0f/LUTSize;
        float angle = rotor_position * angle_scaler;
        float deg_to_radians = 0.017453f;

        if (angle >= 0 && angle < 60) {

            r1 = (sqrt(3) / 2) * modulation_index * sin((60 - angle) * deg_to_radians);
            r2 = (sqrt(3) / 2) * modulation_index * sin((angle) * deg_to_radians);

            t1 = r1 * (MAX_DUTY_CYCLE);
            t2 = r2 * (MAX_DUTY_CYCLE);
            t0 = (MAX_DUTY_CYCLE / 2) *
                 (1 - modulation_index * cos(angle * deg_to_radians) + (modulation_index / 6) * cos(3 * angle * deg_to_radians));
            t7 = MAX_DUTY_CYCLE - (t1 + t2 + t0);

            IN_W_duty_cycle = (t2 + t7 + t1);
            //IN_U_duty_cycle = t2 + t7;
            //IN_V_duty_cycle = t7;

        } else if (angle >= 60 && angle < 120) {
            r1 = (sqrt(3) / 2) * modulation_index * sin((60 - (angle - 60)) * deg_to_radians);
            r2 = (sqrt(3) / 2) * modulation_index * sin((angle - 60) * deg_to_radians);

            t1 = r1 * (MAX_DUTY_CYCLE);
            t2 = r2 * (MAX_DUTY_CYCLE);
            t0 = (MAX_DUTY_CYCLE / 2) * (1 - modulation_index * cos((angle - 60) * deg_to_radians) +
                                         (modulation_index / 6) * cos(3 * (angle - 60) * deg_to_radians));
            t7 = MAX_DUTY_CYCLE - (t1 + t2 + t0);

            IN_W_duty_cycle = t1 + t0;
            //IN_U_duty_cycle = t1 + t2 + t0;
            //IN_V_duty_cycle = t0;

        } else if (angle >= 120 && angle < 180) {

            r1 = (sqrt(3) / 2) * modulation_index * sin((60 - (angle - 120)) * deg_to_radians);
            r2 = (sqrt(3) / 2) * modulation_index * sin((angle - 120) * deg_to_radians);

            t1 = r1 * (MAX_DUTY_CYCLE);
            t2 = r2 * (MAX_DUTY_CYCLE);
            t0 = (MAX_DUTY_CYCLE / 2) * (1 - modulation_index * cos((angle - 120) * deg_to_radians) +
                                         (modulation_index / 6) * cos(3 * (angle - 120) * deg_to_radians));
            t7 = MAX_DUTY_CYCLE - (t1 + t2 + t0);

            IN_W_duty_cycle = t7;
            //IN_U_duty_cycle = t1 + t2 + t7;
            //IN_V_duty_cycle = t2 + t7;


        } else if (angle >= 180 && angle < 240) {
            r1 = (sqrt(3) / 2) * modulation_index * sin((60 - (angle - 180)) * deg_to_radians);
            r2 = (sqrt(3) / 2) * modulation_index * sin((angle - 180) * deg_to_radians);

            t1 = r1 * (MAX_DUTY_CYCLE);
            t2 = r2 * (MAX_DUTY_CYCLE);
            t0 = (MAX_DUTY_CYCLE / 2) * (1 - modulation_index * cos((angle - 180) * deg_to_radians) +
                                         (modulation_index / 6) * cos(3 * (angle - 180) * deg_to_radians));
            t7 = MAX_DUTY_CYCLE - (t1 + t2 + t0);

            IN_W_duty_cycle = t0;
            //IN_U_duty_cycle = t1 + t0;
            //IN_V_duty_cycle = t1 + t2 + t0;

        } else if (angle >= 240 && angle < 300) {
            r1 = (sqrt(3) / 2) * modulation_index * sin((60 - (angle - 240)) * deg_to_radians);
            r2 = (sqrt(3) / 2) * modulation_index * sin((angle - 240) * deg_to_radians);

            t1 = r1 * (MAX_DUTY_CYCLE);
            t2 = r2 * (MAX_DUTY_CYCLE);
            t0 = (MAX_DUTY_CYCLE / 2) * (1 - modulation_index * cos((angle - 240) * deg_to_radians) +
                                         (modulation_index / 6) * cos(3 * (angle - 240) * deg_to_radians));
            t7 = MAX_DUTY_CYCLE - (t1 + t2 + t0);

            IN_W_duty_cycle = t2 + t7;
            //IN_U_duty_cycle = t7;
            //IN_V_duty_cycle = t1 + t2 + t7;


        } else if (angle >= 300 && angle < 360) {

            r1 = (sqrt(3) / 2) * modulation_index * sin((60 - (angle - 300)) * deg_to_radians);
            r2 = (sqrt(3) / 2) * modulation_index * sin((angle - 300) * deg_to_radians);

            t1 = r1 * (MAX_DUTY_CYCLE);
            t2 = r2 * (MAX_DUTY_CYCLE);
            t0 = (MAX_DUTY_CYCLE / 2) * (1 - modulation_index * cos((angle - 300) * DEG_TO_RAD) +
                                         (modulation_index / 6) * cos(3 * (angle - 300) * DEG_TO_RAD));
            t7 = MAX_DUTY_CYCLE - (t1 + t2 + t0);

            IN_W_duty_cycle = t1 + t2 + t0;
            //IN_U_duty_cycle = t0;
            //IN_V_duty_cycle = t1 + t0;


        }
        return IN_W_duty_cycle;


    }

    static std::array<uint16_t, 1489> createLUT();

    static  constexpr ModulationIndexScalingParams calculateModulationIndexScalingOffsetParameters() {

            uint16_t rotor_position = 200;
            float modulation_index_max = 1.1547005f;
            float IN_W_duty_cycle_max = calculateDutyCycleW(rotor_position, modulation_index_max, MAX_DUTY_CYCLE);

            float scalar1 = 0.4f;
            float modulationIndexNew = scalar1 * modulation_index_max;
            float dutyCycleScaledWithModuloIndex1 = IN_W_duty_cycle_max * scalar1;
            float dutyCycleCalculatedWithModuloIndex1 = calculateDutyCycleW(rotor_position, modulationIndexNew,
                                                                            MAX_DUTY_CYCLE);

            float scalar2 = 0.9f;
            modulationIndexNew = scalar2* modulation_index_max;
            float dutyCycleScaledWithModuloIndex2 = IN_W_duty_cycle_max * scalar2;
            float dutyCycleCalculatedWithModuloIndex2 = calculateDutyCycleW(rotor_position, modulationIndexNew,
                                                                            MAX_DUTY_CYCLE);


            float yy1 = dutyCycleCalculatedWithModuloIndex1 - dutyCycleScaledWithModuloIndex1;
            float yy2 = dutyCycleCalculatedWithModuloIndex2 - dutyCycleScaledWithModuloIndex2;

            float xx1 =  scalar1*100;
            float xx2 =  scalar2*100;

            float m = (yy2 - yy1) / (xx2 - xx1);

            float c = yy2 - (m * xx2);


            ModulationIndexScalingParams params{m, c};

            /*Serial.print(", c = ");
            Serial.print(static_cast<int16_t >(params.offsetParam_c));
            Serial.print(", m = ");
            Serial.println(static_cast<int16_t >(floorf(params.offsetParam_m )));
*/


        return params;

    }




};

class SVPWM {
private:
    static constexpr uint16_t LUTSize = LUTGenerator::LUTSize;
    static constexpr uint16_t angleOffset = LUTSize/4; // @TODO calculate -90 + 90, plot for variations
    static constexpr auto LUT = LUTGenerator::generate();
    static constexpr ModulationIndexScalingParams modulationIndexParams = LUTGenerator::calculateModulationIndexScalingOffsetParameters();


public:

    static SPWMDutyCycles calculateDutyCycles(Motor &x){ //@TODO should return a const ref as every non trivial return value should
        SPWMDutyCycles temp;
       float modulationIndexOffset =  scaleDutyCyclesToModulationIndex(x.speedScalar);

        uint16_t base = (x.scaledRotaryEncoderPosition + ((x.fieldWeakening + angleOffset) * x.direction - 20) + LUTSize) % LUTSize;
        /*
         * This part is tricky; there is a field-weakening and the best results has been found at -120 and + 80
         * To avoid an else-if check the fieldWeakening is set to 100 and with this -20 it is set to -120 or 80 according to the direction of the motor
         * It is very peculiar that the implicit Sensor Offet in the scaledRotaryEncoderPosition is different in each direction, one direction has a 40 degrees offset compared to the other
         * This issue clearly shows itself with the max speed difference if left unattended. With the current fieldWeakening parameter themax speed is around 13.50 in each direction
         *
         *
         * */
        uint16_t LUTIndexW = base;
        uint16_t LUTIndexU = (base + (LUTSize / 3) ) % LUTSize;
        uint16_t LUTIndexV = (base + (2 * (LUTSize / 3)) ) % LUTSize;
        temp.inDutyCycleW = LUT[LUTIndexW]* x.speedScalar * 0.01f + modulationIndexOffset;
        temp.inDutyCycleU = LUT[LUTIndexU]* x.speedScalar *0.01f + modulationIndexOffset;
        temp.inDutyCycleV = LUT[LUTIndexV]* x.speedScalar *0.01f + modulationIndexOffset;
        return temp;

    };
    inline static uint16_t scaleDutyCyclesToModulationIndex(float scalar){
        return (modulationIndexParams.offsetParam_m*scalar + modulationIndexParams.offsetParam_c);
    }


    static const auto &getLUT(){
        return LUT;

    }
    static const uint16_t getLutSize() {
        return LUTSize;
    }


};
#endif //INC_1MOTOR_REFACTOR_SVPWM_H
