#pragma once
#include <cstdint>
#include <vector>


typedef struct {
    uint64_t start;
    uint64_t end;
    RegionPurpose purpose;
} AddressSpaceRegion;

enum RegionPurpose {
    TEXT,
    DATA,
    BSS,
    Stack,
    HEAP,
}; 

class AddressSpace {
    private:
    std::vector<AddressSpaceRegion> regions;
    public:
    void allocateRegion(uint64_t start, uint64_t end, RegionPurpose purpose);
    void freeRegion(uint64_t start);

};