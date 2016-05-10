//-------------------------------------------------------------------
// sw_defs.h
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

#ifndef SWDEFS_H
#define SWDEFS_H

#define INPPCH         "> "     // Input prompt character
#define OUTPCH         "< "     // Output prompt character

#define MAXACHANS       4       // max # of analog chans available
#define MAXINSZ         12      // input buffer size
#define MAXOUTSZ        12      // output buffer size

#define CMD_ST          0       // Command codes
#define CMD_AN          1
#define CMD_OUT         2
#define CMD_SP          3
#define CMD_GP          4

#define CMD_ALL         255     // Flag for an all-bits operation

#define OK              1
#define ERR             -1

#endif
