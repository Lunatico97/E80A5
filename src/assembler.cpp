#include <assembler.hpp>

Assembler::Assembler()
{
    loadOpcodeLookup();
}

u16 Assembler::assemble(MMU& mmu, const std::string& filename)
{
    u16 origin = 0x0000;
    u16 counter = 0x0000;
    std::stringstream ss(Utils::readTextFromFile(filename.c_str()));
    std::string line;

    while (std::getline(ss, line)) 
    { 
        size_t commentPos = line.find(';');
        if (commentPos != std::string::npos) {
            line = line.substr(0, commentPos);
        }

        line = Utils::trim(line);
        if (!line.empty()) 
        {
            if (counter == 0 && line.rfind("ORG ", 0) == 0) 
            { 
                origin = parseHexStrToVal(line.substr(4));
                counter = origin; 
            }
            else
            {
                std::string label;
                size_t pos = line.find(':');
                if(pos != std::string::npos)
                {
                    label = line.substr(0, pos);
                    if(label.find(' ') != std::string::npos) 
                        throw(std::runtime_error("No whitespaces allowed in label !"));
                    else
                    {
                        label_map[label] = counter-1;
                        line.replace(0, pos+1, "");
                    }
                }
                Utils::logU16("CT", counter);
                HEX hex = parseToMachineCode(line);
                for(int i=0; i<IL_MAP[hex.h8[0]]; i++)
                {
                    mmu.load_mem(counter++, hex.h8[i]);
                }
                Utils::logHEX(hex);
            }            
        }
    }

    return origin;
}

OPCODE Assembler::parseMnemonicToOpcode(const std::string& hexstr)
{
    if(opcode_map.find(hexstr) != opcode_map.end())
    {
        u8 opcode = opcode_map[hexstr];
        return {opcode, IL_MAP[opcode]};
    }
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

HEX Assembler::parseToMachineCode(const std::string& instruction) {
    const char* mnemonic = instruction.c_str();
    OPCODE opcode = parseMnemonicToOpcode(mnemonic);
    std::string operand_part;

    // Parse mnemonics with operands
    if(opcode.bytes != 1)
    {
        std::size_t pos = instruction.find(',');
        if(pos != std::string::npos)
        {
            mnemonic = instruction.substr(0, pos).c_str();
            operand_part = instruction.substr(pos+1, instruction.length());
            opcode = parseMnemonicToOpcode(mnemonic);
            if(opcode.value == 0xFF) throw(std::runtime_error("Invalid mnemonic: " + instruction));
        }
        else
        {
            pos = instruction.find(' ');
            if(pos != std::string::npos)
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
            u16 operand = 0x0000;
            if(label_map.find(operand_part) != label_map.end()) operand = label_map[operand_part];
            else operand = parseHexStrToVal(operand_part);
            if(opcode.bytes == 2) return {opcode.value, static_cast<u8>(operand & 0xFF), 0x00};
            return {opcode.value, static_cast<u8>(operand & 0xFF), static_cast<u8>((operand >> 8) & 0xFF)};
        }
        else throw std::runtime_error("Operands required: " + instruction);
    }

    return {opcode.value, 0x00, 0x00}; 
}

void Assembler::loadOpcodeLookup() 
{
    // Data Transfer Instructions
    opcode_map["LDA"] = 0x3A; // LDA Address
    opcode_map["STA"] = 0x32; // STA Address
    opcode_map["LHLD"] = 0x2A; // LHLD Address
    opcode_map["SHLD"] = 0x22; // SHLD Address

    opcode_map["LXI B"] = 0x01; // LXI B, data16
    opcode_map["LXI D"] = 0x11; // LXI D, data16
    opcode_map["LXI H"] = 0x21; // LXI H, data16
    opcode_map["LXI SP"] = 0x31; // LXI SP, data16

    opcode_map["MVI B"] = 0x06; // MVI B, data8
    opcode_map["MVI C"] = 0x0E; // MVI C, data8
    opcode_map["MVI D"] = 0x16; // MVI D, data8
    opcode_map["MVI E"] = 0x1E; // MVI E, data8
    opcode_map["MVI H"] = 0x26; // MVI H, data8
    opcode_map["MVI L"] = 0x2E; // MVI L, data8
    opcode_map["MVI M"] = 0x36; // MVI M, data8 (memory at HL)
    opcode_map["MVI A"] = 0x3E; // MVI A, data8

    opcode_map["LDAX B"] = 0x0A; // LDAX B (load A from (BC))
    opcode_map["LDAX D"] = 0x1A; // LDAX D (load A from (DE))
    opcode_map["STAX B"] = 0x02; // STAX B (store A to (BC))
    opcode_map["STAX D"] = 0x12; // STAX D (store A to (DE))

    // MOV instructions (MOV Dst, Src)
    opcode_map["MOV B,A"] = 0x47; opcode_map["MOV B,B"] = 0x40; opcode_map["MOV B,C"] = 0x41;
    opcode_map["MOV B,D"] = 0x42; opcode_map["MOV B,E"] = 0x43; opcode_map["MOV B,H"] = 0x44;
    opcode_map["MOV B,L"] = 0x45; opcode_map["MOV B,M"] = 0x46;

    opcode_map["MOV C,A"] = 0x4F; opcode_map["MOV C,B"] = 0x48; opcode_map["MOV C,C"] = 0x49;
    opcode_map["MOV C,D"] = 0x4A; opcode_map["MOV C,E"] = 0x4B; opcode_map["MOV C,H"] = 0x4C;
    opcode_map["MOV C,L"] = 0x4D; opcode_map["MOV C,M"] = 0x4E;

    opcode_map["MOV D,A"] = 0x57; opcode_map["MOV D,B"] = 0x50; opcode_map["MOV D,C"] = 0x51;
    opcode_map["MOV D,D"] = 0x52; opcode_map["MOV D,E"] = 0x53; opcode_map["MOV D,H"] = 0x54;
    opcode_map["MOV D,L"] = 0x55; opcode_map["MOV D,M"] = 0x56;

    opcode_map["MOV E,A"] = 0x5F; opcode_map["MOV E,B"] = 0x58; opcode_map["MOV E,C"] = 0x59;
    opcode_map["MOV E,D"] = 0x5A; opcode_map["MOV E,E"] = 0x5B; opcode_map["MOV E,H"] = 0x5C;
    opcode_map["MOV E,L"] = 0x5D; opcode_map["MOV E,M"] = 0x5E;

    opcode_map["MOV H,A"] = 0x67; opcode_map["MOV H,B"] = 0x60; opcode_map["MOV H,C"] = 0x61;
    opcode_map["MOV H,D"] = 0x62; opcode_map["MOV H,E"] = 0x63; opcode_map["MOV H,H"] = 0x64;
    opcode_map["MOV H,L"] = 0x65; opcode_map["MOV H,M"] = 0x66;

    opcode_map["MOV L,A"] = 0x6F; opcode_map["MOV L,B"] = 0x68; opcode_map["MOV L,C"] = 0x69;
    opcode_map["MOV L,D"] = 0x6A; opcode_map["MOV L,E"] = 0x6B; opcode_map["MOV L,H"] = 0x6C;
    opcode_map["MOV L,L"] = 0x6D; opcode_map["MOV L,M"] = 0x6E;

    opcode_map["MOV M,A"] = 0x77; opcode_map["MOV M,B"] = 0x70; opcode_map["MOV M,C"] = 0x71;
    opcode_map["MOV M,D"] = 0x72; opcode_map["MOV M,E"] = 0x73; opcode_map["MOV M,H"] = 0x74;
    opcode_map["MOV M,L"] = 0x75;

    opcode_map["MOV A,A"] = 0x7F; opcode_map["MOV A,B"] = 0x78; opcode_map["MOV A,C"] = 0x79;
    opcode_map["MOV A,D"] = 0x7A; opcode_map["MOV A,E"] = 0x7B; opcode_map["MOV A,H"] = 0x7C;
    opcode_map["MOV A,L"] = 0x7D; opcode_map["MOV A,M"] = 0x7E;

    // Arithmetic Instructions
    opcode_map["ADD A"] = 0x87; opcode_map["ADD B"] = 0x80; opcode_map["ADD C"] = 0x81;
    opcode_map["ADD D"] = 0x82; opcode_map["ADD E"] = 0x83; opcode_map["ADD H"] = 0x84;
    opcode_map["ADD L"] = 0x85; opcode_map["ADD M"] = 0x86;

    opcode_map["ADC A"] = 0x8F; opcode_map["ADC B"] = 0x88; opcode_map["ADC C"] = 0x89;
    opcode_map["ADC D"] = 0x8A; opcode_map["ADC E"] = 0x8B; opcode_map["ADC H"] = 0x8C;
    opcode_map["ADC L"] = 0x8D; opcode_map["ADC M"] = 0x8E;

    opcode_map["SUB A"] = 0x97; opcode_map["SUB B"] = 0x90; opcode_map["SUB C"] = 0x91;
    opcode_map["SUB D"] = 0x92; opcode_map["SUB E"] = 0x93; opcode_map["SUB H"] = 0x94;
    opcode_map["SUB L"] = 0x95; opcode_map["SUB M"] = 0x96;

    opcode_map["SBB A"] = 0x9F; opcode_map["SBB B"] = 0x98; opcode_map["SBB C"] = 0x99;
    opcode_map["SBB D"] = 0x9A; opcode_map["SBB E"] = 0x9B; opcode_map["SBB H"] = 0x9C;
    opcode_map["SBB L"] = 0x9D; opcode_map["SBB M"] = 0x9E;

    opcode_map["INR A"] = 0x3C; opcode_map["INR B"] = 0x04; opcode_map["INR C"] = 0x0C;
    opcode_map["INR D"] = 0x14; opcode_map["INR E"] = 0x1C; opcode_map["INR H"] = 0x24;
    opcode_map["INR L"] = 0x2C; opcode_map["INR M"] = 0x34;

    opcode_map["DCR A"] = 0x3D; opcode_map["DCR B"] = 0x05; opcode_map["DCR C"] = 0x0D;
    opcode_map["DCR D"] = 0x15; opcode_map["DCR E"] = 0x1D; opcode_map["DCR H"] = 0x25;
    opcode_map["DCR L"] = 0x2D; opcode_map["DCR M"] = 0x35;

    opcode_map["INX B"] = 0x03; opcode_map["INX D"] = 0x13; opcode_map["INX H"] = 0x23;
    opcode_map["INX SP"] = 0x33;

    opcode_map["DCX B"] = 0x0B; opcode_map["DCX D"] = 0x1B; opcode_map["DCX H"] = 0x2B;
    opcode_map["DCX SP"] = 0x3B;

    opcode_map["DAD B"] = 0x09; opcode_map["DAD D"] = 0x19; opcode_map["DAD H"] = 0x29;
    opcode_map["DAD SP"] = 0x39;

    opcode_map["ADI"] = 0xC6; // ADI data8
    opcode_map["ACI"] = 0xCE; // ACI data8
    opcode_map["SUI"] = 0xD6; // SUI data8
    opcode_map["SBI"] = 0xDE; // SBI data8

    // Logical Instructions
    opcode_map["ANA A"] = 0xA7; opcode_map["ANA B"] = 0xA0; opcode_map["ANA C"] = 0xA1;
    opcode_map["ANA D"] = 0xA2; opcode_map["ANA E"] = 0xA3; opcode_map["ANA H"] = 0xA4;
    opcode_map["ANA L"] = 0xA5; opcode_map["ANA M"] = 0xA6;

    opcode_map["XRA A"] = 0xAF; opcode_map["XRA B"] = 0xA8; opcode_map["XRA C"] = 0xA9;
    opcode_map["XRA D"] = 0xAA; opcode_map["XRA E"] = 0xAB; opcode_map["XRA H"] = 0xAC;
    opcode_map["XRA L"] = 0xAD; opcode_map["XRA M"] = 0xAE;

    opcode_map["ORA A"] = 0xB7; opcode_map["ORA B"] = 0xB0; opcode_map["ORA C"] = 0xB1;
    opcode_map["ORA D"] = 0xB2; opcode_map["ORA E"] = 0xB3; opcode_map["ORA H"] = 0xB4;
    opcode_map["ORA L"] = 0xB5; opcode_map["ORA M"] = 0xB6;

    opcode_map["CMP A"] = 0xBF; opcode_map["CMP B"] = 0xB8; opcode_map["CMP C"] = 0xB9;
    opcode_map["CMP D"] = 0xBA; opcode_map["CMP E"] = 0xBB; opcode_map["CMP H"] = 0xBC;
    opcode_map["CMP L"] = 0xBD; opcode_map["CMP M"] = 0xBE;

    opcode_map["ANI"] = 0xE6; // ANI data8
    opcode_map["XRI"] = 0xEE; // XRI data8
    opcode_map["ORI"] = 0xF6; // ORI data8
    opcode_map["CPI"] = 0xFE; // CPI data8

    opcode_map["RLC"] = 0x07; opcode_map["RRC"] = 0x0F; opcode_map["RAL"] = 0x17; opcode_map["RAR"] = 0x1F;

    opcode_map["CMA"] = 0x2F; opcode_map["CMC"] = 0x3F; opcode_map["STC"] = 0x37; opcode_map["DAA"] = 0x27;

    // Branching Instructions
    opcode_map["JMP"] = 0xC3; // JMP address
    opcode_map["JC"] = 0xDA;  // JC address
    opcode_map["JNC"] = 0xD2; // JNC address
    opcode_map["JZ"] = 0xCA;  // JZ address
    opcode_map["JNZ"] = 0xC2; // JNZ address
    opcode_map["JP"] = 0xF2;  // JP address
    opcode_map["JM"] = 0xFA;  // JM address
    opcode_map["JPE"] = 0xEA; // JPE address
    opcode_map["JPO"] = 0xE2; // JPO address

    opcode_map["CALL"] = 0xCD; // CALL address
    opcode_map["CC"] = 0xDC;   // CC address
    opcode_map["CNC"] = 0xD4;  // CNC address
    opcode_map["CZ"] = 0xCC;   // CZ address
    opcode_map["CNZ"] = 0xC4;  // CNZ address
    opcode_map["CP"] = 0xF4;   // CP address
    opcode_map["CM"] = 0xFC;   // CM address
    opcode_map["CPE"] = 0xEC;  // CPE address
    opcode_map["CPO"] = 0xE4;  // CPO address

    opcode_map["RET"] = 0xC9; // RET
    opcode_map["RC"] = 0xD8;  // RC
    opcode_map["RNC"] = 0xD0; // RNC
    opcode_map["RZ"] = 0xC8;  // RZ
    opcode_map["RNZ"] = 0xC0; // RNZ
    opcode_map["RP"] = 0xF0;  // RP
    opcode_map["RM"] = 0xF8;  // RM
    opcode_map["RPE"] = 0xE8; // RPE
    opcode_map["RPO"] = 0xE0; // RPO

    // Stack Operations
    opcode_map["PUSH B"] = 0xC5; opcode_map["PUSH D"] = 0xD5; opcode_map["PUSH H"] = 0xE5;
    opcode_map["PUSH PSW"] = 0xF5;

    opcode_map["POP B"] = 0xC1; opcode_map["POP D"] = 0xD1; opcode_map["POP H"] = 0xE1;
    opcode_map["POP PSW"] = 0xF1;

    // I/O and Interrupts
    opcode_map["IN"] = 0xDB; // IN port8
    opcode_map["OUT"] = 0xD3; // OUT port8
    opcode_map["EI"] = 0xFB; opcode_map["DI"] = 0xF3;
    opcode_map["RIM"] = 0x20; opcode_map["SIM"] = 0x30;
    opcode_map["RST 0"] = 0xC7; opcode_map["RST 1"] = 0xCF; opcode_map["RST 2"] = 0xD7;
    opcode_map["RST 3"] = 0xDF; opcode_map["RST 4"] = 0xE7; opcode_map["RST 5"] = 0xEF;
    opcode_map["RST 6"] = 0xF7; opcode_map["RST 7"] = 0xFF;

    // Special
    opcode_map["NOP"] = 0x00;
    opcode_map["HLT"] = 0x76;
    opcode_map["XCHG"] = 0xEB;
    opcode_map["XTHL"] = 0xE3; 
    opcode_map["SPHL"] = 0xF9;
    opcode_map["PCHL"] = 0xE9; // PCHL (Jump to HL)
}
