#pragma once

#include "../file-formats/elf64.hpp"
#include "../proc/proc.hpp"
#include "memblock.hpp"
#include "symbolresolver.hpp"
#include <unordered_map>
#include <filesystem>

constexpr std::string BASE_ELF_MEMORY_IMAGE = "ELF_EXECUTABLE";



class Loader {
    private:
    Process* proc; 
    MemoryMap memMap;
    SR::SymbolResolver symbolResolver;
    std::unordered_map<std::string, ELF_PARSER::ELF*> libPool;

    void loadElfInMemBlk(ELF_PARSER::ELF* elf, MemBlock* blk);

    public:
    Loader(Process* proc);
    bool fillLibPool( std::filesystem::path pathToLibDir );
    void loadElf(ELF_PARSER::ELF* elfFile);
    void loadDLL(std::string libName, ELF_PARSER::ELF* dynamicLib);
};