#include <iostream>
#include <optional>
#include <memory>

template <typename T>
class TreeNode
{
    public:
        T element;
        std::unique_ptr<TreeNode<T>> left;
        std::unique_ptr<TreeNode<T>> right;

        int height;

        TreeNode<T>(const T& e)
            :element{e}, left{nullptr}, right{nullptr}, height{0} {}

        ~TreeNode() {}

};


template <typename T>
class AVLTree
{
    public:
        std::unique_ptr<TreeNode<T>> root = nullptr;

        ~AVLTree() = default;

        bool insert(const T& key); 
        bool remove(const T& key);

        int get_height(std::unique_ptr<TreeNode<T>>& n) const; 
        int get_balance_factor(std::unique_ptr<TreeNode<T>>& n) const; 


    private:
        bool insert(std::unique_ptr<TreeNode<T>>& n, const T& key); 
        bool remove(std::unique_ptr<TreeNode<T>>& n, const T& key);
        void balance(std::unique_ptr<TreeNode<T>>& n);

        void left_rotate(std::unique_ptr<TreeNode<T>>& n);
        void right_rotate(std::unique_ptr<TreeNode<T>>& n);
        void left_right_rotate(std::unique_ptr<TreeNode<T>>& n);
        void right_left_rotate(std::unique_ptr<TreeNode<T>>& n);

        T find_rightmost_key(std::unique_ptr<TreeNode<T>>& n) const;

};

template <typename T>
bool AVLTree<T>::insert(const T& key) {
    return insert(root, key);
}

template <typename T>
bool AVLTree<T>::remove(const T& key) {
    return remove(root, key);
}

template <typename T>
bool AVLTree<T>::insert(std::unique_ptr<TreeNode<T>>& n, const T& key) {
    // TODO
}

template <typename T>
bool AVLTree<T>::remove(std::unique_ptr<TreeNode<T>>& n, const T& key) {
    // TODO
}


template <typename T>
int AVLTree<T>::get_height(std::unique_ptr<TreeNode<T>>& n) const {
    return n == nullptr ? -1 : n->height;
}

template <typename T>
int AVLTree<T>::get_balance_factor(std::unique_ptr<TreeNode<T>>& n) const {
    // TODO
}


template <typename T>
void AVLTree<T>::balance(std::unique_ptr<TreeNode<T>>& n) {
    // You do not have to use/write balance function if you think it is unnecessary.
}


template <typename T>
void AVLTree<T>::left_rotate(std::unique_ptr<TreeNode<T>>& n) {
    // TODO
}

template <typename T>
void AVLTree<T>::right_rotate(std::unique_ptr<TreeNode<T>>& n) {
    // TODO
}

template <typename T>
void AVLTree<T>::left_right_rotate(std::unique_ptr<TreeNode<T>>& n) {
    // TODO
}

template <typename T>
void AVLTree<T>::right_left_rotate(std::unique_ptr<TreeNode<T>>& n) {
    // TODO
}

template <typename T>
T AVLTree<T>::find_rightmost_key(std::unique_ptr<TreeNode<T>>& n) const { 
    // You do not have to use/write this function if you think it is unnecessary.
}
