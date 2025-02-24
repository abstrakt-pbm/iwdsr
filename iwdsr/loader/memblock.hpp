#pragma once

#include <cstdint>
#include <string>

class MemBlock {
    public:
    uint64_t getStartAddr(); 
};

class MemoryMap{
    public:
    MemBlock* getFreeBlock(uint64_t size);
    
};