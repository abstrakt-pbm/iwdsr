#pragma once
#include "../memory/memory.hpp"

class ProcThread {
    private:
    Memory* procMem;

    public:
    ProcThread(const ELF& gameElf,);
    void run();
};