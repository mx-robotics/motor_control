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
    static constexpr uint16_t MAX_DUTY_CYCLE = 20000;
    static constexpr uint16_t LUTSize = 1489;

    constexpr static  std::array<uint16_t, LUTSize> generate() {

        std::array <uint16_t, LUTSize> temp_table{};


        //uint32_t IN_U_duty_cycle;
        //uint32_t IN_V_duty_cycle;
        float modulation_index = 1.1547005f;



        uint angle = 0;
        for (int i = 0; i < LUTSize; i++) {

            temp_table[i] = calculateDutyCycleW(angle,modulation_index,MAX_DUTY_CYCLE);

            angle++;
        }

        return temp_table;
    }

    constexpr static float calculateDutyCycleW(uint16_t angle, float modulation_index, uint16_t MAX_DUTY_CYCLE){
        float r1 = 0;
        float r2 = 0;
        float t7 = 0;
        float t0 = 0;
        float t1 = 0;
        float t2 = 0;
        float IN_W_duty_cycle = 0;
        float angle_scaler = 360.0f/LUTSize;
        uint16_t oneSixthLUTSize = LUTSize/6;
        float deg_to_radians = 0.01745;
        float scaled_angle = static_cast<float >(angle) * angle_scaler;
        auto maxDutyCycle = static_cast<float >(MAX_DUTY_CYCLE);
        if (angle >= 0 && angle < oneSixthLUTSize) {

            r1 =  sinf((60.0f - scaled_angle) * deg_to_radians);
            r2 =  sinf(scaled_angle * deg_to_radians);


            t1 = r1 * maxDutyCycle;
            t2 = r2 * maxDutyCycle;
            t0 = (maxDutyCycle / 2) *
                 (1 - modulation_index * cosf(scaled_angle* deg_to_radians) + (modulation_index / 6) * cosf(3 * scaled_angle* deg_to_radians));
            t7 = maxDutyCycle - (t1 + t2 + t0);

            IN_W_duty_cycle = (t2 + t7 + t1);
            //IN_U_duty_cycle = t2 + t7;
            //IN_V_duty_cycle = t7;

        } else if (angle >= oneSixthLUTSize && angle < oneSixthLUTSize*2 ) {
            r1 = sinf((60 - (scaled_angle- 60)) * deg_to_radians);
            //r2 = sinf((angle - 60) * deg_to_radians);

            t1 = r1 * (maxDutyCycle);
            //t2 = r2 * (maxDutyCycle);
            t0 = (maxDutyCycle / 2) * (1 - modulation_index * cos((scaled_angle- 60) * deg_to_radians) +
                                         (modulation_index / 6) * cos(3 * (scaled_angle- 60) * deg_to_radians));
            //t7 = maxDutyCycle - (t1 + t2 + t0);

            IN_W_duty_cycle = t1 + t0;
            //IN_U_duty_cycle = t1 + t2 + t0;
            //IN_V_duty_cycle = t0;

        } else if (angle >= oneSixthLUTSize*2 && angle < oneSixthLUTSize*3 ) {

            r1 =  sinf((60 - (scaled_angle- 120)) * deg_to_radians);
            r2 =  sinf((scaled_angle- 120) * deg_to_radians);

            t1 = r1 * (maxDutyCycle);
            t2 = r2 * (maxDutyCycle);
            t0 = (maxDutyCycle / 2) * (1 - modulation_index * cos((scaled_angle- 120.0f) * deg_to_radians) +
                                         (modulation_index / 6) * cos(3 * (scaled_angle- 120.0f) * deg_to_radians));
            t7 = maxDutyCycle - (t1 + t2 + t0);

            IN_W_duty_cycle = t7;
            //IN_U_duty_cycle = t1 + t2 + t7;
            //IN_V_duty_cycle = t2 + t7;


        } else if (angle >= oneSixthLUTSize*3 && angle < oneSixthLUTSize*4) {
            //r1 = sinf((60 - (angle - 180)) * deg_to_radians);
            //r2 = sinf((angle - 180) * deg_to_radians);

            //t1 = r1 * (maxDutyCycle);
            //t2 = r2 * (maxDutyCycle);
            t0 = (maxDutyCycle / 2) * (1 - modulation_index * cos((scaled_angle- 180.0f) * deg_to_radians) +
                                         (modulation_index / 6) * cos(3 * (scaled_angle- 180.0f) * deg_to_radians));
            //t7 = maxDutyCycle - (t1 + t2 + t0);

            IN_W_duty_cycle = t0;
            //IN_U_duty_cycle = t1 + t0;
            //IN_V_duty_cycle = t1 + t2 + t0;

        } else if (angle >= oneSixthLUTSize*4 && angle < oneSixthLUTSize*5) {
            r1 =  sinf((60.0f - (scaled_angle- 240.0f)) * deg_to_radians);
            r2 =  sinf((scaled_angle- 240.0f) * deg_to_radians);

            t1 = r1 * (maxDutyCycle);
            t2 = r2 * (maxDutyCycle);
            t0 = (maxDutyCycle / 2) * (1 - modulation_index * cos((scaled_angle- 240) * deg_to_radians) +
                                         (modulation_index / 6) * cos(3 * (scaled_angle- 240) * deg_to_radians));
            t7 = maxDutyCycle - (t1 + t2 + t0);

            IN_W_duty_cycle = t2 + t7;
            //IN_U_duty_cycle = t7;
            //IN_V_duty_cycle = t1 + t2 + t7;


        } else if (angle >= oneSixthLUTSize*5 && angle < LUTSize) {

            r1 =   sinf((60.0f - (scaled_angle- 300)) * deg_to_radians);
            r2 =  sinf((scaled_angle- 300) * deg_to_radians);

            t1 = r1 * (maxDutyCycle);
            t2 = r2 * (maxDutyCycle);
            t0 = (maxDutyCycle / 2) * (1 - modulation_index * cos((scaled_angle- 300) * deg_to_radians) +
                                         (modulation_index / 6) * cos(3 * (scaled_angle- 300) * deg_to_radians));
            //t7 = maxDutyCycle - (t1 + t2 + t0);

            IN_W_duty_cycle = t1 + t2 + t0;
            //IN_U_duty_cycle = t0;
            //IN_V_duty_cycle = t1 + t0;


        }
        return IN_W_duty_cycle;
    }


    constexpr static  ModulationIndexScalingParams calculateModulationIndexScalingOffsetParameters() {


        uint16_t rotor_position = 1000;
        float modulation_index_max = 1.1547005f;
        float IN_W_duty_cycle_max = calculateDutyCycleW(rotor_position,modulation_index_max,MAX_DUTY_CYCLE);

        float modulationIndexNew = 0.2f * modulation_index_max;
        float dutyCycleScaledWithModuloIndex1 = IN_W_duty_cycle_max * modulationIndexNew;
        float dutyCycleCalculatedWithModuloIndex1= calculateDutyCycleW(rotor_position,modulationIndexNew, MAX_DUTY_CYCLE);

        modulationIndexNew = 0.4f * modulation_index_max;
        float dutyCycleScaledWithModuloIndex2 = IN_W_duty_cycle_max * modulationIndexNew;
        float dutyCycleCalculatedWithModuloIndex2= calculateDutyCycleW(rotor_position,modulationIndexNew, MAX_DUTY_CYCLE);


        float yy1 = dutyCycleCalculatedWithModuloIndex1 - dutyCycleScaledWithModuloIndex1;
        float yy2 = dutyCycleCalculatedWithModuloIndex2 - dutyCycleScaledWithModuloIndex2;

        float xx1 = 0.2f * modulation_index_max;
        float xx2 = 0.4f * modulation_index_max;

        float m = (yy2 - yy1) / (xx2 - xx1);

        float c = yy2 - (m * xx2);


        ModulationIndexScalingParams params{m, c};

        return params;

    }




};

class SVPWM {
private:
    static constexpr uint16_t maxSensorResolution = 16384;
    static constexpr uint8_t motorPoleCount = 11;
    static constexpr uint16_t LUTSize = maxSensorResolution / motorPoleCount;
    static constexpr uint16_t angleOffset = LUTSize/4; // @TODO calculate -90 + 90
    static constexpr auto LUT=LUTGenerator::generate();
    static constexpr auto modulationIndexParams = LUTGenerator::calculateModulationIndexScalingOffsetParameters();


public:

    static SPWMDutyCycles calculateDutyCycles(Motor &x){
        SPWMDutyCycles temp;
        float modulationIndexOffset =  scaleDutyCyclesToModulationIndex(x.modulationIndex);
        uint16_t base = (x.rotaryEncoderPosition + x.sensorOffset ) % LUTSize;
        uint16_t LUTIndexW = (base + angleOffset * x.direction);
        uint16_t LUTIndexU = ((base + angleOffset * x.direction) + LUTSize / 3) % LUTSize;
        uint16_t LUTIndexV = ((base + angleOffset * x.direction) + (2 * (LUTSize / 3)) ) % LUTSize;
        temp.inDutyCycleW = LUT[LUTIndexW]* x.modulationIndex + modulationIndexOffset;
        temp.inDutyCycleU = LUT[LUTIndexU]* x.modulationIndex + modulationIndexOffset;
        temp.inDutyCycleV = LUT[LUTIndexV]* x.modulationIndex + modulationIndexOffset;
        return temp;

    };
    inline static uint16_t scaleDutyCyclesToModulationIndex(float modulationIndex){
        return (modulationIndexParams.offsetParam_m*modulationIndex + modulationIndexParams.offsetParam_c);
    }


    static const auto &getLUT(){
        return LUT;

    }
    static const uint16_t getLutSize() {
        return LUTSize;
    }


};
#endif //INC_1MOTOR_REFACTOR_SVPWM_H
