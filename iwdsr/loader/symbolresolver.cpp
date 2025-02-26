#include "symbolresolver.hpp"

void SR::SymbolResolver::resolveSymbols( std::string imageName ) {
    NamedImage* image = memMap->getImage( imageName );
    if ( image == nullptr ) {
        return;
    }

    GlobalOffsetTable* targetGOT = image->getGlobalOffsetTable();
    ELF_PARSER::ELF* targetImgElf = image->getOriginElf();

    for ( auto libName : targetImgElf->getLibDependencies() ) {
        if ( memMap->isImageExists( libName )) {
            auto libImg = memMap->getImage( libName );
            if ( libImg == nullptr) {
                continue;
            }

            ELF_PARSER::ELF* libImgElf = libImg->getOriginElf();
            auto libSymbols = libImgElf->getSymbols();
            for ( auto libSymb : libSymbols) {
                uint64_t libSymbBaseAddr = libImg->getSymbolAddressByName( libSymb.first );
                uint16_t idInGot = targetImgElf->getSymbolIdInGotByDynSymbolName();
                targetGOT->changePtr(idInGot, libSymbBaseAddr);
            }
        }
    } 
}
