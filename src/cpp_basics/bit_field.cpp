#include <iostream>
#include <bitset>

using namespace std; 

struct S1
{
    int8_t b1:2;
    uint8_t :4;
    uint8_t b2:2;
};

struct S2
{
    // 3 bits: value of b1
    // 2 bits: unused
    // 3 bits: unused (padding)
    // 6 bits: value of b2
    // 1 bit : unused
    // 2 bits: value of b3
    uint8_t b1:3, :2,  b2:5, :1, b3:2;
};

struct S3
{
    // 3 bits: value of b1
    // 6 bits: unused
    // 7 bits: unused (padding)
    // 5 bits: value of b2
    // 1 bit : unused
    // 2 bits: value of b3
    uint8_t b1:3, :6,  b2:5, :1, b3:2;
};

template<typename T>
void printBits(T s) {
    uint8_t* p = reinterpret_cast<uint8_t*>(&s);
    for(size_t i=0; i<sizeof(T); i++) {
        bitset<8> b{*p++};
        cout << b << " ";
    }
    cout << endl;
}

int main()
{
    cout << sizeof(S1) << endl; // usually prints 1
    cout << sizeof(S2) << endl; // usually prints 2
    cout << sizeof(S3) << endl; // usually prints 3

    {
        S1 s{};
        s.b1 = -2;
        s.b2 = 3;
        printBits(s);
    }

    {
        S2 s{};
        s.b1 = 0b111;
        s.b2 = 0b11111;
        s.b3 = 0b11;
        printBits(s);
    }

    {
        S3 s{};
        s.b1 = 0b111;
        s.b2 = 0b11111;
        s.b3 = 0b11;
        printBits(s);    
    }

    return 0;
}