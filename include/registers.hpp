#include <utils.hpp>

#ifndef _REGISTER_BANK_H_
#define _REGISTER_BANK_H_

enum OFF
{
    AF = 0,
    BC, DE, HL, SP, PC, ST, WZ
};

enum REG
{
    A = 0,
    F, B, C, D, E, H, L, S
};

class Registers
{
    public:
        Registers();
        
        // Initialization
        void init();

        // Mapping
        OFF mapToOffset(REG r);

        // Transfers
        void loadU8(OFF off, u8 value, bool ln = false);
        void loadU16(OFF off, u16 value);
        u8 fetchU8(OFF off, bool ln = false);
        u16 fetchU16(OFF off);

        // Incrementor / Decrementor
        void incU8(OFF off, bool ln = false);
        void incU16(OFF off);
        void decU8(OFF off, bool ln = false);
        void decU16(OFF off);

    private:
        // Register Bank
        u16 BANK[8];
};

#endif