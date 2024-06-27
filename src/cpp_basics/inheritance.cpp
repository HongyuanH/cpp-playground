#include <iostream>
#include <memory>

using namespace std;

// Hope this link still works:
// https://nimrod.blog/posts/what-does-cpp-object-layout-look-like/
//
// This might be easier to understand (but no diagram)
// https://leimao.github.io/blog/CPP-Virtual-Table/

/*
Multiple Inheritance (MI):
* Data members are stacked inside an object
* Compiler inserts ~Base1() and ~Base2() into ~Derived()
* There are at least 2 vtables for MI
* vtables can also be stacked
* vptr points to the first virtual function (usually destructor)
* type_info: a pointer pointing to the same std::type_info object, can be used for checking if the object is of type(T) before dynamic_cast
* top_offset: (needed for casting from Base to Derived)
    d = b + b->vptr.top_offset;
* Derived::VirtualFunc() thunk:
    void ~Derived_Thunk(Base2* this) {
        this += this->vptr.top_offset; 
        this->vptr.Derived::~Derived(this);
    }

                object              vtable
                --------------      --------------------------
Base1* b1 --->  | vptr       | -.   | top_offset 0           |
                | data1      |  |   | type_info* for Base1   |
                --------------  '-> | Base1::~Base()         | 
                                    | Base1::VirtualFunc()   |
                                    | Base1::VirtualFunc_1() |
                                    --------------------------

                object              vtable
                --------------      --------------------------
Base2* b2 --->  | vptr       | -.   | top_offset 0           |
                | data2      |  |   | type_info* for Base2   |
                --------------  '-> | Base2::~Base2()        |
                                    | Base2::VirtualFunc()   |
                                    | Base2::VirtualFunc_2() |
                                    --------------------------

                    object                      vtable
                    ----------------------      --------------------------------
Derived* d ----->   | vptr               | -.   | top_offset 0                 |
Base1* b1  --'      | data1              |  |   | type_info* for Derived       |
                    |--------------------|  '-> | Derived::~Derived()          |
Base2* b2 ------>   | vptr               | -.   | Derived::VirtualFunc()       |
                    | data2              |  |   | Derived::VirutalFunc_1()     |
                    |--------------------|  |   | Base2::VirutalFunc_2()       |
                    | data3              |  |   |------------------------------|
                    ----------------------  |   | top_offset -16               |
                                            |   | type_info* for Derived       |
                                            '-> | Derived::~Derived() thunk    |
                                                | Derived::VirtualFunc() thunk |
                                                | Base2::VirutalFunc_2()       |
                                                --------------------------------
*/
class MultipleInheritance
{
public:
    class Base1
    {
    public:
        virtual ~Base1() { cout << "~Base1()" << endl; };
        string NonVirtualFunc() { return "Base1::NonVirtualFunc"; }
        virtual string VirutalFunc() { return "Base1::VirutalFunc"; }
        virtual string VirutalFunc_1() { return "Base1::VirutalFunc_1"; }
        int data1;
    };

    class Base2
    {
    public:
        virtual ~Base2() { cout << "~Base2()" << endl; };
        string NonVirtualFunc() { return "Base2::NonVirtualFunc"; }
        virtual string VirutalFunc() { return "Base2::VirutalFunc"; }
        virtual string VirutalFunc_2() { return "Base2::VirutalFunc_2"; }
        int data2;
    };

    class Derived : public Base1, public Base2
    {
    public:
        ~Derived() { cout << "~Derived()" << endl; }
        string NonVirtualFunc() { return "Derived::NonVirtualFunc"; }
        string VirutalFunc() override { return "Derived::VirutalFunc"; }
        string VirutalFunc_1() override { return "Derived::VirutalFunc_1"; }
        int data3;
    };

    static void Run()
    {
        shared_ptr<Derived> d = make_shared<Derived>();
        shared_ptr<Base1> b1 = static_pointer_cast<Base1>(d);
        shared_ptr<Base2> b2 = static_pointer_cast<Base2>(d);

        // Non-virtual functions are not stored in vtable, these are direct function call
        cout << d->NonVirtualFunc() << endl;        // Derived::NonVirtualFun
        cout << b1->NonVirtualFunc() << endl;       // Base1::NonVirtualFunc
        cout << b2->NonVirtualFunc() << endl;       // Base2::NonVirtualFunc
        cout << d->Base1::NonVirtualFunc() << endl; // Base1::NonVirtualFunc
        cout << d->Base2::NonVirtualFunc() << endl; // Base2::NonVirtualFunc

        // Virutal functions are stored in vtable, thess goes through vptr to Derived's vtable
        cout << d->VirutalFunc() << endl;    // Derived::VirutalFunc, won't compile if not overridden by Derived
        cout << d->VirutalFunc_1() << endl;  // Derived::VirutalFunc_1
        cout << d->VirutalFunc_2() << endl;  // Base2::VirutalFunc_2
        cout << b1->VirutalFunc() << endl;   // Derived::VirutalFunc
        cout << b1->VirutalFunc_1() << endl; // Derived::VirutalFunc_1
        cout << b2->VirutalFunc() << endl;   // Derived::VirutalFunc
        cout << b2->VirutalFunc_2() << endl; // Base2::VirutalFunc_2

        d.reset();
        b1.reset();
        b2.reset();
    }
};

/*
Diamond Inheritance (DI):
* vbase_offset: needed for casting from Derived/Base1/Base2 to VBase:
    v = d + d->vptr.vbase_offset
* vcall_offset: needed for calling overridden virtual function from VBase pointer:
    void Base2_VirutalFunc_V2_Thunk(VBase* this) {
        this += this->vptr.vcall_offset3;
        Base2::VirutalFunc_V2(this);
    }
    void Base1_VirutalFunc_V1_Thunk(VBase* this) {
        this += this->vptr.vcall_offset2;
        Base1::VirutalFunc_V1(this);
    }
    void ~Derived_Thunk(VBase* this) {
        this += this->vptr.vcall_offset1;
        this->vptr.Derived::~Derived(this);
    }

TODO:
* Virtual Table Tables???


                    object                               vtable
                    ----------------------             -----------------------------
Derived* d ----->   | vptr               | --------.   | vbase_offset 40           |
Base1* b1 ---'      | data1              |         |   | top_offset 0              |
                    |--------------------|         |   | type_info* for Derived    |
Base2* b2 ------>   | vptr               | ----.   '-> | Derived::~Derived()       | 
                    | data2              |     |       | Base1::VirtualFunc_V1()    |
                    |--------------------|     |       | Base1::VirutalFunc_1()    |
                    | data3              |     |       |---------------------------|
                    |--------------------|     |       | vbase_offset 24           |
VBase* v  ------>   | vptr               | -.  |       | top_offset -16            |
                    | dataV              |  |  |       | type_info* for Derived    |
                    ----------------------  |  '-----> | Derived::~Derived() thunk |
                                            |          | Base2::VirutalFunc_V2()   |
                                            |          | Base2::VirutalFunc_2()    |
                                            |          |---------------------------|
                                            |          | vcall_offset3 -24         |
                                            |          | vcall_offset2 -40         |
                                            |          | vcall_offset1 -40         |
                                            |          | top_offset -40            |
                                            |          | type_info* for Derived    |
                                            '--------> | Derived::~Derived() thunk |
                                                       | Base1::VirutalFunc_V1() thunk |
                                                       | Base2::VirutalFunc_V2() thunk |
                                                       -----------------------------
*/
class DiamondInheritance
{
public:
    struct VBase {
        virtual ~VBase() { cout << "~VBase()" << endl; };
        virtual string VirutalFunc_V1() { return "VBase::VirutalFunc_V1"; };
        virtual string VirutalFunc_V2() { return "VBase::VirutalFunc_V2"; };
        double dataV;
    };

    struct Base1 : virtual public VBase {
        virtual ~Base1() { cout << "~Base1()" << endl; };
        string VirutalFunc_V1() override { return "Base1::VirutalFunc_V1"; } ;
        virtual string VirutalFunc_1() { return "Base1::VirutalFunc_1"; }
        double data1;
    };

    struct Base2 : virtual public VBase {
        virtual ~Base2() { cout << "~Base2()" << endl; };
        string VirutalFunc_V2() override { return "Base2::VirutalFunc_V2"; } ;
        virtual string VirutalFunc_2() { return "Base2::VirutalFunc_2"; }
        double data2;
    };

    struct Derived: public Base1, public Base2 {
        ~Derived() { cout << "~Derived()" << endl; }
        double data;
    };

    static void Run()
    {
        shared_ptr<Derived> d = make_shared<Derived>();
        shared_ptr<Base1> b1 = static_pointer_cast<Base1>(d);
        shared_ptr<Base2> b2 = static_pointer_cast<Base2>(d);
        shared_ptr<VBase> v = static_pointer_cast<VBase>(d);

        cout << b1->VirutalFunc_V1() << endl;
        cout << b1->VirutalFunc_V2() << endl;

        cout << b2->VirutalFunc_V1() << endl;
        cout << b2->VirutalFunc_V2() << endl;

        d.reset();
        b1.reset();
        b2.reset();
        v.reset();
    }
};

int main() {
    // MultipleInheritance::Run();
    DiamondInheritance::Run();
    return 0;
}