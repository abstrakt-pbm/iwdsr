#include "loader.hpp"
#include <iostream>

bool Loader::loadElf(ELF& elfFile, Process& process) {
    bool isLoaded = true;
    std::vector<ProgramHeader*> loadableProgHeader = elfFile.getProgramHeadersByPType(P_TYPE::PT_LOAD);
    ProcessMemory* procMem = process.getMemory();
    for ( auto lProgHeader : loadableProgHeader ) {
        int8_t* rawProgramPart = elfFile.rawRead(lProgHeader->p_offset, lProgHeader->p_memsz);
        procMem->allocate(lProgHeader->p_offset, lProgHeader->p_memsz, allignmentToPageSize(lProgHeader->p_align));
    }
    return isLoaded;
}

uint64_t Loader::calculatePageCount(uint64_t byteSize, uint64_t alignment) {
    uint64_t pageCount = 0;
    if ( alignment > 0) {
        pageCount = byteSize / alignment;
        if ( byteSize % alignment != 0) {
            pageCount += 1;
        }
    } 
    return pageCount;
}