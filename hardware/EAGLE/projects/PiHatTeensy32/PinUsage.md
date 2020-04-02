#PinUsage
##RPI Teensy v.2020.04

#### IN Pins (PWM)
* 6 pins total

| PWM     | IN_U0 | IN_V0 | IN_W0 | IN_U1 | IN_V1 | IN_W1 |
|:----------:|:-----:|:-----:|:-----:|:-----:|:-----:|:-----:|
| Teensy 3.2 |   23  |   22  |   10  |   5   |   6   |   9   |
| Teensy 4.0 |   23  |   22  |   10  |   5   |   6   |   9   |

* Alternative (6 Pins under the Teensy 4.0, far side)

| PWM     | IN_U0 | IN_V0 | IN_W0 | IN_U1 | IN_V1 | IN_W1 |
|:----------:|:-----:|:-----:|:-----:|:-----:|:-----:|:-----:|
| Teensy 3.2 |   23  |   22  |   10  |   5   |   6   |   9   |
| Teensy 4.0 |   34  |   35  |   36  |   37   |   38   |  39   |

#### I2C Pins
* 2 Pins

|     I2C    | SCL0 | SDA0 | SCL1 | SDA1|
|:----------:|:----:|:----:|:----:|:----:|
| Teensy 3.2 |  19  |  18  |  29  |  30  |
| Teensy 4.0 |  19  |  18  |  24   | 25  |


#### UART Pins(Hardware Serial)
* 2 pins

|    UART    | RX1 & HALL_U1| TX1 & HALL_V1 |
|:----------:|:---:|:---:|
| Teensy 3.2 |  0  |  1  |
| Teensy 4.0 |  0  |  1  |

#### SPI
* 5 Pins

|     SPI    | MISO & HALL_U0 | MOSI | SCK | CS0 & HALL_W0 | CS1 & HALL_V1 |
|:----------:|:----:|:----:|-----|-----|-----|
| Teensy 3.2 |  12  |  11  | 13  | 2   | 14   |
| Teensy 4.0 |  12  |  11  | 13  | 2   | 14   |


#### SERVO Pins
* 2 pins

|  SERVO PWM | SERVO_1 | SERVO_2 |
|:----------:|:-------:|:-------:|
| Teensy 3.2 |    3    |    4    |
| Teensy 4.0 |    3    |    4    |


#### ADC Pins
*6 Pins. It is important to make sure each motor operates from a different ADC module.

|     ADC    | IS_U0 | IS_V0 | IS_W0 | IS_U1 | IS_V1 | IS_W1 |
|:----------:|:-----:|:-----:|:-----:|:-----:|:-----:|:-----:|
| Teensy 3.2 |   15  |   20  |   21  |   27  |   16  |   17  |
| Teensy 4.0 |   15  |   20  |   21  |   28  |   16  |   17  |

#### INH Pins
* 4 Pins. 3 pins for Motor0, 1 pin for Motor1

|     INH    | INH_U0 | INH_V0 | INH_W0 | INH_U1 | INH_V1 | INH_W1 |
|:----------:|:------:|:------:|:------:|:------:|------:|------:|
| Teensy 3.2 |   26   |   33   |   31   |    25   |    8   |    28   |
| Teensy 4.0 |   29   |   32    |   27   |   31   |    8   |    30  |

#### HALL Pins
* 6 Pins (They are interconnected)

|     HALL    | HALL_U0 & MISO | HALL_V0 & CS1 | HALL_W0 & CS0 | HALL_U1 + RX | HALL_V1 + TX| HALL_W1 |
|:----------:|:-------:|:-------:|:-------:|:-------:|:-------:|:-------:|
| Teensy 3.2 |    12   |    14   |    2   |    1   |    0   |    7   |
| Teensy 4.0 |    12   |    14   |     2   |    1   |    0   |    7   |

