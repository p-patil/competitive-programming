#include <algorithm>
#include <cassert>
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
 * 95 -> 99
 * 2532 -> 2552
 * 2582 -> 2662
 * 29522892 -> 29533592
 * 1295228923 -> 1295335921
 * 3995 -> 4004
 * 3999995 -> 4000004
 * 9999 -> 10001
 * 195 -> 202
 * 995 -> 999
 * 973 -> 979
 * 978 -> 979
 * 979 -> 989
 * 9779 -> 9889
 */

/*
 * 29622892
 * 2 962289 2
 * Recurse on 962289:
 *     9 6228 9
 *     Recurse on 6228:
 *         
 */


// Convenience functions

inline int CharToDigit(const char& c) {
    return int(c - '0');
}


// DP solution

void NextBiggestPalindrome(std::vector<char>& K, bool& all_nines) {
    if (K.size() == 1) { // Base case: single-digit
        int digit = CharToDigit(K.front());
        if (digit == 9) {
            all_nines = true;
        }
        else {
            ++K.front();
            all_nines = false;
        }

        return;
    } else if (K.size() == 2) { // Base case: double-digit
        int left = CharToDigit(K.front()), right = CharToDigit(K[1]);
        if (left == 9 && right == 9) {
            all_nines = true;
        } else if (right < left) {
            K.back() = K.front();
            all_nines = false;
        } else {
            assert(left != 9);
            ++K.front();
            K.back() = K.front();
            all_nines = false;
        }
 
        return;
    }

    int left = CharToDigit(K.front()), right = CharToDigit(K.back());
    if (right < left) {
        all_nines = false;
        K.back() = K.front();
        return;
    }

    std::vector<char> sub_K = std::vector<char>(K.begin() + 1, K.end() - 1);
    bool sub_all_nines;
    NextBiggestPalindrome(sub_K, sub_all_nines);
    std::copy(sub_K.begin(), sub_K.end(), K.begin() + 1); // Copy sub_K back into K

    if (sub_all_nines) {
        int left = CharToDigit(K.front()), right = CharToDigit(K.back());
        if (left == 9 && right == 9) {
            // Return 10...01 where number of 0's = K.size().
            std::fill(K.begin(), K.end(), '0');
            K.front() = '1';
            K.push_back('1');

            all_nines = true;
            return;
        }

        // Return F0...0F where F = first digit of K, number of 0's = K.size() - 2.
        std::fill(K.begin() + 1, K.end() - 1, '0');
        K.back() = K.front();
        all_nines = false;
        return;
    } else {
        K.back() = K.front();
        all_nines = false;
    }
}

void NextBiggestPalindrome(std::vector<char>& K) {
    bool all_nines;
    NextBiggestPalindrome(K, all_nines);
}

int main(int argc, char **argv) {
    std::string t;
    std::getline(std::cin, t);

    for (std::string K; std::getline(std::cin, K);) {
        std::vector<char> vec_K (K.begin(), K.end());
        NextBiggestPalindrome(vec_K);
        std::cout << std::string(vec_K.begin(), vec_K.end()) << std::endl;
    }

    return 0;
}
