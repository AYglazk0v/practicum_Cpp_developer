#include <iostream>
#include <cassert>

using namespace std;

template <class T>
struct TreeNode {
    T value;
    TreeNode* parent = nullptr;
    TreeNode* left = nullptr;
    TreeNode* right = nullptr;
};

template <class T>
void DeleteTree(TreeNode<T>* node) {
    if (!node) {
        return;
    }
    DeleteTree(node->left);
    DeleteTree(node->right);
    delete node;
}

template <class T>
void PrintTree(const TreeNode<T>* root, ostream& out = cout) {
    out << " ( "s;
    out << root->value;
    if (root->left || root->right) {
        if (root->left) {
            PrintTree(root->left, out);
        } else {
            out << "*"s;
        }
        if (root->right) {
            PrintTree(root->right, out);
        } else {
            out << "*"s;
        }
    }
    out << " ) "s;
}

template <class T>
ostream& operator << (ostream& out, const TreeNode<T>* node) {
    PrintTree(node, out);
    return out;
}

template <class T>
TreeNode<T>* begin(TreeNode<T>* node) {
    TreeNode<T>* tmp = node;
    while (tmp->left) {
        tmp = tmp->left;
    }
    return tmp;
}

template <class T>
TreeNode<T>* next(TreeNode<T>* node) {
    if (node->right) {
        return begin(node->right);
    }
    auto tmp = node->parent;
    while(tmp && node == tmp->right) {
        node = tmp;
        tmp = tmp->parent;
    }
    return tmp;
}

// функция создаёт новый узел с заданным значением и потомками
TreeNode<int>* N(int val, TreeNode<int>* left = nullptr, TreeNode<int>* right = nullptr) {
    auto res = new TreeNode<int>{val, nullptr, left, right};
    if (left) {
        left->parent = res;
    }
    if (right) {
        right->parent = res;
    }

    return res;
}

int main() {
    using T = TreeNode<int>;

    T* root = N(6, N(4, N(3), N(5)), N(8, N(7)));
    cout << root << endl;

    T* iter = begin(root);

    while (iter) {
        cout << iter->value << " "s;
        iter = next(iter);
    }
    cout << endl;

    DeleteTree(root);
}