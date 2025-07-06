#include <iostream>
#include <cpu.hpp>

/*
    Title: E80A5
    Author: Diwas Adhikari
    Email: lunaticoda123@gmail.com
    Description: My take on emulating Intel 8085 !
    [ The project also contains a two pass 8085 assembler and GUI interface ]
*/

int main(int argc, char *argv[])
{
    CPU *cpu = nullptr;

    try
    {
        cpu = new CPU();
        cpu->create_machine_code("bin/counter3.e8");
        cpu->load_program();
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
    }
    
    delete cpu;
    return 0;
}