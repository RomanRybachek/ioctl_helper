#ifndef IOCTL_HELPER_H
#define IOCTL_HELPER_H

#include <libloaderapi.h>
#include <windows.h>
#include <ntdef.h>
#include <stdio.h>
#include <ntstatus.h>
#include <vector>
#include <string>
#include <iostream>
#include <winternl.h>
#include <fileapi.h>
#include <iomanip>
#include "utils.h"

typedef NTSTATUS (CALLBACK* NTQUERYDIRECTORYOBJECT)(HANDLE, PVOID, ULONG, BOOLEAN, BOOLEAN, PULONG, PULONG);
typedef NTSTATUS (CALLBACK* NTOPENDIRECTORYOBJECT)(PHANDLE, ACCESS_MASK, POBJECT_ATTRIBUTES);
typedef NTSTATUS (CALLBACK* NTOPENSYMBOLIClINKOBJECT)(PHANDLE, ACCESS_MASK, POBJECT_ATTRIBUTES);
typedef NTSTATUS (CALLBACK* NTQUERYSYMBOLIClINKOBJECT)(HANDLE, PUNICODE_STRING, PULONG);
typedef NTSTATUS (CALLBACK* NTCLOSE)(HANDLE);

typedef struct _OBJECT_DIRECTORY_INFORMATION {
    UNICODE_STRING Name;
    UNICODE_STRING TypeName;
} OBJECT_DIRECTORY_INFORMATION, *POBJECT_DIRECTORY_INFORMATION;

#define DIRECTORY_QUERY 0x0001
typedef std::vector<std::pair<std::wstring, std::wstring>> dir_obj_pairs;
typedef std::vector<std::pair<std::wstring, HANDLE>> opened_device_pairs;
//#define STATUS_MORE_ENTRIES ((NTSTATUS)0x00000105L)

class ioctl_helper
{

public:
    opened_device_pairs handles;

    ioctl_helper();
    ~ioctl_helper();
    dir_obj_pairs enum_directory_objects(std::wstring dir_name);
    void get_device_handle(std::wstring device_name);
    HANDLE open_device(std::wstring name, DWORD dwDesiredAccess);

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

    NTSTATUS (*NtClose)(HANDLE handle);
};

#endif // IOCTL_HELPER_H
