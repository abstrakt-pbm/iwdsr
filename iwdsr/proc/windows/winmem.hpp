#pragma once
#include "../procmem.hpp"
#include "windows.h"

typedef NTSTATUS(NTAPI* pNtAllocateVirtualMemoryEx)(
    HANDLE,
    PVOID*,
    SIZE_T*,
    ULONG,
    ULONG,
    PVOID,
    ULONG
);

class WinProcMemory : public ProcessMemory {
private:
    HANDLE procHandle;
    HMODULE ntdllHmod;

    pNtAllocateVirtualMemoryEx NtAllocateVirtualMemoryEx;

    void loadNtFunctions();
public:
    WinProcMemory(HMODULE ntdllHandle, HANDLE procHandle);
    virtual uint64_t allocate(uint64_t baseAddress, uint64_t byteCount ,PAGE_SIZE pageSize) override;
    virtual void dealloc(uint64_t addr) override;
    virtual void writeMem(uint64_t baseAddress, int8_t* payload, uint64_t byteCount) override; 
    virtual uint8_t* readMem(uint64_t baseAddress, uint64_t byteCount) override;
};