#pragma once

#include <cstdint>
#include <unordered_map>
#include <string>
#include <fstream>

#include "procmem.hpp"
#include "procthread.hpp"

class Process {
protected:
    ProcessMemory* mem;
    ProcessThread* mainThread;

public:
    ProcessMemory* getMemory();

    virtual void createMainThread() = 0;
    virtual void start() = 0;
    virtual void stop() = 0;
    
};