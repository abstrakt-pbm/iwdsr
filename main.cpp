#include <iostream>
#include <fstream>
#include "iwdsr/interlayer/os-core/process/address-space.hpp"
int main() {
    AddressSpace aspace(20 * 1024 * 1024);
    std::fstream file("E:\\blackMetal\\a.out");
    aspace.mmap(10, file, 17* 1024);
    
}