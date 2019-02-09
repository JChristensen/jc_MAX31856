// Arduino MAX31856 Library
// https://github.com/JChristensen/jc_MAX31856
// Copyright (C) 2019 by Jack Christensen and licensed under
// GNU GPL v3.0, https://www.gnu.org/licenses/gpl.html

#include <SPI.h>
#include <jc_MAX31856.h>

void jc_MAX31856::begin()
{
    pinMode(m_ssPin, OUTPUT);
    digitalWrite(m_ssPin, HIGH);
    SPI.begin();
}

// read all registers from the MAX31856 (16 bytes)
void jc_MAX31856::read()
{
    SPI.beginTransaction(SPISettings(spiFreq, spiBitOrder, spiMode));
    digitalWrite(m_ssPin, LOW);
    uint8_t *p = &regs.CR0;         // pointer to the register struct
    SPI.transfer(0);                // address byte, start at address zero
    for (uint8_t i=0; i<16; i++)    // read 16 bytes
    {
        *p++ = SPI.transfer(0);
    }
    SPI.endTransaction();
    digitalWrite(m_ssPin, HIGH);
}

// write all registers that are not read-only to the MAX31856 (12 bytes)
void jc_MAX31856::write()
{
    SPI.beginTransaction(SPISettings(spiFreq, spiBitOrder, spiMode));
    digitalWrite(m_ssPin, LOW);
    uint8_t *p = &regs.CR0;         // pointer to the register struct
    SPI.transfer(0x80);             // address byte, start at address zero
    for (uint8_t i=0; i<12; i++)    // write 12 bytes
    {
        SPI.transfer(*p++);
    }
    SPI.endTransaction();
    digitalWrite(m_ssPin, HIGH);
}

// print the register values to serial
void jc_MAX31856::dumpRegs()
{
    Serial << F("0x");
    uint8_t *p = &regs.CR0;         // pointer to the register struct
    for (uint8_t i=0; i<16; i++)    // read 16 bytes
    {
        uint8_t val = *p++;
        if (val < 16) Serial << '0';
        Serial << _HEX(val);
        if (i < 15) Serial << F(", ");
    }
    Serial << endl;
}

// set all registers to factory values.
// after calling factory(), call write() to send the values to the MAX31856.
void jc_MAX31856::factory()
{
    regs.CR0    = 0x00;
    regs.CR1    = 0x03;
    regs.MASK   = 0xff;
    regs.CJHF   = 0x7f;
    regs.CJLF   = 0xc0;
    regs.LTHFTH = 0x7f;
    regs.LTHFTL = 0xff;
    regs.LTLFTH = 0x80;
    regs.LTLFTL = 0x00;
    regs.CJTO   = 0x00;
    regs.CJTH   = 0x00;
    regs.CJTL   = 0x00;
    regs.LTCBH  = 0xff;
    regs.LTCBM  = 0xf0;
    regs.LTCBL  = 0x00;
    regs.SR     = 0x00;
}

// return raw thermocouple temperature as an integer, degrees Celsius * 128.
int32_t jc_MAX31856::getLTCT()
{
return ((static_cast<int32_t>(regs.LTCBH) << 24)
      + (static_cast<int32_t>(regs.LTCBM) << 16)
      + (static_cast<int32_t>(regs.LTCBL) << 8)) / 8192L;
}

// return thermocouple temperature in Celsius as floating point
float jc_MAX31856::getLTCT_C()
{
    return static_cast<float>(getLTCT()) / 128.0;
}

// return thermocouple temperature in Fahrenheit as floating point
float jc_MAX31856::getLTCT_F()
{
    return getLTCT_C() * 1.8 + 32.0;
}
