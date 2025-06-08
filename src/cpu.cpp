#include <cpu.hpp>

CPU::CPU(){}

void CPU::run_instruction(const std::string& instruction)
{
    try
    {
        HEX output = asmb.assemble(instruction);
        decode(output);
        Utils::logU8("A", mmu.tapU8(A));
        Utils::logU8("H", mmu.tapU8(H));
        Utils::logU8("L", mmu.tapU8(L));
        Utils::logU16("PC", mmu.tapU16(PC));
        Utils::logHEX(output);
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
    }
}

void CPU::decode(const HEX& hex)
{
    switch(hex.h8[0])
    {
        // Transfers
        case 0x00: break; // NOP
        case 0x76: break; // HLT
        case 0x3A: mmu.lda(hex.h8[1]); break;
        case 0x32: mmu.sta(hex.h16); break;
        case 0x0A: mmu.ldax(B, hex.h16); break;
        case 0x1A: mmu.ldax(D, hex.h16); break;
        case 0x02: mmu.stax(B); break;
        case 0x12: mmu.stax(D); break;
        case 0x01: mmu.lxi(B, hex.h16); break;
        case 0x11: mmu.lxi(D, hex.h16); break;
        case 0x21: mmu.lxi(H, hex.h16); break;
        case 0x31: mmu.lxi(S, hex.h16); break;
        case 0x2A: mmu.lhld(hex.h16); break;
        case 0x22: mmu.shld(hex.h16); break;
        case 0xEB: mmu.xchg(); break;
        case 0xF9: mmu.sphl(); break;
        case 0xE3: mmu.xthl(); break;
        case 0x06: mmu.mvi(B, hex.h8[1]); break;
        case 0x0E: mmu.mvi(C, hex.h8[1]); break;
        case 0x16: mmu.mvi(D, hex.h8[1]); break;
        case 0x1E: mmu.mvi(E, hex.h8[1]); break;
        case 0x26: mmu.mvi(H, hex.h8[1]); break;
        case 0x2E: mmu.mvi(L, hex.h8[1]); break;
        //case 0x36: mmu.mvi(M, hex.h8[1]); break;
        case 0x3E: mmu.mvi(A, hex.h8[1]); break;

        // Incrementors / Decrementors
        case 0x04: mmu.inc(B); break;
        case 0x0C: mmu.inc(C); break;
        case 0x14: mmu.inc(D); break;
        case 0x1C: mmu.inc(E); break;
        case 0x24: mmu.inc(H); break;
        case 0x2C: mmu.inc(L); break;
        // case 0x34: mmu.inc(M); break;
        case 0x3C: mmu.inc(A); break;
        case 0x05: mmu.dec(B); break;
        case 0x0D: mmu.dec(C); break;
        case 0x15: mmu.dec(D); break;
        case 0x1D: mmu.dec(E); break;
        case 0x25: mmu.dec(H); break;
        case 0x2D: mmu.dec(L); break;
        // case 0x35: mmu.dec(M); break;
        case 0x3D: mmu.dec(A); break;
        case 0x03: mmu.inx(BC); break;
        case 0x13: mmu.inx(DE); break;
        case 0x23: mmu.inx(HL); break;
        case 0x33: mmu.inx(SP); break;
        case 0x0B: mmu.dcx(BC); break;
        case 0x1B: mmu.dcx(DE); break;
        case 0x2B: mmu.dcx(HL); break;
        case 0x3B: mmu.dcx(SP); break;

        default: break;
    }
}
