#include "../../shared/input.hpp"
#include <algorithm>
#include <iostream>
#include <map>

enum class Card { Joker,
                  Two,
                  Three,
                  Four,
                  Five,
                  Six,
                  Seven,
                  Eight,
                  Nine,
                  Ten,
                  Queen,
                  King,
                  Ace };

const std::vector AllCards{Card::Joker, Card::Two, Card::Three, Card::Four, Card::Five, Card::Six, Card::Seven, Card::Eight, Card::Nine, Card::Ten, Card::Queen, Card::King, Card::Ace};

typedef std::vector<Card> Hand;

enum class HandType { HighCard,
                      OnePair,
                      TwoPair,
                      ThreeOfAKind,
                      FullHouse,
                      FourOfAKind,
                      FiveOfAKind };

void printCard(const Card& card, bool newline = true) {
    switch (card) {
    case Card::Two:
        std::cout << '2';
        break;
    case Card::Three:
        std::cout << '3';
        break;
    case Card::Four:
        std::cout << '4';
        break;
    case Card::Five:
        std::cout << '5';
        break;
    case Card::Six:
        std::cout << '6';
        break;
    case Card::Seven:
        std::cout << '7';
        break;
    case Card::Eight:
        std::cout << '8';
        break;
    case Card::Nine:
        std::cout << '9';
        break;
    case Card::Ten:
        std::cout << 'T';
        break;
    case Card::Joker:
        std::cout << 'J';
        break;
    case Card::Queen:
        std::cout << 'Q';
        break;
    case Card::King:
        std::cout << 'K';
        break;
    case Card::Ace:
        std::cout << 'A';
        break;
    default:
        std::cout << 'X';
    }

    if (newline) {
        std::cout << std::endl;
    }
}

void printHand(const Hand& hand, bool newline = true) {
    for (Card card : hand) {
        printCard(card, false);
    }

    if (newline) {
        std::cout << std::endl;
    }
}

void printHandType(const HandType& ht, bool newline = true) {
    switch (ht) {
    case HandType::HighCard:
        std::cout << "High Card";
        break;
    case HandType::OnePair:
        std::cout << "One Pair";
        break;
    case HandType::TwoPair:
        std::cout << "Two Pair";
        break;
    case HandType::ThreeOfAKind:
        std::cout << "Three of a Kind";
        break;
    case HandType::FullHouse:
        std::cout << "Full House";
        break;
    case HandType::FourOfAKind:
        std::cout << "Four of a Kind";
        break;
    case HandType::FiveOfAKind:
        std::cout << "Five of a Kind";
        break;
    default:
        std::cout << "ERROR";
    }

    if (newline) {
        std::cout << std::endl;
    }
}

Card charToCard(const char& c) {
    switch (c) {
    case '2':
        return Card::Two;
        break;
    case '3':
        return Card::Three;
        break;
    case '4':
        return Card::Four;
        break;
    case '5':
        return Card::Five;
        break;
    case '6':
        return Card::Six;
        break;
    case '7':
        return Card::Seven;
        break;
    case '8':
        return Card::Eight;
        break;
    case '9':
        return Card::Nine;
        break;
    case 'T':
        return Card::Ten;
        break;
    case 'J':
        return Card::Joker;
        break;
    case 'Q':
        return Card::Queen;
        break;
    case 'K':
        return Card::King;
        break;
    case 'A':
        return Card::Ace;
        break;
    }

    std::cout << "No card corresponding to " << c << std::endl;
    exit(1);
}

HandType determineHandTypeWithoutJokers(std::map<Card, size_t> cards) {
    if (cards.size() == 1) {
        return HandType::FiveOfAKind;
    }

    if (cards.size() == 2) {
        size_t count = cards.begin()->second;
        if (count == 1 || count == 4) {
            return HandType::FourOfAKind;
        }
        if (count == 2 || count == 3) {
            return HandType::FullHouse;
        }
    }

    if (cards.size() == 3) {
        for (auto const& [key, val] : cards) {
            if (val == 3) {
                return HandType::ThreeOfAKind;
            }

            if (val == 2) {
                return HandType::TwoPair;
            }
        }
    }

    if (cards.size() == 4) {
        return HandType::OnePair;
    }

    if (cards.size() == 5) {
        return HandType::HighCard;
    }

    std::cout << "Failed to determine hand type";
    exit(1);
}

HandType determineHandType(const Hand& hand) {
    std::map<Card, size_t> cards;
    for (Card card : hand) {
        if (cards.count(card)) {
            cards[card]++;
        } else {
            cards[card] = 1;
        }
    }

    if (cards.count(Card::Joker) == 0) {
        return determineHandTypeWithoutJokers(cards);
    }

    if (cards.size() == 1 || cards.size() == 2) {
        return HandType::FiveOfAKind;
    }

    if (cards.size() == 3) {
        // 1 2 2
        // 1 1 3
        // 2 2 1
        // 3 1 1

        if (cards[Card::Joker] == 3 || cards[Card::Joker] == 2) {
            return HandType::FourOfAKind;
        }

        for (auto const& [key, val] : cards) {
            if (key == Card::Joker) {
                continue;
            }

            if (val == 2) {
                return HandType::FullHouse;
            } else {
                return HandType::FourOfAKind;
            }
        }
    }

    if (cards.size() == 4) {
        return HandType::ThreeOfAKind;
    }

    if (cards.size() == 5) {
        return HandType::OnePair;
    }

    std::cout << "Could not determine hand type for hand: ";
    printHand(hand);
    exit(1);
}

bool compareHands(const Hand& a, const Hand& b) {
    HandType aType = determineHandType(a);
    HandType bType = determineHandType(b);
    if (aType != bType) {
        return aType < bType;
    }

    for (int i = 0; i < a.size(); i++) {
        if (a[i] != b[i]) {
            return a[i] < b[i];
        }
    }

    return false;
}

int main(int argc, char** argv) {
    std::vector<std::string> input = aoc::parseInput(argv[1]);
    std::vector<std::pair<Hand, size_t>> hands;
    for (std::string line : input) {
        std::vector<std::string> raw = aoc::splitStringIntoWords(line);
        Hand hand;
        for (char c : raw[0]) {
            hand.push_back(charToCard(c));
        }
        hands.push_back({hand, stoi(raw[1])});
    }

    std::sort(hands.begin(), hands.end(),
              [](const std::pair<Hand, size_t>& a, const std::pair<Hand, size_t>& b) {
                  return compareHands(std::get<0>(a), std::get<0>(b));
              });

    size_t answer = 0;
    for (int i = 0; i < hands.size(); i++) {
        answer += (i + 1) * std::get<1>(hands[i]);
    }

    std::cout << answer << std::endl;
}
