#include <mmu.hpp>

MMU::MMU()
{
    rb.init();
}

void MMU::load_mem(u16 address, u8 value)
{
    mem.store(address, value);
}

u8 MMU::fetch_mem(u16 address)
{
    return mem.retreive(address);
}

void MMU::init_acc(const u16& value)
{
    rb.loadU16(AF, value); 
}

void MMU::init_pc(u16 loc)
{
    rb.loadU16(PC, loc); 
}

u16 MMU::load_pc()
{
    inx(PC);
    return tapU16(PC);
}

void MMU::lda(u16 value)
{   
    rb.loadU8(AF, mem.retreive(value));
}

void MMU::sta(u16 value)
{
    mem.store(value, rb.fetchU8(AF));
}

void MMU::ldax(REG r)
{
    rb.loadU8(AF, mem.retreive(tapU16(r)));
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

u16 MMU::tapU16(OFF o)
{
    return rb.fetchU16(o);
}