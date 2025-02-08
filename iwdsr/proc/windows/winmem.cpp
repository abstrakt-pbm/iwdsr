#pragma comment(lib, "ntdll.lib")

#include <windows.h>
#include <format>
#include <iostream>
#include "winmem.hpp"



PAGE_SIZE allignmentToPageSize(uint64_t allignment) {
    return static_cast<PAGE_SIZE>(allignment);
}

WinProcMemory::WinProcMemory(HMODULE ntdllHandle, HANDLE procHandle) {
    this->procHandle = procHandle; 
    this->ntdllHmod = ntdllHandle;
    loadNtFunctions();
    clearAddressSpace();
}

uint64_t WinProcMemory::allocate(uint64_t baseAddress, uint64_t byteCount ,PAGE_SIZE pageSize) {
    MEMORY_BASIC_INFORMATION mbi;
    VirtualQueryEx(
        procHandle,
        (LPCVOID)baseAddress,
        &mbi,
        sizeof(MEMORY_BASIC_INFORMATION)
    );
    std::cout << mbi.State << std::endl;
    

    SIZE_T regionSize = byteCount;
    uint64_t addrWoffset = baseAddress + BASE_OFFSET;
    NTSTATUS allocationResult = NtAllocateVirtualMemoryEx(
        procHandle,
        (PVOID*)(&addrWoffset),
        &regionSize,
        MEM_COMMIT | MEM_RESERVE,
        PAGE_READWRITE,
        NULL,
        0
    );

    if ( allocationResult != 0 ) {
        std::cerr << "Allocation Erorr: " << std::hex << allocationResult << std::endl;
    } else {
        std::cout << "Allocation Success: " << allocationResult << std::endl;
    }

    return 0;
}





void WinProcMemory::dealloc(uint64_t) {
    
}

void WinProcMemory::writeMem(uint64_t baseAddress, int8_t* payload, uint64_t byteCount) {
}

uint8_t* WinProcMemory::readMem(uint64_t baseAddress, uint64_t byteCount) {
    return 0;
}

void WinProcMemory::clearAddressSpace() {
    PVOID currentAddress = NULL;
    MEMORY_BASIC_INFORMATION mbi;
    SIZE_T bytesReturned;
    while ( !NtQueryVirtualMemory(procHandle, currentAddress, MEMORY_INFORMATION_CLASS::MemoryBasicInformation, &mbi, sizeof(mbi), &bytesReturned) ) {
        NtUnmapViewOfSection(procHandle, mbi.BaseAddress);
        currentAddress = (PVOID)((BYTE*)mbi.BaseAddress + mbi.RegionSize);
    }
}


void WinProcMemory::loadNtFunctions() {
    NtAllocateVirtualMemoryEx = (pNtAllocateVirtualMemoryEx)GetProcAddress(ntdllHmod, "NtAllocateVirtualMemoryEx");
    if ( !NtAllocateVirtualMemoryEx ) {
        std::cerr << "NtAllocateVirtualMemoryEx not found"  << std::endl;
    } 
    
    NtUnmapViewOfSection = (pNtUnmapViewOfSection)GetProcAddress(ntdllHmod, "NtUnmapViewOfSection");
    if ( !NtUnmapViewOfSection ) {
        std::cerr << "NtUnmapViewOfSection not found" << std::endl;
    }

    NtQueryVirtualMemory = (pNtQueryVirtualMemory)GetProcAddress(ntdllHmod, "NtQueryVirtualMemory");
    if ( !NtQueryVirtualMemory ) {
        std::cerr << "NtQueryVirtualMemory not found" << std::endl;
    }

    NtTerminateThread = (pNtTerminateThread)GetProcAddress(ntdllHmod, "NtTerminateThread");
    if ( !NtTerminateThread ) {
        std::cerr << "NtTerminateThread not found" << std::endl;
    }

    NtQueryInformationProcess = (pNtQueryInformationProcess)GetProcAddress(ntdllHmod, "NtQueryInformationProcess"); 
    if( !NtQueryInformationProcess ) {
        std::cerr << "NtQueryInformationProcess not found" << std::endl;
    }

    NtFreeVirtualMemory =  (pNtFreeVirtualMemory)GetProcAddress(ntdllHmod, "NtFreeVirtualMemory");
    if ( !NtFreeVirtualMemory ) {
        std::cerr << "NtFreeVirtualMemory not found";
    }

    NtQuerySystemInformation = (pNtQuerySystemInformation)GetProcAddress(ntdllHmod, "NtQuerySystemInformation");
    if ( !NtQuerySystemInformation ) {
        std::cerr << "NtQuerySystemInformation not found";
    }
}