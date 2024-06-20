#include <iostream>

using namespace std;

class Resource {
public:
    Resource() {
        cout << "Resource() " << data_ << endl;
    }
    Resource(int data) : data_(data) {
        cout << "Resource(int data) " << data_ << endl;
    }
    ~Resource() {
        cout << "~Resource() " << data_ << endl;
    }
    int data_;
};

static int num_of_foos{0};

class Foo
{
public:
    Foo() : res_(nullptr), id_(num_of_foos++) { 
        cout << "Foo() " << id_ << " " << res_ << endl; 
    }
    Foo(int data) : res_(new Resource(data)), id_(num_of_foos++) {
        cout << "Foo(int data) " <<  id_ << " " << res_ << " " << res_->data_ << endl; 
    }
    /* destructor */
    ~Foo()
    {
        cout << "~Foo() " << id_ << " " << res_; 
        if (res_) {
            cout << " " << res_->data_;
        }
        cout << endl;
        if (res_) {
            delete res_;
        }
    }
    /* copy constructor */
    Foo(const Foo& other) : Foo() {
        if (other.res_) {
            res_ = new Resource(other.res_->data_);
        }
        cout << "Foo(const Foo& other) " << id_ << " " << res_;
        if (res_) {
            cout << " " << res_->data_;
        }
        cout << endl;
    }
    /* copy assignment */
    Foo& operator=(const Foo& other) {
        // self-assignment detection
        if (&other == this) {
            return *this;
        }
        delete res_;
        res_ = nullptr;
        if (other.res_) {
            res_ = new Resource(other.res_->data_);
        }
        id_ = other.id_;
        cout << "> operator=(Foo other) " << id_ << " " << res_ << " " << res_->data_ << endl; 
        return *this;
    }
    /* move constructor */
    Foo(Foo&& other) noexcept : Foo()  {
        res_ = other.res_;
        other.res_ = nullptr;
        id_ = other.id_;
        cout << "Foo(Foo&& other) " << id_ << " " << res_ << " " << res_->data_ << endl; 
    }
    /* move assignment */
    Foo& operator=(Foo&& other) noexcept {
        // self-assignment detection
        if (&other == this) {
            return *this;
        }
        delete res_;
        res_ = other.res_;
        id_ = other.id_;
        other.res_ = nullptr;
        cout << "operator=(Foo&& other) " << id_ << " " << res_ << " " << res_->data_ << endl; 
        return *this;
    }
private:
    Resource* res_;
    int id_;
};

void test_copy_move() {
    {
        cout << endl << "====" << endl;
        Foo f1; // default constructor
        cout << "----" << endl;
    }
    {
        cout << endl << "====" << endl;
        Foo f1(10); // constructor
        Foo f2(f1); // copy constructor
        cout << "----" << endl;
    }
    {
        cout << endl << "====" << endl;
        Foo f1(20); // constructor
        Foo f2 = f1; // copy constructor
        cout << "----" << endl;
    }
    {
        cout << endl << "====" << endl;
        Foo f1(30); // constructor
        Foo f2; // default constructor
        f2 = f1; // copy assignment
        cout << "----" << endl;
    }
    {
        cout << endl << "====" << endl;
        Foo f1(40); // constructor
        Foo f2(move(f1)); // move constructor
        cout << "----" << endl;
    }
    {
        cout << endl << "====" << endl;
        Foo f1 = move(Foo(50)); // move constructor
        cout << "----" << endl;
    }
    {
        cout  << endl << "====" << endl;
        Foo f1;
        f1 = move(Foo(60)); // move assignment, could be ambiguous if copy assignment take value instead of const reference
        cout << "----" << endl;
    }
}

int main() {
    test_copy_move();
}