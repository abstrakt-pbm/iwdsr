#include "loader.hpp"
#include <iostream>


void Loader::loadElf(ELF_PARSER::ELF& elfFile, Process& process) {
    std::vector<ELF_PARSER::ProgramHeader*> loadableProgHeader = elfFile.getProgramHeadersByPType(ELF_PARSER::P_TYPE::PT_LOAD);
    ProcessMemory* procMem = process.getMemory();
    for ( auto lProgHeader : loadableProgHeader ) {
        int8_t* rawProgramPart = elfFile.rawRead(lProgHeader->p_offset, lProgHeader->p_memsz);
        procMem->writeMem(lProgHeader->p_vaddr, rawProgramPart, lProgHeader->p_memsz);
        delete[] rawProgramPart;
    }
}

