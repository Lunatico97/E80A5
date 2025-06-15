#include <iostream>
#include <cpu.hpp>

int main(int argc, char *argv[])
{
    CPU *cpu = nullptr;

    try
    {
        cpu = new CPU();
        cpu->create_machine_code("bin/counter2.e8");
        cpu->load_program();
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
    }
    
    delete cpu;
    return 0;
}