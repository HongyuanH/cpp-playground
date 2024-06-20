#include <iostream>
#include <memory>
#include <atomic>

class Resource
{
public:
    Resource(int data_) : data(data_) {}
    int data;
};

template<class T>
class SharedPtr
{
public:
    SharedPtr() : res(nullptr), cnt(nullptr) {
        std::cout << "SharedPtr() " << this << std::endl;
    }

    SharedPtr(T* res_) : res(res_), cnt(new std::atomic<int> {1}) {
        std::cout << "SharedPtr(T* res)" << this << std::endl;
        std::cout << "new cnt" << std::endl;
    }

    ~SharedPtr() {
        std::cout << "~SharedPtr() " << this << std::endl;
        if (cnt) {
            std::cout << "  cnt[0] " << cnt[0] << std::endl;
            (*cnt) --;
            if (*cnt == 0) {
                if (res) {
                    delete res;
                }
                std::cout << "delete cnt" << std::endl;
                delete cnt;
            }
        }
    }

    SharedPtr(const SharedPtr& other) : SharedPtr() {
        std::cout << "SharedPtr(const SharedPtr& other) " << this << std::endl;
        res = other.res;
        cnt = other.cnt;
        if (cnt) {
            (*cnt) ++;
        }
    }

    SharedPtr& operator=(SharedPtr other) {
        std::cout << "operator=(const SharedPtr& other) " << this << std::endl;
        swap(other);
        return *this;
    }

    SharedPtr(SharedPtr&& other) noexcept : SharedPtr() {
        std::cout << "SharedPtr(SharedPtr&& other) " << this << std::endl;
        swap(other);
    }

    void swap(SharedPtr& other) {
        using std::swap;
        swap(res, other.res);
        swap(cnt, other.cnt);
    }

    T* operator->() const {
        return res;
    }

    T& operator*() const {
        return *res;
    }

    T* get() const {
        return res;
    }

    int use_count() const {
        return *cnt;
    }

private:
    T* res;
    std::atomic<int>* cnt;
};

template< class T, class... Args >
SharedPtr<T> make_shared( Args&&... args ){
    return SharedPtr<T>{new Resource {args...}};
}

int main()
{
    auto test = [](SharedPtr<Resource>& obj1) {
        SharedPtr<Resource> obj2(obj1);                    // Copy ctor
        SharedPtr<Resource> obj3(std::move(obj2));         // Move ctor
        SharedPtr<Resource> obj4(new Resource {777});
        obj4 = obj4;                                       // Self copy assign
        obj4->data = 666;      
        obj4 = obj1;                                       // Copy assign
        SharedPtr<Resource> obj5(new Resource {555});      
        obj5 = std::move(obj5);                            // Self move assign
        obj5->data = 444;
        obj5 = std::move(obj4);                            // Move assign
        std::cout << "####" << std::endl << std::endl;
    };
    // empty pointer
    {
        SharedPtr<Resource> obj1;
        test(obj1);
    }
    // with resource
    {
        SharedPtr<Resource> obj1(new Resource {888});
        test(obj1);
    }
    // empty pointer but with counter
    {
        SharedPtr<Resource> obj1(nullptr);
        test(obj1);
    }
    // usage
    {
        auto obj = make_shared<Resource>(123);
        std::cout << obj->data << std::endl;
        std::cout << "obj->data " << obj->data << std::endl;
        std::cout << "(*obj).data " << (*obj).data << std::endl;
        std::cout << "obj.get()->data " << obj.get()->data << std::endl;
        obj->data = 1000;
        std::cout << "obj->data " << obj->data << std::endl;
        std::cout << "(*obj).data " << (*obj).data << std::endl;
        std::cout << "obj.get()->data " << obj.get()->data << std::endl;
        std::cout << "obj.use_count() " << obj.use_count() << std::endl;
    }
    return 0;
}
