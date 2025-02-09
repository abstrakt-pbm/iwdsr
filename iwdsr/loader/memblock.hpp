#pragma once

#include <cstdint>

uint64_t calculatePageCount(uint64_t byteSize, uint64_t alignment);

enum MemState {
    FREE,
    ALLOCATED,
    RESERVED
};

class MemoryBlockList;

class MemoryBlock { 
friend MemoryBlockList;
private:
    uint64_t startAddr;
    uint64_t endAddr;
    uint64_t allignment;

    MemoryBlock* previuosBlk;
    MemoryBlock* nextBlk;
    MemState state;

public:
    MemoryBlock( MemState state, uint64_t startAddr, uint64_t payloadSize, uint64_t allignment, MemoryBlock* previuosBlk = nullptr, MemoryBlock* nextBlk = nullptr );
    bool operator < (uint64_t addr);
    bool operator > (uint64_t addr);
    bool operator == (uint64_t addr);
};


class MemoryBlockList {
private:

uint64_t leftAddrRange;
uint64_t rightAddrRange;

MemoryBlock* root;
MemoryBlock* previuosBlk;
MemoryBlock* nextBlk;

public:
MemoryBlockList(uint64_t lefAddrRange, uint64_t rightAddrRange);
MemoryBlock* find( uint64_t addr );
void append(MemoryBlock* blkToAdd);
void dell(uint64_t blkStartAddr);

};

