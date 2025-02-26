#pragma once

#include "../file-formats/elf64.hpp"
#include "../proc/proc.hpp"
#include "memblock.hpp"
#include "symbolresolver.hpp"

class Loader {
    private:
    Process* proc; 
    MemoryMap memMap;
    SR::SymbolResolver symbolResolver;
    void loadSectionsInMemBlk(ELF_PARSER::ELF& elf, MemBlock* blk);

    public:
    Loader(Process* proc);
    void loadElf(ELF_PARSER::ELF& elfFile);
    void loadDLL(std::string libName, ELF_PARSER::ELF& dynamicLib);
};