#include "../../shared/input.hpp"
#include <iostream>

bool isDigit(const char& c) {
    switch (c) {
    case '0':
    case '1':
    case '2':
    case '3':
    case '4':
    case '5':
    case '6':
    case '7':
    case '8':
    case '9':
        return true;
    default:
        return false;
    }
}

bool isSymbol(const char& c) {
    return isDigit(c) == false && c != '.' && c != '\n';
}

size_t extractNumber(std::vector<std::string>& input, const int row, const int col) {
    // find start and end point of the digits
    int numStart = col;
    int numEnd = col;

    do {
        numStart--;
        if (numStart < 0) {
            break;
        }
    } while (isDigit(input[row][numStart]));
    numStart++;

    do {
        numEnd++;
        if (numEnd >= input[row].size()) {
            break;
        }
    } while (isDigit(input[row][numEnd]));
    numEnd--;

    // parse the digits into a number
    size_t number = stoi(std::string(input[row], numStart, numEnd - numStart + 1));

    // change number into periods so it isn't detected again
    for (int i = numStart; i <= numEnd; i++) {
        input[row][i] = '.';
    }

    return number;
}

int main(int argc, char** argv) {
    std::vector<std::string> input = aoc::parseInput(argv[1]);
    size_t sum = 0;

    for (int i = 0; i < input.size(); i++) {
        for (int j = 0; j < input[i].size(); j++) {
            // scan for symbols (anything not a digit, not a period)
            if (isSymbol(input[i][j]) == false) {
                continue;
            }

            int numbersFound = 0;
            size_t ratio = 1;

            // look in the 8 adjacent spots for digits that haven't been used
            for (int row = i - 1; row <= i + 1; row++) {
                for (int col = j - 1; col <= j + 1; col++) {
                    if (row < 0 || row >= input.size() || col < 0 || col >= input[row].size()) {
                        continue;
                    }

                    if (isDigit(input[row][col])) {
                        ratio *= extractNumber(input, row, col);
                        numbersFound++;
                    }

                    if (numbersFound > 2) {
                        break;
                    }
                }
                if (numbersFound > 2) {
                    break;
                }
            }

            if (numbersFound == 2) {
                sum += ratio;
            }
        }
    }

    std::cout << sum << std::endl;
}
