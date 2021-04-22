#include <iostream>
#include <vector>
#include <random>

#include "btree.hpp"

int main(int argc, char *argv[]) {
    BTree<int, 5> btree;
    std::vector<int> xs;

    for (auto i = 1; i <= 100; i++)
        xs.emplace_back(i);

    std::random_device rd;
    std::mt19937 g(rd());

    std::shuffle(xs.begin(), xs.end(), g);

    for (auto i : xs)
        btree.insert(i);

    // btree.insert(2);
    // btree.insert(1);
    // btree.insert(4);
    // btree.insert(5);
    // btree.insert(3);
    // btree.insert(6);
    // btree.insert(7);
    // btree.insert(8);
    // btree.insert(9);
    // btree.insert(10);
    // btree.insert(11);

    std::cout << btree.format() << '\n';

    return 0;
}
