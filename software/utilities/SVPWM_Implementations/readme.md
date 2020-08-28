This is a c++ implementation of Space Vector Modulation PWM schemes. The implementation also tests the scaling performed from the LUT with and online calculation. The implementation is visualized with plots to check against any weird looking parts. 

The LUT generation and scaling is desinged to be copied directly to inside Teensy code. There is also a dedicated function on Teensy code that outputs calculated duty cycle for testing. The result of this function is to be copied inside print_array.hpp.

Currrently, only one SVPWM scheme is implemented. C implementations for teensy can be found on this folder under Teensy_SVPWM_drivers folder.