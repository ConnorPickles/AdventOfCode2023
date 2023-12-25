#include "../../shared/input.hpp"
#include <algorithm>
#include <iostream>
#include <numeric>

typedef struct {
    int row;
    int col;
} coord;

int main(int argc, char** argv) {
    std::vector<std::string> input = aoc::parseInput(argv[1]);
    std::vector<coord> galaxies;
    std::vector<int> emptyRows(input.size(), 1000000);
    std::vector<int> emptyCols(input[0].size(), 1000000);

    for (int i = 0; i < input.size(); i++) {
        for (int j = 0; j < input[0].size(); j++) {
            if (input[i][j] == '#') {
                coord galaxy;
                galaxy.row = i;
                galaxy.col = j;
                galaxies.push_back(galaxy);

                emptyRows[i] = 1;
                emptyCols[j] = 1;
            }
        }
    }

    size_t answer = 0;
    for (int i = 0; i < galaxies.size() - 1; i++) {
        for (int j = i + 1; j < galaxies.size(); j++) {
            auto minmaxRow = std::minmax(galaxies[i].row, galaxies[j].row);
            auto minmaxCol = std::minmax(galaxies[i].col, galaxies[j].col);
            answer += std::accumulate(emptyRows.begin() + minmaxRow.first, emptyRows.begin() + minmaxRow.second, 0);
            answer += std::accumulate(emptyCols.begin() + minmaxCol.first, emptyCols.begin() + minmaxCol.second, 0);
        }
    }

    std::cout << answer << std::endl;
}
