#ifndef _TYPES_H_
#define _TYPES_H_

typedef unsigned char u8;
typedef unsigned short u16;

struct OPCODE 
{
    u8 value;
    int bytes;
};

struct HEX
{
    u8 h8[2];
    u16 h16;
};

#endif