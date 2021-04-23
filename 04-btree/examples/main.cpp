#include <iostream>
#include <vector>
#include <random>

#include "btree.hpp"

int main(int argc, char *argv[]) {
    BTree<int, 3> btree;
    // std::vector<int> xs;

    for (auto i = 1; i <= 10'000; i++)
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
    // btree.insert(9); // **k
    // btree.insert(10);
    // btree.insert(11);
    // btree.insert(12);

    auto depth = btree.depth();

    std::cout << btree.format() << '\n';
    
    // std::cout << btree.find_rightmost_key().has_value() << std::endl;
    // std::cout << (btree.find_rightmost_key().value() == 10000) << std::endl;
    // std::cout << depth.has_value() << std::endl;

    // std::cout << depth.value() << std::endl;

    // std::cout << "Internal nodes" << std::endl;
    // for (auto i = 0; i < depth.value(); i++) {
    //     auto nodes_at_level_i = btree.root->find_nodes_at_level(i);

    //     std::cout << std::all_of(nodes_at_level_i.begin(),
    //                         nodes_at_level_i.end(),
    //                         [](const BTreeNode<int, 7>* n){
    //                             return n->type == NodeType::INTERNAL;
    //                         }) 
    //     << std::endl;
    // }
    
    // std::cout << "leaf nodes" << std::endl;
    // auto nodes_at_leaf_level = btree.root->find_nodes_at_level(depth.value());
    // std::cout << std::all_of(nodes_at_leaf_level.begin(),
    //                     nodes_at_leaf_level.end(),
    //                     [](const BTreeNode<int, 7>* n){
    //                         return n->type == NodeType::LEAF;
    //                     })
    // << std::endl;

    return 0;
}
