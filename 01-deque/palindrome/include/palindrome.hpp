#include <string>

#include "deque.hpp"

template<typename Deque>
class Palindrome {
public:
    bool is_palindrome(const std::string&);
    void reset_deque();

private:
    Deque deque;
};

template<typename Deque>
bool Palindrome<Deque>::is_palindrome(const std::string& s1) {
    // TODO
    // Store every character in the string in the deque
    for (auto &c : s1) {
        deque.push_back(c);
    }

    // Pop each character stored in deque and match it with the string
    size_t size_of_deque = deque.size();

    for (size_t i = 0; i < size_of_deque; i++) {
        auto c = deque.remove_back();
        if (c != s1[i]) return false;
    }

    return true;
}

template<typename Deque>
void Palindrome<Deque>::reset_deque() {
    while (!deque.empty())
        deque.remove_front();
}
