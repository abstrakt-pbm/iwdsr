#pragma once
#include <cstdint>

enum MemMode {
    NO_ACCESS,
    READ,
    WRITE,
    EXECUTE,
    RW,
    RWE,
    EW,
    ER
};

class LinuxVM {
    public:
    std::uint64_t* pgb;

    public:
    std::uint64_t map(std::uint64_t address, char bytes[], uint64_t lenght, std::uint64_t alignment);
    int unmap();
};