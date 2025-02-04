#include "mem.hpp"

WinProcMemory::WinProcMemory() {
    
}

uint64_t WinProcMemory::allocatePages(uint64_t baseAddress, PAGE_SIZE pageSize, uint64_t pageCount) {
    
    return 0;
}

uint64_t WinProcMemory::allocatePage(uint64_t baseAddress, PAGE_SIZE pageSize) {
    return 0;
}



uint64_t WinProcMemory::allocate(uint64_t byteCount) {
    return 0;
}

void WinProcMemory::dealloc(uint64_t) {

}

void WinProcMemory::mapRawBytes(uint64_t baseAddress, uint8_t* payload, uint64_t byteCount) {

}
