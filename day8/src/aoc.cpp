#include "../../shared/input.hpp"
#include <iostream>
#include <numeric>
#include <regex>

std::string followInstruction(const std::tuple<std::string, std::string>& node, const char instruction) {
    if (instruction == 'L') {
        return std::get<0>(node);
    } else {
        return std::get<1>(node);
    }
}

bool isEndNode(const std::string& node, const std::vector<std::string>& endNodes) {
    if (std::find(endNodes.begin(), endNodes.end(), node) == endNodes.end()) {
        return false;
    } else {
        return true;
    }
}

bool allEndNodes(const std::vector<std::string>& currNodes, const std::vector<std::string>& endNodes) {
    for (std::string curr : currNodes) {
        if (isEndNode(curr, endNodes) == false) {
            return false;
        }
    }

    return true;
}

int main(int argc, char** argv) {
    std::vector<std::string> input = aoc::parseInput(argv[1]);
    std::string instructions = input[0];
    std::map<std::string, std::tuple<std::string, std::string>> nodes;

    const std::regex regex("[A-Z0-9]{3}");
    std::smatch match;

    for (int i = 2; i < input.size(); i++) {
        std::regex_search(input[i], match, regex);
        std::string node = match[0];
        input[i] = match.suffix();
        std::regex_search(input[i], match, regex);
        std::string left = match[0];
        input[i] = match.suffix();
        std::regex_search(input[i], match, regex);
        std::string right = match[0];
        input[i] = match.suffix();

        nodes[node] = {left, right};
    }

    std::vector<std::string> startingNodes;
    std::vector<std::string> endNodes;
    for (const auto [key, _] : nodes) {
        if (key[2] == 'A') {
            startingNodes.push_back(key);
        } else if (key[2] == 'Z') {
            endNodes.push_back(key);
        }
    }

    std::vector<std::vector<std::tuple<size_t, size_t>>> allPossibleSteps;
    for (const std::string& start : startingNodes) {
        std::vector<std::tuple<size_t, size_t>> possibleSteps;
        size_t steps = 0;
        std::string curr = std::move(start);
        size_t firstEndIndex = 0;
        for (int i = 0;; i = (i + 1) % instructions.size()) {
            if (isEndNode(curr, endNodes)) {
                firstEndIndex = i;
                possibleSteps.push_back({steps, i});
                break;
            }
            curr = followInstruction(nodes[curr], instructions[i]);
            steps++;
        }

        curr = followInstruction(nodes[curr], instructions[firstEndIndex]);
        steps++;
        for (int i = (firstEndIndex + 1) % instructions.size(); isEndNode(curr, endNodes) == false || i != firstEndIndex; i = (i + 1) % instructions.size()) {
            if (isEndNode(curr, endNodes)) {
                possibleSteps.push_back({steps, i});
            }
            curr = followInstruction(nodes[curr], instructions[i]);
            steps++;
        }

        allPossibleSteps.push_back(std::move(possibleSteps));
    }

    size_t answer;
    for (auto endIndex : allPossibleSteps[0]) {
        size_t steps = std::get<0>(endIndex);
        size_t index = std::get<1>(endIndex);
        bool matchingIndex;
        for (int i = 1; i < allPossibleSteps.size(); i++) {
            matchingIndex = false;
            for (auto endNode : allPossibleSteps[i]) {
                if (std::get<1>(endNode) != index) {
                    continue;
                }
                matchingIndex = true;
                steps = std::lcm(steps, std::get<0>(endNode));
                break;
            }
            if (matchingIndex == false) {
                break;
            }
        }
        if (matchingIndex == false) {
            continue;
        }

        answer = steps;
    }
    std::cout << answer << std::endl;
}
