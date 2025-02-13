#include "loader.hpp"
#include <iostream>


void Loader::loadElf(ELF& elfFile, Process& process) {
    std::vector<ProgramHeader*> loadableProgHeader = elfFile.getProgramHeadersByPType(P_TYPE::PT_LOAD);
    ProcessMemory* procMem = process.getMemory();
    for ( auto lProgHeader : loadableProgHeader ) {
        int8_t* rawProgramPart = elfFile.rawRead(lProgHeader->p_offset, lProgHeader->p_memsz);
    }
}

