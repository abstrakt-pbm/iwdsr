#pragma once
#include <cstdint>
#include <vector>

enum RegionPurpose {
    TEXT,
    DATA,
    BSS,
    STACK,
    HEAP,
};

enum RegionPermission {
    READ,
    WRITE,
    EXECUTE,
    RW,
    RWE,
    RE,
    WE
};


typedef struct {
    uint64_t start;
    uint64_t lenght;
    char* payload;
    RegionPurpose purpose;
    RegionPermission permission;
} MemoryRegion;

class MemoryMap {
    private:
    uint64_t permissionedStartVaddr;
    uint64_t permissionedEndVaddr;
    std::vector<MemoryRegion> regions; //change to rb tree
    public:
    MemoryMap( uint64_t permissionedStartVadder, uint64_t permissionedEndVadder );
    void addMemoryRegion( uint64_t startVaddr, uint64_t lenght, char payload[], RegionPurpose regionPurpose, RegionPermission regionPermission );
};