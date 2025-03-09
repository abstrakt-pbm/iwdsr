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
            std::cout << std::format("Failed to found memory image for lib : {}", libName) << std::endl;
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
        case ELF_PARSER::R_X86_64_GLOB_DAT:
        case ELF_PARSER::R_X86_64_64:
        case ELF_PARSER::R_X86_64_JUMP_SLOT: {
            uint64_t writeAddr = img->getBaseAddr() + rela->r_offset + rela->r_addend;
            uint16_t dynSymbId = rela->getSymbolId();
            ELF_PARSER::DynamicSymbol* dynSymb = img->getOriginElf()->getDynSymbolById(dynSymbId);
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

        case ELF_PARSER::R_X86_64_RELATIVE: {
            uint64_t resultAddress = img->getBaseAddr() + rela->r_offset + rela->r_addend;
            procMem->writeMem(
                rela->r_offset,
                (int8_t*)(resultAddress),
                8
            );
            break;
        }

        case ELF_PARSER::R_X86_64_REX_GOTPCRELX: {
           uint64_t resultWrite = img->getDynSymAddressById(rela->getSymbolId()) - rela->r_offset; 
           procMem->writeMem(
            rela->r_offset,
            (int8_t*)(resultWrite),
            8
           );
           break;
        }

        case ELF_PARSER::R_X86_64_TPOFF64: {

        }
        

        default : {
            std::cout << std::format( "Unimplemented relocation: {}", (int)reloType ) << std::endl;
        }
    }
}

ElfMemoryImage* SR::SymbolResolver::findImgWithSymbol( std::string symbolName) {
    auto imgs = memMap->getImages();
    for ( auto img : imgs ) {
        ELF_PARSER::ELF* imgElf = img->getOriginElf();
        if ( imgElf->isExportSymbolByName(symbolName)){
            return img;
        }
    }
    return nullptr;
}