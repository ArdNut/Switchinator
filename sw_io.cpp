//-------------------------------------------------------------------
// sw_io.cpp
//
// Created for "Arduino in a Nutshell", 2015, J. M. Hughes
// Chapter 10
//
// Copyight 2015-2016 John M. Hughes
// O'Reilly appreciates, but does not require, attribution. An attribution
// usually includes the title, author, publisher, and ISBN. For example:
// "Arduino in a Nutshell, by John M. Hughes. Copyright 2016 O’Reilly
// Media, Inc., ISBN: 978-1-4919-2176-0.
//
// If you feel your use of code examples falls outside fair use or the
// permission given above, feel free to contact permissions@oreilly.com.
//-------------------------------------------------------------------

#include <Arduino.h>
#include <stdint.h>
#include <Wire.h>

#include "Adafruit_MCP23017.h"

#include "sw_gv.h"
#include "sw_defs.h"
#include "sw_io.h"


// The Adafruit MCP23017 library is instantiated here because we don't
// really need it to be global.
Adafruit_MCP23017 ioExp;


// Calls the library initialization method, begin(), to get things up
// and ready for action.
void initIOexp()
{
    ioExp.begin();

    // Switchinator is an output device only
    for (int i = 0; i < 16; i++) {
        ioExp.pinMode(i, OUTPUT);
    }
}


// Steps through all 16 I/O pins on the MCP23017. Should only be used
// for testing (see Switchinator.ino).
void TestI2C()
{
    for (int i = 0; i < 16; i++) {
        ioExp.digitalWrite(i, LOW);
        delay(100);
    }

    for (int i = 0; i < 16; i++) {
        ioExp.digitalWrite(i, HIGH);
        delay(100);
    }
}


// Verifies that a value is either zero or one.
bool CheckBitVal(uint8_t val)
{
    if ((val < 0) || (val > 1)) {
        return false;
    }

    return true;
}


// Verifies that a bit position is between 0 and 15.
bool CheckBitPos(uint8_t pos)
{
    if ((pos < 0) || (pos > 15)) {
        return false;
    }

    return true;
}


// Updates the gv_curranalog array with data from the
// specified analog input channel.
void GetAnalog(uint16_t chan)
{
    gv_curranalog[chan] = analogRead(gv_aportmap[chan]);
}


// Read the first four analog inputs. Why only four? Because A4 and
// A5 are used for the I2C interface to the MCP23017 and possibly
// other I2C devices.
void ScanAnalog()
{
    GetAnalog(0);
    GetAnalog(1);
    GetAnalog(2);
    GetAnalog(3);
}


// Sets a bit in the output state array. The change is picked up and
// applied when the function SetDigBits() is called.
void SetOutputBit(uint8_t pos, uint8_t val)
{
    if (CheckBitPos(pos)) {
        if (CheckBitVal(val)) {
            bitWrite(gv_statebits, pos, val);
        }
    }
}


// Returns a specific bit from the output state array.
uint8_t GetOutputBit(uint8_t pos)
{
    if (CheckBitPos(pos)) {
        return bitRead(gv_statebits, pos);
    }

    return 0;
}


// note that what gets written to the MCP23017 is inverted so that
// 1 = on (active high) instead of 1 = off (active low). This makes
// it easier on both a human user and whatever is controlling the
// Switchinator.
void SetDigBits()
{
    uint16_t invbits = 0;

    // See if bits need to be updated
    if (gv_lastbits == gv_statebits) {
        return;
    }
    else {
        gv_lastbits = gv_statebits;

    }

    ioExp.writeGPIOAB(~gv_lastbits);
}

