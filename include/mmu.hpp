#include <registers.hpp>
#include <memory.hpp>

#ifndef _MMU_H_
#define _MMU_H_

class MMU
{
    public:
        MMU();

        // Accumulator
        void lda(u8 value);
        void sta(u16 value);
        void ldax(REG r, u16 value);
        void stax(REG r);

        // Register
        void mov(REG r1, REG r2);
        void mvi(REG r, u8 value);
        void lxi(REG r, u16 value);
        void lhld(u16 value);
        void shld(u16 value);

        // Incrementor / Decrementor
        void inc(REG r) { rb.incU8(rb.mapToOffset(r), r%2); }
        void dec(REG r) { rb.decU8(rb.mapToOffset(r), r%2); }
        void inx(OFF o) { rb.incU16(o); }
        void dcx(OFF o) { rb.decU16(o); }

        // Special
        void xchg();
        void sphl();
        void xthl();

        // Taps
        u8 tapU8(REG r);
        u16 tapU16(REG r);
        u16 tapU16(OFF o);

    private:
        Registers rb;
        Memory mem;
};

#endif