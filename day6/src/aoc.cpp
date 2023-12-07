#include "../../shared/input.hpp"
#include <iostream>

void part1(char** argv) {
    std::vector<std::string> input = aoc::parseInput(argv[1]);
    std::vector<size_t> times = aoc::extractNumsFromString(input[0]);
    std::vector<size_t> distances = aoc::extractNumsFromString(input[1]);

    size_t answer = 1;
    for (int i = 0; i < times.size(); i++) {
        size_t ways_to_win = 0;
        for (int t = 1; t < times[i]; t++) {
            if (t * (times[i] - t) > distances[i]) {
                ways_to_win++;
            }
        }
        if (ways_to_win) {
            answer *= ways_to_win;
        }
    }

    std::cout << answer << std::endl;
}

size_t parseNumFromStrings(std::vector<std::string> strings) {
    std::string acc = "";
    for (int i = 1; i < strings.size(); i++) {
        acc += strings[i];
    }
    return stol(acc);
}

int main(int argc, char** argv) {
    std::vector<std::string> input = aoc::parseInput(argv[1]);
    size_t time = parseNumFromStrings(aoc::splitStringIntoWords(input[0]));
    size_t distance = parseNumFromStrings(aoc::splitStringIntoWords(input[1]));

    size_t ways_to_win = 0;
    for (int t = 1; t < time; t++) {
        if (t * (time - t) > distance) {
            ways_to_win++;
        }
    }

    std::cout << ways_to_win << std::endl;
}
