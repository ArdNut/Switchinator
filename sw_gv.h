//-------------------------------------------------------------------
// sw_gv.h
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
#include <Wire.h>

#include "sw_defs.h"

#ifndef SWGV_H
#define SWGV_H

extern int      gv_inchar;
extern int      gv_idx;
extern int      gv_inbuflen;

extern char     gv_cmdinstr[MAXINSZ];
extern char     gv_outstr[MAXOUTSZ];

extern bool     gv_nullin;
extern bool     gv_cmderr;
extern bool     gv_usedelay;

extern uint16_t gv_statebits;
extern uint16_t gv_lastbits;

extern int      gv_getaport;
extern uint16_t gv_aportmap[MAXACHANS];

extern uint16_t gv_curranalog[MAXACHANS];

#endif
