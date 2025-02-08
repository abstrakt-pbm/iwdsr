#pragma once

#include <cstdint>

class MemoryBlock { 
private:
    uint64_t startAddr;
    uint64_t endAddr;
    uint64_t allignment;

public:
    MemoryBlock(uint64_t startAddr);
    bool operator < (uint64_t addr);
    bool operator > (uint64_t addr);
    bool operator = (uint64_t addr);
};


class MemoryBlockList {
private:
MemoryBlock* root;
MemoryBlock* previuosBlk;
MemoryBlock* nextBlk;

public:
MemoryBlockList(MemoryBlock* root);
MemoryBlock* find();
void append(MemoryBlock* blkToAdd);
void dell(uint64_t blkStartAddr);
};