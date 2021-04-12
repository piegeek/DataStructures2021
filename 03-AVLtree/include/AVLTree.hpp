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
    // Is empty tree
    if (n == nullptr) {
        std::unique_ptr<TreeNode<T>> new_node = std::make_unique<TreeNode<T>>(key);
        n = std::move(new_node);

        return true;
    }

    // Extract val
    T val = n->element;

    if (key == val) return false; // Already exists
    
    if (key < val) {
        // No left child : Insert directly to the left
        if (n->left == nullptr) {
            std::unique_ptr<TreeNode<T>> new_node = std::make_unique<TreeNode<T>>(key);
            n->left = std::move(new_node);
        }
        else {
            // Traverse down and insert
            insert(n->left, key);
            // Balance
            balance(n);
        }

        return true;
    }
    if (key > val) {
        // No right child : Insert directly to the right 
        if (n->right == nullptr) {
            std::unique_ptr<TreeNode<T>> new_node = std::make_unique<TreeNode<T>>(key);
            n->right = std::move(new_node);
        }
        else {
            // Traverse down and insert
            return insert(n->right, key);
            // Balance
            balance(n);
        }

        return true;
    }
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
    int l_height = get_height(n->left);
    int r_height = get_height(n->right);

    return n == nullptr ? 0 : l_height - r_height;
}


template <typename T>
void AVLTree<T>::balance(std::unique_ptr<TreeNode<T>>& n) {
    // You do not have to use/write balance function if you think it is unnecessary.
    int balance_factor = get_balance_factor(n);

    if (balance_factor > 1) { // Left is deeper than right by a factor of 2; happens when inserting on the left
        int left_left_h  = get_height(n->left->left);
        int left_right_h = get_height(n->left->right);

        if (left_left_h > left_right_h) left_right_rotate(n);
        if (left_left_h < left_right_h) right_rotate(n);
    }
    if (balance_factor < -1) { // Left is deeper than right by a factor of 2; happens when inserting on the left
        int right_right_h = get_height(n->right->right);
        int right_left_h  = get_height(n->right->left);

        if (right_right_h > right_left_h) right_left_rotate(n);
        if (right_right_h < right_left_h) left_rotate(n);
    }

    n->height = std::max(get_height(n->left), get_height(n->right)) + 1;
}


template <typename T>
void AVLTree<T>::left_rotate(std::unique_ptr<TreeNode<T>>& n) {
    // TODO
    TreeNode<T>* right_node = n->right.get();

    // 1. Move the middle subtree to the right subtree of the root
    n->right = std::move(right_node->left);

    // 2. Set the right node as root node
    right_node->left = std::move(n);
    std::unique_ptr<TreeNode<T>> new_root(right_node);
    n = std::move(new_root);
}

template <typename T>
void AVLTree<T>::right_rotate(std::unique_ptr<TreeNode<T>>& n) {
    // TODO
    TreeNode<T>* left_node = n->left.get();

    // 1. Move the middle subtree to the left subtree of the root
    n->left = std::move(left_node->right);

    // 2. Set the left node as root node
    left_node->right = std::move(n);
    std::unique_ptr<TreeNode<T>> new_root(left_node);
    n = std::move(new_root);
}

template <typename T>
void AVLTree<T>::left_right_rotate(std::unique_ptr<TreeNode<T>>& n) {
    // TODO
    TreeNode<T>* left_right_node = n->left->right.get();
    TreeNode<T>* left_node       = n->left.get();
    TreeNode<T>* right_node      = n.get();

    // 1. Move the middle subtrees to the left and right node respectively.
    left_node->right = std::move(left_right_node->left);
    right_node->left = std::move(left_right_node->right);

    // 2. Set the left-right node as the new root node
    left_right_node->left  = std::move(n->left);
    left_right_node->right = std::move(n);
    std::unique_ptr<TreeNode<T>> new_root(left_right_node);
    n = std::move(new_root);
}

template <typename T>
void AVLTree<T>::right_left_rotate(std::unique_ptr<TreeNode<T>>& n) {
    // TODO
    TreeNode<T>* right_left_node = n->right->left.get();
    TreeNode<T>* right_node      = n->right.get();
    TreeNode<T>* left_node       = n.get();

    // 1. Move the middle subtrees to the left and right node respectively.
    left_node->right = std::move(right_left_node->left);
    right_node->left = std::move(right_left_node->right);

    // 2. Set the left-right node as the new root node
    right_left_node->left  = std::move(n);
    right_left_node->right = std::move(n->right);
    std::unique_ptr<TreeNode<T>> new_root(right_left_node);
    n = std::move(new_root);
}

template <typename T>
T AVLTree<T>::find_rightmost_key(std::unique_ptr<TreeNode<T>>& n) const { 
    // You do not have to use/write this function if you think it is unnecessary.
}
