#ifndef IOCTL_HELPER_H
#define IOCTL_HELPER_H

#include <libloaderapi.h>
#include <windows.h>
#include <ntdef.h>

typedef NTSTATUS (CALLBACK* NTQUERYDIRECTORYOBJECT)(HANDLE, PVOID, ULONG, BOOLEAN, BOOLEAN, PULONG, PULONG);
typedef NTSTATUS (CALLBACK* NTOPENDIRECTORYOBJECT)(PHANDLE, ACCESS_MASK, POBJECT_ATTRIBUTES);
typedef NTSTATUS (CALLBACK* NTOPENSYMBOLIClINKOBJECT)(PHANDLE, ACCESS_MASK, POBJECT_ATTRIBUTES);
typedef NTSTATUS (CALLBACK* NTQUERYSYMBOLIClINKOBJECT)(HANDLE, PUNICODE_STRING, PULONG);

class ioctl_helper
{
public:
    ioctl_helper();
    void enum_directories();
private:
    HMODULE ntdll_hanlde;

    NTSTATUS  (*NtQueryDirectoryObject) (
        HANDLE  DirectoryHandle,
        PVOID   Buffer,
        ULONG   Length,
        BOOLEAN ReturnSingleEntry,
        BOOLEAN RestartScan,
        PULONG  Context,
        PULONG  ReturnLength);

    NTSTATUS (*NtOpenDirectoryObject)(
        PHANDLE            DirectoryHandle,
        ACCESS_MASK        DesiredAccess,
        POBJECT_ATTRIBUTES ObjectAttributes);

    NTSTATUS (*NtOpenSymbolicLinkObject)(
        PHANDLE            LinkHandle,
        ACCESS_MASK        DesiredAccess,
        POBJECT_ATTRIBUTES ObjectAttributes);

    NTSTATUS (*NtQuerySymbolicLinkObject)(
        HANDLE          LinkHandle,
        PUNICODE_STRING LinkTarget,
        PULONG          ReturnedLength);
};

#endif // IOCTL_HELPER_H
