#include <chrono>
#include <iostream>
#include <memory>
#include <mutex>
#include <thread>
#include <string>
#include <map>

using namespace std;

class A {
public:
    A() { cout << "Construct A" << endl; }
    virtual ~A() {}
    A(const A& other) { cout << "Construct A from A" << endl; }
    virtual A& func() { cout << "From A" << endl; return *this; }
    void printA() const { cout << dataA << endl; }
    int dataA{10};
};

class B : public A {
public:
    B() { cout << "Construct B" << endl; }
    B(const A&) { cout << "Construct B from A" << endl; }
    ~B() {}
    virtual B& func() override { cout << "From B" << endl; return *this; }
    void printB() const { cout << dataB << endl; }
    int dataB{20};
};

/*
- static_cast: Compile time cast for implicit/explicit type coersion. 
  Examples:
    float <-> int
    enum <-> int
    ...
    float* <-> void*
    void* <-> int*
    ...
    A <-> B
    A* <-> B*
    A& <-> B&
    ...
    User defined conversion!
*/
void test_static_cast() {
    {
        float a = 1.1;
        int b = static_cast<int>(a);
        cout << b << endl; // 1
    }
    {
        float a = 1.1;
        void* v = static_cast<void*>(&a);
        int* b = static_cast<int*>(v); // 1066192077
        cout << *b << endl;
    }
    // Doesn't compile
    // {
    //     float a = 1.1;
    //     int* b = static_cast<int*>(&a);
    // }
    {
        B b;
        static_cast<A>(b).func();    // From A, copy construct A from A
        static_cast<A&>(b).func();   // From B
        static_cast<A*>(&b)->func(); // From B
    }
    {
        A a;
        static_cast<B>(a).func().printB();    // From B, 20(construct B from A)
        static_cast<B&>(a).func().printB();   // From A, 0(undefined behavior)
        static_cast<B*>(&a)->func().printB(); // From A, 0(undefined behavior)
    }
    {
        B b;
        static_cast<B&>( static_cast<A&>(b) ).func().printB();   // from B, 20
        static_cast<B*>( static_cast<A*>(&b) )->func().printB(); // from B, 20
    }
}

void test_user_defined_cast() {
    class MyDouble {
    public:
        MyDouble(double d) : d_(d) {}
        operator int() const { return static_cast<int>(d_); } // user-defined conversion function
        double d_;
    };

    MyDouble d(3.14159);
    int i = static_cast<int>(d);
    std::cout << "d.d_ = " << d.d_ << ", i = " << i << std::endl;
    std::cout << "d = " << d << std::endl;  // implicit conversion using user-defined conversion function
}


/*
- dynamic_cast: Runtime cast between base class and derived class (usually pointers or references).
                Dynamic_cast is safer to use than static_cast in cases where the type information is not known at compile time.
  Examples:
    A* <-> B*
    A& <-> B&
                
*/
void test_dynamic_cast() {
    auto try_up_cast = [](B& b) {
        // error: cannot dynamic_cast ‘b’ (of type ‘class B’) to type ‘class A’ (target is not pointer or reference)
        // dynamic_cast<A>(b).func(); 
        try {
            dynamic_cast<A&>(b).func();
        } catch (const std::exception &e) {
            cout << e.what() << endl;
        }
        A* pa = dynamic_cast<A*>(&b);
        if (pa) {
            pa->func();
        } else {
            cout << "bad cast" << endl;
        }
    };

    auto try_down_cast = [](A& a) {
        // error: cannot dynamic_cast ‘a’ (of type ‘class A’) to type ‘class B’ (target is not pointer or reference)
        // dynamic_cast<B>(a).func(); 
        try {
            dynamic_cast<B&>(a).func();
        } catch (const std::exception &e) {
            cout << e.what() << endl;
        }
        B* pb = dynamic_cast<B*>(&a);
        if (pb) {
            pb->func();
        } else {
            cout << "bad cast" << endl;
        }      
    };

    A a;
    B b;
    try_up_cast( b );                     // From B, From B
    try_up_cast( static_cast<B&>(a) );    // From A, From A
    try_down_cast( a );                   // std::bad_cast, bad cast
    try_down_cast( dynamic_cast<A&>(b) ); // From B, From B
}

/*
- reinterpret_cast: Converts between types by reinterpreting the underlying bit pattern. 
  Examples:
    void* <-> A*
    A& <-> B&
    A* <-> B*
    int* <-> uint64_t
*/
void test_reinterpret_cast()
{
    {
        B b;
        reinterpret_cast<A&>(b).func().printA();                           // From B, 10
        reinterpret_cast<A*>(&b)->func().printA();                         // From B, 10
        reinterpret_cast<B&>( reinterpret_cast<A&>(b) ).func().printB();   // From B, 20
        reinterpret_cast<B*>( reinterpret_cast<A*>(&b) )->func().printB(); // From B, 20
    }
    {
        int a = 10;
        cout << &a << endl;
        cout << std::hex << reinterpret_cast<uint64_t>(&a) << endl;         // Same as &a
        // error: cannot dynamic_cast ‘pa’ (of type ‘int*’) to type ‘uint64_t’ {aka ‘long unsigned int’} (target is not pointer or reference)
        // cout << std::hex << dynamic_cast<uint64_t>(&a) << endl;
        // error: invalid static_cast from type ‘int*’ to type ‘uint64_t’ {aka ‘long unsigned int’}
        // cout << std::hex << static_cast<uint64_t>(&a) << endl;
    }
}

/*
- const_cast: to add or remove `const`.
  Examples:
    const A <-> A&
    const A* <-> A*
    const int <-> int
    const int <-> int&
*/
void test_const_cast() {
    {
        const A a;
        a.printA(); // 10
        const_cast<A&>(a).dataA = 88;
        a.printA(); // 88
        const_cast<A*>(&a)->dataA = 99;
        a.printA(); // 99
    }
    {
        const int a = 8;
        int& b = const_cast<int&>( a );
        b = 9; // undefined behaviour when a is initialized as const
        cout << &a << ": " << a << endl; // 8, because compiler thinks a is const
        cout << &b << ": " << b << endl; // 9, same address as a
    }
}

int main() {
    // test_static_cast();
    // test_user_defined_cast();
    // test_dynamic_cast();
    // test_reinterpret_cast();
    test_const_cast();
}