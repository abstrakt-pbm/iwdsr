#include "elf64.hpp"
#include <iostream>
#include <string>
#include <span>

ELF::ELF(std::filesystem::path pathToELF) {
    std::fstream elfFile(pathToELF, std::ios::in | std::ios::binary );
    if ( !elfFile.is_open() ) {
        std::cout << "ELF file is not open" << std::endl;
        return;
    }
    this->elfFile = &elfFile;
    parseELFHeader(); 
    elfFile.close();
}; 

void ELF::parseELFHeader(){
    char rawElfHeader[64];
    elfFile->read(rawElfHeader, 64);
    
    std::span<char> rawElfMagic(rawElfHeader,4);
    if (std::string(rawElfMagic.begin(),rawElfMagic.end()).compare(std::string({0x7F, 'E', 'L', 'F'})) != 0) {
        std::cout << "Bad ELF magic symbol" << std::endl; 
        return;
    }

    for (auto i = 0; i < 64; i++) {
        std::cout << std::hex << (int)rawElfHeader[i] << '\n';
    }


    
    elfHeader.ei_class = static_cast<EI_CLASS>(rawElfHeader[4]);
    elfHeader.ei_data = static_cast<EI_DATA>(rawElfHeader[5]);
    elfHeader.ei_version = rawElfHeader[6];
    elfHeader.ei_osabi = static_cast<EI_OSABI>(rawElfHeader[7]);
    elfHeader.ei_abiversion = rawElfHeader[8];
    elfHeader.e_type = static_cast<E_TYPE>((std::int8_t)rawElfHeader[16]);
    elfHeader.e_entry = rawElfHeader[24];
    elfHeader.e_phoff = rawElfHeader[32];
    elfHeader.e_shoff = rawElfHeader[40];
    elfHeader.e_flags = rawElfHeader[48];
    elfHeader.e_ehsize = rawElfHeader[52];
    elfHeader.e_phentsize = rawElfHeader[54];
    elfHeader.e_phnum = rawElfHeader[56];
    elfHeader.e_shentsize = rawElfHeader[58];
    elfHeader.e_shnum = rawElfHeader[60];
    elfHeader.e_shstrndx = rawElfHeader[62];

};