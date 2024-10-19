#include <iostream>
#include <fstream>
#include <cstdint>
#include "iwdsr/file-formats/elf64.hpp"

int main() {
    ELF elf("E:\\blackMetal\\a.out"); 
    for ( auto prh : elf.programHeaders ) {
        std::cout << std::hex << prh.p_align << std::endl;
    }
}