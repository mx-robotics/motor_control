#include <iostream>
#include "matplotlibcpp.h"
#include "print_arrays.hpp"

namespace plt = matplotlibcpp;


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

constexpr float F_BUS = 60000000;

#define DEG_TO_RAD 0.017453292519943295769236907684886
class LUTGenerator{



public:
    static constexpr uint16_t maxSensorResolution = 16384;
    static constexpr uint8_t motorPoleCount = 11;
    static constexpr uint16_t LUTSize = maxSensorResolution / motorPoleCount;
    static constexpr uint16_t PWM_FREQ = 20000;
    static constexpr int MAX_DUTY_CYCLE = (F_BUS / PWM_FREQ) / 2;


     static constexpr  std::array<int, LUTSize> generate() {

        std::array <int, LUTSize> temp_table{};

        constexpr float modulation_index =1.15470053838;
        uint angle = 0;
        for (int i = 0; i < LUTSize; i++) {

            temp_table[i] = calculateDutyCycleW(angle,modulation_index,MAX_DUTY_CYCLE);

            angle++;
        }

        return temp_table;
    }


     static constexpr int calculateDutyCycleW(uint16_t rotor_position,float modulation_index,uint16_t MAX_DUTY_CYCLE){
         constexpr float angleScaler = 360.0f / LUTSize;
         constexpr float deg_to_radians = 0.017453f;
         uint16_t IN_W_duty_cycle=0;

         float r1 = 0;
         float r2 = 0;
         float t7 = 0;
         float t0 = 0;
         float t1 = 0;
         float t2 = 0;
         float angle = static_cast<float>(rotor_position) * angleScaler;

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
             t0 = (MAX_DUTY_CYCLE / 2) * (1 - modulation_index * cos((angle - 300) * deg_to_radians) +
                                          (modulation_index / 6) * cos(3 * (angle - 300) * deg_to_radians));
             t7 = MAX_DUTY_CYCLE - (t1 + t2 + t0);

             IN_W_duty_cycle = t1 + t2 + t0;
             //IN_U_duty_cycle = t0;
             //IN_V_duty_cycle = t1 + t0;


         }
         return IN_W_duty_cycle;




    }



    static constexpr  ModulationIndexScalingParams calculateModulationIndexScalingOffsetParameters() {

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
        //ModulationIndexScalingParams params{-6.0f, 600.0f};

        /*Serial.print(", c = ");
        Serial.print(static_cast<int16_t >(params.offsetParam_c));
        Serial.print(", m = ");
        Serial.println(static_cast<int16_t >(floorf(params.offsetParam_m )));
*/


        return params;

    }




};

void calculateScaledLUTvsOnlineCalculatedDifference(int rotor_position, float modulation_index,const  std::array<int, 1489> &LUT){
    constexpr float modulation_indexFULL =1.15470053838;
    float scalar = modulation_index/modulation_indexFULL;
    constexpr auto params = LUTGenerator::calculateModulationIndexScalingOffsetParameters();
    int dutyCyleWCalculated = LUTGenerator::calculateDutyCycleW(rotor_position,modulation_index,LUTGenerator::MAX_DUTY_CYCLE);
    int dutyCyleScaled = LUT[rotor_position] * scalar + params.offsetParam_c + params.offsetParam_m * (scalar  *100);
    int diff = dutyCyleScaled - dutyCyleWCalculated;
    assert(abs(diff) < 3);


}

void testSVPWMScaling(const  std::array<int, 1489> &LUT){
    constexpr float modulation_indexFULL =1.15470053838;
    for (float modulation_index = 0; modulation_index <= modulation_indexFULL; modulation_index+=0.001) {
        for(int rotor_pos = 0; rotor_pos<1489; rotor_pos++){
            calculateScaledLUTvsOnlineCalculatedDifference(rotor_pos,modulation_index,LUT);

        }

    }
    std::cout << "SVPWM scaling tested, max. difference is under 3" << std::endl;
}

auto constexpr generateOtherPhasesFromLUT(const  std::array<int,1489> LUT, int phaseShift){
    std::array<int,1489> temp{};
    int LUTShift = static_cast<int>((phaseShift * 1489) /360.0f );
    for (int i = 0; i < 1489; ++i) {
        temp[i]=LUT[(i+LUTShift)%1489];
    }
    return temp;


}


int main() {
   
    constexpr auto LUT = LUTGenerator::generate();
    constexpr auto LUT1 = generateOtherPhasesFromLUT(LUT,0);
    constexpr auto LUT2 = generateOtherPhasesFromLUT(LUT1,120);
    constexpr auto LUT3 = generateOtherPhasesFromLUT(LUT1,240);
    constexpr auto params = LUTGenerator::calculateModulationIndexScalingOffsetParameters();
    std::cout << "params.m: " << params.offsetParam_m << " params.c: " << params.offsetParam_c << std::endl;
    int a = LUT1[50];
    int b = LUT2[50];
    int c = LUT3[50];

    std::cout << "a : " << a << std::endl;
    std::cout << "b : " << b << std::endl;
    std::cout << "c : " << c << std::endl;
    std::cout << "Sum of a+b+c : " << a+b+c << std::endl;

    testSVPWMScaling(LUT);
    std::vector<int> xAxis{};
    for (int i = 0; i < 1489; ++i) {
        xAxis.push_back(i);
    }
    std::vector<int > k(LUT1.begin(),LUT1.end());
    std::vector<int > k120(LUT2.begin(),LUT2.end());
    std::vector<int > k240(LUT3.begin(),LUT3.end());
    std::vector<int > printVector(printArray1.begin(),printArray1.end());
    std::vector<int > printVector2(printArray2.begin(),printArray2.end());
    std::vector<int > printVector3(printArray3.begin(),printArray3.end());
    plt::plot(xAxis,printVector);
    plt::plot(xAxis,printVector2);
    plt::plot(xAxis,printVector3);
    //plt::plot(xAxis,k120);
    //plt::plot(xAxis,k240);
    plt::show();

    return 0;
}
