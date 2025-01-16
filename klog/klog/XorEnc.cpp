#include "XorEnc.h"
#include <string>
#include <random>

std::vector<int> xorEncryptPayload(const KeyBuffer& buffer)
{
    std::random_device rd;
    std::mt19937 engine(rd());
    std::uniform_int_distribution<> gen(1);
    std::vector<int> out{};
    out.resize(buffer.size());
    
    for (int i = 0; i < buffer.size(); i++)
    {
       int random = gen(engine) * gen(engine) & 0xfffff00;
       out[i] = (buffer[i] ^ XOR_CONSTANT) | random;
    }
    return out;
}

std::string xorDecryptString(const char* str)
{
    std::string out{};
    
    while (*str)
    {
        out += *str ^ XOR_CONSTANT;
        str++;
    }

    return out;

}

const char* xorDecryptCTStr(const char* str)
{
    std::string out{};

    while (*str)
    {
        out += *str ^ XOR_CONSTANT;
        str++;
    }

    return out.c_str();
}
