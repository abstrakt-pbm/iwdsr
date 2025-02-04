#include "loader.hpp"
#include <iostream>

bool Loader::loadElf(ELF& elfFile, Program& program) {
    bool isLoaded = true;
    std::vector<ProgramHeader*> loadableProgHeader = elfFile.getProgramHeadersByPType(P_TYPE::PT_LOAD);
    ProgramMemory* progMem = program.getMemory();
    for ( auto lProgHeader : loadableProgHeader ) {
        char* rawProgramPart = elfFile.rawRead(lProgHeader->p_offset, lProgHeader->p_memsz);
        uint64_t pageCount = calculatePageCount(lProgHeader->p_memsz, lProgHeader->p_align);
        progMem->allocatePages(lProgHeader->p_vaddr, allignmentToPageSize(lProgHeader->p_align), pageCount);
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