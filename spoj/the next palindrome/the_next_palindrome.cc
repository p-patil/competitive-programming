#include <algorithm>
#include <iostream>
#include <map>
#include <set>
#include <string>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <utility>

constexpr size_t max_digits = 1000000;

/*
 * Examples:
 * 52 -> 55
 * 25 -> 33
 * 33 -> 44
 * 95 -> 
 * 2532 -> 2552
 * 2582 -> 2662
 * 29522892 -> 29533592
 * 1295228923 -> 1295335921
 * 3995 -> 4004
 */

inline int CharToDigit(const char& c) {
    return int(c - '0');
}


void NextBiggestPalindrome(std::vector<char>& K, size_t start, size_t end) {
    if (start == end) { // Single-digit case
        K[start] =  
    } else if (end - start == 1) { // Double-digit case
        int left = CharToDigit(K[start]), right = CharToDigit(K[end]);
        if (right < left) {
            K[end] = K[start];
            return;
        } else if (left == right && left == 9) { // Handle the special case of 99 separately
            //TODO(piyush)
        } else {
            K[start]++;
            K[end] = K[start];
        }
    }

    //TODO(piyush)
}

int main(int argc, char **argv) {
    int t;
    std::cin >> t;

    for (std::string K; std::getline(std::cin, K);) {
        std::vector<char> bigger_palindrome;
        bigger_palindrome.reserve(K.size());

        size_t start;
        if (K.size() % 2 == 0) start = K.size() / 2;
        else start = K.size() % 2 + 1;

        std::copy(K.begin(), K.begin() + (start - 1), bigger_palindrome.begin());

        // Find leftmost digit in the bottom half of K that differs from its
        // mirror counterpart.
        int index = -1;
        for (size_t i = start; i >= 0; --i) {
            if (K.at(i) != K.at(K.size() - 1 - i)) {
                index = -1;
                break;
            }
        }

        if (index != -1) {
            int digit = CharToDigit(K.at(index)),
                mirror = CharToDigit(K.at(K.size() - 1 - index));

            if (digit < mirror) {
                for (size_t i = 0; i < start; ++i)
                    bigger_palindrome[bigger_palindrome.size() - 1 - i] = bigger_palindrome[i];
                std::cout << std::string(bigger_palindrome.begin(), bigger_palindrome.end());
                continue;
            }


        } else { // K is already a palindrome

        }
    }

    return 0;
}
