#include <exception>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

namespace aoc {

inline std::vector<std::string> parseInput(const std::string& filename) {
    std::vector<std::string> input;

    std::ifstream file(filename);
    std::string input_str;
    while (std::getline(file, input_str)) {
        input.push_back(input_str);
    }

    return input;
}

inline std::vector<int> extractNumsFromString(const std::string& str) {
    std::vector<int> nums;
    std::stringstream ss;
    ss << str;
    std::string word;
    int tmp_num;
    while (!ss.eof()) {
        ss >> word;
        if (std::stringstream(word) >> tmp_num) {
            nums.push_back(tmp_num);
        }
        word = "";
    }

    return nums;
}


inline std::vector<std::string> splitString(const std::string& str, char delim) {
    std::vector<std::string> words;
    std::string word = "";

    for (char c : str) {
        if (c == delim) {
            if (word == "") {
                continue;
            }
            words.push_back(word);
            word = "";
        } else {
            word = word + c;
        }
    }
    words.push_back(word);

    return words;
}

inline std::vector<std::string> splitStringIntoWords(const std::string& str) {
    return splitString(str, ' ');
}

inline std::vector<int> parseInts(const std::vector<std::string>& strings) {
    std::vector<int> output;
    try {
        for (const std::string& str : strings) {
            output.push_back(stoi(str));
        }
    } catch (std::exception error) {
        std::cout << "Error while trying to parse strings to ints:\n" << error.what() << std::endl;
        exit(1);
    }

    return output;
}

} // namespace aoc
