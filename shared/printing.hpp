#include <iostream>
#include <vector>
namespace aoc {

template <class T>
constexpr void printVector(const std::vector<T>& vec, bool newlines = true) {
    for (const T& e : vec) {
        std::cout << e;
        if (newlines) {
            std::cout << std::endl;
        }
    }

    if (newlines == false) {
        std::cout << std::endl;
    }
}

template <class T>
constexpr void printVector(const std::vector<std::vector<T>> vec) {
    for (const std::vector<T>& row : vec) {
        for (const T& e : row) {
            std::cout << e << " ";
        }
        std::cout << std::endl;
    }
}

}
