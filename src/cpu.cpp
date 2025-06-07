#include <cpu.hpp>

CPU::CPU()
{
    mmu.lda(0x10);
    mmu.sta(0x1532);
    mmu.inc();
    mmu.inc();
    mmu.inc();
    mmu.sta(0x1533);
    mmu.lhld(0x1532);
    Utils::logU8(mmu.tapU8(H));
    Utils::logU8(mmu.tapU8(L));
    mmu.shld(0x0600);
    mmu.ldax(0x0601);
    Utils::logU8(mmu.tapU8(A));
    Utils::logU16(mmu.tapU16(A));
}
