// Arduino MAX31856 Library
// https://github.com/JChristensen/jc_MAX31856
// Copyright (C) 2019 by Jack Christensen and licensed under
// GNU GPL v3.0, https://www.gnu.org/licenses/gpl.html

#ifndef JC_MAX31856_H_INCLUDED
#define JC_MAX31856_H_INCLUDED
#include <Arduino.h>
#include <SPI.h>
#include <Streaming.h>      // http://arduiniana.org/libraries/streaming/

class jc_MAX31856
{
    public:
        static const uint32_t spiFreq = 4e6;
        static const uint8_t spiBitOrder = MSBFIRST;
        static const uint8_t spiMode = SPI_MODE0;

        jc_MAX31856(uint8_t ssPin) : m_ssPin(ssPin) {}
        void begin();
        void read();
        void write();
        void dumpRegs();
        void factory();

        uint8_t getCR0()   {return regs.CR0;}
        uint8_t getCR1()   {return regs.CR1;}
        uint8_t getMASK()  {return regs.MASK;}
        int8_t  getCJHF()  {return static_cast<int8_t>(regs.CJHF);}
        int8_t  getCJLF()  {return static_cast<int8_t>(regs.CJLF);}
        int16_t getLTHFT() {return (static_cast<int16_t>(regs.LTHFTH) << 8) + regs.LTHFTL;}
        int16_t getLTLFT() {return (static_cast<int16_t>(regs.LTLFTH) << 8) + regs.LTLFTL;}
        int8_t  getCJTO()  {return static_cast<int8_t>(regs.CJTO);}
        int16_t getCJT()   {return static_cast<int16_t>((regs.CJTH << 8) + regs.CJTL) / 4;}
        int32_t getLTCT();
        uint8_t getSR()    {return regs.SR;}

        float getLTCT_C();
        float getLTCT_F();

        void setCR0(uint8_t val) {regs.CR0 = val;}
        void setCR1(uint8_t val) {regs.CR1 = val;}
        void setMASK(uint8_t val) {regs.MASK = val;}
        void setCJHF(int8_t val) {regs.CJHF = static_cast<uint8_t>(val);}
        void setCJLF(int8_t val) {regs.CJLF = static_cast<uint8_t>(val);}
        void setLTHFT(int16_t val) {}
        void setLTLFT(int16_t val) {}
        void setCJTO(int8_t val) {regs.CJTO = static_cast<uint8_t>(val);}
        void setCJT(int16_t val) {}

    private:
        struct              // MAX31856 registers
        {
            uint8_t
                CR0,                    // config reg 0
                CR1,                    // config reg 1
                MASK,                   // fault mask
                CJHF,                   // cold junction high fault threshold, °C
                CJLF,                   // cold junction low fault threshold, °C
                LTHFTH, LTHFTL,         // temperature high fault threshold, MSB, LSB, °C*16
                LTLFTH, LTLFTL,         // temperature low fault threshold, MSB, LSB, °C*16
                CJTO,                   // cold junction temperature offset, °C*16
                CJTH, CJTL,             // cold junction temperature, MSB, LSB, °C*64,
                                        //   14 bits, left justified, 2 LS bits not used.
                LTCBH, LTCBM, LTCBL,    // linearized TC temperature, bytes 2, 1, 0, °C*128,
                                        //   19 bits, left justified, 5 LS bits not used, read only.
                SR;                     // fault status register, read only.
        } regs;

        uint8_t m_ssPin;    // slave select pin
};

#endif
