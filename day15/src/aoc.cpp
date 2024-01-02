#include "../../shared/input.hpp"
#include "../../shared/printing.hpp"
#include <iostream>
#include <map>

using FocalLength = unsigned short;
using Lens = std::pair<std::string, FocalLength>;
using Box = typename std::vector<Lens>;

int hash(const std::string& str) {
    int hash = 0;
    for (const char& c : str) {
        hash += c;
        hash *= 17;
        hash %= 256;
    }

    return hash;
}

int indexOfLabelInBox(const Box& box, const std::string& label) {
    for (int i = 0; i < box.size(); i++) {
        if (box[i].first == label) {
            return i;
        }
    }

    return -1;
}

int main(int argc, char** argv) {
    std::vector<std::string> input = aoc::parseInput(argv[1]);
    std::vector<std::string> strings = aoc::splitString(input[0], ',');
    std::vector<Box> boxes(256);

    for (std::string& str : strings) {
        if (str[str.size() - 1] == '-') {
            str.erase(str.end() - 1);
            int box_index = hash(str);
            for (int i = 0; i < boxes[box_index].size(); i++) {
                if (boxes[box_index][i].first == str) {
                    boxes[box_index].erase(boxes[box_index].begin() + i);
                    break;
                }
            }
        } else {
            auto tmp = aoc::splitString(str, '=');
            std::string label = tmp[0];
            FocalLength fl = stoi(tmp[1]);
            int box_index = hash(label);
            int label_index = indexOfLabelInBox(boxes[box_index], label);
            if (label_index == -1) {
                boxes[box_index].push_back(std::make_pair(label, fl));
            } else {
                boxes[box_index][label_index] = std::make_pair(label, fl);
            }
        }
    }

    size_t answer = 0;
    for (int i = 0; i < boxes.size(); i++) {
        for (int j = 0; j < boxes[i].size(); j++) {
            answer += (1 + i) * (1 + j) * boxes[i][j].second;
        }
    }

    std::cout << answer << std::endl;
}
