#pragma once
#include <cstdint>

enum PAGE_SIZE {
    KB_4 = 4096,
    KB_16 = 16384,
    KB_64 = 65536,
    MB_2 = 1048576,
    GB_1 = 1073741824
};

PAGE_SIZE allignmentToPageSize(uint64_t allignment);

class ProcessMemory {
    virtual void clearAddressSpace() = 0;
public:
    virtual uint64_t allocate(uint64_t baseAddress, uint64_t byteCount ,PAGE_SIZE pageSize) = 0;
    virtual void dealloc(uint64_t addr) = 0;
    virtual void writeMem(uint64_t baseAddress, int8_t* payload, uint64_t byteCount) = 0; 
    virtual int8_t* readMem(uint64_t baseAddress, uint64_t byteCount) = 0; 

};


