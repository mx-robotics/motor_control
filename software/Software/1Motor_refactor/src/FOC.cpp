//
// Created by firat on 21.01.20.
//

#include "FOC.h"
FOC &FOC::getInstance() {
    static FOC instance;
    return instance;


}

void FOC::initInhibitPins(Motor &x) {
    pinMode(x.inhibitPins.InhibitPinW, OUTPUT);
    pinMode(x.inhibitPins.InhibitPinU, OUTPUT);
    pinMode(x.inhibitPins.InhibitPinV, OUTPUT);

}

void FOC::activateInhibitPins(Motor &x) {
    digitalWriteFast(x.inhibitPins.InhibitPinW, HIGH);
    digitalWriteFast(x.inhibitPins.InhibitPinU, HIGH);
    digitalWriteFast(x.inhibitPins.InhibitPinV, HIGH);

}


void FOC::updatePWMPinsDutyCycle(const SPWMDutyCycles &x, Motor &motor) {
    if (motor.initPins.InitPinW == 21 || motor.initPins.InitPinW == 22 || motor.initPins.InitPinW == 23) {

        FTM0_C6V = x.inDutyCycleW;
        FTM0_C0V = x.inDutyCycleU;
        FTM0_C1V = x.inDutyCycleV;

    } else {
        FTM0_C3V = x.inDutyCycleW; //Teency pin 10 -> FTM0_CH3
        FTM0_C7V = x.inDutyCycleU; //Teency pin 5 -> FTM0_CH7
        FTM0_C4V = x.inDutyCycleV; //Teency pin  6 -> FTM0_CH4


    }


}

void FOC::initHardware() {

    initPWMPins();
    RotaryEncoderCommunication::initSPI();
    initADCconversions();


    for (int i = 0; i < numberOfMotors ; ++i) {
        initInhibitPins(*motors[i]);
        activateInhibitPins(*motors[i]);
        RotaryEncoderCommunication::initMotorCSPins(*motors[i]);
    }



}

void FOC::initMotorParams(const uint16_t LUTindex) {
    int16_t sensorOffset = calculateSensorOffset(*motors[0],
                                                 LUTindex); // this calculation can be done only once and then hardcoded until there is a motor change.
    motors[0]->setSensorOffset(sensorOffset);

}

void FOC::initPWMPins() {
    FTM0_SC = 0; // required for other setup

    FTM0_CONF = 0xC0; //set up BDM in 11, FTM Counter functional -> 0000 1101 0000 0000
    FTM0_FMS = 0x00; //clear the WPEN (Write protection disabled) so that WPDIS is set in FTM0_MODE


    //FTM0_MODE|=0x05; // 0000 0101
    // This register contains the global enable bit for FTM-specific features and the control bits
    //used to configure:
    FTM0_MODE = 0b00000110; // 00000111

    //The Modulo register contains the modulo value for the FTM counter. After the FTM
    //counter reaches the modulo value, the overflow flag (TOF) becomes set at the next clock
    FTM0_MOD = (F_BUS / PWM_FREQ) / 2;
    // FTM0_C6SC |= FTM_CSC_CHIE
    FTM0_C6SC = 0b00101000;
    FTM0_C6V = 0; //50%
    PORTD_PCR6 |= PORT_PCR_MUX(4) | PORT_PCR_DSE | PORT_PCR_SRE; //Teency pin 21 -> FTM0_CH6


    FTM0_C0SC = 0b00101000;
    FTM0_C0V = 0; //50%
    PORTC_PCR1 |= PORT_PCR_MUX(4) | PORT_PCR_DSE | PORT_PCR_SRE; //Teency pin 22 (A8) -> FTM0_CH0


    FTM0_C1SC = 0b00101000;
    FTM0_C1V = 0; //50%
    PORTC_PCR2 |= PORT_PCR_MUX(4) | PORT_PCR_DSE | PORT_PCR_SRE; //Teency pin 23 (A9) -> FTM0_CH1

    if (numberOfMotors > 1) {
        FTM0_C7SC = 0b00101000;
        FTM0_C7V = 0; //50%
        PORTD_PCR7 |= PORT_PCR_MUX(4) | PORT_PCR_DSE | PORT_PCR_SRE; //Teensy pin 5 (A8) -> FTM0_CH7


        FTM0_C3SC = 0b00101000;
        FTM0_C3V = 0; //50%
        PORTC_PCR4 |= PORT_PCR_MUX(4) | PORT_PCR_DSE | PORT_PCR_SRE; //Teensy pin 10 -> FTM0_CH3


        FTM0_C4SC = 0b00101000;
        FTM0_C4V = 0; //50%
        PORTD_PCR4 |= PORT_PCR_MUX(4) | PORT_PCR_DSE | PORT_PCR_SRE; //Teensy pin 6 -> FTM0_CH4

    }


    FTM0_CNTIN = 0x00;


    FTM0_SC = 0b01101000; //CPWM MODE 0x48 EPWM -> 0x68 0110 1000

    FTM0_SC = 0b01101000; //CPWM MODE 0x48 EPWM -> 0x68 0110 1000 -> TOF interrupt disabled
    //FTM0_SC = 0b00101000; //CPWM MODE 0x48 EPWM -> 0x68 0110 1000


}

uint16_t FOC::getSpeedFromSomewhere(){
    return setSpeedFromADC();

} //@TODO get speed from ADC, serial port etc, interrupt driven maybe



void FOC::speedSweep(){
    static uint16_t ctr = 0;
    static float speed_ctr = 0;
    uint16_t rotaryEncoderValue = RotaryEncoderCommunication::SPITransfer(*motors[0]);
    motors[0]->updateRotaryEncoderPosition(rotaryEncoderValue);
    motors[0]->cumulativeAdd(rotaryEncoderValue);

    if(ctr == 1000) {
        uint8_t my_buff[8];
        float targetSpeed = speed_ctr; //getSpeedFromSomewhere();
        motors[0]->updateSpeedScalar(targetSpeed);
        float_t rps = VelocityCalculation::getRotationsPerSecond2(*motors[0]);
        motors[0]->setEncoderCumulativeValueToZero();
        memcpy( my_buff, &targetSpeed, 4);
        memcpy( my_buff +4, &rps, 4);
        Serial.write(my_buff,8);

        speed_ctr+=1;
        if (speed_ctr >= 100){
            speed_ctr = 0;
        }

        ctr = 0;

    }
    ++ctr;

    SPWMDutyCycles dutyCycles = SVPWM::calculateDutyCycles(*motors[0]);
    updatePWMPinsDutyCycle(dutyCycles, *motors[0]);



}

void FOC::run() {

    uint16_t rotaryEncoderValue = RotaryEncoderCommunication::SPITransfer(*motors[0]);
    Serial.println(rotaryEncoderValue);
}
void FOC::doTheMagic2() {

    static uint16_t ctr = 0;

    for (int i = 0; i < numberOfMotors ; ++i) {

        uint16_t rotaryEncoderValue = RotaryEncoderCommunication::SPITransfer(*motors[i]);

        motors[i]->updateRotaryEncoderPosition(rotaryEncoderValue);

        motors[i]->cumulativeAdd(rotaryEncoderValue);

        if (ctr == 1000) { //every 0.1 sec
               float_t rps = VelocityCalculation::getRotationsPerSecond2(*motors[i]);
               Serial.println(rps);
               motors[i]->setEncoderCumulativeValueToZero(); // sets to zero
               motors[i]->updateSpeedRPS(rps);
               float speed_command = getSpeedFromSomewhere();
               Serial.println(speed_command);

               //float speed_command = SpeedPIDController::getSpeedCommand(*motors[i], 2);
               motors[i]->updateSpeedScalar(speed_command);
               //if(i == numberOfMotors-1) {
               //}
            ctr = 0;


        }
        SPWMDutyCycles dutyCycles = SVPWM::calculateDutyCycles(*motors[i]);
        updatePWMPinsDutyCycle(dutyCycles, *motors[i]);
    }



    ++ctr;


}

void FOC::primitiveSpin(uint16_t LUTindex) {
    uint16_t LUTSize = SVPWM::getLutSize();
    uint16_t dutyCycleW = SVPWM::getLUT()[LUTindex];
    uint16_t dutyCycleU = SVPWM::getLUT()[(LUTindex + (LUTSize / 3)) % LUTSize];
    uint16_t dutyCycleV = SVPWM::getLUT()[(LUTindex + (LUTSize / 3) * 2) % LUTSize];
    SPWMDutyCycles x{dutyCycleW, dutyCycleU, dutyCycleV };
    updatePWMPinsDutyCycle(x, *motors[0]);


}


void FOC::registerMotors(Motor * m_ptr){
    motors[numberOfMotors++] = m_ptr;



}


void FOC::initADCconversions() {

    pinMode(ADC_PIN, INPUT);
    adc->adc0->setAveraging(4); // set number of averages
    adc->adc0->setResolution(12); // set bits of resolution
    adc->adc0->setConversionSpeed(ADC_CONVERSION_SPEED::VERY_HIGH_SPEED); // change the conversion speed
    adc->adc0->setSamplingSpeed(ADC_SAMPLING_SPEED::VERY_HIGH_SPEED); // change the sampling speed
    adc->startContinuous(ADC_PIN);

}

uint16_t FOC::setSpeedFromADC() {

    if (adc->adc0->isComplete()) {
        uint_fast16_t value1 = adc->analogReadContinuous(ADC_PIN);
        value1 =   (value1  / static_cast<float_t > (adc->adc0->getMaxValue())) * 100;
        return value1;
    }
    return 0;

}

int16_t FOC::calculateSensorOffset(Motor &motor,
                              const uint16_t LUTindex) { //the index is used as a parameter to maybe plot the sensor offset all over the motor range
    uint16_t LUTSize = SVPWM::getLutSize();
    uint16_t dutyCycleW = SVPWM::getLUT()[LUTindex];
    uint16_t dutyCycleU = SVPWM::getLUT()[(LUTindex + (LUTSize / 3)) % LUTSize];
    uint16_t dutyCycleV = SVPWM::getLUT()[(LUTindex + (LUTSize / 3) * 2) % LUTSize];
    SPWMDutyCycles x{dutyCycleW, dutyCycleU, dutyCycleV};
    updatePWMPinsDutyCycle(x, motor);

    delay(500);

    Serial.print("\nCurrent stator flux index: ");
    Serial.println(LUTindex);

    uint16_t encoderVal = RotaryEncoderCommunication::SPITransfer(motor);
    uint16_t encoderValScaled = encoderVal % LUTSize;
    Serial.print(" Modulo scaled encoder reading: ");
    Serial.println(encoderValScaled);

    uint16_t expectedRotorFlux = (LUTindex + LUTSize / 2) % LUTSize; // + 180
    Serial.print("Expected Rotor Flux position: ");
    Serial.println(expectedRotorFlux);

    int16_t sensorOffset = expectedRotorFlux - encoderValScaled;
    Serial.print("Difference between calculated rotor flux and expected rotor flux ");
    Serial.println((1489 - encoderValScaled) -expectedRotorFlux);
    return sensorOffset;
    /**
     *We want to find the SensorOffset because:
     * 1- We want to measure/acquire the flux vector position of permanent magnets (a.k.a. Rotor Flux) because:
     *      - We want to create a flux vector (a.k.a Stator flux) perpendicular to it so we can generate the max. torque.
     *      - In order to do so, we need to know the Rotor flux position which is constant in magnitude and rotates with the motor.
     *      - For example, if we know Rotor flux is at 100°, we can create our Stator flux at 10° or at 190° degrees depending on the desired direction of rotation
     *      - When we get a rotary encoder reading, this tells us the physical position of the rotor and has no info about the position of the rotor flux vector
     *      - Which means there is an offset between the physical position of the motor and the Rotor Flux. We want to calculate this offset.
     * 2- We calculate it in the following way:
     *      - We energize the motor (create a stator flux at an arbitrary position) - for example 100°
     *      - Motor locks itself to a position depending on the stator flux vector.
     *      - Since the motor has locked itself  we know the rotor flux has aligned itself at the opposite direction, i.e. stator flux position + 180°
     *      - Stator flux position was taken 100° so the rotor flux position is at 280°
     *      - We take a rotary encoder value which tells us something between 0 and 360. This is just a physical reading with no reference to anything.
     *      - But we know the expected rotor flux position which is 280°
     *      - We calculate the difference between measured physical position and expected rotor flux position.
     *      - We have our offset which should be added to /substracted from our actual rotary encoder reading to get the rotor flux vector position at any given time.
     *
     * CONCLUSION: At the end, substracting what we have mesaured from the the max sensor reading gave us the rotor flux position ( 360 - measured_rot_pos). The method described above doesnt give us a constant offset
     * because the stator flux position increases as the rotor flux position decreases.
     * stator flux = 10 -> rotor flux = 350 (360-10)
     * stator flux = 11 -> rotor flux = 349 (360-11)
     * stator flux = 12 -> rotor flux = 348 (360-12)
     * stator flux = 13 -> rotor flux = 347 (360-13)
     * stator flux = 14 -> rotor flux = 346 (360-14)
     *
     *
    **/


}

void FOC::testMotors(Motor( &x)) {

    for (int j = 0; j < 1489 ; j+=20 ) { calculateSensorOffset(x,j); }


}
