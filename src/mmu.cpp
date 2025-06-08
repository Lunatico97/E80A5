#include <mmu.hpp>

MMU::MMU()
{
    rb.init();
}

void MMU::lda(u8 value)
{   
    rb.loadU8(AF, value);
}

void MMU::sta(u16 value)
{
    mem.store(value, rb.fetchU8(AF));
}

void MMU::ldax(u16 value)
{
    rb.loadU8(AF, mem.retreive(value));
}

void MMU::stax(REG r)
{
    mem.store(tapU16(r), rb.fetchU8(AF));
}

void MMU::mov(REG r1, REG r2)
{
    rb.loadU8(rb.mapToOffset(r1), tapU8(r2), r1%2);
}

void MMU::mvi(REG r, u8 value)
{
    rb.loadU8(rb.mapToOffset(r), value, r%2);
}

void MMU::lxi(REG r, u16 value)
{
    rb.loadU16(rb.mapToOffset(r), value);
}

void MMU::lhld(u16 value)
{
    rb.loadU8(HL, mem.retreive(value+1));
    rb.loadU8(HL, mem.retreive(value), 1);
}

void MMU::shld(u16 value)
{
    mem.store(value, rb.fetchU8(HL, 1));
    mem.store(value+1, rb.fetchU8(HL));
}

void MMU::xchg()
{
    rb.loadU16(WZ, rb.fetchU16(HL));
    rb.loadU16(HL, rb.fetchU16(DE));
    rb.loadU16(DE, rb.fetchU16(WZ));
}

void MMU::sphl()
{
    rb.loadU16(SP, rb.fetchU16(HL));
}

void MMU::xthl()
{
    // load L to [SP]
    // load H to [SP+1]
}

u8 MMU::tapU8(REG r)
{
    return rb.fetchU8(rb.mapToOffset(r), r%2);
}

u16 MMU::tapU16(REG r)
{
    return rb.fetchU16(rb.mapToOffset(r));
}