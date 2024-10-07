#include "address-space.hpp"


AddressSpace::AddressSpace(unsigned int size) {
    space = new int[size];
}


void AddressSpace::extend(unsigned int bytes) {
    int* newSpace = new int[ size + bytes ];
    for (auto i = 0; i < size ; i++) {
        newSpace[i] = space[i];
    }
    size += bytes;
    delete[] space;
    space = newSpace;
}

void AddressSpace::mmap(unsigned int startAddress, std::fstream fileToMap, unsigned int byteSize) {
    if (fileToMap.is_open()) {
        fileToMap.read(reinterpret_cast<char*>(space + startAddress), byteSize);
    };

}