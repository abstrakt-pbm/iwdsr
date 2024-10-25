#pragma once

class ProcThread {
    private:
    uint64_t* baseSp;
    uint64_t* headSp;
    uint64_t* startIp;
    uint64_t* pageTableAddr; 
    public:
    ProcThread(const ELF& gameElf);
    void run();
};