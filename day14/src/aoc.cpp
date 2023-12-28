#include "../../shared/input.hpp"
#include "../../shared/printing.hpp"
#include <iostream>
#include <set>

using Grid = typename std::vector<std::string>;

void tilt(Grid& grid, char direction) {
    switch (direction) {
    case 'N': {
        for (int col = 0; col < grid[0].size(); col++) {
            int rollPoint = 0;
            for (int row = 0; row < grid.size(); row++) {
                if (grid[row][col] == 'O') {
                    if (rollPoint != row) {
                        grid[rollPoint][col] = 'O';
                        grid[row][col] = '.';
                    }
                    rollPoint++;
                } else if (grid[row][col] == '#') {
                    rollPoint = row + 1;
                }
            }
        }
        break;
    }

    case 'S': {
        for (int col = 0; col < grid[0].size(); col++) {
            int rollPoint = grid.size() - 1;
            for (int row = grid.size() - 1; row >= 0; row--) {
                if (grid[row][col] == 'O') {
                    if (rollPoint != row) {
                        grid[rollPoint][col] = 'O';
                        grid[row][col] = '.';
                    }
                    rollPoint--;
                } else if (grid[row][col] == '#') {
                    rollPoint = row - 1;
                }
            }
        }
        break;
    }

    case 'W': {
        for (int row = 0; row < grid.size(); row++) {
            int rollPoint = 0;
            for (int col = 0; col < grid[0].size(); col++) {
                if (grid[row][col] == 'O') {
                    if (rollPoint != col) {
                        grid[row][rollPoint] = 'O';
                        grid[row][col] = '.';
                    }
                    rollPoint++;
                } else if (grid[row][col] == '#') {
                    rollPoint = col + 1;
                }
            }
        }
        break;
    }

    case 'E': {
        for (int row = 0; row < grid.size(); row++) {
            int rollPoint = grid[0].size() - 1;
            for (int col = grid[0].size() - 1; col >= 0; col--) {
                if (grid[row][col] == 'O') {
                    if (rollPoint != col) {
                        grid[row][rollPoint] = 'O';
                        grid[row][col] = '.';
                    }
                    rollPoint--;
                } else if (grid[row][col] == '#') {
                    rollPoint = col - 1;
                }
            }
        }
        break;
    }

    default:
        std::cout << "Can't tilt in direction: " << direction << std::endl;
        exit(1);
    }
}

size_t load(const Grid& grid) {
    size_t load = 0;
    for (int row = 0; row < grid.size(); row++) {
        for (int col = 0; col < grid[0].size(); col++) {
            if (grid[row][col] == 'O') {
                load += grid.size() - row;
            }
        }
    }
    return load;
}

void cycle(Grid& grid) {
    tilt(grid, 'N');
    tilt(grid, 'W');
    tilt(grid, 'S');
    tilt(grid, 'E');
}

size_t duplicateLayout(const std::vector<Grid>& layouts, const Grid& grid) {
    for (size_t i = 0; i < layouts.size(); i++) {
        if (layouts[i] == grid) {
            return i;
        }
    }

    return -1;
}

int main(int argc, char** argv) {
    Grid grid = aoc::parseInput(argv[1]);
    std::vector<Grid> layouts;

    size_t currCycle = 0;
    while (duplicateLayout(layouts, grid) == -1) {
        layouts.push_back(grid);
        cycle(grid);
        currCycle++;
    }

    size_t cycleBegin = duplicateLayout(layouts, grid);
    size_t cycleLength = currCycle - cycleBegin;
    size_t cyclesRemaining = (1000000000 - currCycle) / cycleLength;
    currCycle += cyclesRemaining * cycleLength;
    for (size_t i = currCycle; i < 1000000000; i++) {
        cycle(grid);
    }

    std::cout << load(grid) << std::endl;

}
