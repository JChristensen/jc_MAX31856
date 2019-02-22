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
        static const uint32_t spiFreq = 2e6;
        static const uint8_t  spiBitOrder = MSBFIRST;
        static const uint8_t  spiMode = SPI_MODE3;

        // bits in config 0 register
        static const uint8_t
            CR0_CMODE     = 7,
            CR0_ONESHOT   = 6,
            CR0_OCFAULT1  = 5,
            CR0_OCFAULT0  = 4,
            CR0_CJDISABLE = 3,
            CR0_FAULTMODE = 2,
            CR0_FAULTCLR  = 1,
            CR0_NOISEFLT  = 0;

        // bits in fault mask register
        static const uint8_t
            MASK_CJ_HIGH = 5,
            MASK_CJ_LOW  = 4,
            MASK_TC_HIGH = 3,
            MASK_TC_LOW  = 2,
            MASK_OVUV    = 1,
            MASK_OPEN    = 0;

        // bits in fault status register
        static const uint8_t
            FAULT_CJ_RANGE = 7,
            FAULT_TC_RANGE = 6,
            FAULT_CJ_HIGH  = 5,
            FAULT_CJ_LOW   = 4,
            FAULT_TC_HIGH  = 3,
            FAULT_TC_LOW   = 2,
            FAULT_OVUV     = 1,
            FAULT_OPEN     = 0;

        // enums for config 1 register
        enum avgsel_t {AVG1=0x00, AVG2=0x10, AVG4=0x20, AVG8=0x30, AVG16=0x40};
        enum tctype_t {B_TYPE, E_TYPE, J_TYPE, K_TYPE, N_TYPE, R_TYPE,
                       S_TYPE, T_TYPE, GAIN8, GAIN32=12};

        jc_MAX31856(uint8_t ssPin) : m_ssPin(ssPin) {}
        void begin();
        uint8_t read();
        void write();
        void dumpRegs();
        void factory();

        // return CR0, CR1, MASK, SR as single bytes
        uint8_t getCR0()  {return regs.CR0;}
        uint8_t getCR1()  {return regs.CR1;}
        uint8_t getMASK() {return regs.MASK;}
        uint8_t getSR()   {return regs.SR;}
        avgsel_t getAvgSel() {return static_cast<avgsel_t>(regs.CR1 & 0xf0);}
        tctype_t getTcType() {return static_cast<tctype_t>(regs.CR1 & 0x0f);}

        // return cold junction high and low fault threshold
        int8_t getCJHF() {return static_cast<int8_t>(regs.CJHF);}           // as integer in °C
        int8_t getCJLF() {return static_cast<int8_t>(regs.CJLF);}           // as integer in °C
        float getCJHF_C() {return static_cast<float>(getCJHF());}           // as floating point in °C
        float getCJLF_C() {return static_cast<float>(getCJLF());}           // as floating point in °C
        float getCJHF_F() {return CtoF(getCJHF_C());}                       // as floating point in °F
        float getCJLF_F() {return CtoF(getCJLF_C());}                       // as floating point in °F

        // return linearized temperature high and low fault threshold
        int16_t getLTHFT();                                                 // as integer in °C*16
        int16_t getLTLFT();                                                 // as integer in °C*16
        float getLTHFT_C() {return static_cast<float>(getLTHFT()) / 16.0;}  // as floating point in °C
        float getLTLFT_C() {return static_cast<float>(getLTLFT()) / 16.0;}  // as floating point in °C
        float getLTHFT_F() {return CtoF(getLTHFT_C());}                     // as floating point in °F
        float getLTLFT_F() {return CtoF(getLTLFT_C());}                     // as floating point in °F

        // return cold junction temperature offset
        int8_t getCJTO() {return static_cast<int8_t>(regs.CJTO);}           // as integer in °C*16
        float getCJTO_C() {return static_cast<float>(getCJTO()) / 16.0;}    // as floating point in °C
        float getCJTO_F() {return getCJTO_C() * 1.8;}                       // as floating point in °F

        // return cold junction temperature
        int16_t getCJT();                                                   // as integer in °C*64
        float getCJT_C() {return static_cast<float>(getCJT()) / 64.0;}      // as floating point in °C
        float getCJT_F() {return CtoF(getCJT_C());}                         // as floating point in °F

        // return linearized thermocouple temperature
        int32_t getLTCT();                                                  // as integer in °C*128
        float getLTCT_C() {return static_cast<float>(getLTCT()) / 128.0;}   // as floating point in °C
        float getLTCT_F() {return CtoF(getLTCT_C());}                       // as floating point in °F

        // set CR0, CR1, MASK as unsigned bytes
        void setCR0(uint8_t val) {regs.CR0 = val;}
        void setCR1(uint8_t val) {regs.CR1 = val;}
        void setMASK(uint8_t val) {regs.MASK = val;}
        void setAvgSel(avgsel_t val);                                       // CR1 bits 6:4
        void setTcType(tctype_t val);                                       // CR1 bits 3:0

        // set cold junction high and low fault thresholds (-128°C to 127°C)
        void setCJHF(int8_t val) {regs.CJHF = static_cast<uint8_t>(val);}   // as integer in °C
        void setCJLF(int8_t val) {regs.CJLF = static_cast<uint8_t>(val);}   // as integer in °C
        void setCJHF_C(float val) {setCJHF(static_cast<float>(val));}       // as floating point in °C
        void setCJLF_C(float val) {setCJLF(static_cast<float>(val));}       // as floating point in °C
        void setCJHF_F(float val) {setCJHF_C(FtoC(val));}                   // as floating point in °F
        void setCJLF_F(float val) {setCJLF_C(FtoC(val));}                   // as floating point in °F

        // set thermocouple temperature high and low fault thresholds (-2048.0000°C to 2047.9375°C)
        void setLTHFT(int16_t val);                                         // as integer in °C*16
        void setLTLFT(int16_t val);                                         // as integer in °C*16
        void setLTHFT_C(float val) {setLTHFT(val * 16.0);}                  // as floating point in °C
        void setLTLFT_C(float val) {setLTLFT(val * 16.0);}                  // as floating point in °C
        void setLTHFT_F(float val) {setLTHFT_C(FtoC(val));}                 // as floating point in °F
        void setLTLFT_F(float val) {setLTLFT_C(FtoC(val));}                 // as floating point in °F

        // set cold junction offset (-8.0000°C to 7.9375°C)
        void setCJTO(int8_t val) {regs.CJTO = static_cast<uint8_t>(val);}   // as integer in °C*16
        void setCJTO_C(float val) {setCJTO(val * 16.0);}                    // as floating point in °C
        void setCJTO_F(float val) {setCJTO_C(val / 1.8);}                   // as floating point in °F

        // set cold junction temperature (for when an external CJ sensor is used)
        // range -64°C to 127.984375°C, the minimum is clamped by the chip (else would be -128°C)
        void setCJT(int16_t val);                                           // as integer in °C*64
        void setCJT_C(float val) {setCJT(val * 64.0);}                      // as floating point in °C
        void setCJT_F(float val) {setCJT_C(FtoC(val));}                     // as floating point in °F

        // temperature conversions
        float CtoF(float c) {return c * 1.8 + 32.0;}
        float FtoC(float f) {return (f - 32.0) / 1.8;}

    private:
        uint8_t m_ssPin;                // slave select pin
        struct                          // MAX31856 registers
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
};

#endif
