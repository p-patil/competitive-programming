#include <algorithm>
#include <cassert>
#include <iostream>
#include <map>
#include <set>
#include <random>
#include <string>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <utility>

#include "BigInteger/BigInteger.h"

constexpr size_t kMaxDigits = 1000000;
constexpr size_t kDefaultMaxLength = 500;
constexpr size_t kDefaultN = 1000;

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
        } else {
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

    std::vector<char> sub_K = std::vector<char>(K.begin() + 1, K.end() - 1);
    bool sub_all_nines;
    NextBiggestPalindrome(sub_K, sub_all_nines);
    assert(sub_K.size() == K.size() - 2);

    if (sub_all_nines) {
        int left = CharToDigit(K.front()), right = CharToDigit(K.back());
        if (left == 9 && right == 9) {
            all_nines = true;
        } else if (right < left) {
            K.back() = K.front();
            all_nines = false;
        } else {
            // Return F0...0F where F = digit after the first digit of K, number of 0's =
            // K.size() - 2.
            std::fill(K.begin() + 1, K.end() - 1, '0');
            ++K.front();
            K.back() = K.front();
            all_nines = false;
        }
    } else {
        std::copy(sub_K.begin(), sub_K.end(), K.begin() + 1); // Copy sub_K back into K
        K.back() = K.front();
        all_nines = false;
    }
}

void NextBiggestPalindrome(std::vector<char>& K) {
    bool all_nines;
    NextBiggestPalindrome(K, all_nines);

    if (all_nines) {
        // Return 10...01 where number of 0's = K.size().
        std::fill(K.begin(), K.end(), '0');
        K.front() = '1';
        K.push_back('1');
    }
}


// Randomized testing

inline bool IsPalindrome(const std::string& number) {
    return std::equal(number.begin(), number.begin() + number.size() / 2, number.rbegin());
}

std::string BruteForceNextPalindrome(const std::string& number) {
    BigInteger big_num{number};
    ++big_num; // Next palindrome must be bigger

    while (!IsPalindrome(big_num.getNumber())) ++big_num;

    return big_num.getNumber();
}

void RandomlyTest(const size_t n, const size_t max_length) {
    std::random_device dev;
    std::mt19937 rng{dev()};
    std::uniform_int_distribution<int> digit_dist{0, 9};
    std::uniform_int_distribution<int> len_dist{1, max_length};

    for (size_t i = 0; i < n; ++i) {
        // Generate a random large number (as a digit string).
        const size_t len = len_dist(rng);
        std::string number;
        number.reserve(len);
        for (size_t j = 0; j < len; ++j)
            number += '0' + digit_dist(rng);

        std::vector<char> vec_number(number.begin(), number.end());
        NextBiggestPalindrome(vec_number);

        const std::string guess{vec_number.begin(), vec_number.end()};
        const std::string& palindrome = BruteForceNextPalindrome(number);

        if (guess != palindrome)
            std::cout << "FAILED: " << number << " (expected: " << palindrome << ", got: "
                      << guess << ")" << std::endl;
    }
}


// Main

int main(int argc, char **argv) {
    if (argc > 1) {
        std::string first_arg{argv[1]};
        if (first_arg == "--test") {
            size_t n = kDefaultN;
            size_t max_length = kDefaultMaxLength;
            if (argc > 2) n = std::stoi(argv[2]);
            if (argc > 3) max_length = std::stoi(argv[3]);

            std::cout << "Randomly testing " << n << " iterations, maximum number length is "
                << max_length << std::endl;
            RandomlyTest(n, max_length);
        } else {
            std::vector<char> vec (first_arg.begin(), first_arg.end());
            NextBiggestPalindrome(vec);
            std::cout << std::string(vec.begin(), vec.end()) << std::endl;
        }

        return 0;
    }

    std::string t;
    std::getline(std::cin, t);

    for (std::string K; std::getline(std::cin, K);) {
        std::vector<char> vec_K (K.begin(), K.end());
        NextBiggestPalindrome(vec_K);
        std::cout << std::string(vec_K.begin(), vec_K.end()) << std::endl;
    }

    return 0;
}
