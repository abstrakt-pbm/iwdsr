#pragma once
#include <cstdint>


typedef struct Registers {

};

class ThreadLocalStorage {
    private:
    public:
};

class ProcessThread {
    private:
    Registers regs;
    ThreadLocalStorage* tls;
    uint64_t stackAddr;
    public:

};