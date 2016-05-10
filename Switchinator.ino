//-------------------------------------------------------------------
// Switchinator.ino
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

#define PROTOMODE

// Include all modules

#include <stdint.h>
#include <Wire.h>

#include "Adafruit_MCP23017.h"

#include "sw_parse.h"
#include "sw_gv.h"
#include "sw_defs.h"
#include "sw_io.h"

// define the clock rate
#define F_CPU 16000000UL

#define TSTLED 2                // prototype mode only


void setup()
{
    initIOexp();                // init the MCP23017 class object

    #ifdef PROTOMODE
    pinMode(TSTLED, OUTPUT);
    digitalWrite(TSTLED, LOW);
    #endif

    // Startup banner and start flag string
    Serial.begin(9600);
    Serial.println();
    Serial.println("SWITCHINATOR V1.0");
    Serial.println("READY");
    Serial.println("####");     // Start flag

    #ifdef PROTOMODE
    delay(500);
    TestI2C();
    digitalWrite(TSTLED, HIGH);
    #endif

    // get serial I/O ready to go
    ClearBuff(0);
    ResetBuffLen();
    FlushInput();
}


void loop()
{
    // Read analog inputs and update GV array
    ScanAnalog();

    // Look for command input and process it
    GetInput();

    // Update the digital outputs using state bits
    SetDigBits();

    // delay briefly if no input detected
    if (gv_usedelay) {
        gv_usedelay = false;
        delay(50);
    }
}

