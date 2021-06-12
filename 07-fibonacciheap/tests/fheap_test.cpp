#define CATCH_CONFIG_MAIN
#include <catch2/catch.hpp>
#include "fheap.hpp"
#include <iostream>
#include <queue>
#include <vector>
#include <random>
#include <algorithm>
#include <map>

template <typename T>
void traverse_root_list(FibonacciNode<T>* min_node) {
    if(!min_node) {
        return;
    }
    std::queue<T> root_key;
    root_key.push(min_node->key);
    FibonacciNode<T>* temp = min_node->right.get();
    while(temp != min_node) {
        root_key.push(temp->key);
        temp = temp->right.get();
    }
    while(!root_key.empty()) {
        T item = root_key.front();
        root_key.pop();
    }
}

template <typename T>
std::vector<FibonacciNode<T>*> get_root_list(FibonacciHeap<T>& h) {
    std::vector<FibonacciNode<T>*> root_list;

    if(h.is_empty())
        return root_list;

    FibonacciNode<T>* node = h.get_min_node();
    do {
        root_list.push_back(node);
        node = node->right.get();
    } while(node != h.get_min_node());

    return root_list;
}

template <typename T>
void check_min_heap_property(FibonacciNode<T>* n) {
    if(!n->child)
        return;

    std::vector<FibonacciNode<T>*> children_list;
    FibonacciNode<T>* node = n->child.get();
    do {
        children_list.push_back(node);
        node = node->right.get();
    } while(node != n->child.get());

    for(FibonacciNode<T>* child: children_list) {
        REQUIRE(n->key <= child->key);
        check_min_heap_property(child);
    }
}

TEST_CASE("Fibonacci Heap Construction", "[Integer]") {
    SECTION("Default Construction") {
        FibonacciHeap<int> heap = {};
        REQUIRE(heap.size() == 0);
        REQUIRE(heap.is_empty() == true);
        REQUIRE(heap.get_min() == std::nullopt);
    }

    SECTION("Construction with Value") {
        FibonacciHeap<int> heap(3);
        REQUIRE(heap.get_min().value() == 3);
    }

    SECTION("Construction with Initialize List") {
        FibonacciHeap<int> heap = { 2, 1 };
        REQUIRE(heap.get_min().value() == 1);
    }
}

TEST_CASE("Insertion", "[Integer]") {
    FibonacciHeap<int> heap = {};
    std::vector<int> inserted;

    SECTION("After One Insertion") {
        heap.insert(3);
        REQUIRE(heap.get_min() == 3);
    }

    SECTION("After Several Insertions") {
        std::random_device rd;
        std::mt19937 g(rd());
        std::uniform_int_distribution<size_t> dis(0,1000);
        size_t num_ins = 10;

        for(int i = 0 ; i < num_ins ; ++i) {
            int temp = dis(g);
            inserted.push_back(temp);
        }

        std::sort(inserted.begin(), inserted.end());
        inserted.erase(std::unique(inserted.begin(), inserted.end()), inserted.end());
        std::shuffle(inserted.begin(), inserted.end(), g);
        num_ins = inserted.size();

        for(int i = 0 ; i < num_ins ; ++i) {
            heap.insert(inserted[i]);
        }

        int min_value = *std::min_element(inserted.begin(), inserted.end());
        REQUIRE(heap.get_min().value() == min_value);
        REQUIRE(heap.is_empty() == false);
        REQUIRE(heap.size() == num_ins);
    }
}

TEST_CASE("Simple Extraction", "[Integer]") {
    FibonacciHeap<int> heap = {10};
    std::optional<int> min_value;

    SECTION("Extraction when only one element exists") {
        min_value = heap.extract_min();
        REQUIRE(min_value.value() == 10);
        REQUIRE(heap.get_min_node() == nullptr);
        REQUIRE(heap.get_min() == std::nullopt);
        REQUIRE(heap.is_empty() == true);
        REQUIRE(heap.size() == 0);
    }

    SECTION("Extraction after several insertions") {
        for(int i = 9 ; i > 0 ; --i) {
            heap.insert(i);
        }
        min_value = heap.extract_min();

        // Check Attributes that doesn't affected by consolidate.
        REQUIRE(min_value.value() == 1);
        REQUIRE(heap.is_empty() == false);
        REQUIRE(heap.size() == 9);
        REQUIRE(heap.get_min() == 2);

        std::map<int, bool> degree_map;
        std::vector<FibonacciNode<int>*> root_list = get_root_list(heap);
        for(FibonacciNode<int>* root : root_list) {
            // Every root in the root list has a distinct degree value
            REQUIRE(degree_map[root->degree] == false);
            degree_map[root->degree] = true;

            check_min_heap_property(root);
        }
    }
}

TEST_CASE("Random Insertion and Extraction", "[Integer]") {
    size_t N = 10;
    for(size_t i = 0 ; i < N; ++i) {
        FibonacciHeap<int> heap;
        std::vector<int> inserted;
        std::vector<int> extracted;
        // Prepare a random vector
        std::random_device rd;
        std::mt19937 g(rd());
        std::uniform_int_distribution<size_t> dis_ins(200,500);
        std::uniform_int_distribution<size_t> dis_ext(100, 200);
        std::uniform_int_distribution<size_t> dis_key(0, 10000);

        size_t num_ins = dis_ins(g);
        size_t num_ext = dis_ext(g);

        for(size_t j = 0 ; j < num_ins ; ++j) {
            int v = dis_key(g);
            inserted.emplace_back(v);
        }

        std::sort(inserted.begin(), inserted.end());
        inserted.erase(std::unique(inserted.begin(), inserted.end()), inserted.end());
        std::shuffle(inserted.begin(), inserted.end(), g);
        num_ins = inserted.size();

        if(num_ins < num_ext)
            continue;

        for(size_t j = 0 ; j < num_ins ; ++j) {
            heap.insert(inserted[j]);
        }
        // TODO:REMOVE
        std::cout << "inserted" << std::endl;
        for (auto i : inserted) {
            std::cout << i << std::endl;
        }

        for(size_t k = 0 ; k < num_ext ; ++k) {
            int ext = heap.extract_min().value();
            extracted.emplace_back(ext);
        }
        // TODO:REMOVE
        std::cout << "extracted" << std::endl;
        for (auto i : extracted) {
            std::cout << i << std::endl;
        }


        std::sort(inserted.begin(), inserted.end(), std::less<int>());
        for(size_t l = 0 ; l < num_ext ; ++l) {
            REQUIRE(inserted[l] == extracted[l]);
        }
        inserted = std::vector<int>(inserted.begin() + num_ext, inserted.end());


        // Every root in the root list has a distinct degree value
        std::map<int, bool> degree_map;
        std::vector<FibonacciNode<int>*> root_list = get_root_list(heap);
        for(FibonacciNode<int>* root : root_list) {
            REQUIRE(degree_map[root->degree] == false);
            degree_map[root->degree] = true;
        }
    }
}

void decrease_key_simple_test(void) {
    // 2 - 3 - 5 - 7 - 9
    FibonacciHeap<int> heap = {2, 3, 5, 7, 9};

    // 1 - 2 - 5 - 7 - 9
    heap.decrease_key(3, 1);

    REQUIRE(heap.get_min().value() == 1);
}

void decrease_key_random_test(void) {
    size_t N = 10;
    size_t num_dec = 10;

    for(size_t i = 0 ; i < N; ++i) {
        FibonacciHeap<int> heap;
        std::vector<int> inserted;
        // Prepare a random vector
        std::random_device rd;
        std::mt19937 g(rd());
        std::uniform_int_distribution<size_t> dis_ins(200,500);
        std::uniform_int_distribution<size_t> dis_key(0, 10000);

        size_t num_ins = dis_ins(g);

        for(size_t j = 0 ; j < num_ins ; ++j) {
            int v = dis_key(g);
            inserted.emplace_back(v*2 + 2);
        }

        std::sort(inserted.begin(), inserted.end());
        inserted.erase(std::unique(inserted.begin(), inserted.end()), inserted.end());
        std::shuffle(inserted.begin(), inserted.end(), g);
        num_ins = inserted.size();


        for(size_t j = 0 ; j < num_ins ; ++j) {
            heap.insert(inserted[j]);
        }

        heap.extract_min();

        // decrease_key
        std::uniform_int_distribution<size_t> dis(0, num_ins-2);
        std::map<int, bool> dec_map;
        for(size_t i = 0; i < num_dec; ++i){
            size_t i1 = dis(g);
            size_t i2 = dis(g);
            if(dec_map[i1] || dec_map[i2])
                continue;
            dec_map[i1] = true;
            dec_map[i2] = true;
            heap.decrease_key(std::max(inserted[i1], inserted[i2]), std::min(inserted[i1], inserted[i2]) - 1);
        }


        // Heap holds min-heap property.
        check_min_heap_property(heap.get_min_node());

        heap.extract_min();

        // Every root in the root list has a distinct degree value
        std::map<int, bool> degree_map;
        std::vector<FibonacciNode<int>*> root_list = get_root_list(heap);
        for(FibonacciNode<int>* root : root_list) {
            REQUIRE(degree_map[root->degree] == false);
            degree_map[root->degree] = true;
        }
    }
}

TEST_CASE("decrease key", "[fheap]") {
    decrease_key_simple_test();
    decrease_key_random_test();
}