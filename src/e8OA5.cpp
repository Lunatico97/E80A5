#include <iostream>
#include <cpu.hpp>

int main(int argc, char *argv[])
{
    CPU *cpu = nullptr;
    cpu = new CPU();
    delete cpu;
    return 0;
}