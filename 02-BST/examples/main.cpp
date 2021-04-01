#include <iostream>
#include <vector>
#include <random>

#include "BST.hpp"

int main() {

    BST<int> bt;

    bt.insert(5);
    bt.search(5);
    bt.remove(5);

    if (bt.root == nullptr) {
        std::cout << "root is nullptr" << std::endl;
        return 0;
    } else {
        std::cout << "something wrong" << std::endl;
        return -1;
    }

}
