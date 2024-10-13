#include "address-space.hpp"

#define STACK_SIZE 1 * 1024 * 1024

AddressSpace::AddressSpace(unsigned int* programSpace) {
    this->programSpace = programSpace;
    this->stack = new unsigned int[STACK_SIZE];
};

AddressSpace::~AddressSpace() {
    delete[] programSpace;
    delete[] stack;
}
