#pragma once
#include <fstream>


class AddressSpace {
    private:
    unsigned int size;
    int* space;
    int* stack;
    void extend(unsigned int bytes);
    public:
    AddressSpace(unsigned int size);
    void mmap(unsigned int startAddress, std::fstream& fileToMap, unsigned int byteSize);
};

