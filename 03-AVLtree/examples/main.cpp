#include <iostream>
#include <vector>
#include <random>
#include <algorithm>

#include "AVLTree.hpp"

int main() {

    auto bt = AVLTree<int>();

    bt.insert(3);
    bt.insert(1);
    bt.insert(2);
    
    std::cout <<"height should be 1" << std::endl;
    std::cout <<"height is " << bt.get_height(bt.root) << std::endl;

    bt.remove(4);
    bt.remove(2);

    std::cout <<"height should be 1" << std::endl;
    std::cout <<"height is " << bt.get_height(bt.root) << std::endl;


    return 0;
}
