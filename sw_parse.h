//-------------------------------------------------------------------
// sw_parse.h
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

#ifndef SWPARSE_H
#define SWPARSE_H

void    ClearBuff(unsigned int offset);
void    ResetBuffLen();

int     CpyStr(char *dest, char *src);
void    CpynStr(char *dest, char *src, int sz);

int     HexDigit(char hexnum);
void    CvtInt2Hex(int val, char *hexstr);

void    FlushInput();
bool    GetCommand();
bool    DecodeCommand();
void    GetInput();

#endif
