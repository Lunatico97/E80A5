#include <mmu.hpp>
#include <assembler.hpp>

#ifndef _CPU_H_
#define _CPU_H_

class CPU
{
    public:
        CPU();
        void create_machine_code(const std::string& filename);
        void load_program();

    private:
        void decode(const HEX& hex);

        Assembler asmb;
        MMU mmu;
        u16 IREG;
};

#endif