//
// Created by firat on 19.01.20.
//
#ifndef DMA_SPI_FOC_H
#define DMA_SPI_FOC_H


#include <SPI.h>
#include <array>
#include <ADC.h>


namespace tuw {

    class FOC {
    public:
        static FOC &getInstance();  // Singleton handler

        void SPITransfer();

        void calculate_offset_parameters();
        void initPins();  // called once in setup
        void doTheMagic(); // called every loop iteration

        void P_controller(float rps_target, float rps_measured);
        void PID_controller(float rps_target, float rps_measured);



        // ########### PID RELATED FUNCTIONS ///////////

        void setSpeedRPM(uint8_t _target_speed);

        // ############# ADC

        void setSpeedFromADC();


        // ######### DEBUG FUNCTIONS ///////

        void printLUToffsetValues();
        void printMaxDutyCylcle();
        void setSpeedSerial();
        void debugPlot();
        std::array<uint16_t , 360> getArray();
        uint16_t getRotorPosition();
        float getMySpeed() const;
        volatile float getTargetSpeed() const;

        void led_foo ();
        void calculateSensorOffsetAngle();
        void getModulationIndexSpeedValues();

        void calculateRPS();








    private:
        FOC();

        // ++++++++++++++initial activation functios
        void initSPI();
        void initInhibitPIN();
        void setPWMpins();
        void initServoPins();
        void activateInhibitPins();

        // ***************** LUT & DUTY CYCLE CALCULATIONS
        void calculateInputPinsDutyCycle(); // on the fly calculation, used once for LUT creation
        inline uint16_t calculate_LUT_offset(); // calculate the modulation index-speed command relation
        std::array<uint16_t , 360> createLUT(); // create the LUT
        void calculate_offset_parameters_short(); // calculates the offset func parameters in a magic way, called 1
        void calculateInputPinsDutyCycleLUT();
        void setInputPinsDutyCycle();

        // ****************** ADC functions
        void initADCconversions();




        // ****************** MATH

        inline int fast_mod(const int input, const int ceil) {
            return input >= ceil ? input % ceil : input; }

        inline float moving_ave(float angle){
            static int i=0;
            static float sum = 0;
            sum -= move_average_list[i];
            move_average_list[i] = angle;
            sum += move_average_list[i];
            i++;
            if (i == 100) {
                i = 0;
            }

            return sum*0.01f;

        }

        // ************* P I N S ******************
        static constexpr uint8_t CS = 10; // has to be static cuz a constexpr must be known at compile time
        static constexpr uint8_t CLK = 14; // if it isn't static, you need to wait runtime until the object is created
        volatile uint16_t rotor_position=0;
        volatile float_t target_rpm=50.f;

        float_t offset_param_m;
        float_t offset_param_c;


        static constexpr uint8_t IS_W= A17;
        static constexpr uint8_t IS_v= A16;
        static constexpr uint8_t IS_U= A15;

        static constexpr uint8_t INH_W= 31;
        static constexpr uint8_t INH_V= 24;
        static constexpr uint8_t INH_U= 33;

        static constexpr uint8_t S_PWM1= 25;
        static constexpr uint8_t S_PWM2= 32;

        static constexpr uint8_t IN_W= 21;
        static constexpr uint8_t IN_V= 22;
        static constexpr uint8_t IN_U= 23;

        // *************** P W M   C O N S T A N T S ****************

        static constexpr uint16_t PWM_FREQ = 20000;
        static constexpr uint16_t MAX_DUTY_CYCLE = (F_BUS/PWM_FREQ)/2;

        volatile uint32_t IN_W_duty_cycle;
        volatile uint32_t IN_U_duty_cycle;
        volatile uint32_t IN_V_duty_cycle;


        static constexpr uint16_t sensor_offset= 180;//85; // 275; //75
        static constexpr uint8_t angle_offset = 95;
        volatile float modulation_index = 1.1547005f;
        volatile int8_t direction = 1; // -1 is CCW


    private:

        const std::array<uint16_t , 360> array = createLUT();
        uint8_t f_to_c_array[160];
        float acc_error = 0;
        std::array<float , 100> move_average_list;


        float target_speed= 60;
        float target_rps= 7.5;
    public:
        volatile float getTargetRps() const;

    private:

        //between o and 100
        volatile uint8_t sensor_overflow = 0;

        volatile uint16_t angle_now=0;

        float my_speed=0.f;

        const SPISettings spiSettings=SPISettings(10000000,MSBFIRST, SPI_MODE1);

        static constexpr uint8_t ADC_PIN = A1;
        ADC *adc = new ADC();
    };

};
#endif //DMA_SPI_FOC_H
