#include <algorithm>
#include <iostream>
#include <vector>
#include <functional>
#include <iterator>
#include <memory>


template <typename T>
class TreeNode
{
    public:
        T element;
        std::unique_ptr<TreeNode<T>> left;
        std::unique_ptr<TreeNode<T>> right;

        TreeNode<T>(const T& e)
            :element{e}, left{nullptr}, right{nullptr} {}

        ~TreeNode() {}

};


template <typename T>
struct BST
{
    public:
        std::unique_ptr<TreeNode<T>> root = nullptr;

        ~BST() {}

        bool insert(const T& key);
        bool search(const T& key);
        bool remove(const T& key);

    private:
        bool insert(std::unique_ptr<TreeNode<T>>& t, const T& key);
        bool search(std::unique_ptr<TreeNode<T>>& t, const T& key);
        bool remove(std::unique_ptr<TreeNode<T>>& t, const T& key);

        // you can define find_right_most_key function

};

template <typename T>
bool BST<T>::insert(const T& key) {
    return insert(root, key);
}

template <typename T>
bool BST<T>::search(const T& key) {
    return search(root, key);
}

template <typename T>
bool BST<T>::remove(const T& key) {
    return remove(root, key);
}

template <typename T>
bool BST<T>::insert(std::unique_ptr<TreeNode<T>>& t, const T& key) {

    // TODO
    // if insertion fails (i.e. if the key already exists in tree), return false
    // otherwise, return true

    // If t is the root pointer <=> t == nullptr
    if (t == nullptr) {
        // Create root node
        TreeNode<T>* new_node = new TreeNode<T>(key);
        // Point to root node
        t.reset(new_node);

        return true;
        }

    // Extract node value
    T val = t->element;

    // Go left and insert
    if (key < val) {
        // Is leaf node? => Insert to the left
        if (t->left == nullptr) {
            // Create new node
            TreeNode<T>* new_node = new TreeNode<T>(key);
            // Point to new node
            t->left.reset(new_node);
            
            return true;
        }
        // Traverse down to the left and try insertion
        else {
            return insert(t->left, key);
        }
    }
    // Go right and insert
    else if (key > val) {
        // Is leaf node? => Insert to the right
        if (t->right == nullptr) {
            // Create new node
            TreeNode<T>* new_node = new TreeNode<T>(key);
            // Point to new node
            t->right.reset(new_node);
            
            return true;
        }
        // Traverse down to the right and try insertion
        else {
            return insert(t->right, key);
        }
    }
    // Already exists
    else {
        return false;
    }
}

template <typename T>
bool BST<T>::search(std::unique_ptr<TreeNode<T>>& t, const T& key) {

    // TODO
    // if key exists in tree, return true
    // otherwise, return false

    // Empty tree 
    if (t == nullptr) return false;

    // Extract node value
    T val = t->element;

    // Traverse down and search
    if      (key < val) return search(t->left,  key);
    else if (key > val) return search(t->right, key);
    else                return true; // match found
}

template <typename T>
bool BST<T>::remove(std::unique_ptr<TreeNode<T>>& t, const T& key) {

    // TODO
    // if key does not exist in tree, return false
    // otherwise, return true

    // Empty tree
    if (t == nullptr) return false;

    // Extract value
    T val = t->element;

    // Traverse down and search
    if      (key < val) return remove(t->left,  key);
    else if (key > val) return remove(t->right, key);
    
    // Match found
    else {
        // Is leaf node
        if (t->left == nullptr && t->right == nullptr) {
            t.reset(nullptr);
        }
        // Has only left child
        else if (t->left != nullptr && t->right == nullptr) {
            TreeNode<T>* np = t.release();
            t.reset(np->left.get());

            // delete np;
        }
        // Has only left child
        else if (t->left == nullptr && t->right != nullptr) {
            TreeNode<T>* np = t.release();
            t.reset(np->right.get());

            // delete np;
        }
        // Has both children
        else {
            // Find max element from left subtree
            T max_val;
            TreeNode<T>* np = t->left.get(); 

            // np already points to the max node
            if (np->right == nullptr) {
                max_val = np->element;
            }
            // Has to traverse down the tree to find maximum node
            else {
                // point np to the last node before max node
                while (np->right->right != nullptr) {
                    np = np->right.get();
                }

                max_val = np->right->element; // Max element found
                np->right.reset(); // Delete node with maximum value
            }
           
            // Propagate max value up
            t->element = max_val;
        }

        return true;
    }
}
