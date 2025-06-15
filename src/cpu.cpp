#include <cpu.hpp>

CPU::CPU()
{
    alu = ALU(alu);
}

void CPU::create_machine_code(const std::string& filename)
{
    IREG = asmb.assemble(mmu, filename);
}

void CPU::load_program()
{
    HEX current;
    // Tap origin
    Utils::logU16("ORG", IREG);
    std::cout << "\n";

    while(1)
    {
        u8 l = IL_MAP[mmu.fetch_mem(IREG)]-1;
        mmu.init_pc(IREG+l);
        for(int i=0; i<=l; i++)
        {
            current.h8[i] = mmu.fetch_mem(IREG+i); 
        }
        decode(current);
        
        // Tap registers stepwise
        Utils::logU8("A", mmu.tapU8(A));
        Utils::logU8("F", mmu.tapU8(F));
        Utils::logU8("B", mmu.tapU8(B));
        Utils::logU8("C", mmu.tapU8(C));
        Utils::logU8("D", mmu.tapU8(D));
        Utils::logU8("E", mmu.tapU8(E));
        Utils::logU8("H", mmu.tapU8(H));
        Utils::logU8("L", mmu.tapU8(L));
        Utils::logHEX(current);

        IREG = mmu.load_pc();
    }    
}

void CPU::decode(const HEX& hex)
{
    u16 h16 = (static_cast<u16>(hex.h8[1]) | (static_cast<u16>(hex.h8[2]) << 8));
    switch(hex.h8[0])
    {
        // Transfers
        case 0x00: break; // NOP
        case 0x76: exit(0); break; // HLT
        case 0x3A: mmu.lda(h16); break;
        case 0x32: mmu.sta(h16); break;
        case 0x0A: mmu.ldax(B); break;
        case 0x1A: mmu.ldax(D); break;
        case 0x02: mmu.stax(B); break;
        case 0x12: mmu.stax(D); break;
        case 0x01: mmu.lxi(B, h16); break;
        case 0x11: mmu.lxi(D, h16); break;
        case 0x21: mmu.lxi(H, h16); break;
        case 0x31: mmu.lxi(S, h16); break;
        case 0x2A: mmu.lhld(h16); break;
        case 0x22: mmu.shld(h16); break;
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
        
        // Comparators
        case 0xB8: alu.cmp(B); break;
        case 0xB9: alu.cmp(C); break;
        case 0xBA: alu.cmp(D); break;
        case 0xBB: alu.cmp(E); break; 
        case 0xBC: alu.cmp(H); break;
        case 0xBD: alu.cmp(L); break;
        // case 0xBE: alu.cmp(M); break;
        case 0xBF: alu.cmp(A); break;

        // Logic
        case 0xE6: alu.ani(hex.h8[1]); break;
        case 0xEE: alu.xri(hex.h8[1]); break;
        case 0xF6: alu.ori(hex.h8[1]); break;
        case 0xFE: alu.cpi(hex.h8[1]); break;

        // Branching
        case 0xC3: jmp(h16); break; 
        case 0xCA: jz(h16); break; 
        case 0xC2: jnz(h16); break; 
        case 0xDA: jc(h16); break; 
        case 0xD2: jnc(h16); break; 

        default: break;
    }
}

void CPU::jmp(u16 address)
{
    mmu.init_pc(address);
}

void CPU::jc(u16 address)
{
    if(mmu.tapU8(F) & HX_CARY != HX_CARY) mmu.init_pc(address);
    else return;
}

void CPU::jnc(u16 address)
{
    if(mmu.tapU8(F) & HX_CARY != HX_CARY) mmu.init_pc(address);
    else return;
}

void CPU::jz(u16 address)
{
    if(mmu.tapU8(F) & HX_ZERO == HX_ZERO) mmu.init_pc(address);
    else return;
}

void CPU::jnz(u16 address)
{
    if(mmu.tapU8(F) & HX_ZERO != HX_ZERO) mmu.init_pc(address);
    else return;
}
