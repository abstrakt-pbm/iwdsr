#include "elf64.hpp"
#include <fstream>
#include <iostream>
#include <string>


ELF::ELF(std::filesystem::path pathToELF) {
    std::fstream elfFile(pathToELF, std::ios::in | std::ios::binary );
    if ( !elfFile.is_open() ) {
        std::cout << "ELF file is not open" << std::endl;
        return;
    }

    char elfMagicSymbolBuffer[4];
    elfFile.read(elfMagicSymbolBuffer, 4);
    if (std::string(elfMagicSymbolBuffer).compare(std::string({0x7F, 'E', 'L', 'F'})) != 0) {
        std::cout << "Bad ELF magic symbol" << std::endl;
    }

    
    
    
}; 