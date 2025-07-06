#include <alu.hpp>
#include <mmu.hpp>
#include <assembler.hpp>
#include <gui.hpp>

#ifndef _CPU_H_
#define _CPU_H_

class CPU
{
    public:
        CPU();
        ~CPU();
        void create_gui();
        void create_machine_code(const char* filename);
        void load_program();

    private:
        void decode(const HEX& hex);

        // Branching (Jumps)
        void jmp(u16 address);
        void jc(u16 address);
        void jz(u16 address);
        void jnc(u16 address);
        void jnz(u16 address);

        // Branching (Calls)
        void call(u16 address);

        // Branching (Returns)
        void ret();

        Assembler asmb;
        Renderer* rndr;
        MMU mmu;
        ALU alu;
        GUI gui;
        u16 IREG;
        bool _halt;
};

#endif