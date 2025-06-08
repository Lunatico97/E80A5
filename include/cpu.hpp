#include <mmu.hpp>
#include <assembler.hpp>

#ifndef _CPU_H_
#define _CPU_H_

class CPU
{
    public:
        CPU();
        void run_instruction(const std::string& instruction);

    private:
        void decode(const HEX& hex);

        Assembler asmb;
        MMU mmu;
        u8* IREG;
};

#endif