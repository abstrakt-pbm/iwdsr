#pragma once
#include "../procmem.hpp"

class MockMemory : public ProcessMemory {
    public:
    virtual uint64_t allocate(uint64_t baseAddress, uint64_t byteCount ,P_SIZE pageSize) override;
    virtual void dealloc(uint64_t addr) override;
    virtual void writeMem(uint64_t baseAddress, int8_t* payload, uint64_t byteCount) override; 
    virtual int8_t* readMem(uint64_t baseAddress, uint64_t byteCount) override;
};