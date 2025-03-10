#pragma once
#include <cstdint>


class ThreadLocalStorage {
    private:
    uint64_t baseAddr;
    uint64_t lenght;
    public:
    ThreadLocalStorage();
};

class ProcessThread {
    private:
    ThreadLocalStorage* tls;
    uint64_t stackAddr;
    public:
     

};