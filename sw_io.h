//-------------------------------------------------------------------
// sw_io.h
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

#include <stdint.h>

#ifndef SWIO_H
#define SWIO_H

void     initIOexp();
void     TestI2C();

bool     CheckBitVal(uint8_t val);
bool     CheckBitPos(uint8_t pos);;

void     GetAnalog(uint16_t chan);
void     ScanAnalog();

void     SetOutputBit(uint8_t pos, uint8_t val);
uint8_t  GetOutputBit(uint8_t pos);

void     SetDigBits();

#endif
