#pragma once

#include <cstdint>
#include <unordered_map>
#include <string>
#include <fstream>

#include "procmem.hpp"

class Process {
protected:
    ProcessMemory* mem;

public:
    ProcessMemory* getMemory();
    virtual void start() = 0;
    virtual void stop() = 0;
    
};