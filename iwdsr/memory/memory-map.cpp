#include "memory-map.hpp"


MemoryMap::MemoryMap( uint64_t permissionedStartVadder, uint64_t permissionedEndVadder ) {
    this->permissionedStartVaddr = permissionedStartVadder;
    this->permissionedEndVaddr = permissionedEndVaddr;
    this->regions = std::vector<MemoryRegion>();
};

void MemoryMap::addMemoryRegion( uint64_t startVaddr, uint64_t lenght, char payload[], RegionPurpose regionPurpose, RegionPermission regionPermission ) {
    if ( (startVaddr >= permissionedStartVaddr) & (startVaddr + lenght <= permissionedEndVaddr) ) {
        regions.push_back(MemoryRegion(startVaddr, lenght, payload, regionPurpose, regionPermission ));
    };
    
}