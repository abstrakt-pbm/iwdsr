#include <iostream>
#include "procmem.hpp"

uint64_t ProcessMemory::getBaseOffset() {
    return this->baseOffset;
}


void ProcessMemory::setBaseOffset( uint64_t baseOffset ) {
    this->baseOffset = baseOffset;
}