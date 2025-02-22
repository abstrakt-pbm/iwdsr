#pragma once

#include "../file-formats/elf64.hpp"
#include <cstdint>
#include <unordered_map>
#include <string>

namespace SR {

class Symbol {
    private:
    bool isResolved;

    std::string name;
    int8_t* rawSymbol;
    std::unordered_map< std::string, Symbol* > dependendSymbols;
    std::unordered_map< uint64_t, std::string > relocations; // <offsetInSymbol | symbol name>

    public:
    Symbol( std::string name, int8_t* rawSymbol, std::unordered_map<std::string, SR::Symbol*> dependendSymbols);

    std::string getName();
    int8_t* getRaw();

    void resolve();

};

class SymbolResolver { 
private:
    std::unordered_map<std::string, Symbol*> rootSymbols;
    std::unordered_map<std::string, Symbol*> searchSymbolDependencies(ELF_PARSER::ELF& searchingFrom, ELF_PARSER::Symbol& symbol);

public:
    SymbolResolver( ELF_PARSER::ELF& rootElf );
    void fetchSymbolsFromElf( ELF_PARSER::ELF& dll);
    
};

}; //SR