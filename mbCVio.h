#pragma once

#include "Adafruit_MCP23008.h"
#include "mcp4728.h"

class mbCVio
{
public:
    mbCVio();

    bool begin();
    void adc();
    void led(uint8_t &ledpin);
    void loop();

    void setDACOffset(float volts); // dac's (all at once) can output relative to +- 5V (typically +5V for CV signals)

    volatile int16_t _adc0;
    volatile int16_t _adc1;

    int16_t _dacOffset; // 2000 = 0V, 0 = -5V, 4000 = +5V

private:
    Adafruit_MCP23008 mcp;
    mcp4728 dac0;
    mcp4728 dac1;

    const uint8_t adcadr = 0b01101001;
};
