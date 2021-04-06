#include <iostream>
#include <vector>
#include <random>

#include "BST.hpp"

// template <typename T>
// void print_bst(BST<T>& bt) {
//     print_tree_node(bt.root);
// }

// template <typename T>
// void print_tree_node(std::unique_ptr<TreeNode<T>> t) {

// }

int main() {

    BST<int> bt;

    // bt.insert(5);
    // bt.search(5);
    // bt.remove(5);

    // if (bt.root == nullptr) {
    //     std::cout << "root is nullptr" << std::endl;
    //     return 0;
    // } else {
    //     std::cout << "something wrong" << std::endl;
    //     return -1;
    // }

    for (int i = 0; i < 50; i++) {
        bt.insert(i);
    }

    // print_bst(bt);

    return 0;

}
