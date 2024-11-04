#pragma once
#include <cstdint>
#include <vector>
#include "memory-map.hpp"


using VAddr = std::uint64_t;


class VMBlock {
    private:
    VAddr start;
    VAddr end;
    void* blockSpace;

    bool isFree;
    public:
    VMBlock(VAddr start, VAddr end);
    [[nodiscard]] void* getBlockSpace() const;
    [[nodiscard]] bool isVAddrInBlock(VAddr vaddr) const;
    [[nodiscard]] VAddr getStartAddr() const;
}; 

class VirtualMemory {
    private:
    std::vector<VMBlock*> vmBlocks;
    uint64_t startVspace;
    uint64_t endVspace;
    public:
    VirtualMemory(uint64_t startVspace, uint64_t endVspace);
    uint64_t* alloc(VAddr start, uint64_t lenght);
    void dealloc(VAddr* vmbStartAddr);
    int unWrap( MemoryMap processMemoryMap);
};