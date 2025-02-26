#include "loader.hpp"
#include <iostream>

Loader::Loader(Process* proc) {
    this->proc = proc;
    this->symbolResolver = SR::SymbolResolver(proc->getMemory(), &memMap); 
}

void Loader::loadElf(ELF_PARSER::ELF* elfFile) {
    MemBlock* elfMemBlk = memMap.allocate( 0x10000, elfFile->getMemImageSize());
    if ( elfMemBlk != nullptr) {
        loadSectionsInMemBlk( elfFile, elfMemBlk ); 
    }

    memMap.makeNamedMapping( "executable", elfMemBlk, elfFile );

    for ( auto lib : elfFile->getLibDependencies() ) {
        loadDLL(lib, nullptr);
    }

    symbolResolver.resolveSymbols("executable");

}

void Loader::loadDLL( std::string libName, ELF_PARSER::ELF* dynamicLib) {
    MemBlock* dllMemBlk = memMap.getFreeBlock(dynamicLib->getMemImageSize());
    ProcessMemory* procMem = proc->getMemory();
    if ( dllMemBlk != nullptr ) {
        loadSectionsInMemBlk( dynamicLib, dllMemBlk );
    }

    memMap.makeNamedMapping( libName, dllMemBlk, dynamicLib );

    for ( auto lib : dynamicLib->getLibDependencies() ) {
        loadDLL(lib, nullptr);
    }
    symbolResolver.resolveSymbols( libName );
}

void Loader::loadSectionsInMemBlk(ELF_PARSER::ELF* elf, MemBlock* blk) {
    ProcessMemory* procMem = proc->getMemory();
    for ( auto loadableSection : elf->getProgramHeadersByPType(ELF_PARSER::P_TYPE::PT_LOAD)) {
        int8_t* rawSection = elf->rawRead(
            loadableSection->p_offset,
            loadableSection->p_memsz
        );
        procMem->writeMem(
            loadableSection->p_vaddr + blk->getStartAddr(),
            rawSection,
            loadableSection->p_memsz
        );
    }
}