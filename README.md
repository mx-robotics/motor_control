# Brushless DC Motor Controller and PCB Layouts
* To build and upload the code, PlatformIO is being used. To get PlatformIo very quickly:

`` sudo python -c "$(curl -fsSL https://raw.githubusercontent.com/platformio/platformio/develop/scripts/get-platformio.py)"``

* Otherwise refer to [PlatformIO Installation Guide](https://docs.platformio.org/en/latest/installation.html#virtual-environment) 


* After getting PlatformIO:
	* Open a terminal and  run ``pio update`` 
	* To configure the project for platformIO and clion, navigate to Software/1Motor.. and run ``pio init --ide clion --board teensy31``. 
You can use another IDE as well, refer to PlatformIO [integration guide](https://docs.platformio.org/en/latest/ide.html#desktop-ide).

* Open your IDE and simply _Open_ the project folder (Software/1Motor) * Choose PLATFORMIO_UPLOAD to upload the code to Teensy. Keep in mind that you need to power Teensy from an external power source. Power over USB must be **DISABLED** otherwise you will burn the board. 

* Compilation might fail due to a missing math library since we are using a newer compiler than the stardart issue. In this case simply copy the library from an older gccarmnoneaebi:
``cp ~/.platformio/packages/toolchain-gccarmnoneeabi@1.50401.190816/arm-none-eabi/lib/libarm_cortexM4l_math.a ~/.platformio/packages/toolchain-gccarmnoneeabi/arm-none-eabi/lib``
For manual copying you can look for the missing library under ``~/.platformio/packages``.


