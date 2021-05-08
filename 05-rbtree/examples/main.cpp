#include <random>
#include <vector>
#include <algorithm>
#include <bitset>

#include "rbtree.hpp"

int main(int argc, char *argv[]) {
    // std::vector<int> xs{};
    // RBTree<int> rbtree{};
    // size_t N = 9;

    // for (auto i = 1; i <= N; i++)
    //     xs.emplace_back(i);

    // std::random_device rd;
    // std::mt19937 g(rd());

    // std::shuffle(xs.begin(), xs.end(), g);

    // for (auto i : xs) {
    //     rbtree.insert(i);
    //     std::cout << rbtree << '\n';
    // }

    // const auto& all_leaves = rbtree.collect_all_leaves();
    // for (const auto &[p, n] : all_leaves)
    //     std::cout << "path: " << std::bitset<16>{p.p_}
    //               << "\tlen: " << p.len_
    //               << "\tblack: " << p.num_black_ << '\n';

    RBTree<int> rbtree;
    size_t n = 10;

    std::vector<int> xs;

    for (auto i = 0; i < n; i++)
        xs.emplace_back(i);

    std::random_device rd;
    std::mt19937 g(rd());

    std::shuffle(xs.begin(), xs.end(), g);

    for (auto x : xs)
        rbtree.insert(x);

    std::cout << rbtree.format_graphviz() << std::endl;

    for (auto j = 0; j < xs.size(); j++) {
        if (j > xs[j]) {
            std::cout << rbtree.format_graphviz() << std::endl;

            std:: cout << rbtree.contains(xs[j]) << std::endl;
            rbtree.remove(xs[j]);
            std::cout << rbtree.contains(xs[j]) << std::endl;

            // auto is_left_lean = test_left_lean(rbtree);
            // REQUIRE(is_left_lean);

            // auto is_black_balance = test_black_balance(rbtree);
            // REQUIRE(is_black_balance);
        }

    }

    return 0;
}
