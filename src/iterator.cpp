#include <iterator>
#include <cstddef>
#include <iostream>

template<typename TYPE>
class Integers
{
public:
    Integers() {
        for(int i=0; i<10; i++)
        {
            m_data[i] = static_cast<TYPE>(i);
        }
    }

    struct Iterator 
    {
        using iterator_category = std::forward_iterator_tag;
        using difference_type   = std::ptrdiff_t;
        using value_type        = TYPE;
        using pointer           = TYPE*;
        using reference         = TYPE&;

        Iterator(pointer ptr) : m_ptr(ptr) {}

        reference operator*() const { return *m_ptr; }
        pointer operator->() { return m_ptr; }
        Iterator& operator++() { m_ptr++; return *this; }  
        Iterator operator++(int) { Iterator tmp = *this; ++(*this); return tmp; }
        friend bool operator== (const Iterator& a, const Iterator& b) { return a.m_ptr == b.m_ptr; };
        friend bool operator!= (const Iterator& a, const Iterator& b) { return a.m_ptr != b.m_ptr; };  

    private:
        pointer m_ptr;
    };

    Iterator begin() { return Iterator(&m_data[0]); }
    Iterator end()   { return Iterator(&m_data[10]); }

private:
    TYPE m_data[10];
};

int main() {
    Integers<double> integers;

    for (auto i : integers) {
        std::cout << i << " ";
    }
    std::cout << std::endl;

    for (auto it=integers.begin(); it!=integers.end(); ++it) { 
        const auto i = *it; 
        std::cout << i << " ";
    }
    std::cout << std::endl;
}