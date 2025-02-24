#include "loader.hpp"
#include <iostream>

Loader::Loader(Process* proc) {
    this->proc = proc;
}

void Loader::loadElf(ELF_PARSER::ELF& elfFile) {
    ProcessMemory* procMem = proc->getMemory();
    for ( auto loadableSection : elfFile.getProgramHeadersByPType(ELF_PARSER::P_TYPE::PT_LOAD)) {
        int8_t* rawSection = elfFile.rawRead(
            loadableSection->p_offset,
            loadableSection->p_memsz
        );
        procMem->writeMem(
            loadableSection->p_vaddr,
            rawSection,
            loadableSection->p_memsz
        );
    } 
}

void Loader::loadDLL(ELF_PARSER::ELF& dynamicLib) {
    MemBlock* dllAddrSpace = memMap->getFreeBlock(dynamicLib.getMemImageSize());


}