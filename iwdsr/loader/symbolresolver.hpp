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

    public:
    SymbolResolver() = default;
    SymbolResolver( ProcessMemory* procMem, MemoryMap* memMap );
    void resolveSymbols( std::string imageName ); // Загружаем библиотеки в MemMap и пытаемся отрезольвить всё что можем 
    bool isImageNeedToBeResolved( std::string imageName );
};

}; //SR