#include <iostream>
#include <unistd.h>
#include <sys/wait.h>

#include "BigInteger/BigInteger.h"


constexpr char kSrcFile[] = "the_next_palindrome.cc";


bool CompileBinary(const std::string& src_file) {
    int pid = fork();
    if (pid == 0) { // Child process
        std::cout << "Trying to compile " << src_file << std::endl;

        const std::string& src_without_ext = src_file.substr(0, src_file.size() - 3);
        execl("/usr/bin/g++", "/usr/bin/g++", "-o", src_without_ext.c_str(), "-std=c++14",
              src_file.c_str(), NULL);

        exit(1); // This should only be reached if compilation fails
    } else {           // Parent process
        int stat;
        waitpid(pid, &stat, 0);
        return stat == 0;
    }
}

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

        // Compute our answer.
        std::string answer = number;
        std::chrono::time_point<std::chrono::high_resolution_clock> start, end;
        if (use_optimized) {
            if (measure_time) start = std::chrono::high_resolution_clock::now();
            NextBiggestPalindrome(answer);
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

int main(int argc, char **argv) {
    if (!CompileBinary(kSrcFile)) {
        std::cerr << "Compilation failed" << std::endl;
        return 1;
    }

    return 0;
}
