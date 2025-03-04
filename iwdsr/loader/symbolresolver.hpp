#pragma once

#include "../file-formats/elf64.hpp"
#include "../proc/proc.hpp"
#include "memblock.hpp"
#include <cstdint>
#include <unordered_map>
#include <string>

namespace SR {

class SymbolResolver {
    private:
    ProcessMemory* procMem;
    MemoryMap* memMap;
    std::unordered_map<std::string, bool> resolveState; //<libname | state>

    ElfMemoryImage* findImgWithSymbol( std::string symbolName);

    public:
    SymbolResolver() = default;
    SymbolResolver( ProcessMemory* procMem, MemoryMap* memMap );
    void resolveImage( std::string imageName ); 
    void makeRelocation( ELF_PARSER::Rela*, ElfMemoryImage* );
};

}; //SR