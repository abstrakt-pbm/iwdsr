#pragma once
#include "../file-formats/elf64.hpp"
#include "../proc/proc.hpp"


class Loader {
    private:

    public:
    void loadElf(ELF_PARSER::ELF& elfFile, Process& proc);
    void loadDLL();
};