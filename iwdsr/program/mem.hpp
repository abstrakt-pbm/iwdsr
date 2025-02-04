#pragma once
#include <cstdint>

enum PAGE_SIZE {
    KB_4 = 4096,
    KB_16 = 16384,
    KB_64 = 65536,
    MB_2 = 1048576,
    GB_1 = 1073741824
};

PAGE_SIZE allignmentToPageSize(uint64_t allignment) {
    return static_cast<PAGE_SIZE>(allignment);
}

class ProgramMemory {
public:
    virtual uint64_t allocatePage(uint64_t baseAddress, PAGE_SIZE pageSize) = 0;
    virtual uint64_t allocatePages(uint64_t baseAddress, PAGE_SIZE pageSize, uint64_t pageCount) = 0;
    virtual uint64_t allocate(uint64_t byteCount) = 0;
    virtual void dealloc(uint64_t) = 0;
    virtual void mapRawBytes(uint64_t baseAddress, uint8_t* payload, uint64_t byteCount) = 0; 
};

class WinProcMemory : public ProgramMemory {
private:
public:
    WinProcMemory();
    virtual uint64_t allocatePage(uint64_t baseAddress, PAGE_SIZE pageSize) override;
    virtual uint64_t allocatePages(uint64_t baseAddress, PAGE_SIZE pageSize, uint64_t pageCount) override;
    virtual uint64_t allocate(uint64_t byteCount) override;
    virtual void dealloc(uint64_t) override;
    virtual void mapRawBytes(uint64_t baseAddress, uint8_t* payload, uint64_t byteCount) override;
};