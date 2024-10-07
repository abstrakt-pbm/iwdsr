#pragma once
#include "../../file-formats/elf64.hpp"

class VirtualMemory {
    public:
    VirtualMemory();
    void mmap(const ELF& elfToLoad)
    
}