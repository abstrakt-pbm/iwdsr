#pragma once
#include "../file-formats/elf64.hpp"
#include "symbol-resolver.hpp"
#include "../proc/proc.hpp"


class Loader {
    private:
    uint64_t calculatePageCount(uint64_t byteSize, uint64_t alignment);
    bool loadSection(Process& proc, const Section& sectionToLoad);
    bool loadSymbol(Process& proc, const Symbol& sectionToLoad);

    public:
    bool loadElf(ELF& elfFile, Process& proc);
    bool loadDLL();
};