#include "virtual-memory.hpp"

VMBlock::VMBlock(VAddr start, VAddr end) {
    this->start = start;
    this->end = end;
    blockSpace = new uint64_t[ end - start ];

}

[[nodiscard]] void* VMBlock::getBlockSpace() const {
    return blockSpace;
}

[[nodiscard]] bool VMBlock::isVAddrInBlock(VAddr addr) const {
    return (addr >= start) & (addr < end);
}

uint64_t* VirtualMemory::alloc(VAddr start, uint64_t lenght) {
    for( auto vmblock : vmBlocks ) {
        
    }
    return 0;
}

 int VirtualMemory::unWrap( MemoryMap processMemoryMap ) {
    return 0;   
}

VirtualMemory::VirtualMemory(uint64_t startVspace, uint64_t endVspace) {
    this->startVspace = startVspace;
    this->endVspace = endVspace;
}


