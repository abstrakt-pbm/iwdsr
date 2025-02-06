#include <iostream>
#include "winmem.hpp"
#include "winproc.hpp"
#include <format>

WinProc::WinProc() {
    loadNtFunctions();

    OBJECT_ATTRIBUTES* objAttr = new OBJECT_ATTRIBUTES;
    SECURITY_DESCRIPTOR* secDesc = new SECURITY_DESCRIPTOR;

    ZeroMemory(objAttr, sizeof(OBJECT_ATTRIBUTES));
    ZeroMemory(secDesc, sizeof(SECURITY_DESCRIPTOR));

    InitializeObjectAttributes(objAttr, NULL, 0, NULL, NULL);
    secDesc->Dacl = NULL;
    

    NTSTATUS creationStatus = NtCreateProcessEx(
        &procHandle,
        PROCESS_ALL_ACCESS,
        objAttr,
        GetCurrentProcess(),
        CREATE_SUSPENDED,
        NULL,
        NULL,
        NULL,
        FALSE
    );

    if ( creationStatus == 0 ) {
        std::cout << std::format("HANDLE: {}", procHandle) << std::endl;
        std::cout << std::format("PID: {}", GetProcessId(procHandle)) << std::endl;
    } else {
        std::cerr << std::format("Creating process failed: {}", creationStatus) << std::endl;
    }
    this->mem = new WinProcMemory(ntDllHdl, this->procHandle);

}

void WinProc::loadNtFunctions() {
    ntDllHdl = LoadLibraryA("ntdll.dll");
    if (!ntDllHdl) {
        std::cout << "ntdll not loaded" << std::endl;
    }

    NtCreateProcessEx = (pNtCreateProcessEx)GetProcAddress(ntDllHdl, "NtCreateProcessEx");
    if (!NtCreateProcessEx) {
        std::cerr << "NtCreateProcessEx not found" << std::endl;
    }


}

void WinProc::start() {

}

void WinProc::stop() {
    
}
