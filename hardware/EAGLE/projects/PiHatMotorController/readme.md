# Pin Maps for 2M Inverter Board and Teensy

## 1M Board

*  SPI: There are 2 AMS Rotary Encoders that send info, so there are 2 CS Pins.
	* CS1: 9
  * CS2: 15/A1
	* CLK: 13
	* MOSI: 11
	* MISO: 12
  
* Current Sense
	* IS_W: A17
	* IS_V: A16
	* IS_U: A15
  * IS_W1: A12
	* IS_V2: A13
	* IS_U2: A17
  
* INHIBIT Pins 
	* INH_W: 33
	* INH_V: 3
	* INH_U: 24
  * INH_W2: 0
	* INH_V2: 1
	* INH_U2: 2
  
* Servo Pins:
	* S_PWM1: 32
	* S_PWM2: 25
  
* IN Pins (for actively controlling the half-bridges with PWM)
	* They do not directly connect to Teensy and must be connected with cables. Candidate pins that use FTM0 are adjacent and are **21, 22 and 23**.
	* IN_W: 10
	* IN_V: 6
	* IN_U: 5
  * IN_W2: 23
	* IN_V2: 22
	* IN_U2: 21
  
* Hall Pins
	* HALL_W: A13
	* HALL_U: A12
	* HALL_V: A11
  * HALL_W: A13
	* HALL_U: A12
	* HALL_V: A11
  
  
* I2C Pins 0
	* SDA0: 18
	* SCL0: 19
* I2C Pins 1:
	* SDA1: 30
	* SCL1: 29


