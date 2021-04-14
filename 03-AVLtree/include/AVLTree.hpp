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
    
    if (key < val) {
        // Traverse down and insert
        bool insert_success = insert(n->left, key);
       
        // Balance
        balance(n);

        return insert_success;
    }
    else if (key > val) {
        // Traverse down and insert
        bool insert_success = insert(n->right, key);
        
        // Balance
        balance(n);

        return insert_success;
    }
    else return false; // Already exists
}

template <typename T>
bool AVLTree<T>::remove(std::unique_ptr<TreeNode<T>>& n, const T& key) {
    // TODO
}


template <typename T>
int AVLTree<T>::get_height(std::unique_ptr<TreeNode<T>>& n) const {
    if (n == nullptr) return -1;
    return n->height;
}

template <typename T>
int AVLTree<T>::get_balance_factor(std::unique_ptr<TreeNode<T>>& n) const {
    // TODO
    if (n == nullptr) return 0;

    int l_height = get_height(n->left);
    int r_height = get_height(n->right);

    return l_height - r_height;
}


template <typename T>
void AVLTree<T>::balance(std::unique_ptr<TreeNode<T>>& n) {
    // You do not have to use/write balance function if you think it is unnecessary.
    int balance_factor = get_balance_factor(n);

    if (balance_factor > 1) { // Left is deeper than right by a factor of 2; happens when inserting on the left
        int left_left_h  = get_height(n->left->left);
        int left_right_h = get_height(n->left->right);

        if (left_left_h > left_right_h) right_rotate(n);
        if (left_left_h < left_right_h) right_left_rotate(n);
    }
    if (balance_factor < -1) { // Left is deeper than right by a factor of 2; happens when inserting on the left
        int right_right_h = get_height(n->right->right);
        int right_left_h  = get_height(n->right->left);

        if (right_right_h > right_left_h) left_rotate(n);
        if (right_right_h < right_left_h) left_right_rotate(n);
    }

    // Set new height
    // Ref: Data Structures and Algorithm Analysis in C++, pg. 173
    n->height = std::max(get_height(n->left), get_height(n->right)) + 1;
}


template <typename T>
void AVLTree<T>::left_rotate(std::unique_ptr<TreeNode<T>>& n) {
    // TODO
    std::unique_ptr<TreeNode<T>> m = std::move(n->right);
    n->right= std::move(m->left);
    m->left= std::move(n); // n turns into nullptr here

    TreeNode<T>* new_left = m->left.get();

    new_left->height = std::max(get_height(new_left->left), get_height(new_left->right)) + 1;
    m->height = std::max(get_height(m->left), get_height(m->right)) + 1;

    n = std::move(m);
}

template <typename T>
void AVLTree<T>::right_rotate(std::unique_ptr<TreeNode<T>>& n) {
    // TODO
    std::unique_ptr<TreeNode<T>> m = std::move(n->left);
    n->left = std::move(m->right); // OK
    m->right = std::move(n); // n is nullptr here

    TreeNode<T>* new_right = m->right.get();

    new_right->height = std::max(get_height(new_right->left), get_height(new_right->right)) + 1;
    m->height = std::max(get_height(m->left), get_height(m->right)) + 1;

    n = std::move(m);
}

template <typename T>
void AVLTree<T>::left_right_rotate(std::unique_ptr<TreeNode<T>>& n) {
    // TODO
    right_rotate(n->right);
    left_rotate(n);
}

template <typename T>
void AVLTree<T>::right_left_rotate(std::unique_ptr<TreeNode<T>>& n) {
    // TODO
    left_rotate(n->left);
    right_rotate(n);
}

template <typename T>
T AVLTree<T>::find_rightmost_key(std::unique_ptr<TreeNode<T>>& n) const { 
    // You do not have to use/write this function if you think it is unnecessary.
}
