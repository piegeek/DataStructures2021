#include <algorithm>
#include <vector>
#include <random>
#include <cmath>
#include <stack>

#include "AVLTree.hpp"

#define CATCH_CONFIG_MAIN
#include <catch2/catch.hpp>


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


TEST_CASE("AVLTree height test", "[AVL]") {

    auto tree = std::make_unique<AVLTree<int>>();

    REQUIRE(tree->get_height(tree->root) == -1);
    REQUIRE(tree->get_balance_factor(tree->root) == 0);

    tree->root = std::make_unique<TreeNode<int>>(3);
    REQUIRE(tree->get_height(tree->root) == 0);
    REQUIRE(tree->get_height(tree->root) == tree->root->height);
    REQUIRE(tree->get_balance_factor(tree->root) == 0);

    tree->root->left = std::make_unique<TreeNode<int>>(1);
    tree->root->height += 1;
    REQUIRE(tree->get_height(tree->root) == 1);
    REQUIRE(tree->get_height(tree->root) == tree->root->height);
    REQUIRE(tree->get_balance_factor(tree->root) == 1);

    tree->root->right = std::make_unique<TreeNode<int>>(4);
    REQUIRE(tree->get_height(tree->root) == 1);
    REQUIRE(tree->get_height(tree->root) == tree->root->height);
    REQUIRE(tree->get_balance_factor(tree->root) == 0);

    tree->root->right->right = std::make_unique<TreeNode<int>>(5);
    tree->root->height += 1;
    tree->root->right->height += 1;
    REQUIRE(tree->get_height(tree->root) == 2);
    REQUIRE(tree->get_height(tree->root) == tree->root->height);
    REQUIRE(tree->get_balance_factor(tree->root) == -1);

}


TEST_CASE("AVLTree insert test", "[AVL]") {

    auto tree = std::make_unique<AVLTree<int>>();

    std::vector<int> v;
    v.resize(1 << 10);
    std::generate(v.begin(), v.end(), std::rand);
    std::sort(v.begin(), v.end());
    v.erase(std::unique(v.begin(), v.end()), v.end());
    std::random_shuffle(v.begin(), v.end());

    for (auto ele: v) {
        REQUIRE(tree->insert(ele) == true);
        REQUIRE(tree->insert(ele) == false);
        REQUIRE(is_AVL(tree) == true);
    }

}

TEST_CASE("AVLTree remove test", "[AVL]") {

    auto tree = std::make_unique<AVLTree<int>>();

    int tree_size = 1 << 10;
    std::vector<int> v;
    v.resize(tree_size);
    std::generate(v.begin(), v.end(), std::rand);
    std::sort(v.begin(), v.end());
    v.erase(std::unique(v.begin(), v.end()), v.end());
    std::random_shuffle(v.begin(), v.end());

    tree_size = v.size();

    for (auto ele: v) {
        tree->insert(ele);
    }

    std::random_shuffle(v.begin(), v.end());   
    auto x = std::vector<int>(v.begin(), v.begin() + std::distance(v.begin(), v.end()) / 2);

    for (auto ele: x) {
        REQUIRE(tree->remove(ele) == true);
        REQUIRE(tree->remove(ele) == false);
        REQUIRE(is_AVL(tree) == true);
    }

}
