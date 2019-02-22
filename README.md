# Arduino MAX31856 Library
https://github.com/JChristensen/jc_MAX31856  
README file  
Jack Christensen  
Feb 2019  

## License
Arduino MAX31856 Library Copyright (C) 2019 Jack Christensen GNU GPL v3.0

This program is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License v3.0 as published by the Free Software Foundation.

This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program. If not, see <https://www.gnu.org/licenses/gpl.html>

## Introduction
The Maxim Integrated MAX31856 Thermocouple to Digital Converter has 16 eight-bit registers representing 11 parameters as follows:

* CR0 - Configuration Register 0
* CR1 - Configuration Register 1
* MASK - Fault Mask Register
* CJHF - Cold Junction High Fault Threshold
* CJLF - Cold Junction Low Fault Threshold
* LTHFT - Linearized Temperature High Fault Threshold (2 bytes)
* LTLFT - Linearized Temperature Low Fault Threshold (2 bytes)
* CJTO - Cold Junction Temperature Offset
* CJT - Cold Junction Temperature (2 bytes)
* LTC - Linearized Thermocouple Temperature (3 bytes, read-only)
* SR - Fault Status Register (read-only)

LTC and SR are read-only. CJT is read-only unless the cold junction sensor is disabled by CJ bit in CR0.

This library works by maintaining a copy of the registers in static RAM. The `read()` function reads all registers from the MAX31856 into SRAM and the `write()` function writes all values from SRAM to the MAX31856 for those registers that are not read-only.

A series of get and set functions are provided to retrieve and change the register values held in static RAM, see [the .h file](https://github.com/JChristensen/jc_MAX31856/blob/master/src/jc_MAX31856.h).

To retrieve current register values from the MAX31856, first call `read()`, then one or more of the get functions as needed.

To change register values in the MAX31856, first call `read()`, then call one or more of the set functions to change values as needed, then call `write()`.

When writing values to any of the fault threshold, cold junction offset or cold junction temperature registers, the user must ensure that the values are constrained to the ranges appropriate for the register(s). Ranges are noted in [the .h file](https://github.com/JChristensen/jc_MAX31856/blob/master/src/jc_MAX31856.h); also see the MAX31856 datasheet.