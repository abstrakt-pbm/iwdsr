#include "loader.hpp"
#include <iostream>

Loader::Loader(Process* proc) {
    this->proc = proc;
}

void Loader::loadElf(ELF_PARSER::ELF& elfFile) {
    
}

void Loader::loadDLL(ELF_PARSER::ELF& dynamicLib) {
    MemBlock* gotTable = memMap->getFreeBlock(dynamicLib.getGOTSize());
    symbolResolver->registerDll(dynamicLib);
    
}