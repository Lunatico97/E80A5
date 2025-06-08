#include <iostream>
#include <cpu.hpp>

int main(int argc, char *argv[])
{
    CPU *cpu = nullptr;
    cpu = new CPU();
    cpu->run_instruction("LDA 1AH");
    cpu->run_instruction("STA 1532H");
    cpu->run_instruction("INR A");
    cpu->run_instruction("INR A");
    cpu->run_instruction("INR A");
    cpu->run_instruction("STA 1533H");
    cpu->run_instruction("LHLD 1532H");
    delete cpu;
    return 0;
}