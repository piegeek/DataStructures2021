#include "fheap.hpp"

int main() {
   FibonacciHeap<int> heap = {1, 2, 3, 4, 5};
   std::cout << heap.extract_min().value() << std::endl;
}
