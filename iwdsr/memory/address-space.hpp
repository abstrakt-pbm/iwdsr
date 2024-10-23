#pragma once
#include <cstdint>
#include <vector>
#include <fstream>

enum ASR_PURPOSE {
    CODE,
    DATA,
    BSS,
    STACK,
    HEAP
};

enum ASR_SECURE {
    PROT_EXEC,
    PROT_READ,
    PROT_WRITE,
    PROT_NONE
};

enum ASR_FLAG {
    MAP_FIXED,
    MAP_SHARED,
    MAP_PRIVATE
};

typedef struct {
    std::uint64_t start;
    std::uint64_t end;
    ASR_PURPOSE purpose;
    ASR_SECURE secureMode;
    ASR_FLAG flag;
} AddressSpaceRegion;

class AddressSpace {
    public:
    std::vector<AddressSpaceRegion*> addrSpcRegions;
    int map(std::uint64_t address, char bytes[], uint64_t lenght, std::uint64_t alignment);
    int unmap(void *start, size_t length);
    public:
    

    
}