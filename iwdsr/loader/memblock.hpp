#pragma once

#include <cstdint>
#include <string>

class MemBlock {
    
};

class MemoryMap{
    public:
    MemBlock* getFreeBlock(uint64_t size);
    
};