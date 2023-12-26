#include "../../shared/input.hpp"
#include "../../shared/printing.hpp"
#include <functional>
#include <iostream>
#include <map>

size_t numAssignments(const std::string& record, const std::vector<int>& group) {
    std::map<std::tuple<size_t, size_t, size_t>, size_t> cache;

    std::function<size_t(size_t, size_t, size_t)> solve;
    solve = [&](size_t position, size_t numGroups, size_t currGroupSize) -> size_t {
        if (cache.count({position, numGroups, currGroupSize})) {
            return cache[{position, numGroups, currGroupSize}];
        }

        if (position == record.size()) {
            if (numGroups == group.size() && currGroupSize == 0) {
                cache[{position, numGroups, currGroupSize}] = 1;
                return 1;
            } else {
                cache[{position, numGroups, currGroupSize}] = 0;
                return 0;
            }
        } else if (numGroups > group.size()) {
            cache[{position, numGroups, currGroupSize}] = 0;
            return 0;
        }

        size_t answer = 0;

        if (record[position] == '#' || record[position] == '?') {
            answer += solve(position + 1, numGroups, currGroupSize + 1);
        }

        if (record[position] == '.' || record[position] == '?') {
            if (currGroupSize > 0) {
                if (group[numGroups] == currGroupSize) {
                    answer += solve(position + 1, numGroups + 1, 0);
                }
                // no else here, if the group size doesn't match the expected size we just don't add to the total
            } else {
                answer += solve(position + 1, numGroups, 0);
            }
        }

        cache[{position, numGroups, currGroupSize}] = answer;
        return answer;
    };

    return solve(0, 0, 0);
}

int main(int argc, char** argv) {
    std::vector<std::string> input = aoc::parseInput(argv[1]);
    std::vector<std::string> records;
    std::vector<std::vector<int>> groups;

    for (std::string& line : input) {
        std::vector<std::string> record = aoc::splitStringIntoWords(line);
        records.push_back(record[0]);
        groups.push_back(aoc::parseInts(aoc::splitString(record[1], ',')));
    }

    // part 2
    for (int i = 0; i < records.size(); i++) {
        std::string originalRecord = records[i];
        std::vector<int> originalGroup = groups[i];
        for (int j = 0; j < 4; j++) {
            records[i].push_back('?');
            for (int k = 0; k < originalRecord.size(); k++) {
                records[i].push_back(originalRecord[k]);
            }
            for (int k = 0; k < originalGroup.size(); k++) {
                groups[i].push_back(originalGroup[k]);
            }
        }
    }

    // groups always end with a '.' this way, makes detecting a whole group easier
    for (std::string& record : records) {
        record.push_back('.');
    }

    size_t answer = 0;
    for (int i = 0; i < records.size(); i++) {
        answer += numAssignments(records[i], groups[i]);
    }

    std::cout << answer << std::endl;
}
