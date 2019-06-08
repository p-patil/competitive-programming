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

void NextBiggestPalindrome(std::vector<char>& K, size_t left, size_t right, bool& all_nines) {
    if (left == right) { // Base case: single-digit
        int digit = CharToDigit(;
        if (digit == 9) {
            all_nines = true;
        }
        else {
            ++left;
            all_nines = false;
        }

        return;
    } else if (left + 1 == right) { // Base case: double-digit
        int l_digit = CharToDigit(*left), r_digit = CharToDigit(*right);
        if (l_digit == 9 && r_digit == 9) {
            all_nines = true;
        } else if (r_digit < l_digit) {
            *right = *left;
            all_nines = false;
        } else {
            assert(l_digit!= 9);
            ++left;
            *right = *left;
            all_nines = false;
        }
 
        return;
    }

    int l_digit = CharToDigit(*left), r_digit = CharToDigit(*right);
    if (r_digit < l_digit) {
        all_nines = false;
        *right = *left;
        return;
    }

    bool sub_all_nines;
    NextBiggestPalindrome(left + 1, right - 1, sub_all_nines);

    if (sub_all_nines) {
        // K was modified increased in size by 1, so set right := right + 1.
        int l_digit = CharToDigit(left), r_digit = CharToDigit(right + 1);
        if (l_digit == 9 && r_digit == 9) {
            // Return 10...01 where number of 0's = K.size().
            std::fill(K.begin(), K.end(), '0');
            K[left] = '1';
            K.push_back('1');

            all_nines = true;
            return;
        }

        // Return F0...0F where F = first digit of K, number of 0's = K.size() - 2.
        std::fill(K.begin() + left + 1, K.begin() + right - 1, '0');
        K[right] = K[left];
        all_nines = false;
        return;
    } else {
        K[right] = K[left];
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
