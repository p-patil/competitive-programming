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

constexpr size_t kMaxDigits = 1000000;
constexpr size_t kDefaultMaxLength = 500;
constexpr size_t kDefaultN = 1000;

void NextBiggestPalindrome(std::vector<char>& K);
void NextBiggestPalindrome(std::string& K);
void NextBiggestPalindromeIterative(std::string& K);

// This code will only be run locally, for debugging purposes. To use it, compile with "-D LOCAL".
#ifdef LOCAL

#include "BigInteger/BigInteger.h"

inline bool IsPalindrome(const std::string& number) {
    return std::equal(number.begin(), number.begin() + number.size() / 2, number.rbegin());
}

std::string BruteForceNextPalindrome(const std::string& number) {
    BigInteger big_num{number};
    ++big_num; // Next palindrome must be bigger

    while (!IsPalindrome(big_num.getNumber())) ++big_num;

    return big_num.getNumber();
}

// Randomized testing.
void RandomlyTest(const size_t n, const size_t max_length, bool measure_time = false,
                  bool use_optimized = false, bool use_iterative = false, bool print_num = false) {
    assert(!(use_optimized && use_iterative));

    size_t last_line_length = 0; // Used for printing the progress bar

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
            std::string line = "Trial " + std::to_string(i + 1) + " of " + std::to_string(n);
            if (print_num) line += " (number: " + number + ", length: " + std::to_string(len) + ")";
            else line += + " (number length: " + std::to_string(len) + ")";

            int diff = last_line_length - line.size();
            last_line_length = line.size();
            if (diff > 0) line += std::string(diff, ' ');

            std::cout << line << '\r' << std::flush;
        } else if (print_num) {
            std::cout << "number: " << number << ", length: " << len << std::endl;
        }

        // Compute our answer.
        std::string answer = number;
        std::chrono::time_point<std::chrono::high_resolution_clock> start, end;
        if (use_optimized) {
            if (measure_time) start = std::chrono::high_resolution_clock::now();
            NextBiggestPalindrome(answer);
            if (measure_time) end = std::chrono::high_resolution_clock::now();
        } else if (use_iterative) {
            if (measure_time) start = std::chrono::high_resolution_clock::now();
            NextBiggestPalindromeIterative(answer);
            if (measure_time) end = std::chrono::high_resolution_clock::now();
        } else {
            std::vector<char> vec_number (answer.begin(), answer.end());
            if (measure_time) start = std::chrono::high_resolution_clock::now();
            NextBiggestPalindrome(vec_number);
            if (measure_time) end = std::chrono::high_resolution_clock::now();
            answer = std::string(vec_number.begin(), vec_number.end());
        }

        // Brute-force the expected answer.
        const std::string& palindrome = BruteForceNextPalindrome(number);

        if (measure_time) {
            const auto& elapsed =
                std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
            std::cout << "Execution time (length " << len << "): " << elapsed << std::endl;
        }

        if (answer != palindrome) {
            if (!measure_time) std::cout << std::endl; // Clean up progress bar
            std::cout << "FAILED: " << number << " (expected: " << palindrome << ", got: "
                << answer << ")" << std::endl;
            return;
        }
    }

    if (!measure_time) std::cout << std::endl; // Clean up progress bar
    std::cout << "Tested " << n << " examples with no failures" << std::endl;
}

bool ParseArgsAndTest(int argc, char **argv) {
    if (argc <= 1) return false;

    bool test = false, measure_time = false, use_optimized = false, use_iterative = false,
         print_num = false;
    size_t n = kDefaultN, max_length = kDefaultMaxLength;
    // Parse arguments.
    for (size_t i = 1; i < argc; ++i) {
        std::string arg_str{argv[i]};
        if (argc == 2) { // Special case
            //NextBiggestPalindrome(arg_str);
            //std::cout << arg_str << std::endl;

            //std::vector<char> v{arg_str.begin(), arg_str.end()};
            //NextBiggestPalindrome(v);
            //std::cout << std::string(v.begin(), v.end()) << std::endl;

            NextBiggestPalindromeIterative(arg_str);
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
        } else if (arg_str == "--iterative") {
            use_iterative = true;
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
    RandomlyTest(n, max_length, measure_time, use_optimized, use_iterative, print_num);

    return true;
}

#else
bool ParseArgsAndTest(int argc, char **argv) { return false; }
#endif

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
            // K.size() - 2. Don't subtract 1 from `right` because the 2nd argument of std::fill
            // takes a range with exclusive end.
            std::fill(K.begin() + left + 1, K.begin() + right, '0');
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

// Iterative solution

void NextBiggestPalindromeIterative(std::string& K) {
    assert(!K.empty());
    if (K.size() == 1) { // Special case, handle differently because below we'd have `left == -1`
        if (K[0] != '9') ++K[0];
        else K = "11";
        return;
    }

    bool odd = K.size() % 2 == 1;
    int left, right, middle;

    left = K.size() / 2 - 1;
    if (odd) {
        middle = left + 1; // Only used if `odd`
        right = left + 2;
    } else {
        right = left + 1;
    }

    // Find right most number in 2nd half that differs from its mirror counterpart.
    while (K[left] == K[right] && left >= 0 && right < K.size()) {
        --left;
        ++right;
    }

    // If K is already a palindrome, increment the center digit (if `odd`) or 2 digits (if even).
    if (left < 0 || right >= K.size()) {
        assert(left < 0 && right >= K.size());
        left = K.size() / 2 - 1;
        if (odd) right = left + 2;
        else right = left + 1;

        // If K has an odd length, we can just increment the middle digit. But if this digit is a 9,
        // we need to carry.
        if (odd && K[middle] != '9') {
            ++K[middle];
            return;
        }

        // Incrementing 9's requires a carry, so find the digits bounding any middle 9's.
        while (K[left] == '9' && left >= 0 && right < K.size()) {
            assert(K[left] == K[right]);
            --left;
            ++right;
        }

        // If the string is an all 9's palindrome, return 10...01 where there are `K.size()` zeros.
        if (left < 0 || right >= K.size()) {
            assert(left < 0 && right >= K.size());
            K[0] = '1';
            std::fill(K.begin() + 1, K.end(), '0');
            K += '1';
        // Otherwise, increment the digits immediately surrounding the 9's.
        } else {
            assert(K[left] == K[right]);
            std::fill(K.begin() + left + 1, K.begin() + right, '0'); // Not `right - 1` because the
                                                                     // end bound is exclusive
            ++K[left];
            ++K[right];
        }
    // Otherwise, everything from `left + 1` to `right - 1` is a palindrome, so we want to copy
    // everything from 0 to `left` in reverse into `right` to `K.size() - 1`.
    } else {
        // If we get lucky, we can directly increase the right value to match the left's, and then
        // are free to copy the remaining portions of each half, no carry required.
        if (K[right] < K[left]) {
            while (left >= 0 && right < K.size()) {
                K[right] = K[left];
                --left;
                ++right;
            }

            assert(left < 0 && right >= K.size());
        // Otherwise, making the right value match the left value (after which we're again free to
        // directly reflect the remaining portions of each half) requires a carry. But all the
        // digits between `left` and `right` are already a palindrome, so incrementing the digit at
        // `right - 1` via the carry will disrupt the internal palindrome, requiring another carry.
        // These carries "propagate" until we get to the middle digit (if `odd`) or 2 digits
        // (otherwise), which we can simply increment.
        } else {
            int center_left = K.size() / 2 - 1, center_right;
            if (odd) center_right = center_left + 2;
            else center_right = center_left + 1;

            // If K has an odd length, we can just increment the middle digit.
            if (odd && K[middle] != '9') {
                ++K[middle];
            // But if this digit is a 9, we need to carry. If K has even length, there's no middle
            // digit at all so we need to carry anyways.
            } else {
                if (odd && K[middle] == '9') K[middle] = '0';

                while (K[center_left] == '9') { // Handle special case where we have middle 9's
                    assert(K[center_right] == '9');
                    K[center_left] = K[center_right] = '0';

                    --center_left;
                    ++center_right;
                }

                // Note that in the worse case, the above loop stops when `center_left == left`,
                // since the `K[left]` can't be a 9 (since `K[right] > K[left]`).
                ++K[center_left];
                K[center_right] = K[center_left];
            }

            while (left >= 0 && right < K.size()) {
                K[right] = K[left];
                --left;
                ++right;
            }
        }
    }
}


// Main

int main(int argc, char **argv) {
    if (ParseArgsAndTest(argc, argv)) return 0;

    std::string t;
    std::getline(std::cin, t);

    for (std::string K; std::getline(std::cin, K);) {
        NextBiggestPalindromeIterative(K);
        std::cout << K << std::endl;
    }

    return 0;
}
