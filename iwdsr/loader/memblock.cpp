#include "memblock.hpp"

bool MemoryBlock::operator < (uint64_t addr) {
    return addr < startAddr ? true : false;
}

bool MemoryBlock::operator > (uint64_t addr) {
    return addr > startAddr ? true : false;
}

bool MemoryBlock::operator = (uint64_t addr) {
    return addr >= startAddr && addr <= endAddr;
}