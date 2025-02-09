#include "memblock.hpp"

uint64_t calculatePageCount(uint64_t byteSize, uint64_t alignment) {
    uint64_t pageCount = 0;
    if ( alignment > 0) {
        pageCount = byteSize / alignment;
        if ( byteSize % alignment != 0) {
            pageCount += 1;
        }
    } 
    return pageCount;
}

MemoryBlockList::MemoryBlockList(uint64_t lefAddrRange, uint64_t rightAddrRange) {
   this->leftAddrRange = leftAddrRange; 
   this->rightAddrRange = rightAddrRange;
   this->root = new MemoryBlock(MemState::FREE, leftAddrRange, rightAddrRange, 0);
}

MemoryBlock* MemoryBlockList::find( uint64_t addr ) {
    MemoryBlock* currentMemBlk = root;
    while ( currentMemBlk != nullptr ) {
        if ( *currentMemBlk == addr ) {
            break;
        }
        currentMemBlk = currentMemBlk->nextBlk;
    }
    return currentMemBlk;
}

void MemoryBlockList::append(MemoryBlock* blkToAdd) {
    if (blkToAdd == nullptr) {
        return;
    }
    MemoryBlock* currentBlk;

}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



bool MemoryBlock::operator < (uint64_t addr) {
    return addr < startAddr ? true : false;
}

bool MemoryBlock::operator > (uint64_t addr) {
    return addr > startAddr ? true : false;
}

bool MemoryBlock::operator == (uint64_t addr) {
    return addr >= startAddr && addr <= endAddr;
}