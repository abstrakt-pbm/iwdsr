#pragma once
#include <cstdint>
#include <vector>
#include "memory-map.hpp"
#include <unordered_map>
#include <string>
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
    std::unordered_map<std::string, VMBlock*> symbols;
    public:
    VirtualMemory(MemoryMap* processMemoryMap);
    uint8_t* loadSymbol();

};
