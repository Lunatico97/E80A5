#include <mmu.hpp>

class Assembler
{
    public:
        Assembler();

        // Assemble
        u16 assemble(MMU& mmu, const std::string& filename);

    private:
        // Opcodes
        void loadOpcodeLookup();

        // Parsers
        HEX parseToMachineCode(const std::string& instruction);
        OPCODE parseMnemonicToOpcode(const std::string& hexstr);
        u16 parseHexStrToVal(const std::string& hexstr);

        // Maps
        std::map<std::string, u8> opcode_map;
        std::map<std::string, u16> label_map;
};