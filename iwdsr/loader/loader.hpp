#pragma once
#include "../file-formats/elf64.hpp"
#include "symbol-resolver.hpp"
#include "../program/program.hpp"


class Loader {
    private:
    uint64_t calculatePageCount(uint64_t byteSize, uint64_t alignment);
    bool loadSection(Program& prog, const Section& sectionToLoad);
    bool loadSymbol(Program& prog, const Symbol& sectionToLoad);

    public:
    bool loadElf(ELF& elfFile, Program& prog);
    bool loadDLL();
};