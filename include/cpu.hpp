#include <mmu.hpp>

#ifndef _CPU_H_
#define _CPU_H_

class CPU
{
    public:
        CPU();

    private:
        MMU mmu;
};

#endif