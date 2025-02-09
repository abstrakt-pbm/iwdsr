#pragma once
#include "../file-formats/elf64.hpp"
#include "symbol-resolver.hpp"
#include "../proc/proc.hpp"


class Loader {
    private:
    bool loadSection(Process& proc, const Section& sectionToLoad);
    bool loadSymbol(Process& proc, const Symbol& sectionToLoad);

    public:
    void loadElf(ELF& elfFile, Process& proc);
    void loadDLL();
};