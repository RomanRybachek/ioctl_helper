#include "ioctl_helper.h"

ioctl_helper::ioctl_helper()
{
    this->ntdll_hanlde = LoadLibrary(L"ntdll.dll");
    if (this->ntdll_hanlde == NULL)
        exit(1);
    this->NtQueryDirectoryObject = (NTQUERYDIRECTORYOBJECT)GetProcAddress(this->ntdll_hanlde, "NtQueryDirectoryObject");
    this->NtOpenDirectoryObject = (NTOPENDIRECTORYOBJECT)GetProcAddress(this->ntdll_hanlde, "NtOpenDirectoryObject");
    this->NtOpenSymbolicLinkObject = (NTOPENSYMBOLIClINKOBJECT)GetProcAddress(this->ntdll_hanlde, "NtOpenSymbolicLinkObject");
    this->NtQuerySymbolicLinkObject = (NTQUERYSYMBOLIClINKOBJECT)GetProcAddress(this->ntdll_hanlde, "NtQuerySymbolicLinkObject");

    if (this->NtOpenDirectoryObject == NULL || \
        this->NtQueryDirectoryObject == NULL || \
        this->NtOpenSymbolicLinkObject == NULL || \
        this->NtQuerySymbolicLinkObject == NULL)
        exit(2);

}
