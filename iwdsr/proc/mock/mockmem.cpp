#include "mockmem.hpp"
#include <iostream>
#include <format>

uint64_t MockMemory::allocate( uint64_t baseAddress, uint64_t byteCount ,P_SIZE pageSize ) {
    std::cout << std::format( "Allocated: {:X} - {:X}",baseAddress, baseAddress + byteCount ) << std::endl; 
    return 0;
}

void MockMemory::dealloc(uint64_t addr) {

}

void MockMemory::writeMem( uint64_t baseAddress, int8_t* payload, uint64_t byteCount ) {
    std::cout << std::format( "Write to: {:X} bytes: {}", baseAddress, byteCount ) << std::endl; 
}

int8_t* MockMemory::readMem(uint64_t baseAddress, uint64_t byteCount) {
    std::cout << std::format( "Read from: {:X} bytes: {}", baseAddress, byteCount) << std::endl;
    return nullptr;
}