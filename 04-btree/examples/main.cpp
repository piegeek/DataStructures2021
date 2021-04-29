#include <iostream>
#include <vector>
#include <random>

#include "btree.hpp"

int main(int argc, char *argv[]) {
    BTree<int, 2> tree;
    size_t N = 100000;

    std::vector<int> xs, ys;

    for (auto i = 1; i <= N; i++) {
        xs.push_back(i);
        ys.push_back(i);
    }

    std::random_device rd;
    std::mt19937 g(rd());

    std::shuffle(xs.begin(), xs.end(), g);
    std::shuffle(ys.begin(), ys.end(), g);

    for (auto i : xs)
        tree.insert(i);

    // std::cout << tree.format() << std::endl;

    for (auto i : ys) {
        tree.remove(i);
        // std::cout << tree.format() << std::endl;
    }

    std::cout << tree.root->n << std::endl;
    
    return 0;
}
