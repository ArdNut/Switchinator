//-------------------------------------------------------------------
// sw_parse.cpp
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
#include <stdlib.h>

#include "sw_parse.h"
#include "sw_gv.h"
#include "sw_defs.h"
#include "sw_io.h"

bool waitinput = true;

// Utility function: Wipe the command I/O buffer
void ClearBuff(unsigned int offset)
{
    // overflow guard
    if (offset > (MAXINSZ - 1))
        return;

    // underflow guard
    if (offset < 0)
        return;

    // Reset array contents to all zero values.
    for (int i = offset; i < MAXINSZ; i++) {
        gv_cmdinstr[i] = '\0';
    }

    for (int i = offset; i < MAXOUTSZ; i++) {
        gv_outstr[i] = '\0';
    }
}


// Could do more, but this is enough for now.
void ResetBuffLen()
{
    gv_inbuflen = 0;    // reset length
}


// Copies n number of characters from src to dest. It is pointer based,
// so the starting pointers in the arrays can be offset from [0]. Has
// a minimal overrun check for the src string.
void CpynStr(char *dest, char *src, int sz)
{
    for (int i = 0; i < sz; i++) {
        if (*src) {
            *dest++ = *src++;
        }
        else
            break;
    }
}


// Return numeric value of a hex digit
int HexDigit(char hexnum)
{
    int rc = 0;
    switch (hexnum) {
        case '0':
        case '1':
        case '2':
        case '3':
        case '4':
        case '5':
        case '6':
        case '7':
        case '8':
        case '9':
            rc = (hexnum - 0x30);
            break;
        case 'A':
        case 'a':
            rc = 10;
            break;
        case 'B':
        case 'b':
            rc = 11;
            break;
        case 'C':
        case 'c':
            rc = 12;
            break;
        case 'D':
        case 'd':
            rc = 13;
            break;
        case 'E':
        case 'e':
            rc = 14;
            break;
        case 'F':
        case 'f':
            rc = 15;
            break;
    }
    return rc;
}


// Convert integer to hex string using the string constructor.
void CvtInt2Hex(int val, char *hexstr)
{
    String outstr;

    outstr = String(val, HEX);
    outstr.toCharArray(hexstr,4);
}


// Flushes out any pending data in the serial input. Includes a counter
// as a sanity measure to prevent it from going into a continuous loop.
void FlushInput()
{
    int cnt = 0;

    while (Serial.available()) {
        Serial.read();
        cnt++;
        // prevent run-away loop
        if (cnt > 255) {
            Serial.println("FLUSH ERR");
            break;
        }
    }
}


// Simple input capture function to get incoming characters until either an
// EOL (LF or CR) is detected or the input buffer is full. If the first
// character detected is a CR or LF and the input count is zero, then it
// sets the gv_nullin flag and exits with a false return. This is used to
// emit a prompt character each time the user (or whatever) hits the "Enter"
// key.
bool GetCommand()
{
    bool rc = false;

    if (Serial.available() == 0) {
        return false;
    }

    gv_cmderr   = false;
    gv_nullin   = false;
    gv_idx      = 0;
    gv_inbuflen = 0;

    ClearBuff(0);

    if (Serial.available() > 0) {
        while (true) {
            if (Serial.available() > 0) {
                gv_inchar = Serial.read();

                // see if first char is LF or CR
                if ((gv_idx == 0) && ((gv_inchar == '\n') || (gv_inchar == '\r'))) {
                    gv_nullin = true;
                    break;
                }

                // read incoming command until LF encountered
                if ((gv_inchar == '\n') || (gv_inchar == '\r')) {
                    gv_cmdinstr[gv_idx] = '\0';
                    gv_inbuflen = gv_idx;
                    rc = true;
                    break;
                }

                // prevent buffer overflow
                // truncate input
                if (gv_idx >= MAXINSZ-1) {
                    gv_cmdinstr[MAXINSZ-1] = '\0';
                    gv_inbuflen = gv_idx;
                    rc = false;
                    break;
                }

                // save incoming char in input buffer
                gv_cmdinstr[gv_idx] = (char) gv_inchar;
                gv_idx++;
            }
        }
    }

    return rc;
}


// Command string decoder
//
// Command  Response  Description                     In/Out Format
// ----------------------------------------------------------------
// A:n      A:n:val   Get analog input n in raw DN    Hex
// R:n      R:n:val   Read status of output n         Hex (0 or 1)
// W:n:val  OK        Write 0 or 1 to output n        Hex
// S:val    OK        Set all outputs to hex value    4 digit hex
// G:?      G:val     Get hex value for all outputs   4 digit hex
//
// Writes response output to gv_outstr.
bool DecodeCommand()
{
    bool rc = false;
    int pos = 0;
    int aport = 0;

    if (!gv_inbuflen) {
        return false;
    }

    // syntax check
    if (gv_cmdinstr[1] != ':')
        return false;

    // Parse commands
    switch (gv_cmdinstr[0]) {
        case 'A':
            if (gv_cmdinstr[1] == ':') {
                // analog chan is 0 to 3, use simple ASCII to int conversion
                aport = (int)(gv_cmdinstr[2] - '0');

                // check range
                if ((aport < 0) || (aport > 3)) {
                    rc = false;
                }
                else {
                    // Leave first 3 chars as-is, add colon and value
                    CpynStr(gv_outstr, gv_cmdinstr, 3);
                    gv_outstr[3] = ':';
                    CvtInt2Hex(gv_curranalog[aport], &gv_outstr[4]);
                    rc = true;
                }
            }
            break;
        case 'G':
            if (gv_cmdinstr[2] != '?') {
                rc = false;
            }
            else {
                // Leave first 2 chars as-is, add colon and value
                CpynStr(gv_outstr, gv_cmdinstr, 2);
                // Replace '?' with hex value
                CvtInt2Hex(gv_statebits, &gv_outstr[2]);
                rc = true;
            }
            break;
        case 'R':
            pos = HexDigit(gv_cmdinstr[2]);

            if (!CheckBitPos(pos)) {
                rc = false;
            }
            else {
                // Leave first 3 chars as-is, add colon and value
                CpynStr(gv_outstr,gv_cmdinstr,3);
                gv_outstr[3] = ':';
                gv_outstr[4] = (char)(GetOutputBit(pos) ? '1':'0');
                rc = true;
            }
            break;
        case 'S':
            rc = true;  // be optimistic
            gv_statebits = strtol(&gv_cmdinstr[2], NULL, 16);
            gv_outstr[0] = 'O';
            gv_outstr[1] = 'K';
            gv_outstr[2] = '\0';
            break;
        case 'W':
            rc = true;  // be optimistic

            // syntax check
            if (gv_cmdinstr[3] != ':') {
                rc = false;
            }
            else {
                pos = HexDigit(gv_cmdinstr[2]);
                if (!CheckBitPos(pos)) {
                    rc = false;
                }
                else {
                    if (gv_cmdinstr[4] == '1') {
                        SetOutputBit(pos, 1);
                        gv_outstr[0] = 'O';
                        gv_outstr[1] = 'K';
                        gv_outstr[2] = '\0';
                    }
                    else if (gv_cmdinstr[4] == '0') {
                        SetOutputBit(pos, 0);
                        gv_outstr[0] = 'O';
                        gv_outstr[1] = 'K';
                        gv_outstr[2] = '\0';
                    }
                    else {
                        rc = false;
                    }
                }
            }
            break;
    }

    return rc;
}


// Input capture and command decode sequencer
void GetInput()
{
    // Emit a prompt character
    if (waitinput) {
        waitinput = false;
        Serial.println();
        Serial.print(INPPCH);
    }

    // Check for incoming command
    if (GetCommand()) {
        waitinput = true;      // Reset input prompt output flag

        if ((((int)gv_cmdinstr == 10) ||
             ((int)gv_cmdinstr == 13)) &&
             (gv_inbuflen == 0)) {

            ClearBuff(0);
            ResetBuffLen();
            // if no input then enable loop delay
            gv_usedelay = true;
            return;
        }

        // Parse command (if received)
        if (DecodeCommand()) {
            // Return response to control host (or user)
            Serial.println();
            Serial.print(OUTPCH);
            Serial.print(gv_outstr);
        }
        else {
            Serial.println("ER");
        }
    }
    else {
        if (gv_cmderr) {
            Serial.println("ER");
            waitinput = true;      // Reset input prompt output flag
        }
        ClearBuff(0);
        ResetBuffLen();
        // if no input then enable loop delay
        gv_usedelay = true;

        if (gv_nullin) {
            waitinput = true;
            gv_nullin = false;
        }
    }
}
