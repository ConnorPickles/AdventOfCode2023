#include <fstream>
#include <sstream>
#include <string>
#include <vector>

namespace aoc {

inline std::vector<std::string> parseInput(std::string filename) {
    std::vector<std::string> input;

    std::ifstream file(filename);
    std::string input_str;
    while (std::getline(file, input_str)) {
        input.push_back(input_str);
    }

    return input;
}

inline std::vector<int> extractNumsFromString(std::string str) {
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

inline std::vector<std::string> splitStringIntoWords(std::string str) {
    std::vector<std::string> words;
    std::string word = "";

    for (char c : str) {
        if (c == ' ') {
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

} // namespace aoc
