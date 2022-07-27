#include <string>
#include <stdexcept>
#include <vector>
#include <cstdint>

#include <iostream>
#include <fstream>
#include <exception>
#include <cstring>
#include <iomanip>
#include <algorithm>
#include "twisted-edwards/Point.h"
#include "uint256.h"
using namespace std;
using namespace NTL;
void convertVectorToBytes(const std::vector<bool>& v, unsigned char* bytes);
void convertBytesVectorToVector(const std::vector<unsigned char>& bytes, std::vector<bool>& v);

std::vector<unsigned char> convertIntToVectorLE(const uint64_t val_int) {
    std::vector<unsigned char> bytes;

    for(size_t i = 0; i < 8; i++) {
        bytes.push_back(val_int >> (i * 8));
    }

    return bytes;
}
std::vector<unsigned char> convertZZToVectorLE(NTL::ZZ val_int) {
    std::vector<unsigned char> bytes;

    while(val_int != 0)
    {
        NTL::ZZ temp = conv<ZZ>(val_int % 256);
        val_int /= 256;
        bytes.push_back(conv<uint>(temp));
    }
    return bytes;
}
int calcDecimalValue(unsigned char c)
{
    std::vector<bool> bits;
    while(c)
    {
        c % 2 == 0 ? bits.push_back(false): bits.push_back(true);
        c /= 2;
    }
    int result=0,temp = 1;
    for(int i = 0 ;i < bits.size();i ++)
    {
        if(bits[i]) result += temp;
        temp *= 2;
    }
    return result;
}
NTL::ZZ convertUint256ToZZ(uint256 x)
{
    NTL::ZZ res = conv<ZZ>(0);
    NTL::ZZ  a = conv<ZZ>(1);
    for(int i = 0;i < 32; i++)
    {
        int t = calcDecimalValue(x.data[i]);
        res = res + a * t;
        t = t * 256;
    }
    return res;
}