#include "fheap.hpp"

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
void print_root_list(FibonacciHeap<T>& h) {
   std::cout << "[";
   for (auto root : get_root_list(h)) {
      std::cout << root->key << ", ";
   }
   std::cout << "]" << std::endl;
}

int main() {
   FibonacciHeap<int> heap;

   for(int i = 9 ; i > 0 ; --i) {
      heap.insert(i);
   }

   heap.insert(9);

   print_root_list(heap);

   
   for(int i = 9 ; i > 0 ; --i) {
      heap.extract_min();
      print_root_list(heap);
   }

}
