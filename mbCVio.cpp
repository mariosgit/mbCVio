#include "mbCVio.h"
#include <mbLog.h>

mbCVio::mbCVio() :
    mcp(),
    dac0(0),
    dac1(1),
    adcadr(0b01101001),
    _dacOffset(4000)
{
}

void mbCVio::setDACOffset(float val)
{
    float fval = fmax(5.0f, val);
    fval = fmin(-5.0f, fval);

    _dacOffset = fval; // 2000 = 0V, 0 = -5V, 4000 = +5V
    dac1.analogWrite(1, _dacOffset); // 0=ADCVref=1,024; 1=DACVref=2,048V
}

bool mbCVio::begin()
{
    bool result = true;
    /*// set second DAC adr to 1 (LED7 pin)
    mcp.digitalWrite(7, HIGH);
    Wire.beginTransmission(0x60);
    Wire.write(0b01100001);
    mcp.digitalWrite(7, LOW);
    Wire.write(0b01100110); //new adr = 1
    Wire.write(0b01100111); //confirmation
    Wire.endTransmission();
    */

    // test port expander
    mcp.begin();      // use default address 0
    mcp.pinMode(0, OUTPUT);
    mcp.pinMode(1, OUTPUT);
    mcp.pinMode(2, OUTPUT);
    mcp.pinMode(3, OUTPUT);
    mcp.pinMode(4, OUTPUT);
    mcp.pinMode(5, OUTPUT);
    mcp.pinMode(6, OUTPUT);
    mcp.pinMode(7, OUTPUT);

    // DAC
    dac0.begin();  // initialize i2c interface
    dac1.begin();  // initialize i2c interface
    dac0.setVref(1,1,1,1);
    dac1.setVref(1,1,1,1);
    dac0.setGain(1, 1, 1, 1);
    dac1.setGain(1, 1, 1, 1);
    dac0.analogWrite(2000,2000,2000,2000);
    dac1.analogWrite(0000,_dacOffset,2000,2000); // 0=ADCVref=1,024; 1=DACVref=2,048V

    // ADC
    Wire.beginTransmission(adcadr);
    // Send configuration command
    // Continuous conversion mode, Channel-1, 12-bits resolution
    Wire.write(0x10);
    uint8_t retadc = Wire.endTransmission();
    // 0:success
    // 1:data too long to fit in transmit buffer
    // 2:received NACK on transmit of address
    // 3:received NACK on transmit of data
    // 4:other error
    result &= (retadc == 0);
    // Serial.print("adc result:"); Serial.println(retadc);

    // Teensy Internal DACs, Pin3 as Gate
    analogWriteResolution(12);
    pinMode(3, OUTPUT);

    return result;
}

bool channel = false;
bool active = true;

void mbCVio::adc()
{
    if(!active)
        return;
    unsigned int data[2];

    // Start I2C Transmission
    Wire.beginTransmission(adcadr);
    // Select data register
    if(channel)
        Wire.write(0x10);
    else
        Wire.write(0x30);
    // Stop I2C Transmission
    Wire.endTransmission();

    // Request 2 bytes of data
    Wire.requestFrom(adcadr,2);

    // Read 2 bytes of data
    // raw_adc msb, raw_adc lsb 
    if(Wire.available() == 2)
    {
        data[0] = Wire.read();
        data[1] = Wire.read();

        // Convert the data to 12-bits
        int16_t raw_adc = (data[0] & 0x0F) * 256 + data[1];
        if(raw_adc > 2047)
        {
            raw_adc -= 4096;
        }

        if(channel)
            _adc0 = raw_adc;
        else
            _adc1 = raw_adc;

        // if(channel)
        //     LOG << "ADC0: ";
        // else
        //     LOG << "\t\tADC1: ";
        // LOG << raw_adc <<"\n";
    }
    else
    {
        // LOG << "Digital Value of Analog Input is : FAIL\n";
        // active = false;
    }
    channel = !channel;
}

bool leddir = true;
void mbCVio::led(uint8_t &ledpin)
{
    mcp.digitalWrite(ledpin, LOW);
    if(leddir)
        ledpin = (ledpin+1);
    else
        ledpin = (ledpin-1);
    if(ledpin == 0)
        leddir = true; //up
    if(ledpin == 7)
        leddir = false; //down

    mcp.digitalWrite(ledpin, HIGH);
    // Serial.print("led:");
    // Serial.println(ledpin);

}

// internal DAC, goes to 3.3V only !
float phase = 0.0;
float twopi = 3.14159 * 2;
void mbCVio::loop()
{
    float val = sin(phase) * 2000.0 + 2000.0;
    // Log.notice("\t\t\t\tdac: %d\n", (uint16_t)val);
    phase = phase + twopi * 0.01;
    if (phase >= twopi)
    {
        phase = 0;
    }
    // analogWrite(A14, (int)val);
    // digitalWrite(3, (phase > 3.14));
    dac0.analogWrite(2, (uint16_t)val);
    dac0.analogWrite(3, (uint16_t)val);
    dac0.analogWrite(0, (uint16_t)val);
    dac0.analogWrite(1, (uint16_t)val);
    dac1.analogWrite(2, (uint16_t)val);
    dac1.analogWrite(3, (uint16_t)val);
}
