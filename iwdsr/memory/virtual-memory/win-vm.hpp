#pragma once
#include <fstream>
#include <cstdint>

class WinVirtualMemory { 
    public: 
    public:
    WinVirtualMemory();
    std::uint64_t mmap(std::uint64_t startAddr, std::uint64_t bytesFromFile, int protectionMode , int flags, std::fstream file, std::uint64_t offsetInFile);
    
}