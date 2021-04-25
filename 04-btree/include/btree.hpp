#include <cstddef>
#include <array>
#include <iostream>
#include <optional>
#include <iterator>
#include <algorithm>
#include <cstring>
#include <string>
#include <sstream>
#include <functional>

enum class NodeType { LEAF, INTERNAL };

template<typename T, size_t B = 6>
struct BTreeNode;

template<typename T, size_t B = 6>
struct BTree {
    BTreeNode<T, B>* root = nullptr;

    ~BTree() { if (root) delete root; }

    bool insert(const T&);
    bool remove(const T&);

    void for_all(std::function<void(T&)>);
    void for_all_nodes(std::function<void(const BTreeNode<T,B>&)>);

    const std::optional<T> find_rightmost_key() const;
    const std::optional<size_t> depth() const;

    std::string format(void) const;
};

template<typename T, size_t B>
struct BTreeNode {
    NodeType type;
    size_t n;
    std::array<T, 2 * B - 1> keys;
    std::array<BTreeNode *, 2 * B> edges;

    BTreeNode();
    BTreeNode(const T& t);
    BTreeNode(std::initializer_list<T>);

    template<typename InputIt>
    BTreeNode(InputIt begin, InputIt end);

    ~BTreeNode();

    bool insert(const T& t);
    size_t get_index(const T& t);

    void for_all(std::function<void(T&)> func);

    bool remove(const T& t);

    size_t depth(void);
    std::string format_subtree(size_t) const;
    std::string format_level(size_t) const;
    std::string format_node(void) const;
    std::vector<BTreeNode<T, B>*> find_nodes_at_level(size_t) const;

    void for_all_nodes(std::function<void(const BTreeNode&)>);

    static std::pair<BTreeNode*, size_t> search(BTreeNode<T, B>*, const T& t);
    static void split_child(BTreeNode<T, B>&, size_t);
    static bool try_borrow_from_sibling(BTreeNode<T, B>&, size_t);
    static bool borrow_from_right(BTreeNode<T, B>&, size_t);
    static bool borrow_from_left(BTreeNode<T, B>&, size_t);

    // Custom func
    static bool borrow_from_successor(BTreeNode<T, B>&, size_t);
    static bool borrow_from_predecessor(BTreeNode<T, B>&, size_t);
    static void balance(BTreeNode<T, B>&, size_t);

    /* NOTE: If the root node has only one key, it will be empty after
      merging the children. Take care of updating the root. I guess this is
      the only way a B-tree may shrink its height. */
    static bool merge_children(BTreeNode<T, B>&, size_t);

    static T& find_rightmost_key(BTreeNode<T, B>&);
};

template<typename T,  size_t B>
bool BTree<T, B>::insert(const T& t) {
    if (!root) {
        root = new BTreeNode<T, B>(t);
        return true;
    }

    /* Make sure the root node is not full. Create an empty tree which has
       the original root as a child. Then split the original root. */
    if (root->n >= 2 * B - 1) {
        BTreeNode<T, B>* new_root = new BTreeNode<T, B>{};
        new_root->edges[0] = root;
        BTreeNode<T, B>::split_child(*new_root, 0);
        root = new_root;
    }

    return root->insert(t);
}

/* By default, use in-order traversal */
template<typename T, size_t B>
void BTree<T, B>::for_all(std::function<void(T&)> func) {
    if (root)
        root->for_all(func);
}

/* This isn't necessarily the in-order traversal */
template<typename T, size_t B>
void BTree<T, B>::for_all_nodes(std::function<void(const BTreeNode<T,B>&)> func) {
    if (root)
        root->for_all_nodes(func);
}

template<typename T, size_t B>
const std::optional<T> BTree<T, B>::find_rightmost_key() const {
    if (!root)
        return std::nullopt;

    return BTreeNode<T, B>::find_rightmost_key(*root);
}

template<typename T, size_t B>
const std::optional<size_t> BTree<T, B>::depth() const {
    if (!root)
        return std::nullopt;

    return root->depth();
}

template<typename T, size_t B>
bool BTreeNode<T, B>::insert(const T& t) {
    // TODO
    int idx = get_index(t);

    // Make sure the index is within the range
    if (idx > 2 * B || idx < 0) {
        return false;
    }

    // If internal node
    if (type == NodeType::INTERNAL) {
        // Check if child is full
        if (edges[idx]->n == 2 * B - 1) {
            split_child(*this, idx); 
            
            // Change value of idx
            int new_idx = get_index(t);
            return edges[new_idx]->insert(t);
        }
        else {
            return edges[idx]->insert(t);
        }

    }
    // If leaf node
    if (type == NodeType::LEAF) {
        // Shift every key after idx to the right
        for (int i = n; i >= (int)idx; i--) {
            keys[i+1] = keys[i];
        }

        // Insert
        keys[idx] = t;
        n++;

        return true;
    }

    return false;
}

/**
 * Find the desired position of t in current node.
 *
 * For example, if `n` looks like the following:
 *
 * [ 3 | 9 | 13 | 27 ]
 *
 * Then,
 *     n.get_index(2) = 0
 *     n.get_index(5) = 1
 *     n.get_index(10) = 2
 *     n.get_index(19) = 3
 *     n.get_index(31) = 4
 */
template<typename T, size_t B>
size_t BTreeNode<T, B>::get_index(const T& t) {
    // TODO
    // t is smaller than largest key
    // for (size_t i = 0; i < n; i++) {
    //     if (t <= keys[i]) return i;
    // }

    // // t is greater than largest key
    // return n;

    size_t count = 0;

    for (size_t i = 0; i < n; i++) {
        if (t == keys[i]) return count;
        if (t > keys[i]) count++;
    }

    return count;
}

// NOTE: `for_all` and `for_all_nodes` are used internally for testing.
// I'd not recommend using them in your functions...
template<typename T, size_t B>
void BTreeNode<T, B>::for_all(std::function<void(T&)> func) {
    if (type == NodeType::LEAF) {
        for (auto j = 0; j < n; j++)
            func(keys[j]);
    } else {
        if (n < 1)
            return;

        for (auto j = 0; j < n; j++) {
            edges[j]->for_all(func);
            func(keys[j]);
        }

        /* The rightest edge */
        edges[n]->for_all(func);
    }
}

/* This isn't necessarily the in-order traversal */
template<typename T, size_t B>
void BTreeNode<T, B>::for_all_nodes(std::function<void(const BTreeNode<T,B>&)> func) {
    if (type == NodeType::LEAF) {
        func(*this);
    } else {
        if (n < 1)
            return;

        func(*this);

        for (auto j = 0; j < n + 1; j++) {
            edges[j]->for_all_nodes(func);
        }
    }
}

/* Assume this is called only when the child parent->edges[idx] is full, and
   the parent is not full. */
template<typename T, size_t B>
void BTreeNode<T, B>::split_child(BTreeNode<T, B>& parent, size_t idx) {
    // TODO
    // n == 2B - 1
    BTreeNode<T, B>*& this_node = parent.edges[idx];

    // Create new empty node
    BTreeNode<T, B>* new_node = new BTreeNode<T, B>();
    new_node->type = this_node->type;

    // Find middle item
    T middle_item = this_node->keys[B - 1];

    // Shift every key of the parent after idx to the right by 1
    for (int i = parent.n - 1; i >= (int)idx; i--) {
        parent.keys[i+1] = parent.keys[i];
    }
    
    // Shift every edge of the parent after idx
    for (int i = parent.n; i >= (int)idx + 1; i--) {
        parent.edges[i+1] = parent.edges[i];
    }
    
    // Insert middle item to parent
    parent.keys[idx] = middle_item;

    // Insert new empty node to parent
    parent.edges[idx + 1] = new_node;
    
    parent.n++;
    parent.type = NodeType::INTERNAL;

    // Copy over keys from old leaf node to new node
    for (int i = B; i < this_node->n; i++)  new_node->keys[i - B] = this_node->keys[i];

    // Copy over edges from old leaf node to new node
    for (int i = B; i <= this_node->n; i++) new_node->edges[i - B] = this_node->edges[i];

    this_node->n = B - 1;
    new_node->n = B - 1;
}

template<typename T, size_t B>
bool BTree<T, B>::remove(const T& t) {
    if (!root)
        return false;

    root->remove(t);

    /* After merging, the size of the root may become 0. */
    if (root->n == 0 && root->type == NodeType::INTERNAL) {
        auto prev_root = root;
        root = root->edges[0];
        prev_root->type = NodeType::LEAF;
        delete prev_root;
    }

    return true;
}

template<typename T, size_t B>
bool BTreeNode<T, B>::remove(const T& t) {
    // TODO

    int idx = get_index(t);

    // Match found
    if (keys[idx] == t) {
        if (type == NodeType::LEAF) {
            for (int i = idx; i < n - 1; i++) {
                keys[i] = keys[i + 1];
            }
            n--;

            return true;
        }
        else {
            BTreeNode<T, B>* predecessor = edges[idx];
            BTreeNode<T, B>* successor   = edges[idx + 1];

            if (predecessor->n  > B - 1) return borrow_from_predecessor(*this, idx);
            if (successor->n > B - 1) return borrow_from_successor(*this, idx);
            if (predecessor->n == B - 1 && successor->n == B - 1) {
                bool success = merge_children(*this, idx);
                edges[idx]->remove(t);
                return success;
            }

        }

    }
    // Match not found -> traverse down
    else if (type == NodeType::INTERNAL) {
        bool success = edges[idx]->remove(t);

        // Balance
        if (edges[idx]->n < B - 1) {
            // Merge or Borrow
            if (idx > 0) {
                BTreeNode<T, B>* left_child  = edges[idx - 1];
                if (left_child->n > B - 1) return borrow_from_left(*this, idx);
            }
            if (idx < n) {
                BTreeNode<T, B>* right_child = edges[idx + 1];
                if (right_child->n > B - 1) return borrow_from_right(*this, idx);
            }
            if (edges[idx + 1]->n == B - 1) return merge_children(*this, idx);
        }

        return success;
    }
    else return false;
}

template<typename T, size_t B>
void BTreeNode<T, B>::balance(BTreeNode<T, B>&node, size_t e) {
    return;
}

/**
 * Try to borrow a key from sibling.
 *
 * @e: The index of the edge that are trying to borrow a key
 * @return true if borrowing succeed, false otherwise
 */
template<typename T, size_t B>
bool BTreeNode<T, B>::try_borrow_from_sibling(BTreeNode<T, B>&node, size_t e) {
    // TODO
    BTreeNode<T, B>* left_child  = node.edges[e];
    BTreeNode<T, B>* right_child = node.edges[e + 1];

    if      (left_child->n  > B - 1) return borrow_from_left(node, e);
    else if (right_child->n > B - 1) return borrow_from_right(node, e);

    return false;
}

template<typename T, size_t B>
bool BTreeNode<T, B>::borrow_from_successor(BTreeNode<T, B>& node, size_t edge) {
    // TODO
    BTreeNode<T, B>* right_child = node.edges[edge + 1];
    node.keys[edge] = right_child->keys[0];

    // Shift left by 1
    for (int i = 0; i < right_child->n - 1; i++) {
        right_child->keys[i] = right_child->keys[i + 1];
    }
    for (int i = 0; i < right_child->n; i++) {
        right_child->edges[i] = right_child->edges[i + 1];
    }

    right_child->n--;

    return true;
}

template<typename T, size_t B>
bool BTreeNode<T, B>::borrow_from_predecessor(BTreeNode<T, B>& node, size_t edge) {
    // TODO
    BTreeNode<T, B>* child = node.edges[edge];
    node.keys[edge] = child->keys[child->n - 1];

    child->n--;

    return true;
}

template<typename T, size_t B>
bool BTreeNode<T, B>::borrow_from_left(BTreeNode<T, B>& node, size_t edge) {
    // TODO
    BTreeNode<T, B>* child      = node.edges[edge];
    BTreeNode<T, B>* left_child = node.edges[edge - 1];

    // Shift child node keys by 1 to the right
    for (int i = child->n - 1; i >= 0; i--) {
        child->keys[i + 1] = child->keys[i];
    }

    child->keys[0] = node.keys[edge - 1];
    child->n++;

    node.keys[edge - 1] = left_child->keys[left_child->n - 1];
    left_child->n--;

    return true;
}

template<typename T, size_t B>
bool BTreeNode<T, B>::borrow_from_right(BTreeNode<T, B>& node, size_t edge) {
    // TODO
    BTreeNode<T, B>* child       = node.edges[edge];
    BTreeNode<T, B>* right_child = node.edges[edge + 1];

    child->keys[child->n] = node.keys[edge];
    child->n++;

    node.keys[edge] = right_child->keys[0];
    
    // Shift keys of right child by 1 to the left
    for (int i = 0; i < right_child->n - 1; i++) {
        right_child->keys[i] = right_child->keys[i + 1];
    }

    right_child->n--;

    return true;
}

template<typename T, size_t B>
bool BTreeNode<T, B>::merge_children(BTreeNode<T, B> & node, size_t idx) {
    // TODO
    BTreeNode<T, B>* new_node = new BTreeNode<T, B>();
    BTreeNode<T, B>* left_child  = node.edges[idx];
    BTreeNode<T, B>* right_child = node.edges[idx + 1];

    // Copy keys in left child to new node
    for (int i = 0; i < left_child->n; i++) {
        new_node->keys[i] = left_child->keys[i];
        new_node->n++;
    }

    // Copy edges in left child to new node
    for (int i = 0; i < left_child->n + 1; i++) {
        new_node->edges[i] = left_child->edges[i];
    }

    new_node->keys[new_node->n] = node.keys[idx];
    new_node->n++;

    int top = new_node->n;
    
    // Copy keys from right child to new node
    for (int i = 0; i < right_child->n; i++) {
        new_node->keys[top + i] = right_child->keys[i];
        new_node->n++;
    }

    // Copy edges from right child to new node
    for (int i = 0; i < right_child->n + 1; i++) {
        new_node->edges[top + i] = right_child->edges[i];
    }

    // Handle parent
    // Shift left by 1
    for (int i = idx; i < node.n - 1; i++) {
        node.keys[i] = node.keys[i + 1];
    }

    for (int i = idx; i < node.n; i++) {
        node.edges[i] = node.edges[i + 1];
    }

    node.n--;

    node.edges[idx] = new_node;
    
    left_child->edges.fill(nullptr);
    right_child->edges.fill(nullptr);

    delete left_child;
    delete right_child;

    return true;
}

template<typename T, size_t B>
T& BTreeNode<T, B>::find_rightmost_key(BTreeNode<T, B>& node) {
    if (node.type == NodeType::LEAF)
        return node.keys[node.n - 1];

    return find_rightmost_key(*node.edges[node.n]);
}

// NOTE: `search` function is originally intended to be used by testing code.
// Don't modify this function. You can reuse this function 'as-is', or, if
// you want to do something different, add another function based on this function.
template<typename T, size_t B>
std::pair<BTreeNode<T, B>*, size_t>
BTreeNode<T, B>::search(BTreeNode<T, B>* node, const T& t) {
    if (node->type == NodeType::LEAF) {
        for (auto i = 0; i < node->keys.size(); i++)
            if (t == node->keys[i])
                return { node, i };

        return { nullptr, -1 };
    }

    size_t i;
    for (i = 0; i < node->n; i++) {
        if (t == node->keys[i])
            return { node, i };

        if (t < node->keys[i]) {
            return search(node->edges[i], t);
        }
    }

    return search(node->edges[i], t);
}

template<typename T, size_t B>
size_t BTreeNode<T, B>::depth(void) {
    if (type == NodeType::LEAF)
        return 0;

    return 1 + edges[0]->depth();
}

template<typename T, size_t B>
std::ostream& operator<<(std::ostream& os, const BTree<T, B>& btree) {
    os << btree.format();
    return os;
}

template <typename T, size_t B>
std::string BTree<T, B>::format(void) const {
    if (!root)
        return std::string{};

    return root->format_subtree(root->depth());
}

template<typename T, size_t B>
std::string BTreeNode<T, B>::format_subtree(size_t depth) const {
    std::ostringstream os;

    for (auto i = 0; i <= depth; i++)
        os << format_level(i) << '\n';

    return os.str();
}

template<typename T, size_t B>
std::string BTreeNode<T, B>::format_level(size_t level) const {
    std::ostringstream os;
    auto nodes_at_level = find_nodes_at_level(level);

    for (auto node : nodes_at_level)
        os << node->format_node() << ' ';

    return os.str();
}


template<typename T, size_t B>
std::string BTreeNode<T, B>::format_node(void) const {
    std::ostringstream os;

    if (n < 1) {
        os << "[]";
        return os.str();
    }

    os << '[';
    for (auto i = 0; i < n - 1; i++)
        os << keys[i] << '|';
    os << keys[n - 1];
    os << ']';

    return os.str();
}

template<typename T, size_t B>
std::vector<BTreeNode<T, B>*> BTreeNode<T, B>::find_nodes_at_level(size_t lv) const {
    std::vector<BTreeNode<T, B>*> nodes;

    if (lv == 0) {
        nodes.emplace_back(const_cast<BTreeNode<T, B>*>(this));
        return nodes;
    } else {
        std::vector<BTreeNode<T, B>*> tmp;
        for (auto i = 0; i < n + 1; i++) {
            tmp = edges[i]->find_nodes_at_level(lv - 1);
            std::copy(tmp.begin(), tmp.end(), std::back_inserter(nodes));
        }

        return nodes;
    }
}

template<typename T, size_t B>
BTreeNode<T, B>::BTreeNode() : n(0), type(NodeType::LEAF) {}

template<typename T, size_t B>
BTreeNode<T, B>::BTreeNode(const T& t) : n(1), type(NodeType::LEAF) {
    keys[0] = t;
}

/* Assume the input initializer list is sorted */
template<typename T, size_t B>
BTreeNode<T, B>::BTreeNode(std::initializer_list<T> l)
    : n(l.size()), type(NodeType::LEAF) {
    std::copy(l.begin(), l.end(), keys.begin());
}

/* Assume the input iterator is sorted. */
template<typename T, size_t B>
template<typename InputIt>
BTreeNode<T, B>::BTreeNode(InputIt begin, InputIt end)
    : n(end - begin), type(NodeType::LEAF) {
    std::copy(begin, end, keys.begin());
}

template<typename T, size_t B>
BTreeNode<T, B>::~BTreeNode() {
    if (this->type == NodeType::LEAF)
        return;

    for (auto i = 0; i < n + 1; i++)
        if (edges[i]) delete edges[i];
}
