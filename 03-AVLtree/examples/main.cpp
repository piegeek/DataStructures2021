#include <iostream>
#include <vector>
#include <random>
#include <algorithm>

#include "AVLTree.hpp"

template <typename T>
void print_avl_tree(TreeNode<T>* t, int depth = 0) {
    int to_print = t == nullptr ? 0 : t->element;

    std::cout << std::string(4 * depth, ' ') << to_print << std::endl;
    print_avl_tree(t->left.get(), depth+1);
    print_avl_tree(t->left.get(), depth+1);
}

int main() {

    auto bt = AVLTree<int>();

    bt.insert(3);
    bt.insert(1);
    bt.insert(2);
    bt.insert(4);
    bt.insert(5);
    bt.insert(7);
    bt.insert(19);

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
