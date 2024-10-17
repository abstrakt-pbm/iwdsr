#include <iostream>
#include <fstream>
#include <cstdint>
#include "iwdsr/file-formats/elf64.hpp"

int main() {
    ELF elf("/home/pablo/tmp/a.out");
    for ( auto sec : elf.sectionHeaders ) {
        std::cout << std::hex << sec.sh_addr << std::endl;
    } 
}