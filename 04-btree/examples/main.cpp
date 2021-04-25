#include <iostream>
#include <vector>
#include <random>

#include "btree.hpp"

int main(int argc, char *argv[]) {
    BTree<int, 3> btree;
    std::vector<int> xs;

    for (auto i = 1; i <= 10000; i++)
        xs.emplace_back(i);

    std::random_device rd;
    std::mt19937 g(rd());

    std::shuffle(xs.begin(), xs.end(), g);

    for (auto i : xs)
        btree.insert(i);

    std::shuffle(xs.begin(), xs.end(), g);

    for (auto i : xs) {
        btree.remove(i);
        std::cout << btree.format() << std::endl;
    }
    
    return 0;
}
