//
// Created by firat on 25.01.20.
//

#ifndef INC_1MOTOR_REFACTOR_UTILS_H
#define INC_1MOTOR_REFACTOR_UTILS_H


#include <cstdint>
#include "SVPWM.h"

class utils {
    static uint16_t plot_counter;

public:
    static void plot_16t(SPWMDutyCycles &x){
    uint8_t arrayToSend[6];
    memcpy( arrayToSend + plot_counter, &x.inDutyCycleW, 2);
    memcpy( arrayToSend + plot_counter + 4, &x.inDutyCycleU, 2 );
    memcpy( arrayToSend + plot_counter + 8, &x.inDutyCycleV, 2 );
    plot_counter+=6;
    Serial.write(arrayToSend,6);
    //Serial.println(target_rps);
    //Serial.println(my_speed);
}

};

#endif //INC_1MOTOR_REFACTOR_UTILS_H
