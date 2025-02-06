#pragma once
#pragma comment(lib, "ntdll.lib")

#include "windows.h"
#include <winternl.h>
#include "../proc.hpp"


typedef NTSTATUS (NTAPI *pNtCreateProcessEx)(
    PHANDLE ProcessHandle, 
    ACCESS_MASK DesiredAccess, 
    POBJECT_ATTRIBUTES ObjectAttributes, 
    HANDLE ParentProcess, 
    ULONG Flags, 
    HANDLE SectionHandle, 
    HANDLE DebugPort, 
    HANDLE ExceptionPort, 
    ULONG JobMemberLevel
);


class WinProc : public Process {
    private:
    HMODULE ntDllHdl;
    HANDLE procHandle;
    pNtCreateProcessEx NtCreateProcessEx;

    void loadNtFunctions();

    public:
    WinProc();
    virtual void start() override;
    virtual void stop() override;
};