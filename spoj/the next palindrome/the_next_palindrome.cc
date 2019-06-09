#include <algorithm>
#include <cassert>
#include <chrono>
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
    } else if (K.size() == 3) { // Base case: For odd number of digits (3 digits)
        int left = CharToDigit(K.front()), middle = CharToDigit(K[1]),
            right = CharToDigit(K.back());

        if (right < left) {
            K.back() = K.front();
            all_nines = false;
            return;
        }

        // We need to increment the middle value.
        if (middle != 9) {
            K.back() = K.front();
            ++K[1];
            all_nines = false;
        } else {
            if (left == 9 && right == 9) {
                all_nines = true;
            } else {
                ++K[0]; // left is guaranteed not to be 9
                K[1] = '0';
                K.back() = K.front();
            }
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

// Optimized version (no copies).
void NextBiggestPalindrome(std::string& K, const size_t left, const size_t right,
                           bool& all_nines) {
    if (left == right) { // Base case: single-digit
        int digit = CharToDigit(K[left]);
        if (digit == 9) {
            all_nines = true;
        } else {
            ++K[left];
            all_nines = false;
        }

        return;
    } else if (left + 1 == right) { // Base case: double-digit
        int l_digit = CharToDigit(K[left]), r_digit = CharToDigit(K[right]);
        if (l_digit == 9 && r_digit == 9) {
            all_nines = true;
        } else if (r_digit < l_digit) {
            K[right] = K[left];
            all_nines = false;
        } else {
            assert(l_digit != 9);
            ++K[left];
            K[right] = K[left];
            all_nines = false;
        }

        return;
    } else if (left + 2 == right) { // Base case: For odd number of digits (3 digits)
        int l_digit = CharToDigit(K[left]), middle = CharToDigit(K[left + 1]),
            r_digit = CharToDigit(K[right]);

        if (r_digit < l_digit) {
            K[right] = K[left];
            all_nines = false;
            return;
        }

        // We need to increment the middle value.
        if (middle != 9) {
            K[right] = K[left];
            ++K[left + 1];
            all_nines = false;
        } else {
            if (l_digit == 9 && r_digit == 9) {
                all_nines = true;
            } else {
                ++K[left];
                ++K[left + 1] = '0';
                K[right] = K[left];
            }
        }

        return;
    }

    bool sub_all_nines;
    NextBiggestPalindrome(K, left + 1, right - 1, sub_all_nines);

    if (sub_all_nines) {
        int l_digit = CharToDigit(K[left]), r_digit = CharToDigit(K[right]);
        if (l_digit == 9 && r_digit == 9) {
            all_nines = true;
        } else if (r_digit < l_digit) {
            K[right] = K[left];
            all_nines = false;
        } else {
            // Return F0...0F where F = digit after the first digit of K, number of 0's =
            // K.size() - 2.
            std::fill(K.begin() + left + 1, K.begin() + right - 1, '0');
            ++K[left];
            K[right] = K[left];
            all_nines = false;
        }
    } else {
        K[right] = K[left];
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

// Optimized version (no copies).
void NextBiggestPalindrome(std::string& K) {
    bool all_nines;
    NextBiggestPalindrome(K, 0, K.size() - 1, all_nines);

    if (all_nines) {
        // Return 10...01 where number of 0's = K.size().
        std::fill(K.begin(), K.end(), '0');
        K.front() = '1';
        K += '1';
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

void RandomlyTest(const size_t n, const size_t max_length, bool measure_time = false,
                  bool use_optimized = false, bool print_num = false) {
    std::random_device dev;
    std::mt19937 rng{dev()};
    std::uniform_int_distribution<int> digit_dist{0, 9};
    std::uniform_int_distribution<int> len_dist(1, max_length);

    for (size_t i = 0; i < n; ++i) {
        // Generate a random number length.
        size_t len;
        if (measure_time) len = max_length;
        else len = len_dist(rng);

        // Generate a random number (as a digit string) of length `len`.
        std::string number;
        number.reserve(len);
        for (size_t j = 0; j < len; ++j)
            number += '0' + digit_dist(rng);

        // Unless we're also printing timing information, add a progress bar.
        if (!measure_time) {
            std::cout << "Trial " << i + 1 << " of " << n;
            if (print_num) std::cout << " (number: " << number << ", length: " << len << ")";
            else std::cout << " (number length: " << len << ")";
            std::cout << '\r' << std::flush;
        } else if (print_num) {
            std::cout << "number: " << number << ", length: " << len << std::endl;
        }

        // Brute-force the expected answer.
        const std::string& palindrome = BruteForceNextPalindrome(number);

        // Compute our answer (stored back in `number`).
        std::chrono::time_point<std::chrono::high_resolution_clock> start, end;
        if (use_optimized) {
            if (measure_time) start = std::chrono::high_resolution_clock::now();
            NextBiggestPalindrome(number);
            if (measure_time) end = std::chrono::high_resolution_clock::now();
        } else {
            std::vector<char> vec_number (number.begin(), number.end());
            if (measure_time) start = std::chrono::high_resolution_clock::now();
            NextBiggestPalindrome(vec_number);
            if (measure_time) end = std::chrono::high_resolution_clock::now();
            number = std::string(vec_number.begin(), vec_number.end());
        }

        if (measure_time) {
            const auto& elapsed =
                std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
            std::cout << "Execution time (length " << len << "): " << elapsed << std::endl;
        }

        if (number != palindrome) {
            std::cout << "FAILED: " << number << " (expected: " << palindrome << ", got: "
                << number << ")" << std::endl;
            return;
        }
    }

    if (!measure_time) // Clean up progress bar
        std::cout << std::endl;

    std::cout << "Tested " << n << " examples with no failures" << std::endl;
}


// Local stuff (not submitted) for debugging

bool ParseArgsAndTest(int argc, char **argv) {
    if (argc <= 1) return false;

    bool test = false, measure_time = false, use_optimized = false, print_num = false;
    size_t n = kDefaultN, max_length = kDefaultMaxLength;
    // Parse arguments.
    for (size_t i = 1; i < argc; ++i) {
        std::string arg_str{argv[i]};
        if (argc == 2) { // Special case
            NextBiggestPalindrome(arg_str);
            std::cout << arg_str << std::endl;
            return true;
        } else if (arg_str == "--test") {
            test = true;
        } else if (arg_str == "--n") {
            ++i;
            n = std::stoi(argv[i]);
        } else if (arg_str == "--max-length") {
            ++i;
            max_length = std::stoi(argv[i]);
        } else if (arg_str == "--time") {
            measure_time = true;
        } else if (arg_str == "--optimized") {
            use_optimized = true;
        } else if (arg_str == "--print-num") {
            print_num = true;
        } else {
            std::cerr << "Unrecognized argument: " << arg_str << std::endl;
            exit(1);
        }
    }

    if (!test) return false;

    std::cout << "Randomly testing " << n << " iterations, maximum number length is "
        << max_length << std::endl;
    RandomlyTest(n, max_length, measure_time, use_optimized, print_num);

    return true;
}


// Main

int main(int argc, char **argv) {
    if (ParseArgsAndTest(argc, argv)) return 0;

    std::string t;
    std::getline(std::cin, t);

    for (std::string K; std::getline(std::cin, K);) {
        NextBiggestPalindrome(K);
        std::cout << K << std::endl;
    }

    return 0;
}
