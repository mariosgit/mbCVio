# mb'CVio
Library to control the mb'CVio board. This contains I2C devices:
- MCP23008 controlls 8 LEDs
- MCP4728 2x 4Ch DACs for 6 out channels and 2 bias values
- MCP3426 2Ch ADC for 2 input CV.
In and outputs are converted to +-5V range by OpAmp adders.

## Used Libraries
- mcp4728 https://github.com/BenoitSchillings/mcp4728
- Adafruit-MCP23008-library https://github.com/adafruit/Adafruit-MCP23008-library

## Internal  Remarks

Range 12bit 4096 -> 0-4,096V amped to -5..5V with val-2048*2,5

All Value Inverted:
DAC [0..4095] > [5..-5V]
ADC [-5..5V]  > [2047..0]

connector:  X8  ,   X7,    X6,     X5    , X4    , X3    , X2    , X1
function:   adc0,  adc1,   dac1.3, dac1.2, dac0.1, dac0.0, dac0.3, dac0.2
min val:       0                           -4,96 , -4,874, -4,928, -4,842
max val:    2047                            4,791,  4,868,  4,809,  4,851
