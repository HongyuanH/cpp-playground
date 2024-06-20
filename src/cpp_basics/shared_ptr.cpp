#include <chrono>
#include <iostream>
#include <memory>
#include <mutex>
#include <thread>
 
using namespace std::chrono_literals;
 
struct Base
{
    Base() { std::cout << "Base::Base()\n"; }
    // Note: non-virtual destructor is OK here
    ~Base() { std::cout << "Base::~Base()\n"; }
};
 
struct Derived: public Base
{
    Derived() { std::cout << "Derived::Derived()\n"; }
 
    ~Derived() { std::cout << "Derived::~Derived()\n"; }
};
 
void print(const std::string& rem, std::shared_ptr<Base> const& sp)
{
    std::cout << rem << "\tget() = " << sp.get()
              << ", use_count() = " << sp.use_count() << '\n';
}
 
void thr(std::shared_ptr<Base> p)
{
    printf("Enter thread.\n");
    print("p:", p);

    std::this_thread::sleep_for(987ms);

    {    
        std::shared_ptr<Base> lp = p; // thread-safe, even though the shared use_count is incremented
        static std::mutex io_mutex;
        std::lock_guard<std::mutex> lk(io_mutex);
        printf("Local pointer in a thread\n");
        print("lp:", lp);
    }

    printf("Exit thread.\n");
    print("p:", p);
}
 
void test1()
{
    std::shared_ptr<Base> p1 = std::make_shared<Derived>();
    printf("p1 = std::make_shared<Derived>()\n");
    print("p1:", p1);
 
    std::shared_ptr<Base> p2;
    p2 = p1;
    printf("p2 = p1;\n");
    print("p1:", p1);
    print("p2:", p2);
    
    std::shared_ptr<Base> p3;
    p3 = std::move(p2);
    printf("p3 = std::move(p2);\n");
    print("p1:", p1);
    print("p2:", p2);
    print("p3:", p3);

    std::thread t1{thr, p1}, t2{thr, p1}, t3{thr, p1};

    p1.reset();
    printf("p1.reset();\n");
    print("p1:", p1);
    print("p2:", p2);
    print("p3:", p3);
 
    t1.join(); t2.join(); t3.join();
 
    printf("All threads completed.\n");
    print("p1:", p1);
    print("p2:", p2);
    print("p3:", p3);
}

int main()
{
    test1();
}