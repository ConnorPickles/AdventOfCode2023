#include "../../shared/input.hpp"
#include <algorithm>
#include <cmath>
#include <iostream>
#include <regex>

size_t proccessCards(std::vector<std::string>& cards, const std::vector<std::string>& og_cards, size_t depth) {
    std::cout << "Pass number " << depth << ". Cards to process: " << cards.size() << std::endl;
    if (cards.size() == 0) {
        return 0;
    }

    std::vector<std::string> new_cards;
    const std::regex numbers_regex("[0-9]+:|[0-9]+|\\|");
    const std::regex card_regex("Card +([0-9]+):");
    std::smatch match;

    for (std::string line : cards) {
        size_t wins = 0;
        std::vector<std::string> winning_numbers;

        std::regex_search(line, match, card_regex);
        size_t card_id;
        card_id = stoi(match[1]);

        while (std::regex_search(line, match, numbers_regex)) {
            if (match[0].str()[match[0].length() - 1] == ':') {
                line = match.suffix();
                continue;
            }
            if (match[0] == "|") {
                line = match.suffix();
                break;
            }

            winning_numbers.push_back(match[0]);
            line = match.suffix();
        }

        while (std::regex_search(line, match, numbers_regex)) {
            if (std::find(winning_numbers.begin(), winning_numbers.end(), match[0]) != winning_numbers.end()) {
                wins++;
            }
            line = match.suffix();
        }

        for (int i = 0; i < wins; i++) {
            std::string new_card = og_cards[card_id + i];
            new_cards.push_back(new_card);
        }
    }

    return cards.size() + proccessCards(new_cards, og_cards, depth + 1);
}

int main(int argc, char** argv) {
    std::vector<std::string> input = aoc::parseInput(argv[1]);
    std::vector<std::string> input_copy = input;
    std::cout << proccessCards(input, input_copy, 0) << std::endl;
}
