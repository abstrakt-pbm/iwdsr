#pragma once

#include <filesystem>
#include "../../file-formats/elf.hpp"

class ElfLoader {
    private:
    public:
    ElfLoader();
    void load(const ELF& elf);
    
};

