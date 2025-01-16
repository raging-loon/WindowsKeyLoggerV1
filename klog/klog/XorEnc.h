#ifndef XOR_ENC_H_
#define XOR_ENC_H_

#include <string>
#include <vector>
#include <array>
#include "types.h"

constexpr unsigned int XOR_CONSTANT = 131;


std::vector<int> xorEncryptPayload(const KeyBuffer& buffer);

std::string xorDecryptString(const char* str);


template<std::size_t N>
consteval auto CT_XOR_ENC_STRING(const char(&str)[N])
{
    std::array<char,N> enc = {};
    for (std::size_t i = 0; i < N - 1; ++i)
        enc[i] = str[i] ^ XOR_CONSTANT;
    enc[N - 1] = 0;
    return enc;
}


#endif // XOR_ENC_H_