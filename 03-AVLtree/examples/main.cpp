#include <iostream>
#include <vector>
#include <stack>
#include <random>
#include <algorithm>

#include "AVLTree.hpp"

template <typename T>
bool is_AVL(std::unique_ptr<AVLTree<T>> &t) {

    if (t->root == nullptr) return false;

    std::stack<TreeNode<T>*> s1;

    s1.push(t->root.get());

    while (!s1.empty()) {

        auto node = s1.top();

        auto n_h = node->height;
        int l_h = (node->left == nullptr) ? -1 : node->left->height;
        int r_h = (node->right == nullptr) ? -1 : node->right->height;
        int bf = l_h - r_h;
        
        if (std::abs(bf) >= 2){
            std::cout << "balance factor err" << std::endl;
            print_avl_tree(node);
            return false;
        }

        if (n_h != std::max(l_h, r_h) + 1) {
            std::cout << "node height err" << std::endl;
            print_avl_tree(node);
            return false;
        }

        s1.pop();

        auto ele = node->element;

        if (node->left != nullptr) {
//            REQUIRE(ele > node->left->element);
            if (!(ele > node->left->element))  {
                std::cout << "parent smaller than left err" << std::endl;
                print_avl_tree(node);
                return false;
            }
            s1.push(node->left.get());
        }
        if (node->right != nullptr) {
//            REQUIRE(ele < node->right->element);
            if (!(ele < node->right->element)) {
                std::cout << "parent larger than right err" << std::endl;
                print_avl_tree(node);
                return false;
            }
            
            s1.push(node->right.get());
        }
    }

    return true;
}

template <typename T>
void print_avl_tree(TreeNode<T>* t, int depth = 0) {
    if (t == nullptr) return;

    int to_print = t->element;
    std::cout << std::string(4 * depth, ' ') << t->element << "(" << t->height << ")" << std::endl;

    print_avl_tree(t->left.get(), depth+1);
    print_avl_tree(t->right.get(), depth+1);
}

int main() {

    auto tree = std::make_unique<AVLTree<int>>();
    std::random_device rd;
    std::mt19937 g(rd());

    int tree_size = 50;
    std::vector<int> v;
    v.resize(tree_size);
    std::generate(v.begin(), v.end(), std::rand);
    std::sort(v.begin(), v.end());
    v.erase(std::unique(v.begin(), v.end()), v.end());
    std::shuffle(v.begin(), v.end(), g);

    tree_size = v.size();

    for (auto ele: v) {
        tree->insert(ele);
    }

    std::shuffle(v.begin(), v.end(), g);   
    auto x = std::vector<int>(v.begin(), v.begin() + std::distance(v.begin(), v.end()) / 2);

    for (auto ele: x) {
        tree->remove(ele);
        if (is_AVL(tree) == false) {
            is_AVL(tree);
            print_avl_tree(tree->root.get());
        } 
    }


    // auto bt = AVLTree<int>();

    // for (auto i = 1; i <= 100; i++) {
    //     bt.insert(i);
    // }

    // TreeNode<int>* root_ptr = bt.root.get(); 

    // for (auto i = 1; i <= 100; i++) {
    //     if (is_AVL(bt) == false) {
    //     std::cout << i << std::endl;
    //     }
    //     // std::cout << "BEFORE DELETE: " << i << std::endl;
    //     // root_ptr = bt.root.get(); 
    //     // print_avl_tree(root_ptr);

    //     bt.remove(i);

    //     // std::cout << "AFTER DELETE: " << i << std::endl;
    //     // root_ptr = bt.root.get(); 
    //     // print_avl_tree(root_ptr);
    // }

    // // TreeNode<int>* root_ptr = bt.root.get(); 

    // // print_avl_tree(root_ptr);
    
    // // std::cout <<"height should be 1" << std::endl;
    // // std::cout <<"height is " << bt.get_height(bt.root) << std::endl;

    // // bt.remove(4);
    // // bt.remove(2);

    // // std::cout <<"height should be 1" << std::endl;
    // // std::cout <<"height is " << bt.get_height(bt.root) << std::endl;


    return 0;
}
