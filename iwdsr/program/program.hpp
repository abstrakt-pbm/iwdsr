#pragma once
#include <cstdint>
#include <fstream>
#include "mem.hpp"

enum MemoryAccess {
    FILE_READ,
    FILE_WRITE,
    FILE_EXECUTE,
    FILE_RW,
    FILE_RE,
    FILE_RWE,
    FILE_WE
};

class Program {
protected:
    ProgramMemory* mem;
    bool loadSection();
    bool loadSymbol();
public:
    bool writeMemory(); 
    int64_t readMemory();
    bool mapRawBytes(uint64_t baseAddress, uint8_t* rawPayload, uint8_t payloadSize);
    bool mapFile(uint64_t baseAddress, std::ifstream fileToMap, uint8_t byteCount, MemoryAccess memAccess);
    ProgramMemory* getMemory();
    
};

class WinProgram : public Program {
private:
    
public:
    WinProgram();
    bool writeMemory(); 
    int64_t readMemory();
    
};
