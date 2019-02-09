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
    Serial << tc1.getLTCT_C() << ' ' << tc1.getLTCT_F() << endl;
}

void loop()
{
}
