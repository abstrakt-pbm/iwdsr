#pragma once

#include "../file-formats/elf64.hpp"
#include <cstdint>
#include <unordered_map>
#include <string>

namespace SR {

class Symbol {
    private:
    std::string name;
    bool isLoaded;
    uint64_t startAddr;
    int8_t* rawSymbol;


    public:
    Symbol();

    bool isLoaded();
    uint64_t getStartAddr();
    std::string getName();
};


class SymbolResolver { 
private:
public:
    SymbolResolver( ELF& rootElf );
    void attachSymbol(Symbol* symbol);
    uint64_t whereSymbol(std::string symbolName);
    
};

}; //SR