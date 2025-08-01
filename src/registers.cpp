#include <registers.hpp>

Registers::Registers() {}

void Registers::init()
{
    for(int i=0; i<7; i++)
    {
        BANK[i] = 0x0000;
    }
}

OFF Registers::mapToOffset(REG r)
{
    switch(r)
    {
        case A: return AF;
        case F: return AF;
        case B: return BC;
        case C: return BC;
        case D: return DE;
        case E: return DE;
        case H: return HL;
        case L: return HL;
        case S: return SP;
        default: return WZ;
    }
}

u8 Registers::fetchU8(OFF off, bool ln)
{
    if(!ln) return (*(BANK+off) & 0xFF00) >> 8;
    return *(BANK+off) & 0x00FF;
}

u16 Registers::fetchU16(OFF off)
{
    return *(BANK+off);
}

void Registers::loadU8(OFF off, u8 value, bool ln)
{
    if(!ln)
    {
        *(BANK+off) &= 0x00FF;
        *(BANK+off) |= (value << 8);
    }
    else
    {
        *(BANK+off) &= 0xFF00;
        *(BANK+off) |= value;
    }
}

void Registers::loadU16(OFF off, u16 value)
{
    *(BANK+off) = value;
}

void Registers::incU8(OFF off, bool ln)
{
    if(!ln) *(BANK+AF) += 0x0100;
    else *(BANK+off) = (*(BANK+off) & 0xFF00) | ((*(BANK+off) & 0x00FF) + 0x0001);
}

void Registers::incU16(OFF off)
{
    *(BANK+off) += 0x0001;
}

void Registers::decU8(OFF off, bool ln)
{
    if(!ln) *(BANK+AF) -= 0x0100;
    else *(BANK+off) = (*(BANK+off) & 0xFF00) | ((*(BANK+off) & 0x00FF) - 0x0001);
}

void Registers::decU16(OFF off)
{
    *(BANK+off) -= 0x0001;
}
