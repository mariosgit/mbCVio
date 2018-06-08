
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

private:
    Adafruit_MCP23008 mcp;
    mcp4728 dac0;
    mcp4728 dac1;

    const uint8_t adcadr = 0b01101001;
};
