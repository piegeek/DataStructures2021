#include <iostream>
#include <vector>
#include <stack>
#include <random>
#include <algorithm>

#include "AVLTree.hpp"

template <typename T>
bool is_AVL(AVLTree<T> &bt) {
    if (bt.root == nullptr) return false;

    std::stack<TreeNode<T>*> s1;

    s1.push(bt.root.get());

    while (!s1.empty()) {

        auto node = s1.top();

        auto n_h = node->height;
        int l_h = (node->left == nullptr) ? -1 : node->left->height;
        int r_h = (node->right == nullptr) ? -1 : node->right->height;
        int bf = l_h - r_h;
        
        if (std::abs(bf) >= 2) return false;

        if (n_h != std::max(l_h, r_h) + 1) return false;

        s1.pop();

        auto ele = node->element;

        if (node->left != nullptr) {
//            REQUIRE(ele > node->left->element);
            if (!(ele > node->left->element))  return false;
            s1.push(node->left.get());
        }
        if (node->right != nullptr) {
//            REQUIRE(ele < node->right->element);
            if (!(ele < node->right->element)) return false;
            
            s1.push(node->right.get());
        }
    }

    return true;
}

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

    for (auto i = 1; i <= 10; i++) {
        bt.insert(i);
    }

    TreeNode<int>* root_ptr = bt.root.get(); 

    for (auto i = 1; i <= 10; i++) {
        std::cout << is_AVL(bt) << std::endl;
        bt.remove(i);
    }

    // TreeNode<int>* root_ptr = bt.root.get(); 

    // print_avl_tree(root_ptr);
    
    // std::cout <<"height should be 1" << std::endl;
    // std::cout <<"height is " << bt.get_height(bt.root) << std::endl;

    // bt.remove(4);
    // bt.remove(2);

    // std::cout <<"height should be 1" << std::endl;
    // std::cout <<"height is " << bt.get_height(bt.root) << std::endl;


    return 0;
}
