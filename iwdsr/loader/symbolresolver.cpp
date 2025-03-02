#include "symbolresolver.hpp"
#include <iostream>
#include <format>

SR::SymbolResolver::SymbolResolver( ProcessMemory* procMem, MemoryMap* memMap ) {
    this->procMem = procMem;
    this->memMap = memMap;
}

void SR::SymbolResolver::resolveImage( std::string imageName ) {
    ElfMemoryImage* image = memMap->getImage( imageName );
    if ( image == nullptr ) {
        std::cout << std::format("Failed to found memory image for: {}", imageName) << std::endl;
        return;
    }

    ELF_PARSER::ELF* targetImgElf = image->getOriginElf();

    for ( auto libName : targetImgElf->getLibDependencies() ) {
        auto libImg = memMap->getImage( libName );
        if ( libImg == nullptr) {
            std::cout << std::format("Failed to found memory image for: {}", libName) << std::endl;
            continue;
        }

        ELF_PARSER::ELF* libImgElf = libImg->getOriginElf();
        for ( auto targetImgRela : targetImgElf->getRelas()) {
            makeRelocation( targetImgRela, image);
        }
    }
}

void SR::SymbolResolver::makeRelocation( ELF_PARSER::Rela* rela, ElfMemoryImage* img) {
    ELF_PARSER::RelocationType reloType = rela->getRelocationType();
    switch ( reloType ) {
        case ELF_PARSER::R_X86_64_JUMP_SLOT: {
            uint64_t writeAddr = img->getBaseAddr() + rela->r_offset;
            uint16_t dynSymbId = rela->getSymbolId();
            ELF_PARSER::Symbol* dynSymb = img->getOriginElf()->getDynSymbolById(dynSymbId);
            ElfMemoryImage* libImg = findImgWithSymbol( dynSymb->getName());
            if ( libImg == nullptr) {
                std::cout << std::format("Symbol not found in any memory images: {}", dynSymb->getName()) << std::endl;
                return;
            }

            uint64_t symbAddr = libImg->getSymbolAddressByName( dynSymb->getName() ); 
            procMem->writeMem( 
                writeAddr,
                (int8_t*)( &symbAddr ),
                8
            );
            std::cout << std::format("Resolved Symbol: {}", dynSymb->getName()) << std::endl;
            break;
        }

        default : {
            std::cout << std::format( "Relocation is not implemented" ) << std::endl;
        }
    }
}

ElfMemoryImage* SR::SymbolResolver::findImgWithSymbol( std::string symbolName) {
    for ( auto img : memMap->getImages()) {
        ELF_PARSER::ELF* imgElf = img->getOriginElf();
        if ( imgElf->containSymbolByName(symbolName)){
            return img;
        }
    }
    return nullptr;
}