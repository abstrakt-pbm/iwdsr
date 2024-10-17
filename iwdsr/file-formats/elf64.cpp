#include "elf64.hpp"
#include <iostream>
#include <string>
#include <span>

template<typename Type>
Type changeEndian(Type value) {
    Type changedEndian;
    for( int i = sizeof(Type) - 1 ; i >= 0 ; i-- ) {
        ((std::uint8_t*)&changedEndian)[i] = ((std::uint8_t*)&value)[sizeof(Type) - i - 1];
    }
    return changedEndian;
};


ELF::ELF(std::filesystem::path pathToELF) {
    std::fstream elfFile(pathToELF, std::ios::in | std::ios::binary );
    if ( !elfFile.is_open() ) {
        std::cout << "ELF file is not open" << std::endl;
        return;
    }
    this->elfFile = &elfFile;
    elfHeader = parseELFHeader();
    sectionHeaders = parseSectionHeaders();
    elfFile.close();
}; 

ELF::~ELF() {
    delete elfHeader;
}

ELF_Header* ELF::parseELFHeader(){
    char rawElfHeader[64];
    elfFile->read(rawElfHeader, 64);
    
    std::span<char> rawElfMagic(rawElfHeader,4);
    if (std::string(rawElfMagic.begin(),rawElfMagic.end()).compare(std::string({0x7F, 'E', 'L', 'F'})) != 0) {
        std::cout << "Bad ELF magic symbol" << std::endl; 
        return nullptr;
    } 
    ELF_Header* elfHdr = new ELF_Header;
    if ( elfHdr != nullptr) {
    elfHdr->ei_class = static_cast<EI_CLASS>(rawElfHeader[4]);
    elfHdr->ei_data = static_cast<EI_DATA>(rawElfHeader[5]);
    elfHdr->ei_version = rawElfHeader[6];
    elfHdr->ei_osabi = static_cast<EI_OSABI>(rawElfHeader[7]);
    elfHdr->ei_abiversion = *((std::int8_t*)(rawElfHeader + 8));
    elfHdr->e_type = static_cast<E_TYPE>(*((std::uint8_t*)(rawElfHeader + 16)));
    elfHdr->e_entry = (*((std::uint64_t*)(rawElfHeader + 24)));
    elfHdr->e_phoff = (*((std::uint64_t*)(rawElfHeader + 32)));
    elfHdr->e_shoff = (*((std::uint64_t*)(rawElfHeader + 40)));
    elfHdr->e_flags = *((std::int32_t*)(rawElfHeader + 48));
    elfHdr->e_ehsize = *((std::int8_t*)(rawElfHeader + 52));
    elfHdr->e_phentsize = *((std::int8_t*)(rawElfHeader + 54));
    elfHdr->e_phnum = *((std::int8_t*)(rawElfHeader + 56));
    elfHdr->e_shentsize = *((std::int8_t*)(rawElfHeader + 58));
    elfHdr->e_shnum = *((std::int8_t*)(rawElfHeader + 60));
    elfHdr->e_shstrndx = *((std::int8_t*)(rawElfHeader + 62));
    }

    return elfHdr;
};

std::vector<SectionHeader> ELF::parseSectionHeaders() {
    std::vector<SectionHeader> sectionHdrs;
    if ( elfHeader->e_shoff == 0) {
        return sectionHdrs;
    }
    char* rawSectionTable = new char[elfHeader->e_shnum * elfHeader->e_shentsize];

    elfFile->seekg(elfHeader->e_shoff, std::ios::beg);
    

    elfFile->read(rawSectionTable ,elfHeader->e_shnum * elfHeader->e_shentsize);


    for ( auto i = 0; i < elfHeader->e_shnum ; i++ ) {
        SectionHeader sectionHdr;
        sectionHdr.sh_name = (std::int32_t)rawSectionTable[ i * elfHeader->e_shentsize];
        sectionHdr.sh_type = static_cast<SH_TYPE>( rawSectionTable[ i * elfHeader->e_shentsize + 4] );
        sectionHdr.sh_flags = static_cast<SH_FLAGS>( rawSectionTable[ i * elfHeader->e_shentsize + 8] );
        sectionHdr.sh_addr =  (std::int64_t)rawSectionTable[ i * elfHeader->e_shentsize + 16];
        sectionHdr.sh_offset = (std::int64_t)rawSectionTable[ i * elfHeader->e_shentsize + 24];
        sectionHdr.sh_size = (std::int64_t)rawSectionTable[ i * elfHeader->e_shentsize + 32 ];
        sectionHdr.sh_link = (std::int32_t)rawSectionTable [ i * elfHeader->e_shentsize + 40 ];
        sectionHdr.sh_info = (std::int32_t)rawSectionTable [ i * elfHeader->e_shentsize + 44 ];
        sectionHdr.sh_addralign = (std::int64_t)rawSectionTable[ i * elfHeader->e_shentsize + 48 ];
        sectionHdr.sh_entsize = (std::int64_t)rawSectionTable[ i * elfHeader->e_shentsize + 56 ];
        sectionHdrs.push_back(sectionHdr);

    }

    delete [] rawSectionTable;
    return sectionHdrs;

}

std::vector<std::string> ELF::getSectionNames(char* rawShStrNdx) {
    std::vector<std::string> sectionNames;
    SectionHeader shStrNdx = parseOneSectionHeader(rawShStrNdx);


    return sectionNames;
}

SectionHeader ELF::parseOneSectionHeader(char* rawSectionHeader) {
    SectionHeader sectionHdr;
    sectionHdr.sh_name = (std::int32_t)rawSectionHeader[0];
    sectionHdr.sh_type = static_cast<SH_TYPE>( rawSectionHeader[4] );
    sectionHdr.sh_flags = static_cast<SH_FLAGS>( rawSectionHeader[8] );
    sectionHdr.sh_addr =  (std::int64_t)rawSectionHeader[16];
    sectionHdr.sh_offset = (std::int64_t)rawSectionHeader[24];
    sectionHdr.sh_size = (std::int64_t)rawSectionHeader[32];
    sectionHdr.sh_link = (std::int32_t)rawSectionHeader[40];
    sectionHdr.sh_info = (std::int32_t)rawSectionHeader[44];
    sectionHdr.sh_addralign = (std::int64_t)rawSectionHeader[48];
    sectionHdr.sh_entsize = (std::int64_t)rawSectionHeader[56];    
    return sectionHdr;
}