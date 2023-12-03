#include <fstream>
#include <functional>
#include <iostream>
#include <numeric>
#include <string>
#include <vector>

std::vector<std::string> parseInput(std::string filename) {
    std::vector<std::string> input;

    std::ifstream file(filename);
    std::string input_str;
    while (std::getline(file, input_str)) {
        input.push_back(input_str);
    }

    return input;
}

std::vector<size_t> callFunctionWithInputs(std::function<size_t(std::string, size_t)> func, std::vector<std::string> param1, std::vector<size_t> param2) {
    std::vector<size_t> results(param1.size());

    for (size_t i = 0; i < param1.size(); i++) {
        results.push_back(func(param1[i], param2[i]));
    }

    return results;
}

char wordToChar(std::string word) {
    if (word == "zero")
        return '0';
    if (word == "one")
        return '1';
    if (word == "two")
        return '2';
    if (word == "three")
        return '3';
    if (word == "four")
        return '4';
    if (word == "five")
        return '5';
    if (word == "six")
        return '6';
    if (word == "seven")
        return '7';
    if (word == "eight")
        return '8';
    if (word == "nine")
        return '9';
    return 'x';
}

char findFirstDigit(std::string str, std::vector<std::string> digits) {
    size_t first = std::string::npos;
    char res;

    first = str.find_first_of("0123456789");
    if (first != std::string::npos) {
        res = str[first];
    }

    for (std::string digit : digits) {
        size_t index = str.find(digit);
        if (index == std::string::npos) {
            continue;
        }

        if (index < first) {
            first = index;
            res = wordToChar(digit);
        }
    }

    if (first == std::string::npos) {
        std::cerr << "Did not find a digit in: " << str << std::endl;
        exit(1);
    }

    return res;
}

char findLastDigit(std::string str, std::vector<std::string> digits) {
    size_t last = std::string::npos;
    char res;

    last = str.find_last_of("0123456789");
    if (last != std::string::npos) {
        res = str[last];
    }

    for (std::string digit : digits) {
        size_t index = str.rfind(digit);
        if (index == std::string::npos) {
            continue;
        }

        if (index > last || last == std::string::npos) {
            last = index;
            res = wordToChar(digit);
        }
    }

    if (last == std::string::npos) {
        std::cerr << "Did not find a digit in: " << str << std::endl;
        exit(1);
    }

    return res;
}

int main(int argc, char** argv) {
    std::vector<std::string> input = parseInput(argv[1]);
    size_t sum = 0;
    std::vector<std::string> digits{
        "zero",
        "one",
        "two",
        "three",
        "four",
        "five",
        "six",
        "seven",
        "eight",
        "nine",
    };

    for (auto line : input) {
        char firstChar = findFirstDigit(line, digits);
        char lastChar = findLastDigit(line, digits);
        std::string line_result = std::string() + firstChar + lastChar;
        sum += stoi(line_result);
    }

    std::cout << sum << std::endl;
}
