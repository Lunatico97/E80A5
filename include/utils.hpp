#include <iostream>
#include <iomanip>
#include <string>
#include <sstream>
#include <stdexcept>
#include <map>
#include <types.hpp>

#ifndef _UTILS_H_
#define _UTILS_H_

namespace Utils
{
    static void logU8(const char* r, u8 value)
    {
        std::cout << std::uppercase << r << "[" << std::hex << std::setfill('0') << std::setw(2) << (value & 0xFF) << "] ";
    }

    static void logU16(const char* r, u16 value)
    {
        std::cout << std::uppercase << r << "[" << std::hex << std::setfill('0') << std::setw(4) << (value & 0xFFFF) << "] ";
    }

    static void logHEX(HEX value)
    {
        std::cout << "< ";
        logU8("OP", value.h8[0]);
        logU8("H8", value.h8[1]);
        logU16("H16", value.h16);
        std::cout << ">" << std::endl;
    }
};

#endif