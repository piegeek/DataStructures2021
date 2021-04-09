# HW#3: AVL Tree

* **Due date: 11:59pm, 4/21/2021**

## Goals

* Get familiar with C++ programming by implementing the balanced binary tree.

* Get to know what is `std::unique_ptr` and how to use it.

### Table of contents
1. [AVL Tree](#AVLTree)
2. [Types](#Types)
3. [Insertion](#Insertion)
4. [Deletion](#Deletion)
5. [Submission and Grading](#Submission-and-Grading)

## AVLTree

### Definitions
According to http://web.stanford.edu/class/archive/cs/cs106x/cs106x.1174/lectures/20-BinarySearchTrees/20-BinarySearchTrees.pdf,

> An AVL tree (Adelson-Velskii and Landis) is a compromise. It is the
>
> same as a binary search tree, except that for every node, the height
>
> of the left and right subtrees can differ only by 1 (and an empty tree
>
> has a height of -1).

## Types

We will take a look at the types:

```c++
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
```


An `AVLTree` has a pointer to a root, and a `TreeNode` has a element, 
height and two pointers for subtrees (i.e. children).

The `AVLTree` has following interfaces which is defined in `AVLTree.hpp`:

* `bool insert(const T& key)` inserts `key` into the tree, balances the tree, and returns true if sucessfully inserted. 
    Otherwise, returns false.

* `bool remove(const T& key)` removes `key` from the tree, balances the tree, and returns false if sucessfully removed.
    Otherwise, returns false.

* `int get_height(std::unique_ptr<TreeNode<T>> &n)` gets the height of node `n`. 
    The height of the empty tree is -1.

* `int get_balance_factor(std::unique_ptr<TreeNode<T>> &n)` gets the balance factor of node `n` 
(i.e., height_of_left_subtree - height_of_right_subtree).
If the node `n` is nullptr, it returns 0.

* `void balance(std::unique_ptr<TreeNode<T>>& n)` balances the subtree (where the node `n` is root) 
    if it is unbalanced tree. You do not have to use `balance` function if you think it is unnecessary. 

* Each rotation function (e.g., `void left_rotate(std::unique_ptr<TreeNode<T>>& n)`) 
    is used to balance the unbalanced subtree (where the node `n` is root).
  You can check the usage of rotation functions at [Insertion Example](#Insertion-Example) and
  [Deletion Example](#Deletion-Example).

## Insertion

The strategy for insertion is simple. 
Same as BST, we start at the root and recursively go down the tree searching for a location to insert a new node.
After that, we have to balance the tree to maintain the property of AVL Tree.

### Insertion Example
Given an AVL Tree like this and try to insert a new node with the element "8" into the tree:
```
    5                  5 (node n)                      8
     \    insert(8)     \      right_left_rotate(n)   / \
      9      ==>         9             ==>           5   9
                        /           
                       8            
```
If new node is sucessfully inserted and the tree is balanced, 
the function `insert(const T& key)` returns true, otherwise return false.

## Deletion

The deletion strategy is same as BST (see BST README.md), 
but after deleting the node, 
we have to balance the tree to maintain the property of AVL Tree.


### Deletion Example
Given an AVL Tree, we try to delete a node with the element "3" into the tree:

```
    5                  5 (node n)                     9
   / \    remove(3)     \          left_rotate(n)    / \
  3   9      ==>         9              ==>         5  11
       \                  \            
       11                 11             
```
If new node is sucessfully deleted and the tree is balanced, 
the function `remove(const T& key)` returns true, otherwise return false.


## Submission and Grading

### Testing your own programs

In `03-AVLTree` directory, you run the following commands to compile your own implementation.

```
$ pwd
/xxx/xxx/xxx/03-AVLTree
$ mkdir build
$ cd build
$ cmake ../
$ make
$ ls ./tests/AVLTree_test
./tests/AVLTree_test
$ ./tests/AVLTree_test
[...]
```

If you've all done correctly, the output will look like:

```
===============================================================================
All tests passed (XX assertions in XX test cases)

```

Finally, to check the memory leaks, please run your program with valgrind.
```shell
$ valgrind ./tests/AVLTree_test
```

If you've all done correctly, the output should be like this:

```
==12565== All heap blocks were freed -- no leaks are possible
==12565== 
==12565== For counts of detected and suppressed errors, rerun with: -v
==12565== ERROR SUMMARY: 0 errors from 0 contexts (suppressed: 0 from 0)
```

and you are ready to submit!

## References
1. std::unique_ptr--[Link](https://en.cppreference.com/w/cpp/memory/unique_ptr)
2. AVL Tree--[Link](https://courses.cs.washington.edu/courses/cse373/06sp/handouts/lecture12.pdf)
