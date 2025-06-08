#include <iostream>
#include <iomanip>
#include <types.hpp>

#ifndef _UTILS_H_
#define _UTILS_H_

namespace Utils
{
    static void logU8(u8 value)
    {
        std::cout << std::uppercase << std::hex << std::setfill('0') << std::setw(2) << (value & 0xFF) << " ";
    }

    static void logU16(u16 value)
    {
        std::cout << std::uppercase << std::hex << std::setfill('0') << std::setw(4) << (value & 0xFFFF) << " ";
    }
};

#endif