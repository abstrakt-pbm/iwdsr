#pragma comment(lib, "ntdll.lib")

#include <windows.h>
#include <iostream>
#include "winmem.hpp"



PAGE_SIZE allignmentToPageSize(uint64_t allignment) {
    return static_cast<PAGE_SIZE>(allignment);
}

WinProcMemory::WinProcMemory(HMODULE ntdllHandle, HANDLE procHandle) {
    this->procHandle = procHandle; 
    this->ntdllHmod = ntdllHandle;
    loadNtFunctions();
}

uint64_t WinProcMemory::allocate(uint64_t baseAddress, uint64_t byteCount ,PAGE_SIZE pageSize) {
 

    return 0;
}





void WinProcMemory::dealloc(uint64_t) {
    
}

void WinProcMemory::writeMem(uint64_t baseAddress, int8_t* payload, uint64_t byteCount) {
}

uint8_t* WinProcMemory::readMem(uint64_t baseAddress, uint64_t byteCount) {
    return 0;
}


void WinProcMemory::loadNtFunctions() {
    NtAllocateVirtualMemoryEx = (pNtAllocateVirtualMemoryEx)GetProcAddress(ntdllHmod, "NtAllocateVirtualMemoryEx");
    if ( !NtAllocateVirtualMemoryEx ) {
        std::cerr << "NtAllocateVirtualMemoryEx not found: "  << std::endl;
    } 
}