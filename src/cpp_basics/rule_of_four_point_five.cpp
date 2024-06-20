/*
 * This is the easy way of doing things, rule of 5/5.5 is discouraged.
 * Reference: 
 * https://stackoverflow.com/questions/3279543/what-is-the-copy-and-swap-idiom
 */

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
     * Note: Passing other by value would result in ambiguous overload, if move assignment is also defined.
     * Generally, Foo& operator=(Foo other) and Foo& operator=(Foo&& other) cannot coexist, we only need one.
     */
    Foo& operator=(Foo other) {
        cout << "operator=(Foo other) " << id_  << " " << res_ << endl;
        swap(*this, other);
        cout << "> operator=(Foo other) " << id_ << " " << res_ << " " << res_->data_ << endl; 
        return *this;
    }
    /* move constructor */
    Foo(Foo&& other) noexcept : Foo()  {
        cout << "Foo(Foo&& other) " << id_  << " " << res_ << endl; 
        swap(*this, other);
        cout << "> Foo(Foo&& other) " << id_ << " " << res_ << " " << res_->data_ << endl; 
    }
    /* friend swap */
    /* std::swap relies on the assignment operator. So if the assignment operator were to call std::swap, this would lead to an infinite chain of calls back and forth between the assignment operator and std::swap.
     * For a well-behaved class with value semantics, it is safe to call std::swap on objects of that class. Only the implementer of the class cannot use it for the copy-and-swap idiom for the reason stated above. 
     * 
     * We also cannot specialize template classes with swap, only particular instantiations:
        namespace std
        {
            template <typename T>
            void swap<T>(Foo<T>&, Foo<T>&) // error! no partial specialization
            {
                // swap
            }
        }
     * 
     * friend member function is the solution
     */
    friend void swap(Foo& first, Foo& second)
    {
        using std::swap;               // allow use of std::swap...
        swap(first.res_, second.res_); // ...but select overloads, first
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
        f1 = move(Foo(60)); // copy assignment, could be ambiguous if copy assignment take value instead of const reference
        cout << "----" << endl;
    }
}

int main() {
    test_copy_move();
}