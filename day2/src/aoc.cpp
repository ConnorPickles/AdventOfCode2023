#include "../../shared/input.hpp"
#include <iostream>
#include <regex>

int main(int argc, char** argv) {
    std::vector<std::string> input = aoc::parseInput(argv[1]);
    size_t sum = 0;

    std::regex regex("([0-9]+) (red|green|blue)");
    std::smatch matches;

    for (int i = 0; i < input.size(); i++) {
        std::map<std::string, size_t> max_cubes = {
            {"red", 0},
            {"green", 0},
            {"blue", 0}};

        while (regex_search(input[i], matches, regex)) {
            size_t number = stoi(matches[1]);
            std::string color = matches[2];
            max_cubes[color] = std::max(max_cubes[color], number);
            input[i] = matches.suffix();
        }

        /*
        if (max_cubes["red"] <= 12 && max_cubes["green"] <= 13 && max_cubes["blue"] <= 14) {
            sum += i + 1;
        }
        */

        sum += max_cubes["red"] * max_cubes["green"] * max_cubes["blue"];
    }

    std::cout << sum << std::endl;
}
