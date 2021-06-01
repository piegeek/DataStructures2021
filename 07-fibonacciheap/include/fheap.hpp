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
    return std::nullopt;
}

template <typename T>
void FibonacciHeap<T>::insert(const T& item) {
    // TODO
}

template <typename T>
void FibonacciHeap<T>::insert(std::shared_ptr<FibonacciNode<T>>& node) {
    // TODO
}

template <typename T>
std::optional<T> FibonacciHeap<T>::extract_min() {
    // TODO
    return std::nullopt;
}

template <typename T>
void FibonacciHeap<T>::consolidate() {
    // TODO
}

template <typename T>
void FibonacciHeap<T>::merge(std::shared_ptr<FibonacciNode<T>>& x, std::shared_ptr<FibonacciNode<T>>& y) {
    // TODO
}

template <typename T>
void FibonacciHeap<T>::decrease_key(const T& key, const T& update_key) {
    // TODO
}

#endif // __FHEAP_H_
