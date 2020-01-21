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
public:
    static constexpr std::array<uint16_t, LUTSize> generate() {
        {
            std::array<uint16_t , LUTSize> temp_table{};

            for(int i = 0;i<LUTSize;i++){
                temp_table[i]= 123;
            }
            return temp_table;
        }

    }
    //static constexpr std::array<uint16_t, LUTSize > LUT =createPureSineLUT();
    //static constexpr std::array<uint16_t , LUTSize> SVPWMLUT = generate();

};
#endif //INC_1MOTOR_REFACTOR_SVPWMLOOKUPTABLE_H
