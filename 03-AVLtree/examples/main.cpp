#include <iostream>
#include <vector>
#include <random>
#include <algorithm>

#include "AVLTree.hpp"

template <typename T>
void print_avl_tree(TreeNode<T>* t, int depth = 0) {
    if (t == nullptr) return;

    int to_print = t->element;
    std::cout << std::string(4 * depth, ' ') << to_print << std::endl;

    print_avl_tree(t->left.get(), depth+1);
    print_avl_tree(t->right.get(), depth+1);
}

int main() {

    auto bt = AVLTree<int>();

    for (auto i = 1; i <= 1000; i++) {
        bt.insert(i);
    }

    TreeNode<int>* root_ptr = bt.root.get(); 

    print_avl_tree(root_ptr);
    
    // std::cout <<"height should be 1" << std::endl;
    // std::cout <<"height is " << bt.get_height(bt.root) << std::endl;

    // bt.remove(4);
    // bt.remove(2);

    // std::cout <<"height should be 1" << std::endl;
    // std::cout <<"height is " << bt.get_height(bt.root) << std::endl;


    return 0;
}
