#ifndef __FHEAP_H_
#define __FHEAP_H_

#include <iostream>
#include <initializer_list>
#include <optional>
#include <vector>
#include <cmath>
#include <memory>
#include <queue>
#include <map>

template <typename T>
class PriorityQueue {
    public:
        virtual void insert(const T& item) = 0;
        virtual std::optional<T> extract_min() = 0;
        virtual bool is_empty() const = 0;
};

template <typename T>
class FibonacciNode {
    public:
        // constructors
        FibonacciNode()
            :key(std::nullopt), degree(0), child(nullptr), right(nullptr), mark(false) {}
        FibonacciNode(const T& item)
            :key(item), degree(0), child(nullptr), right(nullptr), mark(false) {}

        // destructor
        ~FibonacciNode();

        T key;
        size_t degree;
        bool mark;

        std::shared_ptr<FibonacciNode<T>> right;
        std::shared_ptr<FibonacciNode<T>> child;
        // NOTE: If you set left pointer to shared_ptr, circular reference may cause!
        // So, left pointer should be set to weak_ptr.
        std::weak_ptr<FibonacciNode<T>> left;
        std::weak_ptr<FibonacciNode<T>> parent;
};

template <typename T>
class FibonacciHeap : public PriorityQueue<T> {
    public:
        // Default Constructor
        FibonacciHeap()
            : min_node(nullptr), size_(0) {}

        // Constructor with Value
        FibonacciHeap(const T& item)
            : min_node(nullptr), size_(0) { insert(item); }

        // Disable copy constructor.
        FibonacciHeap(const FibonacciHeap<T> &);

        // Constructor with initializer_list
        // ex) FibonacciHeap<int> fheap = {1, 2, 3, 4};
        FibonacciHeap(std::initializer_list<T> list): min_node(nullptr), size_(0) {
            for(const T& item : list) {
                insert(item);
            }
        }

        // Destructor
        ~FibonacciHeap();

        // Insert item into the heap.
        void insert(const T& item) override;

        // Return raw pointer of the min node.
        // If the heap is empty, return nullptr.
        FibonacciNode<T>* get_min_node() { return min_node.get(); }

        // Return minimum value of the min node.
        // If the heap is empty, return std::nullopt.
        std::optional<T> get_min() const;

        // 1. Return minimum value of the min node
        // 2. Remove the node which contains minimum value from the heap.
        // If the heap is empty, return std::nullopt;
        std::optional<T> extract_min() override;

        // Decrease the key of the node.
        // Assume that key > update_key.
        void decrease_key(const T& key, const T& update_key);

        // Return true if the heap is empty, false if not.
        bool is_empty() const override { return !size_; }

        // Return the number of nodes the heap contains.
        size_t size() const { return size_; }


    private:
        // Points to the node which has minimum value.
        // Can you guess why is it defined std::shared_ptr, not std::unique_ptr?
        std::shared_ptr<FibonacciNode<T>> min_node;

        // Value that represents how many nodes the heap contains.
        size_t size_;

        // Mapping the key and the node.
        std::map<T, std::weak_ptr<FibonacciNode<T>>> table;

        void insert(std::shared_ptr<FibonacciNode<T>>& node);

        // After extract, clean up the heap.
        void consolidate();

        // Combine two nodes.
        void merge(std::shared_ptr<FibonacciNode<T>>& x, std::shared_ptr<FibonacciNode<T>>& y);
};

template <typename T>
FibonacciNode<T>::~FibonacciNode() {
    // TODO (if necessary)
}

template <typename T>
FibonacciHeap<T>::~FibonacciHeap() {
    // TODO (if necessary)
}

template <typename T>
std::optional<T> FibonacciHeap<T>::get_min() const {
    // TODO
    if (is_empty()) return std::nullopt;
    return min_node->key;
}

template <typename T>
void FibonacciHeap<T>::insert(const T& item) {
    // TODO
    // Create new node
    std::shared_ptr<FibonacciNode<T>> new_node = std::make_shared<FibonacciNode<T>>(item);
    insert(new_node);

}

template <typename T>
void FibonacciHeap<T>::insert(std::shared_ptr<FibonacciNode<T>>& node) {
    // TODO
    // Check if heap empty
    if (is_empty()) {
        // Insert new node into root list
        node->left = node;
        node->right = node;
        // Set new node as min_node 
        min_node = node;
    }
    else {
        // Insert node into root list
        node->right                  = min_node;
        node->left                   = min_node->left;
        min_node->left.lock()->right = node;
        min_node->left               = node;

        // Update min_node
        if (node->key < min_node->key) {
            min_node = node;
        }
    }

    size_++;
}

template <typename T>
std::optional<T> FibonacciHeap<T>::extract_min() {
    // TODO
    if (is_empty()) return std::nullopt;

    // Extract minimum value
    T min_val = min_node->key;

    if (min_node->child) {
        // Add all immediate children of minimum node to root list
        std::shared_ptr<FibonacciNode<T>> first_child = min_node->child;
        std::shared_ptr<FibonacciNode<T>> last_child  = min_node->child->left.lock();

        if (size() == 1) {
            first_child->left = last_child;
            last_child->right = first_child;

            // New minimum node
            min_node = first_child;
        }
        else {
            first_child->left            = min_node->left;
            min_node->left.lock()->right = first_child;
            last_child->right            = min_node->right;
            min_node->right->left        = last_child;
            
            // New minimum node
            std::shared_ptr<FibonacciNode<T>> new_min_node = min_node->right;
            min_node = new_min_node;
            
            consolidate();
        }
        // std::shared_ptr<FibonacciNode<T>> np = first_child;
        // do {
        //     std::shared_ptr<FibonacciNode<T>> next = np->right;

        //     min_node->left.lock()->right = np;
        //     np->right = min_node;
        //     np->left = min_node->left;
        //     min_node->left = np;
            
        //     if (np->key < min_node->key) min_node = np;

        //     // np->parent = nullptr;
        //     np = next;

        // } while (np != first_child);

    }
    else {
        if (size() == 1) {
            // New minimum node
            min_node = nullptr;
        }
        else {
            min_node->left.lock()->right = min_node->right;
            min_node->right->left         = min_node->left;

            // New minimum node
            std::shared_ptr<FibonacciNode<T>> new_min_node = min_node->right;
            min_node = new_min_node;
            
            consolidate();
        }

    }
    // Remove min node
    // min_node->left.lock()->right = min_node->right;
    // min_node->right->left = min_node->left;

    // if (min_node == min_node->right && min_node->child == nullptr) {
    //     min_node = nullptr;
    // }
    // else {
    //     min_node = min_node->right;
    //     consolidate();
    // }

    // size_--;


    size_--;
 
    return min_val;
}

template <typename T>
void FibonacciHeap<T>::consolidate() {
    // TODO

    const double phi = 1.61803398875;
    size_t arr_size = std::floor(std::log(size()) / std::log(2));
    std::vector<std::shared_ptr<FibonacciNode<T>>> degree_arr(arr_size);

    for (auto& ptr : degree_arr) ptr = nullptr;

    std::shared_ptr<FibonacciNode<T>> np = min_node;

    do {
        std::shared_ptr<FibonacciNode<T>> x = np;
        size_t deg = x->degree;

        while(degree_arr[deg]) {
            std::shared_ptr<FibonacciNode<T>> y = degree_arr[deg];
            if (x->key > y->key) {
                std::swap(x, y);
            }

            if (y == min_node) min_node = x; // x has to be new min_node because y is going to be the child after merge
            merge(y, x); // y is child of x
            if (x->right == x) min_node = x;

            degree_arr[deg] = nullptr;
            deg = (deg + 1) % arr_size;
        }
        degree_arr[deg] = x;
        np = np->right;
    } while (np != min_node);

    min_node = nullptr;
    
    // const size_t degree_arr_size = std::floor(std::log(size()) / std::log(phi)) + 2;
    // size_t deg;

    // std::vector<std::shared_ptr<FibonacciNode<T>>> degree_arr(degree_arr_size);

    // std::shared_ptr<FibonacciNode<T>> np = min_node;
    // do {
    //     deg = np->degree;

    //     while (degree_arr[deg]) {
    //         std::shared_ptr<FibonacciNode<T>> t = degree_arr[deg];

    //         if (np->key > t->key) {
    //            std::shared_ptr<FibonacciNode<T>> temp = np; 
    //            np = t;
    //            t = temp;
    //         }
    //         if (t == min_node) min_node = np;
    //         merge(t, np);
    //         if (np->right == np) min_node = np;

    //         degree_arr[deg] = nullptr;
    //         deg++;
    //     }
    //     degree_arr[deg] = np;
    //     np = np->right;
    // } while (np != min_node);

    min_node = nullptr;

    for (auto& node : degree_arr) {
        if (node) {
            node->left = node;
            node->right = node;
            if (min_node) {
                min_node->left.lock()->right = node;
                node->right = min_node;
                node->left = min_node->left;
                min_node->left = node;
                if (node->key < min_node->key) min_node = node;
            }
            else {
                min_node = node;
            }
            if (!min_node) {
                min_node = node;
            }
            else if (node->key < min_node->key) min_node = node;
        }
    }
}

template <typename T>
void FibonacciHeap<T>::merge(std::shared_ptr<FibonacciNode<T>>& x, std::shared_ptr<FibonacciNode<T>>& y) {
    // TODO           
    x->left.lock()->right = x->right;
    x->right->left = x->left;

    if (y->right == y) {
        min_node = y;
    }
    x->left = x;
    x->right = x;
    x->parent = y;
    if (y->child == nullptr) {
        y->child = x;
    }

    x->right = y->child;
    x->left = y->child->left;
    y->child->left.lock()->right = x;
    y->child->left = x;

    if (x->key < y->child->key) {
        y->child = x;
    }

    y->degree++;

    // if (x == nullptr) {
    //     x = y;
    //     return;
    // } 
    // if (y == nullptr) return;
    // if (x->key > y->key) {
    //     std::shared_ptr<FibonacciNode<T>> temp = x;
    //     x = y;
    //     y = temp;
    // }


    // std::shared_ptr<FibonacciNode<T>> xn = x->right;
    // std::shared_ptr<FibonacciNode<T>> yp = y->left.lock();

    // x->right = y;
    // y->left.lock() = x;
    // xn->left = yp;
    // yp->right = xn;
}

template <typename T>
void FibonacciHeap<T>::decrease_key(const T& key, const T& update_key) {
    // TODO
}

#endif // __FHEAP_H_
