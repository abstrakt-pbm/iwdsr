#pragma once

#include "../file-formats/elf64.hpp"
#include <cstdint>
#include <unordered_map>
#include <string>

namespace SR {

class DLL {
    private:
    uint64_t startVaddr;
    ELF_PARSER::ELF* dllElf;
    
    public:
    DLL(ELF_PARSER::ELF* dllElf);
};

class Symbol {
    private:

    public:
};

class Relocation {
    private:
    std::string dllnameGot;
};

class SymbolResolver {
    private:
    std::unordered_map<std::string, SR::Symbol*> accountingSymbols;
    public:
    void registerDll(ELF_PARSER::ELF& dynamicLib);
    void registerSymbol( ELF_PARSER::Symbol& elfSymbol);

};

}; //SR