#pragma once
#include <cstdint>

class VirtualMemory {
    private:
    uint64_t* pdg;
    public:
    int map(uint64_t startVaddr, char payload[]);
};