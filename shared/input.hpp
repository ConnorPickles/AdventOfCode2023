#include <string>
#include <vector>
#include <fstream>
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

} // namespace aoc
