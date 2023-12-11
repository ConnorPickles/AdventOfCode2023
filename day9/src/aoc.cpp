#include "../../shared/input.hpp"
#include <iostream>

bool isZeroSequence(const std::vector<int>& seq) {
    for (int num : seq) {
        if (num) {
            return false;
        }
    }

    return true;
}

std::vector<int> diffSequence(const std::vector<int>& seq) {
    std::vector<int> diff(seq.size() - 1);

    for (int i = 0; i < seq.size() - 1; i++) {
        diff[i] = seq[i + 1] - seq[i];
    }

    return diff;
}

int extrapolate(const std::vector<int>& seq) {
    std::vector<int> diff = diffSequence(seq);
    if (isZeroSequence(diff)) {
        return seq[0];
    }

    return seq[0] - extrapolate(diff);
}

int main(int argc, char** argv) {
    std::vector<std::string> input = aoc::parseInput(argv[1]);
    std::vector<std::vector<int>> histories;
    for (std::string line : input) {
        histories.push_back(aoc::extractNumsFromString(line));
    }

    int sum = 0;
    for (auto history : histories) {
        sum += extrapolate(history);
    }

    std::cout << sum << std::endl;
}
