#include <assembler.hpp>

Assembler::Assembler()
{
    loadOpcodeLookup();
}

OPCODE Assembler::parseMnemonicToOpcode(const std::string& hexstr)
{
    if(opcode_map.find(hexstr) != opcode_map.end()) return opcode_map[hexstr];
    return {0xFF, 0};
}

u16 Assembler::parseHexStrToVal(const std::string& hexstr)
{
    std::string s = hexstr;
    if (s.back() == 'H' || s.back() == 'h') s.pop_back();
    std::stringstream ss;
    ss << std::hex << s;
    u16 value;
    ss >> value;
    if (ss.fail() || !ss.eof()) throw std::runtime_error("Invalid hex: " + hexstr);
    return value;
}

HEX Assembler::assemble(const std::string& instruction) {
    const char* mnemonic = instruction.c_str();
    OPCODE opcode = parseMnemonicToOpcode(mnemonic);
    std::string operand_part;

    // Parse mnemonics with operands
    if(opcode.bytes != 1)
    {
        std::size_t pos = instruction.find(',');
        if(pos != -1)
        {
            mnemonic = instruction.substr(0, pos).c_str();
            operand_part = instruction.substr(pos+1, instruction.length());
            opcode = parseMnemonicToOpcode(mnemonic);
            if(opcode.value == 0xFF) throw(std::runtime_error("Invalid mnemonic: " + instruction));
        }
        else
        {
            pos = instruction.find(' ');
            if(pos != -1)
            {
                mnemonic = instruction.substr(0, pos).c_str();
                operand_part = instruction.substr(pos+1, instruction.length());
                opcode = parseMnemonicToOpcode(mnemonic);
                if(opcode.value == 0xFF) throw(std::runtime_error("Invalid mnemonic: " + instruction));
            }
        }

        // Parse operands
        if (!operand_part.empty()) 
        {
            u16 operand = static_cast<u16>(parseHexStrToVal(operand_part));
            return {opcode.value, static_cast<u8>(operand & 0xFF), operand};
        }
        else throw std::runtime_error("Operands required: " + instruction);
    }

    return {opcode.value, 0x00, 0x0000}; 
}

void Assembler::loadOpcodeLookup() 
{
    // Data Transfer Instructions
    opcode_map["LDA"] = {0x3A, 3}; // LDA Address
    opcode_map["STA"] = {0x32, 3}; // STA Address
    opcode_map["LHLD"] = {0x2A, 3}; // LHLD Address
    opcode_map["SHLD"] = {0x22, 3}; // SHLD Address

    opcode_map["LXI B"] = {0x01, 3}; // LXI B, data16
    opcode_map["LXI D"] = {0x11, 3}; // LXI D, data16
    opcode_map["LXI H"] = {0x21, 3}; // LXI H, data16
    opcode_map["LXI SP"] = {0x31, 3}; // LXI SP, data16

    opcode_map["MVI B"] = {0x06, 2}; // MVI B, data8
    opcode_map["MVI C"] = {0x0E, 2}; // MVI C, data8
    opcode_map["MVI D"] = {0x16, 2}; // MVI D, data8
    opcode_map["MVI E"] = {0x1E, 2}; // MVI E, data8
    opcode_map["MVI H"] = {0x26, 2}; // MVI H, data8
    opcode_map["MVI L"] = {0x2E, 2}; // MVI L, data8
    opcode_map["MVI M"] = {0x36, 2}; // MVI M, data8 (memory at HL)
    opcode_map["MVI A"] = {0x3E, 2}; // MVI A, data8

    opcode_map["LDAX B"] = {0x0A, 1}; // LDAX B (load A from (BC))
    opcode_map["LDAX D"] = {0x1A, 1}; // LDAX D (load A from (DE))
    opcode_map["STAX B"] = {0x02, 1}; // STAX B (store A to (BC))
    opcode_map["STAX D"] = {0x12, 1}; // STAX D (store A to (DE))

    // MOV instructions (MOV Dst, Src)
    opcode_map["MOV B,A"] = {0x47, 1}; opcode_map["MOV B,B"] = {0x40, 1}; opcode_map["MOV B,C"] = {0x41, 1};
    opcode_map["MOV B,D"] = {0x42, 1}; opcode_map["MOV B,E"] = {0x43, 1}; opcode_map["MOV B,H"] = {0x44, 1};
    opcode_map["MOV B,L"] = {0x45, 1}; opcode_map["MOV B,M"] = {0x46, 1};

    opcode_map["MOV C,A"] = {0x4F, 1}; opcode_map["MOV C,B"] = {0x48, 1}; opcode_map["MOV C,C"] = {0x49, 1};
    opcode_map["MOV C,D"] = {0x4A, 1}; opcode_map["MOV C,E"] = {0x4B, 1}; opcode_map["MOV C,H"] = {0x4C, 1};
    opcode_map["MOV C,L"] = {0x4D, 1}; opcode_map["MOV C,M"] = {0x4E, 1};

    opcode_map["MOV D,A"] = {0x57, 1}; opcode_map["MOV D,B"] = {0x50, 1}; opcode_map["MOV D,C"] = {0x51, 1};
    opcode_map["MOV D,D"] = {0x52, 1}; opcode_map["MOV D,E"] = {0x53, 1}; opcode_map["MOV D,H"] = {0x54, 1};
    opcode_map["MOV D,L"] = {0x55, 1}; opcode_map["MOV D,M"] = {0x56, 1};

    opcode_map["MOV E,A"] = {0x5F, 1}; opcode_map["MOV E,B"] = {0x58, 1}; opcode_map["MOV E,C"] = {0x59, 1};
    opcode_map["MOV E,D"] = {0x5A, 1}; opcode_map["MOV E,E"] = {0x5B, 1}; opcode_map["MOV E,H"] = {0x5C, 1};
    opcode_map["MOV E,L"] = {0x5D, 1}; opcode_map["MOV E,M"] = {0x5E, 1};

    opcode_map["MOV H,A"] = {0x67, 1}; opcode_map["MOV H,B"] = {0x60, 1}; opcode_map["MOV H,C"] = {0x61, 1};
    opcode_map["MOV H,D"] = {0x62, 1}; opcode_map["MOV H,E"] = {0x63, 1}; opcode_map["MOV H,H"] = {0x64, 1};
    opcode_map["MOV H,L"] = {0x65, 1}; opcode_map["MOV H,M"] = {0x66, 1};

    opcode_map["MOV L,A"] = {0x6F, 1}; opcode_map["MOV L,B"] = {0x68, 1}; opcode_map["MOV L,C"] = {0x69, 1};
    opcode_map["MOV L,D"] = {0x6A, 1}; opcode_map["MOV L,E"] = {0x6B, 1}; opcode_map["MOV L,H"] = {0x6C, 1};
    opcode_map["MOV L,L"] = {0x6D, 1}; opcode_map["MOV L,M"] = {0x6E, 1};

    opcode_map["MOV M,A"] = {0x77, 1}; opcode_map["MOV M,B"] = {0x70, 1}; opcode_map["MOV M,C"] = {0x71, 1};
    opcode_map["MOV M,D"] = {0x72, 1}; opcode_map["MOV M,E"] = {0x73, 1}; opcode_map["MOV M,H"] = {0x74, 1};
    opcode_map["MOV M,L"] = {0x75, 1};

    opcode_map["MOV A,A"] = {0x7F, 1}; opcode_map["MOV A,B"] = {0x78, 1}; opcode_map["MOV A,C"] = {0x79, 1};
    opcode_map["MOV A,D"] = {0x7A, 1}; opcode_map["MOV A,E"] = {0x7B, 1}; opcode_map["MOV A,H"] = {0x7C, 1};
    opcode_map["MOV A,L"] = {0x7D, 1}; opcode_map["MOV A,M"] = {0x7E, 1};

    // Arithmetic Instructions
    opcode_map["ADD A"] = {0x87, 1}; opcode_map["ADD B"] = {0x80, 1}; opcode_map["ADD C"] = {0x81, 1};
    opcode_map["ADD D"] = {0x82, 1}; opcode_map["ADD E"] = {0x83, 1}; opcode_map["ADD H"] = {0x84, 1};
    opcode_map["ADD L"] = {0x85, 1}; opcode_map["ADD M"] = {0x86, 1};

    opcode_map["ADC A"] = {0x8F, 1}; opcode_map["ADC B"] = {0x88, 1}; opcode_map["ADC C"] = {0x89, 1};
    opcode_map["ADC D"] = {0x8A, 1}; opcode_map["ADC E"] = {0x8B, 1}; opcode_map["ADC H"] = {0x8C, 1};
    opcode_map["ADC L"] = {0x8D, 1}; opcode_map["ADC M"] = {0x8E, 1};

    opcode_map["SUB A"] = {0x97, 1}; opcode_map["SUB B"] = {0x90, 1}; opcode_map["SUB C"] = {0x91, 1};
    opcode_map["SUB D"] = {0x92, 1}; opcode_map["SUB E"] = {0x93, 1}; opcode_map["SUB H"] = {0x94, 1};
    opcode_map["SUB L"] = {0x95, 1}; opcode_map["SUB M"] = {0x96, 1};

    opcode_map["SBB A"] = {0x9F, 1}; opcode_map["SBB B"] = {0x98, 1}; opcode_map["SBB C"] = {0x99, 1};
    opcode_map["SBB D"] = {0x9A, 1}; opcode_map["SBB E"] = {0x9B, 1}; opcode_map["SBB H"] = {0x9C, 1};
    opcode_map["SBB L"] = {0x9D, 1}; opcode_map["SBB M"] = {0x9E, 1};

    opcode_map["INR A"] = {0x3C, 1}; opcode_map["INR B"] = {0x04, 1}; opcode_map["INR C"] = {0x0C, 1};
    opcode_map["INR D"] = {0x14, 1}; opcode_map["INR E"] = {0x1C, 1}; opcode_map["INR H"] = {0x24, 1};
    opcode_map["INR L"] = {0x2C, 1}; opcode_map["INR M"] = {0x34, 1};

    opcode_map["DCR A"] = {0x3D, 1}; opcode_map["DCR B"] = {0x05, 1}; opcode_map["DCR C"] = {0x0D, 1};
    opcode_map["DCR D"] = {0x15, 1}; opcode_map["DCR E"] = {0x1D, 1}; opcode_map["DCR H"] = {0x25, 1};
    opcode_map["DCR L"] = {0x2D, 1}; opcode_map["DCR M"] = {0x35, 1};

    opcode_map["INX B"] = {0x03, 1}; opcode_map["INX D"] = {0x13, 1}; opcode_map["INX H"] = {0x23, 1};
    opcode_map["INX SP"] = {0x33, 1};

    opcode_map["DCX B"] = {0x0B, 1}; opcode_map["DCX D"] = {0x1B, 1}; opcode_map["DCX H"] = {0x2B, 1};
    opcode_map["DCX SP"] = {0x3B, 1};

    opcode_map["DAD B"] = {0x09, 1}; opcode_map["DAD D"] = {0x19, 1}; opcode_map["DAD H"] = {0x29, 1};
    opcode_map["DAD SP"] = {0x39, 1};

    opcode_map["ADI"] = {0xC6, 2}; // ADI data8
    opcode_map["ACI"] = {0xCE, 2}; // ACI data8
    opcode_map["SUI"] = {0xD6, 2}; // SUI data8
    opcode_map["SBI"] = {0xDE, 2}; // SBI data8

    // Logical Instructions
    opcode_map["ANA A"] = {0xA7, 1}; opcode_map["ANA B"] = {0xA0, 1}; opcode_map["ANA C"] = {0xA1, 1};
    opcode_map["ANA D"] = {0xA2, 1}; opcode_map["ANA E"] = {0xA3, 1}; opcode_map["ANA H"] = {0xA4, 1};
    opcode_map["ANA L"] = {0xA5, 1}; opcode_map["ANA M"] = {0xA6, 1};

    opcode_map["XRA A"] = {0xAF, 1}; opcode_map["XRA B"] = {0xA8, 1}; opcode_map["XRA C"] = {0xA9, 1};
    opcode_map["XRA D"] = {0xAA, 1}; opcode_map["XRA E"] = {0xAB, 1}; opcode_map["XRA H"] = {0xAC, 1};
    opcode_map["XRA L"] = {0xAD, 1}; opcode_map["XRA M"] = {0xAE, 1};

    opcode_map["ORA A"] = {0xB7, 1}; opcode_map["ORA B"] = {0xB0, 1}; opcode_map["ORA C"] = {0xB1, 1};
    opcode_map["ORA D"] = {0xB2, 1}; opcode_map["ORA E"] = {0xB3, 1}; opcode_map["ORA H"] = {0xB4, 1};
    opcode_map["ORA L"] = {0xB5, 1}; opcode_map["ORA M"] = {0xB6, 1};

    opcode_map["CMP A"] = {0xBF, 1}; opcode_map["CMP B"] = {0xB8, 1}; opcode_map["CMP C"] = {0xB9, 1};
    opcode_map["CMP D"] = {0xBA, 1}; opcode_map["CMP E"] = {0xBB, 1}; opcode_map["CMP H"] = {0xBC, 1};
    opcode_map["CMP L"] = {0xBD, 1}; opcode_map["CMP M"] = {0xBE, 1};

    opcode_map["ANI"] = {0xE6, 2}; // ANI data8
    opcode_map["XRI"] = {0xEE, 2}; // XRI data8
    opcode_map["ORI"] = {0xF6, 2}; // ORI data8
    opcode_map["CPI"] = {0xFE, 2}; // CPI data8

    opcode_map["RLC"] = {0x07, 1}; opcode_map["RRC"] = {0x0F, 1}; opcode_map["RAL"] = {0x17, 1}; opcode_map["RAR"] = {0x1F, 1};

    opcode_map["CMA"] = {0x2F, 1}; opcode_map["CMC"] = {0x3F, 1}; opcode_map["STC"] = {0x37, 1}; opcode_map["DAA"] = {0x27, 1};

    // Branching Instructions
    opcode_map["JMP"] = {0xC3, 3}; // JMP address
    opcode_map["JC"] = {0xDA, 3};  // JC address
    opcode_map["JNC"] = {0xD2, 3}; // JNC address
    opcode_map["JZ"] = {0xCA, 3};  // JZ address
    opcode_map["JNZ"] = {0xC2, 3}; // JNZ address
    opcode_map["JP"] = {0xF2, 3};  // JP address
    opcode_map["JM"] = {0xFA, 3};  // JM address
    opcode_map["JPE"] = {0xEA, 3}; // JPE address
    opcode_map["JPO"] = {0xE2, 3}; // JPO address

    opcode_map["CALL"] = {0xCD, 3}; // CALL address
    opcode_map["CC"] = {0xDC, 3};   // CC address
    opcode_map["CNC"] = {0xD4, 3};  // CNC address
    opcode_map["CZ"] = {0xCC, 3};   // CZ address
    opcode_map["CNZ"] = {0xC4, 3};  // CNZ address
    opcode_map["CP"] = {0xF4, 3};   // CP address
    opcode_map["CM"] = {0xFC, 3};   // CM address
    opcode_map["CPE"] = {0xEC, 3};  // CPE address
    opcode_map["CPO"] = {0xE4, 3};  // CPO address

    opcode_map["RET"] = {0xC9, 1}; // RET
    opcode_map["RC"] = {0xD8, 1};  // RC
    opcode_map["RNC"] = {0xD0, 1}; // RNC
    opcode_map["RZ"] = {0xC8, 1};  // RZ
    opcode_map["RNZ"] = {0xC0, 1}; // RNZ
    opcode_map["RP"] = {0xF0, 1};  // RP
    opcode_map["RM"] = {0xF8, 1};  // RM
    opcode_map["RPE"] = {0xE8, 1}; // RPE
    opcode_map["RPO"] = {0xE0, 1}; // RPO

    // Stack Operations
    opcode_map["PUSH B"] = {0xC5, 1}; opcode_map["PUSH D"] = {0xD5, 1}; opcode_map["PUSH H"] = {0xE5, 1};
    opcode_map["PUSH PSW"] = {0xF5, 1};

    opcode_map["POP B"] = {0xC1, 1}; opcode_map["POP D"] = {0xD1, 1}; opcode_map["POP H"] = {0xE1, 1};
    opcode_map["POP PSW"] = {0xF1, 1};

    // I/O and Interrupts
    opcode_map["IN"] = {0xDB, 2}; // IN port8
    opcode_map["OUT"] = {0xD3, 2}; // OUT port8
    opcode_map["EI"] = {0xFB, 1}; opcode_map["DI"] = {0xF3, 1};
    opcode_map["RIM"] = {0x20, 1}; opcode_map["SIM"] = {0x30, 1};
    opcode_map["RST 0"] = {0xC7, 1}; opcode_map["RST 1"] = {0xCF, 1}; opcode_map["RST 2"] = {0xD7, 1};
    opcode_map["RST 3"] = {0xDF, 1}; opcode_map["RST 4"] = {0xE7, 1}; opcode_map["RST 5"] = {0xEF, 1};
    opcode_map["RST 6"] = {0xF7, 1}; opcode_map["RST 7"] = {0xFF, 1};

    // Special
    opcode_map["NOP"] = {0x00, 1};
    opcode_map["HLT"] = {0x76, 1};
    opcode_map["XCHG"] = {0xEB, 1};
    opcode_map["XTHL"] = {0xE3, 1}; 
    opcode_map["SPHL"] = {0xF9, 1};
    opcode_map["PCHL"] = {0xE9, 1}; // PCHL (Jump to HL)
}
