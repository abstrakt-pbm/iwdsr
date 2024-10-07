#pragma once

#include <filesystem>
#include "../../file-formats/elf64.hpp"

class ElfLoader {
    private:
    public:
    ElfLoader();
    void load(const ELF& elf);
    
};

