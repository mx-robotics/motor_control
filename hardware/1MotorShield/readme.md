# Pin Maps for 1M Inverter Board and Teensy

## 1M Board

*  SPI
	* CS: 10
	* CLK: 14
	* MOSI: 11
	* MISO: 12
* Current Sense
	* IS_W: A17
	* IS_V: A16
	* IS_U: A15
* INHIBIT Pins 
	* INH_W: 31/A20
	* INH_V: 24
	* INH_U: 33
* Servo Pins:
	* S_PWM1: 25
	* S_PWM2: 32
* IN Pins (for actively controlling the half-bridges with PWM)
	* They do not directly connect to Teensy and must be connected with cables. Candidate pins that use FTM0 are adjacent and are **21, 22 and 23**.
* Hall Pins
	* HALL_W: A13
	* HALL_U: A12
	* HALL_V: A11
* I2C Pins 0
	* SDA0: 18
	* SCL0: 19
* I2C Pins 1:
	* SDA1: 30
	* SCL1: 29


