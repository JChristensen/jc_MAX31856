// Arduino MAX31856 Library
// https://github.com/JChristensen/jc_MAX31856
// Copyright (C) 2019 by Jack Christensen and licensed under
// GNU GPL v3.0, https://www.gnu.org/licenses/gpl.html
//
// Example sketch...

#include <jc_MAX31856.h>    // https://github.com/JChristensen/jc_MAX31856
#include <Streaming.h>      // http://arduiniana.org/libraries/streaming/

jc_MAX31856 tc1(10);

void setup()
{
    Serial.begin(115200);
    Serial << F( "\n" __FILE__ " " __DATE__ " " __TIME__ "\n" );
    tc1.begin();
    //tc1.read();
    tc1.factory();
    tc1.dumpRegs();
    Serial << "CR0 0x" << _HEX(tc1.getCR0()) << endl;
    Serial << "CR1 0x" << _HEX(tc1.getCR1()) << endl;
    Serial << "MASK 0x" << _HEX(tc1.getMASK()) << endl;
    Serial << "SR 0x" << _HEX(tc1.getSR()) << endl;
    Serial << "CJHF " << tc1.getCJHF_C() << "C " << tc1.getCJHF_F() << "F\n";
    Serial << "CJLF " << tc1.getCJLF_C() << "C " << tc1.getCJLF_F() << "F\n";
    Serial << "LTHFT " << tc1.getLTHFT_C() << "C " << tc1.getLTHFT_F() << "F\n";
    Serial << "LTLFT " << tc1.getLTLFT_C() << "C " << tc1.getLTLFT_F() << "F\n";
    Serial << "CJTO " << tc1.getCJTO_C() << "C " << tc1.getCJTO_F() << "F\n";
    Serial << "CJT " << tc1.getCJT_C() << "C " << tc1.getCJT_F() << "F\n";
    Serial << "LTC " << tc1.getLTCT_C() << "C " << tc1.getLTCT_F() << "F\n";
}

void loop()
{
}
