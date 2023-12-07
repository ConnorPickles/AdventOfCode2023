#include "../../shared/input.hpp"
#include <algorithm>
#include <assert.h>
#include <iostream>
#include <sstream>
#include <tuple>

typedef std::tuple<size_t, size_t, size_t> Mapping;
typedef std::tuple<size_t, size_t> Range;

std::vector<size_t> extractNumsFromString(std::string str) {
    std::vector<size_t> nums;
    std::stringstream ss;
    ss << str;
    std::string word;
    size_t tmp_num;
    while (!ss.eof()) {
        ss >> word;
        if (std::stringstream(word) >> tmp_num) {
            nums.push_back(tmp_num);
        }
        word = "";
    }

    return nums;
}

int diff(size_t x, size_t y) {
    return std::abs((int)x - (int)y);
}

void sortRanges(std::vector<Range>& ranges) {
    std::sort(ranges.begin(), ranges.end(),
              [](Range const& a, Range const& b) {
                  return std::get<0>(a) < std::get<0>(b);
              });
}

void printRange(const Range& range, bool newline = true) {
    std::cout << "[" << std::get<0>(range) << ", " << std::get<0>(range) + std::get<1>(range) << ") ";
    if (newline) {
        std::cout << std::endl;
    }
}

void printRanges(const std::vector<Range>& ranges) {
    for (auto range : ranges) {
        printRange(range, false);
    }
    std::cout << std::endl;
}

void printMapping(const Mapping& map, bool newline = true) {
    const size_t length = std::get<2>(map);
    std::cout << "[" << std::get<1>(map) << ", " << std::get<1>(map) + length << ") -> [" << std::get<0>(map) << ", " << std::get<0>(map) + length << ")";
    if (newline) {
        std::cout << std::endl;
    }
}

std::vector<Range> findDestination(const Range& range, const std::vector<Mapping>& map) {
    std::vector<Range> results;
    std::vector<Range> mapped;

    const auto [rangeStart, rangeLength] = range;
    const size_t rangeEnd = rangeStart + rangeLength;
    for (Mapping m : map) {
        const auto [destStart, sourceStart, mapLength] = m;
        const size_t sourceEnd = sourceStart + mapLength;

        if (rangeStart >= sourceStart && rangeEnd <= sourceEnd) {
            // map entirely covers range
            results.clear();
            results.push_back({destStart + diff(rangeStart, sourceStart), rangeLength});
            return results;
        } else if (rangeStart < sourceStart && rangeEnd > sourceEnd) {
            // map is entirely within range
            results.push_back({destStart, mapLength});
            mapped.push_back({sourceStart, mapLength});
        } else if (rangeEnd <= sourceEnd && rangeEnd > sourceStart) {
            // map overlaps end of range
            size_t length = rangeLength - diff(sourceStart, rangeStart);
            results.push_back({destStart, length});
            mapped.push_back({sourceStart, length});
        } else if (rangeStart >= sourceStart && rangeStart < sourceEnd) {
            // map overlaps beginning of range
            size_t length = rangeLength - diff(sourceEnd, rangeEnd);
            results.push_back({destStart + diff(rangeStart, sourceStart), length});
            mapped.push_back({rangeStart, length});
        }
    }

    // fill in ranges that weren't covered by a map
    sortRanges(mapped);
    size_t nextStart = rangeStart;
    for (auto mapped_range : mapped) {
        const auto [start, length] = mapped_range;
        if (start > nextStart) {
            results.push_back({nextStart, diff(start, nextStart)});
        }
        nextStart = start + length;
    }
    if (nextStart < rangeEnd) {
        results.push_back({nextStart, diff(nextStart, rangeEnd)});
    }

    return results;
}

int main(int argc, char** argv) {
    std::vector<std::string> input = aoc::parseInput(argv[1]);
    std::vector<size_t> seed_inputs = extractNumsFromString(input[0]);
    std::vector<Range> seed_ranges;
    for (int i = 0; i + 1 < seed_inputs.size(); i += 2) {
        seed_ranges.push_back({seed_inputs[i], seed_inputs[i + 1]});
    }

    std::vector<std::vector<Mapping>> maps;
    maps.push_back(std::vector<Mapping>());
    int curr_map = 0;
    for (int i = 3; i < input.size(); i++) {
        if (input[i].find_first_of("0123456789") == std::string::npos) {
            i++;
            curr_map++;
            maps.push_back(std::vector<Mapping>());
            continue;
        }

        std::vector<size_t> nums_for_mapping = extractNumsFromString(input[i]);
        maps[curr_map].push_back({nums_for_mapping[0], nums_for_mapping[1], nums_for_mapping[2]});
    }

    for (auto map : maps) {
        std::vector<Range> new_ranges;
        for (auto range : seed_ranges) {
            auto temp_ranges = findDestination(range, map);
            new_ranges.insert(new_ranges.end(), temp_ranges.begin(), temp_ranges.end());
        }
        seed_ranges = std::move(new_ranges);
    }

    sortRanges(seed_ranges);
    std::cout << std::get<0>(seed_ranges[0]) << std::endl;
}
