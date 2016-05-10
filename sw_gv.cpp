//-------------------------------------------------------------------
// sw_gv.cpp
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

#include "sw_defs.h"

// Incoming command status
int     gv_inchar   = 0;            // Single input character
int     gv_idx      = 0;            // Input character counter
int     gv_inbuflen = 0;            // Number of chars in input buffer

char    gv_cmdinstr[MAXINSZ];       // Input buffer
char    gv_outstr[MAXOUTSZ];        // Output buffer

bool    gv_nullin   = false;        // Null (CR or LF) input flag
bool    gv_cmderr   = false;        // Syntax or format error flag
bool    gv_usedelay = false;        // True if no input detected

// I/O variables
uint16_t gv_statebits= 0;           // Output state bits
uint16_t gv_lastbits = 0xff;        // Change tracking

// Analog input mapping
int      gv_getaport = 0;           // last requested analog port (0..3)
uint16_t gv_aportmap[MAXACHANS] = {A0, A1, A2, A3};

// Analog input values
uint16_t gv_curranalog[MAXACHANS] = {0, 0, 0, 0};

// The trikery with gv_aportmap involves writing the actual analog
// port numbers into the array for the first four names defined in
// the Arduino include files for a particular board type. This allows
// Switchinator code to be used with something other than the custom
// PCB, like a Mega2560.
