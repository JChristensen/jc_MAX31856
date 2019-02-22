// Arduino MAX31856 Library
// https://github.com/JChristensen/jc_MAX31856
// Copyright (C) 2019 by Jack Christensen and licensed under
// GNU GPL v3.0, https://www.gnu.org/licenses/gpl.html

#include <jc_MAX31856.h>

// set up the hardware and do an initial read
void jc_MAX31856::begin()
{
    pinMode(m_ssPin, OUTPUT);
    digitalWrite(m_ssPin, HIGH);
    SPI.begin();
    read();
}

// read all registers from the MAX31856 (16 bytes),
// return value of fault status register
uint8_t jc_MAX31856::read()
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
    return regs.SR;
}

// write all registers that are not read-only to the MAX31856
void jc_MAX31856::write()
{
    // if the cold junction sensor is disabled, then the cold
    // junction temperature registers can be written, so
    // write 12 bytes, else write 10 bytes (excluding CJTH and CJTL)
    uint8_t nReg = (regs.CR0 & _BV(CR0_CJDISABLE)) ? 12 : 10;
    
    SPI.beginTransaction(SPISettings(spiFreq, spiBitOrder, spiMode));
    digitalWrite(m_ssPin, LOW);
    uint8_t *p = &regs.CR0;         // pointer to the register struct
    SPI.transfer(0x80);             // address byte, start at address zero
    for (uint8_t i=0; i<nReg; i++)
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
    regs.LTCBH  = 0x00;
    regs.LTCBM  = 0x00;
    regs.LTCBL  = 0x00;
    regs.SR     = 0x00;
}

// return linearized temperature high fault threshold as integer in °C*16
int16_t jc_MAX31856::getLTHFT()
{
    return (static_cast<int16_t>(regs.LTHFTH) << 8)
          + static_cast<int16_t>(regs.LTHFTL);
}

// return linearized temperature low fault threshold as integer in °C*16
int16_t jc_MAX31856::getLTLFT()
{
    return (static_cast<int16_t>(regs.LTLFTH) << 8)
          + static_cast<int16_t>(regs.LTLFTL);
}

// return cold junction temperature as integer in °C*64
int16_t jc_MAX31856::getCJT()
{
    return static_cast<int16_t>((regs.CJTH << 8) + regs.CJTL) / 4;
}

// return linearized thermocouple temperature as an integer in °C*128
int32_t jc_MAX31856::getLTCT()
{
    return ((static_cast<int32_t>(regs.LTCBH) << 24)
          + (static_cast<int32_t>(regs.LTCBM) << 16)
          + (static_cast<int32_t>(regs.LTCBL) << 8)) / 8192L;
}

// set averaging mode
void jc_MAX31856::setAvgSel(avgsel_t val)
{
    uint8_t cr1 = regs.CR1 & 0x0f;
    regs.CR1 = cr1 | val;
}

// set thermocouple type
void jc_MAX31856::setTcType(tctype_t val)
{
    uint8_t cr1 = regs.CR1 & 0xf0;
    regs.CR1 = cr1 | val;
}

// set thermocouple temperature high and low fault thresholds as integer in °C*16
void jc_MAX31856::setLTHFT(int16_t val)
{
    regs.LTHFTH = val >> 8;
    regs.LTHFTL = val & 0xff;
}

void jc_MAX31856::setLTLFT(int16_t val)
{
    regs.LTLFTH = val >> 8;
    regs.LTLFTL = val & 0xff;
}

// set cold junction temperature given as integer in °C*64
// (for when an external CJ sensor is used)
// max 127.984375°C, min -128°C but clamped by the chip at -64°C
void jc_MAX31856::setCJT(int16_t val)
{
    val <<= 2;
    regs.CJTH = val >> 8;
    regs.CJTL = val & 0xff;
}
