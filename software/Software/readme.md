Software folder consists of 3 packages:

- 1Motor: This is a working version for the 1Motor board software. PID can be tweaked and there are certainly some improvements to be made but in general it works good enough.

- 2Motor: This _should_ be a working version for the 2Motor board software. It is a crude port of 1Motor software for the 2Motor board and suffers from all the shortcomings of its predecessor.

- 1Motor_refactor: This is the future of this project which is yet _in_ _development_. It has a decoupled OOP design, but still aims to be lightweight. There are some expremental stuff with Singleton vs _-all-static-class-_ implementations, _constexper_ classes and some other things. 
It is compiled with a _newer_ version of the default arm_gcc_aebi which is stated in the platform.ini file and should be automatically download. If an issue arises due to a missing library, just find the .a from the default compiler location and copy it to the same folder in the newer version. There might also be some issues due to compiling the Teensy libraries with gcc17 so watch out for weird stuff.