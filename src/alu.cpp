#include <alu.hpp>

ALU::ALU() {}
ALU::ALU(MMU& mmu) { this->mmu = mmu; }

void ALU::add(REG r)
{
    fetch(r);
    ACC += TEMP;
    load();
}

void ALU::adi(u8 value)
{
    fetch(value);
    ACC += TEMP;
    load();
}

void ALU::sub(REG r)
{
    fetch(r);
    ACC -= TEMP;
    load();
}

void ALU::sui(u8 value)
{
    fetch(value);
    ACC -= TEMP;
    load();
}

void ALU::ana(REG r)
{
    fetch(r);
    ACC &= TEMP;
    load();
}

void ALU::ani(u8 value)
{
    fetch(value);
    ACC &= TEMP;
    load();
}

void ALU::ora(REG r)
{
    fetch(r);
    ACC |= TEMP;
    load();
}

void ALU::ori(u8 value)
{
    fetch(value);
    ACC |= TEMP;
    load();
}

void ALU::xra(REG r)
{
    fetch(r);
    ACC ^= TEMP;
    load();
}

void ALU::xri(u8 value)
{
    fetch(value);
    ACC ^= TEMP;
    load();
}

void ALU::cmp(REG r)
{
    fetch(r);
    if(ACC < TEMP) { SF |= HX_CARY; SF &= ~HX_ZERO; /* A < R => CY = 1, Z = 0 */ }
    else if(ACC == TEMP) { SF &= ~HX_CARY; SF |= HX_ZERO; /* A = R => CY = 0, Z = 1 */ }
    else { SF &= ~(HX_CARY | HX_ZERO); /* A > R => CY = 0, Z = 0 */ }
    load();
}

void ALU::cpi(u8 value)
{
    fetch(value);
    if(ACC < TEMP) { SF |= HX_CARY; SF &= ~HX_ZERO; /* A < R => CY = 1, Z = 0 */ }
    else if(ACC == TEMP) { SF &= ~HX_CARY; SF |= HX_ZERO; /* A = R => CY = 0, Z = 1 */ }
    else { SF &= ~(HX_CARY | HX_ZERO); /* A > R => CY = 0, Z = 0 */ }
    load();
}

void ALU::fetch(u8 value)
{
    ACC = mmu.tapU8(A);
    SF = mmu.tapU8(F);
    TEMP = value;
}

void ALU::fetch(REG r)
{
    ACC = mmu.tapU8(A);
    SF = mmu.tapU8(F);
    TEMP = mmu.tapU8(r); 
}

void ALU::load()
{
    u16 h16 = (static_cast<u16>(SF) | (static_cast<u16>(ACC) << 8));
    mmu.init_acc(h16);   
}
