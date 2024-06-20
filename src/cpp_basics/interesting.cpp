#include <chrono>
#include <iostream>
#include <memory>
#include <mutex>
#include <thread>
#include <string>
#include <vector>
#include <map>
#include <type_traits>
#include <typeinfo>
#include <cxxabi.h>
#include <cstdlib>

namespace test1 {
struct A {
    A(int&& n) { std::cout << "rvalue overload, n=" << n << "\n"; }
    A(int& n)  { std::cout << "lvalue overload, n=" << n << "\n"; }
};

template<class T, class U>
std::unique_ptr<T> make_unique1(U&& u)
{
    return std::unique_ptr<T>(new T(std::forward<U>(u)));
}

void test1() {
    auto p1 = make_unique1<A>(2); // rvalue
    int i = 1;
    auto p2 = make_unique1<A>(i); // lvalue
}
}

void test2() {
    std::string s1("1");
    std::string&& s1_rvr = std::move(s1); // same as static_cast<std::string&&>(s1)
    s1.append("m");
    std::cout << s1 << "|" << s1_rvr << std::endl;
    std::cout << static_cast<const void*>(s1.data()) << "|" << static_cast<const void*>(s1_rvr.data()) << std::endl;

    std::string s2("2");
    std::string s2_mv = std::move(s2);
    s2.append("m"); // With a few exceptions (smart pointers, for instance), moved-from objects are left in a valid but unspecified state. 
    std::cout << s2 << "|" << s2_mv << std::endl;
    std::cout << static_cast<const void*>(s2.data()) << "|" << static_cast<const void*>(s2_mv.data()) << std::endl;
}

void test3()
{
    struct A {
        int8_t a;
        int32_t b;
        int64_t c;
    };
    struct B {
        int64_t c;
        int32_t b;
        int8_t a;
    };
    struct C {
        B a;
        int8_t b;
        int8_t c;
    };
    struct __attribute__ ((packed)) D {
        int64_t c;
        int32_t b;
        int8_t a;
    };
    struct alignas(max_align_t) E {
        int32_t b;
        int64_t c;
        int8_t a;
    };

    std::cout << alignof(A) << ", " << sizeof(A) << std::endl; //  8, 16 <- 1 + (3) + 4 + 8
    std::cout << alignof(B) << ", " << sizeof(B) << std::endl; //  8, 16 <- 8 + 4 + 1 + (3)
    std::cout << alignof(C) << ", " << sizeof(C) << std::endl; //  8, 24 <- 16 + 1 + 1 + (6)
    std::cout << alignof(D) << ", " << sizeof(D) << std::endl; //  1, 13 <- 8 + 4 + 1
    std::cout << alignof(E) << ", " << sizeof(E) << std::endl; // 16, 32 <- 4 + (4) + 8 + 1 + (15)
}

namespace test4 {
int c = 0;
int cc = 0;

struct A {
    A(int d) : data(d) { c++; }
    A(const A& a) { cc++; }
    int data;
};

void test4() {
    std::map<int, A> m;
    m.emplace(1, 88);
    m.emplace(2, 99);
    std::cout << c << "," << cc << std::endl;
    for (const std::pair<int, A>& item : m) {
        std::cout << "  " << item.first << "|" << item.second.data << std::endl;
    }
    std::cout << c << "," << cc << std::endl;
    for (const std::pair<const int, A>& item : m) {
        std::cout << "  " << item.first << "|" << item.second.data << std::endl;
    }
    std::cout << c << "," << cc << std::endl;
}
}

void test5() {
    bool wait = true;

    auto worker = [](bool& w) {
        // This will never exit if optimised to:
        //   if (!w) return; while(1) {}
        // It won't happen if compiled with -O0
        // The solution is to use volatile or atomic
        // volatile tells the compiler to not optimise w
        while (w) {}
        std::cout << "worker exit" << std::endl;
        return;
    };

    std::thread t(worker, std::ref(wait));
    std::this_thread::sleep_for(std::chrono::seconds(1));
    wait = false;
    std::cout << "reset wait" << std::endl;
    t.join();
}

namespace test6{
/* A constexpr specifier used in an object declaration or non-static member function (until C++14) 
 * implies const. 
 * A constexpr specifier used in a function or static data member (since C++17) declaration implies 
 * inline.
 * 
 * The definition of constexpr functions in C++ is such that the function is guaranteed to be able
 * to produce a constant expression when called such that only constant expressions are used in the
 * evaluation.
 * When passing non-constant expressions to a constexpr you may not get a constant expression.
 */
constexpr int foo(int a) {
    return 2 * a;
}
void test6() {
    int A[1];
    int b = (int64_t)(&A[0]);
    std::cout << foo(b) << std::endl;

    constexpr int i = 2;
    int arr[foo(i)];                  // error if not constexpr
    for (int j = 0; j < foo(i); j++)
        arr[j] = j;
    for (int j = 0; j < foo(i); j++)
        std::cout << arr[j] << " ";
    std::cout << std::endl;

    static const int N = 3;
    constexpr const int *NP = &N; // `constexpr int *NP = &N` is wrong
    std::cout << NP << std::endl;
}
}

void test7() {
    /*
    * In C, when you convert a signed 16-bit integer (int16) to an unsigned 32-bit 
    * integer (uint32), the integer value will be expanded to 32 bits by adding 16 
    * leading zeros. The sign of the original 16-bit integer will be ignored and the 
    * resulting 32-bit unsigned integer will have the same bit pattern as the original 
    * 16-bit integer.
    * For example, if you have a 16-bit integer with a binary representation of 
    * 1111 0000 0101 0110 (which represents the decimal value -4066 in two's complement), 
    * when you convert it to a 32-bit unsigned integer, the resulting binary 
    * representation will be 0000 0000 0000 0000 1111 0000 0101 0110 (which represents 
    * the decimal value 4294963226).
    * It is important to note that when converting from a signed integer to an unsigned 
    * integer, there is a risk of losing information. If the original signed integer 
    * value is negative, the resulting unsigned integer value will be a large positive 
    * value, which may not represent the original value correctly. Therefore, it is 
    * important to check for this possibility and handle it appropriately in your code.
    *
    * When you convert an unsigned 16-bit integer (uint16) to a signed 32-bit integer 
    * (int32), since any value that can be stored in an uint16_t can also be stored in an
    * int32_t, the value is unchanged when converted to the new type.
    */
    {
        int16_t a = 10;
        std::cout << (uint32_t)a << std::endl;
    }
    {
        // Pay attention to this
        int16_t a = -10;
        std::cout << (uint32_t)a << std::endl;
    }
    {
        uint16_t a = 10;
        std::cout << (int32_t)a << std::endl;
    }
    {
        uint16_t a = -10;
        std::cout << (int32_t)a << std::endl;
    }
}

namespace test8 {
template <class T>
std::string type_name() {
    typedef typename std::remove_reference<T>::type TR;
    std::unique_ptr<char, void(*)(void*)> own(abi::__cxa_demangle(typeid(TR).name(), nullptr, nullptr, nullptr), std::free);
    std::string r = own != nullptr ? own.get() : typeid(TR).name();
    return r;
}

/*
* == Usual Arithmetic Conversions ==
* long double, double, float, uint64, int64, uint, int
*/
uint32_t add16(int16_t a, uint16_t b) {
    std::cout << "a:" << a << ", b:" << b << std::endl;
    auto c = a + b;
    std::cout << type_name<decltype(c)>() << " c:" << c << std::endl;
    uint16_t u16 = a + b;
    std::cout << "u16:" << u16 << ", (uint16_t)c:" << (uint16_t)c << std::endl;
    int16_t i16 = a + b;
    std::cout << "i16:" << i16 << ", (int16_t)c:" << (int16_t)c << std::endl;
    int32_t i32 = a + b;
    std::cout << "i32:" << i32 << ", (int32_t)c:" << (int32_t)c << std::endl;
    uint32_t u32 = a + b;
    std::cout << "u32:" << u32 << ", (uint32_t)c:" << (uint32_t)c << std::endl;
    return a + b;
}
uint64_t add32(int32_t a, uint32_t b) {
    std::cout << "a:" << a << ", b:" << b << std::endl;
    auto c = a + b;
    std::cout << type_name<decltype(c)>() << " c:" << c << std::endl;
    return a + b;
}

void test8() {
    std::cout << add16(-10, 4) << std::endl;
    std::cout << "----" << std::endl;
    std::cout << add16(-4, 10) << std::endl;
    std::cout << "----" << std::endl;
    std::cout << add16(4, -10) << std::endl;
    std::cout << "----" << std::endl;
    std::cout << add16(-4, 10) << std::endl;
    std::cout << "----" << std::endl;
    std::cout << add32(-10, 4) << std::endl;
    std::cout << "----" << std::endl;
    std::cout << add32(-4, 10) << std::endl;
    std::cout << "----" << std::endl;
    std::cout << add32(4, -10) << std::endl;
    std::cout << "----" << std::endl;
    std::cout << add32(-4, 10) << std::endl;
    std::cout << "----" << std::endl;
}
}

void test9() {
    {
        std::vector<std::string> vec{"a", "b"};
        for (size_t i=0; i<vec.size(); i++)  {
            if (i==2) {
                std::cout << "Unreachable" << std::endl;
                break;
            }
            vec.push_back( vec[i] + "1" ); // not undefined behaviour, but would change vec.size()
        }
        for (const auto& s : vec) {
            std::cout << s << std::endl;
        }
        std::cout << std::endl;
    }
    {
        std::vector<std::string> vec{"a", "b"};
        for (const auto& c : vec)  {
            vec.push_back( c + "1" ); // undefined behaviour
        }
        for (const auto& s : vec) {
            std::cout << s << std::endl;
        }
    }
}

int main()
{
    // test1::test1();
    // test2();
    test3();
    // test4::test4();
    // test5();
    // test6::test6();
    // test7();
    // test8::test8();
    // test9();
    return 0;
}
