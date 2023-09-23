#include "ioctl_helper.h"
#include <algorithm>

ioctl_helper::ioctl_helper()
{
    this->ntdll_hanlde = LoadLibrary(L"ntdll.dll");
    if (this->ntdll_hanlde == NULL)
        exit(1);
    this->NtQueryDirectoryObject = (NTQUERYDIRECTORYOBJECT)GetProcAddress(this->ntdll_hanlde, "NtQueryDirectoryObject");
    this->NtOpenDirectoryObject = (NTOPENDIRECTORYOBJECT)GetProcAddress(this->ntdll_hanlde, "NtOpenDirectoryObject");
    this->NtOpenSymbolicLinkObject = (NTOPENSYMBOLIClINKOBJECT)GetProcAddress(this->ntdll_hanlde, "NtOpenSymbolicLinkObject");
    this->NtQuerySymbolicLinkObject = (NTQUERYSYMBOLIClINKOBJECT)GetProcAddress(this->ntdll_hanlde, "NtQuerySymbolicLinkObject");
    this->NtClose = (NTCLOSE)GetProcAddress(this->ntdll_hanlde, "NtClose");

    if (this->NtOpenDirectoryObject == NULL || \
        this->NtQueryDirectoryObject == NULL || \
        this->NtOpenSymbolicLinkObject == NULL || \
        this->NtQuerySymbolicLinkObject == NULL)
        exit(2);
}

ioctl_helper::~ioctl_helper(){

}

dir_obj_pairs ioctl_helper::enum_directory_objects(std::wstring dir_name)
{
    OBJECT_ATTRIBUTES 			obj_attr;
    UNICODE_STRING 				dir_str;
    dir_obj_pairs 				dir_vector;

    HANDLE 		dir_handle 	= NULL;
    WCHAR 		*str_buf	= (WCHAR *)malloc(dir_name.length() * 2 + 2);

    wcscpy(str_buf, dir_name.data());
    dir_str.Buffer 			= str_buf;
    dir_str.Length 			= wcslen(dir_str.Buffer) * 2;
    dir_str.MaximumLength 	= dir_str.Length + 2;

    InitializeObjectAttributes(&obj_attr, &dir_str, 0, NULL, NULL);

    this->NtOpenDirectoryObject(&dir_handle, 0x0001, &obj_attr);

    NTSTATUS query_status 						= STATUS_MORE_ENTRIES;
    ULONG dir_buffer_size 						= 0x100;
    ULONG context 								= 0;
    ULONG prev_context 							= 0;
    ULONG ret_length 							= 0;
    bool first_time 							= true;
    POBJECT_DIRECTORY_INFORMATION dir_buffer 	= (POBJECT_DIRECTORY_INFORMATION)malloc(dir_buffer_size);

    while (query_status == STATUS_MORE_ENTRIES){

        query_status = this->NtQueryDirectoryObject(dir_handle, dir_buffer, dir_buffer_size, false, first_time, &context, &ret_length);

        while (dir_buffer->Name.Buffer == NULL){
            context = prev_context;
            dir_buffer_size *= 2;
            dir_buffer = (POBJECT_DIRECTORY_INFORMATION)realloc(dir_buffer, dir_buffer_size);
            query_status = this->NtQueryDirectoryObject(dir_handle, dir_buffer, dir_buffer_size, false, first_time, &context, &ret_length);
        }

        first_time = false;

        POBJECT_DIRECTORY_INFORMATION dir_iter = dir_buffer;

        while (dir_iter->Name.Buffer != NULL){
            std::pair<std::wstring, std::wstring> pair_name_type(dir_iter->Name.Buffer, dir_iter->TypeName.Buffer);
            dir_vector.push_back(pair_name_type);
            dir_iter++;
        }
        prev_context = context;
    }
    free(str_buf);
    free(dir_buffer);
    std::sort(dir_vector.begin(), dir_vector.end());
    this->NtClose(dir_handle);
    return dir_vector;
}

HANDLE ioctl_helper::open_device(std::wstring name, DWORD dwDesiredAccess){

    DWORD 	dwShareMode 		= FILE_SHARE_DELETE | FILE_SHARE_READ | FILE_SHARE_WRITE;
    HANDLE 	opened_device		= NULL;

    opened_device = CreateFileW(name.c_str(), dwDesiredAccess, dwShareMode, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
    if ((long long)opened_device > 0){
        std::pair<std::wstring, HANDLE> new_opened_device_pair(name, opened_device);
        this->handles.push_back(new_opened_device_pair);
    }
    return opened_device;
}
