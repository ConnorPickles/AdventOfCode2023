#include "../../shared/input.hpp"
#include <iostream>
#include <tuple>

class Tile {
  public:
    int row;
    int col;

    Tile() : row(-1), col(-1){};
    Tile(int r, int c) : row(r), col(c){};

    bool initialized() const {
        return row != -1;
    }

    bool operator==(const Tile& other) const {
        return row == other.row && col == other.col;
    }

    void next(Tile& prev, char symbol) {
        Tile result;

        if (initialized() == false || prev.initialized() == false) {
            std::cout << "Trying to use un-initialized Tile" << std::endl;
            exit(1);
        }

        switch (symbol) {
        case '-':
            (col < prev.col) ? result = Tile(row, col - 1) : result = Tile(row, col + 1);
            break;
        case '|':
            (row < prev.row) ? result = Tile(row - 1, col) : result = Tile(row + 1, col);
            break;
        case 'F':
            (row != prev.row) ? result = Tile(row, col + 1) : result = Tile(row + 1, col);
            break;
        case '7':
            (row != prev.row) ? result = Tile(row, col - 1) : result = Tile(row + 1, col);
            break;
        case 'L':
            (row != prev.row) ? result = Tile(row, col + 1) : result = Tile(row - 1, col);
            break;
        case 'J':
            (row != prev.row) ? result = Tile(row, col - 1) : result = Tile(row - 1, col);
            break;
        default:
            std::cout << "Bad symbol" << std::endl;
            exit(1);
        }

        prev = *this;
        this->row = result.row;
        this->col = result.col;
    }
};

Tile findStartingTile(const std::vector<std::string>& input) {
    for (int i = 0; i < input.size(); i++) {
        for (int j = 0; j < input[0].size(); j++) {
            if (input[i][j] == 'S') {
                return Tile(i, j);
            }
        }
    }

    std::cout << "Could not find starting position" << std::endl;
    exit(1);
}

int main(int argc, char** argv) {
    std::vector<std::string> input = aoc::parseInput(argv[1]);

    Tile start = findStartingTile(input);

    // find the 2 paths leading away from the starting tile
    Tile path1;
    Tile path2;

    if (start.col - 1 >= 0) {
        switch (input[start.row][start.col - 1]) {
        case '-':
        case 'F':
        case 'L':
            path1.row = start.row;
            path1.col = start.col - 1;
            break;
        default:
            break;
        }
    }

    if (start.col + 1 < input[0].size()) {
        switch (input[start.row][start.col + 1]) {
        case '-':
        case '7':
        case 'J': {
            Tile tmp(start.row, start.col + 1);
            (path1.initialized()) ? path2 = std::move(tmp) : path1 = std::move(tmp);
            break;
        }
        default:
            break;
        }
    }

    if (start.row - 1 >= 0) {
        switch (input[start.row - 1][start.col]) {
        case '|':
        case 'F':
        case '7': {
            Tile tmp(start.row - 1, start.col);
            (path1.initialized()) ? path2 = std::move(tmp) : path1 = std::move(tmp);
            break;
        }
        default:
            break;
        }
    }

    if (start.row + 1 < input.size()) {
        switch (input[start.row + 1][start.col]) {
        case '|':
        case 'L':
        case 'J': {
            Tile tmp(start.row + 1, start.col);
            (path1.initialized()) ? path2 = std::move(tmp) : path1 = std::move(tmp);
        }
        default:
            break;
        }
    }

    // follow both of the paths until they converge
    Tile prevPath1 = start;
    Tile prevPath2 = start;
    for (int i = 1;; i++) {
        if (path1 == path2 || prevPath1 == path2) {
            std::cout << i << std::endl;
            break;
        }

        path1.next(prevPath1, input[path1.row][path1.col]);
        path2.next(prevPath2, input[path2.row][path2.col]);
    }
}
