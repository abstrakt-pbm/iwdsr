#pragma once
#include "../proc.hpp"

class PosixProc : public Process {
    private:
    pid_t pid;
    public:
    PosixProc();
    virtual void start() override;
    virtual void stop() override;
};