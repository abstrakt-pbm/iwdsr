#pragma once
#include "../proc.hpp"

class MockProcess : public Process {
    public:
    MockProcess();
    virtual void createMainThread() override;
    virtual void start() override;
    virtual void stop() override;
};