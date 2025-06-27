#include <mmu.hpp>

class Assembler
{
    public:
        Assembler();

        // Assemble
        u16 assemble(MMU& mmu, const char* filename);

    private:
        // Opcodes
        u16 origin = 0x0000, cur_off = 0x0000;
        void loadOpcodeLookup();

        // Steps
        std::string preprocess(const std::string& raw_str);
        u16 map(MMU& mmu, const std::string& proc_str);
        u8 get_instr_len(const std::string& instruction);

        // Parsers
        HEX parseToMachineCode(const std::string& instruction);
        OPCODE parseMnemonicToOpcode(const std::string& hexstr);
        u16 parseHexStrToVal(const std::string& hexstr);

        // Maps
        std::map<std::string, u8> opcode_map;
        std::map<std::string, u16> label_map;
};