#pragma once
#include <fstream>


class AddressSpace {
    private:

    unsigned int* programSpace;
    unsigned int* stack;

    public:
    AddressSpace(unsigned int* programSpace);
    ~AddressSpace();

    unsigned int* getStackBase();
    unsigned int* getDataStart();
    unsigned int* getBssStart();
};

