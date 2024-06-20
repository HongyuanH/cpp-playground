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
    /* Copy-and-swap idiom:
     * Can only take const Foo&, copy needs to be done within function.
     */
    Foo& operator=(const Foo& other) {
        Foo temp(other);
        cout << "operator=(Foo other) " << id_  << " " << res_ << endl;
        swap(*this, temp);
        cout << "> operator=(Foo other) " << id_ << " " << res_ << " " << res_->data_ << endl; 
        return *this;
    }
    /* move constructor */
    Foo(Foo&& other) noexcept : Foo()  {
        cout << "Foo(Foo&& other) " << id_  << " " << res_ << endl; 
        swap(*this, other);
        cout << "> Foo(Foo&& other) " << id_ << " " << res_ << " " << res_->data_ << endl; 
    }
    /* move assignment */
    Foo& operator=(Foo&& other) noexcept {
        Foo temp(move(other));
        cout << "operator=(Foo&& other) " << id_  << " " << res_ << endl;
        swap(*this, temp);
        cout << "> operator=(Foo&& other) " << id_ << " " << res_ << " " << res_->data_ << endl; 
        return *this;
    }
    /* friend swap */
    friend void swap(Foo& first, Foo& second)
    {
        using std::swap;
        swap(first.res_, second.res_);
        swap(first.id_, second.id_);
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
    {
        cout  << endl << "====" << endl;
        Foo f1(70);
        Foo f2(80);
        f1 = move(f2); // move assignment, contents of f1 should have been destructed after this
        cout << "----" << endl;
    }
}

int main() {
    test_copy_move();
}