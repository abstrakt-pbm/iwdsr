#pragma once
#include "../file-formats/elf64.hpp"
#include "symbol-resolver.hpp"
#include "../pthread/pthread.hpp"
class Loader {
    SymbolResolver smbRsvr; 
    public:
    ProcThread loadElf(const ELF& elfFile);
};