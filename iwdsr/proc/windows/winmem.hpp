#pragma once
#include "../procmem.hpp"
#include "windows.h"
#include "winternl.h"
#include "winnt.h"

typedef enum _MEMORY_INFORMATION_CLASS {
    MemoryBasicInformation = 0,
    MemoryWorkingSetInformation = 1,
    MemoryMappedFilenameInformation = 2,
    MemoryRegionInformation = 3,
    MemoryBasicVlmInformation = 4
} MEMORY_INFORMATION_CLASS;

typedef NTSTATUS (NTAPI *pNtQuerySystemInformation)(
    ULONG SystemInformationClass,
    PVOID SystemInformation,
    ULONG SystemInformationLength,
    PULONG ReturnLength
);


typedef NTSTATUS (NTAPI *pNtFreeVirtualMemory)(
    HANDLE ProcessHandle,
    PVOID *BaseAddress,
    PSIZE_T RegionSize,
    ULONG FreeType
);

typedef NTSTATUS(NTAPI* pNtAllocateVirtualMemoryEx)(
    HANDLE,
    PVOID*,
    SIZE_T*,
    ULONG,
    ULONG,
    PVOID,
    ULONG
);
typedef NTSTATUS(NTAPI* pNtUnmapViewOfSection)(
    HANDLE,
    PVOID
);

typedef NTSTATUS(NTAPI* pNtQueryVirtualMemory)(
    HANDLE,
    PVOID,
    MEMORY_INFORMATION_CLASS,
    PVOID,
    SIZE_T,
    PSIZE_T
);

typedef NTSTATUS(NTAPI* pNtTerminateThread)(
    HANDLE ThreadHandle,
    NTSTATUS ExitStatus
);

typedef NTSTATUS(NTAPI* pNtQueryInformationProcess)(
    HANDLE ProcessHandle,
    PROCESSINFOCLASS ProcessInformationClass,
    PVOID ProcessInformation,
    ULONG ProcessInformationLength,
    PULONG ReturnLength
);

class WinProcMemory : public ProcessMemory {
private:
    const uint64_t BASE_OFFSET = 0x10000;

    HANDLE procHandle;
    HMODULE ntdllHmod;

    pNtAllocateVirtualMemoryEx NtAllocateVirtualMemoryEx;
    pNtUnmapViewOfSection NtUnmapViewOfSection;
    pNtQueryVirtualMemory NtQueryVirtualMemory;
    pNtTerminateThread NtTerminateThread;
    pNtQueryInformationProcess NtQueryInformationProcess;
    pNtFreeVirtualMemory NtFreeVirtualMemory;
    pNtQuerySystemInformation NtQuerySystemInformation;

    void loadNtFunctions();
    virtual void clearAddressSpace() override;

public:
    WinProcMemory(HMODULE ntdllHandle, HANDLE procHandle);
    virtual uint64_t allocate(uint64_t baseAddress, uint64_t byteCount ,PAGE_SIZE pageSize) override;
    virtual void dealloc(uint64_t addr) override;
    virtual void writeMem(uint64_t baseAddress, int8_t* payload, uint64_t byteCount) override; 
    virtual uint8_t* readMem(uint64_t baseAddress, uint64_t byteCount) override;
};