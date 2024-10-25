#include "elf64.hpp"
#include <iostream>
#include <string>
#include <span>

template<typename Type>
Type changeEndian(Type value) {
    Type changedEndian;
    for( int i = sizeof(Type) - 1 ; i >= 0 ; i-- ) {
        ((uint8_t*)&changedEndian)[i] = ((uint8_t*)&value)[sizeof(Type) - i - 1];
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
    programHeaders = parseProgramHeaders();
    elfFile.close();
}; 

ELF::~ELF() {
}

ELF_Header ELF::parseELFHeader(){
    char rawElfHeader[64];
    elfFile->read(rawElfHeader, 64);
    
    std::span<char> rawElfMagic(rawElfHeader,4);
    if (std::string(rawElfMagic.begin(),rawElfMagic.end()).compare(std::string({0x7F, 'E', 'L', 'F'})) != 0) {
        std::cout << "Bad ELF magic symbol" << std::endl;
        return ELF_Header();
    } 
    ELF_Header elfHdr;
    elfHdr.ei_class = static_cast<EI_CLASS>(rawElfHeader[4]);
    elfHdr.ei_data = static_cast<EI_DATA>(rawElfHeader[5]);
    elfHdr.ei_version = rawElfHeader[6];
    elfHdr.ei_osabi = static_cast<EI_OSABI>(rawElfHeader[7]);
    elfHdr.ei_abiversion = *((int8_t*)(rawElfHeader + 8));
    elfHdr.e_type = static_cast<E_TYPE>(*((uint8_t*)(rawElfHeader + 16)));
    elfHdr.e_entry = *((uint64_t*)(rawElfHeader + 24));
    elfHdr.e_phoff = *((uint64_t*)(rawElfHeader + 32));
    elfHdr.e_shoff = *((uint64_t*)(rawElfHeader + 40));
    elfHdr.e_flags = *((int32_t*)(rawElfHeader + 48));
    elfHdr.e_ehsize = *((int8_t*)(rawElfHeader + 52));
    elfHdr.e_phentsize = *((int8_t*)(rawElfHeader + 54));
    elfHdr.e_phnum = *((int8_t*)(rawElfHeader + 56));
    elfHdr.e_shentsize = *((int8_t*)(rawElfHeader + 58));
    elfHdr.e_shnum = *((int8_t*)(rawElfHeader + 60));
    elfHdr.e_shstrndx = *((int8_t*)(rawElfHeader + 62));

    return elfHdr;
};

std::vector<SectionHeader> ELF::parseSectionHeaders() {
    std::vector<SectionHeader> sectionHdrs (elfHeader.e_shnum);
    if ( elfHeader.e_shoff == 0) {
        return sectionHdrs;
    }
    char* rawSectionTable = new char[elfHeader.e_shnum * elfHeader.e_shentsize];

    elfFile->seekg(elfHeader.e_shoff, std::ios::beg);
    elfFile->read(rawSectionTable ,elfHeader.e_shnum * elfHeader.e_shentsize);
    

    for ( auto i = 0; i < elfHeader.e_shnum ; i++ ) {
        SectionHeader sectionHdr;
        sectionHdr.sh_name = *(int32_t*)(rawSectionTable + i * elfHeader.e_shentsize);
        sectionHdr.sh_type = static_cast<SH_TYPE>( rawSectionTable[ i * elfHeader.e_shentsize + 4] );
        sectionHdr.sh_flags = static_cast<SH_FLAGS>( rawSectionTable[ i * elfHeader.e_shentsize + 8] );
        sectionHdr.sh_addr =  *(int64_t*)(rawSectionTable + i * elfHeader.e_shentsize + 16);
        sectionHdr.sh_offset = *(int64_t*)(rawSectionTable + i * elfHeader.e_shentsize + 24);
        sectionHdr.sh_size = *(int64_t*)(rawSectionTable + i * elfHeader.e_shentsize + 32);
        sectionHdr.sh_link = *(int32_t*)(rawSectionTable + i * elfHeader.e_shentsize + 40);
        sectionHdr.sh_info = *(int32_t*)(rawSectionTable + i * elfHeader.e_shentsize + 44);
        sectionHdr.sh_addralign = *(int64_t*)(rawSectionTable + i * elfHeader.e_shentsize + 48);
        sectionHdr.sh_entsize = *(int64_t*)(rawSectionTable + i * elfHeader.e_shentsize + 56);
        sectionHdrs[i] = sectionHdr;
    }

    delete [] rawSectionTable;
    return sectionHdrs;
}

std::vector<ProgramHeader> ELF::parseProgramHeaders() {
    std::vector<ProgramHeader> programHdrs(elfHeader.e_phnum);
    char* rawProgramHeaderTable = new char [ elfHeader.e_phentsize * elfHeader.e_phnum ];
    elfFile->seekg(elfHeader.e_phoff, std::ios::beg);
    elfFile->read(rawProgramHeaderTable, elfHeader.e_phentsize * elfHeader.e_phnum);

    for( auto i = 0 ; i < elfHeader.e_phnum ; i++ ) {
        ProgramHeader programHdr;
        programHdr.p_type = static_cast<P_TYPE>(*(uint32_t*)(rawProgramHeaderTable + i * elfHeader.e_phentsize ));
        programHdr.p_flag = static_cast<P_FLAGS>(*(uint32_t*)(rawProgramHeaderTable + i * elfHeader.e_phentsize + 4));
        programHdr.p_offset = *(uint64_t*)(rawProgramHeaderTable + i * elfHeader.e_phentsize + 8);
        programHdr.p_vaddr  = *(uint64_t*)(rawProgramHeaderTable + i * elfHeader.e_phentsize + 16);
        programHdr.p_paddr = *(uint64_t*)(rawProgramHeaderTable + i * elfHeader.e_phentsize + 24);
        programHdr.p_filesz = *(uint64_t*)(rawProgramHeaderTable + i * elfHeader.e_phentsize + 32);
        programHdr.p_memsz = *(uint64_t*)(rawProgramHeaderTable + i * elfHeader.e_phentsize + 40);
        programHdr.p_align = *(uint64_t*)(rawProgramHeaderTable + i * elfHeader.e_phentsize + 48);
        programHdrs[i] = programHdr;
    } 

    delete rawProgramHeaderTable; 
    return programHdrs;
}