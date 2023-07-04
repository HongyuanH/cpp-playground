#include <iterator>
#include <cstddef>
#include <iostream>

// Use a parent pointer would make it much easier
// Or using a stack to store the path

template<typename TYPE>
class Node
{
public:
    TYPE val;
    Node<TYPE>* parent = nullptr;
    Node<TYPE>* left = nullptr;
    Node<TYPE>* right = nullptr;

    Node<TYPE>* findLeftMost() {
        if (left == nullptr) {
            return this;
        } else {
            return left->findLeftMost();
        }
    }

    Node<TYPE>* findRightMost() {
        if (right == nullptr) {
            return this;
        } else {
            return right->findRightMost();
        }
    }

    struct Iterator 
    {
        using iterator_category = std::bidirectional_iterator_tag;
        using difference_type   = std::ptrdiff_t;
        using value_type        = Node<TYPE>;
        using pointer           = Node<TYPE>*;
        using reference         = Node<TYPE>&;

        Iterator(pointer ptr_) : ptr(ptr_) {}

        reference operator*() const { return *ptr; }
        pointer operator->() { return ptr; }

        Iterator& operator++() { 
            if (ptr->right) {
                ptr = ptr->right;
                while (ptr->left) {
                    ptr = ptr->left;
                }
                return *this;
            } else {
                while (ptr->parent) {
                    if (ptr->parent->left == ptr) {
                        ptr = ptr->parent;
                        return *this;
                    } else {
                        ptr = ptr->parent;
                    }
                }
                ptr = nullptr; // indicate end node
                return *this;  
            }
        }  
        Iterator operator++(int) { Iterator tmp{ptr}; ++(*this); return tmp; }

        Iterator& operator--() { 
            if (ptr->left) {
                ptr = ptr->left;
                while (ptr->right) {
                    ptr = ptr->right;
                }
                return *this;
            } else {
                while (ptr->parent) {
                    if (ptr->parent->right == ptr) {
                        ptr = ptr->parent;
                        return *this;
                    } else {
                        ptr = ptr->parent;
                    }
                }
                ptr = nullptr; // indicate end node
                return *this;  
            }
            return *this;
        }
        Iterator operator--(int) { Iterator tmp{ptr}; --(*this); return tmp; }

        // const member function, or friend?
        bool operator== (const Iterator& other) const { return ptr == other.ptr; };
        bool operator!= (const Iterator& other) const { return ptr != other.ptr; };  

    private:
        pointer ptr;
    };

    Iterator begin() { return Iterator(findLeftMost()); }
    Iterator end()   { return Iterator(nullptr); } // Use nullptr to indicate end()
    Iterator rbegin() { return Iterator(findRightMost()); }
    Iterator rend()   { return Iterator(nullptr); } // Use nullptr to indicate end()
};

int main() {
    using NodeT = Node<int>;

    NodeT nodes[7];
    for (int i=0; i<7; i++){
        nodes[i].val = i;
    }
    nodes[1].left  = &nodes[0]; nodes[0].parent = &nodes[1];
    nodes[1].right = &nodes[2]; nodes[2].parent = &nodes[1];
    nodes[3].left  = &nodes[1]; nodes[1].parent = &nodes[3];
    nodes[3].right = &nodes[5]; nodes[5].parent = &nodes[3];
    nodes[5].left  = &nodes[4]; nodes[4].parent = &nodes[5];
    nodes[5].right = &nodes[6]; nodes[6].parent = &nodes[5];
    
    NodeT& root = nodes[3];

    for (auto iter : root) {
        std::cout << iter.val << " ";
    }
    std::cout << std::endl;

    for (auto iter=root.begin(); iter!=root.end(); iter++) {
        std::cout << iter->val << " ";
    }
    std::cout << std::endl;

    for (auto iter=root.rbegin(); iter!=root.rend(); iter--) {
        std::cout << iter->val << " ";
    }
    std::cout << std::endl;

}