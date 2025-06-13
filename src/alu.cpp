#include <alu.hpp>

ALU::ALU() {}

void ALU::fetch(Registers& rb)
{
    ACC = rb.fetchU8(AF);
    SF = rb.fetchU8(AF, true);
}

void ALU::fetch(Registers& rb, REG r)
{
    TEMP = rb.fetchU8(rb.mapToOffset(r), r%2);  
} 

void ALU::load(Registers& rb)
{
    u16 h16 = (static_cast<u16>(SF) | (static_cast<u16>(ACC) << 8));
    rb.loadU16(AF, h16);
}
