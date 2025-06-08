#include <iostream>
#include <cpu.hpp>

int main(int argc, char *argv[])
{
    CPU *cpu = nullptr;
    cpu = new CPU();
    cpu->create_machine_code("bin/example.e8");
    cpu->load_program();
    delete cpu;
    return 0;
}