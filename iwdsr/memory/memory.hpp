#pragma once
#include <cstdint>


class Memory {
    private:
    public:
    uint64_t* map( uint64_t startAddr, char bytes[], uint64_t lenght );
    uint64_t* getPageTableBase();
    uint64_t* getBaseIp();
    uint64_t* getStackBase();
    uint64_t* getStackHead();

};
