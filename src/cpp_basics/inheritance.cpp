#include <iostream>
#include <memory>

using namespace std;

/*
Memory layout:
                object              vtable for Base1
                --------------      --------------------------
Base1* b1 --->  | vptr_Base1 | ---> | type info for Base1    |
                | data1      |      | Base1::VirtualFunc()   |
                --------------      | Base1::VirtualFunc_1() |
                                    --------------------------

                object              vtable for Base2
                --------------      --------------------------
Base2* b2 --->  | vptr_Base2 | ---> | type info for Base2    |
                | data2      |      | Base2::VirtualFunc()   |
                --------------      | Base2::VirtualFunc_2() |
                                    --------------------------

                    object                      vtable for Derived_Base1
                    ----------------------      ----------------------------
Derived* d ----->   | vptr_Derived_Base1 | ---> | type info for Derived    |
Base1* d   --'      | data1              |      | Derived::VirtualFunc()   |
Base2* d --->       | vptr_Derived_Base2 | -.   | Derived::VirutalFunc_1() |
                    | data2              |  |   | Base2::VirutalFunc_2()   |
                    | data3              |  |   ----------------------------
                    ----------------------  |   vtable for Derived_Base2
                                            |   ----------------------------
                                            '-> | type info for Derived    |
                                                | Derived::VirtualFunc()   |
                                                | Base2::VirutalFunc_2()   |
                                                ----------------------------
*/
class MultipleInheritance
{
public:
    class Base1
    {
    public:
        string NonVirtualFunc() { return "Base1::NonVirtualFunc"; }
        virtual string VirutalFunc() { return "Base1::VirutalFunc"; }
        virtual string VirutalFunc_1() { return "Base1::VirutalFunc_1"; }
        int data1;
    };

    class Base2
    {
    public:
        string NonVirtualFunc() { return "Base2::NonVirtualFunc"; }
        virtual string VirutalFunc() { return "Base2::VirutalFunc"; }
        virtual string VirutalFunc_2() { return "Base2::VirutalFunc_2"; }
        int data2;
    };

    class Derived : public Base1, public Base2
    {
    public:
        string NonVirtualFunc() { return "Derived::NonVirtualFunc"; }
        string VirutalFunc() override { return "Derived::VirutalFunc"; }
        string VirutalFunc_1() override { return "Derived::VirutalFunc_1"; }
        int data3;
    };

    static void Run()
    {
        std::shared_ptr<Derived> d = std::make_shared<Derived>();
        std::shared_ptr<Base1> b1 = std::static_pointer_cast<Base1>(d);
        std::shared_ptr<Base2> b2 = std::static_pointer_cast<Base2>(d);

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
    }
};

//TODO: diamond inheritance

int main() {
    MultipleInheritance::Run();
    return 0;
}