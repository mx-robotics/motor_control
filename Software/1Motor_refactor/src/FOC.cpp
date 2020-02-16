//
// Created by firat on 21.01.20.
//

#include "FOC.h"
FOC &FOC::getInstance() {
    static FOC instance;
    return instance;


}