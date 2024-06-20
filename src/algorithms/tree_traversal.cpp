#include <iostream>
#include <stack>
#include <queue>

using namespace std;

class Node {
public:
    int val;
    Node* left = nullptr;
    Node* right = nullptr;
};

void visitNode(Node* node) {
    cout << node->val << " ";
}

namespace recursive {
void preOrder(Node* node) {
    if (!node) {
        return;
    }
    visitNode(node);
    preOrder(node->left);
    preOrder(node->right);
}

void inOrder(Node* node) {
    if (!node) {
        return;
    }
    inOrder(node->left);
    visitNode(node);
    inOrder(node->right);
}

void postOrder(Node* node) {
    if (!node) {
        return;
    }
    postOrder(node->left);
    postOrder(node->right);
    visitNode(node);
}
}

// Read this:
// https://www.enjoyalgorithms.com/blog/iterative-binary-tree-traversals-using-stack
// The idea is to simulate the recursion with a stack, need to understand what's in the stack
// Normally, the while condition is 
//   `while(!s.empty() || cur)`, 
// cur represents the node being processed, it can be null
// So again, normally, the if condition within the loop is 
//   `if (cur) {} else {}`
//
// postOrder1() with 2 stacks, read this:
// https://www.geeksforgeeks.org/iterative-postorder-traversal/
//
// postOrder2 is an improvement over the 2 stacks method 
namespace iterarive {
void preOrder(Node* node) {
    if (!node) {
        return;
    }
    // stack stores both right tree and left tree
    stack<Node*> s;
    s.push(node);
    while (!s.empty()) {
        // process current node
        Node* cur = s.top();
        s.pop();
        visitNode(cur);
        // process left tree later, then right tree
        if (cur->right) {
            s.push(cur->right);
        }
        if (cur->left) {
            s.push(cur->left);
        }
    }
}

void preOrder2(Node* node) {
    if (!node) {
        return;
    }
    // stack stores the right tree 
    // (in recursion, the right tree is processed when the function returns after processing the left tree)
    stack<Node*> s;
    Node* cur = node;
    while (!s.empty() || cur) {
        if (cur) {
            // process the current node
            visitNode(cur);
            // process right tree later
            if (cur->right) {
                s.push(cur->right);
            }
            // process the left tree
            cur = cur->left;
        } else {
            // left most node reached, process the last right tree
            Node* right = s.top();
            s.pop();
            cur = right;
        }
    }
}

void inOrder(Node* node) {
    if (!node) {
        return;
    }
    // stack stores the node to be processed, together with its right tree
    stack<Node*> s;
    Node* cur = node;
    while (!s.empty() || cur) {
        if (cur) {
            // push node to be processed later, together with its right tree
            s.push(cur);
            // go to the left tree
            cur = cur->left;
        } else {
            // left most node reached, pop from stack and process it
            cur = s.top();
            s.pop();
            visitNode(cur);
            // process right tree
            cur = cur->right;
        }
    }
}

void postOrder(Node* node) {
    if (!node) {
        return;
    }
    // stack stores the node to be processed immediately next 
    // however, its left tree and right tree still need to be processed before this next node
    stack<Node*> s; 
    // last
    Node* lastProcessedNode = nullptr;
    Node* cur = node;
    while (!s.empty() || cur) {
        if (cur) {
            // push node to be processed later, together with its right tree
            s.push(cur);
            // go to the left tree
            cur = cur->left;
        } else {
            // left most node reached, get from stack (don't pop yet)/
            // need to check if right tree has been processed
            cur = s.top();
            if (cur->right && lastProcessedNode!=cur->right) {
                // right tree not processed yet, go to the right tree
                cur = cur->right;
            } else {
                // right tree already processed, or doesn't exist, we can process the node
                visitNode(cur);
                lastProcessedNode = cur;
                s.pop();
                // reset cur to nullptr so that in the next iteration, first node in stack (the parent) is processed
                // otherwise cur could be processed again (infinite loop)
                cur = nullptr;
            }
        }
    }
}

void postOrder2(Node* node) {
    if (!node) {
        return;
    }   
    // s1 stores the node to be processed immediately next 
    // however, its left tree and right tree still need to be processed before this next node
    stack<Node*> s1; 
    // s2 stores the node in postorder
    stack<Node*> s2;
    s1.push(node);
    while (!s1.empty()) {
        Node* cur = s1.top();
        s1.pop();
        s2.push(cur);
        // first node to s1 will be last node to s2
        // therefore it becomes first node to be processed
        // LIFO of LIFO
        if (cur->left) {
            s1.push(cur->left);
        }
        if (cur->right) {
            s1.push(cur->right);
        } 
    }
    while (!s2.empty()) {
        Node* cur = s2.top();
        visitNode(cur);
        s2.pop();
    }
}

void levelOrder(Node* root) {
    if (root == NULL) {
        return;
    }
    queue<Node*> queue;
    queue.push(root);
    while (!queue.empty()) {
        for (size_t i=0; i<queue.size(); i++) {
            Node* node = queue.front();
            queue.pop();
            visitNode(node);
            if (node->left != NULL) {
                queue.push(node->left);
            }
            if (node->right != NULL) {
                queue.push(node->right);
            }
        }
    }
}

}

/*
Tree:
      0
     / \
    1   2
   /   / \
  3   4   5
 / \     / \
6   7   8   9
 \     /     \
 10   11     12

Pre-order: 0 1 3 6 10 7 2 4 5 8 11 9 12
In-order: 6 10 3 7 1 0 4 2 11 8 5 9 12
Post-order: 10 6 7 3 1 4 11 8 12 9 5 2 0
Level-order: 0 1 2 3 4 5 6 7 8 9 10 11 12
*/

int main() {
    Node nodes[13];
    for (int i=0; i<=12; i++){
        nodes[i].val = i;
    }
    nodes[0].left  = &nodes[1];
    nodes[0].right = &nodes[2];
    nodes[1].left  = &nodes[3];
    nodes[2].left  = &nodes[4];
    nodes[2].right = &nodes[5];
    nodes[3].left  = &nodes[6];
    nodes[3].right = &nodes[7];
    nodes[5].left  = &nodes[8];
    nodes[5].right = &nodes[9];
    nodes[6].right = &nodes[10];
    nodes[8].left  = &nodes[11];
    nodes[9].right = &nodes[12];
    
    Node* root = &nodes[0];

    recursive::preOrder(root); 
    cout << endl;
    recursive::inOrder(root);
    cout << endl;
    recursive::postOrder(root);
    cout << endl;

    cout << endl;

    iterarive::preOrder(root);
    cout << endl;
    iterarive::preOrder2(root);
    cout << endl;
    iterarive::inOrder(root);
    cout << endl;
    iterarive::postOrder(root);
    cout << endl;
    iterarive::postOrder2(root);
    cout << endl;
    iterarive::levelOrder(root);
    cout << endl;

    return 0;
}