#include <utils.hpp>

class Assembler
{
    public:
        Assembler();

        // Assemble
        HEX assemble(const std::string& instruction);

    private:
        // Opcodes
        void loadOpcodeLookup();

        // Parsers
        OPCODE parseMnemonicToOpcode(const std::string& hexstr);
        u16 parseHexStrToVal(const std::string& hexstr);

        // Opcode hashmap
        std::map<std::string, OPCODE> opcode_map;
};