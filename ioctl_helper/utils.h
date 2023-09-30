#ifndef UTILS_H
#define UTILS_H

#include <iostream>
#include <string>
#include <windows.h>
#include <iomanip>

std::string GetLastErrorAsString();
std::wstring toHex(size_t val);

template <typename T>
extern std::wstring toHex(T val){

    std::wstringstream stream;
    stream << std::hex << val;
    return stream.str();
}

template <typename T>
extern size_t hexToDec(T val){

    size_t ret;
    std::wstringstream stream;

    stream << std::hex << val;
    stream >> ret;
    return ret;
}

template <typename T>
extern size_t toDec(T val){

    size_t ret;
    std::wstringstream stream;

    stream << std::dec << val;
    stream >> ret;
    return ret;
}
#endif // UTILS_H
