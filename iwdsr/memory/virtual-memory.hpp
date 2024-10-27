#pragma once
#include <cstdint>
#include <vector>
#include "memory-map.hpp"

using VAddr = std::uint64_t;

class VMBlock {
    private:
    VAddr start;
    VAddr end;
    uint64_t* rawSpace;
    bool isFree;

    public:
    VMBlock(VAddr start, VAddr end, uint64_t* rawSpace);
    uint64_t* getRawSpace(); 
    bool isAddrInBlock(VAddr vaddr);
}; 

class VirtualMemory {
    private:
    std::vector<VMBlock*> memoryContainer;
    public:
    uint64_t* alloc(VAddr start, uint64_t lenght);
    uint64_t* map(uint64_t start, char payload[], uint64_t lenght); 
    int unWrap( MemoryMap processMemoryMap);
};