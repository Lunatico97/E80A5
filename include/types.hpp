#ifndef _TYPES_H_
#define _TYPES_H_

typedef unsigned char u8;
typedef unsigned short u16;

struct OPCODE
{
    u8 value;
    u8 bytes;
};

struct HEX
{
    u8 h8[3];
};

static const u8 IL_MAP[256] =
{
        //         0  1  2  3  4  5  6  7  8  9  A  B  C  D  E  F
        /* 0x00 */ 1, 3, 1, 1, 1, 1, 2, 1, 1, 3, 1, 1, 1, 1, 2, 1, // 0x0X (NOP, LXI BC, STAX BC, INX BC, INR B, DCR B, MVI B, RLC, NOP, DAD BC, LDAX BC, DCX BC, INR C, DCR C, MVI C, RRC)
        /* 0x10 */ 1, 3, 1, 1, 1, 1, 2, 1, 1, 3, 1, 1, 1, 1, 2, 1, // 0x1X (NOP, LXI DE, STAX DE, INX DE, INR D, DCR D, MVI D, RAL, NOP, DAD DE, LDAX DE, DCX DE, INR E, DCR E, MVI E, RAR)
        /* 0x20 */ 1, 3, 3, 1, 1, 1, 2, 1, 1, 3, 3, 1, 1, 1, 2, 1, // 0x2X (NOP, LXI HL, SHLD, INX HL, INR H, DCR H, MVI H, DAA, NOP, DAD HL, LHLD, DCX HL, INR L, DCR L, MVI L, CMA)
        /* 0x30 */ 1, 3, 3, 1, 1, 1, 2, 1, 1, 3, 3, 1, 1, 1, 2, 1, // 0x3X (NOP, LXI SP, STA, INX SP, INR M, DCR M, MVI M, STC, NOP, DAD SP, LDA, DCX SP, INR A, DCR A, MVI A, CMC)
        /* 0x40 */ 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, // 0x4X (MOV B,B - MOV B,A)
        /* 0x50 */ 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, // 0x5X (MOV D,B - MOV D,A)
        /* 0x60 */ 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, // 0x6X (MOV H,B - MOV H,A)
        /* 0x70 */ 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, // 0x7X (MOV M,B - MOV A,A, HLT)
        /* 0x80 */ 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, // 0x8X (ADD B - ADD A)
        /* 0x90 */ 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, // 0x9X (SUB B - SUB A)
        /* 0xA0 */ 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, // 0xAX (ANA B - ANA A)
        /* 0xB0 */ 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, // 0xBX (ORA B - ORA A, CMP B - CMP A)
        /* 0xC0 */ 1, 1, 3, 3, 3, 1, 2, 1, 1, 1, 3, 3, 3, 3, 3, 1, // 0xC0 (Conditional/Unconditional RET/JMP/CALL, ADI, RST)
        /* 0xD0 */ 1, 1, 3, 3, 3, 1, 2, 1, 1, 1, 3, 3, 3, 1, 2, 1, // 0xD0 (Conditional/Unconditional RET/JMP/CALL, SUI, RST, IN/OUT)
        /* 0xE0 */ 1, 1, 3, 1, 3, 1, 2, 1, 1, 1, 3, 1, 1, 1, 2, 1, // 0xE0 (PCHL, XCHG, XTHL, JMP, ANI, RST, SIM/RIM)
        /* 0xF0 */ 1, 1, 3, 1, 3, 1, 2, 1, 1, 1, 3, 1, 1, 1, 2, 1  // 0xF0 (PUSH, POP, DI, EI, SPHL, STI, ORI, RM/SIM, CPI)
};

#endif