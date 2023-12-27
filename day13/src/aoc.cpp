#include "../../shared/input.hpp"
#include "../../shared/printing.hpp"
#include <iostream>

bool hasPerfectVerticalReflection(const std::vector<std::string>& pattern, int ver) {
    bool foundSmudge = false;
    for (int col = ver; col >= 0; col--) {
        for (int row = 0; row < pattern.size(); row++) {
            int rightCol = col + 1 + 2 * (ver - col);
            if (rightCol >= pattern[0].size()) {
                return foundSmudge;
            }
            if (pattern[row][col] != pattern[row][rightCol]) {
                if (foundSmudge == false) {
                    foundSmudge = true;
                } else {
                    return false;
                }
            }
        }
    }

    return foundSmudge;
}

bool hasPerfectHorizontalReflection(const std::vector<std::string>& pattern, int hor) {
    bool foundSmudge = false;
    for (int row = hor; row >= 0; row--) {
        for (int col = 0; col < pattern[0].size(); col++) {
            int bottomRow = row + 1 + 2 * (hor - row);
            if (bottomRow >= pattern.size()) {
                return foundSmudge;
            }
            if (pattern[row][col] != pattern[bottomRow][col]) {
                if (foundSmudge == false) {
                    foundSmudge = true;
                } else {
                    return false;
                }
            }
        }
    }

    return foundSmudge;
}

size_t valueForPattern(const std::vector<std::string>& pattern) {
    for (int ver = 0; ver < pattern[0].size() - 1; ver++) {
        if (hasPerfectVerticalReflection(pattern, ver)) {
            return 1 + ver;
        }
    }

    for (int hor = 0; hor < pattern.size() - 1; hor++) {
        if (hasPerfectHorizontalReflection(pattern, hor)) {
            return 100 * (1 + hor);
        }
    }

    std::cout << "Could not find reflection for the following pattern:" << std::endl;
    aoc::printVector(pattern);
    exit(1);
}

int main(int argc, char** argv) {
    std::vector<std::string> input = aoc::parseInput(argv[1]);
    std::vector<std::vector<std::string>> patterns;

    std::vector<std::string> tmpPattern;
    for (std::string& line : input) {
        if (line == "") {
            patterns.push_back(std::move(tmpPattern));
            tmpPattern.clear();
        } else {
            tmpPattern.push_back(std::move(line));
        }
    }
    patterns.push_back(std::move(tmpPattern));

    size_t answer = 0;
    for (const std::vector<std::string>& pattern : patterns) {
        answer += valueForPattern(pattern);
    }

    std::cout << answer << std::endl;
}
